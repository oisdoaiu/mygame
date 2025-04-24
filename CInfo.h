#pragma once
#include "afxdialogex.h"
#include "GameDlg.h"


// CInfo 对话框

class CInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CInfo)

public:
	CInfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGameDlg* prt;
	Card cur;
	bool Del;
	int curid;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedDelete();
	virtual BOOL OnInitDialog();
};
