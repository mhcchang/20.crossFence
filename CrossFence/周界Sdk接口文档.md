# 周界Sdk接口文档

## 版本变更
- v1.0 初始版本
  2021/01/29
- v1.1 sdk实现后调整
  2021/02/08
- v1.2 规则结构体内添加新属性，修改部分函数返回值，添加距离计算，中心点等函数
  2021/05/11
- v1.3 第1.4/1.5节 报警信息结构体 热力图报警结构体 修改 添加图像信息 联动设备id等
  2021/05/25
- v1.4 增加距离接口 主要用于报警标志位置的显示
  2021/07/29  
- v1.5 修改布防时间段的定义 一个报警规则内 支持多时间段的布防撤防 插入1.3结构体 后续序号顺延
  2021/09/30  

## 1、定义

### 1.1 点结构体定义
```C++
  typedef struct stru_Point2D
  {
    double x;          // x横坐标
    double y;          // y纵坐标
    stru_Point2D() : x(0.0), y(0.0) {};
    stru_Point2D(double _x, double _y) : x(_x), y(_y) {};    
  } Point2D, CFG_POINT2D;
```

### 1.2 时间段信息
```C++
typedef struct tagCFG_TIME_SECTION
{
  int    nBeginHour;             // 开始时间 小时 0-23
  int    nBeginMin;              // 开始时间 分钟 0-59
  int    nBeginSec;              // 开始时间 秒 0-59
  int    nEndHour;               // 结束时间 小时 0-23
  int    nEndMin;                // 结束时间 分钟 0-59
  int    nEndSec;                // 结束时间 秒 0-59
} CFG_TIME_SECTION;
```

### 1.3 布防时间段结构体
**在预警规则内部支持多时间段设置**
```C++
typedef struct tagCFG_TIMEGUARD_INFO
{
	int				 guardDateTimeId;				// id	   
	int				 guardDateTimeType;				// 0不启用 1 手动 2按月份每天 3按星期每周 
	CFG_TIME_SECTION stuDayTimeSection[MAX_MONTH_NUM][MAX_DAY_NUM][MAX_REC_TSECT_EX];			// 事件响应时间段 1手动时只有一条2按每月为周期时MAX_WEEK_DAY_NUM 最大有效值为31, 按周为周期时 MAX_WEEK_DAY_NUM 最大有效值为7 
	tagCFG_TIMEGUARD_INFO() : guardDateTimeId(-1), guardDateTimeType(0) {};
} CFG_TIMEGUARD_INFO;
```

### 1.4 拌线规则结构体
**事件类型 EVENT_IVS_CROSSLINEDETECTION (警戒线事件)对应的规则配置**
```C++
typedef struct tagCFG_CROSSLINE_INFO
{
    // 信息
  int                ruleId;
  char               szRuleName[MAX_NAME_LEN];         // 规则名称,不同规则不能重名
  bool               bRuleEnable;                      // 规则使能
  bool               bTrackEnable;                     // 触发跟踪使能,仅对绊线，入侵规则有效
  int                nDirection;                       // 检测方向:当ruleType类型为 CrossLine为绊线 时 检测方向 0:由左至右;1:由右至左;2:两者都可以
                                                       // 当ruleType类型为 CrossRegion为周界 时 检测方向 0:由外到里;1:由里到外 2:两者都可以
                                                       // 当ruleType类型为 Density 时 此项无效
  int                nPlace;
  float              fDistance;                        // 警戒线报警距离  
  int                nDetectLinePoint;                 // 警戒线/绊线顶点数
  CFG_POINT2D        stuDetectLine[MAX_POLYLINE_NUM];  // 警戒线/绊线顶点坐标 原则上不封闭 不能交叉！
  int                nTimeSection;                     // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
	CFG_TIMEGUARD_INFO guardTimes[MAX_GUARDTIME_NUM];	   // 布防时间
  int                nTriggerPosition;                 // 触发报警位置数
  unsigned char      bTriggerPosition[8];              // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
  int                nTrackDuration;                   // 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
  unsigned char      bReserved[20];                    // 保留字段 
  tagCFG_CROSSLINE_INFO() : ruleId(-1), bRuleEnable(true), bTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0), fDistance(0.0) {};
} CFG_CROSSLINE_INFO;
```

