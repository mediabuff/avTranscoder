#include "StreamProperties.hpp"

#include <stdexcept>

namespace avtranscoder
{

StreamProperties::StreamProperties( const FormatContext& formatContext, const size_t index )
	: _formatContext( &formatContext.getAVFormatContext() )
	, _codecContext( NULL )
	, _codec( NULL )
	, _streamIndex( index )
{
	if( _formatContext )
		detail::fillMetadataDictionnary( _formatContext->streams[index]->metadata, _metadatas );

	if( _formatContext )
	{
		if( _streamIndex > _formatContext->nb_streams )
		{
			std::stringstream ss;
			ss << "Stream at index " << _streamIndex << " does not exist.";
			throw std::runtime_error( ss.str() );
		}
		_codecContext = _formatContext->streams[_streamIndex]->codec;
	}

	if( _formatContext && _codecContext )
		_codec = avcodec_find_decoder( _codecContext->codec_id );
}

StreamProperties::~StreamProperties()
{
	
}

size_t StreamProperties::getStreamId() const
{
	if( ! _formatContext )
		throw std::runtime_error( "unknown format context" );
	return _formatContext->streams[_streamIndex]->id;
}

Rational StreamProperties::getTimeBase() const
{
	if( ! _formatContext )
		throw std::runtime_error( "unknown format context" );

	Rational timeBase = {
		_formatContext->streams[_streamIndex]->time_base.num,
		_formatContext->streams[_streamIndex]->time_base.den,
	};
	return timeBase;
}

float StreamProperties::getDuration() const
{
	Rational timeBase = getTimeBase();
	return ( timeBase.num / (float) timeBase.den ) * _formatContext->streams[_streamIndex]->duration;
}

AVMediaType StreamProperties::getStreamType() const
{
	if( ! _formatContext )
		throw std::runtime_error( "unknown format context" );
	return _formatContext->streams[_streamIndex]->codec->codec_type;
}

size_t StreamProperties::getCodecId() const
{
	if( ! _codecContext )
		throw std::runtime_error( "unknown codec context" );
	return _codecContext->codec_id;
}

std::string StreamProperties::getCodecName() const
{
	if( ! _codecContext || ! _codec )
		throw std::runtime_error( "unknown codec" );

	if( _codec->capabilities & CODEC_CAP_TRUNCATED )
		_codecContext->flags |= CODEC_FLAG_TRUNCATED;

	if( ! _codec->name )
		throw std::runtime_error( "unknown codec name" );

	return std::string( _codec->name );
}

std::string StreamProperties::getCodecLongName() const
{
	if( ! _codecContext || ! _codec )
		throw std::runtime_error( "unknown codec" );

	if( _codec->capabilities & CODEC_CAP_TRUNCATED )
		_codecContext->flags |= CODEC_FLAG_TRUNCATED;

	if( ! _codec->long_name )
		throw std::runtime_error( "unknown codec long name" );

	return std::string( _codec->long_name );
}

PropertyVector StreamProperties::getPropertiesAsVector() const
{
	PropertyVector data;

	addProperty( data, "streamId", &StreamProperties::getStreamId );
	addProperty( data, "streamIndex", &StreamProperties::getStreamIndex );
	addProperty( data, "timeBase", &StreamProperties::getTimeBase );
	addProperty( data, "duration", &StreamProperties::getDuration );
	addProperty( data, "codecId", &StreamProperties::getCodecId );
	addProperty( data, "codecName", &StreamProperties::getCodecName );
	addProperty( data, "codecLongName", &StreamProperties::getCodecLongName );

	for( size_t metadataIndex = 0; metadataIndex < _metadatas.size(); ++metadataIndex )
	{
		detail::add( data, _metadatas.at( metadataIndex ).first, _metadatas.at( metadataIndex ).second );
	}

	return data;
}

PropertyMap StreamProperties::getPropertiesAsMap() const
{
	PropertyMap dataMap;

	PropertyVector dataVector( getPropertiesAsVector() );
	for( PropertyVector::const_iterator it = dataVector.begin();
			it != dataVector.end();
			++it )
	{
		dataMap.insert( std::make_pair( it->first, it->second ) );
	}

	return dataMap;
}

}
