#pragma once

#include "ZhcvSmartDefs.h"

#if __WINDOWS_OS__
#ifdef ZHSMARTALERM_EXPORTS
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

//#define MAX_NAME_LEN                128             // 通用名字字符串长度
//#define MAX_OBJECT_LIST_SIZE        256             // 视频分析设备支持的检测物体类型列表个数上限
//#define MAX_POLYLINE_NUM            20              // 视频分析设备折线顶点个数上限
//#define MAX_DAY_NUM                 31              // 一月的天数
//#define MAX_WEEK_DAY_NUM			  7	    		  // 每周的天数
//#define MAX_MONTH_NUM               12              // 一年12个月
//#define MAX_REC_TSECT_EX            24              // 录像时间段扩展个数

//#define MAX_POLYGON_NUM             256             // 视频分析设备区域顶点个数上限
//#define MAX_CHANNELNAME_LEN         64              // 最大通道名称长度
//#define MAX_CHANNEL_COUNT           36
//#define MAX_VIDEO_CHANNEL_NUM       256             // 最大通道数256
//#define MAX_DBKEY_NUM               64              // 数据库关键字最大值
//#define MAX_SUMMARY_LEN             1024            // 叠加到JPEG图片的摘要信息最大长度
//#define MAX_CALIBRATEBOX_NUM        10              // 智能分析校准框个数上限

//#define MAX_PATH					  255

#include <string>
#include <vector>
#include <functional>
#include <map>

class ZhcvMath;

/**
* @brief 报警处理类
* @detail 先实现绊线 周界 人群密度  后期可能加相机的其它smart功能
*/
class DLL_API ZhSmartAlerm
{
public:
	//手动时间
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
	ZhSmartAlerm();
	~ZhSmartAlerm();

	/**
	* @brief 2.1 设置报警规则
	*	detail 输入参数为 void*可根据报警类型，输入不同的报警参数结构体**
	* @param 输入参数
		ruleType : 报警类型 1 绊线， 2 周界， 3 人群密度
		lpAlarmSetupParam : 报警参数结构体
		当ruleType = 1时 为CFG_CROSSLINE_INFO指针
		当ruleType = 2时为CFG_CROSSFENCEDETECTION_INFO指针
		当ruleType = 3时 为CFG_DENSITYDETECTION_INFO指针
	* @return 设置成功返回true 否则返回false
	**/
	bool SetupPara(int ruleType, void* lpAlarmSetupParam);

	/**
	* @brief 2.2 周界绊线报警判断
	* @param
	*	int ruleType 检测类型  1 绊线， 2 周界
	*	alermInfo 检测数据信息
	* @return 返回值
	*	成功返回 0 符合报警规则 1 不符合无警报 不在区域内 2不在时间范围 天 3不在时间范围 星期
	*	失败返回负值
	*/
	int DetectCheck(int ruleType, DETECTALERM_INFO &alermInfo);

	/**
	* @brief 2.3 人群密度报警判断
	* @param
	*	alermInfo 检测数据信息
	* @return 返回值
	*	成功返回 1 符合报警规则 0不符合无警报
	*	失败返回负值
	*/
	int DesityCheck(DESITYALERM_INFO &alermInfo);

	/**
	* @brief 2.4 人群密度统计
	* @param
	*	alermInfo 检测数据信息
	*   blUseCfg 使用设置判断 设置为false时使用vp里面的周界判断 否则使用配置的规则
	*   vp 周界顶点链表
	* @return 返回值
	*	成功返回 输入参数里面大于等于 densityThreshold项的值 报警门限 为当前画面上周界内具体的目标个数
	*	失败返回负值
	*/
	int DesityStatistical(DESITYALERM_INFO &alermInfo, bool blUseCfg, std::vector<Point2D> vp);

	/**
	* @brief 2.11 获取最后报错信息id
	*/
	int GetErrorCode() { return m_nErrorCode; };
#ifndef DEBUG
protected:
#endif // DEBUG

	/**
	* @brief 2.5 设置报警回调
	* @param 输入参数
	*	std::function<int(int, void**)> callback 回调函数
	*	回调函数参数
	*/
	void SetAlarmDetectCallback(std::function<int(int, std::vector<DETECTALERM_INFO> &)> callback);
	void SetAlarmDesityCallback(std::function<int(int, std::vector<DESITYALERM_INFO> &)> callback);

	/**
	* @brief 2.7 设置报警数据源
	* @param 输入参数
	*	std::function<int(int, void**)> callback 回调函数
	*	回调函数参数
	*/
	int SetDetectDataSource(int ruleType, std::vector<DETECTALERM_INFO> &alerms);
	int SetDesityDataSource(std::vector<DESITYALERM_INFO> &alerms);
	/**
	* @brief 2.9 获取绊线周界报警信息
	* 没有设置回调的情况下，sdk内部自动保存300个报警信息，每种类型100个 如果需要两种类型的信息需要使用不同的ruleType调用两次**
	* @param 输入参数
	*	ruleType : 报警类型 1 绊线， 2 周界
	*	alerms : 报警信息链表
	* @return 返回值
	*	返回报警信息数量 ， 小于0表示调用有错误
	*/
	int GetDetectAlarmInfo(int ruleType, std::vector<DETECTALERM_INFO> &alerms);

	/**
	* @brief 2.10 获取物体（人群）密度报警信息
	*  detail 没有设置回调的情况下，sdk内部自动保存300个报警信息，每种类型100个 可以依次取出**
	* @param 输入参数
	*   ruleType : 报警类型== 3 物体（人群）密度 只能为3
	*alerms : 报警信息链表
	* @return 返回值
	*	返回报警信息数量 ， 小于0表示调用有错误
	*/
	int GetDetectAlarmInfo(int ruleType, std::vector<DESITYALERM_INFO> &alerms);

private:

	std::function<int(int, std::vector<DETECTALERM_INFO> &)> m_pDecectCall;
	std::function<int(int, std::vector<DESITYALERM_INFO> &)> m_pDesityCall;

	//判断是否在时间段内
	int InTimeDaySeg(std::vector<TimeDaySegment> vtm, time_t tmWhen);
	int InTimeWeekSeg(std::vector<TimeWeekSegment> vtm, time_t tmWhen);
	//与detectInfo 里面的direct 和place做判断 一致的为true 否则为false 
	//blAtLine 表示在边或定点上是否认为
	bool CheckPlace(int ruleType, const DETECTALERM_INFO &detectInfo);
	void SetErrorCode(int nErr) { m_nErrorCode = nErr; };
private:
	CFG_CROSSLINE_INFO m_struCrosslineInf;
	CFG_CROSSFENCEDETECTION_INFO m_struCrossFenceInf;
	CFG_DENSITYDETECTION_INFO m_struDensityInf;

	int m_nErrorCode;
};
