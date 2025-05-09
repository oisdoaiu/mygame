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
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()

void CGame_Over::SetPos(int x, int y, int& resx, int& resy) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的大小和位置
	resx = static_cast<int>(screenWidth * (x / 2560.0));
	resy = static_cast<int>(screenHeight * (y / 1600.0));
}

void CGame_Over::CalcMyFont(CDC* pDC, CFont& resfont) {
	// 获取当前屏幕的垂直 DPI
	int currentDPI = pDC->GetDeviceCaps(LOGPIXELSY);

	// 预设 DPI（对应 2560*1600 分辨率和 150% 缩放的情况）
	int defaultDPI = 96 * 1.5;

	// 预设字体的高度和宽度（逻辑单位）
	int defaultFontHeight = 18;
	int defaultFontWidth = 9;

	// 计算当前字体的高度和宽度
	int currentFontHeight = static_cast<int>(defaultFontHeight * static_cast<double>(currentDPI) / defaultDPI);
	int currentFontWidth = static_cast<int>(defaultFontWidth * static_cast<double>(currentDPI) / defaultDPI);

	// 创建字体
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -currentFontHeight; // 负数表示以逻辑单位指定字体高度
	lf.lfWidth = currentFontWidth;    // 直接设置字体宽度
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("SimHei")); // 设置字体名称

	resfont.CreateFontIndirect(&lf);
}
// CGame_Over 消息处理程序


void CGame_Over::OnPaint()
{
	CDC* pDC = GetDC();
	CalcMyFont(pDC, curfont);
	pDC->SelectObject(&curfont);
	CPaintDC dc(this); // device context for painting
	int dx1, dy1, dx2, dy2;
	SetPos(100, 100, dx1, dy1);
	SetPos(1500, 1250, dx2, dy2);
	MoveWindow(dx1, dy1, dx2, dy2);
	//MoveWindow(100, 100, 1500, 1200);
	SetPos(700, 1100, dx1, dy1);
	SetPos(140, 40, dx2, dy2);
	CConfirm.MoveWindow(dx1, dy1, dx2, dy2);
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	CString tmp;
	tmp.Format(TEXT("总计获得经费：%lld"), prt->Total_Money);
	SetPos(630, 70, dx1, dy1);
	dc.TextOutW(dx1, dy1, tmp);
	tmp.Format(TEXT("单回合最大收益：%lld"), prt->Max_Money);
	SetPos(630, 100, dx1, dy1);
	dc.TextOutW(dx1, dy1, tmp);

	SetPos(0, 24, dx1, dy1);
	CFont newFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(dy1, dc.GetDeviceCaps(LOGPIXELSY), 72); // 设置字体高度为24点
	lf.lfWeight = FW_NORMAL; // 字体粗细
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS; // 仅使用TrueType字体
	wcscpy_s(lf.lfFaceName, _T("Arial")); // 字体名称
	newFont.CreateFontIndirect(&lf);
	// 保存原来的字体
	CFont* pOldFont = dc.SelectObject(&newFont);
	SetPos(640, 10, dx1, dy1);
	dc.TextOutW(dx1, dy1, TEXT("游戏结束！"));
	//dc.TextOutW(640, 10, TEXT("游戏结束！"));
	dc.SelectObject(&curfont);

	Card tc;
	int px=30, py=150;
	SetPos(30, 150, px, py);
	SetPos(125, 40, dx1, dy1);
	int dx3, dy3;
	SetPos(0, 130, dx2, dy2);
	SetPos(280,150, dx3, dy3);
	for (int i = 1; i <= 34; i++) {
		tc.Create(i);
		tc.pos = CPoint(px, py);
		tc.Draw(pDC);
		tmp.Format(TEXT("%lld"), prt->Sum[i]);
		dc.TextOutW(px + dx1, py + dy1, tmp);
		py += dy2;
		if (i % 7 == 0) px += dx3, py = dy3;

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

	/*CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);*/
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CGame_Over::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHitTest == HTCAPTION)
	{
		return;
	}
	else
	{
		// 对于其他区域，执行默认处理
		return CDialogEx::OnNcLButtonDown(nHitTest, point);
	}
}


void CGame_Over::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

}
