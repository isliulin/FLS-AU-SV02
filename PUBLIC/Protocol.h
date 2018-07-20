/*------------------------------------------------------------------------
 Module:            protocol.h
 Author:            linxueqin
 Project:           ��Լƽ̨
 State:             V1.0
 Creation Date:     2012-10-8
 Description:       ��Լ���������
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
    

//ͨѶ���λ����
#define FRAME_SHIELD   0xFFFF0000      //������
#define FRAME_OK       0x00010000      //��⵽һ��������֡
#define FRAME_ERR      0x00020000      //��⵽һ��У������֡
#define FRAME_LESS     0x00030000      //��⵽һ����������֡����δ���룩


struct VCommBuf 
{
    WORD   wBufSize;        //��������С
    WORD   wReadPtr;        //��ָ��
    WORD   wOldReadPtr;     //��ָ��
    WORD   wWritePtr;       //дָ��
    DWORD  dwFlag;          //������(��Ҫ���ڷ���)
    BYTE   *pBuf;           //������
};



typedef struct YtOrder//[����״̬(0xA5A5)+��ʶ(16bit:0xC3)]+��ַ(16bit)+����(16bit)+У��(16bit)
{
    WORD wYtNum;
    WORD wYtNo;
    WORD wOrder;
    WORD wCheck;//У��
}YtOrder;

typedef struct YkOrder//[����״̬(0xA5A5)+��ʶ(16bit:0xC3)]+��ַ(16bit)+����(16bit)+У��(16bit)
{
    WORD wYkNo;
    WORD wOrder;
    WORD wCheck;//У��
}YkOrder;

typedef struct OrderHead//����״̬(0xA5A5)+��ʶ(16bit:0xC3)+[��ַ(16bit)+����(16bit)+У��(16bit)]
{
    WORD wStatus;
    WORD wSign;
}OrderHead;

typedef struct OrderFeedBack//��ʶ(16bit:0xC3)+����״̬(0xA5A5)
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
  //struct VCalClock Time  ;//����������󣬴˽ṹ��ָ�벻����ȷָ��һ���ڴ�
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
  WORD wID ;       /*ң�غ�*/
  BYTE byValue ;   /*ң������
                  D0~D1
                   00 �Ƿ�
                   01 ��բ�̵���
                   10 ��բ�̵���
                   11 NULL���������ֺ�/��բ�̵�����
                  D2��1���ƶ���Ϊ���غţ����п��غŲ��ң���ȱʡ�� 0�����ƶ���Ϊ˳���ţ�ƫ�ƣ�
                  D3������
                  D4-D7:  0���豸�Զ������壨ȱʡ��
                          1��������
                          2 ������
                          3 �������*/
  BYTE byStatus ;  /*��ʾ������������Ľ��*/
};

struct VDBYT{
  WORD wID ;      /*ң�غ�*/
  WORD wValue ;   /*ң��ֵ*/
  BYTE byStatus ;
  BYTE pad1[3]; /*armalign add*/
};



struct VMsgHead{
  WORD wLength;
  BYTE byMsgID;    /*D0~D6  MsgID
          D7 �������У�1��ʾ�Է���Ӧ����ʱ���跢�ͽ�������
           ��0��ʾ�Է�����Ӧ���Ļ��Ӧ����ʱ�跢�ͽ�������*/
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

#define SOE_GRP_NUM    22//ÿ������������22��soe ÿ��SOEռ11�ֽ�



//=================RF��Լ��ODU��Լ==========================================================

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


#define   RF001FTDANUM        17  //ÿһ��ָʾ���������Zռ17�ֽ� �ܿ�2ң
#define   RF001FTDANUM_A0     9  //һңÿһ��ָʾ���������Zռ9�ֽ� �ܿ�1ң
#define   RF001FTDANUM_3P4O   30  //ÿһ��ָʾ���������Zռ30�ֽ� ��Ӧ����3P4O���ָʾ��


#define   RF003FTDANUM  29  //ÿһ��ָʾ���������Zռ29�ֽ�


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
    BYTE byExtTmpt;//�ⲿ�¶�
    BYTE byIntTmpt;//�ڲ��¶�
    BYTE byIL;//�������ֽ�
    BYTE byIH;//�������ֽ�
    BYTE byPhFaultInfo;//����ϱ�־
}PHASEFTDA;

typedef struct sRF001FTDA_3P4O{
    BYTE byAddr[4];
    BYTE byLinePhase;
    BYTE byGroup[2];
    BYTE byDaSt;//������״̬
    BYTE byFaultOld;//��ʷ������
    BYTE byFaultCur;//��ǰ������
    BYTE byFaultComm;//ͨ��������
    BYTE byPhData[18];//A,B,C 3�������Ϣ���� 3*5 + O��3�ֽ�
    //PHASEFTDA *pPhFtConA;//BYTE byPhData[15];
    //PHASEFTDA *pPhFtConB;
    //PHASEFTDA *pPhFtConC;
    //BYTE byOIL;//O��������ֽ�
    //BYTE byOIH;//O��������ֽ�
    //BYTE byOFaultInfo;//O����ϱ�־
    BYTE byFaultInfoRst;//������Ϣ��λ��־
}RF001FTDA_3P4O;

