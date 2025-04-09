
// GameDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Game.h"
#include "GameDlg.h"
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
END_MESSAGE_MAP()


// CGameDlg 消息处理程序

int POSX[5], POSY[5], Game_Round;
const int SPACE_X = 200, SPACE_Y = 200;
const int SCORE[14] = { 0,1,2,1,3,2,1,1,1,1,1,2,3,0 };

BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	int START_X = 500, START_Y = 100;
	Cost = 20; Game_Round = 0; Money = 0; cards.clear();
	POSX[0] = START_X; for (int i = 1; i < 5; i++) POSX[i] = POSX[i - 1] + SPACE_X;
	POSY[0] = START_Y; for (int i = 1; i < 5; i++) POSY[i] = POSY[i - 1] + SPACE_Y;
	Card new_card;
	new_card.Create(0);
	cards.push_back(new_card);
	new_card.Create(13);
	cards.push_back(new_card);
	new_card.Create(12);
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
	tmp.Format(TEXT("金钱：%d"), Money);
	memDC.TextOut(250, 300, tmp);
	memDC.SetTextColor(RGB(255, 0, 0));
	tmp.Format(TEXT("需求：%d"), Cost);
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
	CCheck.EnableWindow(FALSE);
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
	}
}

void CGameDlg::AddScore(int x, int y, int add, CDC* pDC)
{
	//擦除之前的
	int sx = POSX[x], sy = POSY[y];
	CRect rect(sx, sy - 100, sx + 100, sy - 10);
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
	tmp.Format(TEXT("+ %d"), add);
	pDC->TextOutW(sx + 5, sy - 60, tmp);
}

void CGameDlg::MulScore(int x, int y, int mul, CDC* pDC)
{
	//擦除之前的
	int sx = POSX[x], sy = POSY[y];
	CRect rect(sx, sy - 100, sx + 100, sy - 10);
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
	tmp.Format(TEXT("X %d"), mul);
	pDC->TextOutW(sx + 5, sy - 60, tmp);
}

void CGameDlg::InitScore(int x, int y, int score, CDC* pDC)
{
	CString tmp;
	tmp.Format(TEXT("%d"), score);
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

int CGameDlg::GetScore(int x, int y, CDC* pDC)
{
	Card& cur = cards[card_table[x][y]];
	if (cur.initscore) {
		cur.initscore = false;
		InitScore(x, y, SCORE[cur.type], pDC);
	}
	return cur.score;
}

void CGameDlg::AddMul(int x, int y, int mul, char opt, CDC* pDC)
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
	CString tmp; tmp.Format(TEXT("X %d"), cards[card_table[x][y]].mul);
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

bool CGameDlg::AddCard(int x, int y, int type, int opt, CDC* pDC)
{
	if (opt == 0) {
		CPoint np = GetRandomBlock(-1, -1);
		if (np.x == -1) return false;
		AddCard(np.x, np.y, type, pDC);
	}
	if (opt == 1) {
		CPoint np = GetRandomBlock(x, y);
		if (np.x == -1) return false;
		AddCard(np.x, np.y, type, pDC);
	}
	return true;
}

void CGameDlg::AddCard(int x, int y, int type, CDC* pDC)
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
	assert(GetType(x, y) == type);

	ncard.MarkGreen(pDC);
	Wait(TIMEG);

	int repeat_num = 0;
	for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
		if (GetType(i,j) == 5) repeat_num++;
	while (repeat_num --> 0) {
		CPoint new_pos = GetRandomBlock(-1,-1);
		if (new_pos.x == -1) break;
		ncard.pos = CPoint(POSX[new_pos.x],POSY[new_pos.y]);
		if (type == 1) {
			ncard.vis = true;
		}
		cards.push_back(ncard);
		card_table[new_pos.x][new_pos.y] = cards.size()-1;
		ncard.MarkBlue(pDC);
		Wait(TIMEG);
	}

	for (int i = startpos; i < cards.size(); i++)
		cards[i].DisMark(pDC);
}

bool CGameDlg::AddCardDraw(int x, int y, int type, int opt, CDC* pDC)
{
	int cur = card_table[x][y];
	CPoint new_card;
	if (opt == 0) new_card = GetRandomBlock(-1, -1);
	if (opt == 1) new_card = GetRandomBlock(x, y);
	if (new_card.x == -1) return false;
	cards[cur].MarkYellow(pDC);
	Wait(TIMEG);
	AddCard(x, y, type, opt, pDC);
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
	cards[0].pos = cards[card_table[x][y]].pos;
	cards[0].DisMark(pDC);
	card_table[x][y] = 0;
}

