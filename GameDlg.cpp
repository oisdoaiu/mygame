
// GameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Game.h"
#include "GameDlg.h"
#include "CAbility.h"
#include "afxdialogex.h"
#include<cassert>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "Chose_Card.h"
#include "CLibrary.h"
#include "CInfo.h"
#include "CGame_Over.h"


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGameDlg 对话框



CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, CCALC);
	DDX_Control(pDX, IDC_BUTTON2, CCheck);
	DDX_Control(pDX, IDC_BUTTON3, CCheckA);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CGameDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGameDlg::OnBnClickedButton2)
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON3, &CGameDlg::OnBnClickedButton3)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()


// CGameDlg 消息处理程序

int POSX[5], POSY[5], Game_Round;
int SPACE_X = 200, SPACE_Y = 200;
const int SCORE[CARD_NUM+1] = { 0,1,2,1,3,2,1,1,1,1,1,2,3,0,1,1,3,2,3,3,1,1,1,2,5,1,3,1,1,1,2,3,0,0,2 };

void CGameDlg::SetPos(int x, int y, int& resx, int& resy) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算窗口的大小和位置
	resx = static_cast<int>(screenWidth * (x / 2560.0));
	resy = static_cast<int>(screenHeight * (y / 1600.0));
}

void CGameDlg::SetPos(int x, int y, int lx, int ly, int &resx1, int &resy1, int &resx2, int &resy2) {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 计算相对比例
	double relativeWidthRatio = lx / 2560.0; // 预设宽度与预设分辨率的比例
	double relativeHeightRatio = ly / 1600.0; // 预设高度与预设分辨率的比例

	// 计算窗口的大小和位置
	resx2 = static_cast<int>(screenWidth * relativeWidthRatio);
	resy2 = static_cast<int>(screenHeight * relativeHeightRatio);
	resx1 = static_cast<int>(screenWidth * (x / 2560.0));
	resy1 = static_cast<int>(screenHeight * (y / 1600.0));
}

CFont curfont;
BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDC* pDC = GetDC();
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
	//lf.lfWeight = FW_BOLD; // 设置字体为粗体

	//CString tmp;
	//tmp.Format(TEXT("%d %d"), lf.lfHeight, lf.lfWidth);
	//MessageBox(tmp);

	curfont.CreateFontIndirect(&lf);
	int px, py, plx, ply;
	SetPos(100, 100, 1500, 1300, px, py, plx, ply);
	MoveWindow(px, py, plx, ply);

	
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetPos(250, 500, 140, 40, px, py, plx, ply);
	CCheck.MoveWindow(px, py, plx, ply);
	SetPos(250, 550, 140, 40, px, py, plx, ply);
	CCheckA.MoveWindow(px, py, plx, ply);
	SetPos(880, 1150, 140, 40, px, py, plx, ply);
	CCALC.MoveWindow(px, py, plx, ply);
	int START_X = 500, START_Y = 100;
	SetPos(500, 100, START_X, START_Y);
	SetPos(200, 220, SPACE_X, SPACE_Y);
	Cost = 20; 
	Game_Round = 0; 
	memset(Sum, 0, sizeof Sum);
	Total_Money = 0;
	Max_Money = 0;
	Money = 0; 
	cards.clear(); 
	reroll_num = 3;
	delete_num = 1;
	POSX[0] = START_X; for (int i = 1; i < 5; i++) POSX[i] = POSX[i - 1] + SPACE_X;
	POSY[0] = START_Y; for (int i = 1; i < 5; i++) POSY[i] = POSY[i - 1] + SPACE_Y;
	Card new_card;
	new_card.Create(0);
	cards.push_back(new_card);
	
	new_card.Create(6);
	cards.push_back(new_card);
	new_card.Create(9);
	cards.push_back(new_card);
	new_card.Create(32);
	cards.push_back(new_card);
	new_card.Create(14);
	cards.push_back(new_card);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

// 双缓冲绘制
bool START_GAME = true;
void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // 设备上下文
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	
	CString tmp;
	int px, py;
	//显示钱数
	CDC* pDC = GetDC();
	//SetMyFont(pDC);
	pDC->SelectObject(&curfont);
	tmp.Format(TEXT("金钱：%lld"), Money);
	SetPos(250, 300, px, py);
	pDC->TextOutW(px, py, tmp);
	pDC->SetTextColor(RGB(255, 0, 0));
	//SetMyFont(pDC);
	tmp.Format(TEXT("需求：%lld"), Cost);
	SetPos(250, 340, px, py);
	pDC->TextOutW(px, py, tmp);
	pDC->SetTextColor(RGB(0, 0, 0));

	// 获取当前屏幕的 DPI
	int currentDPI = pDC->GetDeviceCaps(LOGPIXELSY);

	// 预设 DPI（对应你的 2560*1600 分辨率和 150% 缩放的情况）
	int defaultDPI = 96 * 1.5; // 标准 DPI 是 96，150% 缩放就是 96 * 1.5

	// 预设字体高度（点）
	int defaultFontHeightPoints = 24;

	// 计算当前字体高度
	int currentFontHeight = -MulDiv(defaultFontHeightPoints, currentDPI, 72);

	// 创建字体
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = currentFontHeight;
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial")); // 设置字体名称

	CFont font;
	font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&font);

	tmp.Format(TEXT("第%d年"), Game_Round / 4 + 1);

	SetPos(240, 80, px, py);
	pDC->TextOut(px, py, tmp);
	switch (Game_Round%4) {
	case 0:
		tmp.Format(TEXT("春"));
		break;
	case 1:
		tmp.Format(TEXT("夏"));
		break;
	case 2:
		tmp.Format(TEXT("秋"));
		break;
	case 3:
		tmp.Format(TEXT("冬"));
		break;
	}
	SetPos(270, 150, px, py);
	pDC->TextOutW(px, py, tmp);
	// 恢复原来的字体
	pDC->SelectObject(pOldFont);

	//绘制游戏区域
	SpawnTable();
	DrawBlocks(pDC);

	//启动一轮计算
	CCALC.EnableWindow(FALSE);
	CCheck.EnableWindow(FALSE);
	CCheckA.EnableWindow(FALSE);
	SetTimer(3, 100, NULL);
}

