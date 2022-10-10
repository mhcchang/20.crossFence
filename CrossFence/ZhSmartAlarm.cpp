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

		//�ȿ�ʼʱ����  �Ƚ���ʱ���� 
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
			//tm������´�0��ʼ
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

		//�ȿ�ʼʱ����  �Ƚ���ʱ���� ��ʼʱ�����
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
			//tm������´�0��ʼ
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
	//���������ж�
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
		//ֻ�жϷ���
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

			//�ж�λ��   //nPlace ���� 0: �ұ� 1: ���  2:������; -1���ж�
			//�����߶���β����  ����
			
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				pointN.x = detectInfo.currentAlert.refx;
				pointN.y = detectInfo.currentAlert.refy;

				//��һ���߶β����� ����Ϊ������ //nPlace���� 0: �ұ� 1: ���  2:������; -1���ж�; 
				//LeftOfLine_Area��tmpx>p.x��ʱ��˵�������ߵ����1��С�����ұ�-1��������������0��
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

				//�жϾ��� 
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

			//�����߶���β����
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				//��һ����
				pointN.x = detectInfo.lastAlert.fx;// + detectInfo.lastAlert.fw / 2.0f;
				pointN.y = detectInfo.lastAlert.fy;// + detectInfo.lastAlert.fh / 2.0f;
				//��һ���߶β����� ����Ϊ������
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection)
				{
				case 0:
				{
					//Ҫ�� ������ ʵ��Ϊ�һ���������
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

				//�ڶ�����
				pointN.x = detectInfo.currentAlert.refx;// + detectInfo.currentAlert.fw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy;// + detectInfo.currentAlert.fh / 2.0f;
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection) 
				{
				case 0:
				{
					//Ҫ�� ������ ʵ��Ϊ�һ���������
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
				//������һ���߶�Խ�� ����Ϊ����
				if (nPlace1 != nPlace2)
					return true;
			}

			return false;
		}
	}
	case 2:
	{
		//�������ò���ȷ
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

			//�ж�λ��1����
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
			pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
			//�ڶ�����ڲ�  nPlace 0:����; 1:���� 2:������;

			return ZhcvMath::IsPtInPoly(polygon, pointN) == nPlace;// detectInfo.place;
		}
		else
		{
			//�жϷ��� ������
			int nPlace1 = -2;
			int nPlace2 = -2;
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			//��һ����
			pointL.x = detectInfo.lastAlert.fx + detectInfo.lastAlert.fw / 2.0f;
			pointL.y = detectInfo.lastAlert.fy + detectInfo.lastAlert.fh / 2.0f;
			nPlace1 = ZhcvMath::IsPtInPoly(polygon, pointL);

			switch (nDirection) //detectInfo.direct)
			{
				//0:���⵽��;1:���ﵽ�� 2:���߶�����
			case 0:
			{
				if (nPlace1 == 1)
					return false;
				if (nPlace1 == 0 && detectInfo.blBoundOrVertex == false)
				{
					return false;
				}
				//�ڶ����� ���ټ�����
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
				//�ڶ����� ���ټ�����
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
				//�ڶ����� ���ټ�����
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
		//�ж�Խ�� ������
		switch (ruleType)
		{
		case 1:
		{ //����
			//bool bl = false;

			int nPlace1 = -2;
			int nPlace2 = -2;
		  //�����߶���β����
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				//��һ����
				pointN.x = detectInfo.lastAlert.x;
				pointN.y = detectInfo.lastAlert.y;
				//��һ���߶β����� ����Ϊ������
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);
				switch (m_struCrosslineInf.nDirection) //detectInfo.direct)
				{
					case 0:
					{
						//Ҫ�� ������ ʵ��Ϊ�һ���������
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

				//�ڶ�����
				pointN.x = detectInfo.currentAlert.x;
				pointN.y = detectInfo.currentAlert.y;
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);
				switch (m_struCrosslineInf.nDirection) //switch (detectInfo.direct)
				{
					case 0:
					{
						//Ҫ�� ������ ʵ��Ϊ�һ���������
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
				//������һ���߶�Խ�� ����Ϊ����
				if (nPlace1 != nPlace2)
					return true;
			}
			
			return false;
		}

		case 2:
		{ //�ܽ�
			int nPlace1 = -2;
			int nPlace2 = -2;
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			//��һ����
			pointL.x = detectInfo.lastAlert.x;
			pointL.y = detectInfo.lastAlert.y;
			nPlace1 = ZhcvMath::IsPtInPoly(polygon, pointL);

			switch (m_struCrossFenceInf.nDirection) //detectInfo.direct)
			{
				//0:���⵽��;1:���ﵽ�� 2:���߶�����
			case 0:
			{
				if (nPlace1 == 1)
					return false;
				if (nPlace1 == 0 && detectInfo.boundOrVertex == false)
				{
					return false;
				}
				//�ڶ����� ���ټ�����
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
				//�ڶ����� ���ټ�����
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
				//�ڶ����� ���ټ�����
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
		//{ //����0:������; 1:���ҵ��� 2 : ����   �ܽ�0:���⵽��;1:���ﵽ�� 2:���߶�����
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
	//��ʼʱ��Ϊ �����Ĳ���Ч
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
	{ //����
		//if (m_struCrosslineInf.nPlace != -1) //alarmInfo.place != -1)
		if (alarmInfo.blUseRule == true)
		{
			//1. �ж�ʱ��
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
					//���ж�ʱ��
					break;
				}
				}
			}
		}
		//2. �жϹ���
		if (CheckPlace(ruleType, alarmInfo))
			return 1;
		else
			return 0;
		break;
	}
	case 2:
	{   //�ܽ�
	    //1. ʱ��
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
	   //2. ����
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
* @brief 2.3 ��Ⱥ�ܶȱ����ж�
* @param
*	alarmInfo ���������Ϣ
* @return ����ֵ
*	�ɹ����� 1 ���ϱ������� 0�������޾���
*	ʧ�ܷ��ظ�ֵ
*/
int ZhSmartAlarm::DensityCheck(DENSITYALARM_INFO &alarmInfo)
{
	SET_NOERROR;
	if (alarmInfo.vStatistical.size() == 0)
		return 0;
	//������Ч
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

	//2. ������� 
	//2.1 ͳ�������ڵ�ֵ
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

	//û������ֵ ������ byUnit==0 �ܶȵȼ� gbt
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
* @brief 2.4 ��Ⱥ�ܶ�ͳ�� 
* @param
*	alarmInfo ���������Ϣ
* @return ����ֵ
*	�ɹ����� �������������ڵ��� densityThreshold���ֵ �������� Ϊ��ǰ�������ܽ��ھ����Ŀ�����
*	ʧ�ܷ��ظ�ֵ
*/
int ZhSmartAlarm::DensityStatistical(DENSITYALARM_INFO &alarmInfo, bool blUseCfg, std::vector<Point2D> vp)
{
	SET_NOERROR;
	if (alarmInfo.vStatistical.size() == 0)
		return 0;

	//for (auto k : alarmInfo.vStatistical)
	//{
		//Ŀǰֻ���� ��Ⱥ �������ݲ�����
		//if (k.first == "person")
		//{
			// ͳ�������ڵ�ֵ
			float ftotal = 0.0f;
			//ʹ������ͳ��
			if (blUseCfg)
			{
				//��һ�� �����������
				for (int ii = 0; ii < m_struDensityInf.nDetectRegionPoint; ii++)
				{
					for (int ic = 0; ic < alarmInfo.nWidth - 1; ic++)
					{
						for (int ir = 0; ir < alarmInfo.nHeight - 1; ir++)
						{
							//�н���ľͼ���
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

							//�н���ľͼ���
							if (ZhcvMath::CSClip_Line(1.0f / alarmInfo.nWidth * (ic + 1), 1.0f / alarmInfo.nHeight * (ir + 1),
								1.0f / alarmInfo.nWidth * ic, 1.0f / alarmInfo.nHeight * ir, false,
								m_struDensityInf.stuDetectRegion[ii].x, m_struDensityInf.stuDetectRegion[ii].y,
								m_struDensityInf.stuDetectRegion[ii + 1].x, m_struDensityInf.stuDetectRegion[ii + 1].y) > 0)
							{
								//������
								ftotal += alarmInfo.vStatistical[ir * alarmInfo.nWidth + ic];
							}
						}
					}
				}
			}
			else
			{
				//ʹ���ⲿ����ͳ��
				//��һ�� �����������
				for (int ii = 0; ii < vp.size() - 1; ii++)
				{
					for (int ic = 0; ic < alarmInfo.nWidth - 1; ic++)
					{
						for (int ir = 0; ir < alarmInfo.nHeight - 1; ir++)
						{
							//�н���ľͼ���
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
							//���ߴ�������
							if (ZhcvMath::CSClip_Line(1.0f / alarmInfo.nWidth * (ic + 1), 1.0f / alarmInfo.nHeight * (ir + 1),
								1.0f / alarmInfo.nWidth * ic, 1.0f / alarmInfo.nHeight * ir, false,
								x1, y1, x2, y2) > 0)
							{
								//������
								ftotal += alarmInfo.vStatistical[ir * alarmInfo.nWidth + ic];
							}
						}
					}
				}
			}

			////total �����key���������ݱ����statistical ����ĸ���������һ��
			//std::map<std::string, int>::iterator it = alarmInfo.total.find(k.first);
			//if (it == alarmInfo.total.end())
			//{
			//	//������ƥ��
			//	SET_ERRPARAM;
			//	return -1;
			//}
			alarmInfo.fAlarmTotal = ftotal;
			//����������Ŀ����� �ٷ��� * ����
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

	//ʹ�õ�һ��Ԫ�ص������������ж�
	TRAFFICALARM_INFO & alarmInfo = detectInfo[0];

	if (alarmInfo.blUseRule == true)
	{
		//ʱ��
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
	//��һ��Ԫ�صõ�����ƽ��ֵ ������Ķ���ÿ��Ԫ�ض�Ӧ��һ��Ԫ�ص�˲ʱֵ ��ƽ��ֵ> ����ֵʱ������
	alarmInfo.fSpeed = speed_avg;

	//1.1 ϵ���ǹ��Ҽ����ֹ涨��������Χ
	if (Alarm_RuleType::ZH_ALARM_EXCESSIVESPEED == nRuleType && alarmInfo.fSpeed > alarmInfo.fSpeedLimit *(1.1))
	{ //����
		return true;
	}
	else if (Alarm_RuleType::ZH_ALARM_WRONGSIDE && alarmInfo.fSpeed < 5.0f)
	{
		//���� ʱ��5�������ϵ� �ж�Ϊ ������������
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
		//ʱ��
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

	//ֻ�жϷ���
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

			//�ж�λ��   //nPlace ���� 0: �ұ� 1: ���  2:������; -1���ж�
			//�����߶���β����  ����
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				pointN.x = detectInfo.currentAlert.refx;
				pointN.y = detectInfo.currentAlert.refy;

				//��һ���߶β����� ����Ϊ������ nPlace���� 0: �ұ� 1: ���  2:������; -1���ж�; 
				//LeftOfLine_Area��tmpx>p.x��ʱ��˵�������ߵ����1��С�����ұ�-1��������������0��
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

				//�жϾ��� 
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

			//�����߶���β����
			for (int i = 0; i < m_struCrosslineInf.nDetectLinePoint - 1; i++)
			{
				pointS = m_struCrosslineInf.stuDetectLine[i];
				pointE = m_struCrosslineInf.stuDetectLine[i + 1];

				//��һ����
				pointN.x = detectInfo.lastAlert.fx;
				pointN.y = detectInfo.lastAlert.fy;
				//��һ���߶β����� ����Ϊ������
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection)
				{
				case 0:
				{
					//Ҫ�� ������ ʵ��Ϊ�һ���������
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

				//�ڶ�����
				pointN.x = detectInfo.currentAlert.refx;
				pointN.y = detectInfo.currentAlert.refy;
				nPlace1 = ZhcvMath::LeftOfLine_Area(pointN, pointS, pointE);

				switch (nDirection)
				{
				case 0:
				{
					//Ҫ�� ������ ʵ��Ϊ�һ���������
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
				//������һ���߶�Խ�� ����Ϊ����
				if (nPlace1 != nPlace2)
					return true;
			}

			return false;
		}
	}
	case 2:
	{
		//�������ò���ȷ
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

			//�ж�λ��1����
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
			pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
			//�ڶ�����ڲ�  nPlace 0:����; 1:���� 2:������;
			return ZhcvMath::IsPtInPoly(polygon, pointN) == nPlace;// detectInfo.place;
		}
		else
		{
			//�жϷ��� ������
			int nPlace1 = -2;
			int nPlace2 = -2;
			std::vector<Point2D> polygon;
			for (int i = 0; i < m_struCrossFenceInf.nDetectRegionPoint; i++)
			{
				pointS = m_struCrossFenceInf.stuDetectRegion[i];
				polygon.push_back(pointS);
			}
			//��һ����
			pointL.x = detectInfo.lastAlert.fx + detectInfo.lastAlert.fw / 2.0f;
			pointL.y = detectInfo.lastAlert.fy + detectInfo.lastAlert.fh / 2.0f;
			nPlace1 = ZhcvMath::IsPtInPoly(polygon, pointL);

			switch (nDirection) //detectInfo.direct)
			{
				//0:���⵽��;1:���ﵽ�� 2:���߶�����
			case 0:
			{
				if (nPlace1 == 1)
					return false;

				//�ڶ����� ���ټ�����
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
				//�ڶ����� ���ټ�����
				pointN.x = detectInfo.currentAlert.refx + detectInfo.currentAlert.refw / 2.0f;
				pointN.y = detectInfo.currentAlert.refy + detectInfo.currentAlert.refh / 2.0f;
				nPlace2 = ZhcvMath::IsPtInPoly(polygon, pointN);
				if (nPlace2 == 1)
					return false;

				return true;
			}
			case 2:
			{
				//�ڶ����� ���ټ�����
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

	//ʹ�õ�һ��Ԫ�ص������������ж�
	TRAFFICALARM_INFO & alarmInfo = alarmInfos[0];
	
	if (alarmInfo.blUseRule == true)
	{
		//ʱ��
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
	case Alarm_RuleType::ZH_ALARM_PARKINGRESTRICTEDSTOP:  //Υͣ
	case Alarm_RuleType::ZH_ALARM_VIOLATIONTRAFFICLINE:  //����
	{ 
		//2. λ���жϹ���
		if (CheckTrafficPlace(nRuleType, alarmInfo))
			return true;
		else
			return false;
		break;
	}
	case Alarm_RuleType::ZH_ALARM_EXCESSIVESPEED:  //����
	case Alarm_RuleType::ZH_ALARM_WRONGSIDE:  //����
	{   
		//2. ���� �ٶ� ����λʱ��
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
