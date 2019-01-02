#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "RTree.h"
#include <stdio.h>
#include <memory.h>
#include <crtdbg.h>
#include<fstream>
#include<iostream>
using namespace std;
#include<string>
#include <vector>

int RANGE = 2;
//#define DEMAND_RANGE 10
int DIMEN;
int DEMAND_RANGE;

//
// MemoryTest.cpp
//
// This demonstrates（鞫刻） a use of RTree
//

// Use CRT Debug facility（鞫刻딧桿구） to dump memory leaks （瘻닸코닸橘짤）on app exit（壇痰써監）
#ifdef WIN32
// These two are for MSVS 2005 security consciousness until safe std lib funcs are available
#pragma warning(disable : 4996) // Deprecated functions（굳록폴돨묘콘）
#define _CRT_SECURE_NO_DEPRECATE // Allow old unsecure standard library functions, Disable some 'warning C4996 - function was deprecated'

// The following macros set and clear, respectively, given bits
// of the C runtime library debug flag, as specified by a bitmask.
#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
              _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) \
              _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif
#endif //WIN32

//
// Get a random float b/n two values
// The returned value is >= min && < max (exclusive of max)
// Note this is a low precision random value since it is generated from an int.   （쑹똑꼇뭘）
//

int GetStr(string &str)
{

	if (DEMAND_RANGE == 700)
	{
		str = "..\\src\\MFCApplication1\\MFCApplication1\\color_feature2.txt";
		RANGE = 2;
	}
	else if (DEMAND_RANGE == 10)
	{
		str = "..\\src\\MFCApplication1\\MFCApplication1\\color_feature.txt";
		RANGE = 2;
	}
	else if (DEMAND_RANGE == 3)
	{
		str = "..\\src\\MFCApplication1\\MFCApplication1\\color_feature3.txt";
		RANGE = 10;
	}
	else
		return false;

	return true;
}
static float RandFloat(float a_min, float a_max)
{
	const float ooMax = 1.0f / (float)(RAND_MAX + 1);

	float retValue = ((float)rand() * ooMax * (a_max - a_min) + a_min);

	ASSERT(retValue >= a_min && retValue < a_max); // Paranoid check  （쇱꿎角뤠唐댄）

	return retValue;
}


static float randfunction(int a_min, int a_max)
{
	return (a_min + rand() % (a_max - a_min));
}

struct Vec
{
	Vec() { v = new float[DIMEN]; }


	Vec(float a[])
	{
		v = new float[DIMEN];
		for (int i = 0; i<DIMEN; i++)
		{
			v[i] = a[i];
		}
	}

	Vec operator+ (Vec& a_other)
	{
		for (int i = 0; i<DIMEN; i++)
		{
			a_other.v[i] += v[i];
		}
		return a_other;
	}

	Vec operator- (Vec& a_other)
	{
		for (int i = 0; i<DIMEN; i++)
		{
			a_other.v[i] = v[i] - a_other.v[i];
		}
		return a_other;
	}

	float *v;
};

struct Image
{
	string name;
	double relevent;
};

static bool BoxesIntersect(const Vec& a_boxMinA, const Vec& a_boxMaxA,
	const Vec& a_boxMinB, const Vec& a_boxMaxB)
{
	for (int axis = 0; axis<DIMEN; ++axis)   //괏聯좃몸앤近唐슥섞
	{
		if (a_boxMinA.v[axis] > a_boxMaxB.v[axis] ||
			a_boxMaxA.v[axis] < a_boxMinB.v[axis])
		{
			return false;
		}
	}
	return true;
}

struct SomeThing
{
	SomeThing()
	{
		++s_outstandingAllocs;
	}
	~SomeThing()
	{
		--s_outstandingAllocs;
	}

	int m_creationCounter;                          ///< Just a number for identifying within test program  깃街횅땍
	Vec m_min, m_max;                              ///< Minimal bounding rect, values must be known and constant in order to remove from RTree  
												   //離鬼긋썹앤近櫓잼覩듐離쐤돨듐뵨離陶돨듐
	static int s_outstandingAllocs;                 ///< Count how many outstanding objects remain  뻘假苟뜩灌썩엄돨뚤蹶
	string filename;
};

