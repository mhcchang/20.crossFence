#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#ifdef __WINDOWS_OS__
#undef __WINDOWS_OS__
#endif
#define __WINDOWS_OS__ 1
#endif

#define MAX_NAME_LEN                            128             // ͨ�������ַ�������
#define MAX_OBJECT_LIST_SIZE                    256             // ��Ƶ�����豸֧�ֵļ�����������б��������
#define MAX_POLYLINE_NUM                        20              // ��Ƶ�����豸���߶����������
#define MAX_DAY_NUM                             31              // һ�µ�����
#define MAX_WEEK_DAY_NUM						7				// ÿ�ܵ�����
#define MAX_MONTH_NUM                           12              // һ��12����
#define MAX_REC_TSECT_EX                        24              // ¼��ʱ�����չ���� ��24Сʱ����

#define MAX_POLYGON_NUM                         256             // ��Ƶ�����豸���򶥵��������
#define MAX_CHANNELNAME_LEN                     64              // ���ͨ�����Ƴ���
#define MAX_CHANNEL_COUNT                       36
#define MAX_VIDEO_CHANNEL_NUM                   256             // ���ͨ����256
#define MAX_DBKEY_NUM                           64              // ���ݿ�ؼ������ֵ
#define MAX_SUMMARY_LEN                         1024            // ���ӵ�JPEGͼƬ��ժҪ��Ϣ��󳤶�
#define MAX_CALIBRATEBOX_NUM                    10              // ���ܷ���У׼���������

#define MAX_GUARDTIME_NUM		MAX_REC_TSECT_EX
//windows
#ifndef MAX_PATH
#define MAX_PATH								260
#endif // !

//double �����ж�
#ifndef EPSILON
#define EPSILON   (1e-10)
#endif 
//float
#ifndef EPSILON_FLOAT
#define EPSILON_FLOAT   (1e-6)
#endif 

#include <map>
#include <vector>

typedef struct stru_Point2D
{
	double x;
	double y;

	stru_Point2D() : x(0.0), y(0.0) {};
	stru_Point2D(double _x, double _y) : x(_x), y(_y) {};
} Point2D, CFG_POINT2D;

typedef struct stru_Point3D
{
	double x;
	double y;
	double z;
	stru_Point3D() : x(0.0), y(0.0), z(0.0) {};
	stru_Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

} Point3D, CFG_POINT3D;

enum ALARM_ERROR_CODE {
	ERROR_NONE = 0,           //û�д���
	ERROR_PARAMS,             //�����������
	ERROR_TYPE,               //�������ʹ���
	ERROR_VALIDTIME,          //����ʱ����Ч
	ERROR_NOIMPL = 0xFF       //����û��ʵ��
};

// Size
typedef struct tagCFG_SIZE
{
	union
	{
		float	fWidth;			// ��
		float	fArea;			// ���
	};
	float		fHeight;		// ��
} CFG_SIZE;

// ������Ϣ
typedef struct tagCFG_REGION
{
	int         nPointNum;
	CFG_POINT2D stuPolygon[MAX_POLYGON_NUM];
}CFG_REGION;

// RGBA��Ϣ
typedef struct tagCFG_RGBA
{
	int	nRed;
	int	nGreen;
	int	nBlue;
	int	nAlpha;
} CFG_RGBA;

// ʱ�����Ϣ
typedef struct tagCFG_TIME_SECTION
{
	int	nBeginHour;
	int	nBeginMin;
	int	nBeginSec;
	int	nEndHour;
	int	nEndMin;
	int	nEndSec;

	tagCFG_TIME_SECTION() : nBeginHour(-1), nBeginMin(-1), nBeginSec(0), nEndHour(0), nEndMin(0), nEndSec(0) {};
} CFG_TIME_SECTION;

// У׼����Ϣ
typedef struct tagCFG_CALIBRATEBOX_INFO
{
	CFG_POINT2D stuCenterPoint;     // У׼�����ĵ�����(��������һ����[0,8191]����)
	float       fRatio;             // ��Ի�׼У׼��ı���(����1��ʾ��׼���С��0.5��ʾ��׼���С��һ��)
} CFG_CALIBRATEBOX_INFO;

// �ߴ������
typedef struct tagCFG_SIZEFILTER_INFO
{
	int                   nCalibrateBoxNum;                           // У׼�����
	CFG_CALIBRATEBOX_INFO stuCalibrateBoxs[MAX_CALIBRATEBOX_NUM];     // У׼��(Զ�˽��˱궨ģʽ����Ч)
	bool                  bMeasureModeEnable;                         // ������ʽ�����Ƿ���Ч
	unsigned char         bMeasureMode;                               // ������ʽ,0-���أ�����ҪԶ�ˡ����˱궨, 1-ʵ�ʳ��ȣ���λ����, 2-Զ�˽��˱궨�������
	bool                  bFilterTypeEnable;                          // �������Ͳ����Ƿ���Ч
																	  // ByArea,ByRatio�������ݣ�ʹ�ö�����ByArea��ByRatioѡ����� 2012/03/06
	unsigned char		  bFilterType;			 				      // ��������:0:"ByLength",1:"ByArea", 2"ByWidthHeight"
	unsigned char         bReserved[2];                               // �����ֶ�
	bool                  bFilterMinSizeEnable;                       // ������С�ߴ�����Ƿ���Ч
	bool                  bFilterMaxSizeEnable;                       // �������ߴ�����Ƿ���Ч
	CFG_SIZE			  stuFilterMinSize;						      // ������С�ߴ� "ByLength"ģʽ�±�ʾ��ߵĳߴ磬"ByArea"ģʽ�¿��ʾ���������Ч(Զ�˽��˱궨ģʽ�±�ʾ��׼��Ŀ�߳ߴ�)��
	CFG_SIZE			  stuFilterMaxSize;				              // �������ߴ� "ByLength"ģʽ�±�ʾ��ߵĳߴ磬"ByArea"ģʽ�¿��ʾ���������Ч(Զ�˽��˱궨ģʽ�±�ʾ��׼��Ŀ�߳ߴ�)��

	bool                  abByArea;
	bool                  abMinArea;
	bool                  abMaxArea;
	bool                  abMinAreaSize;
	bool                  abMaxAreaSize;
	bool                  bByArea;                                    // �Ƿ�������� ͨ������ComplexSizeFilter�ж��Ƿ����
	bool                  bReserved1[2];
	float                 nMinArea;                                   // ��С���
	float                 nMaxArea;                                   // ������
	CFG_SIZE              stuMinAreaSize;                             // ��С������ο�ߴ� "������ʽ"Ϊ"����"ʱ����ʾ��С������ο�Ŀ�߳ߴ磻"������ʽ"Ϊ"Զ�˽��˱궨ģʽ"ʱ����ʾ��׼�����С��߳ߴ磻
	CFG_SIZE              stuMaxAreaSize;                             // ���������ο�ߴ�, ͬ��

	bool                  abByRatio;
	bool                  abMinRatio;
	bool                  abMaxRatio;
	bool                  abMinRatioSize;
	bool                  abMaxRatioSize;
	bool                  bByRatio;                                   // �Ƿ񰴿�߱ȹ��� ͨ������ComplexSizeFilter�ж��Ƿ����
	bool                  bReserved2[2];
	double                dMinRatio;                                  // ��С��߱�
	double                dMaxRatio;                                  // ����߱�
	CFG_SIZE              stuMinRatioSize;                            // ��С��߱Ⱦ��ο�ߴ磬��С��߱ȶ�Ӧ���ο�Ŀ�߳ߴ硣
	CFG_SIZE              stuMaxRatioSize;                            // ����߱Ⱦ��ο�ߴ磬ͬ��

	int                   nAreaCalibrateBoxNum;                       // ���У׼�����
	CFG_CALIBRATEBOX_INFO stuAreaCalibrateBoxs[MAX_CALIBRATEBOX_NUM]; // ���У׼��
	int                   nRatioCalibrateBoxs;                        // ���У׼�����
	CFG_CALIBRATEBOX_INFO stuRatioCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// ���У׼��

	bool                  blBySizeEnable;                             // �������ʹ�ܲ����Ƿ���Ч
	bool                  blBySize;                                   // �������ʹ��
}CFG_SIZEFILTER_INFO;

