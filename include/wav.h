#ifndef _WAV_H_
#define _WAV_H_

#include <vector>
#include <pthread.h>
#include <types.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define WAV_BUFFER_SIZE 2048

template <class DataType>
class WavStream {
public:
    int Write();
    int Read(DataType * chunk, int chunk_size, int index, int len);
private:
    vector<DataType> _wavData;
};

template <class DataType>
class WavReader {
public:
    WavReader();
    void SetStream(WavStream<DataType> *wavStream);
    void SetSource(int fileno);
    int  Read(DataType * chunk, int chunk_size);
    bool BeginRead();
    
private:
    int checkUnreadData();
    void * readingThread(void * param);
    WavStream<DataType> * _pWavStream;
    int _fileno;
    bool _isReading;
    int _cur_pos;
};

template <class DataType>
class WavWriter {
public:

private:
    WavStream<DataType> * _pWavStream;
};
#endif
