#ifndef FEATURES_H
#define FEATURES_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<fstream>
#include<cmath>
#include "stdafx.h"

using namespace cv;

class features
{
private:
	TYPE average_X, average_Y;
	TYPE variance_X, variance_Y;
	TYPE skewness_X, skewness_Y;
	int num;
	TYPE image_size;
	TYPE dis_X, dis_Y, rate;

public:
	vector<Point2f> centerMoments;

public:
	features()
	{
		average_X = average_Y = 0;
		variance_X = variance_Y = 0;
		skewness_X = skewness_Y = 0;
		num = 0;
		image_size = 0;
		dis_X = dis_Y = 0;
		rate = 1;
	}
	features(int size, int isize, int m_rate)
	{
		centerMoments = vector<Point2f>(size);
		average_X = average_Y = 0;
		variance_X = variance_Y = 0;
		skewness_X = skewness_Y = 0;
		image_size = isize;
		num = size;
		dis_X = dis_Y = 0;
		rate = m_rate;
	}
	~features() {}

	void checkData();
	void average();
	void variance();
	void skewness();
	void selectPartData();
	void processData();
	void output();
	void outToFile(ofstream &out_txtFile);
};

void load_proc(string in_filename, string out_filename);

void extract_picture_byContours(string pic_name, ofstream &out_txtFile);

#endif // !FEATURES_H