//CrossLine ���� CrossRegion �ܽ� Density �����ܶ�  [v1.3] ParkingRestrictedStop��ͣ����ͣ�� ViolationTrafficLine ������ʻ ExcessiveSpeed ���� WrongSide���� FireAlarm
enum Alarm_RuleType {
	ZH_ALARM_CROSSLINE = 0x00,
	ZH_ALARM_CROSSREGION,
	ZH_ALARM_DENSITY,
	ZH_ALARM_PARKINGRESTRICTEDSTOP,
	ZH_ALARM_VIOLATIONTRAFFICLINE,
	ZH_ALARM_EXCESSIVESPEED,
	ZH_ALARM_WRONGSIDE = 0x06,
	ZH_ALARM_FIREALARM = 0x10,
	ZH_ALARM_UNKNOWN = 0x99
};

//����ʱ���  
typedef struct tagCFG_TIMEGUARD_INFO
{
	int				 guardDateTimeId;				// id	   
	int				 guardDateTimeType;				// 0������ 1 �ֶ� 2������ÿ�� 3���·�ÿ�� ����ֻ����һ������ ��ʡ�ڴ�
													// 0���ж�ʱ�� 1һ���Ե� 2�ж� stuDayTimeSection 3 �ж� stuWeekTimeSection 
	CFG_TIME_SECTION stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
																								//���� CFG_TIME_SECTION stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// �¼���Ӧʱ��� ��ÿ����Ϊ����
	tagCFG_TIMEGUARD_INFO() : guardDateTimeId(-1), guardDateTimeType(0) {};
} CFG_TIMEGUARD_INFO;

struct alertDetailInfo {              //���������Ϣ�ṹ�� �����ʱ���
	int64_t timestamp;                //�������ʱ���
	float fx;                         //����������� x ���ĵ�
	float fy;                         //����������� y ���ĵ�
	float fw;
	float fh;
	float ftx;                        //�������������� x
	float fty;                        //�������������� y
									  //v1.8������
	float refx;
	float refy;
	float refw;
	float refh;
	float reftx;                        //�������������� x ȫ��
	float refty;                        //�������������� y

	float pan;
	float tilt;
	float zoom;

	float azimuth;
	float elevation;
	float latitude;
	float longitude;

	alertDetailInfo() : timestamp(0), fx(0.0f), fy(0.0f), fw(0.0f), fh(0.0f), ftx(0.0f), fty(0.0f), refx(0.0f), refy(0.0f), refw(0.0f), refh(0.0f),
		reftx(0.0f), refty(0.0f), pan(0.0f), tilt(0.0f), zoom(0.0f), azimuth(0.0f), elevation(0.0f), latitude(0.0f), longitude(0.0f) {};
	alertDetailInfo(int64_t _timestamp, float _x, float _y, float _w, float _h, float _tx, float _ty, float _refx, float _refy, float _refw, float _refh,
		float _reftx, float _refty,
		float _pan, float _tilt, float _zoom, float _azimuth, float _elevation, float _latitude, float _longitude) :
		timestamp(_timestamp), fx(_x), fy(_y), fw(_w), fh(_h), ftx(_tx), fty(_ty), refx(_refx), refy(_refy), refw(_refw), refh(_refh),
		reftx(_reftx), refty(_refty), pan(_pan), tilt(_tilt), zoom(_zoom), azimuth(_azimuth), elevation(_elevation), latitude(_latitude), longitude(_longitude) {};
};

//### 1.1 ���߹���ṹ��
typedef struct tagCFG_CROSSLINE_INFO
{
	// ��Ϣ
	int				    ruleId;
	char			    szRuleName[MAX_NAME_LEN];		  // ��������,��ͬ����������
	bool			    blRuleEnable;					  // ����ʹ��
	bool		        blTrackEnable;                    // ��������ʹ��,���԰��ߣ����ֹ�����Ч
	int				    nDirection;						  // ��ⷽ��:��ruleType����Ϊ CrossLineΪ���� ʱ ��ⷽ�� 0:��������;1:��������;2:���߶�����
														  // ��ruleType����Ϊ CrossRegionΪ�ܽ� ʱ ��ⷽ�� 0:���⵽��;1:���ﵽ�� 2:���߶�����
														  // ��ruleType����Ϊ Densityʱ ������Ч
	int				    nPlace;
	float			    fDistance;						  // �����߱�������  
	int				    nDetectLinePoint;				  // ������/���߶�����
	CFG_POINT2D		    stuDetectLine[MAX_POLYLINE_NUM];  // ������/���߶������� ԭ���ϲ���� ���ܽ��棡

	int                 nTimeSection;                     // ����ʱ������
	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	  // ����ʱ��
														  //int               nTimeSection;                     // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
														  //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
														  //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	// �¼���Ӧʱ��� ��ÿ����Ϊ����

														  //bool              bSizeFileter;                     // �����ض��ĳߴ�������Ƿ���Ч
														  //CFG_SIZEFILTER_INFO stuSizeFileter;                 // �����ض��ĳߴ������

	int                 nTriggerPosition;                 // ��������λ����
	unsigned char       byTriggerPosition[8];             // ��������λ��,0-Ŀ����ӿ�����, 1-Ŀ����ӿ��������, 2-Ŀ����ӿ򶥶�����, 3-Ŀ����ӿ��Ҷ�����, 4-Ŀ����ӿ�׶�����
	int				    nTrackDuration;				      // ���ٳ���ʱ��,0��:һֱ����,1~300��:���ٳ���ʱ��
	unsigned char       byReserved[20];                   // �����ֶ� 
	tagCFG_CROSSLINE_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0), nDirection(0), nPlace(0), fDistance(0.0f) {};
} CFG_CROSSLINE_INFO;

//### 1.2 �ܽ����ṹ��
//�¼�����EVENT_IVS_CROSSFENCEDETECTION(��ԽΧ������)��Ӧ�Ĺ�������
typedef struct tagCFG_CROSSFENCEDETECTION_INFO
{
	// ��Ϣ
	int				    ruleId;
	char			    szRuleName[MAX_NAME_LEN];				 // ��������,��ͬ����������
	bool			    blRuleEnable;							 // ����ʹ��
	bool			    blTrackEnable;                           // ��������ʹ��,���԰��ߣ����ֹ�����Ч
	int				    nDirection;							     // ��ԽΧ������:0:��������;1:��������;2:���߶�����
	int				    nPlace;
	int				    nDetectRegionPoint;					     // �ܽ綥����
	CFG_POINT2D   	    stuDetectRegion[MAX_POLYLINE_NUM];		 // �ܽ����� ��������
	int                 nTimeSection;                            // ����ʱ������
	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	         // ����ʱ��
																 //bool			    guardTimeEnabled;						 // ����ʱ��
																 //int               nTimeSection;                            // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection 3 һ���Ե� ʹ��stuDayTimeSection
																 //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
																 //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// �¼���Ӧʱ��� ��ÿ����Ϊ����

																 //bool              bSizeFileter;                            // �����ض��ĳߴ�������Ƿ���Ч
																 //CFG_SIZEFILTER_   INFO stuSizeFileter;                     // �����ض��ĳߴ������
	int                 nTriggerPosition;                        // ��������λ����
	unsigned char       byTriggerPosition[8];                    // ��������λ��,0-Ŀ����ӿ�����, 1-Ŀ����ӿ��������, 2-Ŀ����ӿ򶥶�����, 3-Ŀ����ӿ��Ҷ�����, 4-Ŀ����ӿ�׶�����
	int				    nTrackDuration;						     // ���ٳ���ʱ��,0��:һֱ����,1~300��:���ٳ���ʱ��
	unsigned char       byReserved[20];                          // �����ֶ�
	tagCFG_CROSSFENCEDETECTION_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTriggerPosition(0), nTrackDuration(0), nTimeSection(0), nDetectRegionPoint(0), nDirection(0), nPlace(0) {};
} CFG_CROSSFENCEDETECTION_INFO;

