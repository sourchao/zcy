/* WavStream Implement */

template <class DataType>
int WavStream<DataType>::Write(DataType * chunk, int len)
{
    if (chunk == NULL || len < 0)
        return -1;

    for (int i = 0; i < len; i++) {
        _wavData.push_back(chunk[i]);
    }
    return 0; 
}

template <class DataType>
int WavStream<DataType>::Read(DataType * chunk, int index, int len)
{
    if (_wavData.size() <= index || chunk == NULL || len < 0)
        return 0;

    DataType * pData = _wavData.data();
    pData += index;
    int readableDataCnt = _wavData.size() - index;
    len = len <= readableDataCnt ? len : readableDataCnt;
    memcpy(chunk, pData, len * sizeof(DataType));
    return len;
}

template <class DataType>
DataType * WavStream<DataType>::ReadAll()
{
    return _wavData.data();
}
/* WavReader Implement */

template <class DataType>
WavReader<DataType>::WavReader()
{
    _pWavStream = NULL;
    _fileno     = -1;
    _cur_pos    = 0;
    _isReading  = false;
}

template <class DataType>
bool WavReader<DataType>::BeginRead()
{
    if(_fileno == -1 || _pWavStream == NULL)
        return false;

    const char * endFlag = "stopSend";
    thread t(&WavReader<DataType>::readingThread, this, (const void *)endFlag, sizeof(endFlag));
    t.detach();    
    // 线程启动需要时间，虽然不需要这么大的休眠时间
    usleep(100 * 1000);

    return true;
}

template <class DataType>
void WavReader<DataType>::readingThread(const void * endFlag, int endFlagSize)
{
    _isReading = true;
    
    int len, type_size = sizeof(DataType);
    DataType buff[WAV_BYTE_BUFFER_SIZE / type_size];
    while (_isReading) {
        len = read(_fileno, buff, sizeof(buff));
        if (len == -1) {
#ifdef DEBUG
            cout << "    DEBUG: In BeginRead readingThread, read error occured." << endl;
#endif
            _isReading = false;
            return;
        }
            
        int index = len - endFlagSize;
        if (memcmp(endFlag, (Byte *)buff + index, endFlagSize) == 0) {
            len -= endFlagSize;
            _isReading = false;
        }
        
        // if DataType not char or Byte, could be buggy.
        _pWavStream->Write(buff, len / type_size);
    }
#ifdef DEBUG
    cout << "    DEBUG: WavReader stop reading from source." << endl;
#endif
}

template <class DataType>
void WavReader<DataType>::EndRead()
{
    _isReading = false;
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
    int unread_data_size = 0;
    while (true) {
        unread_data_size = checkUnreadData();
        if (_isReading && unread_data_size < chunk_size)
        {
            usleep(100 * 1000);
            continue;
        }
        
        read_size = chunk_size <= unread_data_size ? chunk_size : unread_data_size;
        
        _pWavStream->Read(chunk, _cur_pos, read_size);
        _cur_pos += read_size;
        break;
    }
    
    return read_size;
}

template <class DataType>
int  WavReader<DataType>::checkUnreadData()
{
    return (_pWavStream->Size() - _cur_pos);
}

/* WavWriter Implement */
