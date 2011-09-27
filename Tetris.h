#pragma once

#include "resource.h"

class CTetrisApp : public CWinApp
{
public:
    CTetrisApp();
    ~CTetrisApp();

public:
    virtual BOOL InitInstance();

    MCIDEVICEID m_se_apprID;
    MCIDEVICEID m_se_turnID;
    MCIDEVICEID m_se_dsprID;
    MCIDEVICEID m_se_dsblID;
    MCIDEVICEID m_se_crsrID;
    MCIDEVICEID m_se_slctID;
    MCIDEVICEID m_me_gmvrID;
};

extern CTetrisApp theApp;