// 1.3 ��Ⱥ�ܶȼ��ṹ��
//�¼�����EVENT_IVS_DENSITYDETECTION(��Ⱥ�ܶȼ���¼�)��Ӧ�Ĺ�������
typedef struct tagCFG_DENSITYDETECTION_INFO
{
	// ��Ϣ
	int				     ruleId;
	char                 szRuleName[MAX_NAME_LEN];          // ��������,��ͬ����������
	bool                 blRuleEnable;                      // ����ʹ��
	bool		         blTrackEnable;                     // ��������ʹ��, ���԰��ߣ����ֹ�����Ч
	char                 szObjectTypes[MAX_NAME_LEN];		// ��Ӧ��������
	unsigned char        byUnit;                            // �ܶȼ�ⱨ����ʽ : byUnit==0 �ܶȵȼ� ��ֵ��λGB/T-30147������Ϊ�ĸ�����ϡ�裬ϡ�裬�ܼ������ܼ���
															//1)��ϡ��:Ŀ�긲�����ռ������������0~25%,
															//2)ϡ��:Ŀ�긲�����ռ������������26%~50 %,
															//3)�ܼ�:Ŀ�긲�����ռ������������51%~75 %,
															//4)���ܼ�:Ŀ�긲�����ռ������������76%~100%  ; nThresholdNum ����ȼ� 1��2��3��4
															//byUnit==1 �ܶȰٷֱ� nThresholdNum ��ʱ����ٷֱ� 1-100 ; byUnit==2 ��ֵ  ʹ��nThresholdNum ��ʱ������� >0

															//unsigned char      bySensitivity;                     // ������(ȡֵ1-100��ֵԽС������Խ�ͣ���Ӧ��Ⱥ���ܼ��̶�Խ��)
															//unsigned char      byMinAlarmDensity;                 // ��С�����ܶ� ȡֵ1-100,��������ֵ�ٷֱ� �������ڵ��ܶ�ֵ����ȫ������ڵİٷֱȱ��� 0ʱ��ʹ��

	int                  nThresholdNum;                     // ������ֵ nThresholdNum ����ȼ� 1��2��3��4 byUnit==1 �ܶȰٷֱ� nThresholdNum ��ʱ����ٷֱ� 1-100 ; byUnit==2 ��ֵ  ʹ��nThresholdNum ��ʱ������� >0
															//int                  nTimePercent;			        // ������λʱ���ڰٷֱ� 0~100 ������Ϊ80 ���nMinDuration=10 ��ô����10������8�� �����ﵽ������ֵ -1ʱ��ʹ�ó���ʱ�� ֻ����ʱ��������  ��������
	int                  nMinDuration;                      // ��̳���ʱ�� ��λ���룬0~65535�� 0 ÿ�ζ����� > 0 ���Ǽ��ʱ�� 

	int                  nDetectRegionPoint;                // ������򶥵���
	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // �������
	int                  nTimeSection;                      // ����ʱ������
	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // ����ʱ��

															//int                nTimeSection;                      // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
															//CFG_TIME_SECTION   stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];        // �¼���Ӧʱ��� ����
															//CFG_TIME_SECTION   stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	 // �¼���Ӧʱ��� ������ 7��Ϊ��λ

	int                  nWidth;                            // �������ݷֿ���Ϣ ������� ����3840x2160�Ļ����н�����ȷ�Ϊ16��9�У���ô��ⷵ�ص�DESITYALARM_INFO����statistical��vector��Ԫ���е�ͳ����ֵ������240*240��С��ָ���ͳ����ֵ
	int                  nHeight;                           // �������ݷֿ���Ϣ ������� ��ֵΪȱʡֵ ���������Ϊ�����С

	unsigned char        byReserved[20];                    // �����ֶ� 

	tagCFG_DENSITYDETECTION_INFO() : ruleId(-1), blRuleEnable(true), byUnit(1), nThresholdNum(50), nMinDuration(80), nWidth(-1), nHeight(-1), nTimeSection(0), nDetectRegionPoint(0) {};
} CFG_DENSITYDETECTION_INFO;

// 1.4 ��ͨΥ�¼��
//�¼�����EVENT_IVS_DENSITYDETECTION(��Ⱥ�ܶȼ���¼�)��Ӧ�Ĺ�������
typedef struct tagCFG_TRAFFICVIOLATIONS_INFO
{
	// ��Ϣ
	int				     ruleId;
	char                 szRuleName[MAX_NAME_LEN];          // ��������,��ͬ����������
	bool                 blRuleEnable;                      // ����ʹ��
	bool		         blTrackEnable;                     // ��������ʹ��, ���԰��ߣ����ֹ�����Ч
	char                 szObjectTypes[MAX_NAME_LEN];		// ��Ӧ��������
	Alarm_RuleType       ruleType;                          // Ԥ���������� ParkingRestrictedStop��ͣ����ͣ�� ViolationTrafficLine ������ʻ ExcessiveSpeed ���� WrongSide���� FireAlarm
	int				     nDirection;	  			        // ��ԽΧ������:0:��������;1:��������;2:���߶�����
	int				     nPlace;
	float                fSpeedLimit;                       // ����ֵ  һ����ݹ���Ҫ�� ����10%�������� ����������豸��10%�����
	int                  nMinDuration;                      // ��̳���ʱ�� ��λ���룬0~65535�� 0 ÿ�ζ����� > 0 ���Ǽ��ʱ�� 

	int                  nDetectRegionPoint;                // ������򶥵���
	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // �������
	int                  nTimeSection;                      // ����ʱ������
	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // ����ʱ��

	unsigned char        byReserved[20];                    // �����ֶ�

	tagCFG_TRAFFICVIOLATIONS_INFO() : ruleId(-1), blRuleEnable(true), nDetectRegionPoint(0), nMinDuration(80), fSpeedLimit(0.0f), ruleType(ZH_ALARM_UNKNOWN) {};
} CFG_TRAFFICVIOLATIONS_INFO;

