#ifndef _WAV_H_
#define _WAV_H_

#include <vector>
#include <types.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <string.h>
#include <iostream>

using namespace std;

#define WAV_BYTE_BUFFER_SIZE 2048

template <class DataType>
class WavStream {
public:
    int Write(DataType * chunk, int len);
    int Read(DataType * chunk, int index, int len);
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
    
private:
    int checkUnreadData();
    void readingThread(const void * endFlag, int endFlagSize);
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

#include "wav-inl.h"

#endif
