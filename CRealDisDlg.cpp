// CRealDisDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "CRealDisDlg.h"
#include "afxdialogex.h"


// CRealDisDlg 对话框

IMPLEMENT_DYNAMIC(CRealDisDlg, CDialogEx)

CRealDisDlg::CRealDisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REAL_DIS, pParent)
	, m_nRealDis(0)
{

}

CRealDisDlg::~CRealDisDlg()
{
}

void CRealDisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REAL_DIS, m_nRealDis);
}


BEGIN_MESSAGE_MAP(CRealDisDlg, CDialogEx)
END_MESSAGE_MAP()


// CRealDisDlg 消息处理程序
