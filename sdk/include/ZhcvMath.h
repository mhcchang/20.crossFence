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
	* �жϵ��Ƿ��ڶ������
	* @param polygon	�����
	* @param point		����
	* @return			�ڱ߻��߶�����0 ���ڶ�����ڷ���1�����򷵻�-1 
	*/
	static int IsPtInPoly(std::vector<Point2D> polygon, Point2D point);

	/**
	* �жϵ��Ƿ���ֱ�ߵ�һ�� �����
	* @param p ��
	* @param p1 �����
	* @param p2 ���յ�
	* @return ��tmpx>p.x��ʱ��˵�������ߵ����1��С�����ұ�-1��������������0��
	*/
	static int LeftOfLine_Vector(const Point2D p, const Point2D p1, const Point2D p2);

	/**
	* �жϵ��Ƿ���ֱ�ߵ�һ�� �����
	* @param p ��
	* @param p1 �����
	* @param p2 ���յ�
	* @return ��tmpx>p.x��ʱ��˵�������ߵ����1��С�����ұ�-1��������������0��
	*/
	static int LeftOfLine_Area(Point2D p0, Point2D p1, Point2D p2);

	/**
	* �ж��߶��Ƿ�;����н��� Cohen-Sutherland�ü��㷨
	* @detail ��λ����
	*		0x0009 = 1001       0x0008 = 1000       0x000A = 1010
	*		0x0001 = 0001       0x0000 = 0000       0x0002 = 0010
	*		0x0005 = 0101       0x0004 = 0100       0x0006 = 0110
	* @param p1x p1y �������Ͻ�
	* @param q1x q1y �������½�
	* @param blClip �Ƿ���Ҫ�ü��߶� ���ü�ֱ�ӷ��ؽ��
	* @param [out] x1, y1 �߶���� �������α��н��� ���ؽ����
	* @param [out] x2, y2 �߶���� �������α��н��� ���ؽ����
	* @return ��tmpx>p.x��ʱ���н���㷵��1��û�н��淵��0
	*/
	static int CSClip_Line(double p1x, double p1y, double q1x, double q1y, bool blClip, double &x1, double &y1, double &x2, double &y2);

	/**
	* ����㵽�߶εľ��� 
	* @param x1, y1, x2, y2 �߶�
	* @param x0, y0 �߶���ĵ�
	* @param [out] blCrossΪtrueʱ �������߶���
	* @return ���ؾ���
	*/
	static double calculateDistance(double x1, double y1, double x2, double y2, double x0, double y0, bool &blCross);

	/**
	* ��ȡ��������ĵ�
	* @param mPoints ����εĶ˵�
	* @return ���ĵ�
	*/
	static Point2D GetCenterOfGravityPoint(std::vector<Point2D> mPoints);

	/**
	* ��֪�߶� ���߶����һ�� ���߶εľ���Ϊd ͨ������ȷʵ����߻����ұߵĵ�
	* @param x1 y1 �߶εĶ˵� ʸ��!!!!
	* @param distance ����
	* @param direction ���� 0 ��� 1 �ұ� ��������ϵ���� 
	* @return ��
	*/
	static Point2D GetPerpendicularPointWithDistance(double x1, double y1, double x2, double y2, double distance, int direction);

	/**
	* ���ݲ����� ʵ�ʾ��� 
	* @param mapTable ��һ��pointΪ��һ���ں����� �ڶ�Ϊ���� ��λ��
	* @param p1 p2 Ϊ��Ҫ����ĵ�
	* @return 
	*/
	static bool CalculateRealDistance(std::map<double, double> mapTableX, std::map<double, double> mapTableY, Point2D p1, Point2D p2, double &distance);

private:
	/**
	*  �ж��Ƿ��ھ�γ�Ⱦ��η�Χ��
	* @param lat	���Ե�γ��
	* @param lng	���Ե㾭��
	* @param minLat	γ�ȷ�Χ1
	* @param maxLat	γ�ȷ�Χ2
	* @param minLng	���ȷ�Χ1
	* @param maxLng	���ȷ�Χ2
	* @return �� ����true ����false
	*/
	static bool IsInRectangleArea(double lat, double lng, double minLat, double maxLat, double minLng, double maxLng);

	/**
	* �����Ƿ���Բ��(��λ/ǧ��)
	* @param radius	�뾶
	* @param lat1		ά��
	* @param lng1		����
	* @return -1 ��Բ���� 0��Բ���� 1 ��Բ�ڲ�
	*/
	static int IsInCircle(double radius, double lat1, double lng1, double lat2, double lng2);
	//����
	static double rad(double d);
	static bool IsInRange(double point, double left, double right);
	//�Ƿ�Ϊ�����
	static bool IsValidRegion(const std::vector<Point2D> &polygon);
	//���е㶼��һ������
	static bool IsStraightLine(const std::vector<Point2D> &polygon);
	//����֮���ֽ���
	static bool IsCrossLine(const std::vector<Point2D> &polygon);
	//�߶ν����ж�
	static bool LineCrossDetect(Point2D p1, Point2D p2, Point2D q1, Point2D q2);

	static void GetFootOfPerpendicular(double x1, double y1, double x2, double y2, double x0, double y0, double &cx, double &cy);
};
