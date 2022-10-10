#include "ZhcvMath.h"
#include "ZhSmartAlarm.h"

#if __WINDOWS_OS__
#pragma warning(disable : 4996 4244 4267)
#else
#include <strings.h>

#define VERSION_STR v1.0.0_alpha
#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x
static char LIB_INFO[] = {
	"$" SmartAlermSdk version:"	STRINGIFY(VERSION_STR)
	"  Built: "
	__DATE__
	" "
	__TIME__
	" $"
};
#endif

#define SET_NOERROR SetErrorCode(ALARM_ERROR_CODE::ERROR_NONE)
#define SET_NOIMPL SetErrorCode(ALARM_ERROR_CODE::ERROR_NOIMPL)
#define SET_ERRPARAM SetErrorCode(ALARM_ERROR_CODE::ERROR_PARAMS)
#define SET_ERRTYPE SetErrorCode(ALARM_ERROR_CODE::ERROR_TYPE)

#include <time.h>
#include <chrono>

ZhSmartAlarm::ZhSmartAlarm()
{
	//printf("ZhSmartAlarm \n");
}

ZhSmartAlarm::~ZhSmartAlarm()
{
}

void ZhSmartAlarm::SetAlarmDetectCallback(std::function<int(int, std::vector<DETECTALARM_INFO> &)> callback)
{
	m_pDecectCall = callback;
}

void ZhSmartAlarm::SetAlarmDensityCallback(std::function<int(int, std::vector<DENSITYALARM_INFO> &)> callback)
{
	m_pDensityCall = callback;
}

int ZhSmartAlarm::GetDetectAlarmInfo(int ruleType, std::vector<DETECTALARM_INFO> &alerms)
{
	SET_NOIMPL;
	return -1;
}

int ZhSmartAlarm::GetDetectAlarmInfo(int ruleType, std::vector<DENSITYALARM_INFO> &alerms)
{
	SET_NOIMPL;
	return -1;
}

int ZhSmartAlarm::SetDetectDataSource(int ruleType, std::vector<DETECTALARM_INFO> &alerms)
{
	SET_NOIMPL;
	return -1;
}

int ZhSmartAlarm::SetDensityDataSource(std::vector<DENSITYALARM_INFO> &alerms)
{
	SET_NOIMPL;
	return -1;
}

int ZhSmartAlarm::InTimeDaySeg(std::vector<TimeDaySegment> vtm, time_t tmWhen)
{
	SET_NOERROR;
	for (int i = 0; i < vtm.size(); i++)
	{
		TimeDaySegment tm1 = vtm[i];
		struct tm * when1;
		//time(&tmWhen);
		when1 = localtime(&tmWhen);

		int is = tm1.start_tm_min + tm1.start_tm_hour * 60;
		int ib = when1->tm_min + when1->tm_hour * 60;
		int ie = tm1.end_tm_min + tm1.end_tm_hour * 60;

		//比开始时间早  比结束时间晚 
		if (is > ib || ie < ib)
		{
			//SET_ERRPARAM;
			continue;
		}

		bool blfound = false;
		for (int id : tm1.days)
		{
			if (when1->tm_mday == id)
			{
				blfound = true;
				break;
			}
		}
		if (!blfound)
		{
			continue;
		}
		blfound = false;
		for (int im : tm1.mons)
		{
			//tm里面的月从0开始
			if (when1->tm_mon == im)
			{
				blfound = true;
				break;
			}
		}
		if (!blfound)
		{
			continue;
		}
		
		return i;
	}

	return -1;
}

int ZhSmartAlarm::InTimeWeekSeg(std::vector<TimeWeekSegment> vtm, time_t tmWhen)
{
	SET_NOERROR;
	for (int i = 0; i < vtm.size(); i++)
	{
		TimeWeekSegment tm1 = vtm[i];
		struct tm * when1;
		//time(&tmWhen);
		when1 = localtime(&tmWhen);

		int is = tm1.start_tm_min + tm1.start_tm_hour * 60;
		int in = when1->tm_min + when1->tm_hour * 60;
		int ie = tm1.end_tm_min + tm1.end_tm_hour * 60;

		//比开始时间早  比结束时间晚 起始时间错误
		if (is > in && ie > in)
		{
			SET_ERRPARAM;
			return -2;
		}

		bool blfound = false;
		for (int iw : tm1.weeks)
		{
			if (when1->tm_wday == iw)
			{
				blfound = true;
				break;
			}
		}
		if (!blfound)
		{
			return -3;
		}

		blfound = false;
		for (int im : tm1.mons)
		{
			//tm里面的月从0开始
			if (when1->tm_mon == im)
			{
				blfound = true;
				break;
			}
		}
		if (!blfound)
		{
			return -4;
		}

		
		return i;
	}

	return -1;
}

bool ZhSmartAlarm::SetupPara(int ruleType, void* lpAlarmSetupParam)
{
	//根据类型判断
	switch (ruleType)
	{
	case 1:
	{
		CFG_CROSSLINE_INFO* tmp = static_cast<CFG_CROSSLINE_INFO*>(lpAlarmSetupParam);
		if (nullptr != tmp)
			memcpy(&m_struCrosslineInf, tmp, sizeof(CFG_CROSSLINE_INFO));
		else
		{
			SET_ERRPARAM;
			return false;
		}
		SET_NOERROR;
		return true;
	}
	case 2:
	{
		CFG_CROSSFENCEDETECTION_INFO* tmp = static_cast<CFG_CROSSFENCEDETECTION_INFO*>(lpAlarmSetupParam);
		if (nullptr != tmp)
			memcpy(&m_struCrossFenceInf, tmp, sizeof(CFG_CROSSFENCEDETECTION_INFO));
		else
		{
			SET_ERRPARAM;
			return false;
		}
		SET_NOERROR;
		return true;
	}
	case 3:
	{
		CFG_DENSITYDETECTION_INFO* tmp = static_cast<CFG_DENSITYDETECTION_INFO*>(lpAlarmSetupParam);
		if (nullptr != tmp)
			memcpy(&m_struDensityInf, tmp, sizeof(CFG_DENSITYDETECTION_INFO));
		else
		{
			SET_ERRPARAM;
			return false;
		}
		SET_NOERROR;
		return true;
	}
	case 4:
	{
		CFG_TRAFFICVIOLATIONS_INFO* tmp = static_cast<CFG_TRAFFICVIOLATIONS_INFO*>(lpAlarmSetupParam);
		if (nullptr != tmp)
			memcpy(&m_struTrafficInf, tmp, sizeof(CFG_TRAFFICVIOLATIONS_INFO));
		else
		{
			SET_ERRPARAM;
			return false;
		}
		SET_NOERROR;
		return true;
	}
	default:
		SET_ERRTYPE;
		return false;
	}
}

