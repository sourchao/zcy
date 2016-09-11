#ifndef _SR_MGR_H_
#define _SR_MGR_H_

#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"
#include "types.h"

class SRManager {
public:
    SRManager();
    bool Login(string username, string password);
    bool BeginSession(string gramma);
    int GetLastErrcode() { return _errcode; }
    bool SendData(Byte * buff, int len);
    bool SendFinish();
    string GetResult();
private:
    string _szLoginParams;
    string _szSessionParams;
    string _session_id;
    string _sr_result;
    int _errcode;
    int _aud_stat;
    int _ep_stat;
    int _rec_stat;
    int _send_cnt;
};

#endif
