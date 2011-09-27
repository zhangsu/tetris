#include "stdafx.h"
#include "Tetris.h"
#include "TetrisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTetrisDlg::CTetrisDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTetrisDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bk.Load(_T("res\\003-StarlitSky01.jpg"));
    m_block.Load(_T("res\\178-Switch01.png"));
    m_window.Load(_T("res\\window.png"));
    m_gameover.Load(_T("res\\gameover.png"));
    m_board = new BYTE*[ROW];
    for(BYTE i = 0; i < ROW; ++i)
    {
        m_board[i] = new BYTE[COL];
    }
    m_level = 0;
    m_gameParam = 0x08;
    m_pBlock = NULL;
}

CTetrisDlg::~CTetrisDlg()
{
    m_memBmp.DeleteObject();
    m_memDC.DeleteDC();
    delete m_pDC;
    delete m_pBlock;
    for(BYTE i = 0; i < ROW; ++i)
    {
        delete[] m_board[i];
    }
    delete[] m_board;
}

BEGIN_MESSAGE_MAP(CTetrisDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_GETDLGCODE()
    ON_WM_ERASEBKGND()
    ON_WM_TIMER()
    ON_COMMAND(ID_GAME_NEW, &CTetrisDlg::OnGameNew)
    ON_COMMAND(ID_GAME_PAUSE, &CTetrisDlg::OnGamePause)
    ON_COMMAND(ID_GAME_STOP, &CTetrisDlg::OnGameStop)
    ON_COMMAND(ID_LEVEL_BEGINNER, &CTetrisDlg::OnLevelBeginner)
    ON_COMMAND(ID_LEVEL_INTERMEDIATE, &CTetrisDlg::OnLevelIntermediate)
    ON_COMMAND(ID_LEVEL_ADVANCED, &CTetrisDlg::OnLevelAdvanced)
    ON_COMMAND(ID_LEVEL_EXPERT, &CTetrisDlg::OnLevelExpert)
    ON_COMMAND(ID_LEVEL_DEVIL, &CTetrisDlg::OnLevelDevil)
    ON_COMMAND(ID_LEVEL_HELL, &CTetrisDlg::OnLevelHell)
    ON_COMMAND(ID_GAME_SOUND, &CTetrisDlg::OnGameSound)
    ON_COMMAND(ID_GAME_EXIT, &CTetrisDlg::OnGameExit)
	ON_COMMAND(ID_HELP_ABOUT, &CTetrisDlg::OnHelpAbout)
END_MESSAGE_MAP()

void CTetrisDlg::Update()
{
    RedrawBkgnd(CRect(0, 0, WIDTH + 320, HEIGHT));
    UpdateBlock();
    UpdateWindow();
    if(m_gameParam & 0x04)
    {
        CDC gameoverMemDC;
        gameoverMemDC.CreateCompatibleDC(&m_memDC);
        gameoverMemDC.SelectObject(m_gameover);
        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 255;
        bf.AlphaFormat = AC_SRC_ALPHA;
        ::AlphaBlend(m_memDC, 0, 0, WIDTH + 320, HEIGHT, gameoverMemDC, 0, 0, WIDTH + 320, HEIGHT, bf);
    }
    m_pDC->BitBlt(0, 0, WIDTH + 320, HEIGHT, &m_memDC, 0, 0, SRCCOPY);
}

void CTetrisDlg::AdjustFrame()
{
    CRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = WIDTH + 320;
    rect.bottom = HEIGHT;
    ::AdjustWindowRectEx(rect, ::GetWindowLong(m_hWnd, GWL_STYLE), TRUE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE));
    SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER);
}

void CTetrisDlg::Initialize()
{
    KillTimer(555);
    m_lines = 0;
    m_score = 0;
    for(BYTE i = 0; i < ROW; ++i)
    {
        memset(m_board[i], 0, COL);
    }
    delete m_pBlock;
    m_pBlock = NULL;
    m_menu.EnableMenuItem(ID_GAME_STOP, MF_DISABLED | MF_GRAYED);
    m_menu.EnableMenuItem(ID_GAME_PAUSE, MF_DISABLED | MF_GRAYED);
    m_menu.CheckMenuItem(ID_GAME_PAUSE, MF_UNCHECKED);
}

void CTetrisDlg::RedrawBkgnd(RECT rect)
{
    CBrush bkBr(CBitmap::FromHandle(m_bk));
    m_memDC.FillRect(&rect, &bkBr);
}

