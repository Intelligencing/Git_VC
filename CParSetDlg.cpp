// CParSetDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "CParSetDlg.h"
#include "afxdialogex.h"
#include "CRealDisDlg.h"


// CParSetDlg 对话框

IMPLEMENT_DYNAMIC(CParSetDlg, CDialogEx)

CParSetDlg::CParSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAR_SET, pParent)
	, m_nMPerPix(0)
{

}

CParSetDlg::~CParSetDlg()
{
}

void CParSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MM_PER_PIX, m_nMPerPix);
}


BEGIN_MESSAGE_MAP(CParSetDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CParSetDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_PIX_BD, &CParSetDlg::OnBnClickedButtonPixBd)
END_MESSAGE_MAP()


// CParSetDlg 消息处理程序


void CParSetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CParSetDlg::OnBnClickedButtonPixBd()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON_PIX_BD)->EnableWindow(FALSE);
	
	CMFCApplication3Dlg* pMainDlg = (CMFCApplication3Dlg*)theApp.m_pMainWnd;
	
	//CMFCApplication3Dlg *pMain = (CMFCApplication3Dlg*) AfxGetMainWnd();//返回窗口句柄

	try {
		DispObj(pMainDlg->ho_Image, pMainDlg->hWndID);
		SetColor(pMainDlg->hWndID, "green");
		SetTposition(pMainDlg->hWndID, 50, 100);
		WriteString(pMainDlg -> hWndID, "请选择点：");
		//画点
		HObject CrossPoint;
		HTuple RowP, ColP;
		DrawPoint(pMainDlg->hWndID, &RowP, &ColP);
		GenCrossContourXld(&CrossPoint, RowP, ColP, 60, 0);
		DispObj(CrossPoint, pMainDlg->hWndID);

		SetTposition(pMainDlg->hWndID, 100, 100);
		WriteString(pMainDlg->hWndID, "请选择线");

			//画线
		HObject RegionLine;
		HTuple  RowL1, ColL1, RowL2, ColL2;
		DrawLine(pMainDlg->hWndID, &RowL1, &ColL1, &RowL2, &ColL2);
		GenRegionLine(&RegionLine, RowL1, ColL1, RowL2, ColL2);
		DispObj(RegionLine, pMainDlg->hWndID);

		HTuple DistancePix;
		DistancePl(RowP, ColP, RowL1, ColL1, RowL2, ColL2, &DistancePix);

		CRealDisDlg RealDisDlg;

		if (RealDisDlg.DoModal() == IDOK)
		{
			double pixDis = DistancePix[0].D();//读取对话框输入数据
			m_nMPerPix = RealDisDlg.m_nRealDis / pixDis;
			
			pMainDlg->m_gMmPerPix = m_nMPerPix;//数据传到主对话框
			//保存
			CString strPar;
			strPar.Format(_T("%f"), m_nMPerPix);
			WritePrivateProfileString(_T("BD"), _T("m_gMmPerPix"), strPar,pMainDlg->strIniFilePath);

			UpdateData(FALSE);

			DispObj(pMainDlg->ho_Image, pMainDlg->hWndID);
			AfxMessageBox(_T("标定成功！"));
		}
		else
		{
			DispObj(pMainDlg->ho_Image, pMainDlg->hWndID);
			AfxMessageBox(_T("标定失败！"));

		}
	}	

	catch (...)
	{
		AfxMessageBox(_T("操作失败！"));
	}

	GetDlgItem(IDC_BUTTON_PIX_BD)->EnableWindow(TRUE);

}
