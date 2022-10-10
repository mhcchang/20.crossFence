#pragma once

#include "ZhcvSmartDefs.h"

#if __WINDOWS_OS__
#ifdef ZHSMARTALARM_EXPORTS
#pragma warning (push)
//#pragma warning (disable: 4251)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API
#define _access access
/////gunc
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wreorder"
#endif

#include <vector>
#define Math_PI (3.14159265758)

class DLL_API ZhcvMath
{
public:
	ZhcvMath();
	~ZhcvMath();

	/**
	* 判断点是否在多边形内
	* @param polygon	多边形
	* @param point		检测点
	* @return			在边或者顶点上0 点在多边形内返回1，否则返回-1 
	*/
	static int IsPtInPoly(std::vector<Point2D> polygon, Point2D point);

	/**
	* 判断点是否在直线的一侧 叉积法
	* @param p 点
	* @param p1 线起点
	* @param p2 线终点
	* @return 当tmpx>p.x的时候，说明点在线的左边1，小于在右边-1，等于则在线上0。
	*/
	static int LeftOfLine_Vector(const Point2D p, const Point2D p1, const Point2D p2);

	/**
	* 判断点是否在直线的一侧 面积法
	* @param p 点
	* @param p1 线起点
	* @param p2 线终点
	* @return 当tmpx>p.x的时候，说明点在线的左边1，小于在右边-1，等于则在线上0。
	*/
	static int LeftOfLine_Area(Point2D p0, Point2D p1, Point2D p2);

	/**
	* 判断线段是否和矩形有交点 Cohen-Sutherland裁剪算法
	* @detail 方位编码
	*		0x0009 = 1001       0x0008 = 1000       0x000A = 1010
	*		0x0001 = 0001       0x0000 = 0000       0x0002 = 0010
	*		0x0005 = 0101       0x0004 = 0100       0x0006 = 0110
	* @param p1x p1y 矩形左上角
	* @param q1x q1y 矩形右下角
	* @param blClip 是否需要裁剪线段 不裁剪直接返回结果
	* @param [out] x1, y1 线段起点 如果与矩形边有交叉 返回交叉点
	* @param [out] x2, y2 线段起点 如果与矩形边有交叉 返回交叉点
	* @return 当tmpx>p.x的时候，有交叉点返回1，没有交叉返回0
	*/
	static int CSClip_Line(double p1x, double p1y, double q1x, double q1y, bool blClip, double &x1, double &y1, double &x2, double &y2);

	/**
	* 计算点到线段的距离 
	* @param x1, y1, x2, y2 线段
	* @param x0, y0 线段外的点
	* @param [out] blCross为true时 垂足在线段上
	* @return 返回距离
	*/
	static double calculateDistance(double x1, double y1, double x2, double y2, double x0, double y0, bool &blCross);

	/**
	* 获取多边形中心点
	* @param mPoints 多边形的端点
	* @return 中心点
	*/
	static Point2D GetCenterOfGravityPoint(std::vector<Point2D> mPoints);

	/**
	* 已知线段 求线段外的一点 到线段的距离为d 通过参数确实是左边还是右边的点
	* @param x1 y1 线段的端点 矢量!!!!
	* @param distance 距离
	* @param direction 方向 0 左边 1 右边 按照左手系计算 
	* @return 点
	*/
	static Point2D GetPerpendicularPointWithDistance(double x1, double y1, double x2, double y2, double distance, int direction);

	/**
	* 根据查表计算 实际距离 
	* @param mapTable 第一个point为归一化融合坐标 第二为长度 单位米
	* @param p1 p2 为需要计算的点
	* @return 
	*/
	static bool CalculateRealDistance(std::map<double, double> mapTableX, std::map<double, double> mapTableY, Point2D p1, Point2D p2, double &distance);

private:
	/**
	*  判断是否在经纬度矩形范围内
	* @param lat	测试点纬度
	* @param lng	测试点经度
	* @param minLat	纬度范围1
	* @param maxLat	纬度范围2
	* @param minLng	经度范围1
	* @param maxLng	经度范围2
	* @return 是 返回true 否则false
	*/
	static bool IsInRectangleArea(double lat, double lng, double minLat, double maxLat, double minLng, double maxLng);

	/**
	* 计算是否在圆上(单位/千米)
	* @param radius	半径
	* @param lat1		维度
	* @param lng1		经度
	* @return -1 在圆外面 0在圆弧上 1 在圆内部
	*/
	static int IsInCircle(double radius, double lat1, double lng1, double lat2, double lng2);
	//弧度
	static double rad(double d);
	static bool IsInRange(double point, double left, double right);
	//是否为多边形
	static bool IsValidRegion(const std::vector<Point2D> &polygon);
	//所有点都在一条线上
	static bool IsStraightLine(const std::vector<Point2D> &polygon);
	//折线之间又交叉
	static bool IsCrossLine(const std::vector<Point2D> &polygon);
	//线段交叉判断
	static bool LineCrossDetect(Point2D p1, Point2D p2, Point2D q1, Point2D q2);

	static void GetFootOfPerpendicular(double x1, double y1, double x2, double y2, double x0, double y0, double &cx, double &cy);
};
