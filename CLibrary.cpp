// CLIBRARY.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "CLIBRARY.h"
#include "CInfo.h"


// CLIBRARY 对话框

IMPLEMENT_DYNAMIC(CLibrary, CDialogEx)

CLibrary::CLibrary(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIBRARY, pParent)
{

}

CLibrary::~CLibrary()
{
}



void CLibrary::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, CConfirm);
}


BEGIN_MESSAGE_MAP(CLibrary, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CLibrary::SetPos(int x, int y, int& resx, int& resy) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的大小和位置
	resx = static_cast<int>(screenWidth * (x / 2560.0));
	resy = static_cast<int>(screenHeight * (y / 1600.0));
}

// CLIBRARY 消息处理程序

void CLibrary::OnPaint()
{
	CPaintDC dc(this);
	int DISX = 120;
	int DISY = 120;
	SetPos(120, 120, DISX, DISY);
	int dx1, dy1;
	SetPos(20, 20, dx1, dy1);
	int it = 1;
	vector<Card>Show = prt->cards;
	int sx = dx1;
	for (int i = 0; i < 6 && it < Show.size(); i++) {
		int sy = dy1;
		for (int j = 0; j < 11 && it < Show.size(); j++) {
			Show[it].pos = CPoint(sy, sx);
			Show[it].Draw(&dc);
			sy += DISY;
			it++;
		}
		sx += DISX;
	}
}


void CLibrary::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int DISX = 120;
	int DISY = 120;
	SetPos(120, 120, DISX, DISY);
	int dx1, dy1;
	int dx2, dy2;
	SetPos(20, 20, dx1, dy1);
	SetPos(100, 100, dx2, dy2);
	int sx = dx1;
	int it = 1;
	vector<Card>Show = prt->cards;
	for (int i = 0; i < 6 && it < Show.size(); i++) {
		int sy = dy1;
		for (int j = 0; j < 11 && it < Show.size(); j++) {
			if (sx <= point.y && point.y <= sx + dx2 && sy <= point.x && point.x <= sy + dy2) {
				CInfo new_info;
				new_info.cur = Show[it];
				new_info.Del = true;
				new_info.prt = prt;
				new_info.curid = it;
				new_info.DoModal();
				return;
			}
			sy += DISY;
			it++;
		}
		sx += DISX;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CLibrary::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int dx1, dy1, dx2, dy2;
	SetPos(630, 750, dx1, dy1);
	SetPos(140, 40, dx2, dy2);
	CConfirm.MoveWindow(dx1, dy1, dx2, dy2);
	SetPos(150, 150, dx1, dy1);
	SetPos(1400, 900, dx2, dy2);
	MoveWindow(dx1, dy1, dx2, dy2);
	//MoveWindow(150, 150, 1400, 900);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLibrary::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);*/
	CDialogEx::OnRButtonDown(nFlags, point);
}
