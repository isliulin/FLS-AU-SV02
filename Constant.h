//在MSP430中的Long型的数组是低字在前，高字在后

#ifndef _CONSTANT_H
#define _CONSTANT_H 

#define DEBUG
//#define GETSOEFROMRAM   发送的SOE从外部FLASH取，还是从内部取。从外部取
//#define ZDSENDADDR //主动给中心站定时发送自身站地址(特殊应用)
//#define FILTER_COM 定义后会发送滤波后的波形
//#define PHASELUBO


#define         TYPE_EQU                0x1111  //设备代码

#define        INVALIDYC  0xD8F0//-9999
#define        INVALTEYC  0xFF9D//-99
#define        INITYC     0xDD47//-8888
#define        ODUINITYC     0xFFFF//ODU初始化值


 
#define         YES                     0x55
#define         NO                      0xAA
#define         ON                      0x5F
#define         OFF                     0x50
#define         CLOSE                   0x55
#define         TRIP                    0xAA

#define  RP_LBSTORAGE_INVALID        0XFFFF  //录波有效期设为该值时，表示录波不受该参数的设置
#define  RP_LUBO_INVALID                 64//个数设为该值时不生效

#define  OPERATING_LOGO1 1 //操作标识1 读目录
#define  OPERATING_LOGO2 2//操作标识2 读目录确认
#define  OPERATING_LOGO3 3//读文件激活
#define  OPERATING_LOGO4 4 //读文件激活确认
#define  OPERATING_LOGO5 5 //读文件数据响应
#define  OPERATING_LOGO6 6 //读文件数据响应确认

#define  GPS_PCAS00     0 //保存配置信息
#define  GPS_PCAS01     1//串口波特率配置
#define  GPS_PCAS02     2//设置定位更新率
#define  GPS_PCAS03     3 //开始或停止输出NMEA语句
#define  GPS_PCAS04     4 //配置工作系统
#define  GPS_PCAS05     5 //设置NMEA语句中的发送器标识符
#define  GPS_CCSIR      6 //当前系统仿真信号及设备状态
#define  GPS_CCRM0      7 //开始或停止输出NMEA语句
#define  GPS_PCAS10     10 //接收机重启
#define  GPS_PCAS11     11 //设置当前导航平台模型
#define FSOE_NUM_MAX    8//22  由于增加了故障事件 22改为8

#define  LUBO_FILE     0x01 //录波文件
#define  FIXPT_FILE     0x02 //定点记录文件
#define  SOE_FILE       0x04 //SOE文件
#define  ULOG_FILE       0x08 //LOG文件




/*-------------------------------------------------------------------------
 *   Standard Bits
 *-------------------------------------------------------------------------*/
#define NBIT0                (0xFFFE)
#define NBIT1                (0xFFFD)
#define NBIT2                (0xFFFB)
#define NBIT3                (0xFFF7)
#define NBIT4                (0xFFEF)
#define NBIT5                (0xFFDF)
#define NBIT6                (0xFFBF)
#define NBIT7                (0xFF7F)
#define NBIT8                (0xFEFF)
#define NBIT9                (0xFDFF)
#define NBITA                (0xFBFF)
#define NBITB                (0xF7FF)
#define NBITC                (0xEFFF)
#define NBITD                (0xDFFF)
#define NBITE                (0xBFFF)
#define NBITF                (0x7FFF)

#define LINE_NUM   3 //12  //支持的指示器线路上限
#define PHASE_NUM  4  //支持的指示器相位 A,B,C,0
#define FTTPYE_NUM   9//8//7//6 //瞬时短路，接地，低压，温度 电场有电，永久短路 短路 遥控翻牌复归 有电无电
#define TEMP_PHASE_NUM  3  //温度只有A,B,C三相
#define YCTPYE_NUM   6 //突变遥测，定时，上电，停电、 电场下降遥测，心跳遥测 6种//zxx

#define REPEAT_START_NUM  37  //中继器电池电压的起始地址

#define PA_ADDR_NUM 20

//===================数据类型定义============================
typedef unsigned char  INT8U;       //无符号8位数
typedef          char  INT8S;       //有符号8位数
typedef unsigned int   INT16U;      //无符号16位数
typedef signed   int   INT16S;      //有符号16位数
typedef unsigned long  INT32U;      //无符号32位数
typedef signed   long  INT32S;      //有符号32位数
typedef float          FP32;        //单精度浮点数
typedef double         FP64;            //双精度浮点数
//===================接收指示器信息的标志位============================
#define  COMTRADE_ITYPE 0x30//指示器发送电流录波数据时的标志位

#define  COMTRADE_VTYPE 0x31 //指示器发送电压录波数据时的标志位

#define  COMTRADE_FINISH 0x50 //召完指示器录波的标志位

////============================  AD采集  =============================================
#define AC_AD_CHAN_NUM                          4 //交流AD采样通道数

#define AC_BUFF_LEN                             10//交流采集缓冲区保存10个点

//采样通道号定义
#define CHAN_CAP                                 0               
#define CHAN_SUN                                 1//以上是同一个时间点采集

#define CHAN_BAT                                 2   //以下是同一个时间点采集
#define CHAN_BATF                                3
////============================  AD采集  电压量=============================================
#define OPEN_SUNC                                0       //打开太阳能电容的采集管教        
#define OPEN_BAT                                 1      //打开锂电池的采集管教
#define OPEN_BATSUNC                             2      //打开所有的采集管教

//============================  SMS-电话号码        =======================================

#define PHONE_CHARNUM         16     //每个电话号码占的字节数/字符数 最大字符数 包括1字节长度
#define PHONE_NUM             5

#define PHONE_ENCTL           0 //电话号码使能控制字
#define PHONE_ENCTLH          1
#define PN_START              2//电话号码的起始位置
#define PHONE1                (PN_START+0*PHONE_CHARNUM+1)//+1是定位到电话1的号码，0位置为电话1的号码长度
#define PHONE2                (PN_START+1*PHONE_CHARNUM+1)
#define PHONE3                (PN_START+2*PHONE_CHARNUM+1)
#define PHONE4                (PN_START+3*PHONE_CHARNUM+1)
#define PHONE5                (PN_START+4*PHONE_CHARNUM+1)
#define VERSION               (PN_START+5*PHONE_CHARNUM)  //定位到版本号长度那个字节 总12个字节
#define COMPNAME              (VERSION + 12)  //版本号占12个字符,其中包含1字节长度
#define DEVID                 (VERSION + 15)  //公司名占3个字节,其中包含1字节长度 

#define PHONE_CS              (VERSION + 40)//产品型号占25字节，其中包含1字节长度       
#define PHONE_PA_NUM           PHONE_CS + 1



//============================  运行参数        =======================================

#define RP_CFG_KEY               0      //配置控制字1
#define RP_CFG_KEYH              1      //配置控制字2

#define YX_MODEl                 2//1     //脉冲还是普通I/O 控制字
#define YX_MODEh                 3//2 
#define YX_DSl                   4//3     //单点还是双点
#define YX_DSh                   5 
#define RP_YXYCMODE              6     //路数是否合并遥信遥测
#define Terminal_Type            7     // 0 表示架空 2表示电缆终端ODU通信1 表示电缆终端无线通信
#define RP_POWER_MODE            8     //电源模式，0 = 准实时在线 1 = 实时在线
#define RP_GPRS_COMP             9     //GPRS厂家选择 1 = 映翰通
#define RP_DEVICE_TYPE           10     //设备类型 0 = 一遥； 1 = 二遥
#define RP_ZSQ_TYPE              11    //指示器类型 0=RF001 1=RF003 for 遥控

#define RP_FLTLK_T               12   //故障闭锁时间 步长10S
#define RP_ZSQCST_T              13   //故障指示器通信状态检测周期
#define RP_ADCOL_T               14    //AD采集时间间隔
#define RP_BEAT_T                15    //定时上传心跳 步长1S
#define RP_SENDYC_T              16    //定时上传遥测时间间隔
#define RP_YXRSD_T               17   //遥信重发间隔步长
#define YX_delay                 18   //防抖时间间隔
#define RP_SAVELOAD_T            19  //保存负荷电流的时间间隔，步长1S 最小为15分钟
#define RP_ADBAT_T               20   //锂电池采样间隔，每次检测需要打开锂电池去采集
#define RP_valt_T                21   //电源不稳时，准实时模式闭锁时间
#define RP_STATUSRP_TIME         22   //状态汇报时刻
#define RP_STATUSRP_INTERVAL     23   //状态汇报间隔
#define RP_CLOSEGPRSPWR_T        24   //准实时模式下GPRS开启时间(默认5m，电缆默认3分)

#define RP_COMM_ADDR             25   //子站站地址 
#define RP_SYX_INFADDR           26    //单点遥信起始信息体地址
#define RP_DYX_INFADDR           27    //双点遥信起始信息体?
#define RP_RFBAND                28    //RF通信频段，遥控使用
#define RP_YCLIMIT               29   //遥测绝对越限值
#define RP_TELIMIT               30   //温度越限值
#define RP_YCCAP                 31   //遥测相对越限值 相对指百分比
#define RP_SNTP_T        32  //SNTP对时时间间隔
#define RP_ALARM_BAT             33  //电池电压报警值
#define RP_ABSOLUTE_CURREN       34  //电流绝对突变
#define RP_ABSOLUTE_VOLTAGE      35  //电压绝对突变
#define RP_COEFFI_VOLTAGE        36  //电压基准值，暂时不先使用
#define RP_diff_NUM              37  //电压与电流突变的位置的差
#define RP_electric_Vari         38  //电场变化百分比
#define RP_VOLTAGE_RMS           39  //电压初始有效值，用来判断电压的有效性
#define RP_LUBOGPRS_T            40  //录波时上位机与终端的时间重传间隔
#define RP_CURRENT_CY            41  //电流超限设置
#define RP_CURRENT_CA            42  //A相电流校准
#define RP_CURRENT_CB            43  //B相电流校准
#define RP_CURRENT_CC            44  //C相电流校准
#define RP_VOLTAGE_V0            45  //上传给主站的电场系数
#define RP_LBSTORAGE_TIME        46  //录波存储有效期
#define RP_LUBO_NUM              47  //录波存储数量
#define RP_LUBO_CYCLE            48  //总录波周期
#define RP_CALL_TIME             49  //呼叫时间
#define RP_GPS_T                 50  //GPS间隔打开时间min
#define RP_OPENGPS_T             51  //GPS打开后的持续时间s
#define RP_OPENGPS_NUM           52  //GPS打开后的打开RP_OPENGPS_NUM次都没有收到GPS信息，则上报GPS状态异常，同时打开RTC
#define RP_SHORTFAULT_RESETTIME  53  //短路故障复位时间
#define RP_LUBO_RESETTIME        54        //录波标志位复位时间
#define RP_SET_KEY               55        //短路故障时是否由终端复位所有短路故障相
#define RP_YXRSD_NUM              56        //短路故障时是否由终端复位所有短路故障相










//#define RP_RECORDER_TOLNUM             33  //电池电压报警值



#define RP_CRC                   57      //crc校验码
#define RUN_PARA_NUM             58

