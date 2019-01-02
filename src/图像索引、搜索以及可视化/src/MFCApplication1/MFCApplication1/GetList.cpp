#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <iostream>
#include <vector>
#include <string>
// find all matched file in specified directory
// [INPUT]
//   >> const std::string& _path        Search path        ex) c:/directory/
//   >> const std::string& _filter        Search filter    ex) *.exe or *.*
// [RETURN]
//   >> std::vector<std::string>        All matched file name & extension

std::vector<std::string> get_files_inDirectory(const std::string& _path, const std::string& _filter)
{
	std::string searching  = _path  + _filter;

	std::vector<std::string> return_;

	_finddata_t fd;
	long handle  = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle  == -1)    return return_;

	int result  = 0;
	do 
	{
		return_.push_back(fd.name);
	result  = _findnext(handle, &fd);
	} while (result  != -1);

	_findclose(handle);

	return return_;
}

//UINT CMFCApplication1Dlg::Thread1(void* param)
//{
//	CMFCApplication1Dlg *dlg = (CMFCApplication1Dlg*)param;
//	CButton *listButton = (CButton*)dlg->GetDlgItem(IDC_GETLIST);
//	CListBox *listBox = (CListBox*)dlg->GetDlgItem(IDC_LIST1);
//	std::vector<std::string> V;
//	int vecNum;
//	CString str;
//	CString listStr;
//
//	if (dlg->flag)
//	{
//		dlg->flag = false;
//		listButton->EnableWindow(false);
//		V = get_files_inDirectory("imageFile/", "*.JPEG");
//
//		vecNum = V.size();
//
//		for (int i = 0; i < vecNum; i++)
//		{
//			str += V[i].c_str();
//			str += _T("\r\n");
//
//			listStr = V[i].c_str();
//			listBox->AddString(listStr);
//		}
//
//
//	}
//	return 0;
//}