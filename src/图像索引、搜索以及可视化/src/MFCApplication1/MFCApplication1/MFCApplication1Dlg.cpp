
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "GetList.h"
#include <windows.h>
#include <shellapi.h>
#include "RTree.h"


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
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_edit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GETLIST, m_getList);
	//  DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Text(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_PICTURE1, m_picture1);

}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GETLIST, &CMFCApplication1Dlg::OnBnClickedGetlist)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMFCApplication1Dlg::OnLbnSelchangeList1)
//	ON_LBN_DBLCLK(IDC_LIST1, &CMFCApplication1Dlg::OnDblclkList1)
	ON_CBN_SELCHANGE(IDC_COMBO, &CMFCApplication1Dlg::OnCbnSelchangeCombo)
	ON_STN_CLICKED(IDC_PICTURE1, &CMFCApplication1Dlg::OnStnClickedPicture1)
	ON_STN_CLICKED(IDC_PICTURE2, &CMFCApplication1Dlg::OnStnClickedPicture2)
	ON_STN_CLICKED(IDC_PICTURE3, &CMFCApplication1Dlg::OnStnClickedPicture3)
	ON_STN_CLICKED(IDC_PICTURE4, &CMFCApplication1Dlg::OnStnClickedPicture4)
	ON_STN_CLICKED(IDC_PICTURE5, &CMFCApplication1Dlg::OnStnClickedPicture5)
	ON_STN_CLICKED(IDC_PICTURE6, &CMFCApplication1Dlg::OnStnClickedPicture6)
	ON_STN_CLICKED(IDC_PICTURE7, &CMFCApplication1Dlg::OnStnClickedPicture7)
	ON_STN_CLICKED(IDC_PICTURE8, &CMFCApplication1Dlg::OnStnClickedPicture8)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	flag = true;
	nIndex = 0;
	M_DEMAND_RANGE = 700;
	M_DIMEN = 9;
	fileNum = 0;
	curFileIndex = 0;

	m_combo.AddString(_T("1. 助教给出的方式"));
	m_combo.AddString(_T("2. HSV颜色矩方式"));
	m_combo.AddString(_T("3. 轮廓距方式"));
	m_combo.SetCurSel(0);

	HBITMAP   hBitmap; 
    hBitmap = (HBITMAP)::LoadImage( 
        NULL,
        _T("..\\src\\MFCApplication1\\MFCApplication1\\ezgif.com-resize.bmp"),   //图片全路径
        IMAGE_BITMAP,                         // 图片格式
        0,0, 
        LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    ((CButton *)GetDlgItem(IDC_GETLIST))->SetBitmap(hBitmap);
// IDC_GETLIST为按钮ID

	HBITMAP   hBitmap1;
	hBitmap1 = (HBITMAP)::LoadImage(
		NULL,
		_T("..\\src\\MFCApplication1\\MFCApplication1\\jiantou2.bmp"),   //图片全路径
		IMAGE_BITMAP,                         // 图片格式
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	((CButton *)GetDlgItem(IDC_BUTTON1))->SetBitmap(hBitmap1);

	HBITMAP   hBitmap2;
	hBitmap2 = (HBITMAP)::LoadImage(
		NULL,
		_T("..\\src\\MFCApplication1\\MFCApplication1\\jiantou1.bmp"),   //图片全路径
		IMAGE_BITMAP,                         // 图片格式
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	((CButton *)GetDlgItem(IDC_BUTTON2))->SetBitmap(hBitmap2);

	/////////////////////////////////////////
	
	for (int i = 0; i < 8; i++)
		setPicSize(i, 0, 0);

	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);

	////////////////////////////////////////

	CFont *m_Font;
	m_Font = new CFont;
	m_Font->CreateFont(0, 0, 0, 0, 0,
		FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_SWISS, _T("Arial"));
	CEdit *m_Edit = (CEdit *)GetDlgItem(IDC_EDIT1);
	m_Edit->SetFont(m_Font, FALSE);
	GetDlgItem(IDC_EDIT1)->SetFont(m_Font);

	/////////////////////////////////////////

	maxXSize = GetSystemMetrics(SM_CXSCREEN);
	maxYSize = GetSystemMetrics(SM_CYSCREEN);

	CRect rcTemp;
	rcTemp.BottomRight() = CPoint(maxXSize, maxYSize);
	rcTemp.TopLeft() = CPoint(0, 0);
	MoveWindow(&rcTemp);

	rateX = maxXSize / 1920;
	rateY = maxYSize / 1080;

	GetDlgItem(IDC_EDIT)->MoveWindow(683 * rateX, 208 * rateY, 494 * rateX, 49 * rateY);

	GetDlgItem(IDC_GETLIST)->MoveWindow((683 + 494)*rateX, 208 * rateY, 49 * rateX, 49 * rateY);

	GetDlgItem(IDC_PICTURE1)->MoveWindow(543 * rateX, 347 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE2)->MoveWindow(753 * rateX, 347 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE3)->MoveWindow(963 * rateX, 347 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE4)->MoveWindow(1173 * rateX, 347 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE5)->MoveWindow(543 * rateX, 557 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE6)->MoveWindow(753 * rateX, 557 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE7)->MoveWindow(963 * rateX, 557 * rateY, 0 * rateX, 0 * rateY);
	GetDlgItem(IDC_PICTURE8)->MoveWindow(1173 * rateX, 557 * rateY, 0 * rateX, 0 * rateY);

	GetDlgItem(IDC_BUTTON1)->MoveWindow(480 * rateX, 522 * rateY, 28 * rateX, 28 * rateY);
	GetDlgItem(IDC_BUTTON2)->MoveWindow(1386 * rateX, 522 * rateY, 28 * rateX, 28 * rateY);

	///////////////////////////

	edit2Text = _T("访问节点数 : 0");
	edit3Text = _T("平均查询结果 : 0");

	GetDlgItem(IDC_EDIT2)->SetWindowTextW(edit2Text);
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(edit3Text);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::My_DrawPicture(int index, CString fileName)
{
	CString file_address = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	CWnd* pWnd;

	switch (index%8)
	{
	case(0):
		pWnd = GetDlgItem(IDC_PICTURE1);//这里是获得控件句柄
		break;
	case(1):
		pWnd = GetDlgItem(IDC_PICTURE2);//这里是获得控件句柄
		break;
	case(2):
		pWnd = GetDlgItem(IDC_PICTURE3);//这里是获得控件句柄
		break;
	case(3):
		pWnd = GetDlgItem(IDC_PICTURE4);//这里是获得控件句柄
		break;
	case(4):
		pWnd = GetDlgItem(IDC_PICTURE5);//这里是获得控件句柄
		break;
	case(5):
		pWnd = GetDlgItem(IDC_PICTURE6);//这里是获得控件句柄
		break;
	case(6):
		pWnd = GetDlgItem(IDC_PICTURE7);//这里是获得控件句柄
		break;
	case(7):
		pWnd = GetDlgItem(IDC_PICTURE8);//这里是获得控件句柄
		break;
	default:
		pWnd = GetDlgItem(IDC_PICTURE1);//这里是获得控件句柄
		ERROR;
		break;
	}
	CDC* pDC = pWnd->GetDC();
	HDC hDC = pDC->m_hDC;

	CRect rect_frame;
	pWnd->GetClientRect(&rect_frame);
	CImage image1;//image是CImage类的一个对象，用来加载图像的
	file_address += fileName;
	image1.Load(file_address);

	::SetStretchBltMode(hDC, HALFTONE);
	::SetBrushOrgEx(hDC, 0, 0, NULL);

	image1.Draw(hDC, rect_frame);
	ReleaseDC(pDC);//释放picture控件的DC
}

void CMFCApplication1Dlg::setPicSize(int index,int width, int height)
{

	CRect pictureRect[8];
	CStatic* pStatic[8];
	width *= rateX;
	height *= rateY;

	switch (index%8)
	{
	case(0):
		pStatic[0] = (CStatic*)GetDlgItem(IDC_PICTURE1);
		GetDlgItem(IDC_PICTURE1)->GetWindowRect(&pictureRect[0]);
		ScreenToClient(pictureRect[0]);
		pStatic[0]->MoveWindow(pictureRect[0].left, pictureRect[0].top, width, height);
		break;
	case(1):
		pStatic[1] = (CStatic*)GetDlgItem(IDC_PICTURE2);
		GetDlgItem(IDC_PICTURE2)->GetWindowRect(&pictureRect[1]);
		ScreenToClient(pictureRect[1]);
		pStatic[1]->MoveWindow(pictureRect[1].left, pictureRect[1].top, width, height);
		break;
	case(2):
		pStatic[2] = (CStatic*)GetDlgItem(IDC_PICTURE3);
		GetDlgItem(IDC_PICTURE3)->GetWindowRect(&pictureRect[2]);
		ScreenToClient(pictureRect[2]);
		pStatic[2]->MoveWindow(pictureRect[2].left, pictureRect[2].top, width, height);
		break;
	case(3):
		pStatic[3] = (CStatic*)GetDlgItem(IDC_PICTURE4);
		GetDlgItem(IDC_PICTURE4)->GetWindowRect(&pictureRect[3]);
		ScreenToClient(pictureRect[3]);
		pStatic[3]->MoveWindow(pictureRect[3].left, pictureRect[3].top, width, height);
		break;
	case(4):
		pStatic[4] = (CStatic*)GetDlgItem(IDC_PICTURE5);
		GetDlgItem(IDC_PICTURE5)->GetWindowRect(&pictureRect[4]);
		ScreenToClient(pictureRect[4]);
		pStatic[4]->MoveWindow(pictureRect[4].left, pictureRect[4].top, width, height);
		break;
	case(5):
		pStatic[5] = (CStatic*)GetDlgItem(IDC_PICTURE6);
		GetDlgItem(IDC_PICTURE6)->GetWindowRect(&pictureRect[5]);
		ScreenToClient(pictureRect[5]);
		pStatic[5]->MoveWindow(pictureRect[5].left, pictureRect[5].top, width, height);
		break;
	case(6):
		pStatic[6] = (CStatic*)GetDlgItem(IDC_PICTURE7);
		GetDlgItem(IDC_PICTURE7)->GetWindowRect(&pictureRect[6]);
		ScreenToClient(pictureRect[6]);
		pStatic[6]->MoveWindow(pictureRect[6].left, pictureRect[6].top, width, height);
		break;
	case(7):
		pStatic[7] = (CStatic*)GetDlgItem(IDC_PICTURE8);
		GetDlgItem(IDC_PICTURE8)->GetWindowRect(&pictureRect[7]);
		ScreenToClient(pictureRect[7]);
		pStatic[7]->MoveWindow(pictureRect[7].left, pictureRect[7].top, width, height);
		break;
	default:
		ERROR;
		break;
	}

}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1Dlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting  
					   // TODO: 在此处添加消息处理程序代码  
					   // 不为绘图消息调用 CDialogEx::OnPaint()  

	CString strPath;

	strPath  += "..\\src\\MFCApplication1\\MFCApplication1\\background\\img1.jpg";

	CFile m_file(strPath, CFile::modeRead );
	//获取文件长度  
	DWORD m_filelen  = m_file.GetLength();
	//在堆上分配空间  
	HGLOBAL m_hglobal  = GlobalAlloc(GMEM_MOVEABLE, m_filelen);

	LPVOID pvdata  = NULL;
	//锁定堆空间,获取指向堆空间的指针  
	pvdata  = GlobalLock(m_hglobal);

	//将文件数据读区到堆中  
	m_file.Read(pvdata, m_filelen);

	IStream*  m_stream;

	GlobalUnlock(m_hglobal);

	//在堆中创建流对象  
	CreateStreamOnHGlobal(m_hglobal, TRUE, &m_stream);

	//利用流加载图像  
	OleLoadPicture(m_stream, m_filelen, TRUE, IID_IPicture, (LPVOID*)&m_picture);
	m_stream->Release();

	m_picture->get_Width(&m_width);// 宽高，MM_HIMETRIC 模式，单位是0.01毫米  
	m_picture->get_Height(&m_height);
	m_IsShow  = TRUE;
	m_file.Close();

	if (m_IsShow == TRUE)
	{
		CRect rect;
		GetClientRect(rect);
		int nW, nH;
		nW  = (int)(rect.Width());
		nH  = (int)(rect.Height());
		m_picture->Render(dc, 0, 0, nW, nH, 0, m_height, m_width, -m_height, &rect);
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedGetlist()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	UpdateData(false);
	if (flag)
	{
		AfxBeginThread(Thread2, this, THREAD_PRIORITY_IDLE);
		GetDlgItem(IDC_GETLIST)->EnableWindow(false);

		HBITMAP   hBitmap;
		hBitmap = (HBITMAP)::LoadImage(
			NULL,
			_T("..\\src\\MFCApplication1\\MFCApplication1\\ezgif.com-resize.bmp"),   //图片全路径
			IMAGE_BITMAP,                         // 图片格式
			0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		((CButton *)GetDlgItem(IDC_PICTURE1))->SetBitmap(hBitmap);

	}

	/*CDC* pDC;
	pDC = GetDC();
	ShowPicture(pDC, _T("image\\n02278980_1428.JPEG"), 543, 347, 175, 175);*/
}


BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (pMsg->wParam)
	{
	case VK_RETURN:
		UpdateData(true);
	case VK_ESCAPE:
		return true; break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//void CMFCApplication1Dlg::OnEnChangeList()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}


void CMFCApplication1Dlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


//void CMFCApplication1Dlg::OnDblclkList1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString tmp;
//	CString name;
//	int nIndex = m_list1.GetCurSel();
//	name = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
//	m_list1.GetText(nIndex, tmp);
//	name += tmp;
//	ShellExecute(0, _T("open"), name, NULL, _T("input"), SW_NORMAL);
//}


void CMFCApplication1Dlg::OnCbnSelchangeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	nIndex = m_combo.GetCurSel();

	switch (nIndex)
	{
	case 0:
	{
		M_DEMAND_RANGE = 700;
		M_DIMEN = 9;
		curFileIndex = 0;
		break;
	}
	case 1:
	{
		M_DEMAND_RANGE = 10;
		M_DIMEN = 9;
		curFileIndex = 0;
		break;
	}
	default:
	{
		M_DEMAND_RANGE = 3;
		M_DIMEN = 6;
		curFileIndex = 0;
		break;
	}
	}
}

void CMFCApplication1Dlg::OnStnClickedPicture1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-8];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-7];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-6];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-5];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-4];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-3];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture7()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-2];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnStnClickedPicture8()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path_ = _T("..\\src\\MFCApplication1\\MFCApplication1\\image\\");
	path_ += fileName[curFileIndex-1];
	ShellExecute(0, NULL, path_, NULL, NULL, SW_NORMAL);
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	int maxIndex = curFileIndex + 8;
	// TODO: 在此添加控件通知处理程序代码
	for (; curFileIndex < fileNum && curFileIndex < maxIndex; curFileIndex++)
		My_DrawPicture(curFileIndex, fileName[curFileIndex]);
	for (; curFileIndex < maxIndex; curFileIndex++)
		setPicSize(curFileIndex, 0, 0);

	if (curFileIndex == maxIndex && curFileIndex >= fileNum)
		GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	if (curFileIndex > 7)
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
}