//控制字具体定义
#define RPCFG_ISSENDYC              0      //是否定时上传全遥测 0=不上传 1=定时上传
#define RPCFG_ADCHG_SDYC            1     //AD采集时，是否立即上传全遥测 1=立即上传 0=不传
#define RPCFG_SEND_FTYC             2      //是否发送故障遥测(全遥测) 1=上传 0=不传
#define RPCFG_SEND_CHANGEYC         3     //是否发送遥测越限值 0=不传 1=上传
#define RPCFG_ZDDI_MODE             4     //0=不开启硬遥信 1=开启,若不开启则不会打开硬遥信的24v电源
#define RPCFG_SEND_SMS              5      //0=不发送短信 1=发送
#define RPCFG_COMMFLTVAL_MODE       6      //0=通道故障时遥测设置成无效值 1=舍弃
#define RPCFG_ISSENDINITFIN         7     //0=不发送初始化长帧，1=发送 默认发送
#define RPCFG_ISDDMODE              8     //0=正常模式 1=对点模式
#define RPCFG_PRINTF                9     //0=不转发ODU或RF数据到调试口 1=转发
#define RPCFG_SENDINITFIN_FIRST     10    //0=按照参数7的设置发送初始化长帧 1=只在上电后的第一次发
#define RPCFG_PRINTDUG               11     //0=不发送调试信息，包括下行通道的转发1=发送
#define RPCFG_SNTP_TIME             12      // 是否启动定时SNTP对时
#define RP_COMTRADE_TYPE             13  //dat 文件传输的格式 0= ASCII码上传 1=二进制上传
#define RP_DEBUG_RATE                14  //调试串口波特率 0= 115200 1=9600
#define RP_START_LUBO                15  //是否启动录波

#define RP_START_TIME                16  //是否关闭录波对时
#define RP_set_TIME                  17  //是否关闭主站对时
#define RP_AVOLTAGE_REVERSE          18 //录波A电场方向是否取反，该设置对应ABC0所有的电场
#define RP_BVOLTAGE_REVERSE          19 //录波B电场方向是否取反，该设置对应ABC0所有的电场
#define RP_CVOLTAGE_REVERSE          20 //录波C电场方向是否取反，该设置对应ABC0所有的电场
#define RP_ACURRENT_REVERSE          21 //录波A相电流方向是否取反，//该设置对应ABC0所有的电流
#define RP_BCURRENT_REVERSE          22 //录波B相电流方向是否取反，
#define RP_CCURRENT_REVERSE          23 //录波C相电流方向是否取反，

#define RP_SHUANGDIAN_SET            24 //所有遥信是否配置成双点
#define RP_SHORT_FLAG                25 //是否由终端启动瞬时转永久故障
#define RP_OFF_GPS                   26 //关闭GPS
#define RP_FAULT_TIME                27 //短路故障时标都按当前收到报文的时间打，而不按RF003对时的时标打时间,为了电科院做24H2S的功能
#define RP_EARTH_FLAG                28 //是否由终端屏蔽不是故障的录波
#define RP_ERROR_FLAG                29 //缺相或对时有问题的是否屏蔽上传录波YX
#define RPCFG_ENCRYPT                30     //是否加密标志位
#define RPCFG_ISSENDFEI              31 //是否发送故障事件

//========================================================================
#define QUASI_REALTIME           0 //准实时模式
#define REALTIME                 1 //实时模式
#define REALTIME_SUNPOWER        2 //太阳能实时模式

#define AERIAL_TERMINAL         0  //架空终端
#define CABLE_TERMINAL_WIRELESS  1  //电缆终端
#define CABLE_TERMINAL_ODU   2  //电缆终端ODU
#define CABLE_TERMINAL_DODU   3  //电缆终端双ODU

//================================  定时器变量  ==========================

#define TIMER_NUM            27  //定时器个数 1s为单位
#define TM_BEAT               0     //定时上传心跳 GPRS通信心跳时间 步长1S
#define TM_ADCOLT             1   //AD采集时间间隔 此时置AD采集位，1s后使能AD采集
#define TM_SENDYC             2   //定时上传遥测时间间隔
#define TM_CLOSECHARGE        3   //关闭充电时间
#define TM_GPRSPWOERDOWN      4   //GPRS掉电时间，用于准实时在线模式
#define TM_SAVELOAD           5   //保存负荷电流
#define TM_ODU_call2          6   //召唤ODU的二级数据
#define TM_ODU_call           7   //召唤ODU数据
#define TM_BATPWOERDOWN       8   //锂电池关断时间
#define TM_SMSSENDALLOW       9  //短信发送延迟
#define TM_WAITXINHAO         10  //短信发送至信号源的等待时间
#define TM_ADBAT              11  //锂电池采样时间?#define TM_ADBATTEN      11  //锂电池的使能AＤ采样
#define TM_Volt               12  //电压不稳时，检测电源模式的时间间隔
#define TM_SENDSTATUSDELAY    13  //状态汇报延时
#define TM_SENDADDR           14  //子站地址发送延时
#define TM_SENDFAULTDELAY     15  //故障主动发送延迟 用于无限制模式
#define TM_SENDSTATE          16  // 发送ODU的状态汇报周期
#define TM_SNTP_TIME          17  //定时SNTP对时间隔
#define TM_GPRS_RW            18  //GPRS读写超时
#define TM_SMS_RW             19  //短信读写命令超时
#define TM_RECORDED           20  //收到指示器有录波数据后延时等待其它相的指示器数据
#define TM_YKTRIGGER          21  //收到指示器有录波数据后延时等待其它相的指示器数据
#define TM_SHORT_FLAG         22  //瞬时转永久的时间
#define TM_GPS_TIME           23  //间隔打开GPS的时间
#define TM_OPENGPS_TIME       24  //GPS打开的持续的最长时间
#define TM_OPENGPRS_RF        25  //GPRS进入转发模式后，30S未收到从GPRS发过来的数据，自动退出转发模式
#define TM_OPENDEG_RF        26  //DEG进入转发模式后，30S未收到从DEUG发过来的数据，自动退出转发模式






//==============================IEC 101相关参数======================================

#define IECP_LINKADDR_NUM    0//链路地址长度 1-2    
#define IECP_TRANSRSN_NUM    1//传输原因长度 1-2   
#define IECP_COMNADDR_NUM    2 //公共地址长度 1-2  
#define IECP_INFO_NUM        3//信息体地址长度 1-3 
#define IECP_COM_MODE        4//0为不平衡时，1为平衡式  默认为1
#define IECP_AUT_MODE        5//主动上传模式，0=主站发送初始化后方可主动上传和总召
                              //1=收到主站任何命令后方可主动上传，任何时候都响应总召命令。
                              //2=主动上传不受任何约束
#define IECP_OBJ_COM0        6 //串口0指定的对象，0=RF,1=GPRS,2=DBG,3=EX,4=ODU1,5=ODU2
#define IECP_OBJ_COM1        7
#define IECP_OBJ_COM2        8
#define IECP_OBJ_COM3        9
#define IECP_YC_TYPE         10 //0=归一化值[9],1=标度化值[11],2=短浮点数[13]

#define IECP_CRC             11//crc校验

#define IEC_PARA_NUM    12  //IEC运行的参数个数

#define IEC_YC_NUM  120//RMT_MEAS_NUM  //允许传给主站的遥测个数上限
#define IEC_YX_NUM   400          //RMT_INFO_NUM允许传给主站的遥信个数上限
#define IEC_YK_NUM  100  //OUT_TYPE_NUM  //允许传给主站的遥控个数上限
#define FRM_MAX_COS_NUM   70 //每帧报文最多传输COS个数，规定为70个

  
//============================  遥测量          =======================================

//故障电流，负荷电流，电缆温度，每类遥测个数
#define RM_BAT_U         0 //电池电压
#define RM_CAP_U         1 //电容电压
#define RM_SUN_U         2 //太阳能电压
#define RM_RSV           3 // 改成备用 电池浮电压RM_BATF_U
#define RM_1A_FAULT_I       4//1a故障电流
#define RM_1B_FAULT_I       5
#define RM_1C_FAULT_I       6
#define RM_1O_FAULT_I       7
#define RM_2A_FAULT_I       8
#define RM_2B_FAULT_I       9
#define RM_2C_FAULT_I       10
#define RM_2O_FAULT_I       11
#define RM_3A_FAULT_I       12
#define RM_3B_FAULT_I       13
#define RM_3C_FAULT_I       14
#define RM_3O_FAULT_I       15
/*#define RM_4A_FAULT_I       16
#define RM_4B_FAULT_I       17
#define RM_4C_FAULT_I       18
#define RM_4O_FAULT_I       19
#define RM_5A_FAULT_I       20
#define RM_5B_FAULT_I       21
#define RM_5C_FAULT_I       22
#define RM_5O_FAULT_I       23
#define RM_6A_FAULT_I       24
#define RM_6B_FAULT_I       25
#define RM_6C_FAULT_I       26
#define RM_6O_FAULT_I       27
#define RM_7A_FAULT_I       28
#define RM_7B_FAULT_I       29
#define RM_7C_FAULT_I       30
#define RM_7O_FAULT_I       31
#define RM_8A_FAULT_I       32
#define RM_8B_FAULT_I       33
#define RM_8C_FAULT_I       34
#define RM_8O_FAULT_I       35
#define RM_9A_FAULT_I       36
#define RM_9B_FAULT_I       37
#define RM_9C_FAULT_I       38
#define RM_9O_FAULT_I       39
#define RM_10A_FAULT_I      40
#define RM_10B_FAULT_I      41
#define RM_10C_FAULT_I      42
#define RM_10O_FAULT_I      43
#define RM_11A_FAULT_I      44
#define RM_11B_FAULT_I      45
#define RM_11C_FAULT_I      46
#define RM_11O_FAULT_I      47
#define RM_12A_FAULT_I      48
#define RM_12B_FAULT_I      49
#define RM_12C_FAULT_I      50
#define RM_12O_FAULT_I      51*/

#define RM_1A_LOAD_I        16//52//1a负荷电流
#define RM_1B_LOAD_I        17//53
#define RM_1C_LOAD_I        18//54
#define RM_1O_LOAD_I        19//55
#define RM_2A_LOAD_I        20//56
#define RM_2B_LOAD_I        21//57
#define RM_2C_LOAD_I        22//58
#define RM_2O_LOAD_I        23//59
#define RM_3A_LOAD_I        24//60
#define RM_3B_LOAD_I        25//61
#define RM_3C_LOAD_I        26//62
#define RM_3O_LOAD_I        27//63
/*#define RM_4A_LOAD_I        64
#define RM_4B_LOAD_I        65
#define RM_4C_LOAD_I        66
#define RM_4O_LOAD_I        67
#define RM_5A_LOAD_I        68
#define RM_5B_LOAD_I        69
#define RM_5C_LOAD_I        70
#define RM_5O_LOAD_I        71
#define RM_6A_LOAD_I        72
#define RM_6B_LOAD_I        73
#define RM_6C_LOAD_I        74
#define RM_6O_LOAD_I        75
#define RM_7A_LOAD_I        76
#define RM_7B_LOAD_I        77
#define RM_7C_LOAD_I        78
#define RM_7O_LOAD_I        79
#define RM_8A_LOAD_I        80
#define RM_8B_LOAD_I        81
#define RM_8C_LOAD_I        82
#define RM_8O_LOAD_I        83
#define RM_9A_LOAD_I        84
#define RM_9B_LOAD_I        85
#define RM_9C_LOAD_I        86
#define RM_9O_LOAD_I        87
#define RM_10A_LOAD_I       88
#define RM_10B_LOAD_I       89
#define RM_10C_LOAD_I       90
#define RM_10O_LOAD_I       91
#define RM_11A_LOAD_I       92
#define RM_11B_LOAD_I       93
#define RM_11C_LOAD_I       94
#define RM_11O_LOAD_I       95
#define RM_12A_LOAD_I       96
#define RM_12B_LOAD_I       97
#define RM_12C_LOAD_I       98
#define RM_12O_LOAD_I       99*/


