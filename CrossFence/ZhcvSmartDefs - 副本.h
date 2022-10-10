#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#ifdef __WINDOWS_OS__
#undef __WINDOWS_OS__
#endif
#define __WINDOWS_OS__ 1
#endif

#define MAX_NAME_LEN                            128             // 通用名字字符串长度
#define MAX_OBJECT_LIST_SIZE                    256             // 视频分析设备支持的检测物体类型列表个数上限
#define MAX_POLYLINE_NUM                        20              // 视频分析设备折线顶点个数上限
#define MAX_DAY_NUM                             31              // 一月的天数
#define MAX_WEEK_DAY_NUM						7				// 每周的天数
#define MAX_MONTH_NUM                           12              // 一年12个月
#define MAX_REC_TSECT_EX                        24              // 录像时间段扩展个数

#define MAX_POLYGON_NUM                         256             // 视频分析设备区域顶点个数上限
#define MAX_CHANNELNAME_LEN                     64              // 最大通道名称长度
#define MAX_CHANNEL_COUNT                       36
#define MAX_VIDEO_CHANNEL_NUM                   256             // 最大通道数256
#define MAX_DBKEY_NUM                           64              // 数据库关键字最大值
#define MAX_SUMMARY_LEN                         1024            // 叠加到JPEG图片的摘要信息最大长度
#define MAX_CALIBRATEBOX_NUM                    10              // 智能分析校准框个数上限

//windows
#ifndef MAX_PATH
#define MAX_PATH								255
#endif // !

//double 精度判断
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
		float				nWidth;			// 宽
		float				nArea;			// 面积
	};
	float					nHeight;		// 高
} CFG_SIZE;


// 区域信息
typedef struct tagCFG_REGION
{
	int         nPointNum;
	CFG_POINT2D stuPolygon[MAX_POLYGON_NUM];
}CFG_REGION;

// RGBA信息
typedef struct tagCFG_RGBA
{
	int					nRed;
	int					nGreen;
	int					nBlue;
	int					nAlpha;
} CFG_RGBA;

// 时间段信息
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

// 校准框信息
typedef struct tagCFG_CALIBRATEBOX_INFO
{
	CFG_POINT2D         stuCenterPoint;     // 校准框中心点坐标(点的坐标归一化到[0,8191]区间)
	float               fRatio;             // 相对基准校准框的比率(比如1表示基准框大小，0.5表示基准框大小的一半)
}CFG_CALIBRATEBOX_INFO;

// 尺寸过滤器
typedef struct tagCFG_SIZEFILTER_INFO
{
	int                   nCalibrateBoxNum;                       // 校准框个数
	CFG_CALIBRATEBOX_INFO stuCalibrateBoxs[MAX_CALIBRATEBOX_NUM]; // 校准框(远端近端标定模式下有效)
	bool                bMeasureModeEnable;                       // 计量方式参数是否有效
	unsigned char       bMeasureMode;                             // 计量方式,0-像素，不需要远端、近端标定, 1-实际长度，单位：米, 2-远端近端标定后的像素
	bool                bFilterTypeEnable;                        // 过滤类型参数是否有效
																  // ByArea,ByRatio仅作兼容，使用独立的ByArea和ByRatio选项代替 2012/03/06
	unsigned char		bFilterType;			 				  // 过滤类型:0:"ByLength",1:"ByArea", 2"ByWidthHeight"
	unsigned char       bReserved[2];                             // 保留字段
	bool                bFilterMinSizeEnable;                     // 物体最小尺寸参数是否有效
	bool                bFilterMaxSizeEnable;                     // 物体最大尺寸参数是否有效
	CFG_SIZE			stuFilterMinSize;						  // 物体最小尺寸 "ByLength"模式下表示宽高的尺寸，"ByArea"模式下宽表示面积，高无效(远端近端标定模式下表示基准框的宽高尺寸)。
	CFG_SIZE			stuFilterMaxSize;				          // 物体最大尺寸 "ByLength"模式下表示宽高的尺寸，"ByArea"模式下宽表示面积，高无效(远端近端标定模式下表示基准框的宽高尺寸)。

	bool                abByArea;
	bool                abMinArea;
	bool                abMaxArea;
	bool                abMinAreaSize;
	bool                abMaxAreaSize;
	bool                bByArea;                                  // 是否按面积过滤 通过能力ComplexSizeFilter判断是否可用
	bool                bReserved1[2];
	float               nMinArea;                                 // 最小面积
	float               nMaxArea;                                 // 最大面积
	CFG_SIZE            stuMinAreaSize;                           // 最小面积矩形框尺寸 "计量方式"为"像素"时，表示最小面积矩形框的宽高尺寸；"计量方式"为"远端近端标定模式"时，表示基准框的最小宽高尺寸；
	CFG_SIZE            stuMaxAreaSize;                           // 最大面积矩形框尺寸, 同上

	bool                abByRatio;
	bool                abMinRatio;
	bool                abMaxRatio;
	bool                abMinRatioSize;
	bool                abMaxRatioSize;
	bool                bByRatio;                                 // 是否按宽高比过滤 通过能力ComplexSizeFilter判断是否可用
	bool                bReserved2[2];
	double              dMinRatio;                                // 最小宽高比
	double              dMaxRatio;                                // 最大宽高比
	CFG_SIZE            stuMinRatioSize;                          // 最小宽高比矩形框尺寸，最小宽高比对应矩形框的宽高尺寸。
	CFG_SIZE            stuMaxRatioSize;                          // 最大宽高比矩形框尺寸，同上

	int                 nAreaCalibrateBoxNum;                     // 面积校准框个数
	CFG_CALIBRATEBOX_INFO stuAreaCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// 面积校准框
	int                 nRatioCalibrateBoxs;                      // 宽高校准框个数
	CFG_CALIBRATEBOX_INFO stuRatioCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// 宽高校准框

	bool                abBySize;                                 // 长宽过滤使能参数是否有效
	bool                bBySize;                                  // 长宽过滤使能
}CFG_SIZEFILTER_INFO;

