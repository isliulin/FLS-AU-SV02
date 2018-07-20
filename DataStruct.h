#ifndef _DATASTRUCT_H

    #define _DATASTRUCT_H
#ifdef __cplusplus
extern "C"
{
#endif  
  
    #include <string.h>
    #include "stdio.h"
    #include <math.h>
    #include "msp430x54xA.h"
    #include "PUBLIC\mytypes.h"
    #include "Constant.h"
    #include "FuncDef.h"
    #include "RemoteUpdata.h"
    #include "RecorderWave.h" 	
	#include "highcoefs.h"
// #pragma location=0x0FF00 
 const unsigned char VerNum[]="SV02.009";
 struct FIPARAINFO//读写指示器参数信息
  {
	  BYTE Fi_rflag;//读写指示器命令的标志位
	  BYTE Fi_wflag;//读写指示器命令的标志位
	  BYTE FiphaseA_flag;//由于主站是按线路下发，因此，分别设置读写ABC三只指示器的标志位
	  BYTE FiphaseB_flag;
	  BYTE FiphaseC_flag;
	  BYTE Filinephase;//当前执行的指示器的线偏和相偏
	  WORD Fi_Info;//读写参数的起始信息体地址
	  WORD Fi_Info_Num; //从起始地址起所有参数的字节数
	  BYTE Fi_InfoPara[40];
	  //BYTE Fi_cpypara[50];
	  
	  BYTE PaWaitflag_fi ;
      BYTE PaWaitCt_fi ;
      BYTE TxNum_fi ;

	  BYTE WPaWaitflag_fi ;
      BYTE WPaWaitCt_fi ;
      BYTE WTxNum_fi ;
	  BYTE Para_guiyue;
  };
 
//================================== AD采集模块 ============================================
#ifdef _DRIVER_AD
  unsigned int  g_sSampleData[AC_AD_CHAN_NUM][AC_BUFF_LEN];//AD采集数据
  unsigned int g_sSampleFinish;//采集完成
  char FarFalutEnFlag; //电源量允许进行数据上传
  unsigned char g_OpenBatFlag;//开锂电池采集标识 ON:打开AD采集 TRUE:已完成单次采集10个点 OFF:AD采集未使能
  unsigned int g_SunHighNum;//采集到的太阳能电压持续高的次数
#else
  extern unsigned int  g_sSampleData[AC_AD_CHAN_NUM][AC_BUFF_LEN];
  extern unsigned int g_sSampleFinish;
  extern unsigned char g_OpenBatFlag;
  extern unsigned int g_SunHighNum;//采集到的太阳能电压持续高的次数
#endif

//================================== 数据处理模块 ============================================
  struct sRMT_BACK
 {
   unsigned long m_RmtInfoBackNew;//有遥信变化时
   unsigned int m_Year;
   unsigned int m_Month;
   unsigned int m_Day;
   unsigned int m_Hour;
   unsigned int m_Minute;
   unsigned int m_Milisecond; 
 };
struct SOE_FLASH_BACK//新增加，为了读取SOE的记录
 {
   unsigned int m_InFlashTailNum;//保存到外部FLASH中SOE的个数;
   unsigned int m_unSSoeInFlashHead;//外部FLASH的头指针，因为外部FLASH插除时，插除4096字节，轮训完一圈时，需要注意，该指针指向下一大段的头指针。
   unsigned char m_OverWriteFlag;//当前FLASH是否写满的标志，写满之后，头指针应指向下一大段
 };

#ifdef _DATA_PROCESS
    unsigned int g_gRmtMeas[RMT_MEAS_NUM];//遥测量，
    unsigned int g_gRmtInfo[RMT_WORD_NUM];//遥信量 一个遥信信息占2个bit位

    unsigned int g_gRmtLock[RI_FTINFO_NUM];//遥信闭锁时间计数
    BYTE g_gRmtLock10s[RI_FTINFO_NUM];//10S计数器
    unsigned int g_gRmtLockLB;//录波标志位遥信闭锁时间计数
    #ifdef LOCALYX 
    unsigned int g_gDIMcLock[EXT_DIN_NUM];//硬遥信脉冲闭锁时间计数
    BYTE g_gDIMcLock10s[EXT_DIN_NUM];//10S计数器
    unsigned int g_unDCosBuffTail;
	unsigned int g_unDSoeSavePtr;//待保存的双点SOE指针
	#endif
    unsigned char g_gSCosBuff[SOE_BUFF_NUM][SOE_DATA_LEN];
    
    unsigned char g_gGroundBuff[SOE_BUFF_NUM];
	unsigned char g_gshortBuff[SOE_BUFF_NUM];
    unsigned int g_unSCosBuffTail;
	unsigned int g_unSSoeInFlashTail;//保存在外部flash中的SOE尾指针
	SOE_FLASH_BACK g_InFlashInfo;//读取历史soe时，外部FLASH的头指针及个数
    unsigned int g_gSmsHeadPt;       //短信头指针
    unsigned int g_gSmsHeadPtrBk;    //短信头指针备份
    //unsigned int g_gSmsStartPtr;     //短信发送开始指针
    unsigned int g_RecFaultNum;      //短信发送条数备份
    unsigned int g_unGroundBuffTail;
	unsigned int g_unShortBufftail;
    
    unsigned int g_unSSoeSavePtr;//待保存的单点SOE指针
    unsigned int g_unSSoeSaveFlashPtr;//待保存到外部flash的单点SOE指针
    unsigned char   soe_ptr; //最新SOE在flash中的段位置
    unsigned char   soe_area;//最新SOE在flash段中位置
    unsigned int    soe_num;//SOE总个数
    unsigned char   soe_overflag;//FLASH是否写满
    unsigned char 	soe_Headptr;//头指针
    unsigned int  g_yxChangeflag;//遥信变位标识 BITF标识有新报文上传(遥信及遥测) BIT0-5,6-11分别标识调试口和gprs口对应的SYX,DYX,SSOE,DSOE,SSMS,DSMS;
    char g_AdChgSdYc;//标识AD采集是否立即上传ON=立即上传
    char g_FltChgSdYc;//标识故障遥测是否立即上传 1=立即上传
    char g_PtLoseFlag;//pt失压标识 0=未失压 1=失压 2=电压回复中
    char g_BatLoseFlag;//电池失压标识 0=未失压 1=失压 2=电压回复中   
    unsigned char g_PtMonitorCnt;//pt电压恢复计数
    unsigned char g_BatMonitorCnt;//电池电压恢复计数
    char g_SendYc;//0=所有遥测均不发送，1=正常发送(定时上报的遥测，故障遥测，AD采集值变化的遥测，越限遥测)
    unsigned char  g_Soenum;
    unsigned int  g_ZsqCmdRBit = 0;
    unsigned int  g_ZsqCmdWBit = 0;
    unsigned int  g_JdgPwMode = 0;
	unsigned int  g_SendLuboNum;
	unsigned int  g_SendLuboPage;
     WORD wCfgTotalNum =0;//总长度
     WORD section_current=1;//当前节*/
    BYTE g_Current_line = 0;
	unsigned int g_FRecorder_Current_COUNT;//flash保存到第几条了
	unsigned int g_Recorder_Total_COUNT;//总的条数
	//unsigned int g_Current_COUNT[FRECORDER_TOLNUM];//该间隔内的第几次故障，1-64对应1-64间隔
	FIPARAINFO  g_RWFiInf;
	WORD g_wUpPaAddr[PA_ADDR_NUM];//主站端下发的参数地址，放这里是为了节省内存
        BYTE g_EncData[256];
    //DWORD  g_IHDCmdRBit = 0;   //映瀚通读指令标志位 DWORD 预留16个命令
   // DWORD  g_IHDCmdWBit = 0;   //映瀚通写指令标志位 DWORD 预留16个命令
#else

    extern unsigned int g_gRmtMeas[RMT_MEAS_NUM];//遥测量，保存需要上传到后台的遥测数据       
    extern unsigned int g_gRmtInfo[RMT_WORD_NUM];//遥信量 一个遥信信息占2个bit位

	extern unsigned int g_gRmtLock[RI_FTINFO_NUM];//遥信闭锁时间计数
	extern BYTE g_gRmtLock10s[RI_FTINFO_NUM];
	extern unsigned int g_gRmtLockLB;//遥信闭锁时间计数
    #ifdef LOCALYX 
    extern unsigned int g_gDIMcLock[EXT_DIN_NUM];//硬遥信脉冲闭锁时间计数
    extern  BYTE g_gDIMcLock10s[EXT_DIN_NUM];//10S计数器
    extern unsigned int g_unDSoeSavePtr;//待保存的双点SOE指针
	extern unsigned int g_unDCosBuffTail;
	#endif
    extern unsigned char g_gSCosBuff[SOE_BUFF_NUM][SOE_DATA_LEN];
    extern unsigned char g_gGroundBuff[SOE_BUFF_NUM];
	extern unsigned char g_gshortBuff[SOE_BUFF_NUM];
    extern unsigned int g_unSCosBuffTail;
    
	extern unsigned int g_unSSoeInFlashTail;//保存在外部flash中的SOE尾指针
	extern SOE_FLASH_BACK g_InFlashInfo;//读取历史soe时，外部FLASH的头指针及个数
    extern unsigned int g_gSmsHeadPt;       //短信头指针
    extern unsigned int g_gSmsHeadPtrBk;    //短信头指针备份
   // extern unsigned int g_gSmsStartPtr;     //短信发送开始指针
    extern unsigned int g_RecFaultNum;      //短信发送条数备份
    extern unsigned int g_unSSoeSavePtr;//待保存的单点SOE指针
    extern unsigned int g_unSSoeSaveFlashPtr;//待保存到外部flash的单点SOE指针
    extern unsigned int g_unGroundBuffTail;
	extern unsigned int g_unShortBufftail;
    extern unsigned char   soe_ptr; //最新SOE在flash中的段位置
    extern unsigned char   soe_area;//最新SOE在flash段中位置
    extern unsigned int    soe_num;//SOE总个数
    extern unsigned int  g_yxChangeflag;
    extern char g_AdChgSdYc;
    extern char g_FltChgSdYc;
    extern char g_PtLoseFlag;
    extern unsigned int g_PtMonitorCnt;
    extern char g_SendYc;
    extern unsigned char  g_Soenum;
    extern unsigned int  g_ZsqCmdRBit;
    extern unsigned int  g_ZsqCmdWBit;
    extern unsigned int  g_JdgPwMode;
	extern unsigned int  g_SendLuboNum;
	extern unsigned int  g_SendLuboPage;
    extern WORD wCfgTotalNum ;//总长度
    extern WORD section_current;//当前节*/
    extern BYTE g_Current_line;
	extern unsigned int g_FRecorder_Current_COUNT;//flash保存到第几条了
	extern unsigned int g_Recorder_Total_COUNT;//总的条数
	//extern unsigned int g_Current_COUNT[FRECORDER_TOLNUM];
	extern FIPARAINFO  g_RWFiInf;
	extern WORD g_wUpPaAddr[PA_ADDR_NUM];//主站端下发的参数地址，放这里是为了节省内存
        extern BYTE g_EncData[256];
        //extern DWORD  g_IHDCmdRBit;   //映瀚通读指令标志位
    //extern DWORD  g_IHDCmdWBit;   //映瀚通写指令标志位    
#endif
struct sYK_Operation//遥控记录
{
	unsigned int YKinfoaddr;//遥控地址
	//unsigned int YKtime[RTC_NUM];//遥控时间
	unsigned char cmd;//select oper cancel
	unsigned char cmd_state;//遥控命令状态
};
//=======================================  参数配置模块  ===================================
#ifdef _PARA_CFG

        unsigned int g_unYxTrans[RMT_WSYX_NUM];  //根据遥信的点表，重新配置生成的遥信内容
        unsigned int g_unDYxTrans[RMT_WDYX_NUM];//上传的双点遥信
        
        unsigned int g_unYcTrans[IEC_YC_NUM];    //上传的遥测数据，如果发生上传的超过刷新死区，则上传
        unsigned char g_ucYcTransNum; //记录需要上传的遥测的个数
        unsigned int g_ucYxTransNum; //记录u需要上传遥信的总个数(包括单点和双点)
        unsigned int g_ucDYxTransNum;//记录上传的双点遥信个数
        unsigned int g_ucSYxTrsStartId;//上传的单点遥信中第一个点的序号(在g_ucYXAddr数组中)
        unsigned int g_ucDYxTrsStartId;//上传的双点遥信中第一个点的序号(在g_ucYXAddr数组中)
        unsigned char g_ucYkTransNum;
        unsigned char g_ucParaChang ; //101参数发生变更标志  BIT0为 101参数，BIT1为遥测点表，BIT2为遥测系数，BIT3为遥测偏移量，BIT4为遥信点表 BIT5为运行参数表
        unsigned char g_ucRefYxYcTs ;//遥信遥测数据变化标志 BIT0为遥信 BIT1为遥测
        unsigned char g_ZsqPara ;   //zx
        unsigned char g_debug_rate; //101参数发生变更标志  BIT0为 101参数，BIT1为遥测点表，BIT2为遥测系数，BIT3为遥测偏移量，BIT4为遥信点表 BIT5为运行参数表
		unsigned char g_rtc_time;//录波对时是否启动
		unsigned char g_off_gps;//GPS是否关闭
		
		unsigned char  g_gSmsPhone[PHONE_PA_NUM];//短信中电话号码
		unsigned char  *g_pVersion;//版本号
		unsigned char  *g_pCompany;//公司名
		unsigned char  *g_pDevId;//产品型号
        unsigned int g_gRunPara[RUN_PARA_NUM];//运行参数，保存装置的一些配置信息，包括通信参数等
     	unsigned char g_ucPara101[IEC_PARA_NUM];    //IEC运行的运行参数
     	unsigned char g_unParaObjComBk[4];//IEC参数中的端口对象4个数据进行备份，有变化时重映射类对象与端口的对应关系
        unsigned char g_ucYCAddr[IEC_YC_NUM + 1];   //遥测的点表，可以把遥测中的数据按照偏移量重新排列，最多按照20个配置，为0的不上传，点号为1-255
        unsigned int g_ucYXAddr[IEC_YX_NUM + 1];   //遥信的点表，为0的不上传，点号为1-255
        unsigned char g_ucYKPa[IEC_YK_NUM + 1];   //遥控参数 标识指示器的线偏相偏
	unsigned char g_gZCZSQ;  //正在召测指示器的遥测值
			
        unsigned int g_gRunParaFltLkTBk;//运行参数中故障闭锁时间备份，用于参数变化时使其立即生效
        unsigned int g_gRunParaZsqStTBk;//运行参数中指示器通信状态检测周期备份
        unsigned int g_gRunParaYxRsdTTBk;//运行参数中遥信重发时间间隔备份
		unsigned int g_gShortFltLkTBk;//运行参数中短路故障复位时间备份，用于参数变化时使其立即生效

		
        unsigned char g_open_oducall;//程序上电时5S召唤一次，等待采集的C的电压，以判断是用电池还是超级电容打开邋邋GPRS
        unsigned int Before4HourSdYcTm;
		#ifdef ODU
        unsigned char g_LineFlag[12];    //ODU所有路数的状态?0为不和其它路合成1 为合成的前一路2 为合成的后一路
		#endif
        unsigned char g_XPPara;
		
        unsigned int g_LogType = 0; //日志文件不同类型
        unsigned char g_Linkflag =0;
		unsigned char g_LogValue =0;
		unsigned char g_VerNum[9] ;//系统版本号
		sYK_Operation g_YKinfo;
        extern const unsigned int BIT[16] =   //获得第几位为1的值，BIT0~BIT15
    	{
                0x01,0x02,0x04,0x08,
        0x10,0x20,0x40,0x80,
        0x100,0x200,0x400,0x800,
        0x1000,0x2000,0x4000,0x8000
    	};
#else
        extern unsigned int g_unYxTrans[RMT_WSYX_NUM];  //根据遥信的点表，重新配置生成的遥信内容
        extern unsigned int g_unDYxTrans[RMT_WDYX_NUM];//上传的双点遥信

        extern unsigned int g_unYcTrans[IEC_YC_NUM];    //上传的遥测数据，如果发生上传的超过刷新死区，则上传
        extern unsigned char g_ucYcTransNum; //记录需要上传的遥测的个数
        extern unsigned int g_ucYxTransNum; //记录u需要上传遥信的个数
        extern unsigned int g_ucSYxTrsStartId;//上传的单点遥信中第一个点的序号(在g_ucYXAddr数组中)
        extern unsigned int g_ucDYxTrsStartId;//上传的双点遥信中第一个点的序号(在g_ucYXAddr数组中)
        extern unsigned int g_ucDYxTransNum;
        extern unsigned char g_ucYkTransNum;
        extern unsigned char g_ucParaChang; //101参数发生变更标志  BIT0为 101参数，BIT1为遥测点表，BIT2为遥测系数，BIT3为遥测偏移量，BIT4为遥信点表 BIT5为运行参数表
        extern unsigned char g_ucRefYxYcTs;//遥信遥测数据变化标志 BIT0为遥信 BIT1为遥测
        extern unsigned char g_debug_rate;   
		
		extern unsigned char  g_gSmsPhone[PHONE_PA_NUM];//短信中电话号码
		extern unsigned char  *g_pVersion;//版本号
		extern unsigned char  *g_pCompany;//公司名
		extern unsigned char  *g_pDevId;//产品型号
		extern unsigned int g_gRunPara[RUN_PARA_NUM];//运行参数，保存装置的一些配置信息，包括通信参数等
    	extern unsigned char g_ucPara101[IEC_PARA_NUM];    //IEC运行的运行参数
        extern unsigned char g_unParaObjComBk[4];//IEC参数中的端口对象4个数据进行备份，有变化时重映射类对象与端口的对应关系
        extern unsigned char g_ucYCAddr[IEC_YC_NUM + 1];   //遥测的点表，可以把遥测中的数据按照偏移量重新排列，最多按照20个配置，为0的不上传，点号为1-255
        extern unsigned int g_ucYXAddr[IEC_YX_NUM + 1];   //遥信的点表，为0的不上传，点号为1-255 
        extern unsigned char g_ucYKPa[IEC_YK_NUM + 1]; 
        extern unsigned char g_gZCZSQ;
        
       	extern unsigned int g_gRunParaFltLkTBk;//运行参数中故障闭锁时间备份，用于参数变化时使其立即生效
		extern unsigned int g_gRunParaZsqStTBk;//运行参数中指示器通信状态检测周期备份
		extern unsigned int g_gRunParaYxRsdTTBk;//运行参数中遥信重发时间间隔备份
        extern const unsigned int BIT[16];
		extern unsigned char g_open_oducall;
		extern unsigned int Before4HourSdYcTm;
		#ifdef ODU
		extern unsigned char g_LineFlag[12];
		#endif
		extern unsigned char g_XPPara;
		extern unsigned int g_LogType; //日志文件不同类型
		extern unsigned char g_Linkflag;
		extern unsigned char g_LogValue ;
		extern unsigned char g_VerNum[9];//系统版本号
		extern sYK_Operation g_YKinfo;
		extern unsigned char g_rtc_time;//录波对时是否启动
		extern unsigned char g_off_gps;//GPS是否关闭
		extern unsigned int g_gShortFltLkTBk;//运行参数中短路故障复位时间备份，用于参数变化时使其立即生效
#endif

//=================================== 通信模块 ==========================================
  struct  sTX_BUFF		   //通信发送缓冲结构体
	{
	  unsigned char m_gBuffer[MAX_TXBUFF_LEN];//发送数据缓冲
	  unsigned int m_ucTx_Head;//GPRS串口中断发送数据的缓存头指针
      unsigned int m_ucTx_Tail;//GPRS串口中断发送数据的缓存尾指针
      unsigned char m_unSendFlag;
	};

struct	sRx_Operation
{
	//unsigned char m_OperaBuf[MAX_RXBUFF_LEN];//接收数据缓冲
    unsigned int m_Operaflag;      //数据缓冲尾指针
    unsigned int m_Operalength;      //数据缓冲头指针
    
    unsigned int  m_OperaBufT; //接收字节时间间隔
    //unsigned char m_iRcvFlag;
};

  struct  sRX_BUFF         //通信接收缓冲结构体
  {
    unsigned char m_gBuf[MAX_RXBUFF_LEN];//接收数据缓冲
    unsigned int m_iBufT;      //数据缓冲尾指针
    unsigned int m_iBufH;      //数据缓冲头指针
    unsigned int m_iRcvCount; //接收字节时间间隔
    unsigned char m_iRcvFlag;
  };
#ifdef _COMM
    __no_init struct  sTX_BUFF g_sTxBuff[COMM_PORT_NUM];
    __no_init struct  sRX_BUFF g_sRxBuff[COMM_PORT_NUM];
		struct sRx_Operation sRx_Oper;
        unsigned int g_sCommStatCount[RMT_ZSQST_NUM];//判断指示器通信状态时间计数
        char g_YkOrderFlag;//标识遥控是否成功
         
        unsigned char g_GprsPowerSt;//GPRS电源状态 0=关闭 1= 打?      
        unsigned char g_batPowerSt;//bat状态 0=关闭 1= 打?	
        unsigned char g_gpsPowerSt;
		unsigned char g_fault_phase;//  
		unsigned int  g_differI_weizhi;
		unsigned int  g_differV_weizhi;
				 int  g_faultI_Valse;
				 int  g_faultV_Valse;
        unsigned char g_SmsWritePtr;
        unsigned char g_EnSmsSend;  //短信发送允许
        unsigned char g_SmsDataLen;
        unsigned char g_EnSendToXinhao;
        unsigned char g_MesReadFlag;   //短信查询标志位
        unsigned int g_SmsCmdCode;    //短信命令校验码
        unsigned char g_phoneNum[16]; //收到短信的手机号存储，用于回短信
        unsigned int g_WFile;
        unsigned char g_Sec;
        char g_WPaOrderFlag;//标识写指示器参数是否成功
        unsigned char g_SmsReSendWaitCount; //短信重发等待时间
        unsigned char g_SmsReSendTime;   //短信重发次数
    	unsigned char g_SmsSendBuf[255];
		unsigned char g_GprsPowerConsum;
		
		unsigned char g_CmIdRF;//RF对象使用的串口号
		unsigned char g_CmIdGps;//GPS对象使用的串口号
		unsigned char  g_CmIdGPRS;
		unsigned char g_CmIdEX;//扩展口使用的串口号
		unsigned char g_CmIdDBG;//调试口使用的串口号
		unsigned char g_CmIdODU1;//ODU1使用的串口号
		unsigned char g_CmIdODU2;//ODU2使用的串口号
		unsigned int g_RstartNum;//系统重启次数
		//char  ComtrderCfg1[700];
		unsigned char g_TimeRF003_flag;//无线对时标记
		BYTE g_step_earth;
		BYTE gloss_phase_flag[3];
		BYTE g_phase_loss[3];
		BYTE g_detephase_flag;
		BYTE gRecorder_flagall;//录播过程标志位
                
		unsigned int g_GPS_READ_NUM;//RTC与GPS之间的切换，同时达到参数设置的次数后上报gps状态异常
		unsigned int g_RF_pps;//RF的min脉冲计时
                BYTE g_RF_Reset;//RF复位延时
		//WORD g_validity[64];
		 unsigned char g_EncComNo;        //哪个串口在跑加密数据1==GPRS 2 ==DeBug， 其余无效
		//WORD g_validity[64];
        unsigned char g_bEncCerTxt[1024];  //加密CER证书1024
        unsigned char g_bHostRandom[8];  //主站随机数
        unsigned char m_bUnitRandom[8];   //终端自身随机数，用于与运维工具通信
        int g_wEncCerWPtr;                 //Cer证书记录位置

        unsigned char g_bCommFlag[COMM_PORT_NUM];        //EB解帧标志位
        unsigned char g_bCheckEBFrameBuf[COMM_PORT_NUM][4]; //EB帧头存储数组
        unsigned int g_wCheckEBByte[COMM_PORT_NUM];                      //EB帧接收字节计数
        unsigned int g_wEBFrameLen[COMM_PORT_NUM];                         //EB帧长
        unsigned char g_bRecFrameEBFlag[COMM_PORT_NUM];      //启动解析EB帧标志位
        //unsigned char g_wTestDebug;
        //unsigned long g_lRunTimes;
		//BYTE g_YC_value[4];
        char g_bUnitID[25];//终端ID,24字节第一个字是节长度   
        unsigned char g_bAppType;//加密报文应用类型
		

#else
    extern struct  sTX_BUFF g_sTxBuff[COMM_PORT_NUM];
    extern struct  sRX_BUFF g_sRxBuff[COMM_PORT_NUM];
	extern struct sRx_Operation sRx_Oper;
    extern unsigned int g_sCommStatCount[RMT_ZSQST_NUM];
    extern char g_YkOrderFlag;//标识遥控是否成功
    extern char g_WPaOrderFlag;//标识写指示器参数是否成功
    extern unsigned char g_GprsPowerSt;//GPRS电源状态 0=关闭 1= 打开
     extern unsigned char g_batPowerSt;//bat状态 0=关闭 1= 打?    extern int g_ODUYK;//
	 extern unsigned char g_fault_phase;
	 extern unsigned int  g_differI_weizhi;
	extern unsigned int  g_differV_weizhi;
	extern int        	g_faultI_Valse;
	extern int        	g_faultV_Valse;
	extern unsigned char g_SmsWritePtr;
	extern unsigned char g_EnSmsSend;
	extern unsigned char g_SmsDataLen;
    extern unsigned char g_EnSendToXinhao;
    extern unsigned char g_MesReadFlag;
    extern unsigned int g_SmsCmdCode;
    extern unsigned char g_phoneNum[16]; 
    extern unsigned int g_WFile;
    extern unsigned char g_Sec;
    extern unsigned char g_SmsReSendWaitCount;
    extern unsigned char g_SmsReSendTime;
    extern unsigned char g_SmsSendBuf[255];
	extern unsigned char g_GprsPowerConsum;
	extern unsigned char g_CmIdRF;//RF对象使用的串口号
	extern unsigned char g_CmIdGps;//GPS对象使用的串口号
	extern unsigned char  g_CmIdGPRS;
	extern unsigned char g_CmIdEX;//扩展口使用的串口号
	extern unsigned char g_CmIdDBG;//调试口使用的串口号
	extern unsigned char g_CmIdODU1;//ODU1使用的串口号
	extern unsigned char g_CmIdODU2;//ODU2使用的串口号
	extern  unsigned char g_gpsPowerSt;
    extern int g_pODU2WaitCount;//ODU电源关闭请求ODU2的接收计时
	extern int g_pODUWaitCount;//ODU电源关闭请求
	extern unsigned int g_RstartNum;//系统重启次数
	//extern char  ComtrderCfg1[700];
	extern unsigned char g_TimeRF003_flag;//无线对时标记
    extern BYTE g_step_earth;
	extern BYTE gRecorder_flagall;//录播过程标志位
	extern BYTE gloss_phase_flag[3];
	extern BYTE g_phase_loss[3];
	extern BYTE g_detephase_flag;
	extern unsigned int g_GPS_READ_NUM;//RTC与GPS之间的切换，同时达到参数设置的次数后上报gps状态异常
	//extern WORD g_validity[64];
	extern unsigned int g_RF_pps;//RF的min脉冲计时
        extern BYTE g_RF_Reset;//RF复位延时
	 extern unsigned char g_EncComNo;
	extern unsigned char g_bEncCerTxt[1024];//1024
        extern unsigned char g_bHostRandom[8];  //主站随机数
        extern unsigned char m_bUnitRandom[8];
        extern int g_wEncCerWPtr;
        extern unsigned char g_bCommFlag[COMM_PORT_NUM];
        extern unsigned char g_bCheckEBFrameBuf[COMM_PORT_NUM][4];
        extern unsigned int g_wCheckEBByte[COMM_PORT_NUM];
        extern unsigned int g_wEBFrameLen[COMM_PORT_NUM];
        extern unsigned char g_bRecFrameEBFlag[COMM_PORT_NUM]; 
        //extern unsigned char g_wTestDebug;
        //extern unsigned long g_lRunTimes;
	//extern unsigned char g_YC_value[4];
        extern char g_bUnitID[25]; 
	extern unsigned char g_bAppType;  
#endif
    
//=======================DriverTimer.c--定时器驱动模块===============================================
 struct sTIMER_MANAGER
  {
    unsigned int m_TmCount;
    unsigned char m_TmFlag;
	unsigned int m_TmCountBk;//保存上一次设置的定时间隔
  };
#ifdef _DRIVER_TIMER
        struct sTIMER_MANAGER g_sTimer[TIMER_NUM];
        unsigned int Time_10ms_T;
		//unsigned int Time_1s_T;
        unsigned int g_DateCount;  //状态汇报天数计数器
        unsigned char g_StatusTime;
        unsigned char g_SendStatus;
        unsigned char g_SmsSendStatus; //短信汇报状态标志位
        unsigned char g_EnAdTmCount;//使能AD采集延时
    	unsigned char g_EnAdTmFlag;//AD采集延时结束(启动AD中断)
        unsigned char g_SendFault;//无限制模式下主动发送延迟
        unsigned char g_OpenAdDelay;//开AD后延时多久启动AD采集，原来用的定时器，现在分开，ODU延时320ms，其他类型延时2s
        unsigned int g_WatchWireTime;//监视无线模块计时
        unsigned char g_WireStatus;//无线模块输出喂狗信号
        unsigned char g_WireStatusOld;//无线模块输出喂狗信号缓存
        unsigned char g_ResetWireFlag;//无线模块重启标志位
#else
        extern struct sTIMER_MANAGER g_sTimer[TIMER_NUM];
        extern unsigned int g_DateCount;
        extern unsigned char g_StatusTime;
        extern unsigned char g_SendStatus;
        extern unsigned char g_SmsSendStatus;
		extern unsigned char g_EnAdTmCount;
    	extern unsigned char g_EnAdTmFlag;
        extern unsigned char g_OpenAdDelay;
        extern unsigned char g_SendFault;
        extern unsigned int g_WatchWireTime;
        extern unsigned char g_WireStatus;
        extern unsigned char g_WireStatusOld;
        extern unsigned char g_ResetWireFlag;
#endif
//=================================== 时钟模块 ==========================================

  struct sRTC_MANAGER
  {
	unsigned char m_FixYCFlag;   //定点遥测点的记录
	unsigned char m_ucRtcSynFlag;   //系统实时时钟从实时芯片中读取的标志，每5分钟读取一次
    unsigned int m_gRealTimer[RTC_NUM]; //系统内部实时时钟，年、月、日、时、分、秒、毫秒
	unsigned int m_gRealTimerBK[RTC_NUM];
	unsigned int m_gRealTimerFlag; //ON接收到对时报文，TRUE MS=1000时写RTC芯片的时间，
};
struct CRYSTAL_ERROR
	{
		DWORD  Crystal_start_num;//GPS每打开两次之间捕获的秒脉冲之间累积的晶振个数
		DWORD  Crystal_end_num;
		unsigned int start_RealTime[3] ;//GPS第一次打开记录其实时时间分钟 秒 毫秒
		unsigned int end_RealTime[3] ;//GPS第二次打开记录其实时时间分钟 秒 毫秒
		unsigned char Crystal_start_flag;//GPS计时结束后第一次捕获秒脉冲的标志位，GPS的秒和分钟是从报文中得到的，毫秒是在秒脉冲中得到的
		unsigned char Crystal_end_flag;
		unsigned char Compen_cry_flag;//gps比自己记得晶振跑的快，则补偿时晶振加1，否则晶振减1
		DWORD Compen_cry_total;//每Compen_cry_num ms补偿一个晶振
		DWORD Compen_cry_num;//ms中断计数
		unsigned char diff_value_num;//连续没有强拉秒的次数
	};
	struct OUTPUT_INFO
	{
		unsigned char Interrupt_flag;
		BYTE g_buf[6];
		unsigned char Interrupt_flag1;
		BYTE g_buf1[24];
		//BYTE g_buf1[9];
		//BYTE g_buf2[5];
	};
	struct RF003_TIME
	{
		unsigned char RF003_num;
		unsigned int RF003_absolute_time[3][RTC_NUM];//收到无线脉冲后置绝对时标
		
		unsigned int RF003_mini_num[3];//RF003的分钟计数
		unsigned char RF003_time_flag;//RF001分两步接收对时，先是脉冲后是报文，收到脉冲置OFF,收到报文置ON
		unsigned int LUBO_absolute_time[RTC_NUM];//录波CFG文件的决对时标，该时标以收到ABC三只指示器最后一只的有效时间打时标
		unsigned char LUBO_time_flag;//录波时标的有效性
	};
  struct RECORDER_LUBO
  {
	
	//unsigned char lubo_flag;
	//WORD  relative_time;
	unsigned char lubo_flag;//录波标志位
	//WORD     relative_timel;
	//WORD     relative_timeh;
	DWORD relative_time1;
	DWORD relative_time2;
	DWORD relative_time3;
	unsigned int total_num1;   //总的帧数
	unsigned int  recorder_long;//总长度
	unsigned char VI_flag; //电压电流标志位
	unsigned int absolute_time[RTC_NUM];//绝对时标
	unsigned char line_recorder;//线偏
	unsigned char phase_recorder;//相偏
	//unsigned int  total_long;
	//unsigned char lubo_type;  
    unsigned int current_num; //当前帧数
    unsigned char lubo_start_type;//录波启动标志
    
  };
  
  
  /*struct DFT_DATA_EXT
  {
    long  Real;
    long  Imag;
    long  LowReal;
    long  LowImag;
    unsigned char Cnt;
   };//为什么不能放在highcoefs.h
   
  #define   lubo_absolute_time  11  //绝对时标的相对位置1+2+2+2+2+1

  struct RECORDER_LUBO_flag
  {
    WORD     relative_time[4];
	unsigned int absolute_time[RTC_NUM];
	unsigned char recorder_flag[4];
	unsigned char recorder_addr[4];
  };*/
  
 //录波记录
#ifdef _DRIVER_RTC
  struct OUTPUT_INFO g_OutputInfo;
  struct RF003_TIME g_RF003_timeInfo;//RF003与指示器时标的相关信息
  struct sRTC_MANAGER g_sRtcManager;  //系统实时时钟
  struct RECORDER_LUBO g_recorder; 
  unsigned int g_buchangnum2;//毫秒的二级补偿，每14的周期计数变一下
  unsigned int g_buchangnum;//毫秒的一级补偿，每4个ms计数算一个周期
  unsigned int g_ccr0;
  struct  CRYSTAL_ERROR g_CrystalError;//晶振本身误差补偿
  //unsigned int g_mem[500];
  //struct RECORDER_LUBO *precorder;
#else
  extern struct OUTPUT_INFO g_OutputInfo;
  extern struct RF003_TIME g_RF003_timeInfo;//RF003与指示器时标的相关信息
  extern struct sRTC_MANAGER g_sRtcManager;
  extern  struct RECORDER_LUBO g_recorder; 
  extern  unsigned int g_buchangnum2;//毫秒的二级补偿，每14的周期计数变一下
  extern  unsigned int g_buchangnum;//毫秒的一级补偿，每4个ms计数算一个周期
  extern  unsigned int g_ccr0;
  extern  struct  CRYSTAL_ERROR g_CrystalError;//晶振本身误差补偿
  //extern  unsigned int g_mem[500];
  //struct RECORDER_LUBO *precorder;
#endif

//================================== IO驱动模块 ===========================================
#ifdef _DRIVER_IO
	#ifdef LOCALYX
	    unsigned long g_gExtDinStat;    //外部开关量输入确认状态
	unsigned long mtemp;
	    unsigned int g_gExtDinYesTimer[EXT_DIN_NUM];    //外部DIN输入确认时间计时
	    unsigned int g_gExtDinNoTimer[EXT_DIN_NUM];     //外部DIN输入清除时间计时
	    unsigned int g_gDinRtcLock[EXT_DIN_NUM][RTC_NUM];   //DIN发生的实时时钟时刻锁存，在DIN确认后产生SOE时作为时标保存
        //unsigned int g_unRF_RST;       //无线通信模块WDT状态
        //unsigned char g_ucActFlag[OUT_TYPE_NUM];//出口逻辑标志，需要出口动作的在该数组中置上标志ON，需要收回出口命令的置标志为 OFF，无任何命令时为0
	unsigned char YX_delay_clear_flag;
    #endif
#else
		#ifdef LOCALYX
    	extern unsigned int g_gDinRtcLock[EXT_DIN_NUM][RTC_NUM];
        //extern unsigned int g_unRF_RST;       //无线通信模块WDT状态
        //extern unsigned char g_ucActFlag[OUT_TYPE_NUM];//出口逻辑标志，需要出口动作的在该数组中置上标志ON，需要收回出口命令的置标志为 OFF，无任何命令时为0
		extern unsigned char YX_delay_clear_flag;
		#endif

#endif

#ifdef __cplusplus
}
#endif

#endif  //end of file