#define RN_1A_BAT_U             28//172           //指示器电池电压，暂时只加3条线路
#define RN_1B_BAT_U             29//173     
#define RN_1C_BAT_U             30//174     
#define RN_2A_BAT_U             31//175     
#define RN_2B_BAT_U             32//176     
#define RN_2C_BAT_U             33//177     
#define RN_3A_BAT_U             34//178     
#define RN_3B_BAT_U             35//179     
#define RN_3C_BAT_U             36//180 

#define RN_1A_CAP_U              37//181   //指示器电场电压，暂时只加3条线路
#define RN_1B_CAP_U              38//182
#define RN_1C_CAP_U              39//183
#define RN_2A_CAP_U              40//184
#define RN_2B_CAP_U              41//185
#define RN_2C_CAP_U              42//186
#define RN_3A_CAP_U              43//187
#define RN_3B_CAP_U              44//188
#define RN_3C_CAP_U              45//189


#define RN_1A_EFIELD_U            46//64//190   //指示器电场电压，暂时只加3条线路
#define RN_1B_EFIELD_U            47//65//191
#define RN_1C_EFIELD_U            48//66//192
#define RN_2A_EFIELD_U            49//67//193
#define RN_2B_EFIELD_U            50//68//194
#define RN_2C_EFIELD_U            51//69//195
#define RN_3A_EFIELD_U            52//70//196
#define RN_3B_EFIELD_U            53//1//197
#define RN_3C_EFIELD_U            54//72//198

#define RM_1A_LUBO       55        //取电电压
#define RM_1B_LUBO       56
#define RM_1C_LUBO       57
//#define RM_1O_LUBO       58
#define RM_2A_LUBO       58
#define RM_2B_LUBO       59
#define RM_2C_LUBO       60
//#define RM_2O_LUBO       62
#define RM_3A_LUBO       61
#define RM_3B_LUBO       62
#define RM_3C_LUBO       63
//#define RM_3O_LUBO       66


/*#define RM_1A_CABLE_T       55//28//100//1a电缆外部温度 
#define RM_1B_CABLE_T       56//29//101
#define RM_1C_CABLE_T       57//30//102
#define RM_2A_CABLE_T       58//31//103
#define RM_2B_CABLE_T       59//32//104
#define RM_2C_CABLE_T       60//33//105
#define RM_3A_CABLE_T       61//34//106
#define RM_3B_CABLE_T       62//35//107
#define RM_3C_CABLE_T       63//36//108
#define RM_4A_CABLE_T       109
#define RM_4B_CABLE_T       110
#define RM_4C_CABLE_T       111
#define RM_5A_CABLE_T       112
#define RM_5B_CABLE_T       113
#define RM_5C_CABLE_T       114
#define RM_6A_CABLE_T       115
#define RM_6B_CABLE_T       116
#define RM_6C_CABLE_T       117
#define RM_7A_CABLE_T       118
#define RM_7B_CABLE_T       119
#define RM_7C_CABLE_T       120
#define RM_8A_CABLE_T       121
#define RM_8B_CABLE_T       122
#define RM_8C_CABLE_T       123
#define RM_9A_CABLE_T       124
#define RM_9B_CABLE_T       125
#define RM_9C_CABLE_T       126
#define RM_10A_CABLE_T      127
#define RM_10B_CABLE_T      128
#define RM_10C_CABLE_T      129
#define RM_11A_CABLE_T      130
#define RM_11B_CABLE_T      131
#define RM_11C_CABLE_T      132
#define RM_12A_CABLE_T      133
#define RM_12B_CABLE_T      134
#define RM_12C_CABLE_T      135*/

#define RM_1A_CABLE_T_INT       64//64//37//136//1a电缆内部温度 架空指示器温度
#define RM_1B_CABLE_T_INT       65//65//38//137
#define RM_1C_CABLE_T_INT       66//66//39//138
#define RM_2A_CABLE_T_INT       67//67//40//139
#define RM_2B_CABLE_T_INT       68//68//41//140
#define RM_2C_CABLE_T_INT       69//69//42//141
#define RM_3A_CABLE_T_INT       70//70//43//142
#define RM_3B_CABLE_T_INT       71//71//44//143
#define RM_3C_CABLE_T_INT       72//72//45//144
/*#define RM_4A_CABLE_T_INT       145
#define RM_4B_CABLE_T_INT       146
#define RM_4C_CABLE_T_INT       147
#define RM_5A_CABLE_T_INT       148
#define RM_5B_CABLE_T_INT       149
#define RM_5C_CABLE_T_INT       150
#define RM_6A_CABLE_T_INT       151
#define RM_6B_CABLE_T_INT       152
#define RM_6C_CABLE_T_INT       153
#define RM_7A_CABLE_T_INT       154
#define RM_7B_CABLE_T_INT       155
#define RM_7C_CABLE_T_INT       156
#define RM_8A_CABLE_T_INT       157
#define RM_8B_CABLE_T_INT       158
#define RM_8C_CABLE_T_INT       159
#define RM_9A_CABLE_T_INT       160
#define RM_9B_CABLE_T_INT       161
#define RM_9C_CABLE_T_INT       162
#define RM_10A_CABLE_T_INT      163
#define RM_10B_CABLE_T_INT      164
#define RM_10C_CABLE_T_INT      165
#define RM_11A_CABLE_T_INT      166
#define RM_11B_CABLE_T_INT      167
#define RM_11C_CABLE_T_INT      168
#define RM_12A_CABLE_T_INT      169
#define RM_12B_CABLE_T_INT      170
#define RM_12C_CABLE_T_INT      171*/

/*#define RN_1A_BAT_U             46//172         //指示器电池电压，暂时只加3条线路
#define RN_1B_BAT_U             47//173     
#define RN_1C_BAT_U             48//174     
#define RN_2A_BAT_U             49//175     
#define RN_2B_BAT_U             50//176     
#define RN_2C_BAT_U             51//177     
#define RN_3A_BAT_U             52//178     
#define RN_3B_BAT_U             53//179     
#define RN_3C_BAT_U             54//180 

#define RN_1A_CAP_U              55//181   //指示器电场电压，暂时只加3条线路
#define RN_1B_CAP_U              56//182
#define RN_1C_CAP_U              57//183
#define RN_2A_CAP_U              58//184
#define RN_2B_CAP_U              59//185
#define RN_2C_CAP_U              60//186
#define RN_3A_CAP_U              61//187
#define RN_3B_CAP_U              62//188
#define RN_3C_CAP_U              63//189


#define RN_1A_EFIELD_U            64//190   //指示器电场电压，暂时只加3条线路
#define RN_1B_EFIELD_U            65//191
#define RN_1C_EFIELD_U            66//192
#define RN_2A_EFIELD_U            67//193
#define RN_2B_EFIELD_U            68//194
#define RN_2C_EFIELD_U            69//195
#define RN_3A_EFIELD_U            70//196
#define RN_3B_EFIELD_U            71//197
#define RN_3C_EFIELD_U            72//198*/


/*#define REPEATER1_BAT_U      190//中继器电池电压
#define REPEATER2_BAT_U      191
#define REPEATER3_BAT_U      192
#define REPEATER4_BAT_U      193
#define REPEATER5_BAT_U      194
#define REPEATER6_BAT_U      195
#define REPEATER7_BAT_U      196
#define REPEATER8_BAT_U      197
#define REPEATER9_BAT_U      198
#define REPEATER10_BAT_U      199
#define REPEATER11_BAT_U      200
#define REPEATER12_BAT_U      201

#define REPEATER1_CAP_U      202//中继器电容电压
#define REPEATER2_CAP_U      203
#define REPEATER3_CAP_U      204
#define REPEATER4_CAP_U      205
#define REPEATER5_CAP_U      206
#define REPEATER6_CAP_U      207
#define REPEATER7_CAP_U      208
#define REPEATER8_CAP_U      209
#define REPEATER9_CAP_U      210
#define REPEATER10_CAP_U      211
#define REPEATER11_CAP_U      212
#define REPEATER12_CAP_U      213*/

#define RM_LUBO_NUM        73//72//45//144
#define RM_ALLLUBO_NUM     74

#define RMT_MEAS_NUM       75//199
#define RM_TEMP_START      RM_1A_CABLE_T_INT//RM_1A_CABLE_T//温度遥测开始的起始位置
#define RM_LOADI_NUM       LINE_NUM * 4 //负荷电流个数
#define LOAD_LEN           RM_1A_LUBO*2+8//RM_TEMP_START*2 + 7 //RM_LOADI_NUM*2 + 7 //单条负荷记录的长度，单位字节，包括起始码(1字节)、长度(1字节)、时间(5字节)


//故障电流存放顺序为1A,1B,1C,1O,...XA,XB,XC,XO




//============================  遥信量          =======================================
//        | 短路 |接地 |低压 |温度 |电场有电 |备用 |通信状态 |相故障(短路||接地)|
//------------------------------------------------------------------------------------
//     A相| C/A  |   A | C/A | C/A |   C/A   | C/A | C/A     |   C/A            |                  |
//------------------------------------------------------------------------------------
//     B相| C/A  |   A | C/A | C/A |   C/A   | C/A | C/A     |   C/A            |     
//------------------------------------------------------------------------------------
//     C相| C/A  |   A | C/A | C/A |   C/A   | C/A | C/A     |   C/A            |     
//------------------------------------------------------------------------------------
//     O相|  -   |   C | -   |  -  |   -     | -   |  C      |   C              |     
//------------------------------------------------------------------------------------
//      线|      |     |     |     |         |     |         |                  |
//------------------------------------------------------------------------------------
//架空没有O序相，电缆接地只在O序
//A相故障 = A短路 || A接地 ;   （B\C\O相同）
//线短路  = A短路 || B短路 || C短路；
//线接地  = A接地 || B接地 || C接地||O接地；
//线总故障 = A相故障 || B相故障 ||C相故障 ||0相故障 ；