bool ZhSmartAlarm::CheckPlace(int ruleType, DETECTALARM_INFO &detectInfo)
{
	//if (detectInfo.direct == -1 && detectInfo.place == -1)
	SET_NOERROR;

	Point2D pointL;
	Point2D pointN;
	Point2D pointS;
	Point2D pointE;
	int place = -2;

	int nret1 = -2;
	int nret2 = -2;

	int nDirection = -1;
	int nPlace = -1;
	if (detectInfo.blUseRule)
	{
		if (ruleType == 1)
		{
			nDirection = m_struCrosslineInf.nDirection;
			nPlace = m_struCrosslineInf.nPlace;
		}
		else
		{
			nDirection = m_struCrossFenceInf.nDirection;
			nPlace = m_struCrossFenceInf.nPlace;
		}
	}
	else
	{
		nDirection = detectInfo.nDirect;
		nPlace = detectInfo.nPlace;
	}
	//if (m_struCrosslineInf.nDirection != -1) //detectInfo.direct == -1)
	//{
		//只判断方向
	switch (ruleType)
	{
	case 1:
	{
		if (nDirection == -1 && nPlace == -1)
		{
			SET_ERRTYPE;
			return false;
		}
		if (nPlace != -1)
		{
			if (m_struCrosslineInf.nDetectLinePoint > 1)
			{
				pointS = ZhcvMath::GetPerpendicularPointWithDistance(m_struCrosslineInf.stuDetectLine[0].x, m_struCrosslineInf.stuDetectLine[0].y, m_struCrosslineInf.stuDetectLine[1].x,
						m_struCrosslineInf.stuDetectLine[1].y, 0.03, nPlace);
				detectInfo.currentAlert.reftx = pointS.x;
				detectInfo.currentAlert.refty = pointS.y;
			}

			//判断位置   //nPlace 绊线 0: 右边 1: 左边  2:在线上; -1不判断
			//多条线段首尾连接  绊线
			
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				pointN.x = detectInfo.currentAlert.refx;
				pointN.y = detectInfo.currentAlert.refy;

				//有一条线段不符合 即认为不符合 //nPlace绊线 0: 右边 1: 左边  2:在线上; -1不判断; 
				//LeftOfLine_Area当tmpx>p.x的时候，说明点在线的左边1，小于在右边-1，等于则在线上0。
				int nr = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);
				bool bl = false;
				if (nr == 0 && 2 == nPlace) //detectInfo.place)
				{
					bl = true;
				}
				else if (nr == 1 && 1 == nPlace)
				{
					bl = true;
				}
				else if (nr == -1 && 0 == nPlace)
					bl = true;

				if (!bl)
					return bl;

				//判断距离 
				if (detectInfo.fDistance > EPSILON_FLOAT)
				{
					bool blCross;
					if (ZhcvMath::calculateDistance(pointS.x, pointS.y, pointE.x, pointE.y, pointN.x, pointN.y, blCross) <= detectInfo.fDistance)
					{
						return blCross;
					}
				}
				else
					return true;
			}
			return false;
		}
		else
		{
			int nPlace1 = -2;
			int nPlace2 = -2;

			if (m_struCrosslineInf.nDetectLinePoint > 1)
			{
				pointS = ZhcvMath::GetPerpendicularPointWithDistance(m_struCrosslineInf.stuDetectLine[0].x, m_struCrosslineInf.stuDetectLine[0].y, m_struCrosslineInf.stuDetectLine[1].x,
					m_struCrosslineInf.stuDetectLine[1].y, 0.03, nPlace);
				detectInfo.currentAlert.reftx = pointS.x;
				detectInfo.currentAlert.refty = pointS.y;
			}
			else
			{
				detectInfo.currentAlert.reftx = detectInfo.currentAlert.refx;// + detectInfo.currentAlert.fw / 2.0f;
				detectInfo.currentAlert.refty = detectInfo.currentAlert.refy;// + detectInfo.currentAlert.fh / 2.0f;
			}

			//多条线段首尾连接
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				//第一个点
				pointN.x = detectInfo.lastAlert.fx;// + detectInfo.lastAlert.fw / 2.0f;
				pointN.y = detectInfo.lastAlert.fy;// + detectInfo.lastAlert.fh / 2.0f;
				//有一条线段不符合 即认为不符合
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection)
				{
				case 0:
				{
					//要求 从左到右 实际为右或者在线上
					if (nPlace1 == -1)
					{
						return false;
					}
					if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
					{
						return false;
					}
					break;
				}
				case 1:
				{
					if (nPlace1 == 1)
					{
						return false;
					}
					if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
					{
						return false;
					}
					break;
				}
				case 2:
				{
					if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
					{
						return false;
					}
					break;
				}
				}

				//第二个点
				pointN.x = detectInfo.currentAlert.refx;// + detectInfo.currentAlert.fw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy;// + detectInfo.currentAlert.fh / 2.0f;
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection) 
				{
				case 0:
				{
					//要求 从左到右 实际为右或者在线上
					if (nPlace2 == -1)
					{
						return false;
					}
					if (nPlace2 == 0 && detectInfo.blBoundOrVertex == false)
					{
						return false;
					}
					break;
				}
				case 1:
				{
					if (nPlace2 == 1)
					{
						return false;
					}
					if (nPlace2 == 0 && detectInfo.blBoundOrVertex == false)
					{
						return false;
					}
					break;
				}
				case 2:
				{
					if (nPlace2 == 0 && detectInfo.blBoundOrVertex == false)
					{
						return false;
					}
					break;
				}
				}
				//从任意一条线段越过 即认为报警
				if (nPlace1 != nPlace2)
					return true;
			}

			return false;
		}
	}
	case 2:
	{
		//规则设置不正确
		if (nDirection == -1 && nPlace == -1)
		{
			SET_ERRTYPE;
			return false;
		}
		if (nPlace != -1)
		{
			if (m_struCrossFenceInf.nDetectRegionPoint > 1)
			{
				pointS = ZhcvMath::GetPerpendicularPointWithDistance(m_struCrossFenceInf.stuDetectRegion[0].x, m_struCrossFenceInf.stuDetectRegion[0].y, m_struCrossFenceInf.stuDetectRegion[1].x,
					m_struCrossFenceInf.stuDetectRegion[1].y, 0.03, nPlace);
				detectInfo.currentAlert.reftx = pointS.x;
				detectInfo.currentAlert.refty = pointS.y;
			}

			//判断位置1个点
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
			pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
			//在多边形内部  nPlace 0:外面; 1:里面 2:在线上;

			return ZhcvMath::IsPtInPoly(polygon, pointN) == nPlace;// detectInfo.place;
		}
		else
		{
			//判断方向 两个点
			int nPlace1 = -2;
			int nPlace2 = -2;
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			//第一个点
			pointL.x = detectInfo.lastAlert.fx + detectInfo.lastAlert.fw / 2.0f;
			pointL.y = detectInfo.lastAlert.fy + detectInfo.lastAlert.fh / 2.0f;
			nPlace1 = ZhcvMath::IsPtInPoly(polygon, pointL);

			switch (nDirection) //detectInfo.direct)
			{
				//0:由外到里;1:由里到外 2:两者都可以
			case 0:
			{
				if (nPlace1 == 1)
					return false;
				if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == -1)
					return false;
				if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}

				return true;
			}
			case 1:
			{
				if (nPlace1 == -1)
					return false;
				if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == 1)
					return false;
				if (nPlace2 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}

				return true;
			}
			case 2:
			{
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);

				if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}
				if (nPlace2 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}

				if (nPlace2 != nPlace1)
					return true;
				else
					return false;
			}
			default:
				SET_ERRPARAM;
				return false;
			}
		}
	}
	default:
	{
		SET_ERRTYPE;
		return false;
	}
	}

	//}
	//else
