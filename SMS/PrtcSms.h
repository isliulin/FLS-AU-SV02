
#ifndef PrtcSMS_H
#define PrtcSMS_H
#include "..\PUBLIC\Pcolsec.h"

 
class CPrtcSms : public CPSecondary
{
    public:
        WORD m_SmsSoeBit;
        WORD m_DSmsSoeBit;
        BYTE m_infoaddlen;
        BYTE m_linkaddrlen;
		
		WORD m_SendStatusBit; //״̬�㱨��־λ
		BYTE m_SendStatusFlag;  //״̬�㱨��־λ
		WORD m_wSendYcNum;		//�Ѿ����͵�ң����������ƶ�֡������
		WORD m_wSendYxNum;		//�Ѿ����͵�ң�Ÿ��������ƶ�֡������
		WORD m_wSendDYxNum;		//�Ѿ����͵�ң�Ÿ��������ƶ�֡������
    public:
        CPrtcSms();
        BOOL Init(WORD wTaskID);
		BOOL DoTimeOut(WORD wTimerID);
        BOOL RecFrameAA(VFrameAA * pReceiveFrame);
       
        void DoSmsSendIdle();
        BOOL SendSms(unsigned char command);
        BOOL SendSmsFrameHead(BYTE Command);
        BOOL TAGMesType(BYTE Type);
        BOOL TAGContent(BYTE *ptr,BYTE num,BYTE type);
        BOOL TAGPhoneNum(BYTE *PHONE,BYTE len);
        BOOL SendSmsFrame(void);
        BOOL SendSmsToXinhao(void);
        BOOL SendRCmdToIHD(unsigned char Cmd,unsigned int Val,CProtocol *pDaSrc);
        BOOL SendWCmdToIHD(unsigned int Val,unsigned char Cmd,BYTE *pDa,CProtocol *pDaSrc);
        BOOL RecIHDPara(VFrameAA *pReceiveFrame);
        //BOOL RecIHDParaMes(VFrameAA *pReceiveFrame);
        BOOL RecIHDWConf(VFrameAA *pReceiveFrame);
        BOOL SendSmsWPConf(BYTE* dataPtr);
		BOOL Send_GPRS_Power(BYTE TYPEL,BYTE TYPEH);
		//BOOL TagMesHead(BYTE Command);
};

#endif


