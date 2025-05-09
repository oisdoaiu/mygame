// Chose_Card.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "Chose_Card.h"
#include "GameDlg.h"


// Chose_Card 对话框

IMPLEMENT_DYNAMIC(Chose_Card, CDialogEx)

Chose_Card::Chose_Card(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHOSE_CARD, pParent)
{

}

Chose_Card::~Chose_Card()
{
}

void Chose_Card::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, CChoice1);
	DDX_Control(pDX, IDC_BUTTON2, CChoice2);
	DDX_Control(pDX, IDC_BUTTON3, CChoice3);
	DDX_Control(pDX, IDC_SKIP, CSkip);
	DDX_Control(pDX, IDC_REROLL, CReroll);
}


BEGIN_MESSAGE_MAP(Chose_Card, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &Chose_Card::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Chose_Card::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Chose_Card::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_SKIP, &Chose_Card::OnBnClickedSkip)
	ON_BN_CLICKED(IDC_REROLL, &Chose_Card::OnBnClickedReroll)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// Chose_Card 消息处理程序

void Chose_Card::SetPos(int x, int y, int& resx, int& resy) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的大小和位置
	resx = static_cast<int>(screenWidth * (x / 2560.0));
	resy = static_cast<int>(screenHeight * (y / 1600.0));
}

