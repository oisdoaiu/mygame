// CStart.cpp: 实现文件
//

#include "pch.h"
#include "Game.h"
#include "afxdialogex.h"
#include "CStart.h"
#include "GameDlg.h"


// CStart 对话框

IMPLEMENT_DYNAMIC(CStart, CDialogEx)

CStart::CStart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_START, pParent)
{

}

CStart::~CStart()
{
}

void CStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStart, CDialogEx)
	ON_BN_CLICKED(IDOK, &CStart::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CStart 消息处理程序


void CStart::OnBnClickedOk()
{
	CGameDlg* pGameDlg = (CGameDlg*)AfxGetMainWnd();
	if (pGameDlg)
	{
		pGameDlg->Restart();
	}
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CStart::OnClose()
{
    // 显示消息框询问用户是否退出
    int nResult = MessageBox(_T("确定要退出程序吗？"), _T("退出确认"), MB_YESNO | MB_ICONQUESTION);

    if (nResult == IDYES)
    {
        // 调用父窗口的DestroyWindow函数销毁父窗口，结束程序
        CGameDlg prt = GetParent();
        if (prt)
            prt.DestroyWindow();
        else PostQuitMessage(0);
    }
    else
    {
        // 如果用户选择不退出，返回不执行默认关闭操作
        return;
    }

    // 调用基类的OnClose函数执行默认关闭操作（可选）
    CDialogEx::OnClose();
}
