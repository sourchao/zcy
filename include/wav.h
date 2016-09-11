#ifndef _WAV_H_
#define _WAV_H_

#include <vector>
#include <pthread.h>
#include <types.h>

#define BUFFER_SIZE 2048

template <class DataType>
class WavStream {
public:
    r Write();
private:
    vector<DataType> _wavData;
};

template <class DataType>
class WavReader {
public:
    void SetStream(WavStream<DataType> *wavStream);
    bool BeginRead();
private:
    void * readingThread(void * param);
    WavStream<DataType> * _pWavStream;
    int _fileno;
    bool _isReading;
};

template <class DataType>
class WavWriter {
public:

private:
    WavStream<DataType> * _pWavStream;
};
#endif