//瞬时性短路，接地，低压(指示器)，温度，电流超限(有电(电场))，永久性短路、遥控翻牌状态、有电无电。通信状态，相故障(短路||接地)，每类的遥信个数
//实点遥信(报文解析的遥信)
#define RI_1A_SHORT     0
#define RI_1B_SHORT     1
#define RI_1C_SHORT     2
#define RI_1O_SHORT     3
#define RI_2A_SHORT     4
#define RI_2B_SHORT     5
#define RI_2C_SHORT     6
#define RI_2O_SHORT     7
#define RI_3A_SHORT     8
#define RI_3B_SHORT     9
#define RI_3C_SHORT     10
#define RI_3O_SHORT     11
/*#define RI_4A_SHORT   12
#define RI_4B_SHORT     13
#define RI_4C_SHORT     14
#define RI_4O_SHORT     15
#define RI_5A_SHORT     16
#define RI_5B_SHORT     17
#define RI_5C_SHORT     18
#define RI_5O_SHORT     19
#define RI_6A_SHORT     20
#define RI_6B_SHORT     21
#define RI_6C_SHORT     22
#define RI_6O_SHORT     23
#define RI_7A_SHORT     24
#define RI_7B_SHORT     25
#define RI_7C_SHORT     26
#define RI_7O_SHORT     27
#define RI_8A_SHORT     28
#define RI_8B_SHORT     29
#define RI_8C_SHORT     30
#define RI_8O_SHORT     31
#define RI_9A_SHORT     32
#define RI_9B_SHORT     33
#define RI_9C_SHORT     34
#define RI_9O_SHORT     35
#define RI_10A_SHORT        36
#define RI_10B_SHORT        37
#define RI_10C_SHORT        38
#define RI_10O_SHORT        39
#define RI_11A_SHORT        40
#define RI_11B_SHORT        41
#define RI_11C_SHORT        42
#define RI_11O_SHORT        43
#define RI_12A_SHORT        44
#define RI_12B_SHORT        45
#define RI_12C_SHORT        46
#define RI_12O_SHORT        47*/
#define RI_1A_GND   12//48
#define RI_1B_GND   13//49
#define RI_1C_GND   14//50
#define RI_1O_GND   15//51
#define RI_2A_GND   16//52
#define RI_2B_GND   17//53
#define RI_2C_GND   18//54
#define RI_2O_GND   19//55
#define RI_3A_GND   20//56
#define RI_3B_GND   21//57
#define RI_3C_GND   22//58
#define RI_3O_GND   23//59
/*#define RI_4A_GND     60
#define RI_4B_GND   61
#define RI_4C_GND   62
#define RI_4O_GND   63
#define RI_5A_GND   64
#define RI_5B_GND   65
#define RI_5C_GND   66
#define RI_5O_GND   67
#define RI_6A_GND   68
#define RI_6B_GND   69
#define RI_6C_GND   70
#define RI_6O_GND   71
#define RI_7A_GND   72
#define RI_7B_GND   73
#define RI_7C_GND   74
#define RI_7O_GND   75
#define RI_8A_GND   76
#define RI_8B_GND   77
#define RI_8C_GND   78
#define RI_8O_GND   79
#define RI_9A_GND   80
#define RI_9B_GND   81
#define RI_9C_GND   82
#define RI_9O_GND   83
#define RI_10A_GND      84
#define RI_10B_GND      85
#define RI_10C_GND      86
#define RI_10O_GND      87
#define RI_11A_GND      88
#define RI_11B_GND      89
#define RI_11C_GND      90
#define RI_11O_GND      91
#define RI_12A_GND      92
#define RI_12B_GND      93
#define RI_12C_GND      94
#define RI_12O_GND      95*/
#define RI_1A_LVD   24//96
#define RI_1B_LVD   25//97
#define RI_1C_LVD   26//98
#define RI_1O_LVD   27//99
#define RI_2A_LVD   28//100
#define RI_2B_LVD   29//101
#define RI_2C_LVD   30//102
#define RI_2O_LVD   31//103
#define RI_3A_LVD   32//104
#define RI_3B_LVD   33//105
#define RI_3C_LVD   34//106
#define RI_3O_LVD   35//107
/*#define RI_4A_LVD     108
#define RI_4B_LVD   109
#define RI_4C_LVD   110
#define RI_4O_LVD   111
#define RI_5A_LVD   112
#define RI_5B_LVD   113
#define RI_5C_LVD   114
#define RI_5O_LVD   115
#define RI_6A_LVD   116
#define RI_6B_LVD   117
#define RI_6C_LVD   118
#define RI_6O_LVD   119
#define RI_7A_LVD   120
#define RI_7B_LVD   121
#define RI_7C_LVD   122
#define RI_7O_LVD   123
#define RI_8A_LVD   124
#define RI_8B_LVD   125
#define RI_8C_LVD   126
#define RI_8O_LVD   127
#define RI_9A_LVD   128
#define RI_9B_LVD   129
#define RI_9C_LVD   130
#define RI_9O_LVD   131
#define RI_10A_LVD      132
#define RI_10B_LVD      133
#define RI_10C_LVD      134
#define RI_10O_LVD      135
#define RI_11A_LVD      136
#define RI_11B_LVD      137
#define RI_11C_LVD      138
#define RI_11O_LVD      139
#define RI_12A_LVD      140
#define RI_12B_LVD      141
#define RI_12C_LVD      142
#define RI_12O_LVD      143*/
#define RI_1A_CABT  36//144
#define RI_1B_CABT  37//145
#define RI_1C_CABT  38//146
#define RI_1O_CABT  39//147
#define RI_2A_CABT  40//148
#define RI_2B_CABT  41//149
#define RI_2C_CABT  42//150
#define RI_2O_CABT  43//151
#define RI_3A_CABT  44//152
#define RI_3B_CABT  45//153
#define RI_3C_CABT  46//154
#define RI_3O_CABT  47//155
/*#define RI_4A_CABT    156
#define RI_4B_CABT  157
#define RI_4C_CABT  158
#define RI_4O_CABT  159
#define RI_5A_CABT  160
#define RI_5B_CABT  161
#define RI_5C_CABT  162
#define RI_5O_CABT  163
#define RI_6A_CABT  164
#define RI_6B_CABT  165
#define RI_6C_CABT  166
#define RI_6O_CABT  167
#define RI_7A_CABT  168
#define RI_7B_CABT  169
#define RI_7C_CABT  170
#define RI_7O_CABT  171
#define RI_8A_CABT  172
#define RI_8B_CABT  173
#define RI_8C_CABT  174
#define RI_8O_CABT  175
#define RI_9A_CABT  176
#define RI_9B_CABT  177
#define RI_9C_CABT  178
#define RI_9O_CABT  179
#define RI_10A_CABT     180
#define RI_10B_CABT     181
#define RI_10C_CABT     182
#define RI_10O_CABT     183
#define RI_11A_CABT     184
#define RI_11B_CABT     185
#define RI_11C_CABT     186
#define RI_11O_CABT     187
#define RI_12A_CABT     188
#define RI_12B_CABT     189
#define RI_12C_CABT     190
#define RI_12O_CABT     191*/
#define RI_1A_ELECON    48//192电流越限
#define RI_1B_ELECON    49//193
#define RI_1C_ELECON    50//194
#define RI_1O_ELECON    51//195
#define RI_2A_ELECON    52//196
#define RI_2B_ELECON    53//197
#define RI_2C_ELECON    54//198
#define RI_2O_ELECON    55//199
#define RI_3A_ELECON    56//200
#define RI_3B_ELECON    57//201
#define RI_3C_ELECON    58//202
#define RI_3O_ELECON    59//203
/*#define RI_4A_ELECON  204
#define RI_4B_ELECON    205
#define RI_4C_ELECON    206
#define RI_4O_ELECON    207
#define RI_5A_ELECON    208
#define RI_5B_ELECON    209
#define RI_5C_ELECON    210
#define RI_5O_ELECON    211
#define RI_6A_ELECON    212
#define RI_6B_ELECON    213
#define RI_6C_ELECON    214
#define RI_6O_ELECON    215
#define RI_7A_ELECON    216
#define RI_7B_ELECON    217
#define RI_7C_ELECON    218
#define RI_7O_ELECON    219
#define RI_8A_ELECON    220
#define RI_8B_ELECON    221
#define RI_8C_ELECON    222
#define RI_8O_ELECON    223
#define RI_9A_ELECON    224
#define RI_9B_ELECON    225
#define RI_9C_ELECON    226
#define RI_9O_ELECON    227
#define RI_10A_ELECON   228
#define RI_10B_ELECON   229
#define RI_10C_ELECON   230
#define RI_10O_ELECON   231
#define RI_11A_ELECON   232
#define RI_11B_ELECON   233
#define RI_11C_ELECON   234
#define RI_11O_ELECON   235
#define RI_12A_ELECON   236
#define RI_12B_ELECON   237
#define RI_12C_ELECON   238
#define RI_12O_ELECON   239*/
#define RI_1A_RSV   60//240永久短路
#define RI_1B_RSV   61//241
#define RI_1C_RSV   62//242
#define RI_1O_RSV   63//243
#define RI_2A_RSV   64//244
#define RI_2B_RSV   65//245
#define RI_2C_RSV   66//246
#define RI_2O_RSV   67//247
#define RI_3A_RSV   68//248
#define RI_3B_RSV   69//249
#define RI_3C_RSV   70//250
#define RI_3O_RSV   71//251


//短路故障
#define RI_1A_TOTALSHORT    72//60//240短路
#define RI_1B_TOTALSHORT    73//61//241
#define RI_1C_TOTALSHORT    74//62//242
#define RI_1O_TOTALSHORT    75//63//243
#define RI_2A_TOTALSHORT    76//64//244
#define RI_2B_TOTALSHORT    77//65//245
#define RI_2C_TOTALSHORT    78//66//246
#define RI_2O_TOTALSHORT    79//67//247
#define RI_3A_TOTALSHORT    80//68//248
#define RI_3B_TOTALSHORT    81//69//249
#define RI_3C_TOTALSHORT    82//70//250
#define RI_3O_TOTALSHORT    83//71//251


//指示器遥控翻牌复归遥信
#define RI_1A_FLOP  84//72//251
#define RI_1B_FLOP  85//73//251
#define RI_1C_FLOP  86//74//251
#define RI_1O_FLOP  87//75//251

#define RI_2A_FLOP  88//76//251
#define RI_2B_FLOP  89//77//251
#define RI_2C_FLOP  90//78//251
#define RI_2O_FLOP  91//79//251

#define RI_3A_FLOP  92//80//251
#define RI_3B_FLOP  93//81//251
#define RI_3C_FLOP  94//82//251
#define RI_3O_FLOP  95//83//251
//停电遥信
#define RI_1A_POWERCUT  96//84//251
#define RI_1B_POWERCUT  97//    85//251
#define RI_1C_POWERCUT  98//    86//251
#define RI_1O_POWERCUT  99//    87//251

#define RI_2A_POWERCUT  100//   88//251
#define RI_2B_POWERCUT  101//   89//251
#define RI_2C_POWERCUT  102//   90//251
#define RI_2O_POWERCUT  103//   91//251

#define RI_3A_POWERCUT  104//   92//251
#define RI_3B_POWERCUT  105//=  93//251
#define RI_3C_POWERCUT  106//   94//251
#define RI_3O_POWERCUT  107//   95//251

/*#define RI_4A_RSV     252
#define RI_4B_RSV   253
#define RI_4C_RSV   254
#define RI_4O_RSV   255
#define RI_5A_RSV   256
#define RI_5B_RSV   257
#define RI_5C_RSV   258
#define RI_5O_RSV   259
#define RI_6A_RSV   260
#define RI_6B_RSV   261
#define RI_6C_RSV   262
#define RI_6O_RSV   263
#define RI_7A_RSV   264
#define RI_7B_RSV   265
#define RI_7C_RSV   266
#define RI_7O_RSV   267
#define RI_8A_RSV   268
#define RI_8B_RSV   269
#define RI_8C_RSV   270
#define RI_8O_RSV   271
#define RI_9A_RSV   272
#define RI_9B_RSV   273
#define RI_9C_RSV   274
#define RI_9O_RSV   275
#define RI_10A_RSV      276
#define RI_10B_RSV      277
#define RI_10C_RSV      278
#define RI_10O_RSV      279
#define RI_11A_RSV      280
#define RI_11B_RSV      281
#define RI_11C_RSV      282
#define RI_11O_RSV      283
#define RI_12A_RSV      284
#define RI_12B_RSV      285
#define RI_12C_RSV      286
#define RI_12O_RSV      287*/

