#include "AudioReader.hpp"

#include <AvTranscoder/decoder/AudioDecoder.hpp>
#include <AvTranscoder/data/AudioFrame.hpp>
#include <AvTranscoder/transform/AudioTransform.hpp>
#include <AvTranscoder/progress/NoDisplayProgress.hpp>

namespace avtranscoder
{

AudioReader::AudioReader(const std::string& filename, const size_t streamIndex, const int channelIndex)
    : IReader(filename, streamIndex, channelIndex)
    , _audioStreamProperties(NULL)
    , _outputSampleRate(0)
    , _outputNbChannels(0)
    , _outputSampleFormat(AV_SAMPLE_FMT_S16)
{
    init();
}

AudioReader::AudioReader(InputFile& inputFile, const size_t streamIndex, const int channelIndex)
    : IReader(inputFile, streamIndex, channelIndex)
    , _audioStreamProperties(NULL)
    , _outputSampleRate(0)
    , _outputNbChannels(0)
    , _outputSampleFormat(AV_SAMPLE_FMT_S16)
{
    init();
}

void AudioReader::init()
{
    // analyse InputFile
    avtranscoder::NoDisplayProgress p;
    _inputFile->analyse(p);
    _streamProperties = &_inputFile->getProperties().getStreamPropertiesWithIndex(_streamIndex);
    _audioStreamProperties = static_cast<const AudioProperties*>(_streamProperties);
    _inputFile->activateStream(_streamIndex);

    // setup decoder
    _decoder = new AudioDecoder(_inputFile->getStream(_streamIndex));
    _decoder->setupDecoder();

    // create transform
    _transform = new AudioTransform();

    // create src frame
    _srcFrame = new AudioFrame(_inputFile->getStream(_streamIndex).getAudioCodec().getAudioFrameDesc());
    AudioFrame* srcFrame = static_cast<AudioFrame*>(_srcFrame);
    // create dst frame
    _outputSampleRate = srcFrame->desc().getSampleRate();
    _outputNbChannels = (_channelIndex == -1) ? srcFrame->desc().getNbChannels() : 1;
    _dstFrame = new AudioFrame(AudioFrameDesc(_outputSampleRate, _outputNbChannels, _outputSampleFormat));
}

AudioReader::~AudioReader()
{
    delete _decoder;
    delete _srcFrame;
    delete _dstFrame;
    delete _transform;
}

void AudioReader::updateOutput(const size_t sampleRate, const size_t nbChannels, const std::string& sampleFormat)
{
    _outputSampleRate = sampleRate;
    _outputNbChannels = nbChannels;
    _outputSampleFormat = av_get_sample_fmt(sampleFormat.c_str());
    // update dst frame
    delete _dstFrame;
    _dstFrame = new AudioFrame(AudioFrameDesc(_outputSampleRate, _outputNbChannels, _outputSampleFormat));
}
}
