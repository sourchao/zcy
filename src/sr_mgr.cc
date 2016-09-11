#include "sr_mgr.h"

SRManager::SRManager()
{
    _szLoginParams   = "appid = 57c3f80f, work_dir = .";
    _szSessionParams = "sub = iat, domain = iat, language = zh_ch, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8";
    _session_id = "";
    _sr_result = "";
    _errcode = MSP_SUCCESS;
    _aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
    _ep_stat = MSP_EP_LOOKING_FOR_SPEECH;
    _rec_stat = MSP_REC_STATUS_SUCCESS;
    _send_cnt = 0;
}

bool SRManager::Login(const char * username, const char * password)
{
    int ret = MSP_SUCCESS;
    ret = MSPLogin(username, password, _szLoginParams.c_str());
    if (ret != MSP_SUCCESS)
        return false;
    return true;
}

bool SRManager::BeginSession(string gramma)
{
    _session_id = QISRSessionBegin(gramma.c_str(), _szSessionParams.c_str(), &_errcode);
    
	if (MSP_SUCCESS != _errcode)
	    return false;
	return true;
}

bool SRManager::SendData(Byte * buff, int len)
{
    if (_send_cnt == 0)
        _aud_stat = MSP_AUDIO_SAMPLE_FIRST;
    else
        _aud_stat = MSP_AUDIO_SAMPLE_CONTINUE;
        
    _errcode = QISRAudioWrite(_session_id.c_str(), (const void *)buff, len, _aud_stat, &_ep_stat, &_rec_stat);
    
    if (MSP_SUCCESS != _errcode)
        return false;
        
    _send_cnt++;
    return true;
}

bool SRManager::SendFinish()
{
    _errcode = QISRAudioWrite(_session_id.c_str(), NULL, 0, MSP_AUDIO_SAMPLE_LAST, &_ep_stat, &_rec_stat);
    if (MSP_SUCCESS != _errcode)
        return false;
    return true;
}

string SRManager::GetResult()
{
    while (MSP_REC_STATUS_COMPLETE != _rec_stat) 
	{
		const char * result = QISRGetResult(_session_id.c_str(), &_rec_stat, 0, &_errcode);
		if (MSP_SUCCESS != _errcode)
		    return NULL;
		    
		if (NULL != result)
		    _sr_result += result;
		    
		usleep(100 * 1000);
    }
    return _sr_result;
}