//总事故信息(在终端合成)
//每条线路总的瞬时性短路故障
#define RI_1SHORT       108//96//72//288
#define RI_2SHORT       109//97//73//289
#define RI_3SHORT       110//98//74//290
/*#define RI_4SHORT     291
#define RI_5SHORT       292
#define RI_6SHORT       293
#define RI_7SHORT       294
#define RI_8SHORT       295
#define RI_9SHORT       296
#define RI_10SHORT      297
#define RI_11SHORT      298
#define RI_12SHORT  299*/
#define RI_1GND     111//99//75//300
#define RI_2GND     112//100//76//301
#define RI_3GND     113//101//77//302
/*#define RI_4GND       303
#define RI_5GND     304
#define RI_6GND     305
#define RI_7GND     306
#define RI_8GND     307
#define RI_9GND     308
#define RI_10GND    309
#define RI_11GND    310
#define RI_12GND    311*/
#define RI_1LVD     114//102//78//312
#define RI_2LVD     115//103//79//313
#define RI_3LVD     116//104//80//314
/*#define RI_4LVD       315
#define RI_5LVD     316
#define RI_6LVD     317
#define RI_7LVD     318
#define RI_8LVD     319
#define RI_9LVD     320
#define RI_10LVD    321
#define RI_11LVD    322
#define RI_12LVD    323*/
#define RI_1CABT    117//105//81//324
#define RI_2CABT    118//106///82//325
#define RI_3CABT    119//107//83//326
/*#define RI_4CABT      327
#define RI_5CABT    328
#define RI_6CABT    329
#define RI_7CABT    330
#define RI_8CABT    331
#define RI_9CABT    332
#define RI_10CABT   333
#define RI_11CABT   334
#define RI_12CABT   335*/
#define RI_1ELECON   120//  108//84//336
#define RI_2ELECON   121//  109//85//337
#define RI_3ELECON   122//  110//86//338
/*#define RI_4ELECON    339
#define RI_5ELECON      340
#define RI_6ELECON      341
#define RI_7ELECON      342
#define RI_8ELECON      343
#define RI_9ELECON      344
#define RI_10ELECON     345
#define RI_11ELECON     346
#define RI_12ELECON 347*/
//每条线路总的永久性短路故障
#define RI_1RSV     123//111//87//348
#define RI_2RSV     124//112//88//349
#define RI_3RSV     125//113//89//350

/*#define RI_4RSV       351
#define RI_5RSV     352
#define RI_6RSV     353
#define RI_7RSV     354
#define RI_8RSV     355
#define RI_9RSV     356
#define RI_10RSV    357
#define RI_11RSV    358
#define RI_12RSV    359*/
//每条线路总的短路故障，不管是瞬时还是永久
#define RI_1_TOTALSHORT 126
#define RI_2_TOTALSHORT 127
#define RI_3_TOTALSHORT 128


#define RI_1FLOP    129//114//87//348
#define RI_2FLOP    130//115//88//349
#define RI_3FLOP    131//116//89//350

#define RI_1POWERCUT 132//117
#define RI_2POWERCUT 133//118
#define RI_3POWERCUT 134//119


//状态信息
//线路通信状态(架空时在终端合成)
#define RI_1ZSQST           135//120//114//90//360
#define RI_2ZSQST           136//121//115//91//361
#define RI_3ZSQST           137//122//116//92//362
/*#define RI_4ZSQST         363
#define RI_5ZSQST           364
#define RI_6ZSQST           365
#define RI_7ZSQST           366
#define RI_8ZSQST           367
#define RI_9ZSQST           368
#define RI_10ZSQST          369
#define RI_11ZSQST          370
#define RI_12ZSQST          371*/
#define RI_1A_ZSQST         138//123//117//93//372
#define RI_1B_ZSQST         139//124//118//94//373
#define RI_1C_ZSQST         140//125//119//95//374
#define RI_1O_ZSQST         141//126//120//96//375
#define RI_2A_ZSQST         142//127//121//97//376
#define RI_2B_ZSQST         143//128//122//98//377
#define RI_2C_ZSQST         144//129//123////378
#define RI_2O_ZSQST         145//130//124//100//379
#define RI_3A_ZSQST         146//131//125//101//380
#define RI_3B_ZSQST         147//132//126////381
#define RI_3C_ZSQST         148//133//127//103//382
#define RI_3O_ZSQST         149//134//128//104//383
/*#define RI_4A_ZSQST       384
#define RI_4B_ZSQST         385
#define RI_4C_ZSQST         386
#define RI_4O_ZSQST         387
#define RI_5A_ZSQST         388
#define RI_5B_ZSQST         389
#define RI_5C_ZSQST         390
#define RI_5O_ZSQST         391
#define RI_6A_ZSQST         392
#define RI_6B_ZSQST         393
#define RI_6C_ZSQST         394
#define RI_6O_ZSQST         395
#define RI_7A_ZSQST         396
#define RI_7B_ZSQST         397
#define RI_7C_ZSQST         398
#define RI_7O_ZSQST         399
#define RI_8A_ZSQST         400
#define RI_8B_ZSQST         401
#define RI_8C_ZSQST         402
#define RI_8O_ZSQST         403
#define RI_9A_ZSQST         404
#define RI_9B_ZSQST         405
#define RI_9C_ZSQST         406
#define RI_9O_ZSQST         407
#define RI_10A_ZSQST    408
#define RI_10B_ZSQST    409
#define RI_10C_ZSQST    410
#define RI_10O_ZSQST    411
#define RI_11A_ZSQST    412
#define RI_11B_ZSQST    413
#define RI_11C_ZSQST    414
#define RI_11O_ZSQST    415
#define RI_12A_ZSQST    416
#define RI_12B_ZSQST    417
#define RI_12C_ZSQST    418
#define RI_12O_ZSQST    419*/


//相总(相故障 在终端合成)
#define RI_1FLT         150//135//129//105//420
#define RI_2FLT         151//136//130//106//421
#define RI_3FLT         152//137//131//107//422
/*#define RI_4FLT           423
#define RI_5FLT         424
#define RI_6FLT         425
#define RI_7FLT         426
#define RI_8FLT         427
#define RI_9FLT         428
#define RI_10FLT        429
#define RI_11FLT        430
#define RI_12FLT        431*/
#define RI_1A_FLT       153//138//132//108//432
#define RI_1B_FLT       154//139//133//433
#define RI_1C_FLT       155//140//134//434
#define RI_1O_FLT       156//141//135//435
#define RI_2A_FLT       157//142//136//436
#define RI_2B_FLT       158//143//137//437
#define RI_2C_FLT       159//144//138//438
#define RI_2O_FLT       160//145//139//439
#define RI_3A_FLT       161//146//140//440
#define RI_3B_FLT       162//147//141//441
#define RI_3C_FLT       163//148//142//442
#define RI_3O_FLT       164//149//143//443
/*#define RI_4A_FLT         444
#define RI_4B_FLT       445
#define RI_4C_FLT       446
#define RI_4O_FLT       447
#define RI_5A_FLT       448
#define RI_5B_FLT       449
#define RI_5C_FLT       450
#define RI_5O_FLT       451
#define RI_6A_FLT       452
#define RI_6B_FLT       453
#define RI_6C_FLT       454
#define RI_6O_FLT       455
#define RI_7A_FLT       456
#define RI_7B_FLT       457
#define RI_7C_FLT       458
#define RI_7O_FLT       459
#define RI_8A_FLT       460
#define RI_8B_FLT       461
#define RI_8C_FLT       462
#define RI_8O_FLT       463
#define RI_9A_FLT       464
#define RI_9B_FLT       465
#define RI_9C_FLT       466
#define RI_9O_FLT       467
#define RI_10A_FLT    468
#define RI_10B_FLT    469
#define RI_10C_FLT    470
#define RI_10O_FLT    471
#define RI_11A_FLT    472
#define RI_11B_FLT    473
#define RI_11C_FLT    474
#define RI_11O_FLT    475
#define RI_12A_FLT    476
#define RI_12B_FLT    477
#define RI_12C_FLT    478
#define RI_12O_FLT    479*/

//翻牌状态 贵州鲁甸联调使用
/*#define RI_1A_STATUS  480
#define RI_1B_STATUS  481
#define RI_1C_STATUS  482
#define RI_1O_STATUS  483
#define RI_2A_STATUS  484
#define RI_2B_STATUS  485
#define RI_2C_STATUS  486
#define RI_2O_STATUS  487
#define RI_3A_STATUS  488
#define RI_3B_STATUS  489
#define RI_3C_STATUS  490
#define RI_3O_STATUS  491
#define RI_4A_STATUS  492
#define RI_4B_STATUS  493
#define RI_4C_STATUS  494
#define RI_4O_STATUS  495
#define RI_5A_STATUS  496
#define RI_5B_STATUS  497
#define RI_5C_STATUS  498
#define RI_5O_STATUS  499
#define RI_6A_STATUS  500
#define RI_6B_STATUS  501
#define RI_6C_STATUS  502
#define RI_6O_STATUS  503*/

//采集单元电池投入
#define RI_1_BATPOW_EN          165//
#define RI_2_BATPOW_EN          166//
#define RI_3_BATPOW_EN          167//
#define RI_BATPOW_EN                168//

#define RI_1A_BATPOW_EN         169//1A电池启用
#define RI_1B_BATPOW_EN     170//
#define RI_1C_BATPOW_EN         171//
#define RI_1O_BATPOW_EN     172//
#define RI_2A_BATPOW_EN     173//
#define RI_2B_BATPOW_EN     174//
#define RI_2C_BATPOW_EN     175//
#define RI_2O_BATPOW_EN     176//
#define RI_3A_BATPOW_EN     177//
#define RI_3B_BATPOW_EN     178//
#define RI_3C_BATPOW_EN     179//
#define RI_3O_BATPOW_EN     180//

//事故总
#define RI_TOTAL       181//150//144//480//504


#define RI_ZD_LVD      182//151//145//481//505    //子站主电源低压告警
#define RI_ZD_BVD      183//152//146//482//506    //子站备用电源低压告警
#define RI_ZD_ZJ       184//153//147//483//507    //终端自检
#define RI_REAL_QUASI  185//154//148//484//508    //准实时、实时模式切换
#define RI_EEP_ERR     186//155//149//485//509    //E2错误
#define RI_IN_DAERR    187//156//150//486//510    //遥信点号错误(内部使用)
#define RI_STATE_CYCLE 188//157//151//487//511    //状态汇报周期

#define RI_LUBO_FLAG   189//158//152//488//511    //录波标志位
#define RI_GPS_ERR     190//159//153//489//511    //GPS状态异常
#define RI_RF_ERR      191//160//154//489//511    //RF对时脉冲异常




