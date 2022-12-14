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
#define MAX_REC_TSECT_EX                        24              // 录像时间段扩展个数 按24小时计算

#define MAX_POLYGON_NUM                         256             // 视频分析设备区域顶点个数上限
#define MAX_CHANNELNAME_LEN                     64              // 最大通道名称长度
#define MAX_CHANNEL_COUNT                       36
#define MAX_VIDEO_CHANNEL_NUM                   256             // 最大通道数256
#define MAX_DBKEY_NUM                           64              // 数据库关键字最大值
#define MAX_SUMMARY_LEN                         1024            // 叠加到JPEG图片的摘要信息最大长度
#define MAX_CALIBRATEBOX_NUM                    10              // 智能分析校准框个数上限

#define MAX_GUARDTIME_NUM		MAX_REC_TSECT_EX
//windows
#ifndef MAX_PATH
#define MAX_PATH								260
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

typedef struct stru_Point3D
{
	double x;
	double y;
	double z;
	stru_Point3D() : x(0.0), y(0.0), z(0.0) {};
	stru_Point3D(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

} Point3D, CFG_POINT3D;

enum ALARM_ERROR_CODE {
	ERROR_NONE = 0,           //没有错误
	ERROR_PARAMS,             //输入参数错误
	ERROR_TYPE,               //输入类型错误
	ERROR_VALIDTIME,          //输入时间无效
	ERROR_NOIMPL = 0xFF       //功能没有实现
};

// Size
typedef struct tagCFG_SIZE
{
	union
	{
		float	fWidth;			// 宽
		float	fArea;			// 面积
	};
	float		fHeight;		// 高
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
	int	nRed;
	int	nGreen;
	int	nBlue;
	int	nAlpha;
} CFG_RGBA;

// 时间段信息
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

// 校准框信息
typedef struct tagCFG_CALIBRATEBOX_INFO
{
	CFG_POINT2D stuCenterPoint;     // 校准框中心点坐标(点的坐标归一化到[0,8191]区间)
	float       fRatio;             // 相对基准校准框的比率(比如1表示基准框大小，0.5表示基准框大小的一半)
} CFG_CALIBRATEBOX_INFO;

// 尺寸过滤器
typedef struct tagCFG_SIZEFILTER_INFO
{
	int                   nCalibrateBoxNum;                           // 校准框个数
	CFG_CALIBRATEBOX_INFO stuCalibrateBoxs[MAX_CALIBRATEBOX_NUM];     // 校准框(远端近端标定模式下有效)
	bool                  bMeasureModeEnable;                         // 计量方式参数是否有效
	unsigned char         bMeasureMode;                               // 计量方式,0-像素，不需要远端、近端标定, 1-实际长度，单位：米, 2-远端近端标定后的像素
	bool                  bFilterTypeEnable;                          // 过滤类型参数是否有效
																	  // ByArea,ByRatio仅作兼容，使用独立的ByArea和ByRatio选项代替 2012/03/06
	unsigned char		  bFilterType;			 				      // 过滤类型:0:"ByLength",1:"ByArea", 2"ByWidthHeight"
	unsigned char         bReserved[2];                               // 保留字段
	bool                  bFilterMinSizeEnable;                       // 物体最小尺寸参数是否有效
	bool                  bFilterMaxSizeEnable;                       // 物体最大尺寸参数是否有效
	CFG_SIZE			  stuFilterMinSize;						      // 物体最小尺寸 "ByLength"模式下表示宽高的尺寸，"ByArea"模式下宽表示面积，高无效(远端近端标定模式下表示基准框的宽高尺寸)。
	CFG_SIZE			  stuFilterMaxSize;				              // 物体最大尺寸 "ByLength"模式下表示宽高的尺寸，"ByArea"模式下宽表示面积，高无效(远端近端标定模式下表示基准框的宽高尺寸)。

	bool                  abByArea;
	bool                  abMinArea;
	bool                  abMaxArea;
	bool                  abMinAreaSize;
	bool                  abMaxAreaSize;
	bool                  bByArea;                                    // 是否按面积过滤 通过能力ComplexSizeFilter判断是否可用
	bool                  bReserved1[2];
	float                 nMinArea;                                   // 最小面积
	float                 nMaxArea;                                   // 最大面积
	CFG_SIZE              stuMinAreaSize;                             // 最小面积矩形框尺寸 "计量方式"为"像素"时，表示最小面积矩形框的宽高尺寸；"计量方式"为"远端近端标定模式"时，表示基准框的最小宽高尺寸；
	CFG_SIZE              stuMaxAreaSize;                             // 最大面积矩形框尺寸, 同上

	bool                  abByRatio;
	bool                  abMinRatio;
	bool                  abMaxRatio;
	bool                  abMinRatioSize;
	bool                  abMaxRatioSize;
	bool                  bByRatio;                                   // 是否按宽高比过滤 通过能力ComplexSizeFilter判断是否可用
	bool                  bReserved2[2];
	double                dMinRatio;                                  // 最小宽高比
	double                dMaxRatio;                                  // 最大宽高比
	CFG_SIZE              stuMinRatioSize;                            // 最小宽高比矩形框尺寸，最小宽高比对应矩形框的宽高尺寸。
	CFG_SIZE              stuMaxRatioSize;                            // 最大宽高比矩形框尺寸，同上

	int                   nAreaCalibrateBoxNum;                       // 面积校准框个数
	CFG_CALIBRATEBOX_INFO stuAreaCalibrateBoxs[MAX_CALIBRATEBOX_NUM]; // 面积校准框
	int                   nRatioCalibrateBoxs;                        // 宽高校准框个数
	CFG_CALIBRATEBOX_INFO stuRatioCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// 宽高校准框

	bool                  blBySizeEnable;                             // 长宽过滤使能参数是否有效
	bool                  blBySize;                                   // 长宽过滤使能
}CFG_SIZEFILTER_INFO;

//CrossLine 绊线 CrossRegion 周界 Density 物体密度  [v1.3] ParkingRestrictedStop禁停区域停车 ViolationTrafficLine 轧线行驶 ExcessiveSpeed 超速 WrongSide逆行 FireAlarm
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

//布防时间段  
typedef struct tagCFG_TIMEGUARD_INFO
{
	int				 guardDateTimeId;				// id	   
	int				 guardDateTimeType;				// 0不启用 1 手动 2按星期每周 3按月份每天 现在只保留一个数组 节省内存
													// 0不判断时间 1一次性的 2判断 stuDayTimeSection 3 判断 stuWeekTimeSection 
	CFG_TIME_SECTION stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
																								//废弃 CFG_TIME_SECTION stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// 事件响应时间段 按每星期为周期
	tagCFG_TIMEGUARD_INFO() : guardDateTimeId(-1), guardDateTimeType(0) {};
} CFG_TIMEGUARD_INFO;

struct alertDetailInfo {              //报警检测信息结构体 坐标和时间戳
	int64_t timestamp;                //报警检测时间戳
	float fx;                         //报警检测坐标 x 中心点
	float fy;                         //报警检测坐标 y 中心点
	float fw;
	float fh;
	float ftx;                        //报警规则标记坐标 x
	float fty;                        //报警规则标记坐标 y
									  //v1.8后新增
	float refx;
	float refy;
	float refw;
	float refh;
	float reftx;                        //报警规则标记坐标 x 全局
	float refty;                        //报警规则标记坐标 y

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

//### 1.1 拌线规则结构体
typedef struct tagCFG_CROSSLINE_INFO
{
	// 信息
	int				    ruleId;
	char			    szRuleName[MAX_NAME_LEN];		  // 规则名称,不同规则不能重名
	bool			    blRuleEnable;					  // 规则使能
	bool		        blTrackEnable;                    // 触发跟踪使能,仅对绊线，入侵规则有效
	int				    nDirection;						  // 检测方向:当ruleType类型为 CrossLine为绊线 时 检测方向 0:由左至右;1:由右至左;2:两者都可以
														  // 当ruleType类型为 CrossRegion为周界 时 检测方向 0:由外到里;1:由里到外 2:两者都可以
														  // 当ruleType类型为 Density时 此项无效
	int				    nPlace;
	float			    fDistance;						  // 警戒线报警距离  
	int				    nDetectLinePoint;				  // 警戒线/绊线顶点数
	CFG_POINT2D		    stuDetectLine[MAX_POLYLINE_NUM];  // 警戒线/绊线顶点坐标 原则上不封闭 不能交叉！

	int                 nTimeSection;                     // 布防时间数量
	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	  // 布防时间
														  //int               nTimeSection;                     // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
														  //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
														  //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	// 事件响应时间段 按每星期为周期

														  //bool              bSizeFileter;                     // 规则特定的尺寸过滤器是否有效
														  //CFG_SIZEFILTER_INFO stuSizeFileter;                 // 规则特定的尺寸过滤器

	int                 nTriggerPosition;                 // 触发报警位置数
	unsigned char       byTriggerPosition[8];             // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
	int				    nTrackDuration;				      // 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
	unsigned char       byReserved[20];                   // 保留字段 
	tagCFG_CROSSLINE_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0), nDirection(0), nPlace(0), fDistance(0.0f) {};
} CFG_CROSSLINE_INFO;

//### 1.2 周界规则结构体
//事件类型EVENT_IVS_CROSSFENCEDETECTION(翻越围栏规则)对应的规则配置
typedef struct tagCFG_CROSSFENCEDETECTION_INFO
{
	// 信息
	int				    ruleId;
	char			    szRuleName[MAX_NAME_LEN];				 // 规则名称,不同规则不能重名
	bool			    blRuleEnable;							 // 规则使能
	bool			    blTrackEnable;                           // 触发跟踪使能,仅对绊线，入侵规则有效
	int				    nDirection;							     // 翻越围栏方向:0:由左至右;1:由右至左;2:两者都可以
	int				    nPlace;
	int				    nDetectRegionPoint;					     // 周界顶点数
	CFG_POINT2D   	    stuDetectRegion[MAX_POLYLINE_NUM];		 // 周界折线 顶点坐标
	int                 nTimeSection;                            // 布防时间数量
	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	         // 布防时间
																 //bool			    guardTimeEnabled;						 // 启用时间
																 //int               nTimeSection;                            // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection 3 一次性的 使用stuDayTimeSection
																 //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
																 //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// 事件响应时间段 按每星期为周期

																 //bool              bSizeFileter;                            // 规则特定的尺寸过滤器是否有效
																 //CFG_SIZEFILTER_   INFO stuSizeFileter;                     // 规则特定的尺寸过滤器
	int                 nTriggerPosition;                        // 触发报警位置数
	unsigned char       byTriggerPosition[8];                    // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
	int				    nTrackDuration;						     // 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
	unsigned char       byReserved[20];                          // 保留字段
	tagCFG_CROSSFENCEDETECTION_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTriggerPosition(0), nTrackDuration(0), nTimeSection(0), nDetectRegionPoint(0), nDirection(0), nPlace(0) {};
} CFG_CROSSFENCEDETECTION_INFO;

// 1.3 人群密度检测结构体
//事件类型EVENT_IVS_DENSITYDETECTION(人群密度检测事件)对应的规则配置
typedef struct tagCFG_DENSITYDETECTION_INFO
{
	// 信息
	int				     ruleId;
	char                 szRuleName[MAX_NAME_LEN];          // 规则名称,不同规则不能重名
	bool                 blRuleEnable;                      // 规则使能
	bool		         blTrackEnable;                     // 触发跟踪使能, 仅对绊线，入侵规则有效
	char                 szObjectTypes[MAX_NAME_LEN];		// 相应物体类型
	unsigned char        byUnit;                            // 密度检测报警方式 : byUnit==0 密度等级 数值单位GB/T-30147，划分为四个，很稀疏，稀疏，密集，很密集。
															//1)很稀疏:目标覆盖面积占检测区域面积的0~25%,
															//2)稀疏:目标覆盖面积占检测区域面积的26%~50 %,
															//3)密集:目标覆盖面积占检测区域面积的51%~75 %,
															//4)很密集:目标覆盖面积占检测区域面积的76%~100%  ; nThresholdNum 代表等级 1，2，3，4
															//byUnit==1 密度百分比 nThresholdNum 此时代表百分比 1-100 ; byUnit==2 阈值  使用nThresholdNum 此时代表个数 >0

															//unsigned char      bySensitivity;                     // 灵敏度(取值1-100，值越小灵敏度越低，对应人群的密集程度越高)
															//unsigned char      byMinAlarmDensity;                 // 最小报警密度 取值1-100,报警门限值百分比 当区域内的密度值超过全局面积内的百分比报警 0时不使用

	int                  nThresholdNum;                     // 报警阈值 nThresholdNum 代表等级 1，2，3，4 byUnit==1 密度百分比 nThresholdNum 此时代表百分比 1-100 ; byUnit==2 阈值  使用nThresholdNum 此时代表个数 >0
															//int                  nTimePercent;			        // 持续单位时间内百分比 0~100 如设置为80 结合nMinDuration=10 那么就是10秒内有8秒 人数达到报警阈值 -1时不使用持续时间 只考虑时间间隔参数  报警完了
	int                  nMinDuration;                      // 最短持续时间 单位：秒，0~65535； 0 每次都报警 > 0 就是间隔时间 

	int                  nDetectRegionPoint;                // 检测区域顶点数
	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // 检测区域
	int                  nTimeSection;                      // 布防时间数量
	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // 布防时间

															//int                nTimeSection;                      // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
															//CFG_TIME_SECTION   stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];        // 事件响应时间段 按天
															//CFG_TIME_SECTION   stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	 // 事件响应时间段 按星期 7天为单位

	int                  nWidth;                            // 画面内容分块信息 块的列数 若在3840x2160的画面中将画面等分为16列9行，那么检测返回的DESITYALARM_INFO里面statistical中vector的元素中的统计数值就是以240*240的小块分割后的统计数值
	int                  nHeight;                           // 画面内容分块信息 块的行数 此值为缺省值 最大行列数为画面大小

	unsigned char        byReserved[20];                    // 保留字段 

	tagCFG_DENSITYDETECTION_INFO() : ruleId(-1), blRuleEnable(true), byUnit(1), nThresholdNum(50), nMinDuration(80), nWidth(-1), nHeight(-1), nTimeSection(0), nDetectRegionPoint(0) {};
} CFG_DENSITYDETECTION_INFO;

// 1.4 交通违章检测
//事件类型EVENT_IVS_DENSITYDETECTION(人群密度检测事件)对应的规则配置
typedef struct tagCFG_TRAFFICVIOLATIONS_INFO
{
	// 信息
	int				     ruleId;
	char                 szRuleName[MAX_NAME_LEN];          // 规则名称,不同规则不能重名
	bool                 blRuleEnable;                      // 规则使能
	bool		         blTrackEnable;                     // 触发跟踪使能, 仅对绊线，入侵规则有效
	char                 szObjectTypes[MAX_NAME_LEN];		// 相应物体类型
	Alarm_RuleType       ruleType;                          // 预警规则类型 ParkingRestrictedStop禁停区域停车 ViolationTrafficLine 轧线行驶 ExcessiveSpeed 超速 WrongSide逆行 FireAlarm
	int				     nDirection;	  			        // 翻越围栏方向:0:由左至右;1:由右至左;2:两者都可以
	int				     nPlace;
	float                fSpeedLimit;                       // 超速值  一般根据国标要求 超速10%触发报警 因允许测量设备有10%的误差
	int                  nMinDuration;                      // 最短持续时间 单位：秒，0~65535； 0 每次都报警 > 0 就是间隔时间 

	int                  nDetectRegionPoint;                // 检测区域顶点数
	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // 检测区域
	int                  nTimeSection;                      // 布防时间数量
	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // 布防时间

	unsigned char        byReserved[20];                    // 保留字段

	tagCFG_TRAFFICVIOLATIONS_INFO() : ruleId(-1), blRuleEnable(true), nDetectRegionPoint(0), nMinDuration(80), fSpeedLimit(0.0f), ruleType(ZH_ALARM_UNKNOWN) {};
} CFG_TRAFFICVIOLATIONS_INFO;

// 1.5 报警信息结构体
//返回报警信息 周界和绊线使用
typedef struct tagDetectAlarm_Info {
	std::string 	    sceneId;		  //报警后 报警的场景
	std::string         detectId;         //检测目标id
	std::string         detectImage;	  //检测目标图像 base64
	std::string         trackId;          //跟踪id
	std::string			klass;			  //目标类型
	std::string			category;		  //目标大类 
	bool 		        blUseRule;		  //使用设定规则检查 true 不使用false
	int                 nDirect;          //方向判断  对于周界来说就是 0:由外到里;1:由里到外 2:两者都可以; -1 不判断 
										  //对应绊线来说就是 0:由左到右; 1:从右到左 2 : 穿过; -1 不判断  绊线和周界判断压线的情况见下
	int                 nPlace;           //位置判断 周界  0:外面; 1:里面 2:在线上; -1不判断 direct和place 不能同时使用 有且必须有一个为-1
										  // 绊线 0: 右边 1: 左边  2:在线上; -1不判断

	float 		        fDistance;		  //检测目标到周界、绊线得垂直距离 缺省为0 不为0时进入到绊线范围一定距离内就报警
	bool		        blBoundOrVertex;  //绊线和周界压线情况 压线时是否认为穿过 或者认为完成从里到外 从左到右之类的动作 缺省为false
	float               fSpeed;           //目标速度

	alertDetailInfo     lastAlert;        //上一次位置和时间 当 place==-1 时 此项无意义 只使用当前数据 
	alertDetailInfo     currentAlert;     //本次位置和时间 当判断方向时 laseAlert 和currentAlert的值都需要
	int 		  	    nRuleId;		  //报警后是否需要调整球机位置
	int			        nAlarmType;	      //报警类型
	bool 			    blDomeTrack;	  //报警后是否需要调整球机位置
	int 			    nDeviceId;		  //报警后需要调整球机位置的 指定球机id 如果-1表示随机

	void*				pUser1;			  // 多目标ObjectState结构体指针  currentAlert使用 不为零时需要自己销毁 为0时需要自己计算方位角和gps信息
	void*				pUser2;			  //
	unsigned char       byReserved[20];   //保留字段  

	tagDetectAlarm_Info() : blUseRule(true), nDirect(-1), nPlace(0), blBoundOrVertex(false), fDistance(0.0f), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
} DETECTALARM_INFO;

//1.6 热力图报警结构体
//返回报警信息 人群聚集使用
typedef struct tagDensityAlarm_Info {
	//前面为输入参数
	std::string			sceneId;
	int64_t				timestamp;			// 服务器Unix时间戳，以毫秒为单位
	unsigned long		nDensityThreshold;  // 因人群密度结构体内未给出具体数值，此处可以设置一个具体的数值，可适应密度报警参数动态调整
	int					nWidth;				// 画面内容分块信息 块的列数  若行数为16 列数为9 那么 map里面的std::vector<int> 共用16*9个元素 每个元素代表分割区域内的检测目标值, 如果是-1表示不需要密度信息
	int					nHeight;			// 画面内容分块信息 块的行数  当设置的参数CFG_DENSITYDETECTION_INFO结构体与此结构体内的行列数值不一致时，以此结构体为准
	std::string			klass;			    // 目标类型
	std::string			category;		    // 目标大类 
	std::string			image;				// 目标图像 base64
	float				fTotal;				// 密度检测输出的画面内目标数量
	std::vector<float>  vStatistical;		// 示例 [1,1,1,1,1,1,1 .....]
											// 下面为输出参数
	int					nRuleId;			// 报警后写入  报警规则id

	alertDetailInfo     currentAlert;
	float				fAlarmTotal;		//报警区域内的数量 或百分比
	int					nAlarmType;			//报警类型
	bool				blDomeTrack;		//报警后是否需要调整球机位置
	int					nDeviceId;			//报警后需要调整球机位置的 指定球机id 如果-1表示随机

	void*				pUser1;			  // 多目标ObjectState结构体指针  currentAlert使用 不为零时需要自己销毁 为0时需要自己计算方位角和gps信息
	void*				pUser2;			  //
	unsigned char       byReserved[20];   //保留字段  
	tagDensityAlarm_Info() : nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
} DENSITYALARM_INFO;

// 1.7 交通类报警信息结构体
typedef struct tagTrafficAlarm_Info {
	std::string 	    sceneId;		  //报警后 报警的场景
	std::string         detectId;         //检测目标id
	std::string         detectImage;	  //检测目标图像 base64
	std::string         trackId;          //跟踪id
	std::string			klass;			  //目标类型
	std::string			category;		  //目标大类 
	bool 		        blUseRule;		  //使用设定规则检查 true 不使用false
	int                 ruleType;         //预警规则类型 ParkingRestrictedStop禁停区域停车 ViolationTrafficLine 轧线行驶 ExcessiveSpeed 超速 WrongSide逆行 FireAlarm
	int                 nDirect;          //方向判断  对于周界来说就是 0:由外到里;1:由里到外 2:两者都可以; -1 不判断 
										  //对应绊线来说就是 0:由左到右; 1:从右到左 2 : 穿过; -1 不判断  绊线和周界判断压线的情况见下
	int                 nPlace;           //位置判断 周界  0:外面; 1:里面 2:在线上; -1不判断 direct和place 不能同时使用 有且必须有一个为-1
										  // 绊线 0: 右边
	float               fSpeedLimit;      // 超速值  一般根据国标要求 超速10%触发报警 因允许测量设备有10%的误差
	int                 nMinDuration;     // 最短持续时间 单位：秒，0~65535； 0 每次都报警 > 0 就是间隔时间 

	alertDetailInfo     lastAlert;        //上一次位置和时间 当 place==-1 时 此项无意义 只使用当前数据 
	alertDetailInfo     currentAlert;     //本次位置和时间 当判断方向时 laseAlert 和currentAlert的值都需要
	int 		  	    nRuleId;		  //报警后是否需要调整球机位置
	int			        nAlarmType;	      //报警类型
	float               fSpeed;           //目标速度

	bool 			    blDomeTrack;	  //报警后是否需要调整球机位置
	int 			    nDeviceId;		  //报警后需要调整球机位置的 指定球机id 如果-1表示随机

	void*				pUser1;			  // 多目标ObjectState结构体指针  currentAlert使用 不为零时需要自己销毁 为0时需要自己计算方位角和gps信息
	void*				pUser2;			  //
	unsigned char       byReserved[20];   //保留字段  

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
//#define MAX_NAME_LEN                            128             // 通用名字字符串长度
//#define MAX_OBJECT_LIST_SIZE                    256             // 视频分析设备支持的检测物体类型列表个数上限
//#define MAX_POLYLINE_NUM                        20              // 视频分析设备折线顶点个数上限
//#define MAX_DAY_NUM                             31              // 一月的天数
//#define MAX_WEEK_DAY_NUM						7				// 每周的天数
//#define MAX_MONTH_NUM                           12              // 一年12个月
//#define MAX_REC_TSECT_EX                        24              // 录像时间段扩展个数 按24小时计算
//
//#define MAX_POLYGON_NUM                         256             // 视频分析设备区域顶点个数上限
//#define MAX_CHANNELNAME_LEN                     64              // 最大通道名称长度
//#define MAX_CHANNEL_COUNT                       36
//#define MAX_VIDEO_CHANNEL_NUM                   256             // 最大通道数256
//#define MAX_DBKEY_NUM                           64              // 数据库关键字最大值
//#define MAX_SUMMARY_LEN                         1024            // 叠加到JPEG图片的摘要信息最大长度
//#define MAX_CALIBRATEBOX_NUM                    10              // 智能分析校准框个数上限
//
//#define MAX_GUARDTIME_NUM		MAX_REC_TSECT_EX
////windows
//#ifndef MAX_PATH
//#define MAX_PATH								255
//#endif // !
//
////double 精度判断
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
//	ERROR_NONE = 0,           //没有错误
//	ERROR_PARAMS,             //输入参数错误
//	ERROR_TYPE,               //输入类型错误
//	ERROR_VALIDTIME,          //输入时间无效
//	ERROR_NOIMPL = 0xFF       //功能没有实现
//};
//
//// Size
//typedef struct tagCFG_SIZE
//{
//	union
//	{
//		float	fWidth;			// 宽
//		float	fArea;			// 面积
//	};
//	float		fHeight;		// 高
//} CFG_SIZE;
//
//// 区域信息
//typedef struct tagCFG_REGION
//{
//	int         nPointNum;
//	CFG_POINT2D stuPolygon[MAX_POLYGON_NUM];
//}CFG_REGION;
//
//// RGBA信息
//typedef struct tagCFG_RGBA
//{
//	int	nRed;
//	int	nGreen;
//	int	nBlue;
//	int	nAlpha;
//} CFG_RGBA;
//
//// 时间段信息
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
//// 校准框信息
//typedef struct tagCFG_CALIBRATEBOX_INFO
//{
//	CFG_POINT2D stuCenterPoint;     // 校准框中心点坐标(点的坐标归一化到[0,8191]区间)
//	float       fRatio;             // 相对基准校准框的比率(比如1表示基准框大小，0.5表示基准框大小的一半)
//} CFG_CALIBRATEBOX_INFO;
//
//// 尺寸过滤器
//typedef struct tagCFG_SIZEFILTER_INFO
//{
//	int                   nCalibrateBoxNum;                           // 校准框个数
//	CFG_CALIBRATEBOX_INFO stuCalibrateBoxs[MAX_CALIBRATEBOX_NUM];     // 校准框(远端近端标定模式下有效)
//	bool                  bMeasureModeEnable;                         // 计量方式参数是否有效
//	unsigned char         bMeasureMode;                               // 计量方式,0-像素，不需要远端、近端标定, 1-实际长度，单位：米, 2-远端近端标定后的像素
//	bool                  bFilterTypeEnable;                          // 过滤类型参数是否有效
//																	  // ByArea,ByRatio仅作兼容，使用独立的ByArea和ByRatio选项代替 2012/03/06
//	unsigned char		  bFilterType;			 				      // 过滤类型:0:"ByLength",1:"ByArea", 2"ByWidthHeight"
//	unsigned char         bReserved[2];                               // 保留字段
//	bool                  bFilterMinSizeEnable;                       // 物体最小尺寸参数是否有效
//	bool                  bFilterMaxSizeEnable;                       // 物体最大尺寸参数是否有效
//	CFG_SIZE			  stuFilterMinSize;						      // 物体最小尺寸 "ByLength"模式下表示宽高的尺寸，"ByArea"模式下宽表示面积，高无效(远端近端标定模式下表示基准框的宽高尺寸)。
//	CFG_SIZE			  stuFilterMaxSize;				              // 物体最大尺寸 "ByLength"模式下表示宽高的尺寸，"ByArea"模式下宽表示面积，高无效(远端近端标定模式下表示基准框的宽高尺寸)。
//
//	bool                  abByArea;
//	bool                  abMinArea;
//	bool                  abMaxArea;
//	bool                  abMinAreaSize;
//	bool                  abMaxAreaSize;
//	bool                  bByArea;                                    // 是否按面积过滤 通过能力ComplexSizeFilter判断是否可用
//	bool                  bReserved1[2];
//	float                 nMinArea;                                   // 最小面积
//	float                 nMaxArea;                                   // 最大面积
//	CFG_SIZE              stuMinAreaSize;                             // 最小面积矩形框尺寸 "计量方式"为"像素"时，表示最小面积矩形框的宽高尺寸；"计量方式"为"远端近端标定模式"时，表示基准框的最小宽高尺寸；
//	CFG_SIZE              stuMaxAreaSize;                             // 最大面积矩形框尺寸, 同上
//
//	bool                  abByRatio;
//	bool                  abMinRatio;
//	bool                  abMaxRatio;
//	bool                  abMinRatioSize;
//	bool                  abMaxRatioSize;
//	bool                  bByRatio;                                   // 是否按宽高比过滤 通过能力ComplexSizeFilter判断是否可用
//	bool                  bReserved2[2];
//	double                dMinRatio;                                  // 最小宽高比
//	double                dMaxRatio;                                  // 最大宽高比
//	CFG_SIZE              stuMinRatioSize;                            // 最小宽高比矩形框尺寸，最小宽高比对应矩形框的宽高尺寸。
//	CFG_SIZE              stuMaxRatioSize;                            // 最大宽高比矩形框尺寸，同上
//
//	int                   nAreaCalibrateBoxNum;                       // 面积校准框个数
//	CFG_CALIBRATEBOX_INFO stuAreaCalibrateBoxs[MAX_CALIBRATEBOX_NUM]; // 面积校准框
//	int                   nRatioCalibrateBoxs;                        // 宽高校准框个数
//	CFG_CALIBRATEBOX_INFO stuRatioCalibrateBoxs[MAX_CALIBRATEBOX_NUM];// 宽高校准框
//
//	bool                  blBySizeEnable;                             // 长宽过滤使能参数是否有效
//	bool                  blBySize;                                   // 长宽过滤使能
//}CFG_SIZEFILTER_INFO;
//
////CrossLine 绊线 CrossRegion 周界 Density 物体密度  [v1.3] ParkingRestrictedStop禁停区域停车 ViolationTrafficLine 轧线行驶 ExcessiveSpeed 超速 WrongSide逆行 FireAlarm
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
////布防时间段  
//typedef struct tagCFG_TIMEGUARD_INFO
//{
//	int				 guardDateTimeId;				// id	   
//	int				 guardDateTimeType;				// 0不启用 1 手动 2按星期每周 3按月份每天 现在只保留一个数组 节省内存
//													// 0不判断时间 1一次性的 2判断 stuDayTimeSection 3 判断 stuWeekTimeSection 
//	CFG_TIME_SECTION stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
//																								//废弃 CFG_TIME_SECTION stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// 事件响应时间段 按每星期为周期
//	tagCFG_TIMEGUARD_INFO() : guardDateTimeId(-1), guardDateTimeType(0) {};
//} CFG_TIMEGUARD_INFO;
//
//struct alertDetailInfo {              //报警检测信息结构体 坐标和时间戳
//	int64_t timestamp;                //报警检测时间戳
//	float fx;                         //报警检测坐标 x 中心点
//	float fy;                         //报警检测坐标 y 中心点
//	float fw;
//	float fh;
//	float ftx;                        //报警规则标记坐标 x
//	float fty;                        //报警规则标记坐标 y
//									  //v1.8后新增
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
////### 1.1 拌线规则结构体
//typedef struct tagCFG_CROSSLINE_INFO
//{
//	// 信息
//	int				    ruleId;
//	char			    szRuleName[MAX_NAME_LEN];		  // 规则名称,不同规则不能重名
//	bool			    blRuleEnable;					  // 规则使能
//	bool		        blTrackEnable;                    // 触发跟踪使能,仅对绊线，入侵规则有效
//	int				    nDirection;						  // 检测方向:当ruleType类型为 CrossLine为绊线 时 检测方向 0:由左至右;1:由右至左;2:两者都可以
//														  // 当ruleType类型为 CrossRegion为周界 时 检测方向 0:由外到里;1:由里到外 2:两者都可以
//														  // 当ruleType类型为 Density时 此项无效
//	int				    nPlace;
//	float			    fDistance;						  // 警戒线报警距离  
//	int				    nDetectLinePoint;				  // 警戒线/绊线顶点数
//	CFG_POINT2D		    stuDetectLine[MAX_POLYLINE_NUM];  // 警戒线/绊线顶点坐标 原则上不封闭 不能交叉！
//
//	int                 nTimeSection;                     // 布防时间数量
//	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	  // 布防时间
//														  //int               nTimeSection;                     // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
//														  //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
//														  //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	// 事件响应时间段 按每星期为周期
//
//														  //bool              bSizeFileter;                     // 规则特定的尺寸过滤器是否有效
//														  //CFG_SIZEFILTER_INFO stuSizeFileter;                 // 规则特定的尺寸过滤器
//
//	int                 nTriggerPosition;                 // 触发报警位置数
//	unsigned char       byTriggerPosition[8];             // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
//	int				    nTrackDuration;				      // 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
//	unsigned char       byReserved[20];                   // 保留字段 
//	tagCFG_CROSSLINE_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0), nDirection(0), nPlace(0), fDistance(0.0f) {};
//} CFG_CROSSLINE_INFO;
//
////### 1.2 周界规则结构体
////事件类型EVENT_IVS_CROSSFENCEDETECTION(翻越围栏规则)对应的规则配置
//typedef struct tagCFG_CROSSFENCEDETECTION_INFO
//{
//	// 信息
//	int				    ruleId;
//	char			    szRuleName[MAX_NAME_LEN];				 // 规则名称,不同规则不能重名
//	bool			    blRuleEnable;							 // 规则使能
//	bool			    blTrackEnable;                           // 触发跟踪使能,仅对绊线，入侵规则有效
//	int				    nDirection;							     // 翻越围栏方向:0:由左至右;1:由右至左;2:两者都可以
//	int				    nPlace;
//	int				    nDetectRegionPoint;					     // 周界顶点数
//	CFG_POINT2D   	    stuDetectRegion[MAX_POLYLINE_NUM];		 // 周界折线 顶点坐标
//	int                 nTimeSection;                            // 布防时间数量
//	CFG_TIMEGUARD_INFO  guardTimes[MAX_GUARDTIME_NUM];	         // 布防时间
//																 //bool			    guardTimeEnabled;						 // 启用时间
//																 //int               nTimeSection;                            // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection 3 一次性的 使用stuDayTimeSection
//																 //CFG_TIME_SECTION  stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 按每月为周期
//																 //CFG_TIME_SECTION  stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];		// 事件响应时间段 按每星期为周期
//
//																 //bool              bSizeFileter;                            // 规则特定的尺寸过滤器是否有效
//																 //CFG_SIZEFILTER_   INFO stuSizeFileter;                     // 规则特定的尺寸过滤器
//	int                 nTriggerPosition;                        // 触发报警位置数
//	unsigned char       byTriggerPosition[8];                    // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
//	int				    nTrackDuration;						     // 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
//	unsigned char       byReserved[20];                          // 保留字段
//	tagCFG_CROSSFENCEDETECTION_INFO() : ruleId(-1), blRuleEnable(true), blTrackEnable(false), nTriggerPosition(0), nTrackDuration(0), nTimeSection(0), nDetectRegionPoint(0), nDirection(0), nPlace(0) {};
//} CFG_CROSSFENCEDETECTION_INFO;
//
//// 1.3 人群密度检测结构体
////事件类型EVENT_IVS_DENSITYDETECTION(人群密度检测事件)对应的规则配置
//typedef struct tagCFG_DENSITYDETECTION_INFO
//{
//	// 信息
//	int				     ruleId;
//	char                 szRuleName[MAX_NAME_LEN];          // 规则名称,不同规则不能重名
//	bool                 blRuleEnable;                      // 规则使能
//	bool		         blTrackEnable;                     // 触发跟踪使能, 仅对绊线，入侵规则有效
//	char                 szObjectTypes[MAX_NAME_LEN];		// 相应物体类型
//	unsigned char        byUnit;                            // 密度检测报警方式 : byUnit==0 密度等级 数值单位GB/T-30147，划分为四个，很稀疏，稀疏，密集，很密集。
//															//1)很稀疏:目标覆盖面积占检测区域面积的0~25%,
//															//2)稀疏:目标覆盖面积占检测区域面积的26%~50 %,
//															//3)密集:目标覆盖面积占检测区域面积的51%~75 %,
//															//4)很密集:目标覆盖面积占检测区域面积的76%~100%  ; nThresholdNum 代表等级 1，2，3，4
//															//byUnit==1 密度百分比 nThresholdNum 此时代表百分比 1-100 ; byUnit==2 阈值  使用nThresholdNum 此时代表个数 >0
//
//															//unsigned char      bySensitivity;                     // 灵敏度(取值1-100，值越小灵敏度越低，对应人群的密集程度越高)
//															//unsigned char      byMinAlarmDensity;                 // 最小报警密度 取值1-100,报警门限值百分比 当区域内的密度值超过全局面积内的百分比报警 0时不使用
//
//	int                  nThresholdNum;                     // 报警阈值 nThresholdNum 代表等级 1，2，3，4 byUnit==1 密度百分比 nThresholdNum 此时代表百分比 1-100 ; byUnit==2 阈值  使用nThresholdNum 此时代表个数 >0
//															//int                  nTimePercent;			        // 持续单位时间内百分比 0~100 如设置为80 结合nMinDuration=10 那么就是10秒内有8秒 人数达到报警阈值 -1时不使用持续时间 只考虑时间间隔参数  报警完了
//	int                  nMinDuration;                      // 最短持续时间 单位：秒，0~65535； 0 每次都报警 > 0 就是间隔时间 
//
//	int                  nDetectRegionPoint;                // 检测区域顶点数
//	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // 检测区域
//	int                  nTimeSection;                      // 布防时间数量
//	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // 布防时间
//
//															//int                nTimeSection;                      // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
//															//CFG_TIME_SECTION   stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];        // 事件响应时间段 按天
//															//CFG_TIME_SECTION   stuWeekTimeSection[MAX_MONTH_NUM][MAX_WEEK_DAY_NUM][MAX_REC_TSECT_EX];	 // 事件响应时间段 按星期 7天为单位
//
//	int                  nWidth;                            // 画面内容分块信息 块的列数 若在3840x2160的画面中将画面等分为16列9行，那么检测返回的DESITYALARM_INFO里面statistical中vector的元素中的统计数值就是以240*240的小块分割后的统计数值
//	int                  nHeight;                           // 画面内容分块信息 块的行数 此值为缺省值 最大行列数为画面大小
//
//	unsigned char        byReserved[20];                    // 保留字段 
//
//	tagCFG_DENSITYDETECTION_INFO() : ruleId(-1), blRuleEnable(true), byUnit(1), nThresholdNum(50), nMinDuration(80), nWidth(-1), nHeight(-1), nTimeSection(0), nDetectRegionPoint(0) {};
//} CFG_DENSITYDETECTION_INFO;
//
//// 1.4 交通违章检测
////事件类型EVENT_IVS_DENSITYDETECTION(人群密度检测事件)对应的规则配置
//typedef struct tagCFG_TRAFFICVIOLATIONS_INFO
//{
//	// 信息
//	int				     ruleId;
//	char                 szRuleName[MAX_NAME_LEN];          // 规则名称,不同规则不能重名
//	bool                 blRuleEnable;                      // 规则使能
//	bool		         blTrackEnable;                     // 触发跟踪使能, 仅对绊线，入侵规则有效
//	char                 szObjectTypes[MAX_NAME_LEN];		// 相应物体类型
//	Alarm_RuleType       ruleType;                          // 预警规则类型 ParkingRestrictedStop禁停区域停车 ViolationTrafficLine 轧线行驶 ExcessiveSpeed 超速 WrongSide逆行 FireAlarm
//	int				     nDirection;	  			        // 翻越围栏方向:0:由左至右;1:由右至左;2:两者都可以
//	int				     nPlace;
//	float                fSpeedLimit;                       // 超速值  一般根据国标要求 超速10%触发报警 因允许测量设备有10%的误差
//	int                  nMinDuration;                      // 最短持续时间 单位：秒，0~65535； 0 每次都报警 > 0 就是间隔时间 
//
//	int                  nDetectRegionPoint;                // 检测区域顶点数
//	CFG_POINT2D          stuDetectRegion[MAX_POLYGON_NUM];  // 检测区域
//	int                  nTimeSection;                      // 布防时间数量
//	CFG_TIMEGUARD_INFO   guardTimes[MAX_GUARDTIME_NUM];	    // 布防时间
//
//	unsigned char        byReserved[20];                    // 保留字段
//
//	tagCFG_TRAFFICVIOLATIONS_INFO() : ruleId(-1), blRuleEnable(true), nDetectRegionPoint(0), nMinDuration(80), fSpeedLimit(0.0f), ruleType(ZH_ALARM_UNKNOWN) {};
//} CFG_TRAFFICVIOLATIONS_INFO;
//
//// 1.5 报警信息结构体
////返回报警信息 周界和绊线使用
//typedef struct tagDetectAlarm_Info {
//	std::string 	    sceneId;		  //报警后 报警的场景
//	std::string         detectId;         //检测目标id
//	std::string         detectImage;	  //检测目标图像 base64
//	std::string         trackId;          //跟踪id
//	std::string			klass;			  //目标类型
//	std::string			category;		  //目标大类 
//	bool 		        blUseRule;		  //使用设定规则检查 true 不使用false
//	int                 nDirect;          //方向判断  对于周界来说就是 0:由外到里;1:由里到外 2:两者都可以; -1 不判断 
//										  //对应绊线来说就是 0:由左到右; 1:从右到左 2 : 穿过; -1 不判断  绊线和周界判断压线的情况见下
//	int                 nPlace;           //位置判断 周界  0:外面; 1:里面 2:在线上; -1不判断 direct和place 不能同时使用 有且必须有一个为-1
//										  // 绊线 0: 右边 1: 左边  2:在线上; -1不判断
//
//	float 		        fDistance;		  //检测目标到周界、绊线得垂直距离 缺省为0 不为0时进入到绊线范围一定距离内就报警
//	bool		        blBoundOrVertex;  //绊线和周界压线情况 压线时是否认为穿过 或者认为完成从里到外 从左到右之类的动作 缺省为false
//	float               fSpeed;           //目标速度
//
//	alertDetailInfo     lastAlert;        //上一次位置和时间 当 place==-1 时 此项无意义 只使用当前数据 
//	alertDetailInfo     currentAlert;     //本次位置和时间 当判断方向时 laseAlert 和currentAlert的值都需要
//	int 		  	    nRuleId;		  //报警后是否需要调整球机位置
//	int			        nAlarmType;	      //报警类型
//	bool 			    blDomeTrack;	  //报警后是否需要调整球机位置
//	int 			    nDeviceId;		  //报警后需要调整球机位置的 指定球机id 如果-1表示随机
//
//	void*				pUser1;			  // 多目标ObjectState结构体指针  currentAlert使用 不为零时需要自己销毁 为0时需要自己计算方位角和gps信息
//	void*				pUser2;			  //
//	unsigned char       byReserved[20];   //保留字段  
//
//	tagDetectAlarm_Info() : blUseRule(true), nDirect(-1), nPlace(0), blBoundOrVertex(false), fDistance(0.0f), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
//} DETECTALARM_INFO;
//
////1.6 热力图报警结构体
////返回报警信息 人群聚集使用
//typedef struct tagDensityAlarm_Info {
//	//前面为输入参数
//	std::string			sceneId;
//	int64_t				timestamp;			// 服务器Unix时间戳，以毫秒为单位
//	unsigned long		nDensityThreshold;  // 因人群密度结构体内未给出具体数值，此处可以设置一个具体的数值，可适应密度报警参数动态调整
//	int					nWidth;				// 画面内容分块信息 块的列数  若行数为16 列数为9 那么 map里面的std::vector<int> 共用16*9个元素 每个元素代表分割区域内的检测目标值, 如果是-1表示不需要密度信息
//	int					nHeight;			// 画面内容分块信息 块的行数  当设置的参数CFG_DENSITYDETECTION_INFO结构体与此结构体内的行列数值不一致时，以此结构体为准
//	std::string			klass;			    // 目标类型
//	std::string			category;		    // 目标大类 
//	std::string			image;				// 目标图像 base64
//	float				fTotal;				// 密度检测输出的画面内目标数量
//	std::vector<float>  vStatistical;		// 示例 [1,1,1,1,1,1,1 .....]
//											// 下面为输出参数
//	int					nRuleId;			// 报警后写入  报警规则id
//
//	alertDetailInfo     currentAlert;
//	float				fAlarmTotal;		//报警区域内的数量 或百分比
//	int					nAlarmType;			//报警类型
//	bool				blDomeTrack;		//报警后是否需要调整球机位置
//	int					nDeviceId;			//报警后需要调整球机位置的 指定球机id 如果-1表示随机
//
//	void*				pUser1;			  // 多目标ObjectState结构体指针  currentAlert使用 不为零时需要自己销毁 为0时需要自己计算方位角和gps信息
//	void*				pUser2;			  //
//	unsigned char       byReserved[20];   //保留字段  
//	tagDensityAlarm_Info() : nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
//} DENSITYALARM_INFO;
//
//// 1.7 交通类报警信息结构体
//typedef struct tagTrafficAlarm_Info {
//	std::string 	    sceneId;		  //报警后 报警的场景
//	std::string         detectId;         //检测目标id
//	std::string         detectImage;	  //检测目标图像 base64
//	std::string         trackId;          //跟踪id
//	std::string			klass;			  //目标类型
//	std::string			category;		  //目标大类 
//	bool 		        blUseRule;		  //使用设定规则检查 true 不使用false
//	int                 ruleType;         //预警规则类型 ParkingRestrictedStop禁停区域停车 ViolationTrafficLine 轧线行驶 ExcessiveSpeed 超速 WrongSide逆行 FireAlarm
//	int                 nDirect;          //方向判断  对于周界来说就是 0:由外到里;1:由里到外 2:两者都可以; -1 不判断 
//										  //对应绊线来说就是 0:由左到右; 1:从右到左 2 : 穿过; -1 不判断  绊线和周界判断压线的情况见下
//	int                 nPlace;           //位置判断 周界  0:外面; 1:里面 2:在线上; -1不判断 direct和place 不能同时使用 有且必须有一个为-1
//										  // 绊线 0: 右边
//	float               fSpeedLimit;      // 超速值  一般根据国标要求 超速10%触发报警 因允许测量设备有10%的误差
//	int                 nMinDuration;     // 最短持续时间 单位：秒，0~65535； 0 每次都报警 > 0 就是间隔时间 
//
//	alertDetailInfo     lastAlert;        //上一次位置和时间 当 place==-1 时 此项无意义 只使用当前数据 
//	alertDetailInfo     currentAlert;     //本次位置和时间 当判断方向时 laseAlert 和currentAlert的值都需要
//	int 		  	    nRuleId;		  //报警后是否需要调整球机位置
//	int			        nAlarmType;	      //报警类型
//	float               fSpeed;           //目标速度
//
//	bool 			    blDomeTrack;	  //报警后是否需要调整球机位置
//	int 			    nDeviceId;		  //报警后需要调整球机位置的 指定球机id 如果-1表示随机
//
//	void*				pUser1;			  // 多目标ObjectState结构体指针  currentAlert使用 不为零时需要自己销毁 为0时需要自己计算方位角和gps信息
//	void*				pUser2;			  //
//	unsigned char       byReserved[20];   //保留字段  
//
//	tagTrafficAlarm_Info() : blUseRule(true), nMinDuration(0), nRuleId(-1), blDomeTrack(false), nAlarmType(-1), pUser1(nullptr), pUser2(nullptr) {};
//} TRAFFICALARM_INFO;
