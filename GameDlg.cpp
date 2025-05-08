
// GameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Game.h"
#include "GameDlg.h"
#include "CAbility.h"
#include "afxdialogex.h"

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
const int SPACE_X = 200, SPACE_Y = 200;
const int SCORE[CARD_NUM+1] = { 0,1,2,1,3,2,1,1,1,1,1,2,3,0,1,1,3,2,3,3,1,1,1,2,5,1,3,1,1,1,2,3,0,0,2 };

BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	MoveWindow(100, 100, 1500, 1200);
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
	CCheck.MoveWindow(250, 500, 140, 40);
	CCheckA.MoveWindow(250, 550, 140, 40);
	CCALC.MoveWindow(880, 1050, 140, 40);
	int START_X = 500, START_Y = 100;
	Cost = 20; 
	Cost = 2000000;
	Game_Round = 3; 
	memset(Sum, 0, sizeof Sum);
	Total_Money = 0;
	Max_Money = 0;
	Money = 0; 
	cards.clear(); 
	reroll_num = 3;
	delete_num = 3;
	POSX[0] = START_X; for (int i = 1; i < 5; i++) POSX[i] = POSX[i - 1] + SPACE_X;
	POSY[0] = START_Y; for (int i = 1; i < 5; i++) POSY[i] = POSY[i - 1] + SPACE_Y;
	Card new_card;
	new_card.Create(0);
	cards.push_back(new_card);
	new_card.Create(4);
	for (int i = 1; i <= 3; i++) cards.push_back(new_card);
	new_card.Create(3);
	for (int i = 1; i <= 3; i++) cards.push_back(new_card);
	//cards.push_back(new_card);
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
	GetClientRect(&rect); // 获取客户区大小

	// 创建内存设备上下文
	CDC memDC;
	memDC.CreateCompatibleDC(&dc); // 创建与屏幕兼容的内存DC

	// 创建位图并选入内存DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// 在内存DC中绘制内容
	memDC.FillSolidRect(&rect, RGB(255, 255, 255)); // 填充背景色
	memDC.SetTextColor(RGB(0, 0, 0)); // 设置文字颜色
	memDC.SetBkMode(TRANSPARENT); // 设置背景模式为透明
	
	CString tmp;
	//显示钱数
	tmp.Format(TEXT("金钱：%lld"), Money);
	memDC.TextOut(250, 300, tmp);
	memDC.SetTextColor(RGB(255, 0, 0));
	tmp.Format(TEXT("需求：%lld"), Cost);
	memDC.TextOut(250, 320, tmp);
	memDC.SetTextColor(RGB(0, 0, 0));

	//显示回合
	CFont newFont;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(24, memDC.GetDeviceCaps(LOGPIXELSY), 72); // 设置字体高度为24点
	lf.lfWeight = FW_NORMAL; // 字体粗细
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS; // 仅使用TrueType字体
	wcscpy_s(lf.lfFaceName, _T("Arial")); // 字体名称
	newFont.CreateFontIndirect(&lf);
	// 保存原来的字体
	CFont* pOldFont = memDC.SelectObject(&newFont);
	// 使用TextOut绘制文字
	tmp.Format(TEXT("第%d年"), Game_Round / 4 + 1);
	memDC.TextOut(230, 80, tmp);
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
	memDC.TextOutW(270, 150, tmp);
	// 恢复原来的字体
	memDC.SelectObject(pOldFont);

	//绘制游戏区域
	SpawnTable();
	DrawBlocks(&memDC);

	//启动一轮计算
	CCALC.EnableWindow(FALSE);
	//CCheck.EnableWindow(FALSE);
	CCheckA.EnableWindow(FALSE);
	SetTimer(3, 100, NULL);
		
	// 将内存DC的内容复制到屏幕
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 清理资源
	memDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
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
		//特殊处理茶和茶杯标记
	}
}

void CGameDlg::AddScore(int x, int y, ll add, CDC* pDC)
{
	//擦除之前的
	int sx = POSX[x], sy = POSY[y];
	CRect rect(sx, sy - 70, sx + 100, sy - 20);
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
		pDC->TextOutW(sx + 5, sy - 60, tmp);
	}
	else if (add < 0) {
		tmp.Format(TEXT("- %lld"), -add);
		pDC->TextOutW(sx + 5, sy - 60, tmp);
	}
}