### 1.5 周界规则结构体
**事件类型EVENT_IVS_CROSSFENCEDETECTION(翻越围栏规则)对应的规则配置**
```C++
typedef struct tagCFG_CROSSFENCEDETECTION_INFO
{
    // 信息
  int                ruleId;
  char               szRuleName[MAX_NAME_LEN];          // 规则名称,不同规则不能重名
  bool               bRuleEnable;                       // 规则使能
  bool               bTrackEnable;                      // 触发跟踪使能,入侵规则有效
  int                nDirection;                        // 翻越围栏方向:0:由左至右;1:由右至左;2:两者都可以
  int                nPlace;
  int                nDetectRegionPoint;                // 周界顶点数
  CFG_POINT2D        stuDetectRegion[MAX_POLYLINE_NUM]; // 周界折线 顶点坐标
  int                nTimeSection;                      // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
  CFG_TIMEGUARD_INFO guardTimes[MAX_GUARDTIME_NUM];     // 布防时间
  int                nTriggerPosition;                  // 触发报警位置数
  unsigned char      bTriggerPosition[8];               // 触发报警位置,0-目标外接框中心, 1-目标外接框左端中心, 2-目标外接框顶端中心, 3-目标外接框右端中心, 4-目标外接框底端中心
  int                nTrackDuration;                    // 跟踪持续时间,0秒:一直跟踪,1~300秒:跟踪持续时间
  unsigned char      bReserved[20];                     // 保留字段
  tagCFG_CROSSFENCEDETECTION_INFO() : ruleId(-1), bRuleEnable(true), bTrackEnable(false), nTimeSection(0), nTriggerPosition(0), nTrackDuration(0) {};
} CFG_CROSSFENCEDETECTION_INFO;
```

### 1.6 人群密度检测结构体
**事件类型EVENT_IVS_DENSITYDETECTION(人群密度检测事件)对应的规则配置**
```c++
typedef struct tagCFG_DENSITYDETECTION_INFO
{
    // 信息
  int                ruleId;
  char               szRuleName[MAX_NAME_LEN];          // 规则名称,不同规则不能重名
  bool               bRuleEnable;                       // 规则使能
  bool               bTrackEnable;                      // 触发跟踪使能
  unsigned char      byUnit;                            // 密度检测数值单位GB/T-30147需要， 0 密度等级，划分为四个，很稀疏，稀疏，密集，很密集。1 密度百分比
  unsigned char      bSensitivity;                      // 灵敏度(取值1-100，值越小灵敏度越低，对应人群的密集程度越高)
  unsigned char      byMinAlarmDensity;                 // 最小报警密度 取值1-100,报警门限值百分比
  int                nDetectRegionPoint;                // 检测区域顶点数
  CFG_POINT2D        stuDetectRegion[MAX_POLYGON_NUM];  // 检测区域
  int                nTimeSection;                      // 0不判断时间 1判断 stuDayTimeSection 2 判断 stuWeekTimeSection
  CFG_TIMEGUARD_INFO guardTimes[MAX_GUARDTIME_NUM];     // 布防时间
	int                nWidth;                            // 画面内容分块信息 块的列数 若在3840x2160的画面中将画面等分为16列9行，那么检测返回的DESITYALARM_INFO里面statistical中vector的元素中的统计数值就是以240*240的小块分割后的统计数值
	int                nHeight;                           // 画面内容分块信息 块的行数 此值为缺省值 最大行列数为画面大小
	unsigned char      byReserved[20];                    // 保留字段 
  tagCFG_DENSITYDETECTION_INFO() : ruleId(-1), bRuleEnable(true), byUnit(1), bSensitivity(50), nTimeSection(0), nWidth(-1), nHeight(-1) {};
} CFG_DENSITYDETECTION_INFO;
```