void CTetrisDlg::DrawText(SHORT x1, SHORT y1, SHORT x2, SHORT y2, CString &text, UINT format, COLORREF clr = 0xFFFFFF)
{
    m_memDC.SetTextColor(0x000000);
    m_memDC.DrawText(text, CRect(x1 + 2, y1 + 2, x2 + 2, y2 + 2), format | DT_NOCLIP);
    m_memDC.SetTextColor(clr);
    m_memDC.DrawText(text, CRect(x1, y1, x2, y2), format | DT_NOCLIP);
}

void CTetrisDlg::SetFontSize(BYTE size)
{
    CFont font;
    font.CreateFont(
        size,                       // height of font
        0,                          // average character width
        0,                          // angle of escapement
        0,                          // base-line orientation angle
        FW_NORMAL,                  // font weight
        FALSE,                      // italic attribute option
        FALSE,                      // underline attribute option
        0,                          // strikeout attribute option
        ANSI_CHARSET,               // character set identifier
        OUT_DEFAULT_PRECIS,         // output precision
        CLIP_DEFAULT_PRECIS,        // clipping precision
        DEFAULT_QUALITY,            // output quality
        DEFAULT_PITCH | FF_SWISS,   // pitch and family
        _T("Impact")                // typeface name
    );
    m_memDC.SelectObject(&font);
}

void CTetrisDlg::Play(MCIDEVICEID id)
{
    if(m_gameParam & 0x08)
    {
        MCI_PLAY_PARMS playParms;
        playParms.dwFrom = 0;
        ::mciSendCommand(id, MCI_PLAY, MCI_FROM, (DWORD)(LPMCI_PLAY_PARMS)&playParms);
    }
}

void CTetrisDlg::UpdateBlock()
{
    CDC blockMemDC;
    blockMemDC.CreateCompatibleDC(&m_memDC);
    blockMemDC.SelectObject(m_block);

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 220;
    bf.AlphaFormat = AC_SRC_ALPHA;

    for(BYTE i = 0; i < ROW; ++i)
    {
        BYTE *nextRow = m_board[i];
        for(BYTE j = 0; j < COL; ++j)
        {
            BYTE clr = nextRow[j];
            if(clr)
            {
                ::AlphaBlend(m_memDC, j << 5, i << 5, 32, 32, blockMemDC, (clr - 1) << 5, 0, 32, 32, bf);
                //m_memDC.StretchBlt(j << 5, i << 5, 32, 32, &blockMemDC, (clr - 1) << 5, 0, 32, 32, SRCCOPY);
            }
        }
    }
    blockMemDC.DeleteDC();
}

