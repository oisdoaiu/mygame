// CInfo.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "CInfo.h"


// CInfo 对话框

IMPLEMENT_DYNAMIC(CInfo, CDialogEx)

CInfo::CInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFO, pParent)
{

}

CInfo::~CInfo()
{
}

void CInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELETE, CDelete);
	DDX_Control(pDX, IDOK, CConfirm);
}


BEGIN_MESSAGE_MAP(CInfo, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_DELETE, &CInfo::OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, &CInfo::OnBnClickedOk)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CInfo 消息处理程序

void CInfo::SetPos(int x, int y, int& resx, int& resy) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的大小和位置
	resx = static_cast<int>(screenWidth * (x / 2560.0));
	resy = static_cast<int>(screenHeight * (y / 1600.0));
}

void CInfo::CalcMyFont(CDC* pDC, CFont& resfont) {
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

void CInfo::OnPaint()
{
	CString tmp;
	CButton* pButton = (CButton*)GetDlgItem(IDC_DELETE);
	if (cur.dead) {
		tmp.Format(TEXT("已删除"));
		pButton->SetWindowTextW(tmp);
		pButton->ShowWindow(SW_SHOW);
		pButton->EnableWindow(false);
	}
	else {
		if (Del) {
			tmp.Format(TEXT("删除(剩余%d次)"), prt->delete_num);
			pButton->SetWindowTextW(tmp);
			pButton->ShowWindow(SW_SHOW);
			if (prt->delete_num == 0) pButton->EnableWindow(false);
			else pButton->EnableWindow(true);
		}
		else {
			pButton->ShowWindow(SW_HIDE);
		}
	}
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	int dx1, dy1;
	SetPos(240, 90, dx1, dy1);
	cur.pos = CPoint(dx1, dy1);
	//cur.pos = CPoint(240, 90);
	CalcMyFont(&dc, curfont);
	cur.Draw(&dc);
	dc.SelectObject(curfont);

	int dx2, dy2;
	int dx3, dy3;
	int dx4, dy4;
	int dx5, dy5;
	int dx6, dy6;
	SetPos(20, 50, dx1, dy1);
	SetPos(50, 50, dx2, dy2);
	SetPos(20, 170, dx3, dy3);
	SetPos(20, 220, dx4, dy4);
	SetPos(20, 22, dx5, dy5);
	SetPos(20, 30, dx6, dy6);
	int tx, ty, sx = dx1, sy = dy1;
	CSize size;
	int nextX;

	//兔子
	if (cur.type == 1) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("兔子"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：当场上兔子数不超过7时，周围有空格时添加一只兔子"));

		ty += dy5;
		dc.TextOut(tx, ty, _T("，仅触发一次"));

		ty += dy5;
		ty += dy5;
		dc.SetTextColor(RGB(123, 104, 238));
		dc.TextOut(tx, ty, _T("当前"));
		size = dc.GetTextExtent(_T("当前"));
		nextX = tx + size.cx;
		if (cur.cnt == 0) {
			dc.TextOut(nextX, ty, _T("：未触发"));
		}
		if (cur.cnt == 1) {
			dc.TextOut(nextX, ty, _T("：已触发"));
		}
	}

	//兔子窝
	if (cur.type == 2) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("兔子窝"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：向一个随机空格添加一只兔子"));
	}

	//狼
	if (cur.type == 3) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("狼"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：吃掉周围的兔子，每只+6"));
	}

	//狼王
	if (cur.type == 4) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("狼王"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：吃掉周围的兔子并且移动过去，获得双倍兔子的金钱"));

		ty += dy5;
		dc.TextOut(tx, ty, _T("并+6"));
	}

	//复制
	if (cur.type == 5) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("复制"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：当有卡牌被加入且场上有空位时，额外加入一张相同卡牌"));
	}

	//猫
	if (cur.type == 6) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("猫"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：吃掉相邻的老鼠和鱼，每个+6"));
	}

	//老鼠
	if (cur.type == 7) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("老鼠"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));
	}

	//鱼
	if (cur.type == 8) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("鱼"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));
	}

	//打工人
	if (cur.type == 9) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("打工人"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：场上有3个打工人时合成1个主管"));
	}

	//主管
	if (cur.type == 10) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("主管"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：场上有3个主管时合成1个经理"));

		size = dc.GetTextExtent(_T("效果："));
		nextX = tx + size.cx;
		ty += dy6;
		dc.TextOut(nextX, ty, _T("吸收相邻打工人的金币，每吸收一个自身金币翻倍"));
	}

	//经理
	if (cur.type == 11) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("经理"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：场上有3个经理时合成1个老板"));

		size = dc.GetTextExtent(_T("效果："));
		nextX = tx + size.cx;
		ty += dy6;
		dc.TextOut(nextX, ty, _T("吸收相邻打工人和主管的基础金币，每吸收一个自身倍"));
		ty += dy5;
		dc.TextOut(nextX, ty, _T("率翻倍"));
		ty += dy6;
		dc.TextOut(nextX, ty, _T("有5%概率产生1个打工人"));
	}

	//老板
	if (cur.type == 12) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("老板"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：场上有3个老板时合成1个首富"));

		size = dc.GetTextExtent(_T("效果："));
		nextX = tx + size.cx;
		ty += dy6;
		dc.TextOut(nextX, ty, _T("吸收相邻打工人、主管和经理的基础金币，每吸收一"));
		ty += dy5;
		dc.TextOut(nextX, ty, _T("个自身倍率翻倍"));
		ty += dy6;
		dc.TextOut(nextX, ty, _T("有10%概率产生1个打工人"));
	}

	//首富
	if (cur.type == 13) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("首富"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：0"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：结算时获得等同于最高金币卡牌的金币"));

		size = dc.GetTextExtent(_T("效果："));
		nextX = tx + size.cx;
		ty += dy6;
		dc.TextOut(nextX, ty, _T("吸收其他的首富，永久获得其永久倍率"));
	}

	if (cur.type == 14) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("茶杯"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：每有一杯茶金币+3"));
	}

	if (cur.type == 15) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("茶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：和相邻的牛奶合成小奶茶，留下茶杯"));
	}

	if (cur.type == 16) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("茶壶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：将相邻的茶杯变为茶,每个杯子每回合仅限一次"));
	}

	if (cur.type == 17) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("奶牛"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：附近有草料或草场时产生牛奶"));
	}

	if (cur.type == 18) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("肥宅"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：消耗相邻的奶茶，每消耗一杯奶茶有1%的概率死亡"));
	}

	if (cur.type == 19) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("草场"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：让相邻的奶牛产生牛奶，并让其增加5金币"));
	}

	if (cur.type == 20) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("牛奶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：和相邻的茶合成小奶茶"));
	}

	if (cur.type == 21) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("小奶茶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：和相邻的小奶茶合成中奶茶，被消除后获得10金币"));
	}

	if (cur.type == 22) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("中奶茶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：和相邻的中奶茶合成大奶茶，被消除后获得25金币"));
	}

	if (cur.type == 23) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("大奶茶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：和相邻的大奶茶合成超大奶茶，被消除后获得60金币"));
	}

	if (cur.type == 24) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("超大奶茶"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：5"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：被消除后获得150金币，有40%概率留下一个小奶茶"));
	}

	if (cur.type == 25) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("吸管"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：消除相邻的奶茶，成功消除并结算后消失"));
	}

	if (cur.type == 26) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("大吸管"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：消除相邻的奶茶并将其金币翻倍，成功消除并结算后"));
		ty += dy5;
		size = dc.GetTextExtent(_T("效果："));
		nextX = tx + size.cx;
		dc.TextOut(nextX, ty, _T("有60%概率消失"));
	}

	if (cur.type == 27) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("茶包"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：消除自身将相邻的茶杯变为茶，并让其增加5金币"));
	}

	if (cur.type == 28) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("草料"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：消除自身让相邻的奶牛产生牛奶，并让其增加5金币"));
	}

	if (cur.type == 29) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("铜指针"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：1"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：让周围随机一个格子倍率+3"));
	}

	if (cur.type == 30) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("银指针"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：让周围随机一个格子倍率+6"));
	}

	if (cur.type == 31) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("金指针"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：3"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：让周围随机一个格子倍率+10或翻倍(取最高)"));
	}

	if (cur.type == 32) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("咖啡"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：0"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：让周围角色倍率翻倍，生效3次以后移除自身"));
	}

	if (cur.type == 33) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("小偷"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：0"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：偷走相邻格子一半基础金币"));
	}

	if (cur.type == 34) {
		tx = sx + dx2, ty = sy + dy2;
		tmp.Format(TEXT("警察"));
		dc.TextOutW(tx, ty, tmp);

		tx = sx + dx3, ty = sy + dy3;
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(tx, ty, _T("基础"));
		size = dc.GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：2"));

		tx = sx + dx4, ty = sy + dy4;
		dc.SetTextColor(RGB(255, 185, 15));
		dc.TextOut(tx, ty, _T("效果"));
		size = dc.GetTextExtent(_T("效果"));
		nextX = tx + size.cx;
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(nextX, ty, _T("：消除相邻小偷获得其3倍基础金币"));
	}
}


BOOL CInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CInfo::OnBnClickedDelete()
{
	prt->cards[curid].dead = true;
	prt->delete_num--;
	EndDialog(IDOK);
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int dx1, dy1, dx2, dy2;
	SetPos(200, 200, dx1, dy1);
	SetPos(620, 900, dx2, dy2);
	MoveWindow(dx1, dy1, dx2, dy2);
	SetPos(230, 780, dx1, dy1);
	SetPos(140, 40, dx2, dy2);
	CConfirm.MoveWindow(dx1, dy1, dx2, dy2);
	SetPos(410, 780, dx1, dy1);
	SetPos(160, 40, dx2, dy2);
	CDelete.MoveWindow(dx1, dy1, dx2, dy2);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CInfo::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	/*CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);*/
	CDialogEx::OnRButtonDown(nFlags, point);
}