//硬遥信 (硬遥信必须放到遥信表的最后)
//单点 
#define RI_ZD_YX1    192//161//155//490//512    
#define RI_ZD_YX2    193//162//156//491//513    
#define RI_ZD_YX3    194//163//157//492//514    
#define RI_ZD_YX4    195//164//158//493//515    
#define RI_ZD_YX5    196//165//159//494//516 
#define RI_ZD_YX6    197//166//160//495//517 
#define RI_ZD_YX7    198//167//161//496//518 
#define RI_ZD_YX8    199//168//162//497//519 
#define RI_ZD_YX9    200//169//163//498//520 
#define RI_ZD_YX10   201//170//164//499//521
#define RI_ZD_YX11   202//171//165//500//522   
#define RI_ZD_YX12   203////166//501//523    
#define RI_ZD_YX13   204//174//168//503//525    
#define RI_ZD_YX15   205//175//169//504//526    
#define RI_ZD_YX16   206//176//170//505//527    
#define RI_ZD_YX17   207//177//171//506//528    
#define RI_ZD_YX18   208//178//172//507//529    
#define RI_ZD_YX19   209//179//173//508//530    
#define RI_ZD_YX20   210//180//174//509//531    
#define RI_ZD_YX21   211//181//175//510//532    
#define RI_ZD_YX22   212//182//176//511//533    
#define RI_ZD_YX23   213//183//177//512//534    
#define RI_ZD_YX24   214//184//178//513//535

//双点
#define RI_ZD_DYX1   215 //     185//179//514//536   
#define RI_ZD_DYX2   216 //     186//180//515//537   
#define RI_ZD_DYX3   217 //     187//181//516//538   
#define RI_ZD_DYX4   218 //     188//182//517//539   
#define RI_ZD_DYX5   219 //     189//183//518//540   
#define RI_ZD_DYX6   220 //     190//184//519//541   
#define RI_ZD_DYX7   221 // 191//185//520//542   
#define RI_ZD_DYX8   222 //     192////521//543   
#define RI_ZD_DYX9   223 //     193//187//522//544   
#define RI_ZD_DYX10  224 // 194//188//523//545   
#define RI_ZD_DYX11  225 //     195//189//524//546   
#define RI_ZD_DYX12  226  //  196//190//525//547




#define RMT_INFO_NUM           227//197//191//526//548         //总遥信个数
#define RMT_WORD_NUM           RMT_INFO_NUM/8+1          //总遥信占用WORD个数 每个遥信占2个bit，一个WORD保存8个遥信
#define RMT_WDYX_NUM            2                        // 12 双点遥信占的字 12/8+1
#define RMT_WSYX_NUM            RMT_INFO_NUM/8-1          //RMT_INFO_NUM - 12 单点遥信字个数 (RMT_INFO_NUM - 12)/8 +1

#define RMT_ZSQST_NUM          LINE_NUM * PHASE_NUM    //线路通信状态个数  12*4
#define RI_ZSQST_H             RI_3O_ZSQST//RI_12O_ZSQST           //指示器状态遥信上限
#define DYX_ADDR               RI_ZD_DYX1             //双点遥信起始号
#define RI_FTINFO_NUM          LINE_NUM * PHASE_NUM * FTTPYE_NUM//需要闭锁的遥信个数 短路(5*12)+接地(5*12)+温度(5*12)
#define RMT_SS_YY              60//对应各相瞬时故障的位置与永久故障位置的差
#define RI_FLT_NUM             LINE_NUM*PHASE_NUM    //配置的指示器总个数，如3线，每线4只指示器



#define EXT_DIN_NUM                 24    //总共的开关量输入个数


#define FTTPYE_SGL   0 //瞬时短路
#define FTTPYE_JD    1 //接地
#define FTTPYE_LOWB  2 //低压
#define FTTPYE_TEM   3 //温度
#define FTTPYE_CX    4 //电流超限
#define FTTPYE_YGL   5 //永久短路
#define FTTPYE_GL    6 //短路
#define FTTPYE_FP    7 //遥控翻牌状态
#define FTTPYE_YD    8 //有电
#define FTTPYE_ST    9 //通道状态(终端判断)
//#define FTTPYE_XFLT  10 //相故障(终端合成)
//#define FTTPYE_YYJD  11 //有源接地
//#define FTTPYE_WYJD  12 //无源接地
//#define FTTPYE_WD    13 //无电


//#define FTTPYE_NUM   10 //瞬时性短路，接地，低压，温度，电流超限，永久性短路、短路、遥控翻牌状态、有电、通道状态、相故障、//有源接地、无源接地、无电




//============================  实时时钟        ==================================
#define RTC_NUM                         8


#define RTC_MICROSEC                    0       //毫秒
#define RTC_SEC                         1       //秒
#define RTC_MINUT                       2       //分
#define RTC_HOUR                        3       //时
#define RTC_WEEK                        4       //星期
#define RTC_DATE                        5       //日
#define RTC_MONTH                       6       //月
#define RTC_YEAR                        7       //年

//遥控选择后等待执行时间
#define REL_WAIT_TIME_101   120  
#define REL_WAIT_TIME_SMS       600          //短信控制用  10分钟




//===============================  COMM模块  =============================
#define COMM_PORT_NUM           4  //物理通信端口个数

#define CMID0                   0   //RF或ODU1
#define CMID1                   1   //GPRS
#define CMID2                   2   //扩展串口或ODU2
#define CMID3                   3   //调试串口
#define CMIDINV                 4   //无效通道


#define OBJRF       0  
#define OBJGPRS     1
#define OBJDBG      2   
#define OBJEX       6      
#define OBJODU1     4 
#define OBJODU2     5 
#define OBJGPS      3




#define MAX_TXBUFF_LEN       256//无论使用哪一种协议，最后发送时使用的缓冲区相同，因此缓冲区长度取最大值。
#define MAX_RXBUFF_LEN       384//无论使用哪一种协议，最后发送时使用的缓冲区相同，因此缓冲区长度取最大值。

#define MAX_TXBUFF_101_LEN       256 //101协议发送缓冲区长度
#define MAX_SOE_TXNUM            8   //16//单次上送SOE条数





//============================  SOE记录 ===========================================
#define         SOE_BUFF_NUM            32      //SOE内存缓冲区中SOE的条数
#define         DSOE_BUFF_NUM           8      //双点SOE内存缓冲区中SOE的条数
//#define         SOE_DATA_LEN            11       //每条SOE数据的长度
#define         SOE_DATA_LEN           26        //每条SOE数据的长度 增加了5组遥测，每组3个字节
#define         SOEVALID               0X55
#define         SOE_SENDDA_LEN         10//SOE_DATA_LEN-1
//SOE数据的定义
#define         SOE_FLAG    0  
#define         SOE_TYPEL   1 
#define         SOE_TYPEH   2 
#define         SOE_STVAL   3 
#define         SOE_MSL     4 
#define         SOE_MSH     5
#define         SOE_MINU    6 
#define         SOE_HOUR     7
#define         SOE_DAY     8
#define         SOE_MONTH   9  
#define         SOE_YEAR    10  


////================================  遥控出口 =======================

#define OUT_RMT1            0  //终端继电器出口1
#define OUT_RMT2            1  //终端继电器出口2
#define OUT_CLEAR_SOE       2    //清除SOE记录
#define OUT_CLEAR_LOAD      3   //清除负荷数据
#define OUT_GPRS_PW         4    //打开/关断GPRS电源
#define OUT_BAT_ACT         5        //打开/关断电池活化
#define OUT_CLEAR_COMTRADE  6  //清除录波记录

#define OUT_ODU_RESET       7  //YK指示器同组触发录波
#define OUT_ODU2_RESET      8  //清除LOG日志
#define OUT_GPS_PW          9  //打开/关断GPS电源
#define OUT_CALL_YC         10 //召唤指示器的遥测
#define OUT_TEST_MODE       11   //广播呼叫指示器进入自动化测试模式
#define OUT_SET_MODE        12   //广播呼叫指示器进入遥测整定模式
#define OUT_RST_ALL1        13           //线路1总翻牌/复归
#define OUT_RST_ALL2        14           //线路2总翻牌/复归
#define OUT_RST_ALL3        15           //线路3总翻牌/复归


#define OUT_RST_ZSQ1        16   //指示器1翻牌/复归
#define OUT_RST_ZSQ2        17      
#define OUT_RST_ZSQ3        18  
#define OUT_RST_ZSQ4        19
#define OUT_RST_ZSQ5        20  
#define OUT_RST_ZSQ6        21  
#define OUT_RST_ZSQ7        22  
#define OUT_RST_ZSQ8        23  
#define OUT_RST_ZSQ9        24



/*#define OUT_RST_ZSQ10       22  
#define OUT_RST_ZSQ11       23  
#define OUT_RST_ZSQ12       24
#define OUT_RST_ZSQ13       22
#define OUT_RST_ZSQ14       23
#define OUT_RST_ZSQ15       24
#define OUT_RST_ZSQ16       25
#define OUT_RST_ZSQ17       26
#define OUT_RST_ZSQ18       27
#define OUT_RST_ZSQ19       28
#define OUT_RST_ZSQ20       29
#define OUT_RST_ZSQ21       30
#define OUT_RST_ZSQ22       31
#define OUT_RST_ZSQ23       32
#define OUT_RST_ZSQ24       33
#define OUT_RST_ZSQ25       34
#define OUT_RST_ZSQ26       35
#define OUT_RST_ZSQ27       36
#define OUT_RST_ZSQ28       37
#define OUT_RST_ZSQ29       38
#define OUT_RST_ZSQ30       39
#define OUT_RST_ZSQ31       40
#define OUT_RST_ZSQ32       41
#define OUT_RST_ZSQ33       42
#define OUT_RST_ZSQ34       43
#define OUT_RST_ZSQ35       44
#define OUT_RST_ZSQ36       45
//召指示器遥测
#define OUT_RYC_ZSQ1        46
#define OUT_RYC_ZSQ2        47
#define OUT_RYC_ZSQ3        48
#define OUT_RYC_ZSQ4        49
#define OUT_RYC_ZSQ5        50
#define OUT_RYC_ZSQ6        51
#define OUT_RYC_ZSQ7        52
#define OUT_RYC_ZSQ8        53
#define OUT_RYC_ZSQ9        54
#define OUT_RYC_ZSQ10       55
#define OUT_RYC_ZSQ11       56
#define OUT_RYC_ZSQ12       57
#define OUT_RYC_ZSQ13       58
#define OUT_RYC_ZSQ14       59
#define OUT_RYC_ZSQ15       60
#define OUT_RYC_ZSQ16       61
#define OUT_RYC_ZSQ17       62
#define OUT_RYC_ZSQ18       63
#define OUT_RYC_ZSQ19       64
#define OUT_RYC_ZSQ20       65
#define OUT_RYC_ZSQ21       66
#define OUT_RYC_ZSQ22       67
#define OUT_RYC_ZSQ23       68
#define OUT_RYC_ZSQ24       69
#define OUT_RYC_ZSQ25       70
#define OUT_RYC_ZSQ26       71
#define OUT_RYC_ZSQ27       72
#define OUT_RYC_ZSQ28       73
#define OUT_RYC_ZSQ29       74
#define OUT_RYC_ZSQ30       75
#define OUT_RYC_ZSQ31       76
#define OUT_RYC_ZSQ32       77
#define OUT_RYC_ZSQ33       78
#define OUT_RYC_ZSQ34       79
#define OUT_RYC_ZSQ35       80
#define OUT_RYC_ZSQ36       81

//按组遥控翻牌复归
#define OUT_RST_ZSQ_GROUP1   82
#define OUT_RST_ZSQ_GROUP2   83
#define OUT_RST_ZSQ_GROUP3   84
#define OUT_RST_ZSQ_GROUP4   85
#define OUT_RST_ZSQ_GROUP5   86
#define OUT_RST_ZSQ_GROUP6   87
#define OUT_RST_ZSQ_GROUP7   88
#define OUT_RST_ZSQ_GROUP8   89
#define OUT_RST_ZSQ_GROUP9   90
#define OUT_RST_ZSQ_GROUP10  91
#define OUT_RST_ZSQ_GROUP11  92
#define OUT_RST_ZSQ_GROUP12  93

//按组召唤遥测
#define OUT_RYC_ZSQ_GROUP1   94
#define OUT_RYC_ZSQ_GROUP2   95
#define OUT_RYC_ZSQ_GROUP3   96
#define OUT_RYC_ZSQ_GROUP4   97
#define OUT_RYC_ZSQ_GROUP5   98
#define OUT_RYC_ZSQ_GROUP6   99
#define OUT_RYC_ZSQ_GROUP7   100
#define OUT_RYC_ZSQ_GROUP8   101
#define OUT_RYC_ZSQ_GROUP9   102
#define OUT_RYC_ZSQ_GROUP10  103
#define OUT_RYC_ZSQ_GROUP11  104
#define OUT_RYC_ZSQ_GROUP12  105*/


