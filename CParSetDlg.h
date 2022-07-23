#pragma once


// CParSetDlg 对话框

class CParSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParSetDlg)

public:
	CParSetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CParSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAR_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonPixBd();
	double m_nMPerPix;
};
