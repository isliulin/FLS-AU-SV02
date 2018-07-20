
#ifndef PrtcRF_H
#define PrtcRF_H

#include "..\PUBLIC\Pcolsec.h"
#include "..\highcoefs.h"

//extern char ComtrderCfg1[];
typedef union 
{
    VFrame55 Frame55;
    VFrameFF Frameff;
	VFrame69 Frame69;//�Զ���Э�� ���Ľṹ��101��ͬ��ֻ�Ǳ���ͷΪ0x69 for ������д
} VIecRfFrame; 
//unsigned char WriteTime_lubo,WriteFlag_lubo;//д�������д���־0x55��ʧ�ܣ�0x66���ɹ�
typedef struct  {
    INT32S  Real;
    INT32S  Imag;
    INT32S  LowReal;
    INT32S  LowImag;
    INT8U   Cnt;
}DFT_DATA_EXT;
const INT16S CosTab[80]= { 
	26214, 26133, 25891, 25490, 24931, 24218, 23357, 22351, 21207, 19933, 18536, 17024, 
		15408, 13697, 11901, 10031, 8100, 6119, 4100, 2056, 0, -2056, -4100, -6119,
	-8100, -10031, -11901, -13696, -15408, -17024, -18536, -19933, -21207, -22351, -23357,
	-24218, -24931, -25490, -25891, -26133, -26214, -26133, -25891, -25490, -24931,
	-24218, -23357, -22351, -21207, -19933, -18536, -17024, -15408, -13697, -11901,
	-10031, -8100, -6119, -4100, -2056, 0, 2056, 4100, 6119, 8100, 10031, 11900, 
	13696, 15408, 17024, 18536, 19933, 21207, 22351, 23357, 24218, 24931, 25490,
	 25891, 26133
};         
const INT16S SinTab[80]= { 
	0, 2056, 4100, 6119, 8100, 10031, 11901, 13696, 15408, 17024, 18536, 19933, 21207, 
		22351, 23357, 24218, 24931, 25490, 25891, 26133, 26214, 26133, 25891, 25490,
	24931, 24218, 23357, 22351, 21207, 19933, 18536, 17024, 15408, 13697, 11901, 10031,
	8100, 6119, 4100, 2056, 0, -2056, -4100, -6119, -8100, -10031, -11901, -13696, 
	-15408, -17024, -18536, -19933, -21207, -22351, -23357, -24218, -24931, -25490, 
	-25891, -26133, -26214, -26133, -25891, -25490, -24931, -24218, -23357, -22351, 
	-21207, -19933, -18536, -17024, -15408, -13697, -11901, -10031, -8100, -6119,
	 -4100, -2056
};

class CPrtcRF : public CProtocol//CPSecondary
{
	public:
		VIecRfFrame *pReceiveFrame; 	//����֡ͷָ��
		VIecRfFrame *pSendFrame;  		//����֡ͷָ��
		BYTE m_PaWaitflag;//ON��ʾ�����ط���ʱ TRUE��ʾ�ط���ʱ�� OFF��ʾ�ط�����(�ط����������յ��ظ�)
		WORD m_PaWaitCt;//du����ʱ�ط���ʱ
		WORD m_TxNum;//�ش�����

		//BYTE m_PaWaitflag_rf003;//ON��ʾ�����ط���ʱ TRUE��ʾ�ط���ʱ�� OFF��ʾ�ط�����(�ط����������յ��ظ�)
		WORD m_PaWaitCt_rf003;//du����ʱ�ط���ʱ
		WORD m_TxNum_rf003;//�ش�����
                
                
                BYTE m_PaWaitflag_rf003;//ON��ʾ�����ط���ʱ TRUE��ʾ�ط���ʱ�� OFF��ʾ�ط�����(�ط����������յ��ظ�)
		
                
                
                BYTE m_Waitflag_rf003;//����͸���������Ҫ�ȴ���RF003��ʱ���־λ
		WORD m_WaitCt_rf003;//����͸���������Ҫ�ȴ�RF003��ʱ��
		
