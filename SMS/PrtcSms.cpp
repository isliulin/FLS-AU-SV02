/*------------------------------------------------------------------------
 Module:            PrtcSms.cpp
 Author:            linxueqin
 Project:           ��Լƽ̨
 State:             V1.0
 Creation Date:     2015-3-16
 Description:       ���Ͷ���--��101Э�鹲��GPRSͨ������˴�����101Э��Ļ���
------------------------------------------------------------------------*/


#ifndef _PRTC_SMS

#define _PRTC_SMS
  #include "PrtcSms.h"
#include "..\Main.h"
  /***************************************************************
      Function��CPrtcSms
          ���캯�����ݿ�
      ��������
  
      ���أ���
  ***************************************************************/
   CPrtcSms::CPrtcSms() : CPSecondary()
  {
  
  }
  
  //ͨ�����Ϳ��д���
void CPrtcSms::DoSmsSendIdle(void)
{
        //���Ͷ���
        if(g_EnSmsSend == ON)
        {
            if(g_SmsReSendWaitCount == 0)  //���ط��Ķ���
            {
                if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))  //���ŷ�������
                {
                    //if(SearchCos(m_SmsSoeBit,0)) 
                    if(g_SmsReSendTime > 3)
                    {
                        g_SmsReSendTime = 0;
                        g_gSmsHeadPtrBk = g_gSmsHeadPt;
                    }                  
                    if(g_gSmsHeadPt != g_gSmsHeadPtrBk)  //����ң�������ط�
                    {
                        SendSms(1);
                        return;
                    }
                    if(SearchCos(m_SmsSoeBit,0))
                    {
                        SendSms(1);
                        return;
                    }
//                    else if(SearchCos(m_DSmsSoeBit,0))
//                    {
//                        SendSms(1);
//                    }
                }
            }
            if(g_EnSendToXinhao == ON)//���ź�Դ������
            {
                g_EnSendToXinhao = OFF;
                OpenTimer(TM_WAITXINHAO);
                if(SendSmsToXinhao())
                    return;
            }
            if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]) && (g_SmsSendStatus == ON))
            {
                g_SmsSendStatus = OFF;
                if(SendSms(0))
                    return;
            }
        }


}
  
  /***************************************************************
      Function��Init
          ��Լ��ʼ��
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
  ***************************************************************/
  BOOL CPrtcSms::Init(WORD uartId)
  {
      BOOL rc;
      rc = CPSecondary::Init(1);
      if (!rc)
      {
        return FALSE;
      }
        m_infoaddlen = g_ucPara101[IECP_INFO_NUM];
        m_linkaddrlen= g_ucPara101[IECP_LINKADDR_NUM];
       
  
      return TRUE;
  }
  /***************************************************************
        Function��OnTimeOut
            ��ʱ����
        ������TimerID
            TimerID ��ʱ��ID
        ���أ���
    ***************************************************************/
    BOOL CPrtcSms::DoTimeOut(WORD wTimerID)
    {
      if((g_SmsReSendWaitCount > 0) && (m_uartId == g_CmIdGPRS))
       {
           g_SmsReSendWaitCount--;
           if(g_SmsReSendWaitCount == 0)
           {
               //g_EnSmsSend = ON;
               g_SmsReSendTime = 0;
           }
       }
    if((g_gRunPara[RP_DEVICE_TYPE] == 0 )&& (g_SendStatus & m_SendStatusBit))
    {
        g_SendStatus &= ~m_SendStatusBit;
        m_SendStatusFlag = YES;
        m_wSendYxNum = 0;
        m_wSendDYxNum = 0;
        if(m_uartId == g_CmIdGPRS)
        {
            if(g_gRunPara[RP_DEVICE_TYPE] == 0)
                g_SmsSendStatus = ON;
            g_yxChangeflag |= BITF;         
        }
    }
     return 1;
    }


  
  
  /***************************************************************
      Function��RecFrameAA
          �����յ��Ķ��ű���
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
  ***************************************************************/
  BOOL CPrtcSms::RecFrameAA(VFrameAA * pReceiveFrame)
  {
      BYTE tempdata[256];
      //BYTE phoneNum[16];
      BYTE *ptr;
     // BYTE temp;
      
      switch(pReceiveFrame->Type)
      {
      case 0x07:      //��GPRSʱ��
          if(pReceiveFrame->byContent[0] == 0x1)
          {
              //WORD MSecond;
              //MSecond = MAKEWORD(pData[0], pData[1]);
              unsigned int unHYM[7];
              unHYM[0] = ((pReceiveFrame->byContent[1]) & 0x7F);//YEAR
              unHYM[1] = (pReceiveFrame->byContent[2]) & 0x0F;//MONTH
              unHYM[2] = (pReceiveFrame->byContent[3]) & 0x1F;//DAY
              unHYM[3] =g_sRtcManager.m_gRealTimer[RTC_WEEK];//SNTP�޵ڼ��ܵ���Ϣ����RTCоƬ�ĵڼ��ܸ���
              unHYM[4] = pReceiveFrame->byContent[4];  //HOUR
              unHYM[5] = pReceiveFrame->byContent[5];//MINUTE
              unHYM[6] = pReceiveFrame->byContent[6];//SEC
              WriteRealTime(unHYM);  //�޸�ʱ��
              //ReadRealTime(0);//��ʵʱоƬ�е�ʱ����µ�
              BYTE bBuf[2] = {0xFC,0XFC};
              MyPrintfD(bBuf,2);
          }
          else
          {
              BYTE bBuf[2] = {0xFB,0XFB};
              MyPrintfD(bBuf,2);
          }            
          break;
      case 0x16:      //��GPRS����
          RecIHDPara(pReceiveFrame);
      case 0x17:      //���Żظ�ȷ��֡
          if((pReceiveFrame->byContent[0] == 0x81) && (pReceiveFrame->byContent[1] == 0x89))
          {
              if((pReceiveFrame->byContent[3] == 0x01) && (g_SmsReSendWaitCount > 0))  //�յ����Żظ�ȷ��֡
              {
                  g_SmsReSendWaitCount = 10;
                 // g_gSmsStartPtr = g_gSmsHeadPt;
                 g_gSmsHeadPtrBk = g_gSmsHeadPt;
              }
          }
          break;
      case 0x18:
          RecIHDWConf(pReceiveFrame);
      case 0x13:      //�յ���������
          for(BYTE i = 0; i < (pReceiveFrame->FrameLen[1] - 6); i++)
          {
              if((pReceiveFrame->byContent[i] == 0x81) && (pReceiveFrame->byContent[i+1] == 0x80))
              {
                  ptr = &(pReceiveFrame->byContent[i+3]);
                  if(*ptr > 15)
                      return false;
                  for(BYTE j = 0; j < pReceiveFrame->byContent[i+3]; j++)
                      g_phoneNum[j] = *++ptr;
                  g_phoneNum[pReceiveFrame->byContent[i+3]] = '\0';
              }
              if((pReceiveFrame->byContent[i] == 0x81) && (pReceiveFrame->byContent[i+1] == 0x82))
              {
                  ptr = &(pReceiveFrame->byContent[i+4]);
                  BYTE temp = pReceiveFrame->byContent[i+3];
                  for(BYTE j = 0; j < temp; j++)
                  {
                      tempdata[j] = *ptr++;
                  }
                  if((tempdata[0] == 0x30) && (tempdata[1] == 0x31) && (tempdata[2] == 0x36))
                  {
                      if(tempdata[5] == 0x31)
                      {
                          if(g_sTimer[TM_WAITXINHAO].m_TmCount > 0)
                          {
                              g_sTimer[TM_WAITXINHAO].m_TmCount = 1;
                          }
                      }
                      else if(tempdata[5] == 0x30)  //�յ��ź�Դ���͵ķ�ȷ��
                      {
                          //�����׼ʵʱģʽ�£��˴���Ҫ�رյ�Դ
                          g_sTimer[TM_WAITXINHAO].m_TmCount = 0;
                          if(g_sTimer[TM_GPRSPWOERDOWN].m_TmCount > 0)
                          {
                              g_sTimer[TM_GPRSPWOERDOWN].m_TmCount = 1;
                          }
                          for(BYTE j = 0; j < g_unGroundBuffTail; j++)    // �ӵر�����0
                          {
                              g_gRmtLock[g_gGroundBuff[j]] = 0;
                          }
                          g_unGroundBuffTail = 0;
                      }
                  }
                  else if(tempdata[0] == 'g' && tempdata[1] == 'r' && tempdata[2] == 'e' && tempdata[3] == 'p')  //���Ŷ�������ʾ��
                  {
                      if((g_gSmsPhone[PHONE_ENCTL] & BIT3) && (g_gSmsPhone[PHONE_ENCTL] & BIT4))   //����4�ͺ���5ͬʱ����
                      {
                          g_gSmsPhone[PHONE4 + 11] = '\0';
                          g_gSmsPhone[PHONE5 + 11] = '\0';
                          if((strcmp((char *)&g_gSmsPhone[PHONE4],(char *)g_phoneNum) == 0) 
                          || (strcmp((char *)&g_gSmsPhone[PHONE5],(char *)g_phoneNum) == 0));
                          else
                              return false;
                      }
                      else if(g_gSmsPhone[PHONE_ENCTL] & BIT3)
                      {
                          g_gSmsPhone[PHONE4 + 11] = '\0';
                          if(strcmp((char *)&g_gSmsPhone[PHONE4],(char *)g_phoneNum) != 0)
                              return false;                             
                      }
                      else if(g_gSmsPhone[PHONE_ENCTL] & BIT4)
                      {
                          g_gSmsPhone[PHONE5 + 11] = '\0';
                          if(strcmp((char *)&g_gSmsPhone[PHONE5],(char *)g_phoneNum) != 0)
                              return false;                         
                      }
                      if(tempdata[5] == 'i' && tempdata[6] == 'p')   
                      {
                          g_MesReadFlag = ON;
                          SendRCmdToIHD(0,0,null);
                      }
                  }
                  else if(tempdata[0] == 's' && tempdata[1] == 'e' && tempdata[2] == 't' 
                  && tempdata[4] == 'p' && tempdata[5] == 'a' && tempdata[6] == 'r' && tempdata[7] == 'a') //����д������ʾ��
                      SendSmsWPConf(&tempdata[9]);
              }
          }  
          break;
      default:
          break;
      }
      return true;
  }
  /***************************************************************
      Function��SendRCmdToIHD
          ��ӳ�ͨģ�鷢����������
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2015��8��20 BY zx
  ***************************************************************/  
  BOOL CPrtcSms::SendRCmdToIHD(unsigned char Cmd,unsigned int Val,CProtocol *pDaSrc)
  {
      //BYTE tempbuff[32];
      //BYTE  chDaSrcCm;
      if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
      //chDaSrcCm = pDaSrc->m_uartId;
      //if(chDaSrcCm == g_CmIdGPRS)
      //{
      //    g_IHDCmdRBit |= (DWORD)BIT[Cmd];
      //}
      //else if(chDaSrcCm == g_CmIdDBG)
      //{
      //    g_IHDCmdRBit |= ((DWORD)BIT[Cmd] << 16);
      //}
      switch(Cmd)
      {
      case 0:       //��IP��ַ�Ͷ˿�
          SendSmsFrameHead(READ_SPE_CONFIG);
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x19;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          //TAGContent(tempbuff,4,INHAND_CHAXUN);
          break;
      case 1://�����Ź��ܿ���״̬
          SendSmsFrameHead(READ_SPE_CONFIG);
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x87;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          //TAGContent(tempbuff,4,INHAND_CHAXUN);
          break;
      case 2:  //APN
      case 3:  //�û���
      case 4:  //����
          SendSmsFrameHead(READ_SPE_CONFIG);
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = Cmd;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          //TAGContent(tempbuff,4,INHAND_CHAXUN);
          break;
      case 15://
          SendSmsFrameHead(READ_STATUS);// TagMesHead(READ_STATUS);
          //TAGContent(tempbuff,0,INHAND_CHAXUN);
          break;
      default:
          return false;
      }
      SendSmsFrame();
      //OpenTimer(TM_GPRS_RW);
      return true;
  }
  
  
   /***************************************************************
      Function��SendWCmdToIHD
          ��ӳ�ͨģ�鷢д��������
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2015��8��20 BY zx
  ***************************************************************/  
  BOOL CPrtcSms::SendWCmdToIHD(unsigned int Val,unsigned char Cmd,BYTE *pDa,CProtocol *pDaSrc)
  {
      //BYTE tempbuff[32];
      //BYTE  chDaSrcCm;
      BYTE dataLen;
      BYTE j;
      //chDaSrcCm = pDaSrc->m_uartId;
      if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
      SendSmsFrameHead(WRITE_SPE_CONFIG);//TagMesHead(WRITE_SPE_CONFIG);
      if(Val)
          dataLen = Val;
      else
          dataLen = pDa[0];
      if(dataLen <= 0)
          return false;
//        if(chDaSrcCm == g_CmIdGPRS)
//        {
//            g_IHDCmdWBit |= (DWORD)BIT[Cmd];
//        }
//        else if(chDaSrcCm == g_CmIdDBG)
//        {
//            g_IHDCmdWBit |= ((DWORD)BIT[Cmd] << 16);
//        }
      switch(Cmd)
      {
      case 0:       //дIP��ַ�Ͷ˿�
          
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x19;
		  OpenTimer(TM_GPRS_RW);//��ʱ2S�����Ƿ����ߵ͹��ĵı���
          if (dataLen > 6)
              return false;
          break;
      case 1://��/�ض��Ź���
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x87;
   
          break;
      case 2:  //APN
      case 3:  //�û���
      case 4:  //����
          if(dataLen > 20)
              return false;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = Cmd;
          break;          
      default:
          return false;
      }
      g_SmsSendBuf[g_SmsWritePtr++] = 0;
      g_SmsSendBuf[g_SmsWritePtr++] = dataLen;      
      for(char i = 0; i < dataLen; i++)
      {
          g_SmsSendBuf[g_SmsWritePtr++] = pDa[1 + i];
      }
	  if (Cmd == 0)
	  {
	  	for ( j =0;j<4;j++)
	  	{
	  		if (pDa[1 + j] != 0)
	  		{
	  			g_GprsPowerConsum=ON;
				break;
	  		}
	  	}
		if (j==4)
			g_GprsPowerConsum=OFF;
	  }
      //TAGContent(tempbuff,dataLen + 4,INHAND_CHAXUN);
      SendSmsFrame();
      
	  //OpenTimer(TM_GPRS_RW);//��ʱ2S�����Ƿ����ߵ͹��ĵı���
      return true;
  }

    /***************************************************************
      Function��Send_GPRS_Power
          ����ӳ�ͨ�ߵ͹���
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2018��4��20 BY zxx
  ***************************************************************/  