#define OUT_TYPE_NUM         25//106 

#define OUT_RMT1_ORDER         0x80  //终端继电器出口1指令值
#define OUT_RMT2_ORDER         0x81  //终端继电器出口2指令值
#define OUT_CLEAR_SOE_ORDER    0x82    //清除SOE记录指令值
#define OUT_CLEAR_LOAD_ORDER   0X83 //清除负荷记录指令值
#define OUT_GPRS_PW_ORDER      0x84   //打开/关断GPRS电源指令值
#define OUT_BAT_ACT_ORDER      0x85  //打开/关闭电池活化指令值
#define OUT_COMTRAD_ORDER      0x86   //清除录波记录

#define OUT_ODU_RESET_ORDER      0x87   //YK指示器同组触发录波
#define OUT_ODU2_RESET_ORDER     0x88   //清除log日志
#define OUT_GPS_PW_ORDER         0x89   //打开/关断GPS电源指令值
#define OUT_CALL_FI_YC           0x8A   //召唤指示器的遥测功能
#define OUT_CALL_AUTO_TEST_MODE  0x8B   //广播呼叫指示器进入自动化测试模式
#define OUT_CALL_SET_MODE        0x8C   //广播呼叫指示器进入遥测整定模式
#define OUT_LINE1_FI          0x8D   //1//
#define OUT_LINE2_FI          0x8E   //2//2
#define OUT_LINE3_FI          0x8F   //3//


//=====================负荷数据===========================================
#define FLOAD_TOTALNUM  0//在FLASH中保存的负荷记录的条数
#define FLOAD_NEW       1//即将存储的记录与第一条记录的偏移位置，
#define FLOAD_OLD       2//最老一条记录存储位置
#define FLOAD_DAY       3//2//最老一条记录存储位置
#define FLOAD_CS        4//3//以上三个数据加和校验

#define FLOADINFONUM  5//4
//=====================日志记录ulog===========================================
#define FLOG_TOTALNUM  0//在FLASH中保存的日志记录的条数
#define FLOG_NEW       1//即将存储的记录与第一条记录的偏移位置，
#define FLOG_OLD       2//最老一条记录存储位置
#define FLOG_CS        3//以上三个数据加和校验
#define FLOGINFONUM  4

//====================  EEPROM 地址分配  CAT25C128 (0~0x3FFF 字节) 16K字节 ======================
//由于CAT25128的页为64字节，则分配空间的起始地址最好为页起始
//EEPROM中保存配置区的参数      分配1K字节的空间
#define EEPADD_RP               0x0000  //运行参数起始地址 200word

//EEPROM中IEC参数保存
#define EEPADD_IECYXADDR        0x0190 //遥信地址  1000word
#define EEPADD_IECPARA          0x0960 //IEC参数   50byte
#define EEPADD_IECYCADDR        0x0992 //遥测参数  1000byte 
#define EEPADD_IECYKADDR        0x0D7A //遥控参数  100byte

//电话号码存储
#define EEPADD_PHONE            0x0DDE 

#define EEPADDBK_IECYXADDR        0x1000
#define EEPADDBK_IECPARA          0x1190
#define EEPADDBK_IECYCADDR        0x1960
#define EEPADDBK_IECYKADDR        0x1992
#define EEPADDBK_PHONE            0x1D7A

//eeprom中负荷记录信息
#define EEPADD_LOADNUM          0x0E74  //在FLASH中保存的负荷记录的条数 占用2字节
#define EEPADD_LOADADDR_NEW     0x0E76  //即将存储的记录与第一条记录的偏移位置，为(0----FLASH_LOAD_MAXNUM-1) 占用2字节
#define EEPADD_LOADADDR_OLD     0x0E78  //最老一条记录存储位置
#define EEPADD_LOADADDR_DAY     0x0E7A  //记录天数
#define EEPADD_LOAD_CHK         0x0E7C//0x0E7A  //以上两个数据加和校验

//E2中存储SOE尾指针和上行通道发送故障的头指针
#define EEPADD_SOE_NUM          0x0E7E  //内部Flash保存的SOE的总个数
#define EEPADD_SOE_FLASHNUM     0x0E88  //外部Flash保存的SOE的信息，个数
#define EEPADD_SOE_FLASHHEAD    0x0E8A  //外部Flash保存的SOE的信息，头指针
#define EEPADD_SOE_FLASHFLAG    0x0E8C  //外部Flash保存的SOE的信息，是否写满标志位


 
#define EEPADD_SSOETAIL         0x0E80
#define EEPADD_SSOEHEAD1        0x0E82 //第1个上行通道的头指针
#define EEPADD_SSOEHEAD2        0x0E84 //第2个上行通道的头指针
#define EEPADD_SSOEHEAD3        0x0E86 //第3个上行通道的头指针

#define EEPADD_SFEIHEAD1        0x0E88 //第1个上行通道的头指针  zx20171206 故障事件
#define EEPADD_SFEIHEAD2        0x0E8A //第2个上行通道的头指针
#define EEPADD_SFEIHEAD3        0x0E8C //第2个上行通道的头指针

//E2中保存系统重启信息
#define EEPADD_RSTARTNUM        0x0E96 


//E2中保存录波的个数
#define EEPADD_LUBOFLAG          0x0E98 //当前录波FLASH保存到的位置
// 占用0.5K字节空间，约512个字节，0x109A
#define EEPADD_ALEN             0x0E9A //A相的偏移量
#define EEPADD_BLEN             0x0E9C //B相的偏移量
#define EEPADD_CLEN             0x0E9E //C相的偏移量
#define EEPADD_COMLEN           0x0EA0 //最大偏移量
#define EELEN_LENG              8 //每条录波占用的偏移量的长度

#define EEPADD_LUBONUM          0x1100 // 当前录波的总个数

#define EEPADD_VerNum            0x1104 // 当前录波的总个数,占用8个字节，SV01.033
#define EEPADD_LOGNUM           0x1110  //在FLASH中保存的负荷记录的条数 占用2字节
#define EEPADD_LOGADDR_NEW      0x1112  //即将存储的记录与第一条记录的偏移位置，为(0----FLASH_LOAD_MAXNUM-1) 占用2字节
#define EEPADD_LOGADDR_OLD      0x1114  //最老一条记录存储位置
#define EEPADD_LOG_CHK          0x1116  //以上两个数据加和校验

#define EEPADD_DAYNUM           0x1200 //定点负荷记录共存31天，每天最多存96条，实际存的条数

#define EEPADD_VALIDITY         0x1300 //有效期，64条录波目录，每条录波目录的有效期为2个字节，共2*64个字节

#define EEPADD_FIXNUM           0x1400 //通过文件名保存定点的索引，共31*20个字节


#define EEPADD_CFG              0x1700 //保存CFG文件，目前CFG文件最多占用700个字节，没有写入FLASH的原因是:每次发送该文件时，都需要对CFG文件的某几个地方做修改，方便操作

#define EEPADD_DEGBF101            0x1A00 // DEG口 读写报文时，备份当前进行的命令。读写文件时，若被其它的进程包括总召、对时、主动上报等命令打断时，可以继续传输
#define EEPADD_GPRSBF101           0x1B00 // GPRS口 读写报文时，备份当前进行的命令。读写文件时，若被其它的进程包括总召、对时、主动上报等命令打断时，可以继续传输


#define EEPADD_COEFFICIENT      0x2000 //录波系数存放的起始位置，ABC三项系数，每相4个字节，共64条，占用64*4*3
#define EEPADD_PARA               0x3800 //参数保存


//eeprom中录波数据信息
/*#define EEPADD_LUBONUM          0x0E9A  //在FLASH中保存的录波记录的条数 占用2字节
#define EEPADD_LUBOADDR_NEW     0x0E9C  //即将存储的记录与第一条记录的偏移位置，为(0----FLASH_LOAD_MAXNUM-1) 占用2字节
#define EEPADD_LUBOADDR_OLD     0x0E9E  //最老一条录波记录存储位置
#define EEPADD_LUBO_CHK         0x0EA0  //以上两个数据加和校验*/




//============================   GPIO对应DI DO 控制  ===============================

#define   OPEN_DI_POWER()             P6OUT |= BIT6;//p10.3 高电平打开硬遥信电源((P10OUT |= BIT3) && (P9OUT |= BIT3));//p10.3 高电平打开硬遥信电源
#define   CLOSE_DI_POWER()            P6OUT &= NBIT6//((P10OUT &= NBIT3)&&(P9OUT &= NBIT3));

#define   READ_DI_POWER_ST          P8IN & BIT6 //P8.6 硬遥信电源状态监测


#define DO00_ON     (P6OUT &= NBIT5)    //继电器出口DO00--p8.7 低电平
#define DO00_OFF    (P6OUT |= BIT5)     //继电器出口DO00--p8.7 高电平
#define DO01_ON    (P6OUT &= NBIT4)     //继电器出口DO00--p9.0 低电平
#define DO01_OFF   (P6OUT |= BIT4)      //继电器出口DO00--p9.0 高电平

#define OPEN_BAT_ACT     P7OUT |= BIT3//硬件上是预留，暂时写这个IOP5OUT |= BIT5  //打开电池浮电压采集(电池活化也是这个IO) p5.5 高电平
#define CLOSE_BAT_ACT    P7OUT &= NBIT3//P5OUT &= NBIT5


//================================led=================
#define   LIGHT_RUN_UP()              P7OUT &= NBIT7;//P7OUT &= NBIT4;//p7.4 运行灯
#define   LIGHT_RUN_DOWN()            P7OUT |= BIT7;//P7OUT |= BIT4;
#define   LIGHT_RUN_FLASH()           P7OUT ^= BIT7;//P7OUT ^= BIT4;

#define   LIGHT_RUN2_UP()              P4OUT &= NBIT7;//p7.4 运行灯
#define   LIGHT_RUN2_DOWN()            P4OUT |= BIT7;
#define   LIGHT_RUN2_FLASH()           P4OUT ^= BIT7;


#define FEED_WATCH_DOG()   P5OUT ^= BIT4;//P7OUT ^= BIT5;// p7.5 看门狗 

//============================ AD采集 GPIO定义  ===============================

#define OPEN_BAT_SAMPLE   P10OUT |= BIT2 //打开电池电压采集 p10.2 高电平
#define CLOSE_BAT_SAMPLE  P10OUT &= NBIT2

#define OPEN_SUN_SAMPLE   P10OUT |= BIT1//打开太阳能电压采集 p10.1 高电平
#define CLOSE_SUN_SAMPLE  P10OUT &= NBIT1

