#pragma once

#include "ZhcvSmartDefs.h"

#if __WINDOWS_OS__
#ifdef ZHSMARTALARM_EXPORTS
#pragma warning (push)
#pragma warning (disable: 4251)
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

//#define MAX_NAME_LEN                128             // ͨ�������ַ�������
//#define MAX_OBJECT_LIST_SIZE        256             // ��Ƶ�����豸֧�ֵļ�����������б��������
//#define MAX_POLYLINE_NUM            20              // ��Ƶ�����豸���߶����������
//#define MAX_DAY_NUM                 31              // һ�µ�����
//#define MAX_WEEK_DAY_NUM			  7	    		  // ÿ�ܵ�����
//#define MAX_MONTH_NUM               12              // һ��12����
//#define MAX_REC_TSECT_EX            24              // ¼��ʱ�����չ����

//#define MAX_POLYGON_NUM             256             // ��Ƶ�����豸���򶥵��������
//#define MAX_CHANNELNAME_LEN         64              // ���ͨ�����Ƴ���
//#define MAX_CHANNEL_COUNT           36
//#define MAX_VIDEO_CHANNEL_NUM       256             // ���ͨ����256
//#define MAX_DBKEY_NUM               64              // ���ݿ�ؼ������ֵ
//#define MAX_SUMMARY_LEN             1024            // ���ӵ�JPEGͼƬ��ժҪ��Ϣ��󳤶�
//#define MAX_CALIBRATEBOX_NUM        10              // ���ܷ���У׼���������

//#define MAX_PATH					  255

#include <string>
#include <vector>
#include <functional>
#include <map>

class ZhcvMath;

/**
* @brief ����������
* @detail ��ʵ�ְ��� �ܽ� ��Ⱥ�ܶ�  ���ڿ��ܼ����������smart����
*/
class DLL_API ZhSmartAlarm
{
public:
	//�ֶ�ʱ��
	struct TimeDaySegment {
		//struct tmSeg {
		//	int tm_min;   // minutes after the hour - [0, 59]
		//	int tm_hour;  // hours since midnight - [0, 23]
		//	int tm_mday;  // day of the month - [1, 31]
		//	int tm_mon;   // months since January - [0, 11]
		//	int tm_wday;  // days since Sunday - [0, 6]
		//};
		int start_tm_min;
		int start_tm_hour;
		int end_tm_min;
		int end_tm_hour;
		std::vector<int> days;  //1-31
		std::vector<int> mons;  //1-12
	};
	struct TimeWeekSegment {
		//struct tmSeg {
		//	int tm_min;   // minutes after the hour - [0, 59]
		//	int tm_hour;  // hours since midnight - [0, 23]
		//	int tm_mday;  // day of the month - [1, 31]
		//	int tm_mon;   // months since January - [0, 11]
		//	int tm_wday;  // days since Sunday - [0, 6]
		//};
		int start_tm_min;
		int start_tm_hour;
		int end_tm_min;
		int end_tm_hour;
		std::vector<int> weeks;  //1~7
		std::vector<int> mons;   //1-12
	};

public:
	ZhSmartAlarm();
	~ZhSmartAlarm();

	/**
	* @brief 2.1 ���ñ�������
	*	detail �������Ϊ void*�ɸ��ݱ������ͣ����벻ͬ�ı��������ṹ��**
	* @param �������
		ruleType : �������� 1 ���ߣ� 2 �ܽ磬 3 ��Ⱥ�ܶ�
		lpAlarmSetupParam : ���������ṹ��
		��ruleType = 1ʱ ΪCFG_CROSSLINE_INFOָ��
		��ruleType = 2ʱΪCFG_CROSSFENCEDETECTION_INFOָ��
		��ruleType = 3ʱ ΪCFG_DENSITYDETECTION_INFOָ��
	* @return ���óɹ�����true ���򷵻�false
	**/
	bool SetupPara(int ruleType, void* lpAlarmSetupParam);

	/**
	* @brief 2.2 �ܽ���߱����ж�
	* @param
	*	int ruleType �������  1 ���ߣ� 2 �ܽ�
	*	alarmInfo ���������Ϣ
	* @return ����ֵ
	*	�ɹ����� 0 ���ϱ������� 1 �������޾��� ���������� 2����ʱ�䷶Χ �� 3����ʱ�䷶Χ ����
	*	ʧ�ܷ��ظ�ֵ
	*/
	int DetectCheck(int ruleType, DETECTALARM_INFO &alarmInfo);

	/**
	* @brief 2.3 ��Ⱥ�ܶȱ����ж�
	* @param
	*	alarmInfo ���������Ϣ
	* @return ����ֵ
	*	�ɹ����� 1 ���ϱ������� 0�������޾���
	*	ʧ�ܷ��ظ�ֵ
	*/
	int DensityCheck(DENSITYALARM_INFO &alarmInfo);

