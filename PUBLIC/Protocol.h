/*------------------------------------------------------------------------
 Module:            protocol.h
 Author:            linxueqin
 Project:           规约平台
 State:             V1.0
 Creation Date:     2012-10-8
 Description:       规约基类的声明
------------------------------------------------------------------------*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "..\DataStruct.h"



#ifdef __cplusplus
}
#endif /* __cplusplus */


//#define   RF_TEST

//#define   INCLUDE_GB104_S
#define INCLUDE_BJ101_S

#define SOE_BYLEN   6
#define SOE_VALUE_L0             1
#define SOE_VALUE_H0             4000

/*YK*/
#define  MI_YKSELECT  0x01
#define  MI_YKOPRATE  0x02
#define  MI_YKCANCEL  0x03

/*YT*/
#define  MI_YTSELECT  0x06
#define  MI_YTOPRATE  0x07
#define  MI_YTCANCEL  0x08



#define MAX_PUBBUF_LEN      256
#define MAX_RECV_LEN        384
#define MAX_SEND_LEN        256//MAX_PUBBUF_LEN * sizeof(DWORD)
    

//通讯检测位定义
#define FRAME_SHIELD   0xFFFF0000      //屏蔽字
#define FRAME_OK       0x00010000      //检测到一个完整的帧
#define FRAME_ERR      0x00020000      //检测到一个校验错误的帧
#define FRAME_LESS     0x00030000      //检测到一个不完整的帧（还未收齐）


struct VCommBuf 
{
    WORD   wBufSize;        //缓冲区大小
    WORD   wReadPtr;        //读指针
    WORD   wOldReadPtr;     //读指针
    WORD   wWritePtr;       //写指针
    DWORD  dwFlag;          //控制码(主要用于发送)
    BYTE   *pBuf;           //缓冲区
};



typedef struct YtOrder//[数据状态(0xA5A5)+标识(16bit:0xC3)]+地址(16bit)+命令(16bit)+校验(16bit)
{
    WORD wYtNum;
    WORD wYtNo;
    WORD wOrder;
    WORD wCheck;//校验
}YtOrder;

typedef struct YkOrder//[数据状态(0xA5A5)+标识(16bit:0xC3)]+地址(16bit)+命令(16bit)+校验(16bit)
{
    WORD wYkNo;
    WORD wOrder;
    WORD wCheck;//校验
}YkOrder;

typedef struct OrderHead//数据状态(0xA5A5)+标识(16bit:0xC3)+[地址(16bit)+命令(16bit)+校验(16bit)]
{
    WORD wStatus;
    WORD wSign;
}OrderHead;

typedef struct OrderFeedBack//标识(16bit:0xC3)+数据状态(0xA5A5)
{
    WORD wSign;
    WORD wStatus;
}OrderFeedBack;


/******************comm***************************/
typedef struct VSysClock{
  WORD wYear  ;    /*19XX-20XX*/
  BYTE byMonth  ;  /*1-12*/
  BYTE byDay  ;    /*1-31*/
  BYTE byWeek  ;   /*0-6  from sunday*/
  BYTE byHour  ;   /*0-23*/
  BYTE byMinute  ; /*0-59*/
  BYTE bySecond  ; /*0-59*/
  WORD wMSecond  ; /*0-999*/
  WORD pad1; /*armalign add*/
}VSysClock;


typedef struct  VDBSOE{
  BYTE byNol  ;
  BYTE byNoh  ;
  BYTE byValue  ;
  BYTE byMSecondl;
  BYTE byMSecondh;
  BYTE byMinute;
  BYTE byHour;
  BYTE byDay;
  BYTE byMonth;
  BYTE byYear;
  //struct VCalClock Time  ;//加这个变量后，此结构体指针不能正确指向一块内存
}VDBSOE;


typedef struct VDBCOS{
 WORD wNo ;
  BYTE byValue ;
}VDBCOS;

typedef struct VDBYCF{
  WORD wNo ;
  WORD nValue ;
}VDBYCF;

