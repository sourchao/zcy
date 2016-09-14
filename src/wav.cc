#include "wav.h"

/* WavHeader Implement */

WavHeader::WavHeader()
{
    _dChunkID       = 0x46464952;            /* "RIFF" */
    _dChunkSize     = 0;                     /* total file length - 8 */
    _dFormat        = 0x45564157;            /* "WAVE" */
    _dSubChunk1ID   = 0x20746d66;            /* "fmt " */
    _dSubChunk1Size = 0x10;                  /* size of fmt chunk */
    _wAudioFormat   = 0x01;                  /* format type */
    _wNumChannels   = 0;                     /* number of channels (i.e. mono, stereo...) */
    _dSampleRate    = 0;                     /* sample rate */
    _dByteRate      = 0;                     /* for buffer estimation */
    _wBlockAlign    = 0;                     /* block size of data */
    _wBitsPerSample = 0;                     /* number of bits per sample of mono data */
    _dDataTag       = 0x61746164;
    _dDataLen       = 0;
}

int WavHeader::Size()
{
    return ((char *)&_dDataLen - (char *)&_dChunkID + sizeof(_dDataLen));
}

void * WavHeader::Data()
{
    return (void *)&_dChunkID;
}

void WavHeader::SetSampleFrequency(int sampleFrequency)
{
    _dSampleRate = sampleFrequency;
}
void WavHeader::SetChannelNum(int channelNum)
{
    _wNumChannels = channelNum;
}
void WavHeader::SetSampleWidth(int sampleWidth)
{
    _wBitsPerSample = sampleWidth * 8;
}

void WavHeader::SetDataLength(unsigned int dataLen)
{
    _dDataLen = dataLen;
}

void WavHeader::Update()
{
    _dByteRate      = _dSampleRate * _wBitsPerSample / 8;
    _wBlockAlign    = _wNumChannels * _wBitsPerSample / 8;
    _dChunkSize     += _dDataLen + Size() - 8;
}
