// test.cpp : 定义控制台应用程序的入口点。
//
#pragma warning(disable : 4996 4244 4267)

#include "stdafx.h"

#include "../CrossFence/ZhcvMath.h"
#include "../CrossFence/ZhSmartAlarm.h"

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

ZhSmartAlarm alarm;

int readtxtfile(std::string szfile, int ncol, int nrow, std::vector<float> &datas)
{
	std::ifstream infile;
	infile.open(szfile);

	std::string line;

	while (getline(infile, line))
	{
		float f;
		char *src = (char*)line.c_str();
		char *tokenPtr = strtok(src, "\t");

    	while (tokenPtr != NULL) 
		{
			f = atof(tokenPtr);
			datas.push_back(f);
			//std::cout << tokenPtr << std::endl;
			tokenPtr = strtok(NULL, "\t");
		}
	}

	infile.close();
	return 0;
}

int v1() 
{
	char s[] = "ab-cd : ef;gh :i-jkl;mnop;qrs-tu: vwx-y;z";
	char *delim = "-: ;";
	char *p;
	int i = 0;
	printf("%d :- %s \n", i++, strtok(s, delim));
	while ((p = strtok(NULL, delim)))
		printf("%d :- %s \n", i++, p);

	//printf("end!! \n");

	//getchar();
	return 0;
}

