
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetlist();
	static UINT Thread1(void *param);
	static UINT Thread2(void *param);
	//void CMFCApplication1Dlg::ShowPicture(CDC *pDC, CString m_strBRoute, int x, int y, int width, int height);
	void CMFCApplication1Dlg::setPicSize(int index,int width, int height);
	void CMFCApplication1Dlg::My_DrawPicture(int index,CString fileName);
	bool flag;
	CButton m_getList;
//	CEdit m_list;
//	afx_msg void OnEnChangeList();
	afx_msg void OnLbnSelchangeList1();
	//afx_msg void OnDblclkList1();
//	CListBox m_list1;
	IPicture *m_picture;
	OLE_XSIZE_HIMETRIC m_width;
	OLE_YSIZE_HIMETRIC m_height;
	BOOL m_IsShow;
	afx_msg void OnCbnSelchangeCombo();
	CComboBox m_combo;
	int M_DIMEN;
	int M_DEMAND_RANGE;
	int nIndex;
	CString m_edit;
	CStatic m_picture1;
	CString fileName[6000];
	int fileNum;
	int curFileIndex;
	afx_msg void OnStnClickedPicture1();
	afx_msg void OnStnClickedPicture2();
	afx_msg void OnStnClickedPicture3();
	afx_msg void OnStnClickedPicture4();
	afx_msg void OnStnClickedPicture5();
	afx_msg void OnStnClickedPicture6();
	afx_msg void OnStnClickedPicture7();
	afx_msg void OnStnClickedPicture8();
//	CButton m_button1;
//	CButton m_button2;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	float rateX, rateY;
	float maxXSize, maxYSize;
	CString edit2Text,edit3Text;
};
