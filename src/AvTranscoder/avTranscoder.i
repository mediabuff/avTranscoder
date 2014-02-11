%module AvTranscoder

%include "std_string.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"

%{
#include <AvTranscoder/common.hpp>

#include <AvTranscoder/DatasStructures/Pixel.hpp>
#include <AvTranscoder/DatasStructures/Image.hpp>
#include <AvTranscoder/DatasStructures/VideoDesc.hpp>
#include <AvTranscoder/DatasStructures/DataStreamDesc.hpp>

#include <AvTranscoder/OutputFile.hpp>

#include <AvTranscoder/OutputStream.hpp>
#include <AvTranscoder/OutputStreamAudio.hpp>
#include <AvTranscoder/OutputStreamVideo.hpp>

#include <AvTranscoder/InputStream.hpp>
#include <AvTranscoder/InputStreamAudio.hpp>
#include <AvTranscoder/InputStreamVideo.hpp>
%}

namespace std {
%template(VideoVector)     vector< avtranscoder::VideoProperties >;
%template(AudioVector)     vector< avtranscoder::AudioProperties >;
%template(MetadataPair)    pair< string, string >;
%template(MetadatasVector) vector< pair< string, string > >;
%template(GopPair)         pair< char, bool >;
%template(GopVector)       vector< pair< char, bool > >;
%template(ChannelVector)   vector< avtranscoder::Channel >;
}

%include <AvTranscoder/common.hpp>

%include <AvTranscoder/DatasStructures/Pixel.hpp>
%include <AvTranscoder/DatasStructures/Image.hpp>
%include <AvTranscoder/DatasStructures/VideoDesc.hpp>
%include <AvTranscoder/DatasStructures/DataStreamDesc.hpp>

%include <AvTranscoder/OutputFile.hpp>

%include <AvTranscoder/OutputStream.hpp>
%include <AvTranscoder/OutputStreamAudio.hpp>
%include <AvTranscoder/OutputStreamVideo.hpp>

%include <AvTranscoder/InputStream.hpp>
%include <AvTranscoder/InputStreamAudio.hpp>
%include <AvTranscoder/InputStreamVideo.hpp>