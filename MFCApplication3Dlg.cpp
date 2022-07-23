
// MFCApplication3Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"
#include "afxdialogex.h"
#include "System_include.h"
#include "ImageProc.h"
#include "halconcpp.h"
#include "CParSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 对话框



CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=nullptr*/)//构造函数
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
	, m_continue_grab(FALSE)
{
	m_fAutoRun = FALSE;//初始化变量
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW_WND, m_ctlshowWnd);
	DDX_Check(pDX, IDC_CHECK1_CONTINUE_GRAB, m_continue_grab);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication3Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1_CONTINUE_GRAB, &CMFCApplication3Dlg::OnBnClickedCheck1ContinueGrab)
	ON_BN_CLICKED(IDC_BUTTON1_SAVE_IMAGE, &CMFCApplication3Dlg::OnBnClickedButton1SaveImage)
	ON_BN_CLICKED(IDC_BUTTON1_SEL_ROI, &CMFCApplication3Dlg::OnBnClickedButton1SelRoi)
	ON_STN_CLICKED(IDC_STATIC_SHOW_WND, &CMFCApplication3Dlg::OnStnClickedStaticShowWnd)
	ON_BN_CLICKED(IDC_BUTTON1_AOTO_RUN, &CMFCApplication3Dlg::OnBnClickedButton1AotoRun)
	ON_BN_CLICKED(IDC_BUTTON_PAR_SET, &CMFCApplication3Dlg::OnBnClickedButtonParSet)
END_MESSAGE_MAP()


// CMFCApplication3Dlg 消息处理程序

BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	strIniFilePath = _T("D:\HALCON\MFCApplication3 - 副本 - 副本\MFCApplication3\SetPar.ini");//初始化存储路径

	//加载标定参数
	CString strPar;
	GetPrivateProfileString(_T("BD"), _T("m_gMmPerPix"), _T("0.0"), strPar.GetBuffer(256), 256, strIniFilePath);
	m_gMmPerPix = _ttof(strPar);//读数据

	ParSetDlg.Create(IDD_DIALOG_PAR_SET,this);

	try {
		//打开相机
		OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
				"default", "[0] Integrated Camera", 0, -1, &hv_AcqHandle);
		//开窗口
		CRect rct;
		m_ctlshowWnd.GetClientRect(rct);
		long lWindowID = 0;
		long m_nPicWidth = rct.Width();
		long m_nPicHeight = rct.Height();
		lWindowID = (long)m_ctlshowWnd.GetSafeHwnd();
		OpenWindow(0, 0, (Hlong)m_nPicWidth, (Hlong)m_nPicHeight, lWindowID, "visible", "", &hWndID);
		//采图
		GrabImage(&ho_Image, hv_AcqHandle);
		//显示
		HTuple hW, hH;
		GetImageSize(ho_Image, &hW, &hH);
		SetPart(hWndID, 0, 0, hH, hW);
		
		DispObj(ho_Image, hWndID);

		m_fCameraOpen = TRUE;
		//加载ROI区域
		m_fRoiExists = FALSE;

		//if (PathFileExists(_T("D:\HALCON\MFCApplication3 - 副本\RecRoi.hobj")))
		//{
			ReadRegion(&ho_RegionRecRol, "D:\HALCON\MFCApplication3 - 副本\RecRoi.hobj");
			m_fRoiExists = TRUE; 

			SetColor(hWndID, "red");
			SetDraw(hWndID, "margin");
			DispObj(ho_RegionRecRol, hWndID);
		//}
		//else
		//{
			//AfxMessageBox(_T("没有选择区域"));
		//}
	}
	catch(...)
	{
		AfxMessageBox(_T("相机打开失败"));
		m_fCameraOpen = FALSE;
		exit(0);
	}
	

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication3Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_fCameraOpen) {
		CloseFramegrabber(hv_AcqHandle);
	}
	
	CDialogEx::OnCancel();
}

CEvent exitGrabThread;
void CMFCApplication3Dlg::OnBnClickedCheck1ContinueGrab()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_continue_grab) //判断按键
	{
		AfxBeginThread(ContinueGrabThread, this);
	}
	else
	{
		exitGrabThread.SetEvent();
	}
}

UINT CMFCApplication3Dlg::ContinueGrabThread(LPVOID p)
{
	CMFCApplication3Dlg *pThis = (CMFCApplication3Dlg *)p;
	
	while (WaitForSingleObject(exitGrabThread.m_hObject, 2) != WAIT_OBJECT_0)
	{
		GrabImage(&(pThis->ho_Image), pThis->hv_AcqHandle);
		DispObj(pThis->ho_Image, pThis->hWndID);
		
		if (pThis->m_fRoiExists)//存在ROI区域则显示
		{
			SetColor(pThis->hWndID, "red");
			SetDraw(pThis->hWndID, "margin");
			DispObj(pThis->ho_RegionRecRol, pThis->hWndID);
		}
	}
	return 0;
}


void CMFCApplication3Dlg::OnBnClickedButton1SaveImage()
{
	// TODO: 在此添加控件通知处理程序代码
	
	

	WriteImage(ho_Image, "bmp", 0, "D:/");
}


