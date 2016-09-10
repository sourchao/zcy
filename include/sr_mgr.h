#ifndef _SR_MGR_H_
#define _SR_MGR_H_

#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"

class SRManager {
public:
    SRManager();
    bool Login(string username, string password);
private:
    string _szSessionParams;
};

#endif