#define OPEN_C_SAMPLE     P10OUT |= BIT0//打开电容电压采集 p10.0 高电平
#define CLOSE_C_SAMPLE    P10OUT &= NBIT0

#define OPEN_BATF_SAMPLE     P7OUT |= BIT3//P5OUT |= BIT5//打开电池浮电压采集(电池活化也是这个IO) p5.5 高电平
#define CLOSE_BATF_SAMPLE    P7OUT &= NBIT3//P5OUT &= NBIT5

#define OPEN_SUN_CHARGE     P5OUT &= NBIT5// P4OUT &= NBIT6 //电池充电控制 P4.6 低电平充电
#define CLOSE_SUN_CHARGE    P5OUT |= BIT5//P4OUT |= BIT6 //高电平停止充电


//============================  无线模块 GPIO定义  ===============================
#define OPEN_WIRE_POWER               P4OUT &= NBIT6//P4OUT &= NBIT0 //P4.0 无线电源控制 低电平接通
#define CLOSE_WIRE_POWER              P4OUT |= BIT6//P4OUT |= BIT0

#define   WIRE_LESS_RST_L()           P4OUT |= BIT3;//P4OUT |= BIT1;//p4.1 RF复位 高电平复位
#define   WIRE_LESS_RST_H()           P4OUT &= NBIT3;//p4.1 RF复位 低电平正常

#define   READ_WIRE_LESS_WDT          (P4IN & BIT4) >> 2 //RF测试 p4.2

#define OPEN_YK_TRIGGER               P4OUT |= BIT5//P1OUT |= BIT2   //P4.0 无线电源控制 低电平接通
#define CLOSE_YK_TRIGGER              P4OUT &= NBIT5//P1OUT &= NBIT2



//#define   SET_WIRE_LESS_WDT_IN        (P4DIR &= NBIT2)

//============================  锂电池控制  ===============================
#define OPEN_BAT_POWER                P7OUT |= BIT2;g_batPowerSt = 1;//P4OUT |= BIT7;P4.0 无线电源控制 低电平接通
#define CLOSE_BAT_POWER              P7OUT &= NBIT2;g_batPowerSt=0;//P4OUT &= NBIT7;

//============================  GPRS GPIO定义  ===============================

#define OPEN_GPRS_POWER    P10OUT |= BIT3;g_GprsPowerSt = 1; //P4OUT |= BIT3GPRS電源控制 p4.3 高电平接通电源
#define CLOSE_GPRS_POWER   P10OUT &= NBIT3;g_GprsPowerSt = 0;

#define GPRS_SETA_H()  //P1OUT |= BIT5 //P6.5 配置引脚P6OUT |= BIT5
#define GPRS_SETA_L()  //P1OUT &= NBIT5

#define GPRS_RST_H()  //P1OUT |= BIT6//P6.4 复位控制P6OUT |= BIT4
#define GPRS_RST_L()  //P1OUT &= NBIT6

#define GPRS_ONOFF_H()  P6OUT |= BIT0 //P6OUT |= BIT6 P6.6 GPRS开机信号
#define GPRS_ONOFF_L()  P6OUT &= NBIT0
#define GPRS_ONOFF_IN()  P6SEL |= BIT0

//============================  GPS控制  ===============================
#define OPEN_GPS_POWER               P1OUT |= BIT7;P9SEL |= 0x30;P9DIR |= BIT4;g_gpsPowerSt = 1;//P9SEL = 0x30;P4OUT |= BIT7;P4.0 无线电源控制 低电平接通
#define CLOSE_GPS_POWER              P1OUT &= NBIT7;P9SEL &= 0xCF;P9DIR &=NBIT4;g_gpsPowerSt=0;//P4OUT &= NBIT7;P9DIR &=NBIT5;P9OUT &= 0xCF;



//============================  内部flash存储  ===============================

#define SOE_RECORD_ADD   0x42000     //SOE记录地址区

#define SOE_NUM_SEG   42//每段存42条记录
#define SOE_SEG       24 //24段，每段512个字节

#define SOE_RECORD_LEN  32//30//0x10        //每条记录16字节
#define SOE_RECORD_NUM  32          //32条记录

//====================  FLASH 地址分配  SST26VF064  每页256字节，16页= 4K 共2048 * 16 = 32768页  8M======================
#define FLASH_PAGE_SIZE         256

//FLASH中负荷数据 从第1页开始存放，存放1080页
#define FADDR_LOAD_START         0x0             //从0地址保存负荷数据
#define FADDR_LOAD_END          0x5D000 //0x43800总共存放1488页的负荷记录 1488 * 256 = 0x5d000

#define FLASH_PLOAD_LEN         128      //FLASH中保存单条负荷记录占用的字节数
#define FLASH_PPAGE_LNUM        2      //FLASH中每页的负荷记录条数为2条  256 = 2*128
#define FLASH_PAGE_NUM          ((31*96)/2)//每页保存2条负荷记录，总共占1080页 //32
#define FLASH_LOAD_MAXNUM       (31*96)  //保存的负荷记录最多条数共90天*24条=2160//64
#define FLASH_DAYLOAD_LEN       0x3000 //每天保存的遥测记录所占的地址

//FLASH中SOE数据        从第1500(1100)页开始，保存64页 每页存16条SOE 每条占16byte
#define FADDR_SOE_START         0x5E000//0x44C00            

#define FSOE_TOLNUM      512 //1024  //存入外部flash的SOE总数上限 64页能存1024条
#define FSOE_PNUM        16    //每页存16条SOE
#define FSOE_LEN         32//16    //没条SOE占16字节 内存中每条占11字节，这里为了flash中容易处理才预留出5字节




//FLASH中录波数据        从第1200页开始，保存640页 每16页(3840)存一只指示器的一条录波数据，16*4=64页存一整条完整的录波数据，可存10条完整的录波数据

#define FADDR_RECORDER_START         0x66000//0x64000//0x4B000   //电流录波起始地址         

#define FRECORDER_TOLNUM      64//40//20  //存入外部flash的录波总数。20条完整的录波
//#define FRECORDER_PNUM        16    //每16页存一条录波数据
#define WRITEPECORDER  228//写入包数据个数 基地址（3）+偏移地址（3）+数据量（1）+数据内容（224）+累加和（1）
#define FLINEADDR  (64*256)
#define FPHASEADDR (16*256)

#define FADDR_RECORDER_STARTV        0x167000//0x165000//0x14C000   //电压录波起始值地址 
//#define FADDR_RECORDER_STARTV        0x9C000   //电压录波起始值地址 
#define RECORDER  (80*15)//上传给主站的报文长度
#define RECORDER_INDI  (80*14)//修改个数需要修改的地方 指示器传给终端的录波长度


#define FADDR_RECORDER_INFO         0x26B000//0x269000//0x250000   //录波目录信息 A20*100A
//#define FADDR_RECORDER_INFO       0xF0000   //录波目录信息 A20*100A
//#define FADDR_RECORDER_INFOB       0xF1000   //录波目录信息 A20*100
//#define FADDR_RECORDER_INFOC       0xF2000   //录波目录信息 A20*100
//#define FADDR_RECORDER_INFO0       0xF3000   //录波目录信息 A20*100
#define FADDR_WAVE_FILTERI           0x36B000//0x369000//0x350000  //电流录波数据的滤波

#define FADDR_WAVE_FILTERV           0x3AB000//0x3A9000//0x390000//电压录波数据的滤波
//FLASH中日志记录 从第16384页开始存放，存放1024页
#define FADDR_LOG_START         0x402000//0x400000             //从0地址保存日志记录
#define FADDR_LOG_END           0x442000//0x440000 //0x400000总共存放1024页的日志记录

#define FLASH_PLOG_LEN         256      //FLASH中保存单条日志记录占用的字节数
//#define FLASH_PPAGE_LNUM        2      //FLASH中每页的负荷记录条数为2条  256 = 2*128
//#define FLASH_PAGE_NUM          ((31*96)/2)//每页保存2条负荷记录，总共占1080页 //32
#define FLASH_LOG_MAXNUM       1024  //保存的日志记录最多1024

#define RECORDER_FILTER_LENGTH      80   //滤波后计算有效值的长度   

#define COEFFICIENT_LINE_LENG       12
#define COEFFICIENT_PHASE_LENG      4

//#define FADDR_RECORDER_VINFO       0xF4000   //录波目录信息 20*100
//#define FADDR_RECORDER_VINFOB       0xF5000   //录波目录信息 20*100
//#define FADDR_RECORDER_VINFOC       0xF6000   //录波目录信息 20*100
//#define FADDR_RECORDER_VINFO0       0xF7000   //录波目录信息 20*100



//#define RECORDER_MULU_LENGTH      13  //回复每条录波目录的长度

//#define FADDR_RECORDER_START_BK         0x100000   //电流录波起始地址 
//#define FADDR_RECORDER_STARTV_BK        0x151000   //电压录波起始值地址 


//#define FSOE_LEN         16    //没条SOE占16字节 内存中每条占11字节，这里为了flash中容易处理才预留出5字节




//===============================入口标志数据及备份校验存储区================
#define FLASH_UPDATE_COUNTER     0x7AF000 //包总数
#define FLASH_UPDATE_CHECK       0x7AF002 //校验值
#define FLASH_UPDATE_STATE       0x7AF004 //升级状态
#define FLASH_UPDATE_EARASE       0x7AF005//内部flash擦除表示用于表示升级过程中是否已经将内部flash擦除，以防止在擦出以后改变升级状态而重启后去寻找原有程序入口
#define FLASH_UPDATE_CCHECK       0x7AF006 //校验
//备份
#define FLASH_UPDATEBK_COUNTER   0x7AF008 //包总数
#define FLASH_UPDATEBK_CHECK     0x7AF00A //校验值
#define FLASH_UPDATEBK_STATE     0x7AF00C //升级状态
#define FLASH_UPDATEBK_EARASE     0x7AF00C //flash擦除标志
#define FLASH_UPDATEBK_CCHECK     0x7AF00D //校验

//===================================应用程序存储区===========================
#define FLASH_UPDATE_START      0X7B0000       //
#define FLASH_UPDATE_END        0X7EFFFF       //
#define FLASH_UPCOUNTER          0x7F0000//无效升级次数      


#define ERASE_DELAY 30000//擦除与写入间隔延时，太小则会造成写入错误（全1）
#define ERASEFLAGADDR   0x45A00//内部flash地址，jtag烧写程序标志首地址,使用6个字节表示，在程序下载完成之后全部写为0x55
//============================= SMS校验=======================================
const static unsigned short fcsTab[256] = {
      0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
      0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
      0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
      0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
      0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
      0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
      0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
      0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
      0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
      0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
      0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
      0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
      0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
      0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
      0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
      0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
      0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
      0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
      0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
      0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
      0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
      0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
      0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
      0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
      0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
      0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
      0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
      0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
      0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
      0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
      0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
      0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
   };



#define  GETBIT(p,i)   ((p[(i>>4)] & BIT[(i&0x0F)]) && 1)
#define  SETBIT(p,i,v) p[(i>>4)] = (v) ? (p[(i>>4)]|BIT[(i&0x0F)]) : (p[(i>>4)]&~BIT[(i&0x0F)])



//============================  测试定义  ===============================
//#define GPRS_POWERDOWN_TIME 180   //300

//#define ODU_LINE_NUM  12

//#define BAT_POWERDOWN_TIME  180  //180电池打开的时间和GPRS打开的时间需要一致
#endif

