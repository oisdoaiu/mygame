// Card.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "Card.h"
#include "GameDlg.h"


// Card 对话框

IMPLEMENT_DYNAMIC(Card, CDialogEx)

Card::Card(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{

}

Card::~Card()
{
}

void Card::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, Check_Lib);
}

void CalcPos(int x, int y, int& resx, int& resy) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的大小和位置
	resx = static_cast<int>(screenWidth * (x / 2560.0));
	resy = static_cast<int>(screenHeight * (y / 1600.0));
}

void CalcMyFont(CDC* pDC, CFont &resfont) {
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

int DIS = 5; //边框
int CARDWIDTH;
int CARDLENGTH;
void Card::Draw(CDC* pDC)
{
	int DISX, DISY;
	CalcPos(5, 5, DISX, DISY);
	CFont curfont;
	CalcMyFont(pDC, curfont);
	pDC->SelectObject(&curfont);
	CalcPos(120, 120, CARDWIDTH, CARDLENGTH);
	CRect tmp_rect(pos.x, pos.y, pos.x + CARDLENGTH, pos.y + CARDWIDTH);
	pDC->Rectangle(tmp_rect);
	CString tmp;
	int dx1 = 50;	
	int dy1 = 50;
	int dx2 = 40;
	int dy2 = 50;
	int dx3 = 30;
	int dy3 = 50;
	int dx4 = 23;
	int dy4 = 50;
	CalcPos(46, 54, dx1, dy1);
	CalcPos(37, 54, dx2, dy2);
	CalcPos(28, 54, dx3, dy3);
	CalcPos(19, 54, dx4, dy4);

	if (mul > 1) {
		tmp.Format(TEXT("X %lld"), mul);
		int ddx1, ddy1;
		CalcPos(10, 95, ddx1, ddy1);
		pDC->TextOutW(pos.x + ddx1, pos.y + ddy1, tmp);
	}

	if (type == 1) {
		tmp.Format(TEXT("兔子"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
		tmp.Format(TEXT("%d"), 1-cnt);
		pDC->TextOutW(pos.x + DISX, pos.y + DISY, tmp);
	}
	if (type == 2) {
		tmp.Format(TEXT("兔子窝"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 3) {
		tmp.Format(TEXT("狼"));
		pDC->TextOutW(pos.x + dx1, pos.y + dy1, tmp);
	}
	if (type == 4) {
		tmp.Format(TEXT("狼王"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 5) {
		tmp.Format(TEXT("复制"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 6) {
		tmp.Format(TEXT("猫"));
		pDC->TextOutW(pos.x + dx1, pos.y + dy1, tmp);
	}
	if (type == 7) {
		tmp.Format(TEXT("老鼠"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 8) {
		tmp.Format(TEXT("鱼"));
		pDC->TextOutW(pos.x + dx1, pos.y + dy1, tmp);
	}
	if (type == 9) {
		tmp.Format(TEXT("打工人"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 10) {
		tmp.Format(TEXT("主管"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 11) {
		tmp.Format(TEXT("经理"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 12) {
		tmp.Format(TEXT("老板"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 13) {
		tmp.Format(TEXT("首富"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 14) {
		tmp.Format(TEXT("茶杯"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 15) {
		tmp.Format(TEXT("茶"));
		pDC->TextOutW(pos.x + dx1, pos.y + dy1, tmp);
		tmp.Format(TEXT("%d"), 1 - cnt);
		pDC->TextOutW(pos.x + DISX, pos.y + DISY, tmp);
	}
	if (type == 16) {
		tmp.Format(TEXT("茶壶"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
		tmp.Format(TEXT("%d"), 1 - cnt);
		pDC->TextOutW(pos.x + DISX, pos.y + DISY, tmp);
	}
	if (type == 17) {
		tmp.Format(TEXT("奶牛"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 18) {
		tmp.Format(TEXT("肥宅"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 19) {
		tmp.Format(TEXT("草场"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 20) {
		tmp.Format(TEXT("牛奶"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 21) {
		tmp.Format(TEXT("小奶茶"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 22) {
		tmp.Format(TEXT("中奶茶"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 23) {
		tmp.Format(TEXT("大奶茶"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 24) {
		tmp.Format(TEXT("超大奶茶"));
		pDC->TextOutW(pos.x + dx4, pos.y + dy4, tmp);
	}
	if (type == 25) {
		tmp.Format(TEXT("吸管"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 26) {
		tmp.Format(TEXT("大吸管"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 27) {
		tmp.Format(TEXT("茶包"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 28) {
		tmp.Format(TEXT("草料"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 29) {
		tmp.Format(TEXT("铜指针"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 30) {
		tmp.Format(TEXT("银指针"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 31) {
		tmp.Format(TEXT("金指针"));
		pDC->TextOutW(pos.x + dx3, pos.y + dy3, tmp);
	}
	if (type == 32) {
		tmp.Format(TEXT("咖啡"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
		tmp.Format(TEXT("%d"), cnt);
		pDC->TextOutW(pos.x + DISX, pos.y + DISY, tmp);
	}
	if (type == 33) {
		tmp.Format(TEXT("小偷"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
	if (type == 34) {
		tmp.Format(TEXT("警察"));
		pDC->TextOutW(pos.x + dx2, pos.y + dy2, tmp);
	}
}

void Card::Mark(CDC* pDC, int R, int G, int B)
{
	int DISX, DISY;
	CalcPos(5, 5, DISX, DISY);
	CRect tmp_rect(pos.x-DISX, pos.y- DISY, pos.x + CARDLENGTH+ DISX, pos.y + CARDWIDTH+ DISY);
	CBrush Brush(RGB(R, G, B));
	//CBrush* pOldBrush = pDC->SelectObject(&Brush);
	pDC->FillRect(tmp_rect,&Brush);
	//pDC->SelectObject(pOldBrush);
	Brush.DeleteObject();
	Draw(pDC);
}

void Card::MarkYellow(CDC* pDC) {
	Mark(pDC, 255, 185, 15);
}

void Card::MarkGreen(CDC* pDC)
{
	Mark(pDC, 6, 197, 38);
}

void Card::MarkRed(CDC* pDC)
{
	Mark(pDC, 255, 0, 0);
}

void Card::MarkBlue(CDC* pDC)
{
	Mark(pDC, 123, 104, 238);
}

void Card::MarkDarkgreen(CDC* pDC)
{
	Mark(pDC, 46, 139, 87);
}

void Card::MarkPink(CDC* pDC)
{
	Mark(pDC, 255, 105, 180);
}

void Card::DisMark(CDC* pDC)
{
	int DISX, DISY;
	CalcPos(5, 5, DISX, DISY);
	CRect tmp_rect(pos.x - DISX, pos.y - DISY, pos.x + CARDLENGTH + DISX, pos.y + CARDWIDTH + DISY);
	CBrush Brush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&Brush);
	pDC->FillRect(tmp_rect,&Brush);
	pDC->SelectObject(pOldBrush);
	Brush.DeleteObject();
	Draw(pDC);
}

void Card::Create(int Type)
{
	type = Type; dead = false; cnt = 0;
	mul = 1; vis = false; score = 0; initscore = true;
}

BEGIN_MESSAGE_MAP(Card, CDialogEx)
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


// Card 消息处理程序

void Card::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bDragFullWindow;
	SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bDragFullWindow, 0);

	// 禁用拖动时的全窗口刷新
	if (bDragFullWindow)
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);
	}

	CDialog::OnNcLButtonDown(nHitTest, point);

	// 恢复系统设置
	if (bDragFullWindow)
	{
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);
	}
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}