const int TIMEG = 300;
const int TIMEG2 = 500;
void Wait(int milliseconds) {
	DWORD startTime = GetTickCount64();
	while ((GetTickCount64() - startTime) < milliseconds) {
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGameDlg::DrawBlocks(CDC* pDC)
{
	for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
		cards[card_table[i][j]].pos = CPoint(POSX[i], POSY[j]);
		cards[card_table[i][j]].Draw(pDC);
	}
}

void CGameDlg::SpawnTable()
{
	static int cardpos[25];
	for (int i = 0; i < 25; i++) cardpos[i] = i;
	random_device rd;
	mt19937 gen(rd());
	shuffle(cardpos, cardpos + 25, gen);

	for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) card_table[i][j] = 0;
	shuffle(++cards.begin(), cards.end(), gen);
	int num = min(25, cards.size()-1);
	for (int i = 0; i < num; i++) {
		int curx = cardpos[i] / 5, cury = cardpos[i] % 5;
		card_table[curx][cury] = i+1;
	}
	for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
		cards[card_table[i][j]].score = 0;
		cards[card_table[i][j]].vis = 0;
		cards[card_table[i][j]].mul = 1;
		cards[card_table[i][j]].initscore = true;
		if (GetType(i, j) == 14 || GetType(i, j) == 15) posCard(i, j).cnt = 0;
		if (GetType(i, j) == 13) {
			posCard(i, j).mul = posCard(i, j).cnt+1;
		}
		//特殊处理茶和茶杯标记
	}
}

void CGameDlg::AddScore(int x, int y, ll add, CDC* pDC)
{
	//擦除之前的
	int sx = POSX[x], sy = POSY[y];
	int dx1, dy1, dx2, dy2;
	SetPos(0, -70, dx1, dy1);
	SetPos(100, -20, dx2, dy2);
	CRect rect(sx, sy + dy1, sx + dx2, sy + dy2);
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->FillRect(rect, &whiteBrush);
	pDC->SelectObject(pOldBrush);
	whiteBrush.DeleteObject();

	//绘制新分数
	GetScore(x, y, pDC);
	cards[card_table[x][y]].score += add;
	InitScore(x, y, cards[card_table[x][y]].score, pDC);

	//绘制加数
	CString tmp;
	if (add > 0) {
		tmp.Format(TEXT("+ %lld"), add);
		SetPos(8, -70, dx1, dy1);
		pDC->TextOutW(sx + dx1, sy + dy1, tmp);
	}
	else if (add < 0) {
		tmp.Format(TEXT("- %lld"), -add);
		SetPos(8, -70, dx1, dy1);
		pDC->TextOutW(sx + dx1, sy + dy1, tmp);
	}
}

void CGameDlg::MulScore(int x, int y, ll mul, CDC* pDC)
{
	//擦除之前的
	int sx = POSX[x], sy = POSY[y];
	int dx1, dy1, dx2, dy2;
	SetPos(0, -70, dx1, dy1);
	SetPos(100, -20, dx2, dy2);
	CRect rect(sx, sy + dy1, sx + dx2, sy + dy2);
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->FillRect(rect, &whiteBrush);
	pDC->SelectObject(pOldBrush);
	whiteBrush.DeleteObject();

	//绘制新分数
	GetScore(x, y, pDC);
	cards[card_table[x][y]].score *= mul;
	InitScore(x, y, cards[card_table[x][y]].score, pDC);

	//绘制加数
	CString tmp;
	tmp.Format(TEXT("X %lld"), mul);
	SetPos(5, -60, dx1, dy1);
	pDC->TextOutW(sx + dx1, sy + dy1, tmp);
}

void CGameDlg::InitScore(int x, int y, ll score, CDC* pDC)
{
	CString tmp;
	tmp.Format(TEXT("%lld"), score);
	int dx1, dy1;
	SetPos(5, -40, dx1, dy1);
	pDC->TextOutW(POSX[x] + dx1, POSY[y] + dy1, tmp);
	cards[card_table[x][y]].initscore = false;
	cards[card_table[x][y]].score = score;
}

void CGameDlg::ClearScore(int x, int y, CDC* pDC)
{
	int sx = POSX[x], sy = POSY[y];
	int dx1, dx2, dy1, dy2;
	SetPos(0, -70, dx1, dy1);
	SetPos(100, -20, dx2, dy2);
	CRect rect(sx, sy + dy1, sx + dx2, sy + dy2);
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->FillRect(rect, &whiteBrush);
	pDC->SelectObject(pOldBrush);
	whiteBrush.DeleteObject();
}

ll CGameDlg::GetScore(int x, int y, CDC* pDC)
{
	Card& cur = cards[card_table[x][y]];
	if (cur.initscore) {
		cur.initscore = false;
		InitScore(x, y, SCORE[cur.type], pDC);
	}
	return cur.score;
}

//0加1乘
void CGameDlg::AddMul(int x, int y, ll mul, char opt, CDC* pDC)
{
	int sx = POSX[x], sy = POSY[y];
	int dx1, dx2, dy1, dy2;
	SetPos(5, 85, dx1, dy1);
	SetPos(90, 100, dx2, dy2);
	CRect rect(sx + dx1, sy + dy1, sx + dx2, sy + dy2);
	//CRect rect(sx + 5, sy + 65, sx + 90, sy + 90);
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->FillRect(rect, &whiteBrush);
	pDC->SelectObject(pOldBrush);
	whiteBrush.DeleteObject();

	if (opt == 1)
		cards[card_table[x][y]].mul *= mul;
	if (opt == 0)
		cards[card_table[x][y]].mul += mul;
	/*
	CString tmp; tmp.Format(TEXT("X %lld"), cards[card_table[x][y]].mul);
	SetPos(10, 70, dx1, dy1);
	pDC->TextOutW(sx + dx1, sy + dy1, tmp);*/
}

