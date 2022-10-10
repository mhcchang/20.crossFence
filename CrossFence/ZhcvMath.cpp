#include "ZhcvMath.h"

#define CLIP_CODE_C 0x0000    // 裁剪区
#define CLIP_CODE_N 0x0008    // 北
#define CLIP_CODE_S 0x0004    // 南
#define CLIP_CODE_E 0x0002    // 东
#define CLIP_CODE_W 0x0001    // 西

#define CLIP_CODE_NW 0x0009    // 西北
#define CLIP_CODE_SW 0x0005    // 西南
#define CLIP_CODE_SE 0x0006    // 东南
#define CLIP_CODE_NE 0x000A    // 东北


#if __WINDOWS_OS__
#pragma warning(disable : 4996 4244 4267)
#else
#include <strings.h>
#endif

static double EARTH_RADIUS = 6378138.0;

ZhcvMath::ZhcvMath()
{
}

ZhcvMath::~ZhcvMath()
{
}

/**
* 判断是否在经纬度范围内
* @Title: isInRange
* @param point
* @param left
* @param right
* @return
*  boolean
*/
bool ZhcvMath::IsInRange(double point, double left, double right)
{
	if (point >= fmin(left, right) && point <= fmax(left, right))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
* 判断是否在矩形范围内
* @param lat		测试点纬度
*		 lng		测试点经度
*		 minLat	纬度范围限制1
*		 maxLat	纬度范围限制2
*		 minLng	经度范围限制1
*		 maxLng	经度范围限制2
* @return
*/
bool ZhcvMath::IsInRectangleArea(double lat, double lng, double minLat, double maxLat, double minLng, double maxLng)
{
	if (IsInRange(lat, minLat, maxLat))
	{
		//如果在纬度范围内
		if (minLng*maxLng > 0)
		{
			if (IsInRange(lng, minLng, maxLng))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			//
			if (abs(minLng) + abs(maxLng) < 180.0)
			{
				if (IsInRange(lng, minLng, maxLng))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				double left = fmax(minLng, maxLng);
				double right = fmin(minLng, maxLng);
				if (IsInRange(lng, left, 180.0) || IsInRange(lng, right, -180.0))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	else {
		return false;
	}
}

double ZhcvMath::rad(double d)
{
	return d * Math_PI / 180.0;
}

/**
* 计算是否在圆上(单位/千米) 地图
* @param radius	半径
*		 lat1		维度
*		 lng1		经度
* @return  -1 在圆外面 0在圆弧上 1 在圆内部
*/
int ZhcvMath::IsInCircle(double radius, double lat1, double lng1, double lat2, double lng2)
{
	double radLat1 = rad(lat1);
	double radLat2 = rad(lat2);
	double a = radLat1 - radLat2;
	double b = rad(lng1) - rad(lng2);
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radLat1)* cos(radLat2)* pow(sin(b / 2), 2)));
	s = s * EARTH_RADIUS;
	s = round(s * 10000) / 10000;
	if (s > radius)
		//不在圆上
		return -1;
	if (s == radius)
		return 0;
	else
		return 1;
}

/**
* 判断点是否在直线的一侧 叉积法
* @param p 点
*		 p1 线起点
*		 p2 线终点
* @return 当tmpx>p.x的时候，说明点在线的左边1，小于在右边-1，等于则在线上0。
*/
int ZhcvMath::LeftOfLine_Vector(const Point2D p, const Point2D p1, const Point2D p2)
{
	//(y1 – y2) * x + (x2 – x1) * y + x1 * y2 – x2 * y1
	double tmpx = (p1.y - p2.y) * p.x + (p2.x - p1.x) * p.y + p1.x * p2.y - p2.x * p1.y;
	//(p1.x - p2.x) / (p1.y - p2.y) * (p.y - p2.y) + p2.x;

	if (tmpx > p.x)
		return 1;
	else if (tmpx == p.x)
		return 0;
	else
		return -1;
}

int ZhcvMath::LeftOfLine_Area(Point2D p0, Point2D p1, Point2D p2)
{
	//(x2 - x1)(y0 - y1) –(y2 - y1)(x0 - x1)  >  0   左侧
	//(x2 - x1)(y0 - y1) –(y2 - y1)(x0 - x1)  <  0   右侧
	//(x2 - x1)(y0 - y1) –(y2 - y1)(x0 - x1) = 0   线段上
	double tmpx = (p2.x - p1.x) * (p0.y - p1.y) - (p2.y - p1.y) * (p0.x - p1.x);
	if (tmpx > 0.0)
		return 1;
	else if (tmpx == 0.0)
		return 0;
	else
		return -1;
}

/**
* 判断点是否在多边形内
* @param polygon	多边形
*		 point		检测点
* @return			在边或者顶点上2 点在多边形内返回0，外面返回1
*/
int ZhcvMath::IsPtInPoly(std::vector<Point2D> polygon, Point2D point)
{
	int N = polygon.size();
	int boundOrVertex = 2; //如果点位于多边形的顶点或边上，也算做点在多边形内，直接返回2
	int intersectCount = 0; //cross points count of x--交叉点计数X
	double precision = EPSILON; //浮点类型计算时候与0比较时候的容差
	Point2D p1, p2; //neighbour bound vertices--临近绑定顶点
	Point2D p = point; //当前点

	p1 = polygon[0]; //left vertex--左顶点
	for (int i = 1; i <= N; ++i)
	{
		//check all rays--检查所有射线
		if (p.x == p1.x && p.y == p1.y)
			return boundOrVertex; //p is an vertex--p是一个顶点

		p2 = polygon.at(i % N); //right vertex--右顶点
		if (p.x < fmin(p1.x, p2.x) || p.x > fmax(p1.x, p2.x))
		{
			//ray is outside of our interests--射线不在我们的兴趣范围之内
			p1 = p2;
			continue;
			//next ray left point--下一条射线的左边点
		}

		if (p.x > fmin(p1.x, p2.x) && p.x < fmax(p1.x, p2.x))
		{
			//ray is crossing over by the algorithm(common part of)--射线交叉
			if (p.y <= fmax(p1.y, p2.y))
			{
				//x is before of ray--x在射线之前
				if (p1.x == p2.x && p.y >= fmin(p1.y, p2.y))
				{
					//overlies on a horizontal ray--在一条水平射线上
					return boundOrVertex;
				}

				if (p1.y == p2.y)
				{
					//ray is vertical--射线是垂直的
					if (p1.y == p.y)
					{
						//overlies on a vertical ray--覆盖在垂直光线上
						return boundOrVertex;
					}
					else
					{
						//before ray--射线之前
						++intersectCount;
					}
				}
				else
				{
					//cross point on the left side--左边的交叉点
					double xinters = (p.x - p1.x) * (p2.y - p1.y) / (p2.x - p1.x) + p1.y;//cross point of y--y的交叉点
					if (abs(p.y - xinters) < precision)
					{
						//overlies on a ray--覆盖在射线
						return boundOrVertex;
					}

					if (p.y < xinters)
					{
						//before ray--射线之前
						++intersectCount;
					}
				}
			}
		}
		else
		{
			//special case when ray is crossing through the vertex--特殊情况下，当射线穿过顶点
			if (p.x == p2.x && p.y <= p2.y)
			{
				//p crossing over p2--p交叉p2
				Point2D p3 = polygon.at((i + 1) % N); //next vertex--下一个顶点
				if (p.x >= fmin(p1.x, p3.x) && p.x <= fmax(p1.x, p3.x))
				{
					//p.x lies between p1.x & p3.x--p.x在p1.x和p3.x之间
					++intersectCount;
				}
				else
				{
					intersectCount += 2;
				}
			}
		}
		p1 = p2; //next ray left point--下一条射线的左边点
	}

	if (intersectCount % 2 == 0)
	{ //偶数在多边形外
		return 1;
	}
	else
	{ //奇数在多边形内
		return 0;
	}
}

bool ZhcvMath::IsStraightLine(const std::vector<Point2D> &polygon)
{
	if (polygon.size() == 0)
	{
		return false;
	}
	unsigned int i = 0;
	bool         bStraightLine;
	float        fSlope;
	double       dDistance;
	double       dCoefficient;
	double       dOffset;
	unsigned int nPointTotal;
	std::vector<Point2D> pPointArray;
	int          nXOffset;
	int          nYOffset;

	pPointArray = polygon;
	nPointTotal = polygon.size();

	nXOffset = (int)(pPointArray[0].x - pPointArray[1].x);
	nYOffset = (int)(pPointArray[0].y - pPointArray[1].y);
	bStraightLine = true;

	//Check if all points construct a vertical line.
	if (0 == nXOffset)
	{
		for (i = 2; i < nPointTotal; i++)
		{
			if (pPointArray[0].x != pPointArray[i].x)
			{
				bStraightLine = false;
				break;
			}
		}
	}

	// Check if all points construct a horizontal line.
	if (0 == nYOffset)
	{
		for (i = 2; i < nPointTotal; i++)
		{
			if (pPointArray[0].y != pPointArray[i].y)
			{
				bStraightLine = false;
				break;
			}
		}
	}

	// Check if all points construct a diagonal.
	if ((0 != nXOffset) && (0 != nYOffset))
	{
		// calculate slope
		fSlope = 1.0f * nYOffset / nXOffset;
		// intercept 
		dOffset = pPointArray[0].y - fSlope * pPointArray[1].x;
		// the coefficient for calculating the distance between point and line. 
		dCoefficient = sqrt(1.0 + fSlope * fSlope);

		for (i = 2; i < nPointTotal; i++)
		{
			// Calculate the distance between other point and the line constructed by point 1 and point 2. 
			dDistance = fabs(fSlope * pPointArray[i].x - pPointArray[i].y + dOffset) / dCoefficient;

			// if there is one distance of points that is more than 0, the polygon is valid.在线上
			if ((dDistance - 0.0) > EPSILON)
			{
				bStraightLine = false;
				break;
			}
		}
	}

	return bStraightLine;
}

bool ZhcvMath::IsValidRegion(const std::vector<Point2D> &polygon)
{
	if (polygon.size() == 0)
		return false;

	// 判断组成多边形的所有点是否在一条直线上 共线情况判断
	if (IsStraightLine(polygon))
	{
		return false;
	}
	// 判断组成多边形的终点和起点的连线与已有的直线是否有交叉
	//if (IsCrossLine(polygon))
	//{
	//	return false;
	//}
	return true;
}

bool ZhcvMath::LineCrossDetect(Point2D p1, Point2D p2, Point2D q1, Point2D q2)
{
	double z1, z2, z3, z4;
	//叉积
	// 1 (p1 - q1) × (q2 - q1)；
	z1 = (p1.x - q1.x) * (q2.y - q1.y) - (q2.x - q1.x) * (q2.y - q1.y);
	// 2 (q2 - q1) × (p2 - q1)；
	z2 = (q2.x - q1.x) * (p2.y - q1.y) - (p2.x - q1.x) * (q2.y - q1.y);
	// 3 (q1 - p1) × (p2 - p1)；
	z3 = (q1.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (q1.y - p1.y);
	//4 (p2 - p1) × (q2 - p1);
	z4 = (p2.x - p1.x) * (q2.y - p1.y) - (q2.x - p1.x) * (p2.y - p1.y);
	//  check the span of two segment
	if (((z1 >= 0.0f && z2 >= 0.0f) || (z1 < 0.0f && z2 < 0.0f)) &&
		((z3 >= 0.0f && z4 >= 0.0f) || (z3 < 0.0f && z4 < 0.0f)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ZhcvMath::IsCrossLine(const std::vector<Point2D> &polygon)
{
	if (polygon.size() == 0)
		return false;

	bool bCrossLine = false;

	for (int i = 0; i < polygon.size() - 1; i++)
	{
		if (LineCrossDetect(polygon[0], polygon[polygon.size() - 1], polygon[i], polygon[i + 1]))
		{
			bCrossLine = true;
		}
	}

	return bCrossLine;
}

int ZhcvMath::CSClip_Line(double p1x, double p1y, double q1x, double q1y, bool blClip, double &x1, double &y1, double &x2, double &y2)
{
	//float q1x, float q1y, float p1y, float p1x, 
	double xc1 = x1;
	double yc1 = y1;
	double xc2 = x2;
	double yc2 = y2;

	int p1_code = 0,    // 第一个点的方位码
		p2_code = 0;    // 第二个点的方位码

	if (y1 < p1y)
		p1_code |= CLIP_CODE_N;
	else
		if (y1 > q1y)
			p1_code |= CLIP_CODE_S;

	if (x1 < p1x)
		p1_code |= CLIP_CODE_W;
	else
		if (x1 > q1x)
			p1_code |= CLIP_CODE_E;

	if (y2 < p1y)
		p2_code |= CLIP_CODE_N;
	else
		if (y2 > q1y)
			p2_code |= CLIP_CODE_S;

	if (x2 < p1x)
		p2_code |= CLIP_CODE_W;
	else
		if (x2 > q1x)
			p2_code |= CLIP_CODE_E;

	//////////////////////排除两种已确定不需要裁剪的情况////////////////////
	// 两个点在外同侧
	if ((p1_code & p2_code))
		return 0;

	// 两点都在矩形内
	if (p1_code == 0 && p2_code == 0)
		return 0;

	//有交点  不需要裁剪
	if (!blClip)
		return 1;
	//////////////可能需要裁剪,假设需要裁剪,最后得出结果再判断//////////////
	//////////////相交只有水平相交和垂直相交两种情况////////////////////////
	switch (p1_code)
	{
	case CLIP_CODE_C:
		break;
	case CLIP_CODE_N:
	{
		// 水平相交
		yc1 = p1y;
		xc1 = x1 + (p1y - y1) * (x2 - x1) / (y2 - y1);
		break;
	}
	case CLIP_CODE_S:
	{
		// 水平相交
		yc1 = q1y;
		xc1 = x1 + (q1y - y1) * (x2 - x1) / (y2 - y1);
		break;
	}
	case CLIP_CODE_W:
	{
		// 垂直相交
		xc1 = p1x;
		yc1 = y1 + (p1x - x1) * (y2 - y1) / (x2 - x1);
		break;
	}
	case CLIP_CODE_E:
	{
		// 垂直相交
		xc1 = q1x;
		yc1 = y1 + (q1x - x1) * (y2 - y1) / (x2 - x1);
		break;
	}
	case CLIP_CODE_NE:
	{
		// 先假设水平相交
		yc1 = p1y;
		xc1 = x1 + (p1y - y1) * (x2 - x1) / (y2 - y1);

		// 判断是否水平相交
		if (xc1 < p1x || xc1 > q1x)
		{
			// 垂直相交
			xc1 = q1x;
			yc1 = y1 + (q1x - x1) * (y2 - y1) / (x2 - x1);
		}
		break;
	}
	case CLIP_CODE_SE:
	{
		// 先假设水平相交
		yc1 = q1y;
		xc1 = x1 + 0.5 + (q1y - y1) * (x2 - x1) / (y2 - y1);

		// 判断是否水平相交
		if (xc1 < p1x || xc1 > q1x)
		{
			// 垂直相交
			xc1 = q1x;
			yc1 = y1 + (q1x - x1) * (y2 - y1) / (x2 - x1);
		}
		break;
	}
	case CLIP_CODE_NW:
	{
		// 先假设水平相交
		yc1 = p1y;
		xc1 = x1 + (p1y - y1) * (x2 - x1) / (y2 - y1);

		// 判断是否水平相交
		if (xc1 < p1x || xc1 > q1x)
		{
			// 垂直相交
			xc1 = p1x;
			yc1 = y1 + (p1x - x1) * (y2 - y1) / (x2 - x1);
		}
		break;
	}
	case CLIP_CODE_SW:
	{
		// 先假设水平相交
		yc1 = q1y;
		xc1 = x1 + (q1y - y1) * (x2 - x1) / (y2 - y1);

		// 判断是否水平相交
		if (xc1 < p1x || xc1 > q1x)
		{
			// 垂直相交
			xc1 = p1x;
			yc1 = y1 + (p1x - x1) * (y2 - y1) / (x2 - x1);
		}
		break;
	}
	default:
		break;
	} // end switch

	switch (p2_code)
	{
	case CLIP_CODE_C:
		break;
	case CLIP_CODE_N:
	{
		// 水平相交
		yc2 = p1y;
		xc2 = x2 + (p1y - y2) * (x1 - x2) / (y1 - y2);
		break;
	}
	case CLIP_CODE_S:
	{
		// 水平相交
		yc2 = q1y;
		xc2 = x2 + (q1y - y2) * (x1 - x2) / (y1 - y2);
		break;
	}
	case CLIP_CODE_W:
	{
		// 垂直相交
		xc2 = p1x;
		yc2 = y2 + (p1x - x2) * (y1 - y2) / (x1 - x2);
		break;
	}
	case CLIP_CODE_E:
	{
		// 垂直相交
		xc2 = q1x;
		yc2 = y2 + (q1x - x2) * (y1 - y2) / (x1 - x2);
		break;
	}
	case CLIP_CODE_NE:
	{
		// 假设水平相交
		yc2 = p1y;
		xc2 = x2 + (p1y - y2) * (x1 - x2) / (y1 - y2);

		// 判断是否水平相交
		if (xc2 < p1x || xc2 > q1x)
		{
			// 垂直相交
			xc2 = q1x;
			yc2 = y2 + (q1x - x2) * (y1 - y2) / (x1 - x2);
		}
		break;
	}
	case CLIP_CODE_SE:
	{
		// 假设水平相交
		yc2 = q1y;
		xc2 = x2 + (q1y - y2)  * (x1 - x2) / (y1 - y2);

		// 判断是否水平相交
		if (xc2 < p1x || xc2 > q1x)
		{
			// 垂直相交
			xc2 = q1x;
			yc2 = y2 + (q1x - x2)  * (y1 - y2) / (x1 - x2);
		}
		break;
	}
	case CLIP_CODE_NW:
	{
		// 假设水平相交
		yc2 = p1y;
		xc2 = x2 + (p1y - y2) * (x1 - x2) / (y1 - y2);

		// 判断是否水平相交
		if (xc2 < p1x || xc2 > q1x)
		{
			// 垂直相交
			xc2 = p1x;
			yc2 = y2 + (p1x - x2) * (y1 - y2) / (x1 - x2);
		}
		break;
	}
	case CLIP_CODE_SW:
	{
		// 假设水平相交
		yc2 = q1y;
		xc2 = x2 + 0.5 + (q1y - y2) * (x1 - x2) / (y1 - y2);

		// 判断是否水平相交
		if (xc2 < p1x || xc2 > q1x)
		{
			// 垂直相交
			xc2 = p1x;
			yc2 = y2 + (p1x - x2) * (y1 - y2) / (x1 - x2);
		}
		break;
	}
	default:
		break;
	} // end switch

	//判断是否裁剪成功
	//if ((xc1 < p1x) || (xc1 > q1x) ||
	//	(yc1 < p1y) || (yc1 > q1y) ||
	//	(xc2 < p1x) || (xc2 > q1y) ||
	//	(yc2 < p1y) || (yc2 > q1y))
	//{
	//	return 0;
	//}
	//

	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return 1;
} // end CS_Clip_Line

double ZhcvMath::calculateDistance(double x1, double y1, double x2, double y2, double x0, double y0, bool &blCross)
{
	double d = (fabs((y2 - y1) * x0 + (x1 - x2) * y0 + ((x2 * y1) - (x1 * y2)))) / (sqrt(pow(y2 - y1, 2) + pow(x1 - x2, 2)));
	//k = (pt2.y - pt1.y) / (pt2.x - pt1.x);
	double cx = 0.0;
	double cy = 0.0;
	GetFootOfPerpendicular(x1, y1, x2, y2, x0, y0, cx, cy);

	if (((x1 > cx && cx > x2) || (x1 < cx && cx < x2)) && ((y1 > cy && cy > y2) || (y1 < cy && cy < y2)))
		blCross = true;
	else
	{
		blCross = false;
	}

	return d;
}

void ZhcvMath::GetFootOfPerpendicular(double x1, double y1, double x2, double y2, double x0, double y0, double &cx, double &cy)
//const Point &pt,     // 直线外一点 x0
//const Point &begin,  // 直线开始点 x1
//const Point &end)   // 直线结束点 x2
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	if (fabs(dx) < 0.00000001 && fabs(dy) < 0.00000001)
	{
		cx = x1;
		cy = y1;
		return;
	}

	double u = (x0 - x1) * (x1 - x2) + (y0 - y1) * (y1 - y2);
	u = u / ((dx * dx) + (dy * dy));

	cx = x1 + u * dx;
	cy = y1 + u * dy;
}

Point2D ZhcvMath::GetPerpendicularPointWithDistance(double x1, double y1, double x2, double y2, double distance, int direction)
{
	//勾股求斜边长 直接取垂线 sin/cos反一下
	double cos = (x2 - x1) / sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	double sin = -(y2 - y1) / sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	//两个点
	double dx1 = distance * sin + (x1 + x2) / 2;
	double dy1 = distance * cos + (y1 + y2) / 2;;
	double dx2 = -distance * sin + (x1 + x2) / 2;
	double dy2 = -distance * cos + (y1 + y2) / 2;;
	
	double tmpx = (x2 - x1) * (dy1 - y1) - (y2 - y1) * (dx1 - x1);
	Point2D p2d;
	//左边
	if (direction == 0)
	{
		if (tmpx < dx1)
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
		if (tmpx > dx1)
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

//* 获取已知点到线段的垂足距离
//* @param x 已知点X坐标
//* @param y 已知点Y坐标
//* @param startx 线段开始点X坐标
//* @param starty 线段开始点Y坐标
//* @param endx 线段结束点X坐标
//* @param endy 线段结束点Y坐标
//* @param outx
//* @param outy
//* @return
//*/
//double getVerticalDistance(double x, double y, double startx, double starty, double endx, double endy, double &outx, double &outy)
//{
//	//PS: 点到线段的最短距离则需要比较垂足是否在线段上or线段的延长线上
//
//	double minx = startx > endx ? endx : startx;
//	double maxx = startx > endx ? startx : endx;
//	double miny = starty > endy ? endy : starty;
//	double maxy = starty > endy ? starty : endy;
//	if (outx<maxx && outx>minx && outy<maxy && outy>miny)
//	{
//		return (x - outx)*(x - outx) + (y - outy)*(y - outy);
//	}
//	else
//	{
//		double ds = (startx - x)*(startx - x) + (starty - y)*(starty - y);
//		double de = (endx - x)*(endx - x) + (endy - y)*(endy - y);
//		if (ds < de)
//		{
//			outx = startx;
//			outy = starty;
//			return ds;
//		}
//		else
//		{
//			outx = endx;
//			outy = endy;
//			return de;
//		}
//	}
//}

Point2D ZhcvMath::GetCenterOfGravityPoint(std::vector<Point2D> mPoints)
{
	double area = 0.0;//多边形面积
	double Gx = 0.0, Gy = 0.0;// 重心的x、y
	for (int i = 1; i <= mPoints.size(); i++) {
		double iLat = mPoints[i % mPoints.size()].x;
		double iLng = mPoints[i % mPoints.size()].y;
		double nextLat = mPoints[i - 1].x;
		double nextLng = mPoints[i - 1].y;
		double temp = (iLat * nextLng - iLng * nextLat) / 2.0;
		area += temp;
		Gx += temp * (iLat + nextLat) / 3.0;
		Gy += temp * (iLng + nextLng) / 3.0;
	}
	Gx = Gx / area;
	Gy = Gy / area;
	return Point2D(Gx, Gy);
}

bool ZhcvMath::CalculateRealDistance(std::map<double, double> mapTableX, std::map<double, double> mapTableY, Point2D p1, Point2D p2, double &distance)
{
	if (mapTableX.size() <= 1)
		return false;
	if (mapTableY.size() <= 1)
		return false;

	std::map<double, double>::reverse_iterator xe = mapTableX.rbegin();
	std::map<double, double>::iterator xs = mapTableX.begin();
	std::map<double, double>::reverse_iterator ye = mapTableY.rbegin();
	std::map<double, double>::iterator ys = mapTableY.begin();

	Point2D ps(xs->first, ys->first);
	Point2D pe(xe->first, ye->first);

	//只关心 在配准范围内的点 其他的返回错误
	double p1x = -EPSILON;
	double p2x = -EPSILON;
	double p1y = -EPSILON;
	double p2y = -EPSILON;

	for (std::map<double, double>::iterator itx = mapTableX.begin(); itx != mapTableX.end(); itx++)
	{
		std::map<double, double>::iterator itxn = next(itx);
		if (itxn != mapTableX.end())
		{
			//找到对应的位置  按照比例计算
			if (itx->first <= p1.x && itxn->first >= p1.x)
			{
				p1x = (p1.x - itx->first) / (itxn->first - itx->first) * (itxn->second - itx->second) + itx->second;
				break;
			}
		}
	}

	for (std::map<double, double>::iterator itx = mapTableX.begin(); itx != mapTableX.end(); itx++)
	{
		std::map<double, double>::iterator itxn = next(itx);
		if (itxn != mapTableX.end())
		{
			//找到对应的位置  按照比例计算
			if (itx->first <= p2.x && itxn->first >= p2.x)
			{
				p2x = (p2.x - itx->first) / (itxn->first - itx->first) * (itxn->second - itx->second) + itx->second;
				break;
			}
		}
	}

	for (std::map<double, double>::iterator ity = mapTableY.begin(); ity != mapTableY.end(); ity++)
	{
		std::map<double, double>::iterator ityn = next(ity);
		if (ityn != mapTableY.end())
		{
			//找到对应的位置  按照比例计算
			if (ity->first <= p1.y && ityn->first >= p1.y)
			{
				p1y = (p1.y - ity->first) / (ityn->first - ity->first) * (ityn->second - ity->second) + ity->second;
				break;
			}
		}
	}

	for (std::map<double, double>::iterator ity = mapTableY.begin(); ity != mapTableY.end(); ity++)
	{
		std::map<double, double>::iterator ityn = next(ity);
		if (ityn != mapTableY.end())
		{
			//找到对应的位置  按照比例计算
			if (ity->first <= p2.y && ityn->first >= p2.y)
			{
				p2y = (p2.y - ity->first) / (ityn->first - ity->first) * (ityn->second - ity->second) + ity->second;
				break;
			}
		}
	}

	if (p1x == -EPSILON || p2x == -EPSILON || p1y == -EPSILON || p2y == -EPSILON)
	{
		return false;
	}

	distance = sqrt( (p2x - p1x) * (p2x - p1x) + (p2y - p1y) * (p2y - p1y) );

	return true;
}
