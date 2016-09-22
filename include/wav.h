#ifndef _WAV_H_
#define _WAV_H_

#include <vector>
#include <thread>
#include "types.h"
#include "common.h"

#define WAV_BYTE_BUFFER_SIZE 2048

template <class DataType>
class WavStream {
public:
    int Write(DataType * chunk, int len);
    int Read(DataType * chunk, unsigned int index, int len);
    DataType * ReadAll();
    int Size() { return _wavData.size(); }
private:
    vector<DataType> _wavData;
};

template <class DataType>
class WavReader {
public:
    WavReader();
    void SetStream(WavStream<DataType> *wavStream);
    void SetSource(int fileno);
    int  Read(DataType * chunk, int len);
    bool BeginRead();
    void EndRead();    
    int GetWavDataLength();
private:
    int checkUnreadData();
    void readingThread(const void * endFlag, int endFlagSize);
    WavStream<DataType> * _pWavStream;
    int _fileno;
    bool _isReading;
    unsigned int _cur_pos;
};

class WavHeader {
public:
    WavHeader();
    void SetSampleFrequency(int sampleFrequency);
    void SetChannelNum(int channelNum);
    void SetSampleWidth(int sampleWidth);
    void SetDataLength(unsigned int dataLen);
    void Update();
    int Size();
    void * Data();

private:
    DWord       _dChunkID;                /* "RIFF" */
    DWord       _dChunkSize;              /* total file length - 8 */
    DWord       _dFormat;                 /* "WAVE" */
    DWord       _dSubChunk1ID;            /* "fmt" */
    DWord       _dSubChunk1Size;          /* size of fmt chunk */
    Word        _wAudioFormat;            /* format type */
    Word        _wNumChannels;            /* number of channels (i.e. mono, stereo...) */
    DWord       _dSampleRate;             /* sample rate */
    DWord       _dByteRate;               /* for buffer estimation */
    Word        _wBlockAlign;             /* block size of data */
    Word        _wBitsPerSample;          /* number of bits per sample of mono data */
    DWord       _dDataTag;
    DWord       _dDataLen;
};

template <class DataType>
class WavWriter {
public:
    WavWriter();
    bool SetFilePath(string wavFilePath);
    void SetStream(WavStream<DataType> * wavStream);
    void SetSampleFrequency(int sampleFrequency);
    void SetChannelNum(int channelNum);
    void SetSampleWidth(int sampleWidth);
    void WriteHeader();
    void Write();
    void WriteAll();
    void Close();
private:
    WavHeader _wavHeader;
    WavStream<DataType> * _pWavStream;
    fstream _wavFile;
    //int _wavDataLength;
};

#include "wav-inl.h"

#endif
