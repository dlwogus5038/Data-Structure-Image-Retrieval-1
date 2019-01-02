// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <iostream>
#include <tchar.h>
#include <fstream>
#include <string>
#include <cmath>
#include "atlimage.h"
using namespace std;

#define MIN __min
#define MAX __max
#define TYPE double

// TODO: reference additional headers your program requires here

class HSV
{
private:
	TYPE H;
	TYPE S;
	TYPE V;

public:
	HSV()
	{
		H = S = V = 0;
	}
	~HSV() {}

	void RGBtoHSV(BYTE r, BYTE g, BYTE b);
	void assignValue(BYTE r, BYTE g, BYTE b)
	{
		RGBtoHSV(r, g, b);
	}
	TYPE getH()
	{
		return H;
	}
	TYPE getS()
	{
		return S;
	}
	TYPE getV()
	{
		return V;
	}

};

class Features
{
private:
	HSV **Array;
	int height_num;
	int width_num;

	TYPE average_H;
	TYPE average_S;
	TYPE average_V;

	TYPE variance_H;
	TYPE variance_S;
	TYPE variance_V;

	TYPE skewness_H;
	TYPE skewness_S;
	TYPE skewness_V;

public:
	Features()
	{
		Array = NULL;
		height_num = width_num = 0;
		average_H = average_S = average_V = 0;
		variance_H = variance_S = variance_V = 0;
		skewness_H = skewness_S = skewness_V = 0;
	}
	Features(HSV **colorArray, int iHeight, int iWidth)
	{
		Array = colorArray;
		height_num = iHeight;
		width_num = iWidth;
		average_H = average_S = average_V = 0;
		variance_H = variance_S = variance_V = 0;
		skewness_H = skewness_S = skewness_V = 0;
	}
	~Features() {}

	void average();
	void variance();
	void skewness();
	void extract();
	void output();
	void outToFile(ofstream &out_txtFile);
};

void extract_HSV_features(string in_filename, string out_filename);

void extract_picture_byHSV(string pic_name, ofstream &out_txtFile);