// 1.5 ������Ϣ�ṹ��
//���ر�����Ϣ �ܽ�Ͱ���ʹ��
typedef struct tagDetectAlarm_Info {
	std::string 	    sceneId;		  //������ �����ĳ���
	std::string         detectId;         //���Ŀ��id
	std::string         detectImage;	  //���Ŀ��ͼ�� base64
	std::string         trackId;          //����id
	std::string			klass;			  //Ŀ������
	std::string			category;		  //Ŀ����� 
	bool 		        blUseRule;		  //ʹ���趨������ true ��ʹ��false
	int                 nDirect;          //�����ж�  �����ܽ���˵���� 0:���⵽��;1:���ﵽ�� 2:���߶�����; -1 ���ж� 
										  //��Ӧ������˵���� 0:������; 1:���ҵ��� 2 : ����; -1 ���ж�  ���ߺ��ܽ��ж�ѹ�ߵ��������
	int                 nPlace;           //λ���ж� �ܽ�  0:����; 1:���� 2:������; -1���ж� direct��place ����ͬʱʹ�� ���ұ�����һ��Ϊ-1
										  // ���� 0: �ұ� 1: ���  2:������; -1���ж�

	float 		        fDistance;		  //���Ŀ�굽�ܽ硢���ߵô�ֱ���� ȱʡΪ0 ��Ϊ0ʱ���뵽���߷�Χһ�������ھͱ���
	bool		        blBoundOrVertex;  //���ߺ��ܽ�ѹ����� ѹ��ʱ�Ƿ���Ϊ���� ������Ϊ��ɴ��ﵽ�� ������֮��Ķ��� ȱʡΪfalse
	float               fSpeed;           //Ŀ���ٶ�

	alertDetailInfo     lastAlert;        //��һ��λ�ú�ʱ�� �� place==-1 ʱ ���������� ֻʹ�õ�ǰ���� 
	alertDetailInfo     currentAlert;     //����λ�ú�ʱ�� ���жϷ���ʱ laseAlert ��currentAlert��ֵ����Ҫ
	int 		  	    nRuleId;		  //�������Ƿ���Ҫ�������λ��
	int			        nAlarmType;	      //��������
	bool 			    blDomeTrack;	  //�������Ƿ���Ҫ�������λ��
	int 			    nDeviceId;		  //��������Ҫ�������λ�õ� ָ�����id ���-1��ʾ���

	void*				pUser1;			  // ��Ŀ��ObjectState�ṹ��ָ��  currentAlertʹ�� ��Ϊ��ʱ��Ҫ�Լ����� Ϊ0ʱ��Ҫ�Լ����㷽λ�Ǻ�gps��Ϣ
	void*				pUser2;			  //
	unsigned char       byReserved[20];   //�����ֶ�  

	tagDetectAlarm_Info() : blUseRule(true), nDirect(-1), nPlace(0), blBoundOrVertex(false), fDistance(0.0f), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
} DETECTALARM_INFO;

//1.6 ����ͼ�����ṹ��
//���ر�����Ϣ ��Ⱥ�ۼ�ʹ��
typedef struct tagDensityAlarm_Info {
	//ǰ��Ϊ�������
	std::string			sceneId;
	int64_t				timestamp;			// ������Unixʱ������Ժ���Ϊ��λ
	unsigned long		nDensityThreshold;  // ����Ⱥ�ܶȽṹ����δ����������ֵ���˴���������һ���������ֵ������Ӧ�ܶȱ���������̬����
	int					nWidth;				// �������ݷֿ���Ϣ �������  ������Ϊ16 ����Ϊ9 ��ô map�����std::vector<int> ����16*9��Ԫ�� ÿ��Ԫ�ش���ָ������ڵļ��Ŀ��ֵ, �����-1��ʾ����Ҫ�ܶ���Ϣ
	int					nHeight;			// �������ݷֿ���Ϣ �������  �����õĲ���CFG_DENSITYDETECTION_INFO�ṹ����˽ṹ���ڵ�������ֵ��һ��ʱ���Դ˽ṹ��Ϊ׼
	std::string			klass;			    // Ŀ������
	std::string			category;		    // Ŀ����� 
	std::string			image;				// Ŀ��ͼ�� base64
	float				fTotal;				// �ܶȼ������Ļ�����Ŀ������
	std::vector<float>  vStatistical;		// ʾ�� [1,1,1,1,1,1,1 .....]
											// ����Ϊ�������
	int					nRuleId;			// ������д��  ��������id

	alertDetailInfo     currentAlert;
	float				fAlarmTotal;		//���������ڵ����� ��ٷֱ�
	int					nAlarmType;			//��������
	bool				blDomeTrack;		//�������Ƿ���Ҫ�������λ��
	int					nDeviceId;			//��������Ҫ�������λ�õ� ָ�����id ���-1��ʾ���

	void*				pUser1;			  // ��Ŀ��ObjectState�ṹ��ָ��  currentAlertʹ�� ��Ϊ��ʱ��Ҫ�Լ����� Ϊ0ʱ��Ҫ�Լ����㷽λ�Ǻ�gps��Ϣ
	void*				pUser2;			  //
	unsigned char       byReserved[20];   //�����ֶ�  
	tagDensityAlarm_Info() : nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
} DENSITYALARM_INFO;

// 1.7 ��ͨ�౨����Ϣ�ṹ��
typedef struct tagTrafficAlarm_Info {
	std::string 	    sceneId;		  //������ �����ĳ���
	std::string         detectId;         //���Ŀ��id
	std::string         detectImage;	  //���Ŀ��ͼ�� base64
	std::string         trackId;          //����id
	std::string			klass;			  //Ŀ������
	std::string			category;		  //Ŀ����� 
	bool 		        blUseRule;		  //ʹ���趨������ true ��ʹ��false
	int                 ruleType;         //Ԥ���������� ParkingRestrictedStop��ͣ����ͣ�� ViolationTrafficLine ������ʻ ExcessiveSpeed ���� WrongSide���� FireAlarm
	int                 nDirect;          //�����ж�  �����ܽ���˵���� 0:���⵽��;1:���ﵽ�� 2:���߶�����; -1 ���ж� 
										  //��Ӧ������˵���� 0:������; 1:���ҵ��� 2 : ����; -1 ���ж�  ���ߺ��ܽ��ж�ѹ�ߵ��������
	int                 nPlace;           //λ���ж� �ܽ�  0:����; 1:���� 2:������; -1���ж� direct��place ����ͬʱʹ�� ���ұ�����һ��Ϊ-1
										  // ���� 0: �ұ�
	float               fSpeedLimit;      // ����ֵ  һ����ݹ���Ҫ�� ����10%�������� ����������豸��10%�����
	int                 nMinDuration;     // ��̳���ʱ�� ��λ���룬0~65535�� 0 ÿ�ζ����� > 0 ���Ǽ��ʱ�� 

	alertDetailInfo     lastAlert;        //��һ��λ�ú�ʱ�� �� place==-1 ʱ ���������� ֻʹ�õ�ǰ���� 
	alertDetailInfo     currentAlert;     //����λ�ú�ʱ�� ���жϷ���ʱ laseAlert ��currentAlert��ֵ����Ҫ
	int 		  	    nRuleId;		  //�������Ƿ���Ҫ�������λ��
	int			        nAlarmType;	      //��������
	float               fSpeed;           //Ŀ���ٶ�

	bool 			    blDomeTrack;	  //�������Ƿ���Ҫ�������λ��
	int 			    nDeviceId;		  //��������Ҫ�������λ�õ� ָ�����id ���-1��ʾ���

	void*				pUser1;			  // ��Ŀ��ObjectState�ṹ��ָ��  currentAlertʹ�� ��Ϊ��ʱ��Ҫ�Լ����� Ϊ0ʱ��Ҫ�Լ����㷽λ�Ǻ�gps��Ϣ
	void*				pUser2;			  //
	unsigned char       byReserved[20];   //�����ֶ�  

	tagTrafficAlarm_Info() : blUseRule(true), nMinDuration(0), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
} TRAFFICALARM_INFO;


