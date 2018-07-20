
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
		
		WORD m_SendStatusBit; //状态汇报标志位
		BYTE m_SendStatusFlag;  //状态汇报标志位
		WORD m_wSendYcNum;		//已经发送的遥测个数，控制多帧传送用
		WORD m_wSendYxNum;		//已经发送的遥信个数，控制多帧传送用
		WORD m_wSendDYxNum;		//已经发送的遥信个数，控制多帧传送用
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