### 1.7 报警输入信息结构体
**返回报警信息 周界和绊线使用**
```C++
typedef struct tagDetectAlarm_Info {
  std::string       detectId;       //检测目标id
  std::string       detectImage;    //检测目标图片 从消息队列取数据时使用

  std::string       trackId;        //跟踪id
  bool              useRule;        //使用设定规则检查 true 不使用false
  int               direct;         //方向判断  对于周界来说就是 0:由外到里;1:由里到外 2:两者都可以; -1 不判断 
                                    //对应绊线来说就是 0:由左到右; 1:从右到左 2 : 穿过; -1 不判断  绊线和周界判断压线的情况见下
  int               place;          //位置判断 周界  0:外面; 1:里面 2:在线上; -1不判断 direct和place 不能同时使用 有且必须有一个为-1
                                    // 绊线 0: 右边 1: 左边  2:在线上; -1不判断
  float             fDistance;      //检测目标到周界、绊线得垂直距离 缺省为0 不为0时进入到绊线范围一定距离内就报警
  bool boundOrVertex;               //绊线和周界压线情况 压线时是否认为穿过 或者认为完成从里到外 从左到右之类的动作 缺省为false
  struct alertDetailInfo {          //报警检测信息结构体 坐标和时间戳
    int64_t timestamp;              //报警检测时间戳
    float x;                        //报警检测坐标 x 左上角点x坐标
    float y;                        //报警检测坐标 y 左上角点y坐标
    float w;                        //报警检测坐标 w 目标宽度
    float h;                        //报警检测坐标 y 目标高度
    alertDetailInfo() : timestamp(0), x(0.0f), y(0.0f), w(0.0f), h(0.0f) {};
    alertDetailInfo(int64_t timestamp, float x, float y, float w, float h) : timestamp(timestamp), x(x), y(y), w(w), h(h) {};
  };                 
  alertDetailInfo  lastAlert;        //上一次位置和时间 当 place==-1 时 此项无意义 只使用当前数据 
  alertDetailInfo  currentAlert;     //本次位置和时间 当判断方向时 laseAlert 和currentAlert的值都需要
  std::string      sceneId;          //报警后 给出报警的场景
  int              ruleId;           //报警后是否需要调整球机位置
  int              nAlarmType;       //报警类型
  bool             blDomeTrack;      //报警后是否需要调整球机位置
  int 			       nDeviceId;		     //报警后需要调整球机位置的 指定球机id 如果-1表示随机
	unsigned char    byReserved[20];   // 保留字段 
  tagDetectAlarm_Info() : useRule(true), direct(-1), place(0), boundOrVertex(false), fDistance(0.0f), ruleId(-1), blDomeTrack(false), nAlarmType(-1) {};
} DETECTALARM_INFO;
```

### 1.8 热力图输出的数据结构体
**输出来自于人群密度估算的密度数据 数据内容中的目标类型可以是人群、车辆、船等等**
```C++
typedef struct tagdensityData_Info {
    int width;
    int height;
    std::vector<float> datas;       // 此链表的长度为width*height
} DESITYDATA_INFO;
```

### 1.9 报警输入信息结构体
```C++
typedef struct tagdensityAlarm_Info {
  int64_t        tm;
  unsigned long  nDensityThreshold;   // 因人群密度结构体内未给出具体数值，此处可以设置一个具体的数值，可适应密度报警参数动态调整
  int            nGridCol;            // 画面内容分块信息 块的列数  若行数为16 列数为9 那么 map里面的std::vector<int> 共用16*9个元素 每个元素代表分割区域内的检测目标值, 如果是-1表示不需要密度信息
  int            nGridRow;            // 画面内容分块信息 块的行数  当设置的参数CFG_DENSITYDETECTION_INFO结构体与此结构体内的行列数值不一致时，以此结构体为准
  std::map<std::string, int> total;   // 密度检测输出的画面内目标数量
  std::map<std::string, std::vector<float> > statistical;  //示例 [["car",[1,1,1,1,1,1,1 .....]],["truck",[1,1,1,1,1,2,3,3,3,....]],["person",[1,3,33,4,4,4,4,0,0,0,0,]]

  std::string    sceneId;
  int            ruleId;
  float          fX;
  float          fY;
  float          fW;
  float          fH;
  int            nAlarmType;         //报警类型
  bool           blDomeTrack;        //报警后是否需要调整球机位置
  int			 nDeviceId;		     //报警后需要调整球机位置的 指定球机id 如果-1表示随机
  tagDensityAlarm_Info() : ruleId(-1), blDomeTrack(false), nAlarmType(-1) {};
} DESITYALARM_INFO;
```

### 1.10 错误信息id枚举类型
```C++
enum ALARM_ERROR_CODE {
  ERROR_NONE = 0,           //没有错误
  ERROR_PARAMS,             //输入参数错误
  ERROR_TYPE,               //输入类型错误
  ERROR_VALIDTIME,          //输入时间无效
  ERROR_NOIMPL = 0xFF       //功能没有实现
};
```

## 2、报警处理类
 - 类名 ZhSmartAlarm
    报警规则设置和检测结果输出

### 2.1 设置报警规则
**输入参数为 void*可根据报警类型，输入不同的报警参数结构体**
 - void ZhSmartAlarm::SetupPara(int ruleType, void* lpAlarmSetupParam);
 - 输入参数
  ```
    ruleType : 报警类型 1 绊线， 2 周界， 3 人群密度
    lpAlarmSetupParam : 报警参数结构体
            当ruleType=1时 为CFG_CROSSLINE_INFO指针
            当ruleType=2时为CFG_CROSSFENCEDETECTION_INFO指针
            当ruleType=3时 为CFG_DENSITYDETECTION_INFO指针
  ```