/*	{
		//判断越线 或入侵
		switch (ruleType)
		{
		case 1:
		{ //绊线
			//bool bl = false;

			int nPlace1 = -2;
			int nPlace2 = -2;
		  //多条线段首尾连接
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				//第一个点
				pointN.x = detectInfo.lastAlert.x;
				pointN.y = detectInfo.lastAlert.y;
				//有一条线段不符合 即认为不符合
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);
				switch (m_struCrosslineInf.nDirection) //detectInfo.direct)
				{
					case 0:
					{
						//要求 从左到右 实际为右或者在线上
						if (nPlace1 == -1)
						{
							return false;
						}
						if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
						{
							return false;
						}
						break;
					}
					case 1:
					{
						if (nPlace1 == 1)
						{
							return false;
						}
						if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
						{
							return false;
						}
						break;
					}
					case 2:
					{
						if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
						{
							return false;
						}
						break;
					}
				}

				//第二个点
				pointN.x = detectInfo.currentAlert.x;
				pointN.y = detectInfo.currentAlert.y;
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);
				switch (m_struCrosslineInf.nDirection) //switch (detectInfo.direct)
				{
					case 0:
					{
						//要求 从左到右 实际为右或者在线上
						if (nPlace2 == -1)
						{
							return false;
						}
						if (nPlace2 == 0 && detectInfo.boundOrVertex == false)
						{
							return false;
						}
						break;
					}
					case 1:
					{
						if (nPlace2 == 1)
						{
							return false;
						}
						if (nPlace2 == 0 && detectInfo.boundOrVertex == false)
						{
							return false;
						}
						break;
					}
					case 2:
					{
						if (nPlace2 == 0 && detectInfo.boundOrVertex == false)
						{
							return false;
						}
						break;
					}
				}
				//从任意一条线段越过 即认为报警
				if (nPlace1 != nPlace2)
					return true;
			}
			
			return false;
		}

		case 2:
		{ //周界
			int nPlace1 = -2;
			int nPlace2 = -2;
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			//第一个点
			pointL.x = detectInfo.lastAlert.x;
			pointL.y = detectInfo.lastAlert.y;
			nPlace1 = ZhcvMath::IsPtInPoly(polygon, pointL);

			switch (m_struCrossFenceInf.nDirection) //detectInfo.direct)
			{
				//0:由外到里;1:由里到外 2:两者都可以
			case 0:
			{
				if (nPlace1 == 1)
					return false;
				if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.x;
				pointN.y = detectInfo.currentAlert.y;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == -1)
					return false;
				if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}
				
				return true;
			}
			case 1:
			{
				if (nPlace1 == -1)
					return false;
				if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.x;
				pointN.y = detectInfo.currentAlert.y;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == 1)
					return false;
				if (nPlace2 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}

				return true;
			}
			case 2:
			{
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.x;
				pointN.y = detectInfo.currentAlert.y;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);

				if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}
				if (nPlace2 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}

				if (nPlace2 != nPlace1)
					return true;
				else
					return false;
			}
			default:
				SET_ERRPARAM;
				return false;
			}
		}
		default:
			SET_ERRTYPE;
			return false;
		}
		*/
		//switch (detectInfo.place)
		//{ //绊线0:由左到右; 1:从右到左 2 : 穿过   周界0:由外到里;1:由里到外 2:两者都可以
		//case 0:
		//{
		//	if (nret1 == 1 && ((nret2 == -1) || (nret2 >= 0 && detectInfo.boundOrVertex)))
		//		return true;
		//	else
		//		return false;
		//}
		//case 1:
		//{
		//	if (nret1 == -1 && ((nret2 == 1) || (nret2 <= 0 && detectInfo.boundOrVertex)))
		//		return true;
		//	else
		//		return false;
		//}
		//case 2:
		//{
		//	if ((nret1 == 1 && ((nret2 == -1) || (nret2 >= 0 && detectInfo.boundOrVertex)))
		//		|| (nret1 == -1 && ((nret2 == 1) || (nret2 <= 0 && detectInfo.boundOrVertex))))
		//		return true;
		//	else
		//		return false;
		//}
		//default:
		//	SET_ERRPARAM;
		//	return false;
		//}
	//}
}