int mainA()
{
	CFG_CROSSLINE_INFO cross;
	CFG_CROSSFENCEDETECTION_INFO crossfenc;
	CFG_DENSITYDETECTION_INFO density;

	//设周界  16*9的矩形框 归一化
	Point2D pd[7] = {
		{ 1.0 / 16, 1.0 / 9 },
		{ 5.0 / 16, 1.0 / 9 },
		{ 8.0 / 16, 2.0 / 9 }, 
		{ 8.0 / 16, 7.0 / 9 }, 
		{ 6.0 / 16, 3.0 / 9 },
		{ 2.0 / 16, 6.0 / 9 },
		{ 0.0 / 16, 4.0 / 9 } };

	//线段
	Point2D p1 = { 0.5 / 16, 5.0 / 9 };
	Point2D p2 = { 9.0 / 16, 3.0 / 9 };
	//两个测试点
	Point2D q1 = { 2.0 / 16, 2.0 / 9 };
	Point2D q2 = { 5.0 / 16, 5.0 / 9 };

	cross.nTimeSection = 0;
	cross.nDetectLinePoint = 2;
	cross.stuDetectLine[0] = pd[4];
	cross.stuDetectLine[1] = pd[5];

	cross.stuDetectLine[2] = pd[0];
	cross.nDirection = -1;
	cross.nPlace = -1;
	crossfenc.nDetectRegionPoint = 7;
	density.nDetectRegionPoint = 7;
	//密度报警值
	density.nThresholdNum = 80;

	std::vector<Point2D> vp;

	for (int ii = 0; ii < 7; ii++)
	{
		//if (ii < 2)
		//	cross.stuDetectLine[ii] = pd[ii];
		crossfenc.stuDetectRegion[ii] = pd[ii];
		density.stuDetectRegion[ii] = pd[ii];

		vp.push_back(pd[ii]);
	}

	//设置参数规则
	int ruleType = 1;
	alarm.SetupPara(ruleType++, &cross);
	alarm.SetupPara(ruleType++, &crossfenc);
	alarm.SetupPara(ruleType, &density);

	ruleType = 1;

	DETECTALARM_INFO alermInfo;
	alermInfo.blBoundOrVertex = false;
	alermInfo.lastAlert = alertDetailInfo( 0, (float)q1.x, (float)q1.y, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f);
	alermInfo.currentAlert = alertDetailInfo(0, (float)q2.x, (float)q2.y, 0.0f, 0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f, (float)0.0f);
	//test 1 检测 位置 //判断点的位置在左边
	alermInfo.nPlace = 0;
	//test crossline tripwire 绊线
	int res = alarm.DetectCheck(1, alermInfo);
	printf("alerm.DetectCheck cross place %d \n", res);

	//test 2  crossfence 周界 里外
	res = alarm.DetectCheck(2, alermInfo);
	printf("alerm.DetectCheck fence place %d \n", res);

	//检测绊线
	alermInfo.nPlace = -1;
	alermInfo.nDirect = 0;
	//test 3 crossline tripwire 绊线 从左到右
	res = alarm.DetectCheck(1, alermInfo);
	printf("alerm.DetectCheck cross direct %d \n", res);

	//test 4 crossfence 周界 穿越
	res = alarm.DetectCheck(2, alermInfo);
	printf("alerm.DetectCheck fence direct %d \n", res);

	//test 5 desity 密度报警
	DENSITYALARM_INFO alarmDesityInfo;
	//人群密度sdk输出的结果
	alarmDesityInfo.timestamp = 0;
	alarmDesityInfo.nWidth = 128;
	alarmDesityInfo.nHeight = 72;
	alarmDesityInfo.fTotal = 9.37714;
	readtxtfile("./data1.txt", 128, 72, alarmDesityInfo.vStatistical);

	res = alarm.DensityCheck(alarmDesityInfo);
	printf("alerm.DesityCheck %d \n", res);

	//test 6 statistical objects 区域
	res = alarm.DensityStatistical(alarmDesityInfo, true, vp);
	printf("alerm.DesityStatistical total number %d \n", res);

	//test 7 time
	time_t tmnow;
	tmnow = time(NULL);
	struct tm *tm1 = localtime(&tmnow);
	tm1->tm_mon = 1;
	tm1->tm_mday = 9;
	tm1->tm_hour = 10;
	tm1->tm_min = 10;
	//输入时间 2020/ 02 / 10  10:10
	cross.nTimeSection = 1;
	//时间规则三个 
	//2020/ 02 / 10  7:00-09:30
	cross.guardTimes[0].stuDayTimeSection[1][9][0].nBeginHour = 7;
	cross.guardTimes[0].stuDayTimeSection[1][9][0].nEndHour = 9;
	cross.guardTimes[0].stuDayTimeSection[1][9][0].nBeginMin = 0;
	cross.guardTimes[0].stuDayTimeSection[1][9][0].nEndMin = 30;
	cross.guardTimes[0].stuDayTimeSection[1][9][0].nBeginSec = 0;
	cross.guardTimes[0].stuDayTimeSection[1][9][0].nEndSec = 0;

	//2020/ 02 / 10  10:00-11:00
	cross.guardTimes[0].stuDayTimeSection[1][9][1].nBeginHour = 10;
	cross.guardTimes[0].stuDayTimeSection[1][9][1].nEndHour = 11;
	cross.guardTimes[0].stuDayTimeSection[1][9][1].nBeginMin = 0;
	cross.guardTimes[0].stuDayTimeSection[1][9][1].nEndMin = 0;
	cross.guardTimes[0].stuDayTimeSection[1][9][1].nBeginSec = 0;
	cross.guardTimes[0].stuDayTimeSection[1][9][1].nEndSec = 0;

	//2020/ 02 / 9  7:00-09:30
	cross.guardTimes[0].stuDayTimeSection[1][8][0].nBeginHour = 7;
	cross.guardTimes[0].stuDayTimeSection[1][8][0].nEndHour = 9;
	cross.guardTimes[0].stuDayTimeSection[1][8][0].nBeginMin = 0;
	cross.guardTimes[0].stuDayTimeSection[1][8][0].nEndMin = 30;
	cross.guardTimes[0].stuDayTimeSection[1][8][0].nBeginSec = 0;
	cross.guardTimes[0].stuDayTimeSection[1][8][0].nEndSec = 0;
	//重新设置规则 
	//判断点的位置在右边
	alermInfo.nPlace = 1;
	alermInfo.nDirect = -1;
	alarm.SetupPara(ruleType++, &cross);

	time_t mkt = mktime(tm1);
	alermInfo.currentAlert.timestamp = mkt;
	res = alarm.DetectCheck(1, alermInfo);

	printf("alerm.DetectCheck time test cross place %d \n", res);
	printf("wait key press to exit \n");
	getchar();
    return 0;
}

