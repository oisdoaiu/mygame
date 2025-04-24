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
}


BEGIN_MESSAGE_MAP(CLibrary, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CLIBRARY 消息处理程序

void CLibrary::OnPaint()
{
	CPaintDC dc(this);
	const int DISX = 120;
	const int DISY = 120;
	int it = 1;
	vector<Card>Show = prt->cards;
	int sx = 20;
	for (int i = 0; i < 6 && it < Show.size(); i++) {
		int sy = 20;
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
	const int DISX = 120;
	const int DISY = 120;
	int sx = 20;
	int it = 1;
	vector<Card>Show = prt->cards;
	for (int i = 0; i < 6 && it < Show.size(); i++) {
		int sy = 20;
		for (int j = 0; j < 11 && it < Show.size(); j++) {
			if (sx <= point.y && point.y <= sx + 100 && sy <= point.x && point.x <= sy + 100) {
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

	MoveWindow(150, 150, 1400, 900);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