struct VDBYK{
  WORD wID ;       /*遥控号*/
  BYTE byValue ;   /*遥控属性
                  D0~D1
                   00 非法
                   01 合闸继电器
                   10 分闸继电器
                   11 NULL（无需区分合/分闸继电器）
                  D2：1控制对象为开关号（进行开关号查找）（缺省） 0：控制对象为顺序点号（偏移）
                  D3：保留
                  D4-D7:  0－设备自定义脉冲（缺省）
                          1－短脉冲
                          2 长脉冲
                          3 持续输出*/
  BYTE byStatus ;  /*表示请求命令产生的结果*/
};

struct VDBYT{
  WORD wID ;      /*遥控号*/
  WORD wValue ;   /*遥调值*/
  BYTE byStatus ;
  BYTE pad1[3]; /*armalign add*/
};



struct VMsgHead{
  WORD wLength;
  BYTE byMsgID;    /*D0~D6  MsgID
          D7 请求报文中＝1表示对方回应报文时无需发送紧急事项
           ＝0表示对方不回应报文或回应报文时需发送紧急事项*/
  BYTE byMsgAttr;
  BYTE abyRsv[4];
  BYTE byTypeID;
  WORD wInfoAddr;
  BYTE byDco;
};





struct VYKInfo
{
    VMsgHead Head;
    VDBYK Info;
};

struct VYTInfo
{
    VMsgHead Head;
    VDBYT Info;
};

#define SOE_GRP_NUM    22//每包报文最多包含22条soe 每条SOE占11字节



//=================RF规约和ODU规约==========================================================

#define     R_YCPARA          5
#define     R_BATPARA         6
#define     R_OTHERPARA       7
#define     R_CURRENT         8
#define     R_LUBOPARA        9


#define     CONNECT           21
#define     DISCONNECT        22

#define     REVERT            13
#define     VERT              14
#define     W_YCPARA          15
#define     W_BATPARA         16
#define     W_OTHERPARA       17
#define     EARTHFAULT        18
#define     W_LUBOPARA        19



#define MAX_FRAME_LEN      255
#define MAX_ENC_FRAME_LEN  384


#define   RF001FTDANUM        17  //每一个指示器的数据蔤占17字节 架空2遥
#define   RF001FTDANUM_A0     9  //一遥每一个指示器的数据蔤占9字节 架空1遥
#define   RF001FTDANUM_3P4O   30  //每一个指示器的数据蔤占30字节 对应电缆3P4O面板指示器


#define   RF003FTDANUM  29  //每一个指示器的数据蔤占29字节


typedef struct sRF001FTDA{
    BYTE byAddr[4];
    BYTE byLinePhase;
    BYTE byGroup[2];
    BYTE byRes;
    BYTE byUH;
    BYTE byIL;
    BYTE byIH;
    BYTE byIMaxL;
    BYTE byIMaxH;
    BYTE byIMinL;
    BYTE byIMinH;
    BYTE byFaultInfo;
    BYTE byUL;  
}RF001FTDA;

typedef struct sRF001FTDA_A0{
    BYTE byAddr[4];
    BYTE byLinePhase;
    BYTE byGroup[2];
    BYTE byRes;
    BYTE byFaultInfo;
}RF001FTDA_A0;

typedef struct sPHASEFTDA_ABC{
    BYTE byExtTmpt;//外部温度
    BYTE byIntTmpt;//内部温度
    BYTE byIL;//电流低字节
    BYTE byIH;//电流高字节
    BYTE byPhFaultInfo;//相故障标志
}PHASEFTDA;

typedef struct sRF001FTDA_3P4O{
    BYTE byAddr[4];
    BYTE byLinePhase;
    BYTE byGroup[2];
    BYTE byDaSt;//传数据状态
    BYTE byFaultOld;//历史故障相
    BYTE byFaultCur;//当前故障相
    BYTE byFaultComm;//通道故障相
    BYTE byPhData[18];//A,B,C 3相故障信息数据 3*5 + O相3字节
    //PHASEFTDA *pPhFtConA;//BYTE byPhData[15];
    //PHASEFTDA *pPhFtConB;
    //PHASEFTDA *pPhFtConC;
    //BYTE byOIL;//O相电流低字节
    //BYTE byOIH;//O相电流高字节
    //BYTE byOFaultInfo;//O相故障标志
    BYTE byFaultInfoRst;//故障信息复位标志
}RF001FTDA_3P4O;

