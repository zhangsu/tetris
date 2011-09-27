#include "stdafx.h"
#include "Tetris.h"
#include "TetrisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTetrisApp::CTetrisApp()
{
    MCI_OPEN_PARMS openParms;
    memset(&openParms, 0, sizeof(openParms));

    openParms.lpstrDeviceType = _T("waveaudio");
    openParms.lpstrElementName = _T("res\\001-System01.wav");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_se_apprID = openParms.wDeviceID;

    openParms.lpstrElementName = _T("res\\003-System03.wav");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_se_turnID = openParms.wDeviceID;

    openParms.lpstrElementName = _T("res\\004-System04.wav");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_se_dsblID = openParms.wDeviceID;

    openParms.lpstrElementName = _T("res\\012-System12.wav");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_se_dsprID = openParms.wDeviceID;

    openParms.lpstrElementName = _T("res\\032-Switch01.wav");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_se_crsrID = openParms.wDeviceID;

    openParms.lpstrElementName = _T("res\\033-Switch02.wav");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_se_slctID = openParms.wDeviceID;

    openParms.lpstrElementName = _T("res\\006-Defeat02.mid");
    openParms.lpstrDeviceType = _T("sequencer");
    ::mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPMCI_OPEN_PARMS)&openParms);
    m_me_gmvrID = openParms.wDeviceID;
}

CTetrisApp::~CTetrisApp()
{
    ::mciSendString(_T("close all"), NULL, 0, 0);
}

CTetrisApp theApp;

BOOL CTetrisApp::InitInstance()
{
	CWinApp::InitInstance();

	CTetrisDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}