int MakeTimeDaySegment(std::vector<ZhSmartAlarm::TimeDaySegment> &vttd, CFG_TIME_SECTION stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX])
{
	for (int im = 0; im < MAX_MONTH_NUM; im++)
	{
		for (int jd = 0; jd < MAX_DAY_NUM; jd++)
		{
			for (int kt = 0; kt < MAX_REC_TSECT_EX; kt++)
			{
				if (stuDayTimeSection[im][jd][kt].nBeginHour >= 0 && stuDayTimeSection[im][jd][kt].nEndHour >= 0)
				{
					bool blFound = false;
					ZhSmartAlarm::TimeDaySegment *ttdtmp;
					for (auto &ttd : vttd)
					{
						if (stuDayTimeSection[im][jd][kt].nBeginHour == ttd.start_tm_hour &&
							stuDayTimeSection[im][jd][kt].nEndHour == ttd.end_tm_hour &&
							stuDayTimeSection[im][jd][kt].nBeginMin == ttd.start_tm_min &&
							stuDayTimeSection[im][jd][kt].nEndMin == ttd.end_tm_min)
						{
							ttdtmp = &ttd;
							blFound = true;
							break;
						}
					}
					if (!blFound)
					{
						ZhSmartAlarm::TimeDaySegment tds;
						tds.start_tm_hour = stuDayTimeSection[im][jd][kt].nBeginHour;
						tds.end_tm_hour = stuDayTimeSection[im][jd][kt].nEndHour;
						tds.start_tm_min = stuDayTimeSection[im][jd][kt].nBeginMin;
						tds.end_tm_min = stuDayTimeSection[im][jd][kt].nEndMin;
						tds.days.push_back(jd);
						tds.mons.push_back(im);

						vttd.push_back(tds);
					}
					else
					{
						blFound = false;
						for (auto td : ttdtmp->days)
						{
							if (td == jd)
							{
								blFound = true;
								break;
							}
						}
						if (!blFound)
						{
							ttdtmp->days.push_back(jd);
						}

						blFound = false;
						for (auto tm : ttdtmp->mons)
						{
							if (tm == im)
							{
								blFound = true;
								break;
							}
						}
						if (!blFound)
						{
							ttdtmp->mons.push_back(im);
						}
					}
				}
			}
		}
	}

	return vttd.size();
}

//int MakeTimeWeekSegment(std::vector<ZhSmartAlarm::TimeWeekSegment> &vttw, CFG_TIME_SECTION stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX])
int MakeTimeWeekSegment(std::vector<ZhSmartAlarm::TimeWeekSegment> &vttw, CFG_TIME_SECTION stuWeekTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX])
{
	//开始时间为 负数的不生效
	for (int im = 0; im < MAX_MONTH_NUM; im++)
	{
		//c
		for (int jd = 0; jd < MAX_WEEK_DAY_NUM; jd++)
		{
			for (int kt = 0; kt < MAX_REC_TSECT_EX; kt++)
			{
				if (stuWeekTimeSection[im][jd][kt].nBeginHour >= 0 && stuWeekTimeSection[im][jd][kt].nEndHour >= 0)
				{
					bool blFound = false;
					ZhSmartAlarm::TimeWeekSegment *ttwtmp;
					for (auto &ttw : vttw)
					{
						if (stuWeekTimeSection[im][jd][kt].nBeginHour == ttw.start_tm_hour &&
							stuWeekTimeSection[im][jd][kt].nEndHour == ttw.end_tm_hour &&
							stuWeekTimeSection[im][jd][kt].nBeginMin == ttw.start_tm_min &&
							stuWeekTimeSection[im][jd][kt].nEndMin == ttw.end_tm_min)
						{
							ttwtmp = &ttw;
							blFound = true;
							break;
						}
					}
					if (!blFound)
					{
						ZhSmartAlarm::TimeWeekSegment tws;
						tws.start_tm_hour = stuWeekTimeSection[im][jd][kt].nBeginHour;
						tws.end_tm_hour = stuWeekTimeSection[im][jd][kt].nEndHour;
						tws.start_tm_min = stuWeekTimeSection[im][jd][kt].nBeginMin;
						tws.end_tm_min = stuWeekTimeSection[im][jd][kt].nEndMin;
						tws.weeks.push_back(jd);
						tws.mons.push_back(im);

						vttw.push_back(tws);
					}
					else
					{
						blFound = false;
						for (auto tw : ttwtmp->weeks)
						{
							if (tw == jd)
							{
								blFound = true;
								break;
							}
						}
						if (!blFound)
						{
							ttwtmp->weeks.push_back(jd);
						}

						blFound = false;
						for (auto tm : ttwtmp->mons)
						{
							if (tm == im)
							{
								blFound = true;
								break;
							}
						}
						if (!blFound)
						{
							ttwtmp->mons.push_back(im);
						}
					}
				}
			}
		}
	}

	return vttw.size();
}

int ZhSmartAlarm::DetectCheck(int ruleType, DETECTALARM_INFO &alarmInfo)
{
	int nDirection = -1;
	int nPlace = -1;
	if (alarmInfo.blUseRule)
	{
		nDirection = m_struCrosslineInf.nDirection;
		nPlace = m_struCrosslineInf.nPlace;
	}
	else
	{
		nDirection = alarmInfo.nDirect;
		nPlace = alarmInfo.nPlace;
	}

	if (alarmInfo.nDirect == -1 && alarmInfo.nPlace == -1)
	{
		SET_ERRTYPE;
		return -1;
	}
	//if (m_struCrosslineInf.nDirection == -1 && m_struCrosslineInf.nPlace == -1)
	//{
	//	SET_ERRTYPE;
	//	return -1;
	//}
	
	SET_NOERROR;
	switch (ruleType)
	{
	case 1:
	{ //拌线
		//if (m_struCrosslineInf.nPlace != -1) //alarmInfo.place != -1)
		if (alarmInfo.blUseRule == true)
		{
			//1. 判断时间
			struct tm *when1 = localtime((time_t*)&alarmInfo.currentAlert.timestamp);

			for (int it = 0; it < m_struCrossFenceInf.nTimeSection; it++)
			{
				switch (m_struCrossFenceInf.guardTimes[it].guardDateTimeType)
				{
				case 1:
				case 2:
				{
					std::vector<TimeDaySegment> vttd;

					if (MakeTimeDaySegment(vttd, m_struCrosslineInf.guardTimes[it].stuDayTimeSection) > 0)
					{
						int ip = InTimeDaySeg(vttd, alarmInfo.currentAlert.timestamp);
						if (ip < 0)
							return 2;
					}

					break;
				}
				case 3:
				{
					std::vector<TimeWeekSegment> vttw;
					if (MakeTimeWeekSegment(vttw, m_struCrosslineInf.guardTimes[it].stuDayTimeSection) > 0)
					{
						int ip = InTimeWeekSeg(vttw, alarmInfo.currentAlert.timestamp);
						if (ip < 0)
							return 3;
					}

					break;
				}
				case 0:
				default:
				{
					//不判断时间
					break;
				}
				}
			}
		}
		//2. 判断规则
		if (CheckPlace(ruleType, alarmInfo))
			return 1;
		else
			return 0;
		break;
	}
	case 2:
	{   //周界
	    //1. 时间
		if (alarmInfo.blUseRule == true)
		{
			struct tm *when1 = localtime((time_t*)&alarmInfo.currentAlert.timestamp);

			for (int it = 0; it < m_struCrossFenceInf.nTimeSection; it++)
			{
				switch (m_struCrossFenceInf.guardTimes[it].guardDateTimeType)
				{
				case 1:
				case 2:
				{
					std::vector<TimeDaySegment> vttd;

					if (MakeTimeDaySegment(vttd, m_struCrossFenceInf.guardTimes[it].stuDayTimeSection) > 0)
					{
						int ip = InTimeDaySeg(vttd, alarmInfo.currentAlert.timestamp);
						if (ip < 0)
							return 2;
					}
					break;
				}
				case 3:
				{
					std::vector<TimeWeekSegment> vttw;
					if (MakeTimeWeekSegment(vttw, m_struCrossFenceInf.guardTimes[it].stuDayTimeSection) > 0)
					{
						int ip = InTimeWeekSeg(vttw, alarmInfo.currentAlert.timestamp);
						if (ip < 0)
							return 3;
					}
					break;
				}
				case 0:
				default:
					break;
				}
			}
		}
	   //2. 规则
		if (CheckPlace(ruleType, alarmInfo))
			return 1;
		else
			return 0;
	}
	default:
		SET_ERRTYPE;
		return -1;
	}
	
	return 0;
}