typedef struct sODUFTDA{
    BYTE byLinePhase;//相偏相偏
    BYTE byRes;
    BYTE byFaultVal;//故障标识位
}ODUFTDA;

typedef struct sODUYCDA{
    BYTE byZsqType;//指示器类型
    BYTE byExtTmpt;//外部温度
    BYTE byIntTmpt;//内部温度
    BYTE byIL;//电流低字节
    BYTE byIH;//电流高字节
    BYTE byZsqSt;//指示器状态
}ODUYCDA;


typedef struct sRF003FTDA{
  
    BYTE len1;  //长度     // 11 D2 11 00
    BYTE subType;//类型
    BYTE len2;//子类 帧号
    BYTE lubo_type;//录波启动类型
    BYTE byUH;
    BYTE byIL;
    BYTE byIH;
    BYTE byIMaxL;
    BYTE byIMaxH;
    BYTE byIMinL;
    BYTE byIMinH;//有录波数据标志位
    BYTE byFaultInfo;//故障标志位
    BYTE byUL;
	BYTE minute_countL;//分钟计数  说明:为了兼容之前版本的程序，该帧报文有录波数据时，表示的是分钟数
	BYTE minute_countH;//分钟计数  说明:录波数据标志位为0时，表示的是温度值及取电值
	BYTE Relative_time[4];// 相对时标字节
	BYTE by_TemperaTure;//温度  说明，以下两个字节只有录波数据标志位为1时才解析
	BYTE by_TakePower;//取电电压
}RF003FTDA;


typedef struct sRFASDU1{
    BYTE bySubType;
    BYTE byInfoNum;
    BYTE byContent[MAX_FRAME_LEN-6];
}RFASDUSUBTP;
typedef struct sRFASDU2{
    BYTE byInfoNum;
    BYTE byContent[MAX_FRAME_LEN-5];
}RFASDU;
typedef struct sRFASDU3{  //指示器上传参数
    BYTE NetID;
    BYTE NetIP;
    BYTE LinePhase;
    BYTE Len;
    BYTE Type;
    BYTE SubSet;
    BYTE SubType;
    BYTE ByContenLen[2];    
    BYTE ByConten[20];
}RF001ASDU;
typedef struct sRFASDU5{  //中继器电池电压
    BYTE NetID;
    BYTE NetIP;
    BYTE LinePhase;
    BYTE Len;
    BYTE Type;
    BYTE SubLen;
    BYTE SubReserve;
    BYTE CapValt[2];   //电容电压 
    BYTE BatValt[2];   //电池电压
}REPEATERASDU;

typedef struct sRFASDU6{  //指示器录波数据报文格式
	BYTE Total_Num ;//帧总数
	BYTE Current_Num;//当前帧号
	BYTE Data_length;//数据区长度
	
	BYTE Content[235];   //数据区内容
}RF003RECORDER;

typedef struct sRFASDU7{  //无线模块发送对时报文
	BYTE minute_countl ;//无线模块分钟计数低字节
	BYTE minute_counth;//无线模块分钟计数高字节
	BYTE Reserve1;//预留
	BYTE Reserve2;//预留
	BYTE Reserve3;//预留
}RF003TIMESYN;
typedef struct sRFASDU8{  //无线模块参数
	BYTE TYPE2 ;//汇集单元或采集单元
	BYTE RF003_channel;//无线频道
	BYTE Group_AddrL;//组地址低
	BYTE Group_AddrH;//组地址高
	BYTE Dormancy_Cycle;//休眠周期
	BYTE Reserve1;//预留
	BYTE Reserve2;//预留
	BYTE Reserve3;//预留
	BYTE Reserve4;//预留
	BYTE Reserve5;//预留
	BYTE Reserve6;//预留
	BYTE Reserve7;//预留
	BYTE Reserve8;//预留
	BYTE Reserve9;//预留
}RF003PARA;

typedef struct sRFASDU4{  //指示器上传参数
    BYTE NetID;
    BYTE NetIP;
    BYTE LinePhase;
    BYTE ByConten[32];
}RF003ASDU;

