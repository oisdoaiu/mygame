﻿#pragma once
#include "afxdialogex.h"
#include "GameDlg.h"


// Chose_Card 对话框

class Chose_Card : public CDialogEx
{
	DECLARE_DYNAMIC(Chose_Card)

public:
	Chose_Card(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Chose_Card();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOSE_CARD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void DrawCard(CDC* pDC, int type, int sx, int sy, int pos);
	void Draw(CPaintDC* pDC);
	void DrawRarity(CDC* pdc, int type, int sx, int sy);
	int SpawnCard();
	afx_msg void OnBnClickedButton1();
	CGameDlg* prt;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedSkip();
	afx_msg void OnBnClickedReroll();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CButton CChoice1;
	CButton CChoice2;
	CButton CChoice3;
	CButton CSkip;
	CButton CReroll;
};