void CTetrisDlg::UpdateWindow()
{
    CDC wndMemDC;
    wndMemDC.CreateCompatibleDC(&m_memDC);
    wndMemDC.SelectObject(m_window);
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255;
    bf.AlphaFormat = AC_SRC_ALPHA;
    ::AlphaBlend(m_memDC, 328, 10, 308, 629, wndMemDC, 0, 0, 308, 629, bf);
    wndMemDC.DeleteDC();

    SetFontSize(30);
    CString str;
    DrawText(360, 32, 600, 64, str = "NEXT TETROMINO:", DT_LEFT);
    CDC blockMemDC;
    blockMemDC.CreateCompatibleDC(&m_memDC);
    blockMemDC.SelectObject(m_block);
    BYTE x = (m_nextColor - 1) << 5;
    switch((m_gameParam & 0xE0) >> 5)
    {
    case 1:
        m_memDC.StretchBlt(414, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(446, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(478, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(510, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        break;
    case 2:
        m_memDC.StretchBlt(430, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(494, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(494, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        break;
    case 3:
        m_memDC.StretchBlt(430, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(494, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(430, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        break;
    case 4:
        m_memDC.StretchBlt(446, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(478, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(446, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(478, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        break;
    case 5:
        m_memDC.StretchBlt(462, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(494, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(430, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        break;
    case 6:
        m_memDC.StretchBlt(430, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(494, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        break;
    case 7:
        m_memDC.StretchBlt(430, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 96, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(462, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
        m_memDC.StretchBlt(494, 128, 32, 32, &blockMemDC, x, 0, 32, 32, SRCCOPY);
    }
    blockMemDC.DeleteDC();
    DrawText(360, 192, 600, 224, str = "LEVEL:", DT_LEFT);
    str.Format(_T("%d"), m_level + 1);
    DrawText(360, 192, 600, 224, str, DT_RIGHT);
    DrawText(360, 256, 600, 288, str = "SCORE:", DT_LEFT);
    str.Format(_T("%d"), m_score);
    DrawText(360, 256, 600, 288, str, DT_RIGHT);
    DrawText(360, 320, 600, 352, str = "LINES:", DT_LEFT);
    str.Format(_T("%d"), m_lines);
    DrawText(360, 320, 600, 352, str, DT_RIGHT);
    SetFontSize(m_mouseOver == 0x01 ? 36 : 30);
    DrawText(360, 384, 600, 416, str = "NEW", DT_CENTER);
    if(!(m_gameParam & 0x01) || (m_gameParam & 0x04))
    {
        SetFontSize(30);
        DrawText(400, 448, 560, 480, str = "PAUSE", DT_LEFT, 0xA0A0A0);
        DrawText(400, 448, 560, 480, str = "STOP", DT_RIGHT, 0xA0A0A0);
    }
    else
    {
        SetFontSize(m_mouseOver == 0x02 ? 36 : 30);
        DrawText(400, 448, 560, 480, str = "PAUSE", DT_LEFT, m_gameParam & 0x02 ? 0x0000FF : 0xFFFFFF);
        SetFontSize(m_mouseOver == 0x04 ? 36 : 30);
        DrawText(400, 448, 560, 480, str = "STOP", DT_RIGHT, 0xFFFFFF);
    }
    SetFontSize(m_mouseOver == 0x08 ? 36 : 30);
    SetTextColor(m_memDC, 0xFFFFFF);
    DrawText(360, 512, 600, 544, str = "SOUND", DT_CENTER, m_gameParam & 0x08 ? 0x0000FF : 0xFFFFFF);
    SetFontSize(m_mouseOver == 0x10 ? 36 : 30);
    DrawText(360, 576, 600, 608, str = "EXIT", DT_CENTER);
}

void CTetrisDlg::NextRandomBlock()
{
    UINT r;
    rand_s(&r);
    m_gameParam &= 0x1F;
    m_gameParam |= ((BYTE)((DOUBLE)r / ((__int64)UINT_MAX + 1) * 7) + 1) << 5;
    m_nextColor = NextRandomColor();
}

BYTE CTetrisDlg::NextRandomColor()
{
    UINT r;
    rand_s(&r);
    return (BYTE)((DOUBLE)r / ((__int64)UINT_MAX + 1) * 4 + 1);
}

Block *CTetrisDlg::BlockFromIndex(BYTE i)
{
    Play(theApp.m_se_apprID);
    switch(i >> 5)
    {
    case 1:
        return new BlockI(m_nextColor, m_board);
    case 2:
        return new BlockJ(m_nextColor, m_board);
    case 3:
        return new BlockL(m_nextColor, m_board);
    case 4:
        return new BlockO(m_nextColor, m_board);
    case 5:
        return new BlockS(m_nextColor, m_board);
    case 6:
        return new BlockT(m_nextColor, m_board);
    default:
        return new BlockZ(m_nextColor, m_board);
    }
}

BOOL CTetrisDlg::CheckLine(BYTE row)
{
    BYTE *thisRow = m_board[row];
    for(BYTE i = 0; i < COL; ++i)
    {
        if(!thisRow[i])
            return false;
    }
    return true;
}

void CTetrisDlg::RemoveLine(BYTE row)
{
    BYTE *prevRow;
    BYTE *thisRow = m_board[row];

    CDC blockMemDC;
    blockMemDC.CreateCompatibleDC(&m_memDC);
    blockMemDC.SelectObject(m_block);

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;

    Play(theApp.m_se_dsprID);

    for(BYTE opacity = 220; opacity != 0; --opacity)
    {
        RedrawBkgnd(CRect(0, row << 5, WIDTH, (row + 1) << 5));
        bf.SourceConstantAlpha = opacity;
        for(BYTE i = 0; i < COL; ++i)
        {
            ::AlphaBlend(m_memDC, i << 5, row << 5, 32, 32, blockMemDC, (thisRow[i] - 1) << 5, 0, 32, 32, bf);
        }
        m_pDC->BitBlt(0, 0, WIDTH, HEIGHT, &m_memDC, 0, 0, SRCCOPY);
    }
    for(CHAR i = row; i > 0; --i)
    {
        prevRow = m_board[i - 1];
        thisRow = m_board[i];
        for(BYTE j = 0; j < COL; ++j)
        {
            thisRow[j] = prevRow[j];
        }
    }
    for(BYTE i = 0; i < COL; ++i)
    {
        prevRow[i] = 0;
    }
    blockMemDC.DeleteDC();
    m_score += 100 + m_level * 50;
    ++m_lines;
    Update();
}

BOOL CTetrisDlg::IsGameOver(BYTE blockType)
{
    switch(blockType)
    {
    case 0:
        return m_board[0][3] || m_board[0][4] || m_board[0][5] || m_board[0][6];
    case 1:
        return m_board[0][3] || m_board[0][4] || m_board[0][5]|| m_board[1][5];
    case 2:
        return m_board[0][3] || m_board[0][4] || m_board[0][5] || m_board[1][3];
    case 3:
        return m_board[0][4] || m_board[0][5] || m_board[1][4] || m_board[1][5];
    case 4:
        return m_board[0][4] || m_board[0][5] || m_board[1][3] || m_board[1][4];
    case 5:
        return m_board[0][3] || m_board[0][4] || m_board[0][5] || m_board[1][4];
    default:
        return m_board[0][3] || m_board[0][4] || m_board[1][4] || m_board[1][5];
    }
}

void CTetrisDlg::GameOver()
{
    Play(theApp.m_me_gmvrID);
    KillTimer(555);
    m_gameParam |= 0x04;
    m_menu.EnableMenuItem(ID_GAME_STOP, MF_DISABLED | MF_GRAYED);
    m_menu.EnableMenuItem(ID_GAME_PAUSE, MF_DISABLED | MF_GRAYED);    
}

BOOL CTetrisDlg::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
        switch(pMsg->wParam)
		{
        // Disable OK & Cancel function
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		}
	}
    return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTetrisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
    m_menu.LoadMenu(IDR_MENU);
    SetMenu(&m_menu);
    m_menu.EnableMenuItem(ID_GAME_PAUSE, MF_DISABLED | MF_GRAYED);
    m_menu.EnableMenuItem(ID_GAME_STOP, MF_DISABLED | MF_GRAYED);
    m_pDC = new CClientDC(this);
    m_memDC.CreateCompatibleDC(m_pDC);
    m_memBmp.CreateCompatibleBitmap(m_pDC, WIDTH + 320, HEIGHT);
    m_memDC.SelectObject(m_memBmp);
    m_memDC.SetBkMode(TRANSPARENT);
    SetFontSize(30);
    AdjustFrame();
    Initialize();
	m_menu.CheckMenuItem(ID_GAME_SOUND, MF_CHECKED);
	m_menu.CheckMenuItem(ID_LEVEL_BEGINNER, MF_CHECKED);


    return TRUE;
}

void CTetrisDlg::OnPaint()
{
    Update();
	if (IsIconic())
	{
	    CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CTetrisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTetrisDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    switch(nChar)
    {
    case VK_F1:
        PostMessage(WM_COMMAND, ID_GAME_NEW, 0L);
        break;
    case VK_F2:
        if((m_gameParam & 0x01) && !(m_gameParam & 0x04))
            PostMessage(WM_COMMAND, ID_GAME_PAUSE, 0L);
        break;
    case VK_F3:
        if((m_gameParam & 0x01) && !(m_gameParam & 0x04))
            PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
        break;
    case VK_F4:
        PostMessage(WM_COMMAND, ID_GAME_SOUND, 0L);
    }
    if((m_gameParam & 0x07) == 0x01)
    {
        switch(nChar)
        {
        case VK_UP:
            if(!(nFlags & 0x4000))
            {
                if(m_pBlock->canTurn())
                {
                    Play(theApp.m_se_turnID);
                    m_pBlock->turn();
                }
                else if(m_gameParam & 0x08)
                {
                    Play(theApp.m_se_dsblID);
                }
            }
            break;
        case VK_DOWN:
            if(m_pBlock->canMoveDown())
            {
                m_pBlock->moveDown();
                m_score += m_level + 1;
            }
            break;
        case VK_LEFT:
            if(m_pBlock->canMoveLeft())
            {
                m_pBlock->moveLeft();
            }
            break;
        case VK_RIGHT:
            if(m_pBlock->canMoveRight())
            {
                m_pBlock->moveRight();
            }
            break;
        }
        Invalidate(FALSE);
    }
    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTetrisDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_mouseOver != 0x00)
    {
        m_gameParam |= 0x10;
    }
}

void CTetrisDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_gameParam & 0x10)
    {
        if(m_mouseOver == 0x01)
        {
            PostMessage(WM_COMMAND, ID_GAME_NEW);
        }
        else if((m_gameParam & 0x01) && !(m_gameParam & 0x04) && m_mouseOver == 0x02)
        {
            PostMessage(WM_COMMAND, ID_GAME_PAUSE);
        }
        else if((m_gameParam & 0x01) && !(m_gameParam & 0x04) && m_mouseOver == 0x04)
        {
            PostMessage(WM_COMMAND, ID_GAME_STOP);
        }
        else if(m_mouseOver == 0x08)
        {
            PostMessage(WM_COMMAND, ID_GAME_SOUND);
        }
        else if(m_mouseOver == 0x10)
        {
            PostMessage(WM_COMMAND, ID_GAME_EXIT);
        }
    }
}

void CTetrisDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rect;
    rect.SetRect(459, 384, 502, 413);
    if(m_mouseOver != 0x01 && ::PtInRect(&rect, point))
    {
        Play(theApp.m_se_crsrID);
        m_mouseOver = 0x01;
        Invalidate(FALSE);
    }
    else if(m_mouseOver == 0x01 && !::PtInRect(&rect, point))
    {
        m_mouseOver = 0x00;
        Invalidate(FALSE);
    }
    else
    {
        rect.SetRect(400, 448, 459, 477);
        if((m_gameParam & 0x01) && !(m_gameParam & 0x04) && m_mouseOver != 0x02 && ::PtInRect(&rect, point))
        {
            Play(theApp.m_se_crsrID);
            m_mouseOver = 0x02;
            Invalidate(FALSE);
        }
        else if(m_mouseOver == 0x02 && !::PtInRect(&rect, point))
        {
            m_mouseOver = 0x00;
            Invalidate(FALSE);
        }
        else
        {
            rect.SetRect(512, 448, 560, 477);
            if((m_gameParam & 0x01) && !(m_gameParam & 0x04) && m_mouseOver != 0x04 && ::PtInRect(&rect, point))
            {
                Play(theApp.m_se_crsrID);
                m_mouseOver = 0x04;
                Invalidate(FALSE);
            }
            else if(m_mouseOver == 0x04 && !::PtInRect(&rect, point))
            {
                m_mouseOver = 0x00;
                Invalidate(FALSE);
            }
            else
            {
                rect.SetRect(448, 512, 512, 541);
                if(m_mouseOver != 0x08 && ::PtInRect(&rect, point))
                {
                    Play(theApp.m_se_crsrID);
                    m_mouseOver = 0x08;
                    Invalidate(FALSE);
                }
                else if(m_mouseOver == 0x08 && !::PtInRect(&rect, point))
                {
                    m_mouseOver = 0x00;
                    Invalidate(FALSE);
                }
                else
                {
                    rect.SetRect(460, 576, 500, 608);
                    if(m_mouseOver != 0x10 && ::PtInRect(&rect, point))
                    {
                        Play(theApp.m_se_crsrID);
                        m_mouseOver = 0x10;
                        Invalidate(FALSE);
                    }
                    else if(m_mouseOver == 0x10 && !::PtInRect(&rect, point))
                    {
                        m_mouseOver = 0x00;
                        Invalidate(FALSE);
                    }
                }
            }
        }
    }
}

UINT CTetrisDlg::OnGetDlgCode()
{
    return DLGC_WANTARROWS | CDialog::OnGetDlgCode();
}

BOOL CTetrisDlg::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}
  
void CTetrisDlg::OnTimer(UINT_PTR nIDEvent)
{
    if(m_pBlock->canMoveDown())
    {
        m_pBlock->moveDown();
    }
    else
    {
        for(CHAR i = ROW - 1; i >= 0; --i)
        {
            if(CheckLine(i))
            {
                RemoveLine(i);
                ++i;
            }
        }
        MSG msg;
        while(::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE));
        if(IsGameOver(m_gameParam & 0xE0))
        {
            GameOver();
        }
        else
        {
            delete m_pBlock;
            m_pBlock = BlockFromIndex(m_gameParam & 0xE0);
            NextRandomBlock();
            m_nextColor = NextRandomColor();
        }
    }
    Invalidate(FALSE);
    CDialog::OnTimer(nIDEvent);
}

void CTetrisDlg::OnGameNew()
{
    if(m_gameParam & 0x01)
    {
        Initialize();
    }
    m_gameParam |= 0x01;
    m_gameParam &= ~0x06;
    m_menu.EnableMenuItem(ID_GAME_STOP, MF_ENABLED);
    m_menu.EnableMenuItem(ID_GAME_PAUSE, MF_ENABLED);
    SetTimer(555, 500 - m_level * 90, NULL);
    NextRandomBlock();
    m_pBlock = BlockFromIndex(m_gameParam & 0xE0);
    NextRandomBlock();
    Invalidate(FALSE);
}

void CTetrisDlg::OnGamePause()
{
    Play(theApp.m_se_slctID);
    if(m_gameParam & 0x02)
    {
        m_menu.CheckMenuItem(ID_GAME_PAUSE, MF_UNCHECKED);
        m_gameParam &= ~0x02;
        SetTimer(555, 500 - m_level * 90, NULL);
    }
    else
    {
        m_menu.CheckMenuItem(ID_GAME_PAUSE, MF_CHECKED);
        m_gameParam |= 0x02;
        KillTimer(555);
    }
    Invalidate(FALSE);
}
void CTetrisDlg::OnGameStop()
{
    Play(theApp.m_se_slctID);
    m_gameParam &= ~0xE7;
    Initialize();
    Invalidate(FALSE);
}

void CTetrisDlg::OnLevelBeginner()
{
    m_menu.GetSubMenu(0)->GetSubMenu(3)->CheckMenuItem(m_level, MF_BYPOSITION | MF_UNCHECKED);
    m_menu.CheckMenuItem(ID_LEVEL_BEGINNER, MF_CHECKED);
    m_level = 0;
    if(m_gameParam & 0x01)
        PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
}

void CTetrisDlg::OnLevelIntermediate()
{
    m_menu.GetSubMenu(0)->GetSubMenu(3)->CheckMenuItem(m_level, MF_BYPOSITION | MF_UNCHECKED);
    m_menu.CheckMenuItem(ID_LEVEL_INTERMEDIATE, MF_CHECKED);
    m_level = 1;
    if(m_gameParam & 0x01)
        PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
}

void CTetrisDlg::OnLevelAdvanced()
{
    m_menu.GetSubMenu(0)->GetSubMenu(3)->CheckMenuItem(m_level, MF_BYPOSITION | MF_UNCHECKED);
    m_menu.CheckMenuItem(ID_LEVEL_ADVANCED, MF_CHECKED);
    m_level = 2;
    if(m_gameParam & 0x01)
        PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
}

void CTetrisDlg::OnLevelExpert()
{
    m_menu.GetSubMenu(0)->GetSubMenu(3)->CheckMenuItem(m_level, MF_BYPOSITION | MF_UNCHECKED);
    m_menu.CheckMenuItem(ID_LEVEL_EXPERT, MF_CHECKED);
    m_level = 3;
    if(m_gameParam & 0x01)
        PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
}

void CTetrisDlg::OnLevelDevil()
{
    m_menu.GetSubMenu(0)->GetSubMenu(3)->CheckMenuItem(m_level, MF_BYPOSITION | MF_UNCHECKED);
    m_menu.CheckMenuItem(ID_LEVEL_DEVIL, MF_CHECKED);
    m_level = 4;
    if(m_gameParam & 0x01)
        PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
}

void CTetrisDlg::OnLevelHell()
{
    m_menu.GetSubMenu(0)->GetSubMenu(3)->CheckMenuItem(m_level, MF_BYPOSITION | MF_UNCHECKED);
    m_menu.CheckMenuItem(ID_LEVEL_HELL, MF_CHECKED);
    m_level = 5;
    if(m_gameParam & 0x01)
        PostMessage(WM_COMMAND, ID_GAME_STOP, 0L);
}

void CTetrisDlg::OnGameSound()
{
    Play(theApp.m_se_slctID);
    if(m_gameParam & 0x08)
    {
        m_menu.CheckMenuItem(ID_GAME_SOUND, MF_UNCHECKED);
        m_gameParam &= ~0x08;
    }
    else
    {
        m_menu.CheckMenuItem(ID_GAME_SOUND, MF_CHECKED);
        m_gameParam |= 0x08;
    }
    Invalidate(FALSE);
}

void CTetrisDlg::OnGameExit()
{
    PostMessage(WM_COMMAND, IDOK, 0L);
}

void CTetrisDlg::OnHelpAbout()
{
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
}
