
#ifndef PrtcGPR_H
#define PrtcGPR_H

#include "..\PUBLIC\Pcolsec.h"



/*typedef struct 
{
    BYTE Start; //$GPRMC�Ƽ���λ��Ϣ���ݸ�ʽ
    BYTE byContent[MAX_FRAME_LEN];
}VFrameGPR;//�Ƽ���С��λ��Ϣ*/

typedef union 
{
	VFrameGPR FrameGPR;//�Զ���Э�� ���Ľṹ��101��ͬ��ֻ�Ǳ���ͷΪ0x69 for ������д
} VIecGprFrame; 
//VIecGprFrame  GprFrame;
//VFrameGPR FrameGPR;

class CPrtcGPS : public CProtocol//CPSecondary
{
	public:
		VIecGprFrame *pReceiveFrame; 	//����֡ͷָ��
  		//GPR_Frame GPR;
		BYTE Start_ptr;//ÿһ�ε���ʼ���
		BYTE End_ptr;//ÿһ�ε�������
		BYTE para_Content_Length[15];//��λ�ı��Ĺ�17��,ÿһ�εĳ���
		//VIecRfFrame *pSendFrame;  		//����֡ͷָ��
		BYTE m_Len;//����ODU��GPRS�ı�־ 0x55:��GPRS 0X66:��GPRS
	public:
		CPrtcGPS();
		BOOL Init(WORD wTaskID);
	    void Run(void);
		BOOL DoReceive();
		DWORD SearchOneFrame(BYTE *Buf, WORD Len);
		//void GPS_Config(BYTE Type);
		BOOL RecFrameGPR(VFrameGPR * pReceiveFrame);
        //void DoCommSendIdle();
        //BOOL DoTimeOut(WORD wTimerID);
		void SendRCmdToGPR(WORD GprPara,BYTE ParaName,CProtocol *pDaSrc=null);
          
};








#endif