/// Init static
int SomeThing::s_outstandingAllocs = 0; //놓迦뺏청唐灌썩엄뚤蹶

int total = 0;
										/// A callback function to obtain query results in this implementation  瞳侶몸꾸鱗櫓관벵꿴璂써벎돨쀼딧변鑒
bool _cdecl QueryResultCallback(SomeThing* a_data, void* a_context)
{
	total += a_data->m_creationCounter;
	//printf("search found %d\n", a_data->m_creationCounter);  //쉥鑒앴돨깃街渴놔

	return true;
}

int GetQueryPicNum(string &s)
{
	string txts;
	ifstream opennamefile("..\\src\\MFCApplication1\\MFCApplication1\\imagelist.txt", ios::in);
	if (opennamefile.fail())
	{
		cout << "open imagelist.txt fail" << endl;
		return -1;
	}
	int count = 0;
	while (!opennamefile.eof())
	{
		getline(opennamefile, txts);
		count++;
		if (txts == s)
		{
			return count;
		}
	}
	return -1;
}

void GetVec(Vec &middle, int location, string &str)
{
	string s;
	ifstream openfile(str, ios::in);
	if (openfile.fail())
	{
		cout << "open color_feature.txt fail" << endl;
		return;
	}
	int count = 0;
	while (count<location)
	{
		getline(openfile, s);
		if (s[0] == 'r')
			count++;
	}
	s = s + '\0';
	//float a[DIMEN]={0};
	float *a = new float[DIMEN];
	for (int i = 0; i<DIMEN; i++)
		a[i] = 0;
	int minus = 0;
	int decimal = 0;
	int counts = 1;
	for (int i = 5, j = 0; s[i] != '\0'; i++)
	{
		if (s[i] == '-')
		{
			minus = 1;
			continue;
		}
		if (s[i] == '.')
		{
			decimal = 1;
			continue;
		}
		if (s[i] != ' ')
		{
			a[j] = a[j] * 10 + s[i] - '0';
			if (decimal == 1)
			{
				counts = 10 * counts;
			}
		}
		else
		{
			a[j] = a[j] / counts;
			counts = 1;
			decimal = 0;
			if (minus == 1)
			{
				a[j] = -a[j];
				minus = 0;
			}
			j++;
			if (j == DIMEN)
				break;
		}
	}
	a[DIMEN - 1] = a[DIMEN - 1] / counts;
	if (minus == 1)
	{
		a[DIMEN - 1] = -a[DIMEN - 1];
	}
	middle = Vec(a);
}

double GetRelevent(Vec a1, Vec a2, Vec b1, Vec b2)
{
	//double a[DIMEN],b[DIMEN];
	double *a = new double[DIMEN], *b = new double[DIMEN];
	for (int i = 0; i<DIMEN; i++)
	{
		a[i] = (a1.v[i] + a2.v[i]) / 2;
		b[i] = (b1.v[i] + b2.v[i]) / 2;
	}
	double relevent = 0;
	for (int i = 0; i<DIMEN; i++)
	{
		relevent += (a[i] - b[i])*(a[i] - b[i]);
	}
	return relevent;
}

void Bubble_Sort(Image *image, int len)
{
	for (int i = 0; i<len; i++)
	{
		for (int j = i + 1; j<len; j++)
		{
			if (image[i].relevent>image[j].relevent)
			{
				swap(image[i], image[j]);
			}
		}
	}
}

void printImage(Image *image, int len)
{
	string names = "..\\src\\MFCApplication1\\MFCApplication1\\image\\";
	string local = "..\\src\\MFCApplication1\\MFCApplication1\\outputImage\\";
	string tail = ".JPEG";
	char buffer[256];
	int n;
	for (int i = 0; i<len; i++)
	{
		ifstream openfile(names + image[i].name, ios_base::in | ios_base::binary);
		ofstream outfile(local + image[i].name, ios::out | ios::binary);
		if (openfile.fail())
		{
			//cout << "sds" << endl;
			//    return;
		}
		while (!openfile.eof())
		{
			openfile.read(buffer, 256);
			n = openfile.gcount();
			outfile.write(buffer, n);
		}

		//cout << "匡숭츰：" << image[i].name << ' ' << "宮밑똑：" << image[i].relevent << endl;
	}
}