/**
* @brief 2.3 人群密度报警判断
* @param
*	alarmInfo 检测数据信息
* @return 返回值
*	成功返回 1 符合报警规则 0不符合无警报
*	失败返回负值
*/
int ZhSmartAlarm::DensityCheck(DENSITYALARM_INFO &alarmInfo)
{
	SET_NOERROR;
	if (alarmInfo.vStatistical.size() == 0)
		return 0;
	//规则不生效
	if (!m_struDensityInf.blRuleEnable)
		return 0;
	uint64_t tm1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	for (int it = 0; it < m_struCrossFenceInf.nTimeSection; it++)
	{
		switch (m_struCrossFenceInf.guardTimes[it].guardDateTimeType)
		{
		case 1:
		case 2:
		{
			std::vector<TimeDaySegment> vttd;

			if (MakeTimeDaySegment(vttd, m_struCrossFenceInf.guardTimes[it].stuDayTimeSection) > 0)
			{
				int ip = InTimeDaySeg(vttd, tm1);
				if (ip < 0)
					return -2;
			}
			break;
		}
		case 3:
		{
			std::vector<TimeWeekSegment> vttw;
			if (MakeTimeWeekSegment(vttw, m_struCrossFenceInf.guardTimes[it].stuDayTimeSection) > 0)
			{
				int ip = InTimeWeekSeg(vttw, tm1);
				if (ip < 0)
					return -3;
			}
			break;
		}
		case 0:
		default:
			break;
		}
	}

	//2. 区域规则 
	//2.1 统计区域内的值
	float ftotal = 0.0f;

	for (int ii = 0; ii < m_struDensityInf.nDetectRegionPoint - 1; ii++)
	{
		for (int ic = 0; ic < alarmInfo.nWidth - 1; ic++)
		{
			for (int ir = 0; ir < alarmInfo.nHeight - 1; ir++)
			{
				if (ZhcvMath::CSClip_Line(1.0f / alarmInfo.nWidth * (ic + 1), 1.0f / alarmInfo.nHeight * (ir + 1),
					1.0f / alarmInfo.nWidth * ic, 1.0f / alarmInfo.nHeight * ir, false,
					m_struDensityInf.stuDetectRegion[ii].x, m_struDensityInf.stuDetectRegion[ii].y,
					m_struDensityInf.stuDetectRegion[ii + 1].x, m_struDensityInf.stuDetectRegion[ii + 1].y) > 0)
				{
					ftotal += alarmInfo.vStatistical[ir * alarmInfo.nWidth + ic];
				}
			}
		}
	}

	//没超过阈值 不报警 byUnit==0 密度等级 gbt
	if ((ftotal < m_struDensityInf.nThresholdNum / 100.0f && m_struDensityInf.byUnit == 1) 
		|| ( (m_struDensityInf.byUnit == 2 || m_struDensityInf.byUnit == 0 ) && ftotal < m_struDensityInf.nThresholdNum))
		return 0;
	else
	{
		std::vector<Point2D> vt1;
		Point2D ptmin(0.0, 0.0);
		Point2D ptmax(0.0, 0.0);
		for (int iii = 0; iii < m_struDensityInf.nDetectRegionPoint; iii++)
		{
			vt1.push_back(m_struDensityInf.stuDetectRegion[iii]);
			if (ptmin.x <= EPSILON_FLOAT)
			{
				ptmin.x = m_struDensityInf.stuDetectRegion[iii].x;
			}
			else if (ptmin.x >= m_struDensityInf.stuDetectRegion[iii].x)
			{
				ptmin.x = m_struDensityInf.stuDetectRegion[iii].x;
			}
			if (ptmin.y<= EPSILON_FLOAT)
			{
				ptmin.y = m_struDensityInf.stuDetectRegion[iii].y;
			}
			else if (ptmin.y >= m_struDensityInf.stuDetectRegion[iii].y)
			{
				ptmin.y = m_struDensityInf.stuDetectRegion[iii].y;
			}

			if (ptmax.x <= EPSILON_FLOAT)
			{
				ptmax.x = m_struDensityInf.stuDetectRegion[iii].x;
			}
			else if (ptmax.x <= m_struDensityInf.stuDetectRegion[iii].x)
			{
				ptmax.x = m_struDensityInf.stuDetectRegion[iii].x;
			}
			if (ptmax.y <= EPSILON_FLOAT)
			{
				ptmax.y = m_struDensityInf.stuDetectRegion[iii].y;
			}
			else if (ptmax.y <= m_struDensityInf.stuDetectRegion[iii].y)
			{
				ptmax.y = m_struDensityInf.stuDetectRegion[iii].y;
			}
		}
		//Point2D pt;
		//pt = ZhcvMath::GetCenterOfGravityPoint(vt1);
		alarmInfo.currentAlert.refx = (ptmax.x + ptmin.x ) /2;
		alarmInfo.currentAlert.refy = (ptmax.y + ptmin.y ) / 2;
		alarmInfo.currentAlert.refw = ptmax.x - ptmin.x;
		alarmInfo.currentAlert.refh = ptmax.y - ptmin.y;
		alarmInfo.fAlarmTotal = ftotal;
		return 1;
	}

	return 0;
}
                     
