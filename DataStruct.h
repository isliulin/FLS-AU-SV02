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
 struct FIPARAINFO//��дָʾ��������Ϣ
  {
	  BYTE Fi_rflag;//��дָʾ������ı�־λ
	  BYTE Fi_wflag;//��дָʾ������ı�־λ
	  BYTE FiphaseA_flag;//������վ�ǰ���·�·�����ˣ��ֱ����ö�дABC��ָֻʾ���ı�־λ
	  BYTE FiphaseB_flag;
	  BYTE FiphaseC_flag;
	  BYTE Filinephase;//��ǰִ�е�ָʾ������ƫ����ƫ
	  WORD Fi_Info;//��д��������ʼ��Ϣ���ַ
	  WORD Fi_Info_Num; //����ʼ��ַ�����в������ֽ���
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
 
//================================== AD�ɼ�ģ�� ============================================
#ifdef _DRIVER_AD
  unsigned int  g_sSampleData[AC_AD_CHAN_NUM][AC_BUFF_LEN];//AD�ɼ�����
  unsigned int g_sSampleFinish;//�ɼ����
  char FarFalutEnFlag; //��Դ��������������ϴ�
  unsigned char g_OpenBatFlag;//��﮵�زɼ���ʶ ON:��AD�ɼ� TRUE:����ɵ��βɼ�10���� OFF:AD�ɼ�δʹ��
  unsigned int g_SunHighNum;//�ɼ�����̫���ܵ�ѹ�����ߵĴ���
#else
  extern unsigned int  g_sSampleData[AC_AD_CHAN_NUM][AC_BUFF_LEN];
  extern unsigned int g_sSampleFinish;
  extern unsigned char g_OpenBatFlag;
  extern unsigned int g_SunHighNum;//�ɼ�����̫���ܵ�ѹ�����ߵĴ���
#endif

//================================== ���ݴ���ģ�� ============================================
  struct sRMT_BACK
 {
   unsigned long m_RmtInfoBackNew;//��ң�ű仯ʱ
   unsigned int m_Year;
   unsigned int m_Month;
   unsigned int m_Day;
   unsigned int m_Hour;
   unsigned int m_Minute;
   unsigned int m_Milisecond; 
 };
struct SOE_FLASH_BACK//�����ӣ�Ϊ�˶�ȡSOE�ļ�¼
 {
   unsigned int m_InFlashTailNum;//���浽�ⲿFLASH��SOE�ĸ���;
   unsigned int m_unSSoeInFlashHead;//�ⲿFLASH��ͷָ�룬��Ϊ�ⲿFLASH���ʱ�����4096�ֽڣ���ѵ��һȦʱ����Ҫע�⣬��ָ��ָ����һ��ε�ͷָ�롣
   unsigned char m_OverWriteFlag;//��ǰFLASH�Ƿ�д���ı�־��д��֮��ͷָ��Ӧָ����һ���
 };

#ifdef _DATA_PROCESS
    unsigned int g_gRmtMeas[RMT_MEAS_NUM];//ң������
    unsigned int g_gRmtInfo[RMT_WORD_NUM];//ң���� һ��ң����Ϣռ2��bitλ

    unsigned int g_gRmtLock[RI_FTINFO_NUM];//ң�ű���ʱ�����
    BYTE g_gRmtLock10s[RI_FTINFO_NUM];//10S������
    unsigned int g_gRmtLockLB;//¼����־λң�ű���ʱ�����
    #ifdef LOCALYX 
    unsigned int g_gDIMcLock[EXT_DIN_NUM];//Ӳң���������ʱ�����
    BYTE g_gDIMcLock10s[EXT_DIN_NUM];//10S������
    unsigned int g_unDCosBuffTail;
	unsigned int g_unDSoeSavePtr;//�������˫��SOEָ��
	#endif
    unsigned char g_gSCosBuff[SOE_BUFF_NUM][SOE_DATA_LEN];
    
    unsigned char g_gGroundBuff[SOE_BUFF_NUM];
	unsigned char g_gshortBuff[SOE_BUFF_NUM];
    unsigned int g_unSCosBuffTail;
	unsigned int g_unSSoeInFlashTail;//�������ⲿflash�е�SOEβָ��
	SOE_FLASH_BACK g_InFlashInfo;//��ȡ��ʷsoeʱ���ⲿFLASH��ͷָ�뼰����
    unsigned int g_gSmsHeadPt;       //����ͷָ��
    unsigned int g_gSmsHeadPtrBk;    //����ͷָ�뱸��
    //unsigned int g_gSmsStartPtr;     //���ŷ��Ϳ�ʼָ��
    unsigned int g_RecFaultNum;      //���ŷ�����������
    unsigned int g_unGroundBuffTail;
	unsigned int g_unShortBufftail;
    
    unsigned int g_unSSoeSavePtr;//������ĵ���SOEָ��
    unsigned int g_unSSoeSaveFlashPtr;//�����浽�ⲿflash�ĵ���SOEָ��
    unsigned char   soe_ptr; //����SOE��flash�еĶ�λ��
    unsigned char   soe_area;//����SOE��flash����λ��
    unsigned int    soe_num;//SOE�ܸ���
    unsigned char   soe_overflag;//FLASH�Ƿ�д��
    unsigned char 	soe_Headptr;//ͷָ��
    unsigned int  g_yxChangeflag;//ң�ű�λ��ʶ BITF��ʶ���±����ϴ�(ң�ż�ң��) BIT0-5,6-11�ֱ��ʶ���Կں�gprs�ڶ�Ӧ��SYX,DYX,SSOE,DSOE,SSMS,DSMS;
    char g_AdChgSdYc;//��ʶAD�ɼ��Ƿ������ϴ�ON=�����ϴ�
    char g_FltChgSdYc;//��ʶ����ң���Ƿ������ϴ� 1=�����ϴ�
    char g_PtLoseFlag;//ptʧѹ��ʶ 0=δʧѹ 1=ʧѹ 2=��ѹ�ظ���
    char g_BatLoseFlag;//���ʧѹ��ʶ 0=δʧѹ 1=ʧѹ 2=��ѹ�ظ���   
    unsigned char g_PtMonitorCnt;//pt��ѹ�ָ�����
    unsigned char g_BatMonitorCnt;//��ص�ѹ�ָ�����
    char g_SendYc;//0=����ң��������ͣ�1=��������(��ʱ�ϱ���ң�⣬����ң�⣬AD�ɼ�ֵ�仯��ң�⣬Խ��ң��)
    unsigned char  g_Soenum;
    unsigned int  g_ZsqCmdRBit = 0;
    unsigned int  g_ZsqCmdWBit = 0;
    unsigned int  g_JdgPwMode = 0;
	unsigned int  g_SendLuboNum;
	unsigned int  g_SendLuboPage;
     WORD wCfgTotalNum =0;//�ܳ���
     WORD section_current=1;//��ǰ��*/
    BYTE g_Current_line = 0;
	unsigned int g_FRecorder_Current_COUNT;//flash���浽�ڼ�����
	unsigned int g_Recorder_Total_COUNT;//�ܵ�����
	//unsigned int g_Current_COUNT[FRECORDER_TOLNUM];//�ü���ڵĵڼ��ι��ϣ�1-64��Ӧ1-64���
	FIPARAINFO  g_RWFiInf;
	WORD g_wUpPaAddr[PA_ADDR_NUM];//��վ���·��Ĳ�����ַ����������Ϊ�˽�ʡ�ڴ�
        BYTE g_EncData[256];
    //DWORD  g_IHDCmdRBit = 0;   //ӳ�ͨ��ָ���־λ DWORD Ԥ��16������
   // DWORD  g_IHDCmdWBit = 0;   //ӳ�ͨдָ���־λ DWORD Ԥ��16������
#else

    extern unsigned int g_gRmtMeas[RMT_MEAS_NUM];//ң������������Ҫ�ϴ�����̨��ң������       
    extern unsigned int g_gRmtInfo[RMT_WORD_NUM];//ң���� һ��ң����Ϣռ2��bitλ

	extern unsigned int g_gRmtLock[RI_FTINFO_NUM];//ң�ű���ʱ�����
	extern BYTE g_gRmtLock10s[RI_FTINFO_NUM];
	extern unsigned int g_gRmtLockLB;//ң�ű���ʱ�����
    #ifdef LOCALYX 
    extern unsigned int g_gDIMcLock[EXT_DIN_NUM];//Ӳң���������ʱ�����
    extern  BYTE g_gDIMcLock10s[EXT_DIN_NUM];//10S������
    extern unsigned int g_unDSoeSavePtr;//�������˫��SOEָ��
	extern unsigned int g_unDCosBuffTail;
	#endif
    extern unsigned char g_gSCosBuff[SOE_BUFF_NUM][SOE_DATA_LEN];
    extern unsigned char g_gGroundBuff[SOE_BUFF_NUM];
	extern unsigned char g_gshortBuff[SOE_BUFF_NUM];
    extern unsigned int g_unSCosBuffTail;
    
	extern unsigned int g_unSSoeInFlashTail;//�������ⲿflash�е�SOEβָ��
	extern SOE_FLASH_BACK g_InFlashInfo;//��ȡ��ʷsoeʱ���ⲿFLASH��ͷָ�뼰����
    extern unsigned int g_gSmsHeadPt;       //����ͷָ��
    extern unsigned int g_gSmsHeadPtrBk;    //����ͷָ�뱸��
   // extern unsigned int g_gSmsStartPtr;     //���ŷ��Ϳ�ʼָ��
    extern unsigned int g_RecFaultNum;      //���ŷ�����������
    extern unsigned int g_unSSoeSavePtr;//������ĵ���SOEָ��
    extern unsigned int g_unSSoeSaveFlashPtr;//�����浽�ⲿflash�ĵ���SOEָ��
    extern unsigned int g_unGroundBuffTail;
	extern unsigned int g_unShortBufftail;
    extern unsigned char   soe_ptr; //����SOE��flash�еĶ�λ��
    extern unsigned char   soe_area;//����SOE��flash����λ��
    extern unsigned int    soe_num;//SOE�ܸ���
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
    extern WORD wCfgTotalNum ;//�ܳ���
    extern WORD section_current;//��ǰ��*/
    extern BYTE g_Current_line;
	extern unsigned int g_FRecorder_Current_COUNT;//flash���浽�ڼ�����
	extern unsigned int g_Recorder_Total_COUNT;//�ܵ�����
	//extern unsigned int g_Current_COUNT[FRECORDER_TOLNUM];
	extern FIPARAINFO  g_RWFiInf;
	extern WORD g_wUpPaAddr[PA_ADDR_NUM];//��վ���·��Ĳ�����ַ����������Ϊ�˽�ʡ�ڴ�
        extern BYTE g_EncData[256];
        //extern DWORD  g_IHDCmdRBit;   //ӳ�ͨ��ָ���־λ
    //extern DWORD  g_IHDCmdWBit;   //ӳ�ͨдָ���־λ    
#endif
struct sYK_Operation//ң�ؼ�¼
{
	unsigned int YKinfoaddr;//ң�ص�ַ
	//unsigned int YKtime[RTC_NUM];//ң��ʱ��
	unsigned char cmd;//select oper cancel
	unsigned char cmd_state;//ң������״̬
};
//=======================================  ��������ģ��  ===================================
#ifdef _PARA_CFG

        unsigned int g_unYxTrans[RMT_WSYX_NUM];  //����ң�ŵĵ�������������ɵ�ң������
        unsigned int g_unDYxTrans[RMT_WDYX_NUM];//�ϴ���˫��ң��
        
        unsigned int g_unYcTrans[IEC_YC_NUM];    //�ϴ���ң�����ݣ���������ϴ��ĳ���ˢ�����������ϴ�
        unsigned char g_ucYcTransNum; //��¼��Ҫ�ϴ���ң��ĸ���
        unsigned int g_ucYxTransNum; //��¼u��Ҫ�ϴ�ң�ŵ��ܸ���(���������˫��)
        unsigned int g_ucDYxTransNum;//��¼�ϴ���˫��ң�Ÿ���
        unsigned int g_ucSYxTrsStartId;//�ϴ��ĵ���ң���е�һ��������(��g_ucYXAddr������)
        unsigned int g_ucDYxTrsStartId;//�ϴ���˫��ң���е�һ��������(��g_ucYXAddr������)
        unsigned char g_ucYkTransNum;
        unsigned char g_ucParaChang ; //101�������������־  BIT0Ϊ 101������BIT1Ϊң����BIT2Ϊң��ϵ����BIT3Ϊң��ƫ������BIT4Ϊң�ŵ�� BIT5Ϊ���в�����
        unsigned char g_ucRefYxYcTs ;//ң��ң�����ݱ仯��־ BIT0Ϊң�� BIT1Ϊң��
        unsigned char g_ZsqPara ;   //zx
        unsigned char g_debug_rate; //101�������������־  BIT0Ϊ 101������BIT1Ϊң����BIT2Ϊң��ϵ����BIT3Ϊң��ƫ������BIT4Ϊң�ŵ�� BIT5Ϊ���в�����
		unsigned char g_rtc_time;//¼����ʱ�Ƿ�����
		unsigned char g_off_gps;//GPS�Ƿ�ر�
		
		unsigned char  g_gSmsPhone[PHONE_PA_NUM];//�����е绰����
		unsigned char  *g_pVersion;//�汾��
		unsigned char  *g_pCompany;//��˾��
		unsigned char  *g_pDevId;//��Ʒ�ͺ�
        unsigned int g_gRunPara[RUN_PARA_NUM];//���в���������װ�õ�һЩ������Ϣ������ͨ�Ų�����
     	unsigned char g_ucPara101[IEC_PARA_NUM];    //IEC���е����в���
     	unsigned char g_unParaObjComBk[4];//IEC�����еĶ˿ڶ���4�����ݽ��б��ݣ��б仯ʱ��ӳ���������˿ڵĶ�Ӧ��ϵ
        unsigned char g_ucYCAddr[IEC_YC_NUM + 1];   //ң��ĵ�����԰�ң���е����ݰ���ƫ�����������У���ఴ��20�����ã�Ϊ0�Ĳ��ϴ������Ϊ1-255
        unsigned int g_ucYXAddr[IEC_YX_NUM + 1];   //ң�ŵĵ��Ϊ0�Ĳ��ϴ������Ϊ1-255
        unsigned char g_ucYKPa[IEC_YK_NUM + 1];   //ң�ز��� ��ʶָʾ������ƫ��ƫ
	unsigned char g_gZCZSQ;  //�����ٲ�ָʾ����ң��ֵ
			
        unsigned int g_gRunParaFltLkTBk;//���в����й��ϱ���ʱ�䱸�ݣ����ڲ����仯ʱʹ��������Ч
        unsigned int g_gRunParaZsqStTBk;//���в�����ָʾ��ͨ��״̬������ڱ���
        unsigned int g_gRunParaYxRsdTTBk;//���в�����ң���ط�ʱ��������
		unsigned int g_gShortFltLkTBk;//���в����ж�·���ϸ�λʱ�䱸�ݣ����ڲ����仯ʱʹ��������Ч

		
        unsigned char g_open_oducall;//�����ϵ�ʱ5S�ٻ�һ�Σ��ȴ��ɼ���C�ĵ�ѹ�����ж����õ�ػ��ǳ������ݴ�����GPRS
        unsigned int Before4HourSdYcTm;
		#ifdef ODU
        unsigned char g_LineFlag[12];    //ODU����·����״̬?0Ϊ��������·�ϳ�1 Ϊ�ϳɵ�ǰһ·2 Ϊ�ϳɵĺ�һ·
		#endif
        unsigned char g_XPPara;
		
        unsigned int g_LogType = 0; //��־�ļ���ͬ����
        unsigned char g_Linkflag =0;
		unsigned char g_LogValue =0;
		unsigned char g_VerNum[9] ;//ϵͳ�汾��
		sYK_Operation g_YKinfo;
        extern const unsigned int BIT[16] =   //��õڼ�λΪ1��ֵ��BIT0~BIT15
    	{
                0x01,0x02,0x04,0x08,
        0x10,0x20,0x40,0x80,
        0x100,0x200,0x400,0x800,
        0x1000,0x2000,0x4000,0x8000
    	};
#else
        extern unsigned int g_unYxTrans[RMT_WSYX_NUM];  //����ң�ŵĵ�������������ɵ�ң������
        extern unsigned int g_unDYxTrans[RMT_WDYX_NUM];//�ϴ���˫��ң��

        extern unsigned int g_unYcTrans[IEC_YC_NUM];    //�ϴ���ң�����ݣ���������ϴ��ĳ���ˢ�����������ϴ�
        extern unsigned char g_ucYcTransNum; //��¼��Ҫ�ϴ���ң��ĸ���
        extern unsigned int g_ucYxTransNum; //��¼u��Ҫ�ϴ�ң�ŵĸ���
        extern unsigned int g_ucSYxTrsStartId;//�ϴ��ĵ���ң���е�һ��������(��g_ucYXAddr������)
        extern unsigned int g_ucDYxTrsStartId;//�ϴ���˫��ң���е�һ��������(��g_ucYXAddr������)
        extern unsigned int g_ucDYxTransNum;
        extern unsigned char g_ucYkTransNum;
        extern unsigned char g_ucParaChang; //101�������������־  BIT0Ϊ 101������BIT1Ϊң����BIT2Ϊң��ϵ����BIT3Ϊң��ƫ������BIT4Ϊң�ŵ�� BIT5Ϊ���в�����
        extern unsigned char g_ucRefYxYcTs;//ң��ң�����ݱ仯��־ BIT0Ϊң�� BIT1Ϊң��
        extern unsigned char g_debug_rate;   
		
		extern unsigned char  g_gSmsPhone[PHONE_PA_NUM];//�����е绰����
		extern unsigned char  *g_pVersion;//�汾��
		extern unsigned char  *g_pCompany;//��˾��
		extern unsigned char  *g_pDevId;//��Ʒ�ͺ�
		extern unsigned int g_gRunPara[RUN_PARA_NUM];//���в���������װ�õ�һЩ������Ϣ������ͨ�Ų�����
    	extern unsigned char g_ucPara101[IEC_PARA_NUM];    //IEC���е����в���
        extern unsigned char g_unParaObjComBk[4];//IEC�����еĶ˿ڶ���4�����ݽ��б��ݣ��б仯ʱ��ӳ���������˿ڵĶ�Ӧ��ϵ
        extern unsigned char g_ucYCAddr[IEC_YC_NUM + 1];   //ң��ĵ�����԰�ң���е����ݰ���ƫ�����������У���ఴ��20�����ã�Ϊ0�Ĳ��ϴ������Ϊ1-255
        extern unsigned int g_ucYXAddr[IEC_YX_NUM + 1];   //ң�ŵĵ��Ϊ0�Ĳ��ϴ������Ϊ1-255 
        extern unsigned char g_ucYKPa[IEC_YK_NUM + 1]; 
        extern unsigned char g_gZCZSQ;
        
       	extern unsigned int g_gRunParaFltLkTBk;//���в����й��ϱ���ʱ�䱸�ݣ����ڲ����仯ʱʹ��������Ч
		extern unsigned int g_gRunParaZsqStTBk;//���в�����ָʾ��ͨ��״̬������ڱ���
		extern unsigned int g_gRunParaYxRsdTTBk;//���в�����ң���ط�ʱ��������
        extern const unsigned int BIT[16];
		extern unsigned char g_open_oducall;
		extern unsigned int Before4HourSdYcTm;
		#ifdef ODU
		extern unsigned char g_LineFlag[12];
		#endif
		extern unsigned char g_XPPara;
		extern unsigned int g_LogType; //��־�ļ���ͬ����
		extern unsigned char g_Linkflag;
		extern unsigned char g_LogValue ;
		extern unsigned char g_VerNum[9];//ϵͳ�汾��
		extern sYK_Operation g_YKinfo;
		extern unsigned char g_rtc_time;//¼����ʱ�Ƿ�����
		extern unsigned char g_off_gps;//GPS�Ƿ�ر�
		extern unsigned int g_gShortFltLkTBk;//���в����ж�·���ϸ�λʱ�䱸�ݣ����ڲ����仯ʱʹ��������Ч
#endif

//=================================== ͨ��ģ�� ==========================================
  struct  sTX_BUFF		   //ͨ�ŷ��ͻ���ṹ��
	{
	  unsigned char m_gBuffer[MAX_TXBUFF_LEN];//�������ݻ���
	  unsigned int m_ucTx_Head;//GPRS�����жϷ������ݵĻ���ͷָ��
      unsigned int m_ucTx_Tail;//GPRS�����жϷ������ݵĻ���βָ��
      unsigned char m_unSendFlag;
	};

struct	sRx_Operation
{
	//unsigned char m_OperaBuf[MAX_RXBUFF_LEN];//�������ݻ���
    unsigned int m_Operaflag;      //���ݻ���βָ��
    unsigned int m_Operalength;      //���ݻ���ͷָ��
    
    unsigned int  m_OperaBufT; //�����ֽ�ʱ����
    //unsigned char m_iRcvFlag;
};

  struct  sRX_BUFF         //ͨ�Ž��ջ���ṹ��
  {
    unsigned char m_gBuf[MAX_RXBUFF_LEN];//�������ݻ���
    unsigned int m_iBufT;      //���ݻ���βָ��
    unsigned int m_iBufH;      //���ݻ���ͷָ��
    unsigned int m_iRcvCount; //�����ֽ�ʱ����
    unsigned char m_iRcvFlag;
  };
#ifdef _COMM
    __no_init struct  sTX_BUFF g_sTxBuff[COMM_PORT_NUM];
    __no_init struct  sRX_BUFF g_sRxBuff[COMM_PORT_NUM];
		struct sRx_Operation sRx_Oper;
        unsigned int g_sCommStatCount[RMT_ZSQST_NUM];//�ж�ָʾ��ͨ��״̬ʱ�����
        char g_YkOrderFlag;//��ʶң���Ƿ�ɹ�
         
        unsigned char g_GprsPowerSt;//GPRS��Դ״̬ 0=�ر� 1= ��?      
        unsigned char g_batPowerSt;//bat״̬ 0=�ر� 1= ��?	
        unsigned char g_gpsPowerSt;
		unsigned char g_fault_phase;//  
		unsigned int  g_differI_weizhi;
		unsigned int  g_differV_weizhi;
				 int  g_faultI_Valse;
				 int  g_faultV_Valse;
        unsigned char g_SmsWritePtr;
        unsigned char g_EnSmsSend;  //���ŷ�������
        unsigned char g_SmsDataLen;
        unsigned char g_EnSendToXinhao;
        unsigned char g_MesReadFlag;   //���Ų�ѯ��־λ
        unsigned int g_SmsCmdCode;    //��������У����
        unsigned char g_phoneNum[16]; //�յ����ŵ��ֻ��Ŵ洢�����ڻض���
        unsigned int g_WFile;
        unsigned char g_Sec;
        char g_WPaOrderFlag;//��ʶдָʾ�������Ƿ�ɹ�
        unsigned char g_SmsReSendWaitCount; //�����ط��ȴ�ʱ��
        unsigned char g_SmsReSendTime;   //�����ط�����
    	unsigned char g_SmsSendBuf[255];
		unsigned char g_GprsPowerConsum;
		
		unsigned char g_CmIdRF;//RF����ʹ�õĴ��ں�
		unsigned char g_CmIdGps;//GPS����ʹ�õĴ��ں�
		unsigned char  g_CmIdGPRS;
		unsigned char g_CmIdEX;//��չ��ʹ�õĴ��ں�
		unsigned char g_CmIdDBG;//���Կ�ʹ�õĴ��ں�
		unsigned char g_CmIdODU1;//ODU1ʹ�õĴ��ں�
		unsigned char g_CmIdODU2;//ODU2ʹ�õĴ��ں�
		unsigned int g_RstartNum;//ϵͳ��������
		//char  ComtrderCfg1[700];
		unsigned char g_TimeRF003_flag;//���߶�ʱ���
		BYTE g_step_earth;
		BYTE gloss_phase_flag[3];
		BYTE g_phase_loss[3];
		BYTE g_detephase_flag;
		BYTE gRecorder_flagall;//¼�����̱�־λ
                
		unsigned int g_GPS_READ_NUM;//RTC��GPS֮����л���ͬʱ�ﵽ�������õĴ������ϱ�gps״̬�쳣
		unsigned int g_RF_pps;//RF��min�����ʱ
                BYTE g_RF_Reset;//RF��λ��ʱ
		//WORD g_validity[64];
		 unsigned char g_EncComNo;        //�ĸ��������ܼ�������1==GPRS 2 ==DeBug�� ������Ч
		//WORD g_validity[64];
        unsigned char g_bEncCerTxt[1024];  //����CER֤��1024
        unsigned char g_bHostRandom[8];  //��վ�����
        unsigned char m_bUnitRandom[8];   //�ն��������������������ά����ͨ��
        int g_wEncCerWPtr;                 //Cer֤���¼λ��

        unsigned char g_bCommFlag[COMM_PORT_NUM];        //EB��֡��־λ
        unsigned char g_bCheckEBFrameBuf[COMM_PORT_NUM][4]; //EB֡ͷ�洢����
        unsigned int g_wCheckEBByte[COMM_PORT_NUM];                      //EB֡�����ֽڼ���
        unsigned int g_wEBFrameLen[COMM_PORT_NUM];                         //EB֡��
        unsigned char g_bRecFrameEBFlag[COMM_PORT_NUM];      //��������EB֡��־λ
        //unsigned char g_wTestDebug;
        //unsigned long g_lRunTimes;
		//BYTE g_YC_value[4];
        char g_bUnitID[25];//�ն�ID,24�ֽڵ�һ�����ǽڳ���   
        unsigned char g_bAppType;//���ܱ���Ӧ������
		

#else
    extern struct  sTX_BUFF g_sTxBuff[COMM_PORT_NUM];
    extern struct  sRX_BUFF g_sRxBuff[COMM_PORT_NUM];
	extern struct sRx_Operation sRx_Oper;
    extern unsigned int g_sCommStatCount[RMT_ZSQST_NUM];
    extern char g_YkOrderFlag;//��ʶң���Ƿ�ɹ�
    extern char g_WPaOrderFlag;//��ʶдָʾ�������Ƿ�ɹ�
    extern unsigned char g_GprsPowerSt;//GPRS��Դ״̬ 0=�ر� 1= ��
     extern unsigned char g_batPowerSt;//bat״̬ 0=�ر� 1= ��?    extern int g_ODUYK;//
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
	extern unsigned char g_CmIdRF;//RF����ʹ�õĴ��ں�
	extern unsigned char g_CmIdGps;//GPS����ʹ�õĴ��ں�
	extern unsigned char  g_CmIdGPRS;
	extern unsigned char g_CmIdEX;//��չ��ʹ�õĴ��ں�
	extern unsigned char g_CmIdDBG;//���Կ�ʹ�õĴ��ں�
	extern unsigned char g_CmIdODU1;//ODU1ʹ�õĴ��ں�
	extern unsigned char g_CmIdODU2;//ODU2ʹ�õĴ��ں�
	extern  unsigned char g_gpsPowerSt;
    extern int g_pODU2WaitCount;//ODU��Դ�ر�����ODU2�Ľ��ռ�ʱ
	extern int g_pODUWaitCount;//ODU��Դ�ر�����
	extern unsigned int g_RstartNum;//ϵͳ��������
	//extern char  ComtrderCfg1[700];
	extern unsigned char g_TimeRF003_flag;//���߶�ʱ���
    extern BYTE g_step_earth;
	extern BYTE gRecorder_flagall;//¼�����̱�־λ
	extern BYTE gloss_phase_flag[3];
	extern BYTE g_phase_loss[3];
	extern BYTE g_detephase_flag;
	extern unsigned int g_GPS_READ_NUM;//RTC��GPS֮����л���ͬʱ�ﵽ�������õĴ������ϱ�gps״̬�쳣
	//extern WORD g_validity[64];
	extern unsigned int g_RF_pps;//RF��min�����ʱ
        extern BYTE g_RF_Reset;//RF��λ��ʱ
	 extern unsigned char g_EncComNo;
	extern unsigned char g_bEncCerTxt[1024];//1024
        extern unsigned char g_bHostRandom[8];  //��վ�����
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
    
//=======================DriverTimer.c--��ʱ������ģ��===============================================
 struct sTIMER_MANAGER
  {
    unsigned int m_TmCount;
    unsigned char m_TmFlag;
	unsigned int m_TmCountBk;//������һ�����õĶ�ʱ���
  };
#ifdef _DRIVER_TIMER
        struct sTIMER_MANAGER g_sTimer[TIMER_NUM];
        unsigned int Time_10ms_T;
		//unsigned int Time_1s_T;
        unsigned int g_DateCount;  //״̬�㱨����������
        unsigned char g_StatusTime;
        unsigned char g_SendStatus;
        unsigned char g_SmsSendStatus; //���Ż㱨״̬��־λ
        unsigned char g_EnAdTmCount;//ʹ��AD�ɼ���ʱ
    	unsigned char g_EnAdTmFlag;//AD�ɼ���ʱ����(����AD�ж�)
        unsigned char g_SendFault;//������ģʽ�����������ӳ�
        unsigned char g_OpenAdDelay;//��AD����ʱ�������AD�ɼ���ԭ���õĶ�ʱ�������ڷֿ���ODU��ʱ320ms������������ʱ2s
        unsigned int g_WatchWireTime;//��������ģ���ʱ
        unsigned char g_WireStatus;//����ģ�����ι���ź�
        unsigned char g_WireStatusOld;//����ģ�����ι���źŻ���
        unsigned char g_ResetWireFlag;//����ģ��������־λ
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
//=================================== ʱ��ģ�� ==========================================

  struct sRTC_MANAGER
  {
	unsigned char m_FixYCFlag;   //����ң���ļ�¼
	unsigned char m_ucRtcSynFlag;   //ϵͳʵʱʱ�Ӵ�ʵʱоƬ�ж�ȡ�ı�־��ÿ5���Ӷ�ȡһ��
    unsigned int m_gRealTimer[RTC_NUM]; //ϵͳ�ڲ�ʵʱʱ�ӣ��ꡢ�¡��ա�ʱ���֡��롢����
	unsigned int m_gRealTimerBK[RTC_NUM];
	unsigned int m_gRealTimerFlag; //ON���յ���ʱ���ģ�TRUE MS=1000ʱдRTCоƬ��ʱ�䣬
};
struct CRYSTAL_ERROR
	{
		DWORD  Crystal_start_num;//GPSÿ������֮�䲶���������֮���ۻ��ľ������
		DWORD  Crystal_end_num;
		unsigned int start_RealTime[3] ;//GPS��һ�δ򿪼�¼��ʵʱʱ����� �� ����
		unsigned int end_RealTime[3] ;//GPS�ڶ��δ򿪼�¼��ʵʱʱ����� �� ����
		unsigned char Crystal_start_flag;//GPS��ʱ�������һ�β���������ı�־λ��GPS����ͷ����Ǵӱ����еõ��ģ����������������еõ���
		unsigned char Crystal_end_flag;
		unsigned char Compen_cry_flag;//gps���Լ��ǵþ����ܵĿ죬�򲹳�ʱ�����1���������1
		DWORD Compen_cry_total;//ÿCompen_cry_num ms����һ������
		DWORD Compen_cry_num;//ms�жϼ���
		unsigned char diff_value_num;//����û��ǿ����Ĵ���
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
		unsigned int RF003_absolute_time[3][RTC_NUM];//�յ�����������þ���ʱ��
		
		unsigned int RF003_mini_num[3];//RF003�ķ��Ӽ���
		unsigned char RF003_time_flag;//RF001���������ն�ʱ������������Ǳ��ģ��յ�������OFF,�յ�������ON
		unsigned int LUBO_absolute_time[RTC_NUM];//¼��CFG�ļ��ľ���ʱ�꣬��ʱ�����յ�ABC��ָֻʾ�����һֻ����Чʱ���ʱ��
		unsigned char LUBO_time_flag;//¼��ʱ�����Ч��
	};
  struct RECORDER_LUBO
  {
	
	//unsigned char lubo_flag;
	//WORD  relative_time;
	unsigned char lubo_flag;//¼����־λ
	//WORD     relative_timel;
	//WORD     relative_timeh;
	DWORD relative_time1;
	DWORD relative_time2;
	DWORD relative_time3;
	unsigned int total_num1;   //�ܵ�֡��
	unsigned int  recorder_long;//�ܳ���
	unsigned char VI_flag; //��ѹ������־λ
	unsigned int absolute_time[RTC_NUM];//����ʱ��
	unsigned char line_recorder;//��ƫ
	unsigned char phase_recorder;//��ƫ
	//unsigned int  total_long;
	//unsigned char lubo_type;  
    unsigned int current_num; //��ǰ֡��
    unsigned char lubo_start_type;//¼��������־
    
  };
  
  
  /*struct DFT_DATA_EXT
  {
    long  Real;
    long  Imag;
    long  LowReal;
    long  LowImag;
    unsigned char Cnt;
   };//Ϊʲô���ܷ���highcoefs.h
   
  #define   lubo_absolute_time  11  //����ʱ������λ��1+2+2+2+2+1

  struct RECORDER_LUBO_flag
  {
    WORD     relative_time[4];
	unsigned int absolute_time[RTC_NUM];
	unsigned char recorder_flag[4];
	unsigned char recorder_addr[4];
  };*/
  
 //¼����¼
#ifdef _DRIVER_RTC
  struct OUTPUT_INFO g_OutputInfo;
  struct RF003_TIME g_RF003_timeInfo;//RF003��ָʾ��ʱ��������Ϣ
  struct sRTC_MANAGER g_sRtcManager;  //ϵͳʵʱʱ��
  struct RECORDER_LUBO g_recorder; 
  unsigned int g_buchangnum2;//����Ķ���������ÿ14�����ڼ�����һ��
  unsigned int g_buchangnum;//�����һ��������ÿ4��ms������һ������
  unsigned int g_ccr0;
  struct  CRYSTAL_ERROR g_CrystalError;//����������
  //unsigned int g_mem[500];
  //struct RECORDER_LUBO *precorder;
#else
  extern struct OUTPUT_INFO g_OutputInfo;
  extern struct RF003_TIME g_RF003_timeInfo;//RF003��ָʾ��ʱ��������Ϣ
  extern struct sRTC_MANAGER g_sRtcManager;
  extern  struct RECORDER_LUBO g_recorder; 
  extern  unsigned int g_buchangnum2;//����Ķ���������ÿ14�����ڼ�����һ��
  extern  unsigned int g_buchangnum;//�����һ��������ÿ4��ms������һ������
  extern  unsigned int g_ccr0;
  extern  struct  CRYSTAL_ERROR g_CrystalError;//����������
  //extern  unsigned int g_mem[500];
  //struct RECORDER_LUBO *precorder;
#endif

//================================== IO����ģ�� ===========================================
#ifdef _DRIVER_IO
	#ifdef LOCALYX
	    unsigned long g_gExtDinStat;    //�ⲿ����������ȷ��״̬
	unsigned long mtemp;
	    unsigned int g_gExtDinYesTimer[EXT_DIN_NUM];    //�ⲿDIN����ȷ��ʱ���ʱ
	    unsigned int g_gExtDinNoTimer[EXT_DIN_NUM];     //�ⲿDIN�������ʱ���ʱ
	    unsigned int g_gDinRtcLock[EXT_DIN_NUM][RTC_NUM];   //DIN������ʵʱʱ��ʱ�����棬��DINȷ�Ϻ����SOEʱ��Ϊʱ�걣��
        //unsigned int g_unRF_RST;       //����ͨ��ģ��WDT״̬
        //unsigned char g_ucActFlag[OUT_TYPE_NUM];//�����߼���־����Ҫ���ڶ������ڸ����������ϱ�־ON����Ҫ�ջس���������ñ�־Ϊ OFF�����κ�����ʱΪ0
	unsigned char YX_delay_clear_flag;
    #endif
#else
		#ifdef LOCALYX
    	extern unsigned int g_gDinRtcLock[EXT_DIN_NUM][RTC_NUM];
        //extern unsigned int g_unRF_RST;       //����ͨ��ģ��WDT״̬
        //extern unsigned char g_ucActFlag[OUT_TYPE_NUM];//�����߼���־����Ҫ���ڶ������ڸ����������ϱ�־ON����Ҫ�ջس���������ñ�־Ϊ OFF�����κ�����ʱΪ0
		extern unsigned char YX_delay_clear_flag;
		#endif

#endif

#ifdef __cplusplus
}
#endif

#endif  //end of file
