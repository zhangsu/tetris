// TetrisDlg.h : 头文件
//

#pragma once
#include "block.h"

// CTetrisDlg 对话框
class CTetrisDlg : public CDialog
{
// 构造
public:
	CTetrisDlg(CWnd* pParent = NULL);	                // 标准构造函数
    ~CTetrisDlg();

// 对话框数据
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

// 实现
protected:
	HICON m_hIcon;


private:
    CMenu m_menu;                                           // 主菜单
    CImage m_bk;                                            // 背景图片对象
    CImage m_block;                                         // 方块图片对象
    CImage m_window;                                        // 窗口图片对象
    CImage m_gameover;                                      // 游戏结束图片对象
    CDC m_memDC;                                            // 缓存兼容设备上下文
    CBitmap m_memBmp;                                       // 缓存兼容位图
    CClientDC *m_pDC;                                       // 客户区设备上下文

    BYTE **m_board;                                         // 棋盘（容器）
    Block *m_pBlock;                                        // 当前活动方块指针
    BYTE m_level;                                           // 游戏难度
    BYTE m_gameParam;                                       // 游戏参数（位->0: 游戏开始, 1: 游戏暂停, 2: 游戏失败, 3: 音效, 4: 左键按下, 5-7: 下一个方块）
    BYTE m_nextColor;                                       // 下一个方块的颜色
    UINT m_lines;                                           // 消除行数
    UINT m_score;                                           // 积分
    BYTE m_mouseOver;                                       // 鼠标焦点（位->0: NEW, 1: PAUSE, 2: STOP, 3: SOUND, 4:EXIT）

    void Update();                                          // 全局刷新
    void AdjustFrame();                                     // 根据客户区大小调节窗口尺寸
    void Initialize();                                      // 初始化游戏
    void RedrawBkgnd(RECT rect);                            // 重绘背景图片
    void DrawText(SHORT x1, SHORT y1, SHORT x2, SHORT y2,
        CString &text, UINT format, COLORREF clr);          // 描绘文字
    void SetFontSize(BYTE size);                            // 设置字体大小
    void Play(MCIDEVICEID id);                              // 播放音效
    void UpdateBlock();                                     // 更新方块图形
    void UpdateWindow();                                    // 更新窗口
    void NextRandomBlock();                                 // 生成下一个随机方块的索引
    BYTE NextRandomColor();                                 // 生成下一个随机方块的颜色
    Block *BlockFromIndex(BYTE i);                          // 从方块索引获取方块对象
    BOOL CheckLine(BYTE row);                               // 检查一行是否可消除
    void RemoveLine(BYTE row);                              // 消除一行
    BOOL IsGameOver(BYTE blockType);                        // 是否游戏失败
    void GameOver();                                        // 游戏失败

public:
    // 生成的消息映射函数
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