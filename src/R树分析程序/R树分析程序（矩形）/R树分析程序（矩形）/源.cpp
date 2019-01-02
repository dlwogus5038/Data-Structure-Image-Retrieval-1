#include <stdio.h>
#include <memory.h>
#include <crtdbg.h>
#include<fstream>
#include<iostream>
using namespace std;
#include<string>
#include "RTree.h"


#define RANGE 3
#define N 100   //��ֵ�����޸�ͼƬ��ģ

int DIMEN;
int DEMAND_RANGE;


long long TOTALNUM = 0;   //���̷�����
double AVERAGE_VISIT = 0;    //ƽ���������
double EFFECTIVENESS = 0;     //������Ч��
double RELEVENT = 0;      //���������

//
// MemoryTest.cpp
//
// This demonstrates����ʾ�� a use of RTree
//

// Use CRT Debug facility����ʾ�����豸�� to dump memory leaks ��ת���ڴ�й©��on app exit��Ӧ�ý�����
#ifdef WIN32
  // These two are for MSVS 2005 security consciousness until safe std lib funcs are available
  #pragma warning(disable : 4996) // Deprecated functions���������Ĺ��ܣ�
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
// Note this is a low precision random value since it is generated from an int.   �����Ȳ�����
//

int GetDimen(string &str)
{
	int n;
	cout<<"��ѡ����������õ�������ʽ:"<<endl;
	cout<<"1,���̸�����"<<endl;
	cout<<"2,HSV��ɫ��"<<endl;
	cout<<"3,������"<<endl;
	cin>>n;
	switch(n)
	{
	case 1:
		{
			DEMAND_RANGE = 700;
			str = "color_feature2.txt";
			return 9;
		}
	case 2:
		{
			DEMAND_RANGE = 10;
			str = "color_feature.txt";
			return 9;
		}
	default:
		{
			DEMAND_RANGE = 3;
			str = "color_feature3.txt";
			return 6;
		}
	}
}
//���ݲ�ͬ������ѡ��ͬ�Ĳ������ļ�
int GetDimen2(string &str,int n)
{
	switch(n)
	{
	case 1:
		{
			DEMAND_RANGE = 700;
			str = "color_feature2.txt";
			return 9;
		}
	case 2:
		{
			DEMAND_RANGE = 10;
			str = "color_feature.txt";
			return 9;
		}
	default:
		{
			DEMAND_RANGE = 3;
			str = "color_feature3.txt";
			return 6;
		}
	}
}

static float RandFloat(float a_min, float a_max)
{
  const float ooMax = 1.0f / (float)(RAND_MAX+1);
  
  float retValue = ( (float)rand() * ooMax * (a_max - a_min) + a_min);

  ASSERT(retValue >= a_min && retValue < a_max); // Paranoid check  ������Ƿ��д�

  return retValue;
}


static float randfunction(int a_min,int a_max)
{
	return (a_min + rand()%(a_max-a_min));
}

struct Vec
{
	Vec() {v = new float[DIMEN];}


  Vec(float a[])
  {
	  v = new float[DIMEN];
	  for(int i=0;i<DIMEN;i++)
	  {
		  v[i] = a[i];
	  }
  }

  Vec operator+ (Vec& a_other)
  {
	  for(int i=0;i<DIMEN;i++)
	  {
		  a_other.v[i] += v[i];
	  }
	  return a_other;
  }  