void CMFCApplication3Dlg::OnBnClickedButton1SelRoi()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_continue_grab)//保证不在实时状态下有冲突
	{
		AfxMessageBox(_T("请先停止实时！"));
		return;
	}
	GetDlgItem(IDC_BUTTON1_SEL_ROI)->EnableWindow(FALSE);

	HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;//定义控制型变量
	HObject ho_Rectangle;//定义对象

	try
	{
		DispObj(ho_Image, hWndID);//刷新
		SetColor(hWndID,"green");//设置
		SetTposition(hWndID, 50, 50);
		WriteString(hWndID, "请框选处理区域：");

		DrawRectangle1(hWndID, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);//画框
		GenRectangle1(&ho_Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
		
		SetDraw(hWndID, "margin");//设置框为空心
		DispObj(ho_Rectangle,hWndID);//显示

		GenEmptyObj(&ho_RegionRecRol);//清空刷新
		ConcatObj(ho_RegionRecRol, ho_Rectangle, &ho_RegionRecRol);
		//储存
		WriteRegion(ho_RegionRecRol, "D:\HALCON\MFCApplication3 - 副本\RecRoi.hobj");
		
	}
	catch (...)
	{
		AfxMessageBox(_T("操作失败"));
		
	}
	GetDlgItem(IDC_BUTTON1_SEL_ROI)->EnableWindow(TRUE);
	AfxMessageBox(_T("设置成功"));
	
}


void CMFCApplication3Dlg::OnStnClickedStaticShowWnd()
{
	// TODO: 在此添加控件通知处理程序代码
}

CEvent eAutoExits;//定义线程退出事件

void CMFCApplication3Dlg::OnBnClickedButton1AotoRun()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_continue_grab)
	{
		AfxMessageBox(_T("请先停止实时！"));
		return;
	}
	if (m_fRoiExists = FALSE)
	{
		AfxMessageBox(_T("没有选择区域"));
		return;
	}
	m_fAutoRun = !m_fAutoRun;
	
	if (m_fAutoRun)
	{
		AfxBeginThread(AutoRunThread, this);
		SetDlgItemText(IDC_BUTTON1_AOTO_RUN, _T("停止运行"));
	}
	else
	{
		eAutoExits.SetEvent();
		SetDlgItemText(IDC_BUTTON1_AOTO_RUN, _T("自动运行"));
	}
}
UINT CMFCApplication3Dlg::AutoRunThread(LPVOID p) 
{
	CMFCApplication3Dlg* pThis = (CMFCApplication3Dlg*)p;
	
	BOOL fProc = FALSE;

	INPUTDATA stInData;
	OUTPUTDATA stOutData;

	GenEmptyObj(&stInData.RegionRoi);//刷新
	Union2(stInData.RegionRoi, pThis->ho_RegionRecRol, &stInData.RegionRoi);


	while (WaitForSingleObject(eAutoExits.m_hObject, 5) != WAIT_OBJECT_0)
	{
		
		//读取串口数据

		//如果是start，就进行拍图处理

		//拍图
		GrabImage(&pThis->ho_Image, pThis->hv_AcqHandle);
		DispObj(pThis->ho_Image, pThis->hWndID);
		SetColor(pThis->hWndID, "red");
		SetDraw(pThis->hWndID, "margin");
		DispObj(pThis->ho_RegionRecRol, pThis->hWndID);
		//处理
		stOutData.minDis = 0.0;//刷新
		GenEmptyObj(&stOutData.hCircle);
		GenEmptyObj(&stOutData.hLine);

		int ret = ImageProc(pThis->ho_Image, pThis->hWndID,&stInData, &stOutData);//调用处理函数 


		double retDis;
		//显示
		if (ret == PROC_OK)
		{
			SetColor(pThis->hWndID, "red");
			SetDraw(pThis->hWndID, "margin");
			DispObj(stOutData.hCircle, pThis->hWndID);

			SetColor(pThis->hWndID, "green");
			DispObj(stOutData.hLine, pThis->hWndID);

			retDis = stOutData.minDis[0]. D();
		}
		else
		{
			retDis = 0.0;
			SetColor(pThis->hWndID, "red");
			SetTposition(pThis->hWndID, 50, 100);
			WriteString(pThis->hWndID, "NG");

			switch (ret)
			{
			case PROC_NO_PRODUCT:
				SetTposition(pThis->hWndID, 100, 100);
				WriteString(pThis->hWndID, "没有找到产品");
				break;

			case PROC_NO_BIANYUAN:
				SetTposition(pThis->hWndID, 100, 100);
				WriteString(pThis->hWndID, "没有找到边缘");
				break;

			case PROC_NO_LIGHT:
				SetTposition(pThis->hWndID, 100, 100);
				WriteString(pThis->hWndID, "没有找到灯珠");
				break;

			case PROC_NG:
				SetTposition(pThis->hWndID, 100, 100);
				WriteString(pThis->hWndID, "图像处理错误");
				break;
			}
		}
		//输出串口数据
		/*double realdis = retDis * pThis->m_gMmPerPix;

		CString strSendMsg;
		strSendMsg.Format(_T("S,%0.2f,E"), retDis);

		char SendBuffer[1024] = { 0 };
		int n = WideCharToMultiByte(CP_ACP, 0, strSendMsg.GetBuffer(), -1, NULL, 0, NULL, false);
		WideCharToMultiByte(CP_ACP, 0, strSendMsg.GetBuffer(), -1, SendBuffer, n, NULL, false);
	
		WriteSerial(pThis->m_hIcon, SendBuffer, n - 1);*/
	}
	return 0;
}

void CMFCApplication3Dlg::OnBnClickedButtonParSet()
{
	// TODO: 在此添加控件通知处理程序代码
	ParSetDlg.m_nMPerPix = m_gMmPerPix;
	ParSetDlg.UpdateData(FALSE);
	ParSetDlg.ShowWindow(SW_SHOW);
}