Point2D dista(double x1, double y1, double x2, double y2, double distance, int direction)
{
	//distance = 0.006
	Point2D p2d;
	//double x1, y1, x2, y2, distance;
	//int direction;

	//x1 = 0;
	//y1 = 0;

	//x2 = 3;
	//y2 = 4;
	//distance = 5;
	//direction = 0;
	
	//勾股 直接取垂线
	double cosAlpha = (x2 - x1) / sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	double sinAlpha = -(y2 - y1) / sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	double dx1 = distance * sinAlpha + (x1 + x2) / 2;
	double dy1 = distance * cosAlpha + (y1 + y2) / 2;;
	double dx2 = -distance * sinAlpha + (x1 + x2) / 2;
	double dy2 = -distance * cosAlpha + (y1 + y2) / 2;;
	////垂线
	//double kp = (x1 - x2) / (y1 - y2);
	////勾股
	//double a = 1.0 + kp * kp;
	//double b = -(y1 + y2) * kp - (x1 + x2);
	//double c = (y1 + y2) * (y1 + y2) / 4.0 + (x1 + x2) * (x1 + x2) / 4.0 - distance * distance;
	////求根
	//double dx1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	//double dx2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	//double dy1 = kp * dx1;
	//double dy2 = kp * dx2;

	//(p2.x - p1.x) * (p0.y - p1.y) - (p2.y - p1.y) * (p0.x - p1.x);
	double tmpx = (x2 - x1) * (dy1 - y1) - (y2 - y1) * (dx1 - x1);
	//double tmpx2 = (x2 - x1) * (dy2 - y1) - (y2 - y1) * (dx2 - x1);
	//左边
	if (direction == 0)
	{
		if (tmpx > 0.0)
		{
			p2d.x = dx1;
			p2d.y = dy1;
		}
		else
		{
			p2d.x = dx2;
			p2d.y = dy2;
		}
	}
	else
	{
		//右边
		if (tmpx > 0.0)
		{
			p2d.x = dx2;
			p2d.y = dy2;
		}
		else
		{
			p2d.x = dx1;
			p2d.y = dy1;
		}
	}

	return p2d;
}

void test1()
{
	/*
			 [0.460677083, 0.861111111],
			      [0.511979167, 0.842592593],
			      [0.529947917, 0.964506173],
			      [0.476302083, 0.963734568],
			      [0.458854167, 0.901234568]
	*/
	//dista(0, 0, 3, 4, 5, 0);
	Point2D p2d[5];
	p2d[0] = dista(0.460677083, 0.861111111, 0.511979167, 0.842592593, 0.04, 1);
	p2d[1] = dista(0.511979167, 0.842592593, 0.529947917, 0.964506173, 0.02, 1);
	p2d[2] = dista(0.529947917, 0.964506173, 0.476302083, 0.963734568, 0.03, 1);
	p2d[3] = dista(0.476302083, 0.963734568, 0.458854167, 0.901234568, 0.05, 1);
	p2d[4] = dista(0.458854167, 0.901234568, 0.460677083, 0.861111111, 0.06, 1);

	getchar();
}

void testspeedtable()
{
	//bool ZhcvMath::CalculateRealDistance(std::map<double, double> mapTableX, std::map<double, double> mapTableY, Point2D p1, Point2D p2, double &distance)
	std::map<double, double> mapTableX;

	mapTableX[0.10] = 0;
	mapTableX[0.30] = 40;
	mapTableX[0.40] = 70;
	mapTableX[0.50] = 90;
	mapTableX[0.56] = 100;
	mapTableX[0.60] = 108;

	std::map<double, double> mapTableY;
	mapTableY[0.2] = 0;
	mapTableY[0.25] = 20;
	mapTableY[0.35] = 30;
	mapTableY[0.40] = 38;
	mapTableY[0.50] = 42;
	mapTableY[0.60] = 47;

	Point2D p1(0.2, 0.25);
	Point2D p2(0.4, 0.38);
	Point2D p3(0.58, 0.58);

	double distance1;
	double distance2;
	bool bl = ZhcvMath::CalculateRealDistance(mapTableX, mapTableY, p1, p2, distance1);
	bl = ZhcvMath::CalculateRealDistance(mapTableX, mapTableY, p1, p3, distance2);

	double distance = distance1 + distance2 / 2.0;

	getchar();
}

int main()
{
	testspeedtable();
	//test1();
	// return dista();
	return 0;
}
