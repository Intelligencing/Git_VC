
// MFCApplication3Dlg.h: 头文件
//

#pragma once
#include "System_include.h"
#include "afxwin.h"
#include "CParSetDlg.h"

// CMFCApplication3Dlg 对话框
class CMFCApplication3Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication3Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
public:
	HTuple hv_AcqHandle;
	HTuple hWndID;//窗口句柄

	HObject ho_Image;
	HObject ho_RegionRecRol;//感兴趣区域

	BOOL m_fCameraOpen;
	BOOL m_fRoiExists;
	BOOL m_fAutoRun;//自动运行状态
	//INI文件预览
	CString strIniFilePath;
	//设置界面
	CParSetDlg ParSetDlg;
	//系统参数
	double m_gMmPerPix;

private:
	static UINT ContinueGrabThread(LPVOID p);
	static UINT AutoRunThread(LPVOID p);

	
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctlshowWnd;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1ContinueGrab();
	BOOL m_continue_grab;
	afx_msg void OnBnClickedButton1SaveImage();
	afx_msg void OnBnClickedButton1SelRoi();
	afx_msg void OnStnClickedStaticShowWnd();
	afx_msg void OnBnClickedButton1AotoRun();
	afx_msg void OnBnClickedButtonParSet();
};