/**
* @brief 2.4 人群密度统计 
* @param
*	alarmInfo 检测数据信息
* @return 返回值
*	成功返回 输入参数里面大于等于 densityThreshold项的值 报警门限 为当前画面上周界内具体的目标个数
*	失败返回负值
*/
int ZhSmartAlarm::DensityStatistical(DENSITYALARM_INFO &alarmInfo, bool blUseCfg, std::vector<Point2D> vp)
{
	SET_NOERROR;
	if (alarmInfo.vStatistical.size() == 0)
		return 0;

	//for (auto k : alarmInfo.vStatistical)
	//{
		//目前只关心 人群 其他的暂不处理
		//if (k.first == "person")
		//{
			// 统计区域内的值
			float ftotal = 0.0f;
			//使用设置统计
			if (blUseCfg)
			{
				//第一层 检测区域折线
				for (int ii = 0; ii < m_struDensityInf.nDetectRegionPoint; ii++)
				{
					for (int ic = 0; ic < alarmInfo.nWidth - 1; ic++)
					{
						for (int ir = 0; ir < alarmInfo.nHeight - 1; ir++)
						{
							//有交叉的就计算
							double x1 = 0.0;
							double y1 = 0.0;
							double x2 = 0.0;
							double y2 = 0.0;
							if (ii < vp.size() - 1)
							{
								x1 = m_struDensityInf.stuDetectRegion[ii].x;
								y1 = m_struDensityInf.stuDetectRegion[ii].y;
								x2 = m_struDensityInf.stuDetectRegion[ii + 1].x;
								y2 = m_struDensityInf.stuDetectRegion[ii + 1].y;
							}
							else
							{
								x1 = m_struDensityInf.stuDetectRegion[ii].x;
								y1 = m_struDensityInf.stuDetectRegion[ii].y;
								x2 = m_struDensityInf.stuDetectRegion[0].x;
								y2 = m_struDensityInf.stuDetectRegion[0].y;
							}

							//有交叉的就计算
							if (ZhcvMath::CSClip_Line(1.0f / alarmInfo.nWidth * (ic + 1), 1.0f / alarmInfo.nHeight * (ir + 1),
								1.0f / alarmInfo.nWidth * ic, 1.0f / alarmInfo.nHeight * ir, false,
								m_struDensityInf.stuDetectRegion[ii].x, m_struDensityInf.stuDetectRegion[ii].y,
								m_struDensityInf.stuDetectRegion[ii + 1].x, m_struDensityInf.stuDetectRegion[ii + 1].y) > 0)
							{
								//加数量
								ftotal += alarmInfo.vStatistical[ir * alarmInfo.nWidth + ic];
							}
						}
					}
				}
			}
			else
			{
				//使用外部参数统计
				//第一层 检测区域折线
				for (int ii = 0; ii < vp.size() - 1; ii++)
				{
					for (int ic = 0; ic < alarmInfo.nWidth - 1; ic++)
					{
						for (int ir = 0; ir < alarmInfo.nHeight - 1; ir++)
						{
							//有交叉的就计算
							double x1 = 0.0;
							double y1 = 0.0;
							double x2 = 0.0;
							double y2 = 0.0;
							if (ii < vp.size() - 1)
							{
								x1 = vp[ii].x;
								y1 = vp[ii].y;
								x2 = vp[ii + 1].x;
								y2 = vp[ii + 1].y;
							}
							else
							{
								x1 = vp[ii].x;
								y1 = vp[ii].y;
								x2 = vp[0].x;
								y2 = vp[0].y;
							}
							//折线穿过区域
							if (ZhcvMath::CSClip_Line(1.0f / alarmInfo.nWidth * (ic + 1), 1.0f / alarmInfo.nHeight * (ir + 1),
								1.0f / alarmInfo.nWidth * ic, 1.0f / alarmInfo.nHeight * ir, false,
								x1, y1, x2, y2) > 0)
							{
								//加数量
								ftotal += alarmInfo.vStatistical[ir * alarmInfo.nWidth + ic];
							}
						}
					}
				}
			}

			////total 里面的key个数和内容必须和statistical 里面的个数和内容一致
			//std::map<std::string, int>::iterator it = alarmInfo.total.find(k.first);
			//if (it == alarmInfo.total.end())
			//{
			//	//参数不匹配
			//	SET_ERRPARAM;
			//	return -1;
			//}
			alarmInfo.fAlarmTotal = ftotal;
			//返回区域内目标个数 百分数 * 总数
			//return ftotal * it->second;
				
			return 1;
		//}
		//else
		//{
		//	//todo
		//	SET_ERRPARAM;
		//}
	//}

	return -1;
}