void Chose_Card::CalcMyFont(CDC* pDC, CFont& resfont) {
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

int Choice[3];
const int RARITY[CARD_NUM + 1] = { 0,1,2,1,3,2,1,0,0,0,1,1,2,3,0,0,2,1,3,2,0,0,0,2,3,1,2,0,0,1,2,3,1,1,2 };
const int RSCORE[4] = { 8,6,4,3 };
int score[CARD_NUM+1];

void Chose_Card::DrawCard(CDC* pDC, int type, int sx, int sy, int pos)
{
	CString tmp;
	int tx, ty;
	CSize size;
	int nextX;
	pDC->SetBkColor(RGB(255, 255, 255));
	int dx1, dy1;
	SetPos(50, 50, dx1, dy1);
	int dx2, dy2;
	SetPos(20, 150, dx2, dy2);
	int dx3, dy3;
	SetPos(0, 200, dx3, dy3);
	int dx4, dy4;
	SetPos(0, 24, dx4, dy4);

	pDC->SelectObject(&curfont);
	if (type == 1) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("兔子"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("当场上兔子数不超过7，且周围有"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("空格时添加一只兔子，仅触发一"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("次"));
	}

	if (type == 2) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("兔子窝"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("向一个随机空格添加一只兔子"));
	}

	if (type == 3) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("狼"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("吃掉周围的兔子，每只+6"));
	}

	if (type == 4) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("狼王"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("吃掉周围的兔子并且移动过去，"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("获得双倍兔子的基础金币并+6"));
	}

	if (type == 5) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("复制"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("当有卡牌被加入且场上有空位时"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("，额外加入一张相同卡牌"));
	}

	if (type == 6) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("猫"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("吃掉相邻的老鼠和鱼，每个+6"));
	}

	if (type == 7) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("老鼠"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));
	}

	if (type == 8) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("鱼"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));
	}

	if (type == 9) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("打工人"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("场上有3个打工人时合成1个主管"));
	}

	if (type == 10) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("主管"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("场上有3个主管时合成1个经理"));

		ty += dy4*2;
		pDC->TextOut(tx, ty, _T("吸收相邻打工人的基础金币，每"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("吸收一个自身倍率翻倍"));
	}

	if (type == 11) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("经理"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("场上有3个经理时合成1个老板"));

		ty += dy4*2;
		pDC->TextOut(tx, ty, _T("吸收相邻打工人和主管的基础金"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("币，每吸收一个自身倍率翻倍"));

		ty += dy4*2;
		pDC->TextOut(tx, ty, _T("有5%概率产生1个打工人"));

	}

	if (type == 12) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("老板"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("场上有3个老板时合成1个首富"));

		ty += dy4;
		ty += dy4;
		pDC->TextOut(tx, ty, _T("吸收相邻打工人、主管、经理"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("的基础金币，每吸收一个自身"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("倍率翻倍"));

		ty += dy4;
		ty += dy4;
		pDC->TextOut(tx, ty, _T("有10%概率产生1个打工人"));

	}

	if (type == 13) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("首富"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：0"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("结算时获得等同于最高金币卡"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("牌的金币"));

		ty += dy4;
		ty += dy4;
		pDC->TextOut(tx, ty, _T("吸收其他的首富，永久获得其"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("永久倍率"));

		ty += dy4;
		ty += dy4;
		pDC->TextOut(tx, ty, _T("有10%概率产生1个打工人"));

	}

	if (type == 14) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("茶杯"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("每有一杯茶金币+3"));

	}

	if (type == 15) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("茶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("和相邻的牛奶合成小奶茶，留"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("下茶杯"));
	}

	if (type == 16) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("茶壶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("将相邻的茶杯变为茶，每个杯"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("子每回合仅限一次"));
	}

	if (type == 17) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("奶牛"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("相邻有草场或草料时产生牛奶"));
	}

	if (type == 18) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("肥宅"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("消耗相邻的奶茶，每消耗一杯"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("奶茶有1%的概率死亡"));
	}

	if (type == 19) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("草场"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("让相邻的奶牛产生牛奶，并让"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("其增加5金币"));
	}

	if (type == 20) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("牛奶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("和相邻的茶合成小奶茶"));
	}

	if (type == 21) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("小奶茶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("和相邻的小奶茶合成中奶茶，"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("被消除后获得10金币"));
	}

	if (type == 22) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("中奶茶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("和相邻的中奶茶合成大奶茶，"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("被消除后获得25金币"));
	}

	if (type == 23) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("大奶茶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("和相邻的大奶茶合成超大奶茶"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("，被消除后获得60金币"));
	}

	if (type == 24) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("超大奶茶"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：5"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("被消除后获得150金币，有40%"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("概率留下一个小奶茶"));
	}

	if (type == 25) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("吸管"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("消除相邻的奶茶，成功消除并"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("结算后消失"));
	}

	if (type == 26) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("大吸管"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("消除相邻的奶茶并将其金币翻"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("倍，成功消除并结算后有70%"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("概率消失"));
	}

	if (type == 27) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("茶包"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("消除自身将相邻的茶杯变为"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("茶，并让其增加5金币"));
	}

	if (type == 28) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("草料"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("消除自身让相邻的奶牛产生牛"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("奶，并让其增加5金币"));
	}

	if (type == 29) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("铜指针"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("让周围随机一个格子倍率+3"));
	}

	if (type == 30) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("银指针"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("让周围随机一个格子倍率+6"));
	}

	if (type == 31) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("金指针"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("让周围随机一个格子倍率+10"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("或翻倍(取最高)"));
	}

	if (type == 32) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("咖啡"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("让周围角色倍率翻倍，生效"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("3次以后移除自身"));
	}

	if (type == 33) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("小偷"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：0"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("偷走相邻格子一半基础金币"));
	}

	if (type == 34) {
		tx = sx + dx1, ty = sy + dy1;
		tmp.Format(TEXT("警察"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + dx2, ty = sy + dy2;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + dy3;
		pDC->TextOut(tx, ty, _T("消除相邻小偷获得其3倍基础"));
		ty += dy4;
		pDC->TextOut(tx, ty, _T("金币"));
	}

	// 12 + 14
}

void Chose_Card::Draw(CPaintDC* pDC)
{
	CString tmp;
	int dx1, dy1;
	SetPos(20, 20, dx1, dy1);
	pDC->SelectObject(&curfont);
	pDC->SetBkMode(TRANSPARENT);
	tmp.Format(TEXT("金钱：%lld"), prt->Money);
	pDC->TextOutW(dx1, dy1, tmp);
	pDC->SetBkMode(OPAQUE);

	int WIDTH = 300;
	int LENGTH = 600;
	int sx = 100, sy = 50;
	SetPos(320, 600, WIDTH, LENGTH);
	SetPos(100, 50, sx, sy);
	CRect rect;
	
	DrawRarity(pDC, Choice[0], sx, sy);
	rect = CRect(sx, sy, sx + WIDTH, sy + LENGTH);
	pDC->Rectangle(rect);
	DrawCard(pDC, Choice[0], sx, sy, 1);

	sx = 500, sy = 50;
	SetPos(500, 50, sx, sy);
	DrawRarity(pDC, Choice[1], sx, sy);
	rect = CRect(sx, sy, sx + WIDTH, sy + LENGTH);
	pDC->Rectangle(rect);
	DrawCard(pDC, Choice[1], sx, sy, 2);

	sx = 900, sy = 50;
	SetPos(900, 50, sx, sy);
	DrawRarity(pDC, Choice[2], sx, sy);
	rect = CRect(sx, sy, sx + WIDTH, sy + LENGTH);
	pDC->Rectangle(rect);
	DrawCard(pDC, Choice[2], sx, sy, 3);

	//绘制刷新
	CButton* pButton = (CButton*)GetDlgItem(IDC_REROLL);
	tmp.Format(TEXT("刷新(还有%d次)"), prt->reroll_num);
	pButton->SetWindowTextW(tmp);
	if (prt->reroll_num == 0) pButton->EnableWindow(false);
	else pButton->EnableWindow(true);
}

void Chose_Card::DrawRarity(CDC* pDC, int type, int sx, int sy)
{
	CRect rect;
	int WIDTH = 300;
	int LENGTH = 600;
	SetPos(320, 600, WIDTH, LENGTH);
	int dx1, dy1;
	SetPos(5, 5, dx1, dy1);
	if (RARITY[type] == 0) {
		CBrush greenBrush(RGB(0, 255, 0));
		rect = CRect(sx - dx1, sy - dy1, sx + WIDTH + dx1, sy + LENGTH + dy1);
		pDC->FillRect(&rect, &greenBrush);
	}
	if (RARITY[type] == 1) {
		CBrush yellowBrush(RGB(255, 204, 51));
		rect = CRect(sx - dx1, sy - dy1, sx + WIDTH + dx1, sy + LENGTH + dy1);
		pDC->FillRect(&rect, &yellowBrush);
	}
	if (RARITY[type] == 2) {
		CBrush purpleBrush(RGB(204, 51, 255));
		rect = CRect(sx - dx1, sy - dy1, sx + WIDTH + dx1, sy + LENGTH + dy1);
		pDC->FillRect(&rect, &purpleBrush);
	}
	if (RARITY[type] == 3) {
		CBrush redBrush(RGB(255, 51, 102));
		rect = CRect(sx - dx1, sy - dy1, sx + WIDTH + dx1, sy + LENGTH + dy1);
		pDC->FillRect(&rect, &redBrush);
	}
	
}

int Chose_Card::SpawnCard()
{
	double sum = 0;
	for (int i = 1; i <= CARD_NUM; i++) sum += score[i];
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(0.0, sum);
	double randomValue = dis(gen);
	double cursum = 0;
	for (int i = 1; i <= CARD_NUM; i++) {
		if (cursum <= randomValue && randomValue <= cursum + score[i]) {
			return i;
		}
		cursum += score[i];
	}
	return 0;
}

void Chose_Card::OnPaint()
{
	CPaintDC dc(this);
	CDC* pDC = GetDC();
	CalcMyFont(pDC, curfont);
	Draw(&dc);
}

BOOL Chose_Card::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int dx1, dy1, dx2, dy2;
	SetPos(150, 150, dx1, dy1);
	SetPos(1400, 900, dx2, dy2);
	MoveWindow(dx1, dy1, dx2, dy2);
	//MoveWindow(150, 150, 1400, 800);
	SetPos(180, 670, dx1, dy1);
	SetPos(140, 50, dx2, dy2);
	CChoice1.MoveWindow(dx1, dy1, dx2, dy2);
	//CChoice1.MoveWindow(180, 670, 140, 40);
	SetPos(580, 670, dx1, dy1);
	CChoice2.MoveWindow(dx1, dy1, dx2, dy2);
	//CChoice2.MoveWindow(580, 670, 140, 40);
	SetPos(980, 670, dx1, dy1);
	CChoice3.MoveWindow(dx1, dy1, dx2, dy2);
	//CChoice3.MoveWindow(980, 670, 140, 40);
	SetPos(500, 735, dx1, dy1);
	CSkip.MoveWindow(dx1, dy1, dx2, dy2);
	//CSkip.MoveWindow(500, 715, 140, 30);
	SetPos(650, 735, dx1, dy1);
	CReroll.MoveWindow(dx1, dy1, dx2, dy2);
	//CReroll.MoveWindow(650, 715, 140, 30);

	//获取权重
	srand((unsigned)time(NULL));
	
	for (int i = 1; i <= CARD_NUM; i++) {
		score[i] = RSCORE[RARITY[i]];
	}

	//生成卡牌
	for (int i = 0; i < 3; i++) Choice[i] = SpawnCard();
	Invalidate();
	return TRUE; // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void Chose_Card::OnBnClickedButton1()
{
	Card new_card;
	new_card.Create(Choice[0]);
	prt->cards.push_back(new_card);
	EndDialog(IDOK);
}

void Chose_Card::OnBnClickedButton2()
{
	Card new_card;
	new_card.Create(Choice[1]);
	prt->cards.push_back(new_card);
	EndDialog(IDOK);
}

void Chose_Card::OnBnClickedButton3()
{
	Card new_card;
	new_card.Create(Choice[2]);
	prt->cards.push_back(new_card);
	EndDialog(IDOK);
}


void Chose_Card::OnBnClickedSkip()
{
	EndDialog(IDOK);
	// TODO: 在此添加控件通知处理程序代码
}


void Chose_Card::OnBnClickedReroll()
{
	prt->reroll_num--;
	OnInitDialog();
	// TODO: 在此添加控件通知处理程序代码
}


void Chose_Card::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);*/
	CDialogEx::OnRButtonDown(nFlags, point);
}
