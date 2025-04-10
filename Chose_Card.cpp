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
	DDX_Control(pDX, IDC_BUTTON1, Show1);
	DDX_Control(pDX, IDC_BUTTON2, Show2);
	DDX_Control(pDX, IDC_BUTTON3, Show3);
}


BEGIN_MESSAGE_MAP(Chose_Card, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &Chose_Card::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Chose_Card::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Chose_Card::OnBnClickedButton3)
END_MESSAGE_MAP()


// Chose_Card 消息处理程序

const int CARD_NUM = 13;
int Choice[3];
const int RARITY[CARD_NUM + 1] = { 0,1,2,1,3,2,1,0,0,0,1,1,2,3 };
const int RSCORE[4] = { 8,6,4,3 };
int score[CARD_NUM+1];

void Chose_Card::DrawCard(CDC* pDC, int type, int sx, int sy, int pos)
{
	CString tmp;
	int tx, ty;
	CSize size;
	int nextX;

	if (type == 1) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("兔子"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("当场上兔子数不超过7，且周围有"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("空格时添加一只兔子，仅触发一"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("次"));
	}

	if (type == 2) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("兔子窝"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("向一个随机空格添加一只兔子"));
	}

	if (type == 3) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("狼"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("吃掉周围的兔子，每只+6"));
	}

	if (type == 4) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("狼王"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("吃掉周围的兔子并且移动过去，"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("获得双倍兔子的基础金币并+6"));
	}

	if (type == 5) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("复制"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("当有卡牌被加入且场上有空位时"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("，额外加入一张相同卡牌"));
	}

	if (type == 6) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("猫"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("吃掉相邻的老鼠和鱼，每个+6"));
	}

	if (type == 7) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("老鼠"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));
	}

	if (type == 8) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("鱼"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));
	}

	if (type == 9) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("打工人"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("场上有3个打工人时合成1个主管"));
	}

	if (type == 10) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("主管"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：1"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("场上有3个主管时合成1个经理"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("吸收相邻打工人的基础金币，每"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("吸收一个自身倍率翻倍"));
	}

	if (type == 11) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("经理"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：2"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("场上有3个经理时合成1个老板"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("吸收相邻打工人和主管的基础金"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("币，每吸收一个自身倍率翻倍"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("有5%概率产生1个打工人"));

	}

	if (type == 12) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("老板"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：3"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("场上有3个老板时合成1个首富"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("吸收相邻打工人、主管、经理"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("的基础金币，每吸收一个自身"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("倍率翻倍"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("有10%概率产生1个打工人"));

	}

	if (type == 13) {
		tx = sx + 50, ty = sy + 50;
		tmp.Format(TEXT("首富"));
		pDC->TextOutW(tx, ty, tmp);

		tx = sx + 20, ty = sy + 150;
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(tx, ty, _T("基础"));
		size = pDC->GetTextExtent(_T("基础"));
		nextX = tx + size.cx;
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(nextX, ty, _T("：0"));

		ty = sy + 200;
		pDC->TextOut(tx, ty, _T("结算时获得等同于最高金币卡"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("牌的金币"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("吸收其他的首富，永久获得其"));
		ty += 20;
		pDC->TextOut(tx, ty, _T("倍率"));

		ty += 40;
		pDC->TextOut(tx, ty, _T("有10%概率产生1个打工人"));

	}
	// 12 + 14
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

	CString tmp;
	tmp.Format(TEXT("金钱：%d"), prt->Money);
	dc.TextOutW(20, 20, tmp);
	
	const int WIDTH = 300;
	const int LENGTH = 600;
	int sx = 100, sy = 50;
	CRect rect(sx, sy, sx + WIDTH, sy + LENGTH);
	dc.Rectangle(rect);
	DrawCard(&dc, Choice[0], sx, sy, 1);

	sx = 500, sy = 50;
	rect = CRect(sx, sy, sx + WIDTH, sy + LENGTH);
	dc.Rectangle(rect);
	DrawCard(&dc, Choice[1], sx, sy, 2);

	sx = 900, sy = 50;
	rect = CRect(sx, sy, sx + WIDTH, sy + LENGTH);
	dc.Rectangle(rect);
	DrawCard(&dc, Choice[2], sx, sy, 3);
}

BOOL Chose_Card::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//获取权重
	srand((unsigned)time(NULL));
	
	for (int i = 1; i <= CARD_NUM; i++) {
		score[i] = RSCORE[RARITY[i]];
	}

	//生成卡牌
	for (int i = 0; i < 3; i++) Choice[i] = SpawnCard();
	
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


void Chose_Card::OnBnClickedButton4()
{
	EndDialog(IDOK);
	// TODO: 在此添加控件通知处理程序代码
}