UINT CMFCApplication1Dlg::Thread2(void* param)
{
	CMFCApplication1Dlg *dlg = (CMFCApplication1Dlg*)param;
	CButton *listButton = (CButton*)dlg->GetDlgItem(IDC_GETLIST);

		string imagefilename;
		string str = CStringA(dlg->m_edit);

		if (dlg->curFileIndex == 0)
		{
			dlg->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
			dlg->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
		}

		DIMEN = dlg->M_DIMEN;
		DEMAND_RANGE = dlg->M_DEMAND_RANGE;
		GetStr(imagefilename);
		const int NUM_OBJECTS = 5613; // Number of objects in test set, must be > FRAC_OBJECTS for this test
		const int FRAC_OBJECTS = 4;
		// typedef the RTree useage just for conveniance with iteration
		typedef RTree<SomeThing*, float, 6> SomeThingTree;
		if (DIMEN == 9)
			typedef RTree<SomeThing*, float, 9> SomeThingTree;

		ASSERT(NUM_OBJECTS > FRAC_OBJECTS);

		int index = 0; // general purpose counter, declared here because MSVC 6 is not ansi compliant with 'for' loops.
		SomeThing* thingArray[NUM_OBJECTS * 2]; // Store objects in another container to test with, sized larger than we need

		memset(thingArray, 0, sizeof(thingArray)); // Nullify array, size is known here


												   // Create intance of RTree   뭐芚R疳茄절

		SomeThingTree tree;

		// Add some nodes
		int counter = 0;

		ifstream openfile(imagefilename, ios::in);
		if (openfile.fail())
		{
			//std::cout << "open fail" << endl;
			return -1;
		}
		ifstream opennamefile("..\\src\\MFCApplication1\\MFCApplication1\\imagelist.txt", ios::in);
		if (opennamefile.fail())
		{
			//std::cout << "open namefile fail" << endl;
			return -1;
		}
		string s;
		string name;
		//float a[DIMEN]={0},b[DIMEN];
		float *a = new float[DIMEN], *b = new float[DIMEN];
		for (int i = 0; i<DIMEN; i++)
		{
			a[i] = 0;
			b[i] = 2 * RANGE;
		}
		int j = 0;

		while (!openfile.eof())
		{
			std::getline(openfile, s);
			s = s + '\0';
			if (s[0] != 'r')
				continue;
			else if (s[0] == 'r')
			{
				int minus = 0;
				int decimal = 0;
				int count = 1;
				for (int i = 5, j = 0; s[i] != '\0'; i++)
				{
					if (s[i] == '-')
					{
						minus = 1;
						continue;
					}
					if (s[i] == '.')
					{
						decimal = 1;
						continue;
					}
					if (s[i] != ' ')
					{
						a[j] = a[j] * 10 + s[i] - '0';
						if (decimal == 1)
						{
							count = 10 * count;
						}
					}
					else
					{
						a[j] = a[j] / count;
						if (minus == 1)
						{
							a[j] = -a[j];
							minus = 0;
						}
						a[j] -= RANGE;
						count = 1;
						decimal = 0;
						j++;
						if (j == 8)
							int e = 4;
						if (j == DIMEN)
							break;
					}
				}
				a[DIMEN - 1] = a[DIMEN - 1] / count;
				if (minus == 1)
				{
					a[DIMEN - 1] = -a[DIMEN - 1];
				}
				a[DIMEN - 1] -= RANGE;
			}
			SomeThing* newThing = new SomeThing;
			newThing->m_creationCounter = counter++;
			getline(opennamefile, name);
			newThing->filename = name;
			newThing->m_min = Vec(a);
			Vec extent = Vec(b);
			newThing->m_max = newThing->m_min + extent;

			thingArray[counter - 1] = newThing;   //쉥돤돕돨踞샙令립令못thingArray

			tree.Insert(newThing->m_min.v, newThing->m_max.v, newThing);
			for (int i = 0; i<DIMEN; i++)
				a[i] = 0;
			/*std::cout<<"min：( ";
			for(int i=0;i<DIMEN;i++)
			{
			std::cout<<newThing->m_min.v[i];
			if(i<DIMEN-1)
			std::cout<<",";
			}
			std::cout<<" ) max：( ";
			for(int i=0;i<DIMEN;i++)
			{
			std::cout<<newThing->m_max.v[i];
			if(i<DIMEN-1)
			std::cout<<",";
			}
			std::cout<<" )"<<endl;*/

		}

		int location = GetQueryPicNum(str);
		Vec middle;
		GetVec(middle, location, imagefilename);
		for (int i = 0; i<DIMEN; i++)
		{
			a[i] = middle.v[i] - DEMAND_RANGE;
			b[i] = middle.v[i] + DEMAND_RANGE;
		}
		Vec searchMin(a);
		Vec searchMax(b);
		tree.Search(searchMin.v, searchMax.v, &QueryResultCallback, NULL);

		SomeThingTree::Iterator it;
		int count = 0;
		Image image[6000];
		for (tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it))
		{
			SomeThing* curThing = tree.GetAt(it);
			if (BoxesIntersect(searchMin, searchMax, curThing->m_min, curThing->m_max))
			{
				image[count].relevent = GetRelevent(searchMin, searchMax, curThing->m_min, curThing->m_max);
				image[count].name = curThing->filename;
				/*cout<<"min：( ";
				for(int i=0;i<DIMEN;i++)
				{
				cout<<curThing->m_min.v[i];
				if(i<DIMEN-1)
				cout<<",";
				}
				cout<<" ) max：( ";
				for(int i=0;i<DIMEN;i++)
				{
				cout<<curThing->m_max.v[i];
				if(i<DIMEN-1)
				cout<<",";
				}
				cout<<" )"<<endl;*/
				//cout<<image[count].name<<" "<<image[count].relevent<<endl;
				count++;
				if (count == 10)
					int r = 0;
			}
		}
		dlg->fileNum = count;
		Bubble_Sort(image, count);
		printImage(image, count);

		CString stringNum;
		//访问节点数 total
		dlg->edit2Text = _T("访问节点数 : ");
		stringNum.Format(_T("%d"), total);
		dlg->edit2Text += stringNum;
		dlg->edit3Text = _T("平均查询结果 : ");
		stringNum.Format(_T("%d"), total/count);
		dlg->edit3Text += stringNum;
		//平均查询结果 total/count

		dlg->GetDlgItem(IDC_EDIT2)->SetWindowTextW(dlg->edit2Text);
		dlg->GetDlgItem(IDC_EDIT3)->SetWindowTextW(dlg->edit3Text);

		total = 0;

#ifdef WIN32
		// Use CRT Debug facility to dump memory leaks on app exit
		SET_CRT_DEBUG_FIELD(_CRTDBG_LEAK_CHECK_DF);
#endif //WIN32

		dlg->GetDlgItem(IDC_GETLIST)->EnableWindow(true);

		for (int i = 0; i < count; i++)
			dlg->fileName[i] = image[i].name.c_str();

		for (dlg->curFileIndex = 0; dlg->curFileIndex < dlg->fileNum 
			&& dlg->curFileIndex < 8; dlg->curFileIndex++)
		{
			dlg->setPicSize(dlg->curFileIndex, 175, 175);
			Sleep(300);
			dlg->My_DrawPicture(dlg->curFileIndex, dlg->fileName[dlg->curFileIndex]);
	
		}
		for (; dlg->curFileIndex < 8; dlg->curFileIndex++)
			dlg->setPicSize(dlg->curFileIndex, 0, 0);
		

		if (dlg->fileNum > 8)
			dlg->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);

		///////////////////////////////////////////////////////
		

	return 0;
}