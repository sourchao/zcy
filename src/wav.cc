#include "wav.h"

/* WavStream Implement */

template <class DataType>
int WavStream<DataType>::Read(DataType * chunk, int chunk_size, int index, int len)
{
    DataType * pData = _wavData.data();
    pData += index;
    len = len <= chunk_size ? len : chunk_size;
    memcpy(chunk, pData, len * sizeof(DataType));
    return len;
}

/* WavReader Implement */

template <class DataType>
WavReader<DataType>::WavReader()
: _pWavStream(NULL), _fileno(-1), _cur_pos(0), _isReading(false)
{
}

template <class DataType>
void WavReader<DataType>::SetStream(WavStream<DataType> *wavStream)
{
    _pWavStream = wavStream;
}

template <class DataType>
void WavReader<DataType>::SetSource(int fileno)
{
    _fileno = fileno;
}

template <class DataType>
int  WavReader<DataType>::Read(DataType * chunk, int chunk_size)
{
    if (!_isReading && checkUnreadData() == 0)
        return 0;
        
    int read_size = 0;
    int unread_data = 0;
    while (true) {
        unread_data = checkUnreadData();
        if (_isReading && unread_data < chunk_size)
        {
            usleep(100 * 1000);
            continue;
        }
        
        read_size = chunk_size <= unread_data ? chunk_size : unread_data;
        
        _pWavStream->Read(chunk, chunk_size, _cur_pos, read_size);
        _cur_pos += read_size;
        break;
    }
    
    return read_size;
}

template <class DataType>
int  WavReader<DataType>::checkUnreadData()
{
    return (
}

/* WavWriter Implement */
