#pragma once
#include "afxdialogex.h"
#include "GameDlg.h"


// CLIBRARY 对话框

class CLibrary : public CDialogEx
{
	DECLARE_DYNAMIC(CLibrary)

public:
	CLibrary(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLibrary();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBRARY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CGameDlg* prt;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void SetPos(int x, int y, int& resx, int& resy);
	CButton CConfirm;
};
