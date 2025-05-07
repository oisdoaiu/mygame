// CGame_Over.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "CGame_Over.h"
#include "GameDlg.h"
#include "CStart.h"


// CGame_Over 对话框

IMPLEMENT_DYNAMIC(CGame_Over, CDialogEx)

CGame_Over::CGame_Over(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_OVER, pParent)
{

}

CGame_Over::~CGame_Over()
{
}

void CGame_Over::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGame_Over, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CGame_Over::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGame_Over 消息处理程序


void CGame_Over::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CFont newFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(24, dc.GetDeviceCaps(LOGPIXELSY), 72); // 设置字体高度为24点
	lf.lfWeight = FW_NORMAL; // 字体粗细
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS; // 仅使用TrueType字体
	wcscpy_s(lf.lfFaceName, _T("Arial")); // 字体名称
	newFont.CreateFontIndirect(&lf);
	// 保存原来的字体
	CFont* pOldFont = dc.SelectObject(&newFont);
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	dc.TextOutW(230, 100, TEXT("游戏结束！"));
	dc.SelectObject(pOldFont);
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CGame_Over::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
	CStart start_game;
	start_game.DoModal();
	OnOK();
}


void CGame_Over::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nResult = MessageBox(_T("确定要退出程序吗？"), _T("退出确认"), MB_YESNO | MB_ICONQUESTION);

	if (nResult == IDYES)
	{
		// 调用父窗口的DestroyWindow函数销毁父窗口，结束程序
		CGameDlg prt = GetParent();
		if (prt)
			prt.DestroyWindow();
		else PostQuitMessage(0);
	}
	else
	{
		// 如果用户选择不退出，返回不执行默认关闭操作
		return;
	}
	CDialogEx::OnClose();
}