		unsigned char VI_flag1;
		BYTE  m_rfSendBuf[30];//����rf��Ҫ�ش��ı���
		WORD  m_rfSendLen;//rf��Ҫ�ش��ı��ĳ���
		WORD  mrecorder_long;
		BYTE m_reNum;//�ش�����
        unsigned long FLbAddr_offset;
		BYTE fault_type;//����rf��Ҫ�ش��ı���
		unsigned int BKrelative_timel[3];
      	unsigned char group11;
     // unsigned char VI_flag =0;
      	unsigned int Max_Len;
	 	unsigned int PhaseFault_Len;
     	INT8U phase_flag;
		BYTE m_EarthFault_flag;//�жϽӵع��ϵı�־λ������ȷ���Ƿ��ϴ�¼����־λ
		//RECORDER_LUBO *precorder;
	public:
		CPrtcRF();
		BOOL Init(WORD wTaskID);
	        void Run(void);
		BOOL DoReceive();
		DWORD SearchOneFrame(BYTE *Buf, WORD Len);
		BOOL RecFrame55(VFrame55 * pReceiveFrame);
		
                void DoCommSendIdle();
				void EarthFaultLocation();
				void SendRfRetry();
				void SaveLBFlash(unsigned char *pLbBuff,unsigned char flag,unsigned int Val,BYTE Data_length1);
				void Recorder_time(unsigned int *precorder_time);
				void Init_RECpara();
                                void cfg_lubo_start();
                                void Set_Lubo_flag();
                                void YC_AD(BYTE Group,BYTE Phase,RF003FTDA *PFaultCon,WORD WYcIndex,BYTE *ByFtTypes);
                                void Mult_Record_Process(BYTE group,RF003FTDA *PFaultCon,BYTE bLuboFt,unsigned int *pabsolute_time);
                                BOOL RecZeroSequence(unsigned int *pBK_relative,unsigned char *Pgroup);
				void Volatge_Coefficient(unsigned int LINE);
                BOOL DoTimeOut(WORD wTimerID);
				DWORD max(DWORD x,DWORD y,DWORD z);
                BOOL SendZsqPara(void);
                BOOL SendYKLBToRf003(BYTE flag);
                BOOL RecRf001Fault_A0(VFrame55 *pReceiveFrame);
                BOOL RecRf001Fault_3P4O(VFrame55 *pReceiveFrame);
                BOOL RecRf001Fault(VFrame55 *pReceiveFrame);
                BOOL RecRf001Para(VFrame55 *pReceiveFrame);
	        	BOOL RecRf003Para(VFrame55 *pReceiveFrame);
                BOOL RecRf003Fault(VFrame55 *pReceiveFrame);
				BOOL Send_Wirter_Ack(BYTE Type);
                BOOL SendYkToRf003(unsigned char chLinePhase,unsigned int Val,CProtocol *pDaSrc=null);
                BOOL SendRCmdToRf001(unsigned char chLinePhase,unsigned int Val,CProtocol *pDaSrc=null);
				BOOL SendRCmdToRf003(CProtocol *pDaSrc=null);
                BOOL SendWCmdToRf001(unsigned char chLinePhase,unsigned int cmd,unsigned char *pdata,CProtocol *pDaSrc=null);
				BOOL SendWCmdToRf003(unsigned int cmd,unsigned char *pdata,CProtocol *pDaSrc=null);
				BOOL SendRCmdToFI(unsigned char chLinePhase,unsigned int Val);//,unsigned char *pdata);,CProtocol *pDaSrc=null);      
				BOOL SendWCmdToFI(unsigned char chLinePhase,unsigned int Val);
                                WORD m_wTxIPBit;//��ǰ���ͱ��ĵ�IP���㲥ң�غ�д����ʱ���õ���Ŀǰ֧��9ָֻʾ����bit0=1a�࣬bit1=1b�࣬bit2=1c�࣬bit3=2A,bit4=2B....
			//	BOOL RecRepeateryc(VFrame55 *pReceiveFrame);
				BOOL SendRCToRf003(unsigned char chLinePhase,unsigned int Val,CProtocol *pDaSrc=null);
				BOOL SendRf003Recorder(unsigned int Num,unsigned char VI_flag);
				BOOL AddrBlock_Erase(unsigned char chLinePhase);
				void SaveLBZS(unsigned int *pBK_relative,unsigned char group,unsigned int *pMAX_Len,INT8U *pphase_flag,unsigned char VI_flag);//DWORD A_Len,DWORD B_Len,DWORD C_Len,unsigned char LINE,
				INT32S DataDeal(INT32S TempRe,INT32S TempIm);
				INT8U DftCalc(INT32S buf,DFT_DATA_EXT * DftDate,INT32S* TempRe,INT32S* TempIm);	
				FP32 max_s(FP32 x,FP32 y,FP32 z);
				BOOL Phase_Detection(int line);
				void Reset_Short(WORD YxIndex,WORD YxIndex1);
};








#endif



