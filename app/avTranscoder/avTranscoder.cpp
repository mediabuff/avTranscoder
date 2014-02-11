#include <iostream>
#include <iomanip>

#include <AvTranscoder/InputStream.hpp>
#include <AvTranscoder/InputStreamAudio.hpp>
#include <AvTranscoder/InputStreamVideo.hpp>
#include <AvTranscoder/OutputStream.hpp>
#include <AvTranscoder/OutputStreamAudio.hpp>
#include <AvTranscoder/OutputStreamVideo.hpp>
#include <AvTranscoder/InputFile.hpp>
#include <AvTranscoder/OutputFile.hpp>

#include <AvTranscoder/ColorTransform.hpp>

#include <AvTranscoder/DatasStructures/VideoDesc.hpp>
#include <AvTranscoder/DatasStructures/Image.hpp>

void transcodeVideo( const char* inputfilename, const char* outputFilename )
{
	using namespace avtranscoder;

	InputFile input;
	input.setup( inputfilename ).analyse();

	// init video decoders
	InputStreamVideo inputStreamVideo; // take the first video stream per default

	av_log_set_level( AV_LOG_FATAL );
	av_log_set_level( AV_LOG_DEBUG );
	

	inputStreamVideo.setup( inputfilename, 0 );

	//dVideo.set( key, value );

	// same as
	//DecoderVideo dVideo( "inputFilename.mov", 1 );

	// init audio decoders
	InputStreamAudio isAudioLeft ( "inputFilename.wav" ); // take the first audio stream per default
	InputStreamAudio isAudioRight( "inputFilename.wav", 2 );

	// init video & audio encoders
	OutputStreamVideo outputStreamVideo;

	VideoDesc& videoDesc = outputStreamVideo.getVideoDesc();

	Pixel oPixel;

	oPixel.setSubsampling( eSubsampling422 );
	oPixel.setBitsPerPixel( 16 );

	ImageDesc imageDesc;
	imageDesc.setWidth ( input.getProperties().videoStreams.at(0).width );
	imageDesc.setHeight( input.getProperties().videoStreams.at(0).height );
	imageDesc.setPixel ( oPixel );

	Image sourceImage( imageDesc );

	videoDesc.setVideoCodec( "mpeg2video" );
	videoDesc.set( "b",50000000 );
	try
	{
		videoDesc.set( "unknownParameter", 120000000 );
	}
	catch( const std::exception& e )
	{
		std::cout << "[ERROR] " << e.what() << std::endl;
	}

	videoDesc.setTimeBase( 1, 25 ); // 25 fps

	videoDesc.setImageParameters( sourceImage );

	//videoDesc.initCodecContext();

	if( !outputStreamVideo.setup( ) )
	{
		std::cout << "error during initialising video output stream" << std::endl;
		exit( -1 );
	}

	DataStreamDesc dataStreamDesc;

	Image imageToEncode( sourceImage );
	DataStream codedImage( dataStreamDesc );


	OutputStreamAudio osAudioLeft ( );  // "AudioStreamEncoder" / "AudioOutputStream" ?
	OutputStreamAudio osAudioRight( );
	OutputStreamAudio osAudioLfe  ( );

	// setup wrapper
	OutputFile of( outputFilename );  // "Format" ? to keep libav naming


	if( ! of.setup( ) )
	{
		std::cout << "error during setup output file" << std::endl;
		exit( -1 );
	}

	InputFile inputFile;
	inputFile.setup( inputfilename );

	of.addVideoStream( inputFile.getVideoDesc( 0 ) );
	/*of.addAudioStream();
	of.addAudioStream();
	of.addAudioStream();
	of.addAudioStream();*/


	/*eVideoLeft  = wrapper.createVideoEncoder( );
	eVideoRight = wrapper.createVideoEncoder( dVideo );

	eAudioLeft = wrapper.createAudioEncoder( 2 );


	wrapper.createAudioEncoder( eAudioLeft, 2 );*/

	ColorTransform ct;


	// Encodage/transcodage
	std::cout << "start transcoding" << std::endl;

	size_t frame = 0;

	while( inputStreamVideo.readNextFrame( sourceImage ) )
	{
		std::cout << "\rprocess frame " << frame << std::flush;

		ct.convert( sourceImage, imageToEncode );

		outputStreamVideo.encodeFrame( imageToEncode, codedImage );

		of.wrap( codedImage, 0 );

		++frame;
	}
	std::cout << std::endl;
}

int main( int argc, char** argv )
{
	if( argc != 2 )
	{
		std::cout << "av++ require a media filename" << std::endl;
		return( -1 );
	}

	std::cout << "start ..." << std::endl;

	// example of video Transcoding
	transcodeVideo( argv[1], "transcodedVideo.mxf" );

	std::cout << "end ..." << std::endl;
}
