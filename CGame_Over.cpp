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
	DDX_Control(pDX, IDOK, CConfirm);
}


BEGIN_MESSAGE_MAP(CGame_Over, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CGame_Over::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CGame_Over 消息处理程序


void CGame_Over::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	MoveWindow(100, 100, 1500, 1200);
	CConfirm.MoveWindow(700, 1100, 140, 40);
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	CString tmp;
	tmp.Format(TEXT("总计获得经费：%lld"), prt->Total_Money);
	dc.TextOutW(630, 70, tmp);
	tmp.Format(TEXT("单回合最大收益：%lld"), prt->Max_Money);
	dc.TextOutW(630, 100, tmp);

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
	dc.TextOutW(640, 10, TEXT("游戏结束！"));
	dc.SelectObject(pOldFont);

	Card tc;
	int px=30, py=150;
	CDC* pDC = GetDC();
	for (int i = 1; i <= 34; i++) {
		tc.Create(i);
		tc.pos = CPoint(px, py);
		tc.Draw(pDC);
		tmp.Format(TEXT("%lld"), prt->Sum[i]);
		dc.TextOutW(px+120, py+40, tmp);
		py += 130;
		if (i % 7 == 0) px += 280, py = 150;

	}	
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


void CGame_Over::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);
	CDialogEx::OnRButtonDown(nFlags, point);
}