- 返回值
  ```
    无
  ```

### 2.2 周界绊线报警判断
**指定CFG_DENSITYDETECTION_INFO 报警规则 时间等**
 - int ZhSmartAlarm::DetectCheck(int ruleType, DETECTALARM_INFO &alarmInfo);
 - 输入参数
  ```
    int ruleType 检测类型  1 绊线， 2 周界
    alarmInfo 检测数据信息
  ```
 - 返回值
  ```
    成功返回 0 符合报警规则 1 不符合无警报 不在区域内 2不在时间范围 天 3不在时间范围 星期
    失败返回负值
  ```
###

### 2.3 人群密度报警判断
 - int ZhSmartAlarm::DesityCheck(DESITYALARM_INFO &alarmInfo);
 - 输入参数
  ```
    alarmInfo 检测数据信息
  ```
 - 返回值
  ```
    成功 ：返回 1 符合报警规则 0不符合无警报
    失败返回 负值
  ```
###

### 2.4 人群密度统计
**根据规则设置或者输入的折线统计人群密度 返回区域内的数量**
 - int ZhSmartAlarm::DesityStatistical(DESITYALARM_INFO &alarmInfo, bool blUseCfg, std::vector<Point2D> vp);
 - 输入参数
  ```
    alarmInfo 检测数据信息
    blUseCfg 使用设置判断 当设置为false时使用vp参数里面的周界判断 否则使用配置的规则
    vp 周界顶点链表
  ```
 - 返回值
  ```
    成功 返回 输入参数里面大于等于 densityThreshold项的值 报警门限 为当前画面上周界内具体的目标个数
    失败 返回负值
  ```
###

### 2.5 设置检测报警回调 (暂不实现)
 - void ZhSmartAlarm::SetAlarmDetectCallback(std::function<int(int, std::vector<DETECTALARM_INFO> &)> callback)
 - 输入参数
  ```
    std::function<int(int, void**)> callback  回调函数具体实现的函数指针
  ```
 - 返回值
  ```
    无
  ```

### 2.6 设置人群密度报警回调 (暂不实现)
 - void ZhSmartAlarm::SetAlarmDesityCallback(std::function<int(int, std::vector<DESITYALARM_INFO> &)> callback);
 - 输入参数
  ```
    std::function<int(int, void**)> callback 回调函数
        回调函数参数
  ```
 - 返回值
  ```
    无
  ```
###

### 2.7 设置报警数据源 (暂不实现)
 - int ZhSmartAlarm::SetDetectDataSource(int ruleType, std::vector<DETECTALARM_INFO> &alarms);
 - 输入参数
  ```
    int ruleType 检测类型  1 绊线， 2 周界
    std::vector<DETECTALARM_INFO> &alarms 报警数据链表
  ```
 - 返回值
  ```
    成功返回alarms 的大小 >=0 
    失败返回负值
  ```
###

### 2.8 设置人群数据源(暂不实现)
**指定CFG_DENSITYDETECTION_INFO 报警规则 时间等**
 - int ZhSmartAlarm::SetDesityDataSource(std::vector<DESITYALARM_INFO> &alarms);
 - 输入参数
  ```
    alarms 数据源链表
  ```
 - 返回值
  ```
    成功返回alarms 的大小 >=0 
    失败返回负值
  ```
###

### 2.9 获取绊线周界报警信息(暂不实现)
**没有设置回调的情况下，sdk内部自动保存300个报警信息，每种类型100个 如果需要两种类型的信息需要使用不同的ruleType调用两次**
 - int ZhSmartAlarm::GetDetectAlarmInfo(int ruleType, std::vector<DETECTALARM_INFO> &alarms);
 - 输入参数
  ```
    ruleType : 报警类型 1 绊线， 2 周界
    alarms : 报警信息链表
  ```
 - 返回值
  ```
    返回报警信息数量 ， 小于0表示调用有错误
  ```

### 2.10 获取物体（人群）密度报警信息(暂不实现)
**没有设置回调的情况下，sdk内部自动保存300个报警信息，每种类型100个 可以依次取出**
 - int ZhSmartAlarm::GetDetectAlarmInfo(int ruleType, std::vector<DESITYALARM_INFO> &alarms);
 - 输入参数
  ```
    ruleType : 报警类型== 3 物体（人群）密度 只能为3
    alarms : 报警信息链表
  ```
 - 返回值
  ```
    返回报警信息数量 ， 小于0表示调用有错误
  ```

