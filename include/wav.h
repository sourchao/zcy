#ifndef _WAV_H_
#define _WAV_H_

#include <vector>

template <class DataType>
class WavStream {

private:
    vector<DataType> _wavData;
};

class WavWriter {
public:

private:
    WavStream * _pWavStream;
};
#endif
