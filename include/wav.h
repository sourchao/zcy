#ifndef _WAV_H_
#define _WAV_H_

#include <vector>
#include <unistd.h>
#include <string.h>

template <class DataType>
class WavStream {
    int Read(DataType * chunk, int chunk_size, int index, int len);
    int Size() { return _wavData.size(); }
    
private:
    vector<DataType> _wavData;
};

template <class DataType>
class WavReader {
public:
    void SetStream(WavStream<DataType> *wavStream);
    void SetSource(int fd);
    int  Read(DataType * chunk, int chunk_size);
    
private:
    int checkUnreadData() { return (_pWavStream->Size() - _cur_pos); }
    WavStream<DataType> * _pWavStream;
    int _fileno;
    int _cur_pos;
    bool _isReading;
};

template <class DataType>
class WavWriter {
public:

private:
    WavStream<DataType> * _pWavStream;
};
#endif
