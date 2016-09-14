#include "sr_mgr.h"

SRManager::SRManager()
{
    _szLoginParams   = "appid = 57c3f80f, work_dir = .";
    _szSessionParams = "sub = iat, domain = iat, language = zh_ch, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8";
    _session_id = NULL;
    _sr_result = "";
    _errcode = MSP_SUCCESS;
    _aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
    _ep_stat = MSP_EP_LOOKING_FOR_SPEECH;
    _rec_stat = MSP_REC_STATUS_SUCCESS;
    _send_cnt = 0;
    _is_session_valid = false;
}

bool SRManager::Login(const char * username, const char * password)
{
    int ret = MSP_SUCCESS;
    ret = MSPLogin(username, password, _szLoginParams);
    if (ret != MSP_SUCCESS)
        return false;
    return true;
}

void SRManager::Logout()
{
    MSPLogout();
}

bool SRManager::BeginSession()
{
    _session_id = QISRSessionBegin(NULL, _szSessionParams, &_errcode);
    
	if (MSP_SUCCESS != _errcode)
	    return false;

    _is_session_valid = true;
	return true;
}

void SRManager::EndSession()
{
    char hints[100] = { 0 };
    QISRSessionEnd(_session_id, hints);
    _is_session_valid = false;
}

bool SRManager::SendData(Byte * buff, int len)
{
    if (len <= 0)
        return true;

    if (_send_cnt == 0)
        _aud_stat = MSP_AUDIO_SAMPLE_FIRST;
    else
        _aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
        
    _errcode = QISRAudioWrite(_session_id, (const void *)buff, len, _aud_stat, &_ep_stat, &_rec_stat);
    
    if (MSP_SUCCESS != _errcode)
        return false;
        
    _send_cnt++;

    if (MSP_EP_AFTER_SPEECH == _ep_stat) {
#ifdef DEBUG
        cout << "    DEBUG: Current speech is ended by Iflytek server." << endl;
#endif
        _is_session_valid = false;
    }

    return true;
}

bool SRManager::SendFinish()
{
    _errcode = QISRAudioWrite(_session_id, NULL, 0, MSP_AUDIO_SAMPLE_LAST, &_ep_stat, &_rec_stat);
    if (MSP_SUCCESS != _errcode)
        return false;
    return true;
}

string SRManager::AskResult()
{
    string cur_result = "";
    if (MSP_REC_STATUS_SUCCESS == _rec_stat) 
    {
        const char *result = QISRGetResult(_session_id, &_rec_stat, 0, &_errcode);
        if (MSP_SUCCESS != _errcode)
            return "err";

        if (NULL != result) {
            cur_result += result;
#ifdef DEBUG
            cout << "    DEBUG: SR Result [ " << result << " ]" << endl;
#endif		    
        }
    }
    _sr_result += cur_result;
    return cur_result;
}

string SRManager::WaitAllResults()
{
    string cur_result = "";
    while (MSP_REC_STATUS_COMPLETE != _rec_stat) 
	{
		const char * result = QISRGetResult(_session_id, &_rec_stat, 0, &_errcode);
		if (MSP_SUCCESS != _errcode)
		    return "err";
		    
		if (NULL != result) {
		    cur_result += result;
#ifdef DEBUG
            cout << "    DEBUG: SR Result [ " << result << " ]" << endl;
#endif		    
        }
		usleep(100 * 1000);
    }
#ifdef DEBUG
    cout << "    DEBUG: All SR results has been received." << endl;
#endif
    _sr_result += cur_result;
    return cur_result;
}

string SRManager::GetResult()
{
    return _sr_result;
}

bool SRManager::IsSessionValid()
{
    return _is_session_valid;
}