  Vec operator- (Vec& a_other)
  {
	  for(int i=0;i<DIMEN;i++)
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
  for(int axis=0; axis<DIMEN; ++axis)   //��֤���������н���
  {
    if(a_boxMinA.v[axis] > a_boxMaxB.v[axis] ||
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

  int m_creationCounter;                          ///< Just a number for identifying within test program  ��ʶȷ��
  Vec m_min, m_max;                              ///< Minimal bounding rect, values must be known and constant in order to remove from RTree  
  //��С�߽��������ԭ������ĵ����Զ�ĵ�
  static int s_outstandingAllocs;                 ///< Count how many outstanding objects remain  ��ʣ�¶���δ����Ķ���
  string filename;
};

/// Init static
int SomeThing::s_outstandingAllocs = 0; //��ʼ��û��δ�������

int totalnum = 0;

/// A callback function to obtain query results in this implementation  ����������а�����ѯ����Ļص�����
bool _cdecl QueryResultCallback(SomeThing* a_data, void* a_context)
{
	totalnum += a_data->m_creationCounter;
    //printf("search found %d\n", a_data->m_creationCounter);  //�����ݵı�ʶ���
    return true;
}

int GetQueryPicNum(string &imagename)
{
	string txts;
	ifstream opennamefile("imagelist.txt",ios::in);
	if(opennamefile.fail())
	{
		cout<<"open imagelist.txt fail"<<endl;
		return -1;
	}
	int count = 0;
	while(!opennamefile.eof())
	{
		getline(opennamefile,txts);
		count++;
		if(txts == imagename)
		{
			return count;
		}
	}
	return -1;
}
//�õ�����ͼƬ�ľ�������
void GetVec(Vec &middle,int location,string &str)
{
	string s;
	ifstream openfile(str,ios::in);
	if(openfile.fail())
	{
		cout<<"open color_feature.txt fail"<<endl;
		return;
	}
	int count = 0;
	while(count<location)
	{
		getline(openfile,s);
		if(s[0]=='r')
			count++;
	}
	s = s + '\0';
	//float a[DIMEN]={0};
	float *a = new float[DIMEN];
	for(int i=0;i<DIMEN;i++)
		a[i] = 0;
	int minus = 0;
		   int decimal = 0;
		   int counts = 1;
		   for(int i=5,j=0;s[i]!='\0';i++)
		   {
			   if(s[i]=='-')
			   {
				   minus = 1;
				   continue;
			   }
			   if(s[i]=='.')
			   {
				   decimal = 1;
				   continue;
			   }
			   if(s[i]!=' ')
			   {
			      a[j] = a[j]*10 + s[i]-'0';
				  if(decimal == 1)
				  {
					  counts = 10*counts;
				  }
			   }
			   else
			   {
				   a[j] = a[j]/counts;
				   counts = 1;
				   decimal = 0;
				   if(minus==1)
				   {
					   a[j] = -a[j];
					   minus = 0;
				   }
				   j++;
				   if(j == DIMEN)
					   break;
			   }
		   }
		   a[DIMEN-1] = a[DIMEN-1]/counts;
		   if(minus==1)
			{
			    a[DIMEN-1] = -a[DIMEN-1];
			}
	middle = Vec(a);
	delete [] a;
}
//��ȡ����ͼƬ֮��������
double GetRelevent(Vec a1,Vec a2,Vec b1,Vec b2)
{
	//double a[DIMEN],b[DIMEN];
	double *a = new double[DIMEN],*b = new double[DIMEN];
	for(int i=0;i<DIMEN;i++)
	{
		a[i] = (a1.v[i] + a2.v[i])/2;
		b[i] = (b1.v[i] + b2.v[i])/2;
	}
	double relevent = 0;
	for(int i=0;i<DIMEN;i++)
	{
		relevent += (a[i]-b[i])*(a[i]-b[i]);
	}
	delete []a;
	delete []b;
	return relevent;
}

void Bubble_Sort(Image *image,int len)
{
	for(int i=0;i<len;i++)
	{
		for(int j=i+1;j<len;j++)
		{
			if(image[i].relevent>image[j].relevent)
			{
				swap(image[i],image[j]);
			} 
		}
	}
}

void printImage(Image *image,int len)
{
	string names = "C://Users/ASUS/Desktop/s/data/image/";
	string local = "image/";
	string tail = ".JPEG";
	char buffer[256];
	int n;
	for(int i=0;i<len;i++)
	{
		ifstream openfile(names+image[i].name,ios_base::in|ios_base::binary);
	    ofstream outfile(local+image[i].name,ios::out|ios::binary);
		if(openfile.fail())
	    {
		    cout<<"sds"<<endl;
		 //    return;
	    }
		while(!openfile.eof())
	    {
		    openfile.read(buffer,256);
		    n = openfile.gcount();
	  	    outfile.write(buffer,n);
	    }
		//cout<<"�ļ�����"<<image[i].name<<' '<<"��ضȣ�"<<image[i].relevent<<endl;
	}
}

typedef RTree<SomeThing*, float,6> SomeThingTree;
typedef RTree<SomeThing*, float,9> SomeThingTree2;

double getEffectiveness(Image *image,int count,string imagename)
{
	double n = 0;
	int flag = 1;
	for(int i=0;i<count;i++)
	{
		flag = 1;
		for(int j=0;image[i].name[j]!='_';j++)
		{
			if(image[i].name[j]!=imagename[j])
			{
				flag = 0;
				break;
			}
		}
		if(flag == 1)
		   n++;
	}
	return 100*n/count;
	//cout<<"������ȷƵ��Ϊ��"<<100*n/count<<"%"<<endl;
}

int makeprogram(int n)
{
	string imagefilename;
	DIMEN = GetDimen2(imagefilename,n);
  int index = 0; // general purpose counter, declared here because MSVC 6 is not ansi compliant with 'for' loops.

  SomeThingTree tree;
  SomeThingTree2 tree2;
  
  // Add some nodes
  int counter = 0;
   
  ifstream openfile(imagefilename,ios::in);
   if(openfile.fail())
   {
	   std::cout<<"open fail"<<endl;
	   return -1;
   }
   ifstream opennamefile("imagelist.txt",ios::in);
   if(opennamefile.fail())
   {
	   std::cout<<"open namefile fail"<<endl;
	   return -1;
   }
   string s;
   string name;
   float *a = new float[DIMEN],*b = new float[DIMEN];
   for(int i=0;i<DIMEN;i++)
   {
	   a[i] = 0;
	   b[i] = 2 * RANGE;
   }
   int j=0;
   int q = 0;

   while(q<N)
   {
    std::getline(openfile,s);
	s = s + '\0';
	if(s[0]!='r')
		continue;
	else if(s[0]=='r')
	   {
		   int minus = 0;
		   int decimal = 0;
		   int count = 1;
		   for(int i=5,j=0;s[i]!='\0';i++)
		   {
			   if(s[i]=='-')
			   {
				   minus = 1;
				   continue;
			   }
			   if(s[i]=='.')
			   {
				   decimal = 1;
				   continue;
			   }
			   if(s[i]!=' ')
			   {
			      a[j] = a[j]*10 + s[i]-'0';
				  if(decimal == 1)
				  {
					  count = 10*count;
				  }
			   }
			   else
			   {
				   a[j] = a[j]/count;
				   if(minus==1)
				   {
					   a[j] = -a[j];
					   minus = 0;
				   }
				   a[j] -= RANGE;
				   count = 1;
				   decimal = 0;
				   j++;
				   if(j==8)
					   int e =4;
				   if(j == DIMEN)
					   break;
			   }
		   }
		   a[DIMEN-1] = a[DIMEN-1]/count;
		   if(minus==1)
			{
			    a[DIMEN-1] = -a[DIMEN-1];
			}
		   a[DIMEN-1] -= RANGE;
	   }
	SomeThing* newThing = new SomeThing;
    newThing->m_creationCounter = counter++;
	getline(opennamefile,name);
	newThing->filename = name;
	newThing->m_min = Vec(a);
	Vec extent  = Vec(b);
	newThing->m_max = newThing->m_min + extent;

	if(DIMEN == 6)
        tree.Insert(newThing->m_min.v, newThing->m_max.v, newThing);
	else if(DIMEN == 9)
		tree2.Insert(newThing->m_min.v, newThing->m_max.v, newThing);
	for(int i=0;i<DIMEN;i++)
		a[i] = 0;
	q++;
  }
  

    string namerr = "n03767203_3133.JPEG";
	ifstream opennamefilse("imagelist.txt",ios::in);
    if(opennamefilse.fail())
    {
	   std::cout<<"open namefile fail"<<endl;
	   return -1;
    }
	int i = 0;
	while(i<N)
	{
		getline(opennamefilse,namerr);
		int location = GetQueryPicNum(namerr);
   Vec middle;
   GetVec(middle,location,imagefilename);
   for(int i=0;i<DIMEN;i++)
   {
	   a[i] = middle.v[i]-DEMAND_RANGE;
	   b[i] = middle.v[i]+DEMAND_RANGE;
   }

   Vec searchMin (a);
   Vec searchMax(b);

   if(DIMEN == 6)
   {
       tree.Search(searchMin.v, searchMax.v, &QueryResultCallback, NULL);
	   SomeThingTree::Iterator it;
       int count = 0;
       Image image[6000];
       for( tree.GetFirst(it); !tree.IsNull(it); tree.GetNext(it) )
       {
            SomeThing* curThing = tree.GetAt(it);
            if(BoxesIntersect(searchMin, searchMax, curThing->m_min, curThing->m_max))
            {
		         image[count].relevent = GetRelevent(searchMin,searchMax,curThing->m_min,curThing->m_max);
		         image[count].name = curThing->filename;
		         count++;
            }
        }
        Bubble_Sort(image,count);
        printImage(image,count);
		if(image[1].name!="")
		    RELEVENT += image[1].relevent;
		EFFECTIVENESS += getEffectiveness(image,count,namerr);
		TOTALNUM += totalnum;
		AVERAGE_VISIT += totalnum/count;
   }
   else
   {
	   tree2.Search(searchMin.v, searchMax.v, &QueryResultCallback, NULL);
	   SomeThingTree2::Iterator it;
       int count = 0;
       Image image[6000];
       for( tree2.GetFirst(it); !tree2.IsNull(it); tree2.GetNext(it) )
       {
            SomeThing* curThing = tree2.GetAt(it);
            if(BoxesIntersect(searchMin, searchMax, curThing->m_min, curThing->m_max))
            {
		         image[count].relevent = GetRelevent(searchMin,searchMax,curThing->m_min,curThing->m_max);
		         image[count].name = curThing->filename;
		         count++;
            }
        }
        Bubble_Sort(image,count);
        printImage(image,count);
		if(image[1].name!="")
		     RELEVENT += image[1].relevent;
		EFFECTIVENESS += getEffectiveness(image,count,namerr);
		TOTALNUM += totalnum;
		AVERAGE_VISIT += totalnum/count;
   }
		i++;
	}
}

int main(int argc, char* argv[])
{
	int n;
	//���������ֱ����
	for(int n = 1;n<=3;n++)
	{
	   makeprogram(n);
	   cout<<"���̷�������"<<TOTALNUM<<endl;
	   cout<<"ƽ����ѯ�����"<<AVERAGE_VISIT/(N)<<endl;
	   cout<<"����������ȷ�ʣ�"<<EFFECTIVENESS/(N)<<"%"<<endl;
	   cout<<"����ԣ�"<<RELEVENT/N<<endl;
	   cout<<endl;
	   TOTALNUM  = 0;
	   AVERAGE_VISIT = 0;
	   EFFECTIVENESS = 0;
	   RELEVENT = 0;
	}
	
#ifdef WIN32
  // Use CRT Debug facility to dump memory leaks on app exit
  SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );
#endif //WIN32

    return 0;
}