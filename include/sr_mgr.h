#ifndef _SR_MGR_H_
#define _SR_MGR_H_

#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "types.h"
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

class SRManager {
public:
    SRManager();
    bool Login(const char * username, const char * password);
    void Logout();
    bool BeginSession();
    void EndSession();
    int GetLastErrcode() { return _errcode; }
    bool SendData(Byte * buff, int len);
    bool SendFinish();
    string AskResult();
    string WaitAllResults();
    string GetResult();
private:
    const char *  _szLoginParams;
    const char *  _szSessionParams;
    const char *  _session_id;
    string _sr_result;
    int _errcode;
    int _aud_stat;
    int _ep_stat;
    int _rec_stat;
    int _send_cnt;
};

#endif
