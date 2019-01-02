// stdafx.cpp : source file that includes just the standard includes
// LoadImage.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void HSV::RGBtoHSV(BYTE r, BYTE g, BYTE b)
{
	BYTE min, max;
	BYTE delta;
	min = MIN(MIN(r, g), b);
	max = MAX(MAX(r, g), b);
	V = (TYPE)max; // v
	delta = max - min;

	S = (max == 0) ? 0 : (TYPE)delta / max;

	if (delta == 0)
		H = 0;
	else if (max == r && g >= b)
		H = (TYPE)(g - b) / delta * 60;
	else if (max == r && g < b)
		H = (TYPE)(g - b) / delta * 60 + 360;
	else if (max == g)
		H = (TYPE)(b - r) / delta * 60 + 120;
	else
		H = (TYPE)(r - g) / delta * 60 + 240;
}

void Features::average()
{
	TYPE sum_H = 0, sum_S = 0, sum_V = 0;
	TYPE sum = width_num * height_num;
	for (int i = 0; i < width_num; i++)
		for (int j = 0; j < height_num; j++)
		{
			sum_H = sum_H + Array[i][j].getH();
			sum_S = sum_S + Array[i][j].getS();
			sum_V = sum_V + Array[i][j].getV();
		}
	average_H = sum_H / sum;
	average_S = sum_S / sum;
	average_V = sum_V / sum;

}

void Features::variance()
{
	TYPE sum_H = 0, sum_S = 0, sum_V = 0;
	for (int i = 0; i < width_num; i++)
		for (int j = 0; j < height_num; j++)
		{
			sum_H = sum_H + pow(Array[i][j].getH() - average_H, 2);
			sum_S = sum_S + pow(Array[i][j].getS() - average_S, 2);
			sum_V = sum_V + pow(Array[i][j].getV() - average_V, 2);
		}
	TYPE sum = width_num * height_num;
	variance_H = sqrt(sum_H / sum);
	variance_S = sqrt(sum_S / sum);
	variance_V = sqrt(sum_V / sum);

}

void Features::skewness()
{
	TYPE sum_H = 0, sum_S = 0, sum_V = 0;
	for (int i = 0; i < width_num; i++)
		for (int j = 0; j < height_num; j++)
		{
			sum_H = sum_H + pow(Array[i][j].getH() - average_H, 3);
			sum_S = sum_S + pow(Array[i][j].getS() - average_S, 3);
			sum_V = sum_V + pow(Array[i][j].getV() - average_V, 3);
		}
	TYPE sum = width_num * height_num;
	skewness_H = pow(abs(sum_H / sum), (TYPE)1 / 3);
	if (sum_H <= 0)
		skewness_H = -skewness_H;
	skewness_S = pow(abs(sum_S / sum), (TYPE)1 / 3);
	if (sum_S <= 0)
		skewness_S = -skewness_S;
	skewness_V = pow(abs(sum_V / sum), (TYPE)1 / 3);
	if (sum_V <= 0)
		skewness_V = -skewness_V;

}

void Features::extract()
{
	average();
	variance();
	skewness();
}

void Features::output()
{
	cout << "[" << average_H << " " << variance_H << " " << skewness_H <<
		" " << average_S << " " << variance_S << " " << skewness_S <<
		" " << average_V << " " << variance_V << " " << skewness_V << "]" << endl;
}

void Features::outToFile(ofstream &out_txtFile)
{
	out_txtFile << "rect " << average_H << " " << variance_H << " " << skewness_H <<
		" " << average_S << " " << variance_S << " " << skewness_S <<
		" " << average_V << " " << variance_V << " " << skewness_V;
}

void extract_picture_byHSV(string pic_name, ofstream &out_txtFile)
{
	CImage image;
	int iHeight = 0, iWidth = 0;
	BYTE byteR, byteG, byteB;

	image.Load(pic_name.c_str());

	iHeight = image.GetHeight();
	iWidth = image.GetWidth();
	HSV **colorArray = new HSV*[iWidth];
	for (int i = 0; i < iWidth; i++)
		colorArray[i] = new HSV[iHeight];

	for (int iRow = 0; iRow < iWidth; iRow++)
		for (int iCol = 0; iCol < iHeight; iCol++)
		{
			COLORREF colorref = image.GetPixel(iRow, iCol);

			if (colorref == CLR_INVALID)
			{
				printf("Out of range!");
				continue;
			}

			byteR = GetRValue(colorref);
			byteG = GetGValue(colorref);
			byteB = GetBValue(colorref);
			colorArray[iRow][iCol].assignValue(byteR, byteG, byteB);
		}

	Features *feature = new Features(colorArray, iHeight, iWidth);
	feature->extract();
	feature->outToFile(out_txtFile);

	image.GetBits();

	image.Destroy();

	for (int i = 0; i < iWidth; i++)
		delete[] colorArray[i];
	delete[] colorArray;
	delete feature;
}

void extract_HSV_features(string in_filename, string out_filename)
{
	ofstream out_txtFile(out_filename);
	out_txtFile << "tech nmos<< polysilicon >>";
	ifstream in_txtFile(in_filename);

	if (!in_txtFile.is_open())
	{
		cout << "Error opening file";
		exit(-1);
	}

	while (!in_txtFile.eof())
	{
		string pic_name = "image\\";
		string temp;
		in_txtFile >> temp;
		pic_name += temp;

		extract_picture_byHSV(pic_name, out_txtFile);
	}
	out_txtFile << "<< end >>";
	out_txtFile.close();

	in_txtFile.close();
}