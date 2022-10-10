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
#define MAX_REC_TSECT_EX                        24              // ¼��ʱ�����չ����

#define MAX_POLYGON_NUM                         256             // ��Ƶ�����豸���򶥵��������
#define MAX_CHANNELNAME_LEN                     64              // ���ͨ�����Ƴ���
#define MAX_CHANNEL_COUNT                       36
#define MAX_VIDEO_CHANNEL_NUM                   256             // ���ͨ����256
#define MAX_DBKEY_NUM                           64              // ���ݿ�ؼ������ֵ
#define MAX_SUMMARY_LEN                         1024            // ���ӵ�JPEGͼƬ��ժҪ��Ϣ��󳤶�
#define MAX_CALIBRATEBOX_NUM                    10              // ���ܷ���У׼���������

//windows
#ifndef MAX_PATH
#define MAX_PATH								255
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

enum ALERM_ERROR_CODE {
	ERROR_NONE = 0,
	ERROR_PARAMS,
	ERROR_TYPE,
	ERROR_VALIDTIME,
	ERROR_NOIMPL = 0xFF
};
// Size
typedef struct tagCFG_SIZE
{
	union
	{
		float				nWidth;			// ��
		float				nArea;			// ���
	};
	float					nHeight;		// ��
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
	int					nRed;
	int					nGreen;
	int					nBlue;
	int					nAlpha;
} CFG_RGBA;

// ʱ�����Ϣ
typedef struct tagCFG_TIME_SECTION
{
	int					nBeginHour;
	int					nBeginMin;
	int					nBeginSec;
	int					nEndHour;
	int					nEndMin;
	int					nEndSec;

	tagCFG_TIME_SECTION() : nBeginHour(-1), nBeginMin(-1), nBeginSec(0), nEndHour(0), nEndMin(0), nEndSec(0) {};
} CFG_TIME_SECTION;

// У׼����Ϣ
typedef struct tagCFG_CALIBRATEBOX_INFO
{
	CFG_POINT2D         stuCenterPoint;     // У׼�����ĵ�����(��������һ����[0,8191]����)
	float               fRatio;             // ��Ի�׼У׼��ı���(����1��ʾ��׼���С��0.5��ʾ��׼���С��һ��)
}CFG_CALIBRATEBOX_INFO;

