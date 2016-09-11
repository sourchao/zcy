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
bool WavReader<DataType>::BeginRead()
{
    if(_fileno == -1 || _pWavStream == NULL)
        return false;
    pthread_t thd;
    int ret;
    ret = pthread_create(&thd, NULL, readingThread, NULL);
    if (ret)
        return false;
        
    ret = pthread_detach(thd);
    if (ret)
        return false;
    return true;
}

template <class DataType>
void * WavReader<DataType>::readingThread(void * param)
{
    _isReading = true;
    
    int len, type_size = sizeof(DataType);
    Byte buff[BUFFER_SIZE];
    while (true) {
        len = read(_fileno, buff, sizeof(buff));
        if (len == -1)
            return false;
            
        int index = len - 4;
        if (*(int *)(buff + index) == -1) {
            len -= 4;
            _isReading = false;
        }
            
        for (int i = 0; i < len; i += sizeof(DataType))
            _pWavStream.push_back(*(DataType *)(buff + i));
            
        if (!_isReading)
            break;
    }
    return true;
}
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
