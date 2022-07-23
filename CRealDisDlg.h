#pragma once


// CRealDisDlg 对话框

class CRealDisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRealDisDlg)

public:
	CRealDisDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRealDisDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REAL_DIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_nRealDis;
};