void CGameDlg::MulScore(int x, int y, ll mul, CDC* pDC)
{
	//擦除之前的
	int sx = POSX[x], sy = POSY[y];
	CRect rect(sx, sy - 70, sx + 100, sy - 20);
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
	pDC->TextOutW(sx + 5, sy - 60, tmp);
}

void CGameDlg::InitScore(int x, int y, ll score, CDC* pDC)
{
	CString tmp;
	tmp.Format(TEXT("%lld"), score);
	pDC->TextOutW(POSX[x] + 5, POSY[y] - 40, tmp);
	cards[card_table[x][y]].initscore = false;
	cards[card_table[x][y]].score = score;
}

void CGameDlg::ClearScore(int x, int y, CDC* pDC)
{
	int sx = POSX[x], sy = POSY[y];
	CRect rect(sx, sy - 70, sx + 100, sy - 20);
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
	CRect rect(sx + 5, sy + 65, sx + 90, sy + 90);
	CBrush whiteBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
	pDC->FillRect(rect, &whiteBrush);
	pDC->SelectObject(pOldBrush);
	whiteBrush.DeleteObject();

	if (opt == 1)
		cards[card_table[x][y]].mul *= mul;
	if (opt == 0)
		cards[card_table[x][y]].mul += mul;
	CString tmp; tmp.Format(TEXT("X %lld"), cards[card_table[x][y]].mul);
	pDC->TextOutW(sx + 10, sy + 70, tmp);
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
					vector<CPoint> tmp = GetTypeCards(i, j, { 9,10,11,12,13 });
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

		int mx = 0;
		CPoint mxpos;
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) if (card_table[i][j]) {
			GetScore(i, j, pDC);
		}
		Wait(1000);

		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
			if (card_table[i][j]) {
				if (GetType(i, j) == 13) continue;
				MulScore(i, j, cards[card_table[i][j]].mul, pDC);
				int sc = GetScore(i, j, pDC);
				if (sc > mx)
					mx = sc,
					mxpos = CPoint(i, j);
			}
		Wait(1000);

		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++) if (GetType(i, j) == 13) {
			cards[card_table[i][j]].MarkYellow(pDC);
			cards[card_table[mxpos.x][mxpos.y]].MarkGreen(pDC);
			Wait(TIMEG);
			AddScore(i, j, mx, pDC);
			Wait(1000);
			cards[card_table[i][j]].DisMark(pDC);
			cards[card_table[mxpos.x][mxpos.y]].DisMark(pDC);
			MulScore(i, j, cards[card_table[i][j]].mul, pDC);
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
		pDC->TextOutW(297, 280, tmp);
		pDC->SetTextColor(RGB(0, 0, 0));

		CRect rect(250, 300, 400, 320);
		CBrush whiteBrush(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
		pDC->FillRect(rect, &whiteBrush);
		pDC->SelectObject(pOldBrush);
		whiteBrush.DeleteObject();
		Money += addscore;
		tmp.Format(TEXT("金钱：%lld"), Money);
		pDC->TextOut(250, 300, tmp);

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
		CCheckA.EnableWindow(TRUE);
	}

#undef In
}


void CGameDlg::OnBnClickedButton1()
{
	CCheck.EnableWindow(FALSE);
	CCALC.EnableWindow(FALSE);
	CCheckA.EnableWindow(FALSE);
	if (Game_Round % 4 == 3) {
		CString tmp;
		CDC* pDC = GetDC();
		tmp.Format(TEXT("-%lld"), Cost);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->TextOutW(301, 280, tmp);
		pDC->SetTextColor(RGB(0, 0, 0));

		CRect rect(250, 300, 400, 320);
		CBrush whiteBrush(RGB(255, 255, 255));
		CBrush* pOldBrush = pDC->SelectObject(&whiteBrush);
		pDC->FillRect(rect, &whiteBrush);
		pDC->SelectObject(pOldBrush);
		whiteBrush.DeleteObject();
		Money -= Cost;
		if (Cost <= 100) Cost += 40;
		else Cost *= 1.5;
		tmp.Format(TEXT("金钱：%lld"), Money);
		pDC->TextOut(250, 300, tmp);

		Wait(1000);
		if (Money < 0) {
			ShowWindow(SW_HIDE);
			CGame_Over game_over;
			game_over.prt = this;
			game_over.DoModal();
			return;
		}
		delete_num += 2;
		reroll_num += 2;
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
	CString tmp;
	tmp.Format(TEXT("%d %d"), point.x, point.y);
	MessageBox(tmp);
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
