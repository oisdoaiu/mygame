#pragma once
#include "afxdialogex.h"


// Card 对话框

class Card : public CDialogEx
{
	DECLARE_DYNAMIC(Card)

public:
	Card(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Card();
	// 拷贝构造函数
	Card(const Card& other) : CDialogEx() {
		// 实现深拷贝逻辑
		this->type = other.type;
		this->cnt = other.cnt;
		this->dead = other.dead;
		this->pos = other.pos;
		this->score = other.score;
		this->vis = other.vis;
		this->mul = other.mul;
		this->initscore = other.initscore;
		this->weight = other.weight;
	}

	// 拷贝赋值运算符
	Card& operator=(const Card& other) {
		if (this != &other) {
			this->type = other.type;
			this->cnt = other.cnt;
			this->dead = other.dead;
			this->pos = other.pos;
			this->score = other.score;
			this->vis = other.vis;
			this->mul = other.mul;
			this->initscore = other.initscore;
			this->weight = other.weight;
		}
		return *this;
	}

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int type, cnt, score, mul;
	double weight;
	bool dead, vis, initscore;
	CPoint pos;
	void Draw(CDC* pDC);
	void Mark(CDC* pDC, int R, int G, int B);
	void MarkYellow(CDC* pDC);
	void MarkGreen(CDC* pDC);
	void MarkRed(CDC* pDC);
	void MarkBlue(CDC* pDC);
	void DisMark(CDC* pDC);
	void Create(int Type);
	CButton Check_Lib;
};

/*
	1：兔子
	2：兔子窝
	3：狼
	4：狼王
	5：复制
	6：猫
	7：老鼠
	8：鱼
	9：打工人
	10：主管
	11：经理
	12：老板
	13：首富

	14：茶杯
	15：茶
	16：茶壶
	17：奶牛
	18：肥宅
	19：草场
	20：牛奶
	21：小奶茶
	22：中奶茶
	23：大奶茶
	24：超大奶茶
	25：吸管
	26：大吸管
*/

/*
	1.Card中绘制信息
	2.Chose_Card中card_num，绘制卡面信息
	3.CInfo中卡面信息
	4.GameDlg中SCORE信息和处理信息，NUM数组大小
*/