#include"features.h"
//#include "stdafx.h"

RNG rng(12345);
void features::checkData()
{
	int k = 0;
	for (int i = 0; i < num; i++)
	{
		if (!_isnan(centerMoments[i].x) &&!_isnan(centerMoments[i].y))
			centerMoments[k++] = centerMoments[i];
	}
	num = k;
}

void features::average()
{
	TYPE sum_X = 0, sum_Y = 0;
	for (int i = 0; i < num; i++)
	{
		sum_X += centerMoments[i].x;
		sum_Y += centerMoments[i].y;
	}
	average_X = sum_X / num;
	average_Y = sum_Y / num;
}

void features::variance()
{
	TYPE sum_X = 0, sum_Y = 0;
	for (int i = 0; i < num; i++)
	{
		sum_X += pow(centerMoments[i].x - average_X, 2);
		sum_Y += pow(centerMoments[i].y - average_Y, 2);
	}
	variance_X = sqrt(sum_X / num);
	variance_Y = sqrt(sum_Y / num);
}

void features::skewness()
{
	TYPE sum_X = 0, sum_Y = 0;
	for (int i = 0; i < num; i++)
	{
		sum_X += pow(centerMoments[i].x - average_X, 3);
		sum_Y += pow(centerMoments[i].y - average_Y, 3);
	}
	skewness_X = pow(abs(sum_X / num), (TYPE)1 / 3);
	if (sum_X <= 0)
		skewness_X = -skewness_X;
	skewness_Y = pow(abs(sum_Y / num), (TYPE)1 / 3);
	if (sum_Y <= 0)
		skewness_Y = -skewness_Y;
}

void features::selectPartData()
{
	int k = 0;
	for (int i = 0; i < num; i++)
	{
		if (abs(centerMoments[i].x - average_X) <= dis_X * rate &&
			abs(centerMoments[i].y - average_Y) <= dis_Y * rate)
			centerMoments[k++] = centerMoments[i];
	}
	num = k;
}

void features::processData()
{
	TYPE min_X = INT_MAX, max_X = 0, min_Y = INT_MAX, max_Y = 0;
	checkData();
	for (int i = 0; i < num; i++)
	{
		if (centerMoments[i].x < min_X)
			min_X = centerMoments[i].x;
		if (centerMoments[i].x > max_X)
			max_X = centerMoments[i].x;
		if (centerMoments[i].y < min_Y)
			min_Y = centerMoments[i].y;
		if (centerMoments[i].y > max_Y)
			max_Y = centerMoments[i].y;
	}
	dis_X = max_X - min_X;
	dis_Y = max_Y - min_Y;
	average();
	selectPartData();
	average();
	variance();
	skewness();
}

void features::output()
{
	cout << "[" << average_X / sqrt(image_size) << " " << variance_X / sqrt(image_size) << " " << skewness_X / sqrt(image_size) << " " <<
		average_Y / sqrt(image_size) << " " << variance_Y / sqrt(image_size) << " " << skewness_Y / sqrt(image_size) << "]" << endl;
}

void features::outToFile(ofstream &out_txtFile)
{
	out_txtFile << " " << average_X << " " << variance_X / sqrt(image_size) << " " << skewness_X / sqrt(image_size) << " " <<
		average_Y << " " << variance_Y / sqrt(image_size) << " " << skewness_Y / sqrt(image_size) << endl;
}

void extract_picture_byContours(string pic_name, ofstream &out_txtFile)
{
	Mat srcImage;
	srcImage = imread(pic_name);
	//imshow("original image", srcImage);

	//对图像进行空间的转换,进行灰度化
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	//对灰度图进行滤波
	GaussianBlur(grayImage, grayImage, Size(3, 3), 0, 0);
	//	imshow("滤波图像", grayImage);

	//为了得到二值图像，对灰度图进行边缘检测
	Mat cannyImage;
	Canny(grayImage, cannyImage, 128, 255, 3);
	//在得到的二值图像中寻找轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(cannyImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	Mat drawing = Mat::zeros(cannyImage.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	//绘制轮廓
	for (int i = 0; i < (int)contours.size(); i++)
	{
		//drawContours(cannyImage, contours, i, Scalar(255), 1, 8);
	}

	//计算轮廓的矩
	vector<Moments> g_vMoments(contours.size());
	for (int i = 0; i < (int)contours.size(); i++)
	{
		g_vMoments[i] = moments(contours[i], false);
	}

	//利用计算得到的矩计算中心矩
	features *run = new features(contours.size(), srcImage.cols * srcImage.rows, 1);
	for (int i = 0; i < (int)contours.size(); i++)
	{
		run->centerMoments[i] = Point2f(float(g_vMoments[i].m10 / g_vMoments[i].m00), float(g_vMoments[i].m01 / g_vMoments[i].m00));
	}
	//将得到的中心矩显示出来
	for (int i = 0; i < (int)contours.size(); i++)
	{
		//参数中的 -1 表示填充
		circle(cannyImage, (Point)run->centerMoments[i], 5, Scalar(255), -1, 8);
		//cout << i << " " << centerMoments[i].x << " " << centerMoments[i].y << endl;
	}

	run->processData();
	run->outToFile(out_txtFile);
	delete run;
	//imshow("deal image", cannyImage);
}

void load_proc(string in_filename, string out_filename)
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

		//提取HSV的9维特征
		extract_picture_byHSV(pic_name, out_txtFile);

		//提取轮廓中心距的6维特征
		extract_picture_byContours(pic_name, out_txtFile);
	}
	out_txtFile << "<< end >>";
	out_txtFile.close();

	in_txtFile.close();

}