typedef struct sODUFTDA{
    BYTE byLinePhase;//��ƫ��ƫ
    BYTE byRes;
    BYTE byFaultVal;//���ϱ�ʶλ
}ODUFTDA;

typedef struct sODUYCDA{
    BYTE byZsqType;//ָʾ������
    BYTE byExtTmpt;//�ⲿ�¶�
    BYTE byIntTmpt;//�ڲ��¶�
    BYTE byIL;//�������ֽ�
    BYTE byIH;//�������ֽ�
    BYTE byZsqSt;//ָʾ��״̬
}ODUYCDA;


typedef struct sRF003FTDA{
  
    BYTE len1;  //����     // 11 D2 11 00
    BYTE subType;//����
    BYTE len2;//���� ֡��
    BYTE lubo_type;//¼����������
    BYTE byUH;
    BYTE byIL;
    BYTE byIH;
    BYTE byIMaxL;
    BYTE byIMaxH;
    BYTE byIMinL;
    BYTE byIMinH;//��¼�����ݱ�־λ
    BYTE byFaultInfo;//���ϱ�־λ
    BYTE byUL;
	BYTE minute_countL;//���Ӽ���  ˵��:Ϊ�˼���֮ǰ�汾�ĳ��򣬸�֡������¼������ʱ����ʾ���Ƿ�����
	BYTE minute_countH;//���Ӽ���  ˵��:¼�����ݱ�־λΪ0ʱ����ʾ�����¶�ֵ��ȡ��ֵ
	BYTE Relative_time[4];// ���ʱ���ֽ�
	BYTE by_TemperaTure;//�¶�  ˵�������������ֽ�ֻ��¼�����ݱ�־λΪ1ʱ�Ž���
	BYTE by_TakePower;//ȡ���ѹ
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
typedef struct sRFASDU3{  //ָʾ���ϴ�����
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
typedef struct sRFASDU5{  //�м�����ص�ѹ
    BYTE NetID;
    BYTE NetIP;
    BYTE LinePhase;
    BYTE Len;
    BYTE Type;
    BYTE SubLen;
    BYTE SubReserve;
    BYTE CapValt[2];   //���ݵ�ѹ 
    BYTE BatValt[2];   //��ص�ѹ
}REPEATERASDU;

typedef struct sRFASDU6{  //ָʾ��¼�����ݱ��ĸ�ʽ
	BYTE Total_Num ;//֡����
	BYTE Current_Num;//��ǰ֡��
	BYTE Data_length;//����������
	
	BYTE Content[235];   //����������
}RF003RECORDER;

typedef struct sRFASDU7{  //����ģ�鷢�Ͷ�ʱ����
	BYTE minute_countl ;//����ģ����Ӽ������ֽ�
	BYTE minute_counth;//����ģ����Ӽ������ֽ�
	BYTE Reserve1;//Ԥ��
	BYTE Reserve2;//Ԥ��
	BYTE Reserve3;//Ԥ��
}RF003TIMESYN;
typedef struct sRFASDU8{  //����ģ�����
	BYTE TYPE2 ;//�㼯��Ԫ��ɼ���Ԫ
	BYTE RF003_channel;//����Ƶ��
	BYTE Group_AddrL;//���ַ��
	BYTE Group_AddrH;//���ַ��
	BYTE Dormancy_Cycle;//��������
	BYTE Reserve1;//Ԥ��
	BYTE Reserve2;//Ԥ��
	BYTE Reserve3;//Ԥ��
	BYTE Reserve4;//Ԥ��
	BYTE Reserve5;//Ԥ��
	BYTE Reserve6;//Ԥ��
	BYTE Reserve7;//Ԥ��
	BYTE Reserve8;//Ԥ��
	BYTE Reserve9;//Ԥ��
}RF003PARA;

typedef struct sRFASDU4{  //ָʾ���ϴ�����
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
		RF003TIMESYN rf003timesyn;//����ģ���ʱ����
		RF003PARA rf003para;//����ģ�����
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
    BYTE Start; //��ʼ�� $GPRMC�Ƽ���λ��Ϣ���ݸ�ʽ$BDRMC,$GNRMC
    //BYTE Addr_Sec[5];//��ַ�Σ�5���ֽڣ���Ϊ�����ַ�������ʶ��(2���ֽ�GP/GN/BD)���������(3���ֽ�RMC/GGA)
    BYTE byContent[MAX_FRAME_LEN-5];//���ݶ�
    BYTE FrameLen;
    BYTE flag;//��λ�Ƿ���Ч
    BYTE flag_num;
		
}VFrameGPR;//NMEAЭ���ܣ��Ƽ���С��λ��Ϣ

/*typedef struct 
{
	BYTE Content_Utc[9];//UTCʱ��
	BYTE Content_State;//��λ״̬
	BYTE Content_Latitude[9];//γ��
	BYTE Latitude_Direction;//γ�ȷ�����γ��γ
	BYTE Content_Longitude[9];//����
	BYTE Longitude_Direction;//���ȷ��������򶫾�
	BYTE Content_Date[6];//UTC���� 
}GPR_Frame;//GPR�ķ�����Ϣ*/

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
    BYTE Data[MAX_FRAME_LEN-15];//���ļ�����ʼ
}VFrame69;//69���ĵ���·��ַ������ԭ�򣬹�����ַ����Ϣ���ַ���ȶ�Ϊ���ֽ�
typedef struct 
{
    BYTE line_phase[6]; 
	BYTE Recorder_flag;//30S��ʱ���˵ı�־λ
	BYTE Current_line_phase;//��ǰ����ƫ��ƫ
	
	BYTE Recorder_flag1;//A������ı�־
	BYTE Recorder_flag2;//B ������ı�־
	BYTE Recorder_Allflag;//���඼����ı�־
	BYTE Recorder_Current_Num;//��ǰ�ն��ٵĵڼ�������
	//unsigned int FRecorder_Current_COUNT;//flash���浽�ڼ�����
	//unsigned int Recorder_Total_COUNT;//�ܵ�����
    BYTE Recorder_reflag[6];//ͬһ����·�ϴ�¼��û����ȫ����ʱ�������µĹ����ϴ�
}VRecorder;//69���ĵ���·��ַ������ԭ�򣬹�����ַ����Ϣ���ַ���ȶ�Ϊ���ֽ�
typedef struct 
{
    BYTE Start1; 
    BYTE LengthH; 
    BYTE LengthL; 
    BYTE Start2; 
    BYTE FrameType[2];  //��������,��λ��ǰ����λΪ0
    BYTE AppType;       //Ӧ������
    BYTE Data[MAX_ENC_FRAME_LEN-8];//
}VFrameEB;//EB���ܱ��ĵ�
typedef union 
{
  VFrame69 Frame69;//Ϊ��չ�Ŷ���˹�����

} VParaFrame; 
/*typedef union 
{
  //Ϊ��չ�Ŷ���˹�����

} VParaFrame; */

//====================================================================




/*------------------------------------------------------------------------
 ��Լ���ඨ��
------------------------------------------------------------------------*/
class CProtocol
{
    public:

        CProtocol();//���캯��
        virtual ~CProtocol(){};
        virtual BOOL Init(WORD wMinEqpNum);
        virtual void initpara(BYTE flag = 0){return;};
        virtual BOOL SetEqpInfo(void){return 1;};                      
        virtual DWORD SearchOneFrame(BYTE *Buf,WORD Len){return FRAME_OK|Len;};
        virtual BOOL DoReceive(void){ return TRUE; };
        virtual BOOL DoSend(void){ return TRUE; };
		virtual WORD GetOwnAddr(void);
		virtual void DisableRetry(void){return;};   //��ֹ�ط�
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

		//������д����
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
		VParaFrame *pReceiveFrame; 	//����֡ͷָ��		
		WORD m_wSendPaNum;		//�Ѿ����͵Ĳ������������ƶ�֡������
		

        VCommBuf m_RecBuf;              
        VCommBuf m_RecFrame;          
        VCommBuf m_SendBuf; 
		VRecorder Recorder;
        int m_uartId;
	CProtocol *m_RWPaSrcObj;//���ڶ�/дrf��ODU����������ͨ��
	//unsigned int m_WFile;
        //unsigned char m_Sec;
        
        //char m_WPaOrderFlag;//��ʶдָʾ�������Ƿ�ɹ�
        
        BYTE m_SBuf[MAX_SEND_LEN];
        BYTE m_RBuf[MAX_RECV_LEN];
        BYTE m_initflag;//��ʼֵΪ0111������Զ����·״̬���Ϊ0110����λԶ����·���Ϊ0100������ʼ������֡��Ϊ0000
        BYTE m_linkflag; //1=��·��ʼ�����
        BYTE m_zdflag;//�����ϴ���־�������ϴ�����1���յ���վ�ظ�����0
		BYTE m_initfirstflag;//��ʼ��Ϊ1��ֻ���ϵ��ĵ�һ�η��ͳ�ʼ��
        WORD m_wAreaNo;
		BYTE m_houxuflag;
};




#endif
