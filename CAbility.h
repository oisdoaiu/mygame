#pragma once
#include "afxdialogex.h"
#include "GameDlg.h"
#include "Abi.h"


// CAbility 对话框

class CAbility : public CDialogEx
{
	DECLARE_DYNAMIC(CAbility)

public:
	CAbility(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAbility();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CABILITY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CGameDlg* prt;
	int page;
	afx_msg void OnBnClickedPageup();
	afx_msg void OnBnClickedPagedown();
	CButton CUP;
	CButton CDOWN;
	CButton CPage;
};