void CGameDlg::Eat(int sx, int sy, int tx, int ty, CDC* pDC)
{
	cards[card_table[tx][ty]].MarkRed(pDC);
	cards[card_table[sx][sy]].MarkYellow(pDC);
	Wait(TIMEG);

	cards[card_table[tx][ty]].dead = true;
	GetScore(tx, ty, pDC);
	int add_score = cards[card_table[tx][ty]].score;
	add_score *= 2;
	add_score += 6;

	cards[0].pos = cards[card_table[sx][sy]].pos;
	cards[card_table[sx][sy]].pos = cards[card_table[tx][ty]].pos;
	card_table[tx][ty] = card_table[sx][sy];
	card_table[sx][sy] = 0;
	cards[card_table[tx][ty]].MarkYellow(pDC);
	cards[card_table[sx][sy]].DisMark(pDC);
	ClearScore(sx, sy, pDC);
	AddScore(tx, ty, add_score, pDC);
	Wait(TIMEG);

	cards[card_table[tx][ty]].DisMark(pDC);
}

int CGameDlg::GetType(int x, int y)
{
	return cards[card_table[x][y]].type;
}

void CGameDlg::UpdateCard()
{
	vector<Card>new_deck;
	for (Card x : cards)
		if (x.dead == false)
			new_deck.push_back(x);
	cards = new_deck;
}

//卡(x,y)要添加卡，添加类型为type，opt=0全局，=1周围，是否复制为copy，返回是否添加成功
bool CGameDlg::AddCard(int x, int y, int type, int opt, bool copy, CDC* pDC)
{
	if (opt == 0) {
		CPoint np = GetRandomBlock(-1, -1);
		if (np.x == -1) return false;
		AddCard(np.x, np.y, type, copy, pDC);
	}
	if (opt == 1) {
		CPoint np = GetRandomBlock(x, y);
		if (np.x == -1) return false;
		AddCard(np.x, np.y, type, copy, pDC);
	}
	return true;
}

//在(x,y)添加卡，标绿，复制出的标蓝，copy表示是否考虑复制格
void CGameDlg::AddCard(int x, int y, int type, bool copy, CDC* pDC)
{
	Card ncard;
	ncard.Create(type);
	ncard.pos = CPoint(POSX[x], POSY[y]);
	
	int startpos = cards.size();
	if (type == 1) {
		ncard.vis = true;
	}

	cards.push_back(ncard);
	card_table[x][y] = cards.size() - 1;

	ncard.MarkGreen(pDC);
	Wait(TIMEG);

	if (copy) {
		int repeat_num = 0;
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
			if (GetType(i, j) == 5) repeat_num++;
		while (repeat_num-- > 0) {
			CPoint new_pos = GetRandomBlock(-1, -1);
			if (new_pos.x == -1) break;
			ncard.pos = CPoint(POSX[new_pos.x], POSY[new_pos.y]);
			if (type == 1) {
				ncard.vis = true;
			}
			cards.push_back(ncard);
			card_table[new_pos.x][new_pos.y] = cards.size() - 1;
			ncard.MarkBlue(pDC);
			Wait(TIMEG);
		}
	}

	for (int i = startpos; i < cards.size(); i++)
		cards[i].DisMark(pDC);
}

//把加卡的卡牌标黄，返回是否添加成功
bool CGameDlg::AddCardDraw(int x, int y, int type, int opt, bool copy, CDC* pDC)
{
	int cur = card_table[x][y];
	CPoint new_card;
	if (opt == 0) new_card = GetRandomBlock(-1, -1);
	if (opt == 1) new_card = GetRandomBlock(x, y);
	if (new_card.x == -1) return false;
	cards[cur].MarkYellow(pDC);
	Wait(TIMEG);
	AddCard(x, y, type, opt, copy, pDC);
	cards[cur].DisMark(pDC);
	return true;
}

bool CGameDlg::IsType(int x, int y, vector<int> type)
{
	for (int t : type) if (GetType(x, y) == t) return true;
	return false;
}

void CGameDlg::DelCardDraw(int x, int y, CDC* pDC)
{
	cards[card_table[x][y]].dead = true;
	cards[card_table[x][y]].MarkRed(pDC);
	Wait(TIMEG);
	ClearScore(x, y, pDC);
	cards[0].pos = cards[card_table[x][y]].pos;
	cards[0].DisMark(pDC);
	card_table[x][y] = 0;
}

void CGameDlg::DelCard(int x, int y, CDC* pDC) {
	cards[card_table[x][y]].dead = true;
	cards[0].pos = cards[card_table[x][y]].pos;
	ClearScore(x, y, pDC);
	cards[0].DisMark(pDC);
	card_table[x][y] = 0;
}

CPoint CGameDlg::DrawRandom(vector<CPoint> x, CDC* pDC)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> rnum(10,15);
	int num = rnum(mt);
	uniform_int_distribution<int> rs(0, x.size()-1);
	int s = rs(mt);
	Card dtmp;
	while(num --> 0) {
		s++;
		if (s == x.size()) s = 0;
		posCard(x[s]).MarkGreen(pDC);
		Wait(400 / (num*2 + 1));
		if (num) posCard(x[s]).DisMark(pDC);
	}
	return x[s];
}

bool CGameDlg::CheckChance(double p)
{
	srand((unsigned)time(NULL));
	double randomValue = (double)rand() / (RAND_MAX + 1.0);
	return randomValue < p;
}

//以(x,y)为中心，是否相邻
vector<CPoint> CGameDlg::GetCards(int x, int y, char opt)
{
	const int dx[8] = { -1,-1,-1,0,0,1,1,1 };
	const int dy[8] = { -1,0,1,-1,1,-1,0,1 };
	vector<CPoint> res;
	if (opt == 1) {
		for (int d = 0; d < 8; d++) {
			int tx = x + dx[d], ty = y + dy[d];
			if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5 && card_table[tx][ty]) {
				res.push_back(CPoint(tx, ty));
			}
		}
	}
	return res;
}