//#pragma once
//
//#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
//#ifdef __WINDOWS_OS__
//#undef __WINDOWS_OS__
//#endif
//#define __WINDOWS_OS__ 1
//#endif
//
//#define MAX_NAME_LEN                            128             // ͨ�������ַ�������
//#define MAX_OBJECT_LIST_SIZE                    256             // ��Ƶ�����豸֧�ֵļ�����������б��������
//#define MAX_POLYLINE_NUM                        20              // ��Ƶ�����豸���߶����������
//#define MAX_DAY_NUM                             31              // һ�µ�����
//#define MAX_WEEK_DAY_NUM						7				// ÿ�ܵ�����
//#define MAX_MONTH_NUM                           12              // һ��12����
//#define MAX_REC_TSECT_EX                        24              // ¼��ʱ�����չ���� ��24Сʱ����
//
//#define MAX_POLYGON_NUM                         256             // ��Ƶ�����豸���򶥵��������
//#define MAX_CHANNELNAME_LEN                     64              // ���ͨ�����Ƴ���
//#define MAX_CHANNEL_COUNT                       36
//#define MAX_VIDEO_CHANNEL_NUM                   256             // ���ͨ����256
//#define MAX_DBKEY_NUM                           64              // ���ݿ�ؼ������ֵ
//#define MAX_SUMMARY_LEN                         1024            // ���ӵ�JPEGͼƬ��ժҪ��Ϣ��󳤶�
//#define MAX_CALIBRATEBOX_NUM                    10              // ���ܷ���У׼���������
//
//#define MAX_GUARDTIME_NUM		MAX_REC_TSECT_EX
////windows
//#ifndef MAX_PATH
//#define MAX_PATH								255
//#endif // !
//
////double �����ж�
//#ifndef EPSILON
//#define EPSILON   (1e-10)
//#endif 
////float
//#ifndef EPSILON_FLOAT
//#define EPSILON_FLOAT   (1e-6)
//#endif 
//
//#include <map>
//#include <vector>
//
//typedef struct stru_Point2D
//{
//	double x;
//	double y;
//
//	stru_Point2D() : x(0.0), y(0.0) {};
//	stru_Point2D(double _x, double _y) : x(_x), y(_y) {};
//} Point2D, CFG_POINT2D;
//
//typedef struct stru_Point3D
//{
//	double x;
//	double y;
//	double z;
//	stru_Point3D() : x(0.0), y(0.0), z(0.0) {};
//	stru_Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};
//
//} Point3D, CFG_POINT3D;
//
//enum ALARM_ERROR_CODE {
//	ERROR_NONE = 0,           //û�д���
//	ERROR_PARAMS,             //�����������
//	ERROR_TYPE,               //�������ʹ���
//	ERROR_VALIDTIME,          //����ʱ����Ч
//	ERROR_NOIMPL = 0xFF       //����û��ʵ��
//};
//
//// Size
//typedef struct tagCFG_SIZE
//{
//	union
//	{
//		float	fWidth;			// ��
//		float	fArea;			// ���
//	};
//	float		fHeight;		// ��
//} CFG_SIZE;
//
//// ������Ϣ
//typedef struct tagCFG_REGION
//{
//	int         nPointNum;
//	CFG_POINT2D stuPolygon[MAX_POLYGON_NUM];
//}CFG_REGION;
//
//// RGBA��Ϣ
//typedef struct tagCFG_RGBA
//{
//	int	nRed;
//	int	nGreen;
//	int	nBlue;
//	int	nAlpha;
//} CFG_RGBA;
//
//// ʱ�����Ϣ
//typedef struct tagCFG_TIME_SECTION
//{
//	int	nBeginHour;
//	int	nBeginMin;
//	int	nBeginSec;
//	int	nEndHour;
//	int	nEndMin;
//	int	nEndSec;
//
//	tagCFG_TIME_SECTION() : nBeginHour(-1), nBeginMin(-1), nBeginSec(0), nEndHour(0), nEndMin(0), nEndSec(0) {};
//} CFG_TIME_SECTION;
//
//// У׼����Ϣ
//typedef struct tagCFG_CALIBRATEBOX_INFO
//{
//	CFG_POINT2D stuCenterPoint;     // У׼�����ĵ�����(��������һ����[0,8191]����)
//	float       fRatio;             // ��Ի�׼У׼��ı���(����1��ʾ��׼���С��0.5��ʾ��׼���С��һ��)
//} CFG_CALIBRATEBOX_INFO;
//
//// �ߴ������
//typedef struct tagCFG_SIZEFILTER_INFO
//{
//	int                   nCalibrateBoxNum;                           // У׼�����
//	CFG_CALIBRATEBOX_INFO stuCalibrateBoxs[MAX_CALIBRATEBOX_NUM];     // У׼��(Զ�˽��˱궨ģʽ����Ч)
//	bool                  bMeasureModeEnable;                         // ������ʽ�����Ƿ���Ч
//	unsigned char         bMeasureMode;                               // ������ʽ,0-���أ�����ҪԶ�ˡ����˱궨, 1-ʵ�ʳ��ȣ���λ����, 2-Զ�˽��˱궨�������
//	bool                  bFilterTypeEnable;                          // �������Ͳ����Ƿ���Ч
//																	  // ByArea,ByRatio�������ݣ�ʹ�ö�����ByArea��ByRatioѡ����� 2012/03/06
//	unsigned char		  bFilterType;			 				      // ��������:0:"ByLength",1:"ByArea", 2"ByWidthHeight"
//	unsigned char         bReserved[2];                               // �����ֶ�
//	bool                  bFilterMinSizeEnable;                       // ������С�ߴ�����Ƿ���Ч
//	bool                  bFilterMaxSizeEnable;                       // �������ߴ�����Ƿ���Ч
//	CFG_SIZE			  stuFilterMinSize;						      // ������С�ߴ� "ByLength"ģʽ�±�ʾ��ߵĳߴ磬"ByArea"ģʽ�¿��ʾ���������Ч(Զ�˽��˱궨ģʽ�±�ʾ��׼��Ŀ�߳ߴ�)��
//	CFG_SIZE			  stuFilterMaxSize;				              // �������ߴ� "ByLength"ģʽ�±�ʾ��ߵĳߴ磬"ByArea"ģʽ�¿��ʾ���������Ч(Զ�˽��˱궨ģʽ�±�ʾ��׼��Ŀ�߳ߴ�)��
//
//	bool                  abByArea;
//	bool                  abMinArea;
//	bool                  abMaxArea;
//	bool                  abMinAreaSize;
//	bool                  abMaxAreaSize;
//	bool                  bByArea;                                    // �Ƿ�������� ͨ������ComplexSizeFilter�ж��Ƿ����
//	bool                  bReserved1[2];
//	float                 nMinArea;                                   // ��С���
//	float                 nMaxArea;                                   // ������
//	CFG_SIZE              stuMinAreaSize;                             // ��С������ο�ߴ� "������ʽ"Ϊ"����"ʱ����ʾ��С������ο�Ŀ�߳ߴ磻"������ʽ"Ϊ"Զ�˽��˱궨ģʽ"ʱ����ʾ��׼�����С��߳ߴ磻
//	CFG_SIZE              stuMaxAreaSize;                             // ���������ο�ߴ�, ͬ��
//
//	bool                  abByRatio;
//	bool                  abMinRatio;
//	bool                  abMaxRatio;
//	bool                  abMinRatioSize;
//	bool                  abMaxRatioSize;
//	bool                  bByRatio;                                   // �Ƿ񰴿�߱ȹ��� ͨ������ComplexSizeFilter�ж��Ƿ����
//	bool                  bReserved2[2];
//	double                dMinRatio;                                  // ��С��߱�
//	double                dMaxRatio;                                  // ����߱�
//	CFG_SIZE              stuMinRatioSize;                            // ��С��߱Ⱦ��ο�ߴ磬��С��߱ȶ�Ӧ���ο�Ŀ�߳ߴ硣
//	CFG_SIZE              stuMaxRatioSize;                            // ����߱Ⱦ��ο�ߴ磬ͬ��
//
//	int                   nAreaCalibrateBoxNum;                       // ���У׼�����
//	CFG_CALIBRATEBOX_INFO stuAreaCalibrateBoxs[MAX_CALIBRATEBOX_NUM]; // ���У׼��
//	int                   nRatioCalibrateBoxs;                        // ���У׼�����
//	CFG_CALIBRATEBOX_INFO stuRatioCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// ���У׼��
//
//	bool                  blBySizeEnable;                             // �������ʹ�ܲ����Ƿ���Ч
//	bool                  blBySize;                                   // �������ʹ��
//}CFG_SIZEFILTER_INFO;
//
////CrossLine ���� CrossRegion �ܽ� Density �����ܶ�  [v1.3] ParkingRestrictedStop��ͣ����ͣ�� ViolationTrafficLine ������ʻ ExcessiveSpeed ���� WrongSide���� FireAlarm
//enum Alarm_RuleType {
//	ZH_ALARM_CROSSLINE = 0x00,
//	ZH_ALARM_CROSSREGION,
//	ZH_ALARM_DENSITY,
//	ZH_ALARM_PARKINGRESTRICTEDSTOP,
//	ZH_ALARM_VIOLATIONTRAFFICLINE,
//	ZH_ALARM_EXCESSIVESPEED,
//	ZH_ALARM_WRONGSIDE = 0x06,
//	ZH_ALARM_FIREALARM = 0x10,
//	ZH_ALARM_UNKNOWN = 0x99
//};
//
////����ʱ���  
//typedef struct tagCFG_TIMEGUARD_INFO
//{
//	int				 guardDateTimeId;				// id	   
//	int				 guardDateTimeType;				// 0������ 1 �ֶ� 2������ÿ�� 3���·�ÿ�� ����ֻ����һ������ ��ʡ�ڴ�
//													// 0���ж�ʱ�� 1һ���Ե� 2�ж� stuDayTimeSection 3 �ж� stuWeekTimeSection 
//	CFG_TIME_SECTION stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
//																								//���� CFG_TIME_SECTION stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// �¼���Ӧʱ��� ��ÿ����Ϊ����
//	tagCFG_TIMEGUARD_INFO() : guardDateTimeId(-1), guardDateTimeType(0) {};
//} CFG_TIMEGUARD_INFO;
//
//struct alertDetailInfo {              //���������Ϣ�ṹ�� �����ʱ���
//	int64_t timestamp;                //�������ʱ���
//	float fx;                         //����������� x ���ĵ�
//	float fy;                         //����������� y ���ĵ�
//	float fw;
//	float fh;
//	float ftx;                        //�������������� x
//	float fty;                        //�������������� y
//									  //v1.8������
//	float refx;
//	float refy;
//	float refw;
//	float refh;
//
//	float pan;
//	float tilt;
//	float zoom;
//
//	float azimuth;
//	float elevation;
//	float latitude;
//	float longitude;
//
//	alertDetailInfo() : timestamp(0), fx(0.0f), fy(0.0f), fw(0.0f), fh(0.0f), ftx(0.0f), fty(0.0f), refx(0.0f), refy(0.0f), refw(0.0f), refh(0.0f),
//		pan(0.0f), tilt(0.0f), zoom(0.0f), azimuth(0), elevation(0), latitude(0), longitude(0) {};
//	alertDetailInfo(int64_t _timestamp, float _x, float _y, float _w, float _h, float _tx, float _ty, float _refx, float _refy, float _refw, float _refh,
//		float _pan, float _tilt, float _zoom, float _azimuth, float _elevation, float _latitude, float _longitude) :
//		timestamp(_timestamp), fx(_x), fy(_y), fw(_w), fh(_h), ftx(_tx), fty(_ty), refx(_refx), refy(_refy), refw(_refw), refh(_refh),
//		pan(_pan), tilt(_tilt), zoom(_zoom), azimuth(_azimuth), elevation(_elevation), latitude(_latitude), longitude(_longitude) {};
//};
//
////### 1.1 ���߹���ṹ��
//typedef struct tagCFG_CROSSLINE_INFO
//{
//	// ��Ϣ
//	int				    ruleId;
//	char			    szRuleName[MAX_NAME_LEN];		  // ��������,��ͬ����������
//	bool			    blRuleEnable;					  // ����ʹ��
//	bool		        blTrackEnable;                    // ��������ʹ��,���԰��ߣ����ֹ�����Ч
//	int				    nDirection;						  // ��ⷽ��:��ruleType����Ϊ CrossLineΪ���� ʱ ��ⷽ�� 0:��������;1:��������;2:���߶�����
//														  // ��ruleType����Ϊ CrossRegionΪ�ܽ� ʱ ��ⷽ�� 0:���⵽��;1:���ﵽ�� 2:���߶�����
//														  // ��ruleType����Ϊ Densityʱ ������Ч
//	int				    nPlace;
//	float			    fDistance;						  // �����߱�������  
//	int				    nDetectLinePoint;				  // ������/���߶�����
//	CFG_POINT2D		    stuDetectLine[MAX_POLYLINE_NUM];  // ������/���߶������� ԭ���ϲ���� ���ܽ��棡
//
//	int                 nTimeSection;                     // ����ʱ������
//	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	  // ����ʱ��
//														  //int               nTimeSection;                     // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
//														  //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
//														  //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	// �¼���Ӧʱ��� ��ÿ����Ϊ����
//
//														  //bool              bSizeFileter;                     // �����ض��ĳߴ�������Ƿ���Ч
//														  //CFG_SIZEFILTER_INFO stuSizeFileter;                 // �����ض��ĳߴ������
//
//	int                 nTriggerPosition;                 // ��������λ����
//	unsigned char       byTriggerPosition[8];             // ��������λ��,0-Ŀ����ӿ�����, 1-Ŀ����ӿ��������, 2-Ŀ����ӿ򶥶�����, 3-Ŀ����ӿ��Ҷ�����, 4-Ŀ����ӿ�׶�����
//	int				    nTrackDuration;				      // ���ٳ���ʱ��,0��:һֱ����,1~300��:���ٳ���ʱ��
//	unsigned char       byReserved[20];                   // �����ֶ� 
//	tagCFG_CROSSLINE_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0), nDirection(0), nPlace(0), fDistance(0.0f) {};
//} CFG_CROSSLINE_INFO;
//
////### 1.2 �ܽ����ṹ��
////�¼�����EVENT_IVS_CROSSFENCEDETECTION(��ԽΧ������)��Ӧ�Ĺ�������
//typedef struct tagCFG_CROSSFENCEDETECTION_INFO
//{
//	// ��Ϣ
//	int				    ruleId;
//	char			    szRuleName[MAX_NAME_LEN];				 // ��������,��ͬ����������
//	bool			    blRuleEnable;							 // ����ʹ��
//	bool			    blTrackEnable;                           // ��������ʹ��,���԰��ߣ����ֹ�����Ч
//	int				    nDirection;							     // ��ԽΧ������:0:��������;1:��������;2:���߶�����
//	int				    nPlace;
//	int				    nDetectRegionPoint;					     // �ܽ綥����
//	CFG_POINT2D   	    stuDetectRegion[MAX_POLYLINE_NUM];		 // �ܽ����� ��������
//	int                 nTimeSection;                            // ����ʱ������
//	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	         // ����ʱ��
//																 //bool			    guardTimeEnabled;						 // ����ʱ��
//																 //int               nTimeSection;                            // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection 3 һ���Ե� ʹ��stuDayTimeSection
//																 //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
//																 //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// �¼���Ӧʱ��� ��ÿ����Ϊ����
//
//																 //bool              bSizeFileter;                            // �����ض��ĳߴ�������Ƿ���Ч
//																 //CFG_SIZEFILTER_   INFO stuSizeFileter;                     // �����ض��ĳߴ������
//	int                 nTriggerPosition;                        // ��������λ����
//	unsigned char       byTriggerPosition[8];                    // ��������λ��,0-Ŀ����ӿ�����, 1-Ŀ����ӿ��������, 2-Ŀ����ӿ򶥶�����, 3-Ŀ����ӿ��Ҷ�����, 4-Ŀ����ӿ�׶�����
//	int				    nTrackDuration;						     // ���ٳ���ʱ��,0��:һֱ����,1~300��:���ٳ���ʱ��
//	unsigned char       byReserved[20];                          // �����ֶ�
//	tagCFG_CROSSFENCEDETECTION_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTriggerPosition(0), nTrackDuration(0), nTimeSection(0), nDetectRegionPoint(0), nDirection(0), nPlace(0) {};
//} CFG_CROSSFENCEDETECTION_INFO;
//
//// 1.3 ��Ⱥ�ܶȼ��ṹ��
////�¼�����EVENT_IVS_DENSITYDETECTION(��Ⱥ�ܶȼ���¼�)��Ӧ�Ĺ�������
//typedef struct tagCFG_DENSITYDETECTION_INFO
//{
//	// ��Ϣ
//	int				     ruleId;
//	char                 szRuleName[MAX_NAME_LEN];          // ��������,��ͬ����������
//	bool                 blRuleEnable;                      // ����ʹ��
//	bool		         blTrackEnable;                     // ��������ʹ��, ���԰��ߣ����ֹ�����Ч
//	char                 szObjectTypes[MAX_NAME_LEN];		// ��Ӧ��������
//	unsigned char        byUnit;                            // �ܶȼ�ⱨ����ʽ : byUnit==0 �ܶȵȼ� ��ֵ��λGB/T-30147������Ϊ�ĸ�����ϡ�裬ϡ�裬�ܼ������ܼ���
//															//1)��ϡ��:Ŀ�긲�����ռ������������0~25%,
//															//2)ϡ��:Ŀ�긲�����ռ������������26%~50 %,
//															//3)�ܼ�:Ŀ�긲�����ռ������������51%~75 %,
//															//4)���ܼ�:Ŀ�긲�����ռ������������76%~100%  ; nThresholdNum ����ȼ� 1��2��3��4
//															//byUnit==1 �ܶȰٷֱ� nThresholdNum ��ʱ����ٷֱ� 1-100 ; byUnit==2 ��ֵ  ʹ��nThresholdNum ��ʱ������� >0
//
//															//unsigned char      bySensitivity;                     // ������(ȡֵ1-100��ֵԽС������Խ�ͣ���Ӧ��Ⱥ���ܼ��̶�Խ��)
//															//unsigned char      byMinAlarmDensity;                 // ��С�����ܶ� ȡֵ1-100,��������ֵ�ٷֱ� �������ڵ��ܶ�ֵ����ȫ������ڵİٷֱȱ��� 0ʱ��ʹ��
//
//	int                  nThresholdNum;                     // ������ֵ nThresholdNum ����ȼ� 1��2��3��4 byUnit==1 �ܶȰٷֱ� nThresholdNum ��ʱ����ٷֱ� 1-100 ; byUnit==2 ��ֵ  ʹ��nThresholdNum ��ʱ������� >0
//															//int                  nTimePercent;			        // ������λʱ���ڰٷֱ� 0~100 ������Ϊ80 ���nMinDuration=10 ��ô����10������8�� �����ﵽ������ֵ -1ʱ��ʹ�ó���ʱ�� ֻ����ʱ��������  ��������
//	int                  nMinDuration;                      // ��̳���ʱ�� ��λ���룬0~65535�� 0 ÿ�ζ����� > 0 ���Ǽ��ʱ�� 
//
//	int                  nDetectRegionPoint;                // ������򶥵���
//	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // �������
//	int                  nTimeSection;                      // ����ʱ������
//	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // ����ʱ��
//
//															//int                nTimeSection;                      // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
//															//CFG_TIME_SECTION   stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];        // �¼���Ӧʱ��� ����
//															//CFG_TIME_SECTION   stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	 // �¼���Ӧʱ��� ������ 7��Ϊ��λ
//
//	int                  nWidth;                            // �������ݷֿ���Ϣ ������� ����3840x2160�Ļ����н�����ȷ�Ϊ16��9�У���ô��ⷵ�ص�DESITYALARM_INFO����statistical��vector��Ԫ���е�ͳ����ֵ������240*240��С��ָ���ͳ����ֵ
//	int                  nHeight;                           // �������ݷֿ���Ϣ ������� ��ֵΪȱʡֵ ���������Ϊ�����С
//
//	unsigned char        byReserved[20];                    // �����ֶ� 
//
//	tagCFG_DENSITYDETECTION_INFO() : ruleId(-1), blRuleEnable(true), byUnit(1), nThresholdNum(50), nMinDuration(80), nWidth(-1), nHeight(-1), nTimeSection(0), nDetectRegionPoint(0) {};
//} CFG_DENSITYDETECTION_INFO;
//
//// 1.4 ��ͨΥ�¼��
////�¼�����EVENT_IVS_DENSITYDETECTION(��Ⱥ�ܶȼ���¼�)��Ӧ�Ĺ�������
//typedef struct tagCFG_TRAFFICVIOLATIONS_INFO
//{
//	// ��Ϣ
//	int				     ruleId;
//	char                 szRuleName[MAX_NAME_LEN];          // ��������,��ͬ����������
//	bool                 blRuleEnable;                      // ����ʹ��
//	bool		         blTrackEnable;                     // ��������ʹ��, ���԰��ߣ����ֹ�����Ч
//	char                 szObjectTypes[MAX_NAME_LEN];		// ��Ӧ��������
//	Alarm_RuleType       ruleType;                          // Ԥ���������� ParkingRestrictedStop��ͣ����ͣ�� ViolationTrafficLine ������ʻ ExcessiveSpeed ���� WrongSide���� FireAlarm
//	int				     nDirection;	  			        // ��ԽΧ������:0:��������;1:��������;2:���߶�����
//	int				     nPlace;
//	float                fSpeedLimit;                       // ����ֵ  һ����ݹ���Ҫ�� ����10%�������� ����������豸��10%�����
//	int                  nMinDuration;                      // ��̳���ʱ�� ��λ���룬0~65535�� 0 ÿ�ζ����� > 0 ���Ǽ��ʱ�� 
//
//	int                  nDetectRegionPoint;                // ������򶥵���
//	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // �������
//	int                  nTimeSection;                      // ����ʱ������
//	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // ����ʱ��
//
//	unsigned char        byReserved[20];                    // �����ֶ�
//
//	tagCFG_TRAFFICVIOLATIONS_INFO() : ruleId(-1), blRuleEnable(true), nDetectRegionPoint(0), nMinDuration(80), fSpeedLimit(0.0f), ruleType(ZH_ALARM_UNKNOWN) {};
//} CFG_TRAFFICVIOLATIONS_INFO;
//
//// 1.5 ������Ϣ�ṹ��
////���ر�����Ϣ �ܽ�Ͱ���ʹ��
//typedef struct tagDetectAlarm_Info {
//	std::string 	    sceneId;		  //������ �����ĳ���
//	std::string         detectId;         //���Ŀ��id
//	std::string         detectImage;	  //���Ŀ��ͼ�� base64
//	std::string         trackId;          //����id
//	std::string			klass;			  //Ŀ������
//	std::string			category;		  //Ŀ����� 
//	bool 		        blUseRule;		  //ʹ���趨������ true ��ʹ��false
//	int                 nDirect;          //�����ж�  �����ܽ���˵���� 0:���⵽��;1:���ﵽ�� 2:���߶�����; -1 ���ж� 
//										  //��Ӧ������˵���� 0:������; 1:���ҵ��� 2 : ����; -1 ���ж�  ���ߺ��ܽ��ж�ѹ�ߵ��������
//	int                 nPlace;           //λ���ж� �ܽ�  0:����; 1:���� 2:������; -1���ж� direct��place ����ͬʱʹ�� ���ұ�����һ��Ϊ-1
//										  // ���� 0: �ұ� 1: ���  2:������; -1���ж�
//
//	float 		        fDistance;		  //���Ŀ�굽�ܽ硢���ߵô�ֱ���� ȱʡΪ0 ��Ϊ0ʱ���뵽���߷�Χһ�������ھͱ���
//	bool		        blBoundOrVertex;  //���ߺ��ܽ�ѹ����� ѹ��ʱ�Ƿ���Ϊ���� ������Ϊ��ɴ��ﵽ�� ������֮��Ķ��� ȱʡΪfalse
//	float               fSpeed;           //Ŀ���ٶ�
//
//	alertDetailInfo     lastAlert;        //��һ��λ�ú�ʱ�� �� place==-1 ʱ ���������� ֻʹ�õ�ǰ���� 
//	alertDetailInfo     currentAlert;     //����λ�ú�ʱ�� ���жϷ���ʱ laseAlert ��currentAlert��ֵ����Ҫ
//	int 		  	    nRuleId;		  //�������Ƿ���Ҫ�������λ��
//	int			        nAlarmType;	      //��������
//	bool 			    blDomeTrack;	  //�������Ƿ���Ҫ�������λ��
//	int 			    nDeviceId;		  //��������Ҫ�������λ�õ� ָ�����id ���-1��ʾ���
//
//	void*				pUser1;			  // ��Ŀ��ObjectState�ṹ��ָ��  currentAlertʹ�� ��Ϊ��ʱ��Ҫ�Լ����� Ϊ0ʱ��Ҫ�Լ����㷽λ�Ǻ�gps��Ϣ
//	void*				pUser2;			  //
//	unsigned char       byReserved[20];   //�����ֶ�  
//
//	tagDetectAlarm_Info() : blUseRule(true), nDirect(-1), nPlace(0), blBoundOrVertex(false), fDistance(0.0f), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
//} DETECTALARM_INFO;
//
////1.6 ����ͼ�����ṹ��
////���ر�����Ϣ ��Ⱥ�ۼ�ʹ��
//typedef struct tagDensityAlarm_Info {
//	//ǰ��Ϊ�������
//	std::string			sceneId;
//	int64_t				timestamp;			// ������Unixʱ������Ժ���Ϊ��λ
//	unsigned long		nDensityThreshold;  // ����Ⱥ�ܶȽṹ����δ����������ֵ���˴���������һ���������ֵ������Ӧ�ܶȱ���������̬����
//	int					nWidth;				// �������ݷֿ���Ϣ �������  ������Ϊ16 ����Ϊ9 ��ô map�����std::vector<int> ����16*9��Ԫ�� ÿ��Ԫ�ش���ָ������ڵļ��Ŀ��ֵ, �����-1��ʾ����Ҫ�ܶ���Ϣ
//	int					nHeight;			// �������ݷֿ���Ϣ �������  �����õĲ���CFG_DENSITYDETECTION_INFO�ṹ����˽ṹ���ڵ�������ֵ��һ��ʱ���Դ˽ṹ��Ϊ׼
//	std::string			klass;			    // Ŀ������
//	std::string			category;		    // Ŀ����� 
//	std::string			image;				// Ŀ��ͼ�� base64
//	float				fTotal;				// �ܶȼ������Ļ�����Ŀ������
//	std::vector<float>  vStatistical;		// ʾ�� [1,1,1,1,1,1,1 .....]
//											// ����Ϊ�������
//	int					nRuleId;			// ������д��  ��������id
//
//	alertDetailInfo     currentAlert;
//	float				fAlarmTotal;		//���������ڵ����� ��ٷֱ�
//	int					nAlarmType;			//��������
//	bool				blDomeTrack;		//�������Ƿ���Ҫ�������λ��
//	int					nDeviceId;			//��������Ҫ�������λ�õ� ָ�����id ���-1��ʾ���
//
//	void*				pUser1;			  // ��Ŀ��ObjectState�ṹ��ָ��  currentAlertʹ�� ��Ϊ��ʱ��Ҫ�Լ����� Ϊ0ʱ��Ҫ�Լ����㷽λ�Ǻ�gps��Ϣ
//	void*				pUser2;			  //
//	unsigned char       byReserved[20];   //�����ֶ�  
//	tagDensityAlarm_Info() : nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
//} DENSITYALARM_INFO;
//
//// 1.7 ��ͨ�౨����Ϣ�ṹ��
//typedef struct tagTrafficAlarm_Info {
//	std::string 	    sceneId;		  //������ �����ĳ���
//	std::string         detectId;         //���Ŀ��id
//	std::string         detectImage;	  //���Ŀ��ͼ�� base64
//	std::string         trackId;          //����id
//	std::string			klass;			  //Ŀ������
//	std::string			category;		  //Ŀ����� 
//	bool 		        blUseRule;		  //ʹ���趨������ true ��ʹ��false
//	int                 ruleType;         //Ԥ���������� ParkingRestrictedStop��ͣ����ͣ�� ViolationTrafficLine ������ʻ ExcessiveSpeed ���� WrongSide���� FireAlarm
//	int                 nDirect;          //�����ж�  �����ܽ���˵���� 0:���⵽��;1:���ﵽ�� 2:���߶�����; -1 ���ж� 
//										  //��Ӧ������˵���� 0:������; 1:���ҵ��� 2 : ����; -1 ���ж�  ���ߺ��ܽ��ж�ѹ�ߵ��������
//	int                 nPlace;           //λ���ж� �ܽ�  0:����; 1:���� 2:������; -1���ж� direct��place ����ͬʱʹ�� ���ұ�����һ��Ϊ-1
//										  // ���� 0: �ұ�
//	float               fSpeedLimit;      // ����ֵ  һ����ݹ���Ҫ�� ����10%�������� ����������豸��10%�����
//	int                 nMinDuration;     // ��̳���ʱ�� ��λ���룬0~65535�� 0 ÿ�ζ����� > 0 ���Ǽ��ʱ�� 
//
//	alertDetailInfo     lastAlert;        //��һ��λ�ú�ʱ�� �� place==-1 ʱ ���������� ֻʹ�õ�ǰ���� 
//	alertDetailInfo     currentAlert;     //����λ�ú�ʱ�� ���жϷ���ʱ laseAlert ��currentAlert��ֵ����Ҫ
//	int 		  	    nRuleId;		  //�������Ƿ���Ҫ�������λ��
//	int			        nAlarmType;	      //��������
//	float               fSpeed;           //Ŀ���ٶ�
//
//	bool 			    blDomeTrack;	  //�������Ƿ���Ҫ�������λ��
//	int 			    nDeviceId;		  //��������Ҫ�������λ�õ� ָ�����id ���-1��ʾ���
//
//	void*				pUser1;			  // ��Ŀ��ObjectState�ṹ��ָ��  currentAlertʹ�� ��Ϊ��ʱ��Ҫ�Լ����� Ϊ0ʱ��Ҫ�Լ����㷽λ�Ǻ�gps��Ϣ
//	void*				pUser2;			  //
//	unsigned char       byReserved[20];   //�����ֶ�  
//
//	tagTrafficAlarm_Info() : blUseRule(true), nMinDuration(0), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
//} TRAFFICALARM_INFO;
