#pragma once
#include "afxdialogex.h"


// CGame_Over 对话框

class CGame_Over : public CDialogEx
{
	DECLARE_DYNAMIC(CGame_Over)

public:
	CGame_Over(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGame_Over();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_OVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};