//(x,y)周围type卡，x=-1时取全局
vector<CPoint> CGameDlg::GetTypeCards(int x, int y, int type)
{
	const int dx[8] = { -1,-1,-1,0,0,1,1,1 };
	const int dy[8] = { -1,0,1,-1,1,-1,0,1 };
	vector<CPoint> res;
	if (x != -1) {
		for (int d = 0; d < 8; d++) {
			int tx = x + dx[d], ty = y + dy[d];
			if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5 && GetType(tx, ty) == type) {
				res.push_back(CPoint(tx, ty));
			}
		}
	}
	if (x == -1) {
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
			if (GetType(i, j) == type)
				res.push_back(CPoint(i, j));
		}
	}
	return res;
}

//(x,y)周围type卡，x=-1时取全局
vector<CPoint> CGameDlg::GetTypeCards(int x, int y, vector<int> type)
{
	const int dx[8] = { -1,-1,-1,0,0,1,1,1 };
	const int dy[8] = { -1,0,1,-1,1,-1,0,1 };
	vector<CPoint> res;
	if (x != -1) {
		for (int d = 0; d < 8; d++) {
			int tx = x + dx[d], ty = y + dy[d];
			if (tx >= 0 && tx < 5 && ty >= 0 && ty < 5 && IsType(tx, ty, type)) {
				res.push_back(CPoint(tx, ty));
			}
		}
	}
	if (x == -1) {
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
			if (IsType(i, j, type))
				res.push_back(CPoint(i, j));
		}
	}
	return res;
	return vector<CPoint>();
}

//(x,y)周围随机一个卡，x=-1取全局
CPoint CGameDlg::GetRandomBlock(int x, int y)
{
	CPoint q[25]; int num=0;
	if (x == -1) {
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) if (card_table[i][j] == 0) {
			q[num++] = CPoint(i, j);
		}
	}
	else {
		const int dx[8] = { -1,-1,-1,0,0,1,1,1 };
		const int dy[8] = { -1,0,1,-1,1,-1,0,1 };
		for (int d = 0; d < 8; d++) {
			int tx = x + dx[d], ty = y + dy[d];
			if (tx < 0 || tx >= 5 || ty < 0 || ty >= 5 || card_table[tx][ty] != 0) continue;
			q[num++] = CPoint(tx, ty);
		}
	}
	if (num == 0) return CPoint(-1, -1);
	return q[rand()%num];
}


/*
	计算顺序：
	按坐标顺序，每一次找一个可以处理的处理，然后重复
*/

void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	const int dx[8] = { -1,-1,-1,0,0,1,1,1 };
	const int dy[8] = { -1,0,1,-1,1,-1,0,1 };
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDC* pDC = GetDC();
	pDC->SelectObject(curfont);

