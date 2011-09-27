#pragma once
#include "block.h"

class CTetrisDlg : public CDialog
{
public:
	CTetrisDlg(CWnd* pParent = NULL);
    ~CTetrisDlg();

	enum
    {
        IDD = IDD_TETRIS_DIALOG,
        ROW = 20,
        COL = 10,
        WIDTH = COL << 5,
        HEIGHT = ROW << 5
    };

protected:
    BOOL PreTranslateMessage(MSG *pMsg);

protected:
	HICON m_hIcon;


private:
    CMenu m_menu;
    CImage m_bk;
    CImage m_block;
    CImage m_window;
    CImage m_gameover;
    CDC m_memDC;
    CBitmap m_memBmp;
    CClientDC *m_pDC;

    BYTE **m_board;
    Block *m_pBlock;
    BYTE m_level;
    BYTE m_gameParam;
    BYTE m_nextColor;
    UINT m_lines;
    UINT m_score;
    BYTE m_mouseOver;

    void Update();
    void AdjustFrame();
    void Initialize();
    void RedrawBkgnd(RECT rect);
    void DrawText(SHORT x1, SHORT y1, SHORT x2, SHORT y2,
        CString &text, UINT format, COLORREF clr);
    void SetFontSize(BYTE size);
    void Play(MCIDEVICEID id);
    void UpdateBlock();
    void UpdateWindow();
    void NextRandomBlock();
    BYTE NextRandomColor();
    Block *BlockFromIndex(BYTE i);
    BOOL CheckLine(BYTE row);
    void RemoveLine(BYTE row);
    BOOL IsGameOver(BYTE blockType);
    void GameOver();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg UINT OnGetDlgCode();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnGameNew();
    afx_msg void OnGamePause();
    afx_msg void OnGameStop();
    afx_msg void OnLevelBeginner();
    afx_msg void OnLevelIntermediate();
    afx_msg void OnLevelAdvanced();
    afx_msg void OnLevelExpert();
    afx_msg void OnLevelDevil();
    afx_msg void OnLevelHell();
    afx_msg void OnGameSound();
    afx_msg void OnGameExit();
	afx_msg void OnHelpAbout();
	DECLARE_MESSAGE_MAP()
};

// CAboutDlg dialog
class CAboutDlg : public CDialog
{
public:
    CAboutDlg(CWnd* pParent = NULL) : CDialog(CAboutDlg::IDD, pParent) {}

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

};