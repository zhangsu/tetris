// Tetris.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
    #error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTetrisApp:
// 有关此类的实现，请参阅 Tetris.cpp
//

class CTetrisApp : public CWinApp
{
public:
    CTetrisApp();
    ~CTetrisApp();

// 重写
public:
    virtual BOOL InitInstance();

// 实现
    MCIDEVICEID m_se_apprID;
    MCIDEVICEID m_se_turnID;
    MCIDEVICEID m_se_dsprID;
    MCIDEVICEID m_se_dsblID;
    MCIDEVICEID m_se_crsrID;
    MCIDEVICEID m_se_slctID;
    MCIDEVICEID m_me_gmvrID;
};

extern CTetrisApp theApp;