void CGameDlg::DelCard(int x, int y, CDC* pDC) {
	cards[card_table[x][y]].dead = true;
	cards[0].pos = cards[card_table[x][y]].pos;
	cards[0].DisMark(pDC);
	card_table[x][y] = 0;
}

bool CGameDlg::CheckChance(double p)
{
	srand((unsigned)time(NULL));
	double randomValue = (double)rand() / (RAND_MAX + 1.0);
	return randomValue < p;
}

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

		for (int i = 0; i < 5 && !Updated; i++) for (int j = 0; j < 5 && !Updated; j++) if (card_table[i][j]) {
			int cur = card_table[i][j];

			//兔子
			if (cards[cur].type == 1) {
				if (cards[cur].cnt == 1) continue;
				if (cards[cur].vis) continue;
				if (GetTypeCards(-1, -1, 1).size() > 7) continue;
				if (AddCardDraw(i, j, 1, 1, pDC)) {
					Updated = true;
					cards[cur].cnt = 1;
				}
			}

			//兔子窝
			if (cards[cur].type == 2) {
				if (cards[cur].vis == true) continue;
				if (AddCardDraw(i, j, 1, 0, pDC)) {
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
				cards[cur].DisMark(pDC);
				if (cards[cur].vis == false) {
					cards[cur].vis = true;
					if (CheckChance(0.05)) {
						AddCardDraw(i, j, 9, 0, pDC);
						Updated = true;
					}
				}

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
				cards[cur].DisMark(pDC);
				if (cards[cur].vis == false) {
					cards[cur].vis = true;
					if (CheckChance(0.1)) {
						AddCardDraw(i, j, 9, 0, pDC);
						Updated = true;
					}
				}

			}

			//首富
			if (cards[cur].type == 13) {
				vector<CPoint> tmp = GetTypeCards(-1, -1, 13);
				if (tmp.size() == 1) continue;
				cards[cur].MarkYellow(pDC);
				AddMul(i, j, cards[cur].cnt + 1, 1, pDC);
				Updated = true;
				for (CPoint v : tmp) {
					if (CPoint(i,j) == v) continue;
					cards[card_table[v.x][v.y]].MarkGreen(pDC);
					Wait(TIMEG);
					int dlt = cards[card_table[v.x][v.y]].cnt + 1;
					DelCard(v.x, v.y, pDC);
					cards[cur].cnt += dlt;
					AddMul(i, j, dlt, 0, pDC);
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
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
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
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);

			SetTimer(1, 500, NULL);
			return;
		}

		//3主管合成1经理
		if (GetTypeCards(-1, -1, 10).size() >= 3) {

			vector<CPoint>tmp = GetTypeCards(-1, -1, 10);
			//标出3个主管
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
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
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);

			SetTimer(1, 500, NULL);
			return;
		}

		//3经理合成1老板
		if (GetTypeCards(-1, -1, 11).size() >= 3) {

			vector<CPoint>tmp = GetTypeCards(-1, -1, 11);
			//标出3个经理
			for (int i = 0; i < 3; i++) {
				int mx = tmp[i].x, my = tmp[i].y;
				cards[card_table[mx][my]].MarkGreen(pDC);
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
			ncard.MarkYellow(pDC);
			Wait(TIMEG);

			ncard.DisMark(pDC);

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
		Wait(1000);

		int addscore = 0;
		for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
			if (card_table[i][j])
				addscore += cards[card_table[i][j]].score;
		CString tmp;
		tmp.Format(TEXT("+%d"), addscore);
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
		tmp.Format(TEXT("金钱：%d"), Money);
		pDC->TextOut(250, 300, tmp);


		//后Update，防止统计出错
		CCheck.EnableWindow(TRUE);
		CCALC.EnableWindow(TRUE);
	}

#undef In
}


void CGameDlg::OnBnClickedButton1()
{
	CCheck.EnableWindow(FALSE);
	CCALC.EnableWindow(FALSE);
	if (Game_Round % 4 == 3) {
		CString tmp;
		CDC* pDC = GetDC();
		tmp.Format(TEXT("-%d"), Cost);
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
		if (Cost <= 200) Cost += 20;
		else Cost *= 1.1;
		tmp.Format(TEXT("金钱：%d"), Money);
		pDC->TextOut(250, 300, tmp);

		Wait(1000);
		if (Money < 0) {
			ShowWindow(SW_HIDE);
			CGame_Over game_over;
			game_over.DoModal();
			return;
		}
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
		assert(card_table[px][py] < cards.size());
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