//### 1.1 拌线规则结构体
typedef struct tagCFG_CROSSLINE_INFO
{
	// 信息
	char				szRuleName[MAX_NAME_LEN];								// 规则名称,不同规则不能重名
	bool				bRuleEnable;											// 规则使能
	unsigned char       bTrackEnable;                                           // 触发跟踪使能,仅对绊线，入侵规则有效
	int					nDirection;												// 检测方向:当ruleType类型为 CrossLine为绊线 时 检测方向 0:由左至右;1:由右至左;2:两者都可以
																				// 当ruleType类型为 CrossRegion为周界 时 检测方向 0:由外到里;1:由里到外 2:两者都可以
																				// 当ruleType类型为 Density 时 此项无效
	int					nDetectLinePoint;										// 警戒线/绊线顶点数
	CFG_POINT2D		    stuDetectLine[MAX_POLYLINE_NUM];						// 警戒线/绊线顶点坐标 原则上不封闭 不能交叉！

	int                 nTimeSection;                                           // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
	CFG_TIME_SECTION	stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
	CFG_TIME_SECTION	stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// 事件响应时间段 按每星期为周期

	//bool                bSizeFileter;                                         // 规则特定的尺寸过滤器是否有效
	//CFG_SIZEFILTER_INFO stuSizeFileter;                                       // 规则特定的尺寸过滤器

	int                 nTriggerPosition;                                       // 触发报警位置数
	unsigned char       bTriggerPosition[8];                                    // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
	int					nTrackDuration;											// 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
	unsigned char       bReserved[20];                                          // 保留字段 
	tagCFG_CROSSLINE_INFO() : bRuleEnable(true), bTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0) {};
} CFG_CROSSLINE_INFO;

//### 1.2 周界规则结构体
//事件类型EVENT_IVS_CROSSFENCEDETECTION(翻越围栏规则)对应的规则配置
typedef struct tagCFG_CROSSFENCEDETECTION_INFO
{
	// 信息
	char				szRuleName[MAX_NAME_LEN];								// 规则名称,不同规则不能重名
	bool				bRuleEnable;											// 规则使能
	unsigned char       bTrackEnable;                                           // 触发跟踪使能,仅对绊线，入侵规则有效

	int					nDirection;												// 翻越围栏方向:0:由左至右;1:由右至左;2:两者都可以
	int					nDetectRegionPoint;										// 周界顶点数
	CFG_POINT2D   		stuDetectRegion[MAX_POLYLINE_NUM];						// 周界折线 顶点坐标

	int                 nTimeSection;                                           // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
	CFG_TIME_SECTION	stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
	CFG_TIME_SECTION	stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// 事件响应时间段 按每星期为周期

	//bool                bSizeFileter;                                         // 规则特定的尺寸过滤器是否有效
	//CFG_SIZEFILTER_INFO stuSizeFileter;                                       // 规则特定的尺寸过滤器

	int                 nTriggerPosition;                                       // 触发报警位置数
	unsigned char       bTriggerPosition[8];                                    // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心

	int					nTrackDuration;											// 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
	unsigned char       bReserved[20];                                          // 保留字段
	tagCFG_CROSSFENCEDETECTION_INFO() : bRuleEnable(true), bTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0) {};
} CFG_CROSSFENCEDETECTION_INFO;