void CMFCApplication1Dlg::OnBnClickedButton1()
{
	AfxBeginThread(Thread1, this, THREAD_PRIORITY_IDLE);
}

UINT CMFCApplication1Dlg::Thread1(void* param)
{
	CMFCApplication1Dlg *dlg = (CMFCApplication1Dlg*)param;
	int gap = dlg->curFileIndex - dlg->fileNum;
	// TODO: 在此添加控件通知处理程序代码
	dlg->curFileIndex -= 16;

	if (dlg->curFileIndex == 0)
		dlg->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);

	int maxIndex = dlg->curFileIndex + 8;

	if (gap < 0)
	{
		for (; dlg->curFileIndex < maxIndex; dlg->curFileIndex++)
			dlg->My_DrawPicture(dlg->curFileIndex, dlg->fileName[dlg->curFileIndex]);
	}
	else
	{
		for (; dlg->curFileIndex < maxIndex - gap; dlg->curFileIndex++)
			dlg->My_DrawPicture(dlg->curFileIndex, dlg->fileName[dlg->curFileIndex]);

		for (; dlg->curFileIndex < maxIndex; dlg->curFileIndex++)
		{
			dlg->setPicSize(dlg->curFileIndex, 175, 175);
			Sleep(300);
			dlg->My_DrawPicture(dlg->curFileIndex, dlg->fileName[dlg->curFileIndex]);
		}
	}

	if (dlg->fileNum > 8)
		dlg->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
	return 0;
}