bool ZhSmartAlarm::CheckTrafficSpeed(Alarm_RuleType nRuleType, std::vector<TRAFFICALARM_INFO> &detectInfo)
{
	if (detectInfo.size() == 0)
	{
		m_nErrorCode = -1;
		return false;
	}

	//使用第一个元素的配置内容做判断
	TRAFFICALARM_INFO & alarmInfo = detectInfo[0];

	if (alarmInfo.blUseRule == true)
	{
		//时间
		struct tm *when1 = localtime((time_t*)&alarmInfo.currentAlert.timestamp);

		for (int it = 0; it < m_struTrafficInf.nTimeSection; it++)
		{
			switch (m_struTrafficInf.guardTimes[it].guardDateTimeType)
			{
			case 1:
			case 2:
			{
				std::vector<TimeDaySegment> vttd;

				if (MakeTimeDaySegment(vttd, m_struTrafficInf.guardTimes[it].stuDayTimeSection) > 0)
				{
					int ip = InTimeDaySeg(vttd, alarmInfo.currentAlert.timestamp);
					if (ip < 0)
					{
						m_nErrorCode = -2;
						return false;
					}
				}
				break;
			}
			case 3:
			{
				std::vector<TimeWeekSegment> vttw;
				if (MakeTimeWeekSegment(vttw, m_struTrafficInf.guardTimes[it].stuDayTimeSection) > 0)
				{
					int ip = InTimeWeekSeg(vttw, alarmInfo.currentAlert.timestamp);
					if (ip < 0)
					{
						m_nErrorCode = -3;
						return false;
					}
				}
				break;
			}
			case 0:
			default:
				break;
			}
		}
	}

	Point2D p1(alarmInfo.currentAlert.refx + alarmInfo.currentAlert.refw / 2.0, alarmInfo.currentAlert.refy + alarmInfo.currentAlert.refh / 2.0);

	float speed_sum = 0.0f;
	float speed_avg = 0.0f;
	int speed_count = 0;

	for (auto& inf : detectInfo)
	{
		double distance1;
		if (alarmInfo.currentAlert.timestamp == inf.currentAlert.timestamp)
		{
			continue;
		}
		Point2D p2(inf.currentAlert.refx + inf.currentAlert.refw / 2.0, inf.currentAlert.refy + inf.currentAlert.refh / 2.0);
		bool bl = ZhcvMath::CalculateRealDistance(m_mapTableX, m_mapTableY, p1, p2, distance1);
		// *************** mi /millsecond = mi * 1000 / (millsecond / 1000 / 3600) = km/ h
		inf.fSpeed = distance1 / ((inf.currentAlert.timestamp - inf.currentAlert.timestamp) / 3600);
		
		speed_sum += inf.fSpeed;
		speed_avg = (speed_sum + inf.fSpeed) / 2.0;
		speed_count++;
	}

	speed_avg = speed_sum / speed_count;
	//第一个元素得到的是平均值 ，后面的都是每个元素对应第一个元素的瞬时值 当平均值> 限速值时报警！
	alarmInfo.fSpeed = speed_avg;

	//1.1 系数是国家计量局规定的允许误差范围
	if (Alarm_RuleType::ZH_ALARM_EXCESSIVESPEED == nRuleType && alarmInfo.fSpeed > alarmInfo.fSpeedLimit *(1.1))
	{ //超速
		return true;
	}
	else if (Alarm_RuleType::ZH_ALARM_WRONGSIDE && alarmInfo.fSpeed < 5.0f)
	{
		//逆行 时速5公里以上的 判断为 倒车或者逆行
		return true;
	}
	else
		return false;
}

bool ZhSmartAlarm::CheckTrafficPlace(Alarm_RuleType nRuleType, TRAFFICALARM_INFO &detectInfo)
{
	SET_NOERROR;

	if (detectInfo.blUseRule == true)
	{
		//时间
		struct tm *when1 = localtime((time_t*)&detectInfo.currentAlert.timestamp);

		for (int it = 0; it < m_struTrafficInf.nTimeSection; it++)
		{
			switch (m_struTrafficInf.guardTimes[it].guardDateTimeType)
			{
			case 1:
			case 2:
			{
				std::vector<TimeDaySegment> vttd;

				if (MakeTimeDaySegment(vttd, m_struTrafficInf.guardTimes[it].stuDayTimeSection) > 0)
				{
					int ip = InTimeDaySeg(vttd, detectInfo.currentAlert.timestamp);
					if (ip < 0)
					{
						m_nErrorCode = -2;
						return false;
					}
				}
				break;
			}
			case 3:
			{
				std::vector<TimeWeekSegment> vttw;
				if (MakeTimeWeekSegment(vttw, m_struTrafficInf.guardTimes[it].stuDayTimeSection) > 0)
				{
					int ip = InTimeWeekSeg(vttw, detectInfo.currentAlert.timestamp);
					if (ip < 0)
					{
						m_nErrorCode = -3;
						return false;
					}
				}
				break;
			}
			case 0:
			default:
				break;
			}
		}
	}

	Point2D pointL;
	Point2D pointN;
	Point2D pointS;
	Point2D pointE;
	int place = -2;

	int nret1 = -2;
	int nret2 = -2;

	int nDirection = -1;
	int nPlace = -1;
	if (detectInfo.blUseRule)
	{
		if (nRuleType == 1)
		{
			nDirection = m_struCrosslineInf.nDirection;
			nPlace = m_struCrosslineInf.nPlace;
		}
		else
		{
			nDirection = m_struCrossFenceInf.nDirection;
			nPlace = m_struCrossFenceInf.nPlace;
		}
	}
	else
	{
		nDirection = detectInfo.nDirect;
		nPlace = detectInfo.nPlace;
	}

	//只判断方向
	switch (nRuleType)
	{
	case 1:
	{
		if (nDirection == -1 && nPlace == -1)
		{
			SET_ERRTYPE;
			return false;
		}
		if (nPlace != -1)
		{
			if (m_struCrosslineInf.nDetectLinePoint > 1)
			{
				pointS = ZhcvMath::GetPerpendicularPointWithDistance(m_struCrosslineInf.stuDetectLine[0].x, 
					m_struCrosslineInf.stuDetectLine[0].y, m_struCrosslineInf.stuDetectLine[1].x,
					m_struCrosslineInf.stuDetectLine[1].y, 0.03, nPlace);
				detectInfo.currentAlert.reftx = pointS.x;
				detectInfo.currentAlert.refty = pointS.y;
			}

			//判断位置   //nPlace 绊线 0: 右边 1: 左边  2:在线上; -1不判断
			//多条线段首尾连接  绊线
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				pointN.x = detectInfo.currentAlert.refx;
				pointN.y = detectInfo.currentAlert.refy;

				//有一条线段不符合 即认为不符合 nPlace绊线 0: 右边 1: 左边  2:在线上; -1不判断; 
				//LeftOfLine_Area当tmpx>p.x的时候，说明点在线的左边1，小于在右边-1，等于则在线上0。
				int nr = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);
				bool bl = false;
				if (nr == 0 && 2 == nPlace)
				{
					bl = true;
				}
				else if (nr == 1 && 1 == nPlace)
				{
					bl = true;
				}
				else if (nr == -1 && 0 == nPlace)
					bl = true;

				if (!bl)
					return bl;

				//判断距离 
				return true;
			}
			return false;
		}
		else
		{
			int nPlace1 = -2;
			int nPlace2 = -2;

			if (m_struCrosslineInf.nDetectLinePoint > 1)
			{
				pointS = ZhcvMath::GetPerpendicularPointWithDistance(m_struCrosslineInf.stuDetectLine[0].x, m_struCrosslineInf.stuDetectLine[0].y, m_struCrosslineInf.stuDetectLine[1].x,
					m_struCrosslineInf.stuDetectLine[1].y, 0.03, nPlace);
				detectInfo.currentAlert.reftx = pointS.x;
				detectInfo.currentAlert.refty = pointS.y;
			}
			else
			{
				detectInfo.currentAlert.reftx = detectInfo.currentAlert.refx;
				detectInfo.currentAlert.refty = detectInfo.currentAlert.refy;
			}

			//多条线段首尾连接
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				//第一个点
				pointN.x = detectInfo.lastAlert.fx;
				pointN.y = detectInfo.lastAlert.fy;
				//有一条线段不符合 即认为不符合
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection)
				{
				case 0:
				{
					//要求 从左到右 实际为右或者在线上
					if (nPlace1 == -1)
					{
						return false;
					}
					break;
				}
				case 1:
				{
					if (nPlace1 == 1)
					{
						return false;
					}

					break;
				}
				case 2:
				{
					break;
				}
				}

				//第二个点
				pointN.x = detectInfo.currentAlert.refx;
				pointN.y = detectInfo.currentAlert.refy;
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection)
				{
				case 0:
				{
					//要求 从左到右 实际为右或者在线上
					if (nPlace2 == -1)
					{
						return false;
					}

					break;
				}
				case 1:
				{
					if (nPlace2 == 1)
					{
						return false;
					}

					break;
				}
				case 2:
				{
					break;
				}
				}
				//从任意一条线段越过 即认为报警
				if (nPlace1 != nPlace2)
					return true;
			}

			return false;
		}
	}
	case 2:
	{
		//规则设置不正确
		if (nDirection == -1 && nPlace == -1)
		{
			SET_ERRTYPE;
			return false;
		}
		if (nPlace != -1)
		{
			if (m_struCrossFenceInf.nDetectRegionPoint > 1)
			{
				pointS = ZhcvMath::GetPerpendicularPointWithDistance(m_struCrossFenceInf.stuDetectRegion[0].x, m_struCrossFenceInf.stuDetectRegion[0].y, m_struCrossFenceInf.stuDetectRegion[1].x,
					m_struCrossFenceInf.stuDetectRegion[1].y, 0.03, nPlace);
				detectInfo.currentAlert.reftx = pointS.x;
				detectInfo.currentAlert.refty = pointS.y;
			}

			//判断位置1个点
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
			pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
			//在多边形内部  nPlace 0:外面; 1:里面 2:在线上;
			return ZhcvMath::IsPtInPoly(polygon, pointN) == nPlace;// detectInfo.place;
		}
		else
		{
			//判断方向 两个点
			int nPlace1 = -2;
			int nPlace2 = -2;
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			//第一个点
			pointL.x = detectInfo.lastAlert.fx + detectInfo.lastAlert.fw / 2.0f;
			pointL.y = detectInfo.lastAlert.fy + detectInfo.lastAlert.fh / 2.0f;
			nPlace1 = ZhcvMath::IsPtInPoly(polygon, pointL);

			switch (nDirection) //detectInfo.direct)
			{
				//0:由外到里;1:由里到外 2:两者都可以
			case 0:
			{
				if (nPlace1 == 1)
					return false;

				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == -1)
					return false;

				return true;
			}
			case 1:
			{
				if (nPlace1 == -1)
					return false;
				//if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
				//{
				//	return false;
				//}
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == 1)
					return false;

				return true;
			}
			case 2:
			{
				//第二个点 减少计算量
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);

				if (nPlace2 != nPlace1)
					return true;
				else
					return false;
			}
			default:
				SET_ERRPARAM;
				return false;
			}
		}
	}
	default:
	{
		SET_ERRTYPE;
		return false;
	}
	}
}