### 2.11 获取上次调用函数的报错信息id
 具体id号对于信息参考 1.7错误信息id
- int GetErrorCode();


## 3 相关算法函数类
- 类名 ZhcvMath

### 3.1 判断点是否在多边形内
- static int IsPtInPoly(std::vector<Point2D> polygon, Point2D point);
 - 输入参数
```
    polygon    多边形的各个定点
    point     需要判断的点
```
- 返回值
 ```
  说明点在线的左边1，小于在右边-1，等于则在线上0
 ```

### 3.2 判断点是否在直线的一侧
面积法
- static int LeftOfLine_Area(const Point2D p, const Point2D p1, const Point2D p2);
 - 输入参数
```
    p     需要判断的点
    p1    线起点
    p2    线终点
```
- 返回值
 ```
  点在线的左边1，小于在右边-1，等于则在线上0。 
 ```

### 3.3 判断点是否在直线的一侧
叉积法
- static int LeftOfLine_Vector(Point2D p0, Point2D p1, Point2D p2);
 - 输入参数
```
    p     需要判断的点
    p1    线起点
    p2    线终点
```
- 返回值
 ```
  点在线的左边1，小于在右边-1，等于则在线上0。 
 ```

### 3.4 判断线段是否和矩形有交点
Cohen-Sutherland裁剪算法
- static int CSClip_Line(float p1x, float p1y, float q1x, float q1y, bool blClip, float &x1, float &y1, float &x2, float &y2);
- 输入参数
```
    p1x p1y 矩形左上角
    q1x q1y 矩形右下角
    blClip 是否需要裁剪线段 不裁剪直接返回结果
    [out] x1, y1 线段起点 如果与矩形边有交叉 返回交叉点
    [out] x2, y2 线段起点 如果与矩形边有交叉 返回交叉点
```
- 返回值
 ```
  有交叉点返回1，没有交叉返回0
 ```

### 3.5 计算点到线段的距离 
- static double calculateDistance(double x1, double y1, double x2, double y2, double x0, double y0, bool &blCross);
- 输入参数
```
    x1, y1, x2, y2 线段两个端点
    x0, y0 线段外的点
    [out] blCross为true时 垂足在线段上 false垂足不在线段上
```
- 返回值
 ```
   返回垂距
 ```

### 3.6 获取多边形中心点
- static Point2D GetCenterOfGravityPoint(std::vector<Point2D> mPoints);
- 输入参数
```
    mPoints 多边形的端点
```
- 返回值
 ```
   中心点
 ```

### 3.7 已知矢量线段 求通过线段中心的垂线上的点到线段的距离为distance
- static Point2D GetPerpendicularPointWithDistance(double x1, double y1, double x2, double y2, double distance, int direction);
- 输入参数
```
  x1,y1 矢量线段的起始点
  x2,y2 矢量线段的终止点
  distance 到线段的距离
  direction 方向0 左边; 1 右边 按照左手系计算 
```
- 返回值
 ```
   点坐标
 ```

## 4 调用示例
```C++
  int DecectImp(int, std::vector<DETECTALARM_INFO> &)
  {
      ::printf("DecectImp \n");
      return 0;
  }

  int DesityImp(int, std::vector<DESITYALARM_INFO> &)
  {
      ::printf("DesityImp \n");
      return 0;
  }

  int main(int argc, char** argv)
  {
    //
      ZhSmartAlarm alarm;
      CFG_CROSSLINE_INFO cross;
      CFG_CROSSFENCEDETECTION_INFO crossfenc;
      CFG_DENSITYDETECTION_INFO density;
      std::vector<DETECTALARM_INFO> detectAlarm;
      std::vector<DESITYALARM_INFO> desityAlarm;
      int ruleType = 1;
      alarm.SetupPara(ruleType++, cross);
      alarm.SetupPara(ruleType++, crossfenc);
      alarm.SetupPara(ruleType, density);

      alarm.SetAlarmDetectCallback(std::bind(DecectImp, std::placeholders::_1, std::placeholders::_2));
      alarm.SetAlarmDesityCallback(std::bind(DesityImp, std::placeholders::_1, std::placeholders::_2));
      while (1)
      {
          ::_sleep(1000);
      }
      return 0;
  }
```