#define In (tx >= 0 && tx < 5 && ty >= 0 && ty < 5)

	if (nIDEvent == 3) { //开始计算
		KillTimer(3);
		SetTimer(1, 1000, NULL);
	}

	if (nIDEvent == 1) { //寻找并处理
		KillTimer(1);
		bool Updated = false;


		for (int i = 0; i < 5 && !Updated; i++) for (int j = 0; j < 5 && !Updated; j++){
			//判定动画
			if (card_table[i][j]) {
				posCard(i, j).MarkPink(pDC);
			}
			else {
				cards[0].pos = CPoint(POSX[i], POSY[j]);
				cards[0].MarkPink(pDC);
			}
			Wait(30);
			if (card_table[i][j]) {
				posCard(i, j).DisMark(pDC);
			}
			else {
				cards[0].pos = CPoint(POSX[i], POSY[j]);
				cards[0].DisMark(pDC);
			}

			if (card_table[i][j]) {
				int cur = card_table[i][j];


				//兔子
				if (cards[cur].type == 1) {
					if (cards[cur].cnt == 1) continue;
					if (cards[cur].vis) continue;
					if (GetTypeCards(-1, -1, 1).size() > 7) continue;
					if (AddCardDraw(i, j, 1, 1, 1, pDC)) {
						Updated = true;
						cards[cur].cnt = 1;
					}
				}

				//兔子窝
				if (cards[cur].type == 2) {
					if (cards[cur].vis == true) continue;
					if (AddCardDraw(i, j, 1, 0, 1, pDC)) {
						cards[cur].vis = true;
						Updated = true;
					}
				}

				//狼
				if (cards[cur].type == 3) {

					vector<CPoint> eats = GetTypeCards(i, j, 1);
					if (eats.empty()) continue;
					GetScore(i, j, pDC);
					cards[cur].MarkYellow(pDC);
					for (CPoint rab : eats) {
						AddScore(i, j, 6, pDC);
						DelCardDraw(rab.x, rab.y, pDC);
					}
					cards[cur].DisMark(pDC);
					Updated = true;
					break;
				}

				//狼王
				if (cards[cur].type == 4) {
					while (1) {
						int px = -1, py = -1;
						for (int d = 0; d < 8; d++) {
							int tx = i + dx[d], ty = j + dy[d];
							if (In && GetType(tx, ty) == 1) {
								px = tx, py = ty;
								break;
							}
						}
						if (px == -1) break;
						GetScore(i, j, pDC);
						Eat(i, j, px, py, pDC);
						i = px, j = py;
						Updated = true;
					}
					if (Updated) break;
				}

				//猫
				if (cards[cur].type == 6) {
					vector<CPoint> eats = GetTypeCards(i, j, { 7,8 });
					if (eats.empty()) continue;
					GetScore(i, j, pDC);
					cards[cur].MarkYellow(pDC);
					for (auto tc : eats) {
						AddScore(i, j, 6, pDC);
						DelCardDraw(tc.x, tc.y, pDC);
						cards[cur].DisMark(pDC);
						Updated = true;
						break;
					}
				}

				//主管
				if (cards[cur].type == 10) {
					vector<CPoint> tmp = GetTypeCards(i, j, { 9 });
					for (CPoint v : tmp) {
						if (GetScore(v.x, v.y, pDC)) {
							Updated = true;
							AddScore(i, j, cards[card_table[v.x][v.y]].score, pDC);
							AddMul(i, j, 2, 1, pDC);
							InitScore(v.x, v.y, 0, pDC);
							cards[card_table[v.x][v.y]].MarkGreen(pDC);
							cards[cur].MarkYellow(pDC);
							Wait(TIMEG);
							cards[card_table[v.x][v.y]].DisMark(pDC);
						}
					}
					cards[cur].DisMark(pDC);
				}

				//经理
				if (cards[cur].type == 11) {
					vector<CPoint> tmp = GetTypeCards(i, j, { 9,10 });
					for (CPoint v : tmp) {
						if (GetScore(v.x, v.y, pDC)) {
							Updated = true;
							AddScore(i, j, cards[card_table[v.x][v.y]].score, pDC);
							AddMul(i, j, 2, 1, pDC);
							InitScore(v.x, v.y, 0, pDC);
							cards[card_table[v.x][v.y]].MarkGreen(pDC);
							cards[cur].MarkYellow(pDC);
							Wait(TIMEG);
							cards[card_table[v.x][v.y]].DisMark(pDC);
						}
					}
					bool added = false;
					cards[cur].DisMark(pDC);
					if (cards[cur].vis == false) {
						cards[cur].vis = true;
						if (CheckChance(0.05)) {
							AddCardDraw(i, j, 9, 0, 1, pDC);
							added = true;
							Updated = true;
						}
					}
					if (added) break;
				}

				//老板
				if (cards[cur].type == 12) {
					vector<CPoint> tmp = GetTypeCards(i, j, { 9,10,11 });
					for (CPoint v : tmp) {
						if (GetScore(v.x, v.y, pDC)) {
							Updated = true;
							AddScore(i, j, cards[card_table[v.x][v.y]].score, pDC);
							AddMul(i, j, 2, 1, pDC);
							InitScore(v.x, v.y, 0, pDC);
							cards[card_table[v.x][v.y]].MarkGreen(pDC);
							cards[cur].MarkYellow(pDC);
							Wait(TIMEG);
							cards[card_table[v.x][v.y]].DisMark(pDC);
						}
					}
					bool added = false;
					cards[cur].DisMark(pDC);
					if (cards[cur].vis == false) {
						cards[cur].vis = true;
						if (CheckChance(0.1)) {
							AddCardDraw(i, j, 9, 0, 1, pDC);
							added = true;
							Updated = true;
						}
					}
					if (added) break;
				}

				//首富
				if (cards[cur].type == 13) {
					vector<CPoint> tmp = GetTypeCards(-1, -1, 13);
					if (tmp.size() == 1) continue;
					cards[cur].MarkYellow(pDC);
					AddMul(i, j, cards[cur].cnt + 1, 1, pDC);
					Updated = true;
					for (CPoint v : tmp) {
						if (CPoint(i, j) == v) continue;
						cards[card_table[v.x][v.y]].MarkGreen(pDC);
						Wait(TIMEG);
						int dlt = cards[card_table[v.x][v.y]].cnt + 1;
						DelCard(v.x, v.y, pDC);
						cards[cur].cnt += dlt;
						AddMul(i, j, dlt, 0, pDC);
					}
					cards[cur].DisMark(pDC);
				}

				//茶杯
				if (cards[cur].type == 14) {
					if (cards[cur].vis) continue;
					cards[cur].vis = true;
					vector<CPoint> tmp = GetTypeCards(-1, -1, 15);
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					Wait(TIMEG);
					for (CPoint v : tmp)
						posCard(v).MarkGreen(pDC),
						AddScore(i, j, 3, pDC),
						Wait(100);
					Wait(TIMEG);
					for (CPoint v : tmp)
						posCard(v).DisMark(pDC);
					cards[cur].DisMark(pDC);
					break;
				}

				//茶
				if (cards[cur].type == 15) {
					vector<CPoint> tmp = GetTypeCards(i, j, 20);
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					posCard(tmp[0]).MarkYellow(pDC);
					Wait(TIMEG);
					DelCard(tmp[0].x, tmp[0].y, pDC);
					cards[cur].type = 14;
					cards[cur].MarkYellow(pDC);
					AddCard(-1, -1, 21, 0, false, pDC);
					cards[cur].DisMark(pDC);
					break;
				}

				//茶壶
				if (cards[cur].type == 16) {
					vector<CPoint> tmp = GetTypeCards(i, j, 14);
					vector<CPoint> upd;
					for (CPoint v : tmp) if (posCard(v).cnt == 0) upd.push_back(v);
					if (upd.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					Wait(TIMEG);
					for (CPoint v : upd) {
						Card& cur = posCard(v);
						cur.cnt++;
						cur.type = 15;
						cur.MarkGreen(pDC);
						Wait(200);
						cur.DisMark(pDC);
					}
					cards[cur].DisMark(pDC);
					break;
				}

				//肥宅
				if (cards[cur].type == 18) {
					vector<CPoint> tmp = GetTypeCards(i, j, { 21,22,23,24 });
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					Wait(TIMEG);
					char dead = false;
					for (CPoint v : tmp) {
						Card& t = posCard(v);
						if (t.type == 21) {
							AddScore(i, j, 10, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 22) {
							AddScore(i, j, 25, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 23) {
							AddScore(i, j, 60, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 24) {
							AddScore(i, j, 150, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
							if (CheckChance(0.05)) {
								AddCard(-1, -1, 21, 0, false, pDC);
							}
						}
						if (CheckChance(0.01)) {
							DelCardDraw(i, j, pDC);
							dead = true;
							break;
						}
					}
					if (!dead)
						cards[cur].DisMark(pDC);
					break;
				}

				//草场
				if (cards[cur].type == 19) {
					if (cards[cur].vis) continue;
					cards[cur].vis = true;
					vector<CPoint> tmp = GetTypeCards(i, j, 17);
					if (tmp.empty()) continue;
					if (GetRandomBlock(-1, -1).x == -1) continue;
					Updated = true;
					cards[cur].MarkDarkgreen(pDC);
					Wait(TIMEG);
					for (CPoint v : tmp) {
						AddScore(v.x, v.y, 5, pDC);
						AddCardDraw(v.x, v.y, 20, 0, true, pDC);
					}
					cards[cur].DisMark(pDC);
					break;
				}

				//牛奶
				if (cards[cur].type == 20) {
					vector<CPoint> tmp = GetTypeCards(i, j, 15);
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					posCard(tmp[0]).MarkYellow(pDC);
					Wait(TIMEG);
					DelCard(tmp[0].x, tmp[0].y, pDC);
					cards[cur].type = 14;
					cards[cur].MarkYellow(pDC);
					AddCard(-1, -1, 21, 0, false, pDC);
					cards[cur].DisMark(pDC);
					break;
				}

				//小奶茶
				if (cards[cur].type == 21) {
					vector<CPoint> tmp = GetTypeCards(i, j, 21);
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					posCard(tmp[0]).MarkYellow(pDC);
					Wait(TIMEG);
					DelCard(tmp[0].x, tmp[0].y, pDC);
					DelCard(i, j, pDC);
					AddCard(-1, -1, 22, 0, false, pDC);
					break;
				}

				//中奶茶
				if (cards[cur].type == 22) {
					vector<CPoint> tmp = GetTypeCards(i, j, 22);
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					posCard(tmp[0]).MarkYellow(pDC);
					Wait(TIMEG);
					DelCard(tmp[0].x, tmp[0].y, pDC);
					DelCard(i, j, pDC);
					AddCard(-1, -1, 23, 0, false, pDC);
					break;
				}

				//大奶茶
				if (cards[cur].type == 23) {
					vector<CPoint> tmp = GetTypeCards(i, j, 23);
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					posCard(tmp[0]).MarkYellow(pDC);
					Wait(TIMEG);
					DelCard(tmp[0].x, tmp[0].y, pDC);
					DelCard(i, j, pDC);
					AddCard(-1, -1, 24, 0, false, pDC);
					break;
				}

				//吸管
				if (cards[cur].type == 25) {
					vector<CPoint> tmp = GetTypeCards(i, j, { 21,22,23,24 });
					if (tmp.empty()) continue;
					cards[cur].vis = true;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					Wait(TIMEG);
					for (CPoint v : tmp) {
						Card& t = posCard(v);
						if (t.type == 21) {
							AddScore(i, j, 10, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 22) {
							AddScore(i, j, 25, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 23) {
							AddScore(i, j, 60, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 24) {
							AddScore(i, j, 150, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
							if (CheckChance(0.05)) {
								AddCard(-1, -1, 21, 0, false, pDC);
							}
						}
					}
					cards[cur].DisMark(pDC);
					break;
				}

				//大吸管
				if (cards[cur].type == 26) {
					vector<CPoint> tmp = GetTypeCards(i, j, { 21,22,23,24 });
					if (tmp.empty()) continue;
					cards[cur].vis = true;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					Wait(TIMEG);
					for (CPoint v : tmp) {
						Card& t = posCard(v);
						if (t.type == 21) {
							AddScore(i, j, 20, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 22) {
							AddScore(i, j, 50, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 23) {
							AddScore(i, j, 120, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
						}
						if (t.type == 24) {
							AddScore(i, j, 300, pDC);
							DelCardDraw(v.x, v.y, pDC);
							Wait(TIMEG);
							if (CheckChance(0.05)) {
								AddCard(-1, -1, 21, 0, false, pDC);
							}
						}
					}
					cards[cur].DisMark(pDC);
					break;
				}

				//茶包
				if (cards[cur].type == 27) {
					vector<CPoint> upd = GetTypeCards(i, j, 14);
					if (upd.empty()) continue;
					Updated = true;
					DelCardDraw(i, j, pDC);
					Wait(TIMEG);
					for (CPoint v : upd) {
						Card& cur = posCard(v);
						cur.type = 15;
						cur.MarkGreen(pDC);
						Wait(200);
						cur.DisMark(pDC);
					}
					break;
				}

				//草料
				if (cards[cur].type == 28) {
					vector<CPoint> tmp = GetTypeCards(i, j, 17);
					if (tmp.empty()) continue;
					Updated = true;
					DelCardDraw(i, j, pDC);
					Wait(TIMEG);
					for (CPoint v : tmp) {
						AddScore(v.x, v.y, 5, pDC);
						AddCardDraw(v.x, v.y, 20, 0, true, pDC);
					}
					break;
				}

				//铜指针
				if (cards[cur].type == 29) {
					if (cards[cur].vis) continue;
					vector<CPoint> tmp = GetCards(i, j, 1);
					if (tmp.empty()) continue;
					cards[cur].vis = true;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					CPoint upd = DrawRandom(tmp, pDC);
					AddMul(upd.x, upd.y, 3, 0, pDC);
					posCard(upd).DisMark(pDC);
					cards[cur].DisMark(pDC);
				}

				//银指针
				if (cards[cur].type == 30) {
					if (cards[cur].vis) continue;
					vector<CPoint> tmp = GetCards(i, j, 1);
					if (tmp.empty()) continue;
					cards[cur].vis = true;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					CPoint upd = DrawRandom(tmp, pDC);
					AddMul(upd.x, upd.y, 6, 0, pDC);
					posCard(upd).DisMark(pDC);
					cards[cur].DisMark(pDC);
				}

				//金指针
				if (cards[cur].type == 31) {
					if (cards[cur].vis) continue;
					vector<CPoint> tmp = GetCards(i, j, 1);
					if (tmp.empty()) continue;
					cards[cur].vis = true;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					CPoint upd = DrawRandom(tmp, pDC);
					if(posCard(upd).mul<=10)
						AddMul(upd.x, upd.y, 10, 0, pDC);
					else
						AddMul(upd.x, upd.y, 2, 1, pDC);
					posCard(upd).DisMark(pDC);
					cards[cur].DisMark(pDC);
				}

				//咖啡
				if (cards[cur].type == 32) {
					if (cards[cur].vis) continue;
					if (cards[cur].cnt == 3) continue;
					vector<CPoint> tmp = GetTypeCards(i, j, { 9,10,11,12,13,18,33,34 });
					if (tmp.empty()) continue;
					Updated = true;
					cards[cur].vis = true;
					cards[cur].cnt++;
					cards[cur].MarkYellow(pDC);
					Wait(200);
					for (CPoint v : tmp) {
						AddMul(v.x, v.y, 2, 1, pDC);
						posCard(v).MarkGreen(pDC);
					}
					Wait(500);
					cards[cur].DisMark(pDC);
					for (CPoint v : tmp) {
						posCard(v).DisMark(pDC);
					}
					if (cards[cur].cnt == 3) DelCard(i, j, pDC);
				}

				//小偷
				if (cards[cur].type == 33) {
					if (cards[cur].vis) continue;
					vector<CPoint> tmp = GetCards(i, j, 1);
					if (tmp.empty()) continue;
					cards[cur].vis = true;
					Updated = true;
					cards[cur].MarkYellow(pDC);
					Wait(200);
					ll sum = 0;
					for (CPoint v : tmp) {
						ll val = GetScore(v.x, v.y, pDC);
						sum += val / 2;
						AddScore(v.x, v.y, -val / 2, pDC);
						posCard(v).MarkGreen(pDC);
					}
					AddScore(i, j, sum, pDC);
					Wait(300);
					for (CPoint v : tmp) {
						posCard(v).DisMark(pDC);
					}
					cards[cur].DisMark(pDC);
				}
			}
		}
		if (Updated) {
			SetTimer(1, 500, NULL);
			return;
		}

		//后检测合成

		//3打工人合成1主管
		if (GetTypeCards(-1, -1, 9).size() >= 3) {

			vector<CPoint>tmp = GetTypeCards(-1, -1, 9);
			//标出3个打工人
			int base = 0, mul = 0;
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
				base += GetScore(mx, my, pDC);
				mul += posCard(mx, my).mul - 1;
			}
			Wait(TIMEG2);

			//删掉打工人，合成主管
			for (int i = 0; i < 3; i++) {
				int tx = tmp[i].x, ty = tmp[i].y;
				cards[card_table[tx][ty]].dead = true;
				card_table[tx][ty] = 0;
				ClearScore(tx, ty, pDC);
				cards[0].pos = CPoint(POSX[tx], POSY[ty]);
				cards[0].DisMark(pDC);
			}
			Wait(200);

			Card ncard;
			ncard.Create(10);
			CPoint npos = GetRandomBlock(-1, -1);
			ncard.pos = CPoint(POSX[npos.x], POSY[npos.y]);
			cards.push_back(ncard);
			card_table[npos.x][npos.y] = cards.size() - 1;
			InitScore(npos.x, npos.y, base, pDC);
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);
			AddMul(npos.x, npos.y, mul, 0, pDC);

			SetTimer(1, 500, NULL);
			return;
		}

		//3主管合成1经理
		if (GetTypeCards(-1, -1, 10).size() >= 3) {

			vector<CPoint>tmp = GetTypeCards(-1, -1, 10);
			//标出3个主管
			int base = 0, mul = 0;
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
				base += GetScore(mx, my, pDC);
				mul += posCard(mx, my).mul - 1;
			}
			Wait(TIMEG2);

			//删掉主管，合成经理
			for (int i = 0; i < 3; i++) {
				int tx = tmp[i].x, ty = tmp[i].y;
				cards[card_table[tx][ty]].dead = true;
				card_table[tx][ty] = 0;
				ClearScore(tx, ty, pDC);
				cards[0].pos = CPoint(POSX[tx], POSY[ty]);
				cards[0].DisMark(pDC);
			}
			Wait(200);

			Card ncard;
			ncard.Create(11);
			CPoint npos = GetRandomBlock(-1, -1);
			ncard.pos = CPoint(POSX[npos.x], POSY[npos.y]);
			cards.push_back(ncard);
			card_table[npos.x][npos.y] = cards.size() - 1;
			InitScore(npos.x, npos.y, base, pDC);
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);
			AddMul(npos.x, npos.y, mul, 0, pDC);

			SetTimer(1, 500, NULL);
			return;
		}

		//3经理合成1老板
		if (GetTypeCards(-1, -1, 11).size() >= 3) {

			vector<CPoint>tmp = GetTypeCards(-1, -1, 11);
			//标出3个经理
			int base = 0, mul = 0;
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
				base += GetScore(mx, my, pDC);
				mul += posCard(mx, my).mul - 1;
			}
			Wait(TIMEG2);

			//删掉经理，合成老板
			for (int i = 0; i < 3; i++) {
				int tx = tmp[i].x, ty = tmp[i].y;
				cards[card_table[tx][ty]].dead = true;
				card_table[tx][ty] = 0;
				ClearScore(tx, ty, pDC);
				cards[0].pos = CPoint(POSX[tx], POSY[ty]);
				cards[0].DisMark(pDC);
			}
			Wait(200);

			Card ncard;
			ncard.Create(12);
			CPoint npos = GetRandomBlock(-1, -1);
			ncard.pos = CPoint(POSX[npos.x], POSY[npos.y]);
			cards.push_back(ncard);
			card_table[npos.x][npos.y] = cards.size() - 1;
			InitScore(npos.x, npos.y, base, pDC);
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);
			AddMul(npos.x, npos.y, mul, 0, pDC);

			SetTimer(1, 500, NULL);
			return;
		}

		//3老板合成1首富
		if (GetTypeCards(-1, -1, 12).size() >= 3) {

			vector<CPoint>tmp = GetTypeCards(-1, -1, 12);
			//标出3个经理
			int base = 0, mul = 0;
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
				base += GetScore(mx, my, pDC);
				mul += posCard(mx, my).mul - 1;
			}
			Wait(TIMEG2);

			//删掉老板，合成首富
			for (int i = 0; i < 3; i++) {
				int tx = tmp[i].x, ty = tmp[i].y;
				cards[card_table[tx][ty]].dead = true;
				card_table[tx][ty] = 0;
				ClearScore(tx, ty, pDC);
				cards[0].pos = CPoint(POSX[tx], POSY[ty]);
				cards[0].DisMark(pDC);
			}
			Wait(200);

			Card ncard;
			ncard.Create(13);
			CPoint npos = GetRandomBlock(-1, -1);
			ncard.pos = CPoint(POSX[npos.x], POSY[npos.y]);
			cards.push_back(ncard);
			card_table[npos.x][npos.y] = cards.size() - 1;
			InitScore(npos.x, npos.y, base, pDC);
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);
			AddMul(npos.x, npos.y, mul, 0, pDC);

			SetTimer(1, 500, NULL);
			return;
		}

		SetTimer(4, 500, NULL);
	}

	if (nIDEvent == 2) { // 进入选卡界面
		KillTimer(2);
		Chose_Card dlg;
		dlg.prt = this;
		dlg.DoModal();
		Invalidate();
	}

	if (nIDEvent == 4) { // 处理卡牌奖金，处理死亡卡牌
		KillTimer(4);

		int mx = -1;
		CPoint mxpos;
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) if (card_table[i][j]) {
			GetScore(i, j, pDC);
		}
		Wait(800);

		CPoint xpos(-1, -1);
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
			if (card_table[i][j]) {
				if (GetType(i, j) == 13) {
					xpos = CPoint(i, j);
					continue;
				}
				MulScore(i, j, cards[card_table[i][j]].mul, pDC);
				int sc = GetScore(i, j, pDC);
				if (sc > mx)
					mx = sc,
					mxpos = CPoint(i, j);
			}
		Wait(800);

		if (xpos.x != -1) {
			if (mx >= 0) {
				posCard(xpos).MarkYellow(pDC);
				posCard(mxpos).MarkGreen(pDC);
				Wait(TIMEG);
				AddScore(xpos.x, xpos.y, mx, pDC);
				Wait(500);
				posCard(xpos).DisMark(pDC);
				posCard(mxpos).DisMark(pDC);
			}
			MulScore(xpos.x, xpos.y, cards[card_table[xpos.x][xpos.y]].mul, pDC);
		}

		ll addscore = 0;
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
			if (card_table[i][j]) {
				addscore += cards[card_table[i][j]].score;
				Sum[GetType(i, j)] += cards[card_table[i][j]].score;
			}
		Total_Money += addscore;
		Max_Money = max(Max_Money, addscore);

		CString tmp;
		tmp.Format(TEXT("+%lld"), addscore);
		pDC->SetTextColor(RGB(6, 197, 38));
		int dx1, dy1, dx2, dy2;
		SetPos(297, 280, dx1, dy1);
		pDC->TextOutW(dx1, dy1, tmp);
		pDC->SetTextColor(RGB(0, 0, 0));

		SetPos(250, 300, dx1, dy1);
		SetPos(400, 320, dx2, dy2);
		CRect rect(dx1, dy1, dx2, dy2);
		CBrush whiteBrush(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
		pDC->FillRect(rect, &whiteBrush);
		pDC->SelectObject(pOldBrush);
		whiteBrush.DeleteObject();
		Money += addscore;
		tmp.Format(TEXT("金钱：%lld"), Money);
		SetPos(250, 300, dx1, dy1);
		pDC->TextOut(dx1, dy1, tmp);

		//处理结算后死亡
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
			if (GetType(i, j) == 25) {
				if (posCard(i, j).vis == false) continue;
				DelCardDraw(i, j, pDC);
			}
			if (GetType(i, j) == 26) {
				if (posCard(i, j).vis == false) continue;
				if (CheckChance(0.6)) DelCardDraw(i, j, pDC);
			}
		}

		//后Update，防止统计出错
		CCheck.EnableWindow(TRUE);
		CCALC.EnableWindow(TRUE);
		//CCheckA.EnableWindow(TRUE);
	}

#undef In
}


void CGameDlg::OnBnClickedButton1()
{
	CCheck.EnableWindow(FALSE);
	CCALC.EnableWindow(FALSE);
	CCheckA.EnableWindow(FALSE);
	if (Game_Round % 4 == 3) {
		int dx1, dy1, dx2, dy2;
		CString tmp;
		CDC* pDC = GetDC();
		pDC->SelectObject(curfont);
		tmp.Format(TEXT("-%lld"), Cost);
		pDC->SetTextColor(RGB(255, 0, 0));
		SetPos(297, 280, dx1, dy1);
		pDC->TextOutW(dx1, dy1, tmp);
		pDC->SetTextColor(RGB(0, 0, 0));

		SetPos(250, 300, dx1, dy1);
		SetPos(400, 320, dx2, dy2);
		CRect rect(dx1, dy1, dx2, dy2);
		CBrush whiteBrush(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
		pDC->FillRect(rect, &whiteBrush);
		pDC->SelectObject(pOldBrush);
		whiteBrush.DeleteObject();
		Money -= Cost;
		if (Cost <= 100) Cost += 40;
		else Cost *= 1.4;
		tmp.Format(TEXT("金钱：%lld"), Money);
		SetPos(250, 300, dx1, dy1);
		pDC->TextOutW(dx1, dy1, tmp);

		Wait(1000);
		if (Money < 0) {
			ShowWindow(SW_HIDE);
			CGame_Over game_over;
			game_over.prt = this;
			game_over.DoModal();
			return;
		}
		reroll_num += 2;
		delete_num++;
	}
	UpdateCard();
	Game_Round++;
	SetTimer(2, 0, NULL);
	// TODO: 在此添加控件通知处理程序代码
}


void CGameDlg::OnBnClickedButton2()
{
	CLibrary lib;
	lib.prt = this;
	lib.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}

void CGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int px = -1, py = -1;
	for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) {
		if (POSX[i] <= point.x && point.x <= POSX[i] + SPACE_X && POSY[j] <= point.y && point.y <= POSY[j] + SPACE_Y) {
			px = i, py = j;
			break;
		}
	}

	//点击了一个框查看属性
	if (px != -1 && card_table[px][py]!=0) {
		CInfo info_dia;
		info_dia.Del = false;
		info_dia.prt = this;
		info_dia.curid = card_table[px][py];
		info_dia.cur = cards[card_table[px][py]];
		info_dia.DoModal();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CGameDlg::Restart()
{
	ShowWindow(SW_SHOW);
	OnInitDialog();
}

void CGameDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);*/
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CGameDlg::OnBnClickedButton3()
{
	CAbility ability;
	ability.prt = this;
	ability.page = 0;
	ability.DoModal();
	// TODO: 在此添加控件通知处理程序代码
}


void CGameDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
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


void CGameDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

}
