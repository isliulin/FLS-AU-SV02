
#ifndef PrtcGPR_H
#define PrtcGPR_H

#include "..\PUBLIC\Pcolsec.h"



/*typedef struct 
{
    BYTE Start; //$GPRMC推荐定位信息数据格式
    BYTE byContent[MAX_FRAME_LEN];
}VFrameGPR;//推荐最小定位信息*/

typedef union 
{
	VFrameGPR FrameGPR;//自定义协议 报文结构与101相同，只是报文头为0x69 for 参数读写
} VIecGprFrame; 
//VIecGprFrame  GprFrame;
//VFrameGPR FrameGPR;

class CPrtcGPS : public CProtocol//CPSecondary
{
	public:
		VIecGprFrame *pReceiveFrame; 	//接收帧头指针
  		//GPR_Frame GPR;
		BYTE Start_ptr;//每一段的起始序号
		BYTE End_ptr;//每一段的最后序号
		BYTE para_Content_Length[15];//定位的报文共17段,每一段的长度
		//VIecRfFrame *pSendFrame;  		//发送帧头指针
		BYTE m_Len;//发给ODU打开GPRS的标志 0x55:关GPRS 0X66:开GPRS
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