bool ZhSmartAlarm::TrafficCheck(Alarm_RuleType nRuleType, std::vector<TRAFFICALARM_INFO> &alarmInfos)
{
	if (alarmInfos.size() == 0)
	{
		m_nErrorCode = -1;
		return false;
	}

	//使用第一个元素的配置内容做判断
	TRAFFICALARM_INFO & alarmInfo = alarmInfos[0];
	
	if (alarmInfo.blUseRule == true)
	{
		//时间
		struct tm *when1 = localtime((time_t*)&alarmInfo.currentAlert.timestamp);

		for (int it = 0; it < m_struTrafficInf.nTimeSection; it++)
		{
			switch (m_struTrafficInf.guardTimes[it].guardDateTimeType)
			{
			case 1:
			case 2:
			{
				std::vector<TimeDaySegment> vttd;

				if (MakeTimeDaySegment(vttd, m_struTrafficInf.guardTimes[it].stuDayTimeSection) > 0)
				{
					int ip = InTimeDaySeg(vttd, alarmInfo.currentAlert.timestamp);
					if (ip < 0)
					{
						m_nErrorCode = -2;
						return false;
					}
				}
				break;
			}
			case 3:
			{
				std::vector<TimeWeekSegment> vttw;
				if (MakeTimeWeekSegment(vttw, m_struTrafficInf.guardTimes[it].stuDayTimeSection) > 0)
				{
					int ip = InTimeWeekSeg(vttw, alarmInfo.currentAlert.timestamp);
					if (ip < 0)
					{
						m_nErrorCode = -3;
						return false;
					}
				}
				break;
			}
			case 0:
			default:
				break;
			}
		}
	}

	/*
	ZH_ALARM_PARKINGRESTRICTEDSTOP,
	ZH_ALARM_VIOLATIONTRAFFICLINE,
	ZH_ALARM_EXCESSIVESPEED,
	ZH_ALARM_WRONGSIDE,
	ZH_ALARM_FIREALARM,
	*/
	SET_NOERROR;
	switch (nRuleType)
	{
	case Alarm_RuleType::ZH_ALARM_PARKINGRESTRICTEDSTOP:  //违停
	case Alarm_RuleType::ZH_ALARM_VIOLATIONTRAFFICLINE:  //轧线
	{ 
		//2. 位置判断规则
		if (CheckTrafficPlace(nRuleType, alarmInfo))
			return true;
		else
			return false;
		break;
	}
	case Alarm_RuleType::ZH_ALARM_EXCESSIVESPEED:  //超速
	case Alarm_RuleType::ZH_ALARM_WRONGSIDE:  //逆行
	{   
		//2. 规则 速度 带单位时间
		if (CheckTrafficSpeed(nRuleType, alarmInfos))
			return true;
		else
			return false;

		break;
	}

	default:
		SET_ERRTYPE;
		return false;
	}

	return false;
}

void ZhSmartAlarm::SetSpeedMap(std::map<double, double> &mapTableX, std::map<double, double> &mapTableY)
{
	m_mapTableX = mapTableX;
	m_mapTableY = mapTableY;
}
