#ifndef _WAV_H_
#define _WAV_H_

#include <vector>

template <class DataType>
class WavStream {

private:
    vector<DataType> _wavData;
};

template <class DataType>
class WavReader {
public:
    void SetStream(WavStream<DataType> *wavStream);
private:
    WavStream<DataType> * _pWavStream;
};

template <class DataType>
class WavWriter {
public:

private:
    WavStream<DataType> * _pWavStream;
};
#endif