// �ߴ������
typedef struct tagCFG_SIZEFILTER_INFO
{
	int                   nCalibrateBoxNum;                       // У׼�����
	CFG_CALIBRATEBOX_INFO stuCalibrateBoxs[MAX_CALIBRATEBOX_NUM]; // У׼��(Զ�˽��˱궨ģʽ����Ч)
	bool                bMeasureModeEnable;                       // ������ʽ�����Ƿ���Ч
	unsigned char       bMeasureMode;                             // ������ʽ,0-���أ�����ҪԶ�ˡ����˱궨, 1-ʵ�ʳ��ȣ���λ����, 2-Զ�˽��˱궨�������
	bool                bFilterTypeEnable;                        // �������Ͳ����Ƿ���Ч
																  // ByArea,ByRatio�������ݣ�ʹ�ö�����ByArea��ByRatioѡ����� 2012/03/06
	unsigned char		bFilterType;			 				  // ��������:0:"ByLength",1:"ByArea", 2"ByWidthHeight"
	unsigned char       bReserved[2];                             // �����ֶ�
	bool                bFilterMinSizeEnable;                     // ������С�ߴ�����Ƿ���Ч
	bool                bFilterMaxSizeEnable;                     // �������ߴ�����Ƿ���Ч
	CFG_SIZE			stuFilterMinSize;						  // ������С�ߴ� "ByLength"ģʽ�±�ʾ��ߵĳߴ磬"ByArea"ģʽ�¿��ʾ���������Ч(Զ�˽��˱궨ģʽ�±�ʾ��׼��Ŀ�߳ߴ�)��
	CFG_SIZE			stuFilterMaxSize;				          // �������ߴ� "ByLength"ģʽ�±�ʾ��ߵĳߴ磬"ByArea"ģʽ�¿��ʾ���������Ч(Զ�˽��˱궨ģʽ�±�ʾ��׼��Ŀ�߳ߴ�)��

	bool                abByArea;
	bool                abMinArea;
	bool                abMaxArea;
	bool                abMinAreaSize;
	bool                abMaxAreaSize;
	bool                bByArea;                                  // �Ƿ�������� ͨ������ComplexSizeFilter�ж��Ƿ����
	bool                bReserved1[2];
	float               nMinArea;                                 // ��С���
	float               nMaxArea;                                 // ������
	CFG_SIZE            stuMinAreaSize;                           // ��С������ο�ߴ� "������ʽ"Ϊ"����"ʱ����ʾ��С������ο�Ŀ�߳ߴ磻"������ʽ"Ϊ"Զ�˽��˱궨ģʽ"ʱ����ʾ��׼�����С��߳ߴ磻
	CFG_SIZE            stuMaxAreaSize;                           // ���������ο�ߴ�, ͬ��

	bool                abByRatio;
	bool                abMinRatio;
	bool                abMaxRatio;
	bool                abMinRatioSize;
	bool                abMaxRatioSize;
	bool                bByRatio;                                 // �Ƿ񰴿�߱ȹ��� ͨ������ComplexSizeFilter�ж��Ƿ����
	bool                bReserved2[2];
	double              dMinRatio;                                // ��С��߱�
	double              dMaxRatio;                                // ����߱�
	CFG_SIZE            stuMinRatioSize;                          // ��С��߱Ⱦ��ο�ߴ磬��С��߱ȶ�Ӧ���ο�Ŀ�߳ߴ硣
	CFG_SIZE            stuMaxRatioSize;                          // ����߱Ⱦ��ο�ߴ磬ͬ��

	int                 nAreaCalibrateBoxNum;                     // ���У׼�����
	CFG_CALIBRATEBOX_INFO stuAreaCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// ���У׼��
	int                 nRatioCalibrateBoxs;                      // ���У׼�����
	CFG_CALIBRATEBOX_INFO stuRatioCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// ���У׼��

	bool                abBySize;                                 // �������ʹ�ܲ����Ƿ���Ч
	bool                bBySize;                                  // �������ʹ��
}CFG_SIZEFILTER_INFO;

//### 1.1 ���߹���ṹ��
typedef struct tagCFG_CROSSLINE_INFO
{
	// ��Ϣ
	char				szRuleName[MAX_NAME_LEN];								// ��������,��ͬ����������
	bool				bRuleEnable;											// ����ʹ��
	unsigned char       bTrackEnable;                                           // ��������ʹ��,���԰��ߣ����ֹ�����Ч
	int					nDirection;												// ��ⷽ��:��ruleType����Ϊ CrossLineΪ���� ʱ ��ⷽ�� 0:��������;1:��������;2:���߶�����
																				// ��ruleType����Ϊ CrossRegionΪ�ܽ� ʱ ��ⷽ�� 0:���⵽��;1:���ﵽ�� 2:���߶�����
																				// ��ruleType����Ϊ Density ʱ ������Ч
	int					nDetectLinePoint;										// ������/���߶�����
	CFG_POINT2D		    stuDetectLine[MAX_POLYLINE_NUM];						// ������/���߶������� ԭ���ϲ���� ���ܽ��棡

	int                 nTimeSection;                                           // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
	CFG_TIME_SECTION	stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
	CFG_TIME_SECTION	stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// �¼���Ӧʱ��� ��ÿ����Ϊ����

	//bool                bSizeFileter;                                         // �����ض��ĳߴ�������Ƿ���Ч
	//CFG_SIZEFILTER_INFO stuSizeFileter;                                       // �����ض��ĳߴ������

	int                 nTriggerPosition;                                       // ��������λ����
	unsigned char       bTriggerPosition[8];                                    // ��������λ��,0-Ŀ����ӿ�����, 1-Ŀ����ӿ��������, 2-Ŀ����ӿ򶥶�����, 3-Ŀ����ӿ��Ҷ�����, 4-Ŀ����ӿ�׶�����
	int					nTrackDuration;											// ���ٳ���ʱ��,0��:һֱ����,1~300��:���ٳ���ʱ��
	unsigned char       bReserved[20];                                          // �����ֶ� 
	tagCFG_CROSSLINE_INFO() : bRuleEnable(true), bTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0) {};
} CFG_CROSSLINE_INFO;

