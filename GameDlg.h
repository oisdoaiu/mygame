
// GameDlg.h: 头文件
#include"Card.h"
//

#pragma once


// CGameDlg 对话框
class CGameDlg : public CDialogEx
{
// 构造
public:
	CGameDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int Money, Cost;
	int card_table[5][5];
	vector<Card> cards;
	void DrawBlocks(CDC* pDC);
	void SpawnTable();
	void AddScore(int x, int y, int add, CDC* pDC);
	void MulScore(int x, int y, int add, CDC* pDC);
	void InitScore(int x, int y, int score, CDC* pDC);
	void ClearScore(int x, int y, CDC* pDC);
	int GetScore(int x, int y, CDC* pDC);
	void AddMul(int x, int y, int mul, char opt, CDC* pDC);
	void Eat(int sx, int sy, int tx, int ty, CDC* pDC);
	int GetType(int x, int y);
	void UpdateCard();
	bool AddCard(int x, int y, int type, int opt, CDC* pDC);
	void AddCard(int x, int y, int type, CDC* pDC);
	bool AddCardDraw(int x, int y, int type, int opt, CDC* pDC);
	bool IsType(int x, int y, vector<int>type);
	void DelCardDraw(int x, int y, CDC* pDC);
	void DelCard(int x, int y, CDC* pDC);
	bool CheckChance(double p);
	vector<CPoint> GetCards(int x, int y, char opt);
	vector<CPoint> GetTypeCards(int x, int y, int type);
	vector<CPoint> GetTypeCards(int x, int y, vector<int> type);
	CPoint GetRandomBlock(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	CButton CCALC;
	CButton CCheck;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void Restart();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
