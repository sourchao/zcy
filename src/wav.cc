#include "wav.h"

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
