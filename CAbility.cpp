// CAbility.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "CAbility.h"


// CAbility 对话框

IMPLEMENT_DYNAMIC(CAbility, CDialogEx)

CAbility::CAbility(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABILITY, pParent)
{

}

CAbility::~CAbility()
{
}

void CAbility::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGEUP, CUP);
	DDX_Control(pDX, IDC_PAGEDOWN, CDOWN);
	DDX_Control(pDX, ID_Page, CPage);
}


BEGIN_MESSAGE_MAP(CAbility, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PAGEUP, &CAbility::OnBnClickedPageup)
	ON_BN_CLICKED(IDC_PAGEDOWN, &CAbility::OnBnClickedPagedown)
END_MESSAGE_MAP()


// CAbility 消息处理程序

const int STARTX = 10, STARTY = 10, DISX = 100, DISY = 130;
void CAbility::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	CDC* pDC = GetDC();

	int num = prt->abilities.size();
	vector<Abi>abi = prt->abilities;
	int page_num = 5;

	CUP.EnableWindow(page!=0);
	CDOWN.EnableWindow((page + 1) * page_num < num);
	CString tmp;
	tmp.Format(TEXT("第%d页"), page + 1);
	CPage.SetWindowTextW(tmp);
	CPage.EnableWindow(FALSE);

	CPoint pos = CPoint(STARTX, STARTY);
	for (int i = 0; i < page_num; i++) {
		int cur = page_num * page + i;
		if (cur == abi.size()) break;
		abi[cur].Draw(pDC, pos);
		pos.y += DISY;
	}

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CAbility::OnBnClickedPageup()
{
	page--;
	Invalidate();
	// TODO: 在此添加控件通知处理程序代码
}


void CAbility::OnBnClickedPagedown()
{
	page++;
	Invalidate();
	// TODO: 在此添加控件通知处理程序代码
}