	/**
	* @brief 2.4 ��Ⱥ�ܶ�ͳ��
	* @param
	*	alarmInfo ���������Ϣ
	*   blUseCfg ʹ�������ж� ����Ϊfalseʱʹ��vp������ܽ��ж� ����ʹ�����õĹ���
	*   vp �ܽ綥������
	* @return ����ֵ
	*	�ɹ����� �������������ڵ��� densityThreshold���ֵ �������� Ϊ��ǰ�������ܽ��ھ����Ŀ�����
	*	ʧ�ܷ��ظ�ֵ
	*/
	int DensityStatistical(DENSITYALARM_INFO &alarmInfo, bool blUseCfg, std::vector<Point2D> vp);
	
	/**
	* @brief 2.6 ���þ�����׼�� xy����
	* @param
	*	mapTableX x��������꣨��һ�� ��ʵ�ʾ������׼��ϵ
	*   mapTableY y��������꣨��һ�� ��ʵ�ʾ������׼��ϵ
	* @return ����ֵ ��
	*/
	void SetSpeedMap(std::map<double, double> &mapTableX, std::map<double, double> &mapTableY);
	/**
	* @brief 2.6 ��ͨΥ�����
	* @param
	*	nRuleType ������� 
	*   alarmInfo ��ͨΥ����Ϣ
	* @return ����ֵ
	*	�ɹ����� 1 ���ϱ������� 0�������޾���
	*	ʧ�ܷ��ظ�ֵ
	*/
	
	bool TrafficCheck(Alarm_RuleType nRuleType, std::vector<TRAFFICALARM_INFO> &alarmInfos);
	bool CheckTrafficPlace(Alarm_RuleType nRuleType, TRAFFICALARM_INFO &detectInfo);
	bool CheckTrafficSpeed(Alarm_RuleType nRuleType, std::vector<TRAFFICALARM_INFO> &detectInfo);

	/**
	* @brief 2.11 ��ȡ��󱨴���Ϣid
	*/
	int GetErrorCode() { return m_nErrorCode; };
#ifndef DEBUG
protected:
#endif // DEBUG

	/**
	* @brief 2.5 ���ñ����ص�
	* @param �������
	*	std::function<int(int, void**)> callback �ص�����
	*	�ص���������
	*/
	void SetAlarmDetectCallback(std::function<int(int, std::vector<DETECTALARM_INFO> &)> callback);
	void SetAlarmDensityCallback(std::function<int(int, std::vector<DENSITYALARM_INFO> &)> callback);

	/**
	* @brief 2.7 ���ñ�������Դ
	* @param �������
	*	std::function<int(int, void**)> callback �ص�����
	*	�ص���������
	*/
	int SetDetectDataSource(int ruleType, std::vector<DETECTALARM_INFO> &alarms);
	int SetDensityDataSource(std::vector<DENSITYALARM_INFO> &alarms);
	/**
	* @brief 2.9 ��ȡ�����ܽ籨����Ϣ
	* û�����ûص�������£�sdk�ڲ��Զ�����300��������Ϣ��ÿ������100�� �����Ҫ�������͵���Ϣ��Ҫʹ�ò�ͬ��ruleType��������**
	* @param �������
	*	ruleType : �������� 1 ���ߣ� 2 �ܽ�
	*	alerms : ������Ϣ����
	* @return ����ֵ
	*	���ر�����Ϣ���� �� С��0��ʾ�����д���
	*/
	int GetDetectAlarmInfo(int ruleType, std::vector<DETECTALARM_INFO> &alarms);

	/**
	* @brief 2.10 ��ȡ���壨��Ⱥ���ܶȱ�����Ϣ
	*  detail û�����ûص�������£�sdk�ڲ��Զ�����300��������Ϣ��ÿ������100�� ��������ȡ��**
	* @param �������
	*   ruleType : ��������== 3 ���壨��Ⱥ���ܶ� ֻ��Ϊ3
	*alerms : ������Ϣ����
	* @return ����ֵ
	*	���ر�����Ϣ���� �� С��0��ʾ�����д���
	*/
	int GetDetectAlarmInfo(int ruleType, std::vector<DENSITYALARM_INFO> &alarms);

private:

	std::function<int(int, std::vector<DETECTALARM_INFO> &)> m_pDecectCall;
	std::function<int(int, std::vector<DENSITYALARM_INFO> &)> m_pDensityCall;

	//�ж��Ƿ���ʱ�����
	int InTimeDaySeg(std::vector<TimeDaySegment> vtm, time_t tmWhen);
	int InTimeWeekSeg(std::vector<TimeWeekSegment> vtm, time_t tmWhen);
	//��detectInfo �����direct ��place���ж� һ�µ�Ϊtrue ����Ϊfalse 
	//blAtLine ��ʾ�ڱ߻򶨵����Ƿ���Ϊ
	bool CheckPlace(int ruleType, DETECTALARM_INFO &detectInfo);

	void SetErrorCode(int nErr) { m_nErrorCode = nErr; };
private:
	CFG_CROSSLINE_INFO m_struCrosslineInf;
	CFG_CROSSFENCEDETECTION_INFO m_struCrossFenceInf;
	CFG_TRAFFICVIOLATIONS_INFO m_struTrafficInf;
	CFG_DENSITYDETECTION_INFO m_struDensityInf;

	int m_nErrorCode;

	std::map<double, double> m_mapTableX;
	std::map<double, double> m_mapTableY;

};