//### 1.2 �ܽ����ṹ��
//�¼�����EVENT_IVS_CROSSFENCEDETECTION(��ԽΧ������)��Ӧ�Ĺ�������
typedef struct tagCFG_CROSSFENCEDETECTION_INFO
{
	// ��Ϣ
	char				szRuleName[MAX_NAME_LEN];								// ��������,��ͬ����������
	bool				bRuleEnable;											// ����ʹ��
	unsigned char       bTrackEnable;                                           // ��������ʹ��,���԰��ߣ����ֹ�����Ч

	int					nDirection;												// ��ԽΧ������:0:��������;1:��������;2:���߶�����
	int					nDetectRegionPoint;										// �ܽ綥����
	CFG_POINT2D   		stuDetectRegion[MAX_POLYLINE_NUM];						// �ܽ����� ��������

	int                 nTimeSection;                                           // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
	CFG_TIME_SECTION	stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// �¼���Ӧʱ��� ��ÿ��Ϊ����
	CFG_TIME_SECTION	stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// �¼���Ӧʱ��� ��ÿ����Ϊ����

	//bool                bSizeFileter;                                         // �����ض��ĳߴ�������Ƿ���Ч
	//CFG_SIZEFILTER_INFO stuSizeFileter;                                       // �����ض��ĳߴ������

	int                 nTriggerPosition;                                       // ��������λ����
	unsigned char       bTriggerPosition[8];                                    // ��������λ��,0-Ŀ����ӿ�����, 1-Ŀ����ӿ��������, 2-Ŀ����ӿ򶥶�����, 3-Ŀ����ӿ��Ҷ�����, 4-Ŀ����ӿ�׶�����

	int					nTrackDuration;											// ���ٳ���ʱ��,0��:һֱ����,1~300��:���ٳ���ʱ��
	unsigned char       bReserved[20];                                          // �����ֶ�
	tagCFG_CROSSFENCEDETECTION_INFO() : bRuleEnable(true), bTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0) {};
} CFG_CROSSFENCEDETECTION_INFO;

// 1.3 ��Ⱥ�ܶȼ��ṹ��
//�¼�����EVENT_IVS_DENSITYDETECTION(��Ⱥ�ܶȼ���¼�)��Ӧ�Ĺ�������
typedef struct tagCFG_DENSITYDETECTION_INFO
{
	// ��Ϣ
	char                 szRuleName[MAX_NAME_LEN];                          // ��������,��ͬ����������
	bool                 bRuleEnable;                                       // ����ʹ��
	unsigned char        byUnit;                                            // �ܶȼ����ֵ��λGB/T-30147��Ҫ�� 0 �ܶȵȼ�������Ϊ�ĸ�����ϡ�裬ϡ�裬�ܼ������ܼ���1 �ܶȰٷֱ�
	unsigned char        bSensitivity;                                      // ������(ȡֵ1-100��ֵԽС������Խ�ͣ���Ӧ��Ⱥ���ܼ��̶�Խ��)
	unsigned char        byMinAlarmDensity;                                 // ��С�����ܶ� ȡֵ1-100,��������ֵ�ٷֱ�

	//int                  nObjectTypeNum;                                  // ��Ӧ�������͸��� 
	//char                 szObjectTypes[MAX_OBJECT_LIST_SIZE][MAX_NAME_LEN]; // ��Ӧ���������б�
	//int                  nMinDuration;                                    // ��̳���ʱ�� ��λ���룬0~65535

	int                  nDetectRegionPoint;                                // ������򶥵���
	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];                  // �������

	int                  nTimeSection;                                      // 0���ж�ʱ�� 1�ж� stuDayTimeSection 2 �ж� stuWeekTimeSection
	CFG_TIME_SECTION     stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];      // �¼���Ӧʱ��� ����
	CFG_TIME_SECTION	 stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	// �¼���Ӧʱ��� ������ 7��Ϊ��λ

	int                  nGridCol;                                          // �������ݷֿ���Ϣ ������� ����3840x2160�Ļ����н�����ȷ�Ϊ16��9�У���ô��ⷵ�ص�DESITYALERM_INFO����statistical��vector��Ԫ���е�ͳ����ֵ������240*240��С��ָ���ͳ����ֵ
	int                  nGridRow;                                          // �������ݷֿ���Ϣ ������� ��ֵΪȱʡֵ ���������Ϊ�����С
	tagCFG_DENSITYDETECTION_INFO() : bRuleEnable(true), byUnit(1), bSensitivity(50), nTimeSection(0), nGridCol(-1), nGridRow(-1) {};

} CFG_DENSITYDETECTION_INFO;