BOOL CPrtcSms::Send_GPRS_Power(BYTE TYPEL,BYTE TYPEH)
  {
	  //BYTE tempbuff[32];
	  //BYTE  chDaSrcCm;
	  //BYTE dataLen;
	  //chDaSrcCm = pDaSrc->m_uartId;
	  SendSmsFrameHead(WRITE_SPE_CONFIG);//TagMesHead(WRITE_SPE_CONFIG);
		  
		  //g_SmsSendBuf[g_SmsWritePtr++] = 6;
		  g_SmsSendBuf[g_SmsWritePtr++] = 0x80;
		  g_SmsSendBuf[g_SmsWritePtr++] = 0x16;
		  g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
		  g_SmsSendBuf[g_SmsWritePtr++] = 0x02;
		  g_SmsSendBuf[g_SmsWritePtr++] = TYPEL;
		  g_SmsSendBuf[g_SmsWritePtr++] = TYPEH;
		  
	  
	 
	  SendSmsFrame();
	  
	  return true;
  }


     /***************************************************************
      Function��RecIHDPara
          ����ӳ�ͨ���͵����ò����_�J
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2015��8��20 BY zx
  ***************************************************************/  
  BOOL CPrtcSms::RecIHDWConf(VFrameAA *pReceiveFrame)
  {
      WORD wTag = MAKEWORD(pReceiveFrame->byContent[1],pReceiveFrame->byContent[0]);//��λ��ǰ
     // BYTE secName = 0;
      switch(wTag)
      {
      case 0x0002:   //��APN
        //  secName = 2;
          break;
      case 0x0003:   //���û���
         // secName = 3;
          break;
      case 0x0004:   //������
          //secName = 4;
          break;
      case 0x0019:   //��IP���˿�
         // secName = 0;
          break;
      case 0x800D:
          break;
      case 0x8187:
         // secName = 1;        
          break;
      default:
          return false;
      }
      if(m_RWPaSrcObj !=null)
         m_RWPaSrcObj->SendWrPaSuc(); 
      
      return true;     
  }
  
  
    /***************************************************************
      Function��RecIHDPara
          ����ӳ�ͨ���͵����ò���
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2015��8��20 BY zx
  ***************************************************************/  
  BOOL CPrtcSms::RecIHDPara(VFrameAA *pReceiveFrame)
  {
      BYTE secName;
      WORD wTag = MAKEWORD(pReceiveFrame->byContent[1],pReceiveFrame->byContent[0]);//��λ��ǰ

      switch(wTag)
      {
      case 0x0002:   //��APN
          secName = 2;
          break;
      case 0x0003:   //���û���
          secName = 3;
          break;
      case 0x0004:   //������
          secName = 4;
          break;
      case 0x0019:   //��IP���˿�
          secName = 0;
          break;
      case 0x800D:
          break;
      case 0x8187:
          secName = 1;        
          break;
      default:
          return false;
      }
    if(m_RWPaSrcObj != null)
          m_RWPaSrcObj->SendReadPa(11,secName);
      return true;
  }
  
      /***************************************************************
      Function��RecIHDParaMes
          ����ӳ�ͨ���͵����ò���
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2015��8��20 BY zx
  ***************************************************************/  
  /*BOOL CPrtcSms::RecIHDParaMes(VFrameAA *pReceiveFrame)
  {
      char ch[32];
      BYTE tempData[128];
      BYTE i = 0;
      WORD port;

      if(pReceiveFrame->byContent[0] == 0)
      {
          switch(pReceiveFrame->byContent[1])
          {
          case 0x19:
              SendSmsFrameHead(TXT_SEND);
              if(g_gSmsPhone[PHONE_ENCTL] & BIT3)
                  TAGPhoneNum(g_phoneNum,11);
              else if(g_gSmsPhone[PHONE_ENCTL] & BIT4)
                  TAGPhoneNum(g_phoneNum,11);
              else 
                  return false;
              TAGMesType(ENG);
              sprintf((char *)ch,"set para\n");
              for(BYTE j = 0; j < strlen(ch); j++)
                  tempData[i++] = ch[j];
              
              g_SmsCmdCode =g_sRtcManager.m_gRealTimer[RTC_MICROSEC]*1000 + g_sRtcManager.m_gRealTimer[RTC_MICROSEC];  //����У����
              sprintf((char *)ch,"code:%04x\n",g_SmsCmdCode);
              for(BYTE j = 0; j < strlen(ch); j++)
                  tempData[i++] = ch[j];
              
              port = MAKEWORD(pReceiveFrame->byContent[9],pReceiveFrame->byContent[8]);
              sprintf((char *)ch,"ip=%d.%d.%d.%d:%d#",pReceiveFrame->byContent[4],pReceiveFrame->byContent[5],pReceiveFrame->byContent[6],pReceiveFrame->byContent[7],port);
              for(BYTE j = 0; j < strlen(ch); j++)
                  tempData[i++] = ch[j];              
              TAGContent(tempData,i,INHAND_DUANXIN);
              SendSmsFrame();
              g_MesReadFlag = TRIP;
              break;
          default:
              break;
          }
      }
      OpenTimer(TM_SMS_RW);
      return true;
  }*/
  /***************************************************************
      Function��SendSmsWPConf
      ���ܣ��Զ����޸�����Ļظ�����
      ���������
      ���أ�TRUE �ɹ���FALSE ʧ��
      ���ڣ�2015��11��26 BY zx
  ***************************************************************/
  BOOL CPrtcSms::SendSmsWPConf(BYTE* dataPtr)
  {
      char ch[32];
      BYTE tempData[128];
      BYTE i = 0;
      BYTE code[5];
      BYTE ipTemp[4];
      //BYTE portTemp[4];
      int IpPort[7];
      BYTE CmdData[7];
      unsigned int portInt;
      BYTE err = 0;
      BYTE temp;
      //BYTE temp1;
      //BYTE temp2;
      
      IpPort[0] = 6;
      code[4] = '\0';
//      for(BYTE itTmp = 0; itTmp < 64; itTmp++)
//            tempData[itTmp] = dataPtr[itTmp];
      if(g_MesReadFlag == OFF)
      {
          SendSmsFrameHead(TXT_SEND);
          TAGPhoneNum(g_phoneNum,11);
          TAGMesType(ENG);
          sprintf((char *)ch,"Time Out!\n");
          for(BYTE j = 0; j < strlen(ch); j++)
              tempData[i++] = ch[j];
          TAGContent(tempData,i,INHAND_DUANXIN);
          SendSmsFrame();
          g_MesReadFlag = 0;
          return true;
      }
      if(dataPtr[0] == 'c' && dataPtr[1] == 'o' && dataPtr[2] == 'd' && dataPtr[3] == 'e')
      {
          for(BYTE j = 0; j < 4; j++)
              code[j] = dataPtr[5+j];
          sprintf((char *)ch,"%04x",g_SmsCmdCode); 
          if(strcmp((char *)code,(char *)ch) == 0)  //У����Ա�
          {
              if(dataPtr[10] == 'i' && dataPtr[11] == 'p' && dataPtr[12] == '=')
              {
                  BYTE j = 0;
                  BYTE k = 1;
                  for(temp = 0; dataPtr[13+temp] != '#'; temp++)
                  {
                      if((dataPtr[13+temp] == '.') || (dataPtr[13+temp] == ':'))
                      {
                          if(j == 1)
                              IpPort[k++] =  ipTemp[0]; 
                          else if(j == 2)
                              IpPort[k++] =  ipTemp[0]*10 + ipTemp[1];
                          else if(j == 3)
                              IpPort[k++] =  ipTemp[0]*100 + ipTemp[1]*10 + ipTemp[2];
                          else
                          {
                              err = YES;
                              break;
                          }
                          if(k > 5)
                          {
                              err = YES;
                              break;                          
                          }
                          j = 0;
                      }
                      else
                      {
                          ipTemp[j++] = dataPtr[13+temp] - 0x30;
                          if(j > 4)
                          {
                              err = YES;
                              break;
                          }
                      }
                  }
                  if(k != 5)
                      err = YES;
                  if(err == 0)
                  {
                      if(j == 1) 
                          portInt = ipTemp[0];
                      else if(j == 2)
                          portInt = ipTemp[0]*10 + ipTemp[1];
                      else if(j == 3)
                          portInt = ipTemp[0]*100 + ipTemp[1]*10 + ipTemp[2];
                      else if(j == 4)
                          portInt = ipTemp[0]*1000 + ipTemp[1]*100 + ipTemp[2]*10 + ipTemp[3];
                      else
                          err = YES;
                  }
                  if(err)
                  {
                      SendSmsFrameHead(TXT_SEND);
                      TAGPhoneNum(g_phoneNum,11);
                      TAGMesType(ENG);                    
                      sprintf((char *)ch,"Data Error!");
                      for(BYTE j = 0; j < strlen(ch); j++)
                          tempData[i++] = ch[j];
                      TAGContent(tempData,i,INHAND_DUANXIN);
                      SendSmsFrame();
                      return true;
                  }
                  IpPort[5] = HIBYTE(portInt);
                  IpPort[6] = LOBYTE(portInt);
                  
                  for(BYTE x = 0; x < 7; x++)
                      CmdData[x] = (BYTE)IpPort[x];
                  g_SmsCmdCode = 0;
                  SendWCmdToIHD(CmdData[0],0,CmdData,null);
                  return true;
              }
          }
          else
          {
              SendSmsFrameHead(TXT_SEND);
              TAGPhoneNum(g_phoneNum,11);
              TAGMesType(ENG);  
              sprintf((char *)ch,"Code Error!");
              for(BYTE j = 0; j < strlen(ch); j++)
                  tempData[i++] = ch[j];
              TAGContent(tempData,i,INHAND_DUANXIN);
              SendSmsFrame();
              return true;
          }              
      }
      SendSmsFrameHead(TXT_SEND);
      TAGPhoneNum(g_phoneNum,11);
      TAGMesType(ENG);
      sprintf((char *)ch,"Data Error!");
      for(BYTE j = 0; j < strlen(ch); j++)
          tempData[i++] = ch[j];
      TAGContent(tempData,i,INHAND_DUANXIN);
      SendSmsFrame();     
      return true;
  }
  BOOL CPrtcSms::SendSmsToXinhao(void)
  {
      char ch[32];
      BYTE temp = 0x82 + (g_gRunPara[RP_COMM_ADDR] & 0xff) + (g_gRunPara[RP_COMM_ADDR] >> 8);
      sprintf((char *)ch,"016130%03d%03d%03d022",(g_gRunPara[RP_COMM_ADDR] & 0xFF),((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),temp);
      
      for(BYTE i = 0; i < strlen(ch); i++)
      {
          g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
      }        
//      BYTE i = 0;
//      tempbuff[i++] = 0x10;
//      tempbuff[i++] = 0x82;
//      tempbuff[i++] = g_gRunPara[RP_COMM_ADDR] & 0xff;
//      tempbuff[i++] = g_gRunPara[RP_COMM_ADDR] >> 8;
//      tempbuff[i++] = (BYTE)ChkSum(&tempbuff[1],3);
//      tempbuff[i++] = 0x16; 
      
      if((g_gSmsPhone[PHONE_ENCTL] & BIT2) == 0)
          return FALSE;
      SendSmsFrameHead(TXT_SEND);
      TAGPhoneNum(&g_gSmsPhone[PHONE3],g_gSmsPhone[PHONE3-1]);
      TAGMesType(ENG);
      TAGContent((BYTE*)ch,18,INHAND_DUANXIN);
      
      SendSmsFrame();
      return true;
  }
    /***************************************************************
      Function��SendSms
      ����    ���Ͷ���
      ������command  0:״̬�㱨  1������ң��  2��˫��ң��
      ���أ�TRUE �ɹ���FALSE ʧ��
  ***************************************************************/
  BOOL CPrtcSms::SendSms(unsigned char command)
  {
      BYTE CompID = g_gRunPara[RP_GPRS_COMP];
      BYTE SendType;  //�����������͡��ϱ�����OR״̬�㱨

      WORD YXNo;
      //g_gSmsHeadPt  = g_gSmsStartPtr;   //����ָ�븳ֵ����ָ��
      g_gSmsHeadPt  = g_gSmsHeadPtrBk;
      WORD RecFaultNum = GetCosNum(g_gSmsHeadPt,g_unSCosBuffTail);
      //g_gSmsHeadPtrBk = g_unSCosBuffTail;
      BYTE temp,i;
      BYTE j = 0;
      BYTE tempbuff[256];
      BYTE *pBuf = null;
      if ((RecFaultNum == 0) && (command == 1))
           return FALSE;
      if(RecFaultNum >= 4)    //һ��������౨4�����ϣ���INHAND_B���Ÿ�ʽ����
      {
          RecFaultNum = 4;
          g_yxChangeflag |= m_SmsSoeBit;  
          
      }
      g_RecFaultNum =  RecFaultNum;
      if(command > 0)
      {
          for(temp = 0; temp < RecFaultNum; temp++,g_gSmsHeadPt++)
          {
              g_gSmsHeadPt &= (SOE_BUFF_NUM - 1);
              pBuf = g_gSCosBuff[g_gSmsHeadPt];
              YXNo = MAKEWORD(pBuf[SOE_TYPEL],pBuf[SOE_TYPEH]);
              //write_infoadd(YXNo + ADDR_YX_LO);
              if(pBuf[SOE_STVAL] > 0)  //����Ƿ��и����ź�
              {
                  for(i=0;i<2;i++)
                  {
                      tempbuff[j++]=((YXNo + ADDR_YX_LO)>>(i*8))&0xff;
                  }        
                  //tempbuff[j++] = pBuf[SOE_STVAL];            
              }
              
          }
          if(j == 0)  //û�й�����Ϣ
          {
              g_gSmsHeadPtrBk    = g_gSmsHeadPt;
              //g_gSmsStartPtr  = g_gSmsHeadPt;
              return FALSE;
          }
          if(CompID == INHAND_A01)
          {
              SendType = INHAND_GZ_A01;
          }
          else if(CompID == INHAND_A0GS)
          {
              SendType = INHAND_GZ_A0GS;
          }
          else if(CompID == INHAND_A0)
          {
              SendType = INHAND_GZ_A0;
          }
          g_SmsReSendWaitCount = 90;
          g_SmsReSendTime++;  
      }
      else
      {
          if(CompID == INHAND_A01)
          {
              SendType = INHAND_HB_A01;
          }
          else if(CompID == INHAND_A0GS)
          {
              SendType = INHAND_HB_A0GS;
          }
          else if(CompID == INHAND_A0)
          {
              SendType = INHAND_HB_A0;
          }
      }
      g_SmsDataLen = j/2; //2���ֽڵ���һ�����ϣ�1������ת����4���ֽڶ��ų���
      switch(CompID)
      {
      case INHAND_A01:
      case INHAND_A0GS:
      case INHAND_A0:
          if(g_gSmsPhone[PHONE_ENCTL] & BIT0)
          {
              //memcpy(&(m_SendBuf.pBuf[m_SendBuf.wWritePtr]),&g_gSmsPhone[PHONE1],g_gSmsPhone[PHONE1_CHARLEN]);
              //m_SendBuf.wWritePtr += g_gSmsPhone[PHONE1_CHARLEN];
              SendSmsFrameHead(TXT_SEND);
              TAGPhoneNum(&g_gSmsPhone[PHONE1],g_gSmsPhone[PHONE1-1]);
              if(CompID == INHAND_A0)
              {
                  TAGMesType(ENG_ORDER);
              }
              else
              {
                  TAGMesType(ENG);
              }
              if(TAGContent(tempbuff,g_SmsDataLen,SendType))
              {
                  SendSmsFrame();
              }
              else
              {
                  g_gSmsHeadPtrBk = g_gSmsHeadPt;
                  return false;
              }
              
              if((g_gSmsPhone[PHONE_ENCTL] & BIT1) == 0)
              {
                  return true;
              }
          }
          if(g_gSmsPhone[PHONE_ENCTL] & BIT1)
          {
  
              SendSmsFrameHead(TXT_SEND); 
              TAGPhoneNum(&g_gSmsPhone[PHONE2],g_gSmsPhone[PHONE2-1]);
              TAGMesType(ENG);
              if(TAGContent(tempbuff,g_SmsDataLen,SendType))
              {
                  SendSmsFrame();
              }
              else
              {
                  g_gSmsHeadPtrBk = g_gSmsHeadPt;
                  return false;
              }          
              return true;
          }
          else
          {
            g_EnSmsSend = OFF;
              return false;
          }
          break;
      default:
          return false;
          break;
      }
  }
  
  BOOL CPrtcSms::SendSmsFrameHead(BYTE Command)
  {
  
      g_SmsWritePtr=0;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x55; //��ͷ
      g_SmsSendBuf[g_SmsWritePtr++] = 0xAA;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x55;
      g_SmsSendBuf[g_SmsWritePtr++] = 0xAA;
      g_SmsSendBuf[g_SmsWritePtr++] = Command; //������
      g_SmsSendBuf[g_SmsWritePtr++] = 0;
      g_SmsWritePtr++;                 //Ԥ��֡����
      return true;
  }
  
  BOOL CPrtcSms::TAGMesType(BYTE Type)
  {         
      g_SmsSendBuf[g_SmsWritePtr++] = 0x81;//��������
      g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x01;
      g_SmsSendBuf[g_SmsWritePtr++] = Type;   

      return true;
  }
  
  BOOL CPrtcSms::TAGPhoneNum(BYTE *PHONE,BYTE len)
  {
      g_SmsSendBuf[g_SmsWritePtr++] = 0x81; //�ֻ�����
      g_SmsSendBuf[g_SmsWritePtr++] = 0x80;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
      g_SmsSendBuf[g_SmsWritePtr++] = len;
    for(BYTE i = 0; i < len; i++)
      {
          g_SmsSendBuf[g_SmsWritePtr++] = *PHONE++;
      }
      return true;
  
  }
  BOOL CPrtcSms::TAGContent(BYTE *ptr,BYTE num,BYTE type)
  {
      BYTE temp;
      char ch[60];  //���ڷ��Ͷ����õĻ���
     
      if(type == INHAND_GZ_A0GS)
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //���ݳ���
          BYTE LenthPtr = g_SmsWritePtr;
          sprintf((char *)ch,"065056");
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }           
          for(temp = 0; temp < num; temp++)
          {
              sprintf((char *)ch,"%03d%03d%03d",(g_gRunPara[RP_COMM_ADDR] & 0xFF),((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),ptr[2*temp]);
              for(BYTE i = 0; i < strlen(ch); i++)
              {
                  g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
              }                
          }
          g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_GZ_A01)                            //��ģʽֻ����·�ӵز���
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //���ݳ���
          BYTE LenthPtr = g_SmsWritePtr;        
          char phaseline = 0;
          //68 72 02       ��ͷ      18�ֽ�  00 00 00 02    ��վ��ַ  24�ֽ�
          sprintf((char *)ch,"68 72 02\n 00 00 %02x %02x\n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF));
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }  
          
          //10-09-03 11:52:17   ϵͳʱ��  36�ֽ�
          sprintf((char *)ch,"%02d-%02d-%02d %02d:%02d:%02d\n",g_sRtcManager.m_gRealTimer[RTC_YEAR],g_sRtcManager.m_gRealTimer[RTC_MONTH],
                                                                 g_sRtcManager.m_gRealTimer[RTC_DATE],g_sRtcManager.m_gRealTimer[RTC_HOUR],
                                                                 g_sRtcManager.m_gRealTimer[RTC_MINUT],g_sRtcManager.m_gRealTimer[RTC_SEC]);
                                                                 
          for(BYTE i = 2; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //06 06 06 15 00 02 AA 00 C4    29�ֽ�
          
          for(BYTE i = 0; i < num; i++)
          {
              if((g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]]> RI_1A_SHORT) && (g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]] <= RI_3O_SHORT))//RI_12O_SHORTg_ucYXAddr[]�м�һ��ԭ���ϱ����ϵ�ʵ�ʵ���1��ʼ������ң�ŵ���0��ʼ
              {
                  phaseline = (g_ucYXAddr[ptr[2*i] - 1]) % 4 + ((g_ucYXAddr[ptr[2*i] - 1] >> 2) + 1) * 4;  //����������ƫ
                  sprintf((char *)ch,"00 00 00 00 %02x %02x %02x 00 C1 \n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF),phaseline);
                  for(BYTE i = 0; i < strlen(ch); i++)
                  {
                      g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
                  } 
              } 
              else if((g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]] > RI_1A_GND) && (g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]] <= RI_1A_LVD))
              {
                  phaseline = (g_ucYXAddr[ptr[2*i] - 1] - RI_1A_GND) % 4 + (((g_ucYXAddr[ptr[2*i] - 1] - RI_1A_GND) >> 2) + 1) * 4;
                  sprintf((char *)ch,"00 00 00 00 %02x %02x %02x 00 C2 \n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF),phaseline);
                  for(BYTE i = 0; i < strlen(ch); i++)
                  {
                      g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
                  }             
              }             
              else if((g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]] > RI_1A_LVD) && (g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]] <= RI_1A_CABT))
              {
                  phaseline = (g_ucYXAddr[ptr[2*i] - 1] - RI_1A_LVD) % 4 + (((g_ucYXAddr[ptr[2*i] - 1] - RI_1A_LVD) >> 2) + 1) * 4;
                  sprintf((char *)ch,"00 00 00 00 %02x %02x %02x 00 C4 \n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF),phaseline);
                  for(BYTE i = 0; i < strlen(ch); i++)
                  {
                      g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
                  }              
              }
              else
              {
                  return false;
              }
          }
          unsigned char checktemp = ChkSum(&g_SmsSendBuf[LenthPtr],g_SmsWritePtr - LenthPtr);//AddChar
          sprintf((char *)ch,"%02x 16\n",checktemp);
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          } 
          g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_GZ_A0)
      {
          BYTE temp;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x00;          
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x00; //���ݳ���
          BYTE LenthPtr = g_SmsWritePtr;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x41;   //���ϱ���ͷ  065 056
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x38;
          for(temp = 0; temp < num; temp++)
          {
              for(BYTE i = 0; i < 2;i++)
              {
                  g_SmsSendBuf[g_SmsWritePtr++] = (GetOwnAddr()>>(i*8))&0xff;
              } 
              g_SmsSendBuf[g_SmsWritePtr++] = ptr[2*temp];
          }
          g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_HB_A0GS) //״̬�㱨A��ʽ 
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //���ݳ���
          BYTE LenthPtr = g_SmsWritePtr;        
          sprintf((char *)ch,"065056");
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }    
          sprintf((char *)ch,"%03d%03d055",(g_gRunPara[RP_COMM_ADDR] & 0xFF),((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF));
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_HB_A01)  //״̬�㱨B��ʽ
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //���ݳ���
          BYTE LenthPtr = g_SmsWritePtr;        
          //����ͷ+��վ��ַ
          sprintf((char *)ch,"68 65 01\n00 00 %02x %02x\n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF));
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //ϵͳʱ��
          sprintf((char *)ch,"%02d-%02d-%02d %02d-%02d-%02d\n",g_sRtcManager.m_gRealTimer[RTC_YEAR],g_sRtcManager.m_gRealTimer[RTC_MONTH],
                                                               g_sRtcManager.m_gRealTimer[RTC_DATE],g_sRtcManager.m_gRealTimer[RTC_HOUR],
                                                               g_sRtcManager.m_gRealTimer[RTC_MINUT],g_sRtcManager.m_gRealTimer[RTC_SEC]);
          for(BYTE i = 2; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //��ء����ݵ�ѹ
          sprintf((char *)ch,"%d\n%d\n25\n3300\n29\n",g_gRmtMeas[RM_CAP_U],g_gRmtMeas[RM_BAT_U]);
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //�汾��
          for(BYTE i = 0; i < 5; i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = VerNum[i + 3];
          }
          
          sprintf((char *)ch,"\n1\n0\n");
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //�ֻ���
          //BYTE j = PHONE1;
          for(BYTE i = 0,j = PHONE1; i < g_gSmsPhone[PHONE1-1]; i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = g_gSmsPhone[j++];
          }
          g_SmsSendBuf[g_SmsWritePtr++] = 0x0A;   //�س�����
          for(BYTE i = 0,j = PHONE2; i < g_gSmsPhone[PHONE2-1]; i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = g_gSmsPhone[j++];
          }
          g_SmsSendBuf[g_SmsWritePtr++] = 0x0A;   //�س�����
          //Ԥ���ֻ���
          sprintf((char *)ch,"00000000000\n0\n3\n");
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          unsigned char checktemp = ChkSum(&g_SmsSendBuf[LenthPtr],g_SmsWritePtr - LenthPtr);//AddChar
          sprintf((char *)ch,"%02x 16\n",checktemp);
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_HB_A0)
      {
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x00;          
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x00; //���ݳ���
          BYTE LenthPtr = g_SmsWritePtr;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x41;   //���ϱ���ͷ  065 056
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x38;
          for(BYTE i = 0; i < 2;i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = (GetOwnAddr()>>(i*8))&0xff;
          }
          g_SmsSendBuf[g_SmsWritePtr++] = 0X55;
          g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_DUANXIN)
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = num; //���ݳ���
          //BYTE LenthPtr = g_SmsWritePtr;
          for(temp = 0; temp < num; temp++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ptr[temp];
          }
          //g_SmsSendBuf[LenthPtr - 1] = g_SmsWritePtr - LenthPtr;
      }
      else if(type == INHAND_CHAXUN)
      {
          for(temp = 0; temp < num; temp++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ptr[temp];
          }
      }
      
      return true;
  }
  BOOL CPrtcSms::SendSmsFrame(void)
  {
      g_SmsSendBuf[6] = g_SmsWritePtr - 7;
      ByAddCrc16(g_SmsSendBuf,g_SmsWritePtr);
      g_SmsWritePtr += 2;
      CommSendData((BYTE*)g_SmsSendBuf,g_SmsWritePtr,g_CmIdGPRS);
        if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
      CommSendData((BYTE*)g_SmsSendBuf,g_SmsWritePtr,g_CmIdDBG);
      return true;
  }



#endif


