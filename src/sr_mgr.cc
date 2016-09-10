#include "sr_mgr.h"

SRManager::SRManager()
{
    _szSessionParams = "sub = iat, domain = iat, language = zh_ch, accent = mandarin, sample_rate = 16000, result_type = plain, result_encoding = utf8";
}

bool SRManager::Login(string username, string password)
{
    int ret = MSP_SUCCESS;
    ret = MSPLogin(username, password, _szSessionParams);
    if (ret != MSP_SUCCESS)
        return false;
    return true;
}