typedef struct 
{
    BYTE Start; 
    BYTE Control; 
    BYTE Data[4];//BYTE Address; 
}VFrame10; 

typedef struct 
{
    BYTE Start1; 
    BYTE Length1; 
    BYTE Length2; 
    BYTE Start2; 
    BYTE Control; 
    BYTE Data[MAX_FRAME_LEN-5];//BYTE LinkAddress; 
}VFrame68;



typedef struct 
{
    BYTE Start1; 
    BYTE Start2;
    BYTE FrameLen;//include frame head and tail
    BYTE Type;
    union{
        RFASDUSUBTP RfAsduSubTp;
        RFASDU RfAsdu;
        RF001ASDU Rf001Asdu;
        RF003ASDU Rf003Asdu;
		REPEATERASDU repeaterAsdu;
		RF003RECORDER rf003recorder;
		RF003TIMESYN rf003timesyn;//无线模块对时报文
		RF003PARA rf003para;//无线模块参数
        }RFASDU;
}VFrame55;

typedef struct 
{
    BYTE Start[4]; 
    BYTE Type;
    BYTE FrameLen[2];
    BYTE byContent[MAX_FRAME_LEN-7];
}VFrameAA;

typedef struct 
{
    BYTE Start0;
    BYTE Start1; 
    BYTE Start2;
    BYTE FrameLen;//include frame head and tail
    BYTE Type;
    union{
        RFASDUSUBTP RfAsduSubTp;
        RFASDU RfAsdu;
        }RFASDU;
}VFrameFF;

typedef struct 
{
    BYTE Start1; 
    BYTE Length1; 
    BYTE Length2; 
    BYTE Start2; 
    BYTE Control;
    BYTE Addr_odu;
    BYTE Type_odu;
    BYTE VSQ_odu;
    BYTE COT_odu;
    BYTE Data[MAX_FRAME_LEN-9];//BYTE LinkAddress; 
}VFrame67; 
typedef struct 
{
    BYTE Start; //起始符 $GPRMC推荐定位信息数据格式$BDRMC,$GNRMC
    //BYTE Addr_Sec[5];//地址段，5个字节，分为两部分发送器标识符(2个字节GP/GN/BD)和语句类型(3个字节RMC/GGA)
    BYTE byContent[MAX_FRAME_LEN-5];//数据段
    BYTE FrameLen;
    BYTE flag;//定位是否有效
    BYTE flag_num;
		
}VFrameGPR;//NMEA协议框架；推荐最小定位信息

/*typedef struct 
{
	BYTE Content_Utc[9];//UTC时间
	BYTE Content_State;//定位状态
	BYTE Content_Latitude[9];//纬度
	BYTE Latitude_Direction;//纬度方向南纬或北纬
	BYTE Content_Longitude[9];//经度
	BYTE Longitude_Direction;//经度方向西经或东经
	BYTE Content_Date[6];//UTC日期 
}GPR_Frame;//GPR的分类信息*/

typedef struct 
{
    BYTE Start1; 
    BYTE Length1; 
    BYTE Length2; 
    BYTE Start2; 
    BYTE Control; 
	BYTE LinkAddr[2];
    BYTE Type;
    BYTE VSQ;
    BYTE COT[2];
	BYTE ConAddr[2];
	BYTE InfoAddr[2];
    BYTE Data[MAX_FRAME_LEN-15];//从文件名开始
}VFrame69;//69报文的链路地址，传输原因，公共地址和信息体地址长度都为两字节
typedef struct 
{
    BYTE line_phase[6]; 
	BYTE Recorder_flag;//30S定时到了的标志位
	BYTE Current_line_phase;//当前的线偏相偏
	
	BYTE Recorder_flag1;//A相召完的标志
	BYTE Recorder_flag2;//B 相召完的标志
	BYTE Recorder_Allflag;//三相都召完的标志
	BYTE Recorder_Current_Num;//当前终端召的第几条报文
	//unsigned int FRecorder_Current_COUNT;//flash保存到第几条了
	//unsigned int Recorder_Total_COUNT;//总的条数
    BYTE Recorder_reflag[6];//同一条线路上次录波没有完全结束时，又有新的故障上传
}VRecorder;//69报文的链路地址，传输原因，公共地址和信息体地址长度都为两字节
typedef struct 
{
    BYTE Start1; 
    BYTE LengthH; 
    BYTE LengthL; 
    BYTE Start2; 
    BYTE FrameType[2];  //报文类型,高位在前，高位为0
    BYTE AppType;       //应用类型
    BYTE Data[MAX_ENC_FRAME_LEN-8];//
}VFrameEB;//EB加密报文的
typedef union 
{
  VFrame69 Frame69;//为扩展才定义此共用体

} VParaFrame; 
/*typedef union 
{
  //为扩展才定义此共用体

} VParaFrame; */