// 1.4 ������Ϣ�ṹ��
//���ر�����Ϣ �ܽ�Ͱ���ʹ��
typedef struct tagDetectAlerm_Info {
	std::string detectId;             //���Ŀ��id
	std::string trackId;              //����id
	int direct;                       //�����ж�  �����ܽ���˵���� 0:���⵽��;1:���ﵽ�� 2:���߶�����; -1 ���ж� 
									  //��Ӧ������˵���� 0:������; 1:���ҵ��� 2 : ����; -1 ���ж�  ���ߺ��ܽ��ж�ѹ�ߵ��������
	bool boundOrVertex;				  //���ߺ��ܽ�ѹ����� ѹ��ʱ�Ƿ���Ϊ���� ������Ϊ��ɴ��ﵽ�� ������֮��Ķ��� ȱʡΪfalse
	int place;                        //λ���ж� �ܽ�  0:����; 1:���� 2:������; -1���ж� direct��place ����ͬʱʹ�� ���ұ�����һ��Ϊ-1
	                                  // ���� 0: �ұ� 1: ���  2:������; -1���ж�
	struct alertDetailInfo {          //���������Ϣ�ṹ�� �����ʱ���
		uint64_t timestamp;           //�������ʱ���
		float x;                      //����������� x ���ĵ�
		float y;                      //����������� y ���ĵ�
	};								 
	alertDetailInfo lastAlert;        //��һ��λ�ú�ʱ�� �� place==-1 ʱ ���������� ֻʹ�õ�ǰ���� 
	alertDetailInfo currentAlert;     //����λ�ú�ʱ�� ���жϷ���ʱ laseAlert ��currentAlert��ֵ����Ҫ
	tagDetectAlerm_Info() : direct(-1), place(0), boundOrVertex(false) {};
} DETECTALERM_INFO;

//1.5 ����ͼ�����ṹ��
//���ر�����Ϣ ��Ⱥ�ۼ�ʹ��
typedef struct tagdensityAlerm_Info {
	unsigned long nDensityThreshold;  // ����Ⱥ�ܶȽṹ����δ����������ֵ���˴���������һ���������ֵ������Ӧ�ܶȱ���������̬����
	int nGridCol;					  // �������ݷֿ���Ϣ �������  ������Ϊ16 ����Ϊ9 ��ô map�����std::vector<int> ����16*9��Ԫ�� ÿ��Ԫ�ش���ָ������ڵļ��Ŀ��ֵ, �����-1��ʾ����Ҫ�ܶ���Ϣ
	int nGridRow;					  // �������ݷֿ���Ϣ �������  �����õĲ���CFG_DENSITYDETECTION_INFO�ṹ����˽ṹ���ڵ�������ֵ��һ��ʱ���Դ˽ṹ��Ϊ׼
	std::map<std::string, int> total; // �ܶȼ������Ļ�����Ŀ������
	std::map<std::string, std::vector<float> > statistical;  //ʾ�� [["car",[1,1,1,1,1,1,1 .....]],["truck",[1,1,1,1,1,2,3,3,3,....]],["person",[1,3,33,4,4,4,4,0,0,0,0,]]
} DESITYALERM_INFO;