// 1.3 人群密度检测结构体
//事件类型EVENT_IVS_DENSITYDETECTION(人群密度检测事件)对应的规则配置
typedef struct tagCFG_DENSITYDETECTION_INFO
{
	// 信息
	char                 szRuleName[MAX_NAME_LEN];                          // 规则名称,不同规则不能重名
	bool                 bRuleEnable;                                       // 规则使能
	unsigned char        byUnit;                                            // 密度检测数值单位GB/T-30147需要， 0 密度等级，划分为四个，很稀疏，稀疏，密集，很密集。1 密度百分比
	unsigned char        bSensitivity;                                      // 灵敏度(取值1-100，值越小灵敏度越低，对应人群的密集程度越高)
	unsigned char        byMinAlarmDensity;                                 // 最小报警密度 取值1-100,报警门限值百分比

	//int                  nObjectTypeNum;                                  // 相应物体类型个数 
	//char                 szObjectTypes[MAX_OBJECT_LIST_SIZE][MAX_NAME_LEN]; // 相应物体类型列表
	//int                  nMinDuration;                                    // 最短持续时间 单位：秒，0~65535

	int                  nDetectRegionPoint;                                // 检测区域顶点数
	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];                  // 检测区域

	int                  nTimeSection;                                      // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
	CFG_TIME_SECTION     stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];      // 事件响应时间段 按天
	CFG_TIME_SECTION	 stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	// 事件响应时间段 按星期 7天为单位

	int                  nGridCol;                                          // 画面内容分块信息 块的列数 若在3840x2160的画面中将画面等分为16列9行，那么检测返回的DESITYALERM_INFO里面statistical中vector的元素中的统计数值就是以240*240的小块分割后的统计数值
	int                  nGridRow;                                          // 画面内容分块信息 块的行数 此值为缺省值 最大行列数为画面大小
	tagCFG_DENSITYDETECTION_INFO() : bRuleEnable(true), byUnit(1), bSensitivity(50), nTimeSection(0), nGridCol(-1), nGridRow(-1) {};

} CFG_DENSITYDETECTION_INFO;

// 1.4 报警信息结构体
//返回报警信息 周界和绊线使用
typedef struct tagDetectAlerm_Info {
	std::string detectId;             //检测目标id
	std::string trackId;              //跟踪id
	int direct;                       //方向判断  对于周界来说就是 0:由外到里;1:由里到外 2:两者都可以; -1 不判断 
									  //对应绊线来说就是 0:由左到右; 1:从右到左 2 : 穿过; -1 不判断  绊线和周界判断压线的情况见下
	bool boundOrVertex;				  //绊线和周界压线情况 压线时是否认为穿过 或者认为完成从里到外 从左到右之类的动作 缺省为false
	int place;                        //位置判断 周界  0:外面; 1:里面 2:在线上; -1不判断 direct和place 不能同时使用 有且必须有一个为-1
	                                  // 绊线 0: 右边 1: 左边  2:在线上; -1不判断
	struct alertDetailInfo {          //报警检测信息结构体 坐标和时间戳
		uint64_t timestamp;           //报警检测时间戳
		float x;                      //报警检测坐标 x 中心点
		float y;                      //报警检测坐标 y 中心点
	};								 
	alertDetailInfo lastAlert;        //上一次位置和时间 当 place==-1 时 此项无意义 只使用当前数据 
	alertDetailInfo currentAlert;     //本次位置和时间 当判断方向时 laseAlert 和currentAlert的值都需要
	tagDetectAlerm_Info() : direct(-1), place(0), boundOrVertex(false) {};
} DETECTALERM_INFO;

//1.5 热力图报警结构体
//返回报警信息 人群聚集使用
typedef struct tagdensityAlerm_Info {
	unsigned long nDensityThreshold;  // 因人群密度结构体内未给出具体数值，此处可以设置一个具体的数值，可适应密度报警参数动态调整
	int nGridCol;					  // 画面内容分块信息 块的列数  若行数为16 列数为9 那么 map里面的std::vector<int> 共用16*9个元素 每个元素代表分割区域内的检测目标值, 如果是-1表示不需要密度信息
	int nGridRow;					  // 画面内容分块信息 块的行数  当设置的参数CFG_DENSITYDETECTION_INFO结构体与此结构体内的行列数值不一致时，以此结构体为准
	std::map<std::string, int> total; // 密度检测输出的画面内目标数量
	std::map<std::string, std::vector<float> > statistical;  //示例 [["car",[1,1,1,1,1,1,1 .....]],["truck",[1,1,1,1,1,2,3,3,3,....]],["person",[1,3,33,4,4,4,4,0,0,0,0,]]
} DESITYALERM_INFO;