//====================================================================




/*------------------------------------------------------------------------
 规约基类定义
------------------------------------------------------------------------*/
class CProtocol
{
    public:

        CProtocol();//构造函数
        virtual ~CProtocol(){};
        virtual BOOL Init(WORD wMinEqpNum);
        virtual void initpara(BYTE flag = 0){return;};
        virtual BOOL SetEqpInfo(void){return 1;};                      
        virtual DWORD SearchOneFrame(BYTE *Buf,WORD Len){return FRAME_OK|Len;};
        virtual BOOL DoReceive(void){ return TRUE; };
        virtual BOOL DoSend(void){ return TRUE; };
		virtual WORD GetOwnAddr(void);
		virtual void DisableRetry(void){return;};   //禁止重发
        virtual void DoCommSendIdle(void){DoSend();};
        virtual BOOL DoTimeOut(WORD wTimerID){return FALSE;};
        virtual void CloseTcp(BYTE byReason = 0){return;};
		virtual void Run(void){return;}; 
		virtual void SaveLBFlash(unsigned char *pLbBuff,unsigned char flag,unsigned int Val,BYTE Data_length1){return;};//
               
        void NeatenCommBuf(VCommBuf *pCommIO);
        BOOL SearchFrame(void);
        WORD SearchHead(BYTE *Buf, WORD Len,short Offset1,BYTE Key1);  
        int RcvData(int* iSocket);  
        int CommWrite(char* pbuf, int buflen, DWORD flags=0);
        BYTE ChkSum(BYTE *buf, WORD len);
		BOOL SendFrameHeadForPa(BYTE Style, BYTE Reason);
		BOOL SendFrameTailForPa(void);

		//参数读写部分
		BOOL RecFrame69(void);
		BOOL RecReadFile();
		BOOL RecWriteFile();
		void SendWrPaSuc();
		void SendReadPa(WORD FileName,BYTE SecName);
		
		void SendWrPaAck(WORD wFile,BYTE bySec);
		void SendSavedSoe(BYTE bySec,WORD wStartNo,WORD wSOENum);
		void SendLoadData(BYTE bySec,WORD wStartNo,WORD wLoadNum,BYTE byQds=0);
		void SendOneLoadData(WORD wCurLoadNum,WORD wFlag=0);
		void SendSoeSavedStu();
		void SendTrigAck();
		
		

public:
		VParaFrame *pReceiveFrame; 	//接收帧头指针		
		WORD m_wSendPaNum;		//已经发送的参数个数，控制多帧传送用
		

        VCommBuf m_RecBuf;              
        VCommBuf m_RecFrame;          
        VCommBuf m_SendBuf; 
		VRecorder Recorder;
        int m_uartId;
	CProtocol *m_RWPaSrcObj;//正在读/写rf或ODU参数的上行通道
	//unsigned int m_WFile;
        //unsigned char m_Sec;
        
        //char m_WPaOrderFlag;//标识写指示器参数是否成功
        
        BYTE m_SBuf[MAX_SEND_LEN];
        BYTE m_RBuf[MAX_RECV_LEN];
        BYTE m_initflag;//初始值为0111，请求远方链路状态后变为0110，复位远方链路后变为0100，发初始化结束帧后为0000
        BYTE m_linkflag; //1=链路初始化完成
        BYTE m_zdflag;//主动上传标志，主动上传后置1，收到主站回复后置0
		BYTE m_initfirstflag;//初始化为1，只在上电后的第一次发送初始化
        WORD m_wAreaNo;
		BYTE m_houxuflag;
};




#endif
