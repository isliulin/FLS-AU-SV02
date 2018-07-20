
#ifndef PrtcOdu_H
#define PrtcOdu_H

#include "..\PUBLIC\Pcolsec.h"

typedef union 
{

    VFrame67 Frame67;
    //VFrameFF Frameff;
    VFrame69 Frame69;//�Զ���Э�� ���Ľṹ��101��ͬ��ֻ�Ǳ���ͷΪ0x69 for ������д
} VIecOduFrame; 

class CPrtcOdu : public CProtocol
{
    public:
        BYTE m_OduAddr;//��ǰODU�ĵ�ַ(�ն��ڲ�ʹ�ã���ʵ��ODU��ַ�޹�)���϶���pOduʹ�õĴ���������ODUΪ1-6· ODU��ַ=1����pOdu1ʹ�õĴ���������ODUΪ7-12· ODU��ַ=2��
		BYTE m_ReadPaTp;//��ǰ���Ĳ������� �����������ԭ���Ƕ��ɹ�ʱ���ظ���̨�ı��Ĳ�ͬ
        VIecOduFrame *pReceiveFrame;    //����֡ͷָ��
        VIecOduFrame *pSendFrame;       //����֡ͷָ��
        
        BYTE m_ODUCall2Flag;//ODU�ٶ�������
        BYTE  m_ODUCallFlag;//ODU����
        BYTE m_ODUCall2Bit;//����ODU�����ݲ���ͬһ����ʱ�����ô˱��������ָø��ĸ�ODU������
        BYTE m_ODUCallBit;
		BYTE  m_oduSendBuf[25];//����ODU��Ҫ�ش��ı���
		WORD  m_oduSendLen;//ODU��Ҫ�ش��ı��ĳ���
		BYTE m_WrPaWaitflag;//ON��ʾ�����ط���ʱ TRUE��ʾ�ط���ʱ�� OFF��ʾ�ط�����(�ط����������յ�ODU�ظ�)
		WORD m_WrPaWaitCt;//дODU����ʱ�ط���ʱ
		WORD m_ReTxNum;//�ش�����
		BYTE m_open_gprs;//����ODU��GPRS�ı�־ 0x55:��GPRS 0X66:��GPRS
		BYTE m_RcvCloseZdFlag;//ON:�յ�ODU���͵Ĺر��ն�ָ��
    public:
        CPrtcOdu();
        BOOL Init(WORD wTaskID);
        void Run(void);
        BOOL DoReceive();
        DWORD SearchOneFrame(BYTE *Buf, WORD Len);
        void DoCommSendIdle();
		BOOL DoTimeOut(WORD wTimerID);
		
        BOOL RecFrame67(VFrame67 *pframe67);
        BOOL RecODUFault(BYTE bOduAddr);
        BOOL RecODUFault_yaoce(BYTE bOduAddr); 
        BOOL RecODUPower_Supply();
        BOOL RecODUPara();
		void RecOduWPa(void);
		
        void SendODUFrame(void);
        void SendODUCallFrame(void);
        void SendOWrPaSuc();
        void SendRCmdToOdu(WORD OduPara,BYTE ParaName,CProtocol *pDaSrc=null);
        void SendWCmdToOdu(WORD OduPara,BYTE ParaName, BYTE *pDa=null,CProtocol *pDaSrc=null);
        BOOL SendFrameHead_ODU(BYTE Style, BYTE Reason,BYTE OduPara1, BYTE OduPara2, BYTE OduPara);
        BOOL SendFrameTail_ODU(BYTE Num, BYTE OduPara);
		void SendOduRetry(void);
		
        void RstOdu();
};








#endif


