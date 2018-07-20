/*------------------------------------------------------------------------
 Module:            PrtcSms.cpp
 Author:            linxueqin
 Project:           规约平台
 State:             V1.0
 Creation Date:     2015-3-16
 Description:       发送短信--与101协议共用GPRS通道，因此此类是101协议的基类
------------------------------------------------------------------------*/


#ifndef _PRTC_SMS

#define _PRTC_SMS
  #include "PrtcSms.h"
#include "..\Main.h"
  /***************************************************************
      Function：CPrtcSms
          构造函数，暂空
      参数：无
  
      返回：无
  ***************************************************************/
   CPrtcSms::CPrtcSms() : CPSecondary()
  {
  
  }
  
  //通道发送空闲处理
void CPrtcSms::DoSmsSendIdle(void)
{
        //发送短信
        if(g_EnSmsSend == ON)
        {
            if(g_SmsReSendWaitCount == 0)  //无重发的短信
            {
                if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_SEND_SMS]))  //短信发送允许
                {
                    //if(SearchCos(m_SmsSoeBit,0)) 
                    if(g_SmsReSendTime > 3)
                    {
                        g_SmsReSendTime = 0;
                        g_gSmsHeadPtrBk = g_gSmsHeadPt;
                    }                  
                    if(g_gSmsHeadPt != g_gSmsHeadPtrBk)  //单点遥信有无重发
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
            if(g_EnSendToXinhao == ON)//给信号源发短信
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
      Function：Init
          规约初始化
      参数：
      返回：TRUE 成功，FALSE 失败
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
        Function：OnTimeOut
            定时处理
        参数：TimerID
            TimerID 定时器ID
        返回：无
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
      Function：RecFrameAA
          解析收到的短信报文
      参数：
      返回：TRUE 成功，FALSE 失败
  ***************************************************************/
  BOOL CPrtcSms::RecFrameAA(VFrameAA * pReceiveFrame)
  {
      BYTE tempdata[256];
      //BYTE phoneNum[16];
      BYTE *ptr;
     // BYTE temp;
      
      switch(pReceiveFrame->Type)
      {
      case 0x07:      //读GPRS时间
          if(pReceiveFrame->byContent[0] == 0x1)
          {
              //WORD MSecond;
              //MSecond = MAKEWORD(pData[0], pData[1]);
              unsigned int unHYM[7];
              unHYM[0] = ((pReceiveFrame->byContent[1]) & 0x7F);//YEAR
              unHYM[1] = (pReceiveFrame->byContent[2]) & 0x0F;//MONTH
              unHYM[2] = (pReceiveFrame->byContent[3]) & 0x1F;//DAY
              unHYM[3] =g_sRtcManager.m_gRealTimer[RTC_WEEK];//SNTP无第几周的信息，将RTC芯片的第几周更新
              unHYM[4] = pReceiveFrame->byContent[4];  //HOUR
              unHYM[5] = pReceiveFrame->byContent[5];//MINUTE
              unHYM[6] = pReceiveFrame->byContent[6];//SEC
              WriteRealTime(unHYM);  //修改时间
              //ReadRealTime(0);//将实时芯片中的时间更新到
              BYTE bBuf[2] = {0xFC,0XFC};
              MyPrintfD(bBuf,2);
          }
          else
          {
              BYTE bBuf[2] = {0xFB,0XFB};
              MyPrintfD(bBuf,2);
          }            
          break;
      case 0x16:      //读GPRS配置
          RecIHDPara(pReceiveFrame);
      case 0x17:      //短信回复确认帧
          if((pReceiveFrame->byContent[0] == 0x81) && (pReceiveFrame->byContent[1] == 0x89))
          {
              if((pReceiveFrame->byContent[3] == 0x01) && (g_SmsReSendWaitCount > 0))  //收到短信回复确认帧
              {
                  g_SmsReSendWaitCount = 10;
                 // g_gSmsStartPtr = g_gSmsHeadPt;
                 g_gSmsHeadPtrBk = g_gSmsHeadPt;
              }
          }
          break;
      case 0x18:
          RecIHDWConf(pReceiveFrame);
      case 0x13:      //收到短信内容
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
                      else if(tempdata[5] == 0x30)  //收到信号源发送的否定确认
                      {
                          //如果在准实时模式下，此处需要关闭电源
                          g_sTimer[TM_WAITXINHAO].m_TmCount = 0;
                          if(g_sTimer[TM_GPRSPWOERDOWN].m_TmCount > 0)
                          {
                              g_sTimer[TM_GPRSPWOERDOWN].m_TmCount = 1;
                          }
                          for(BYTE j = 0; j < g_unGroundBuffTail; j++)    // 接地闭锁清0
                          {
                              g_gRmtLock[g_gGroundBuff[j]] = 0;
                          }
                          g_unGroundBuffTail = 0;
                      }
                  }
                  else if(tempdata[0] == 'g' && tempdata[1] == 'r' && tempdata[2] == 'e' && tempdata[3] == 'p')  //短信读命令提示符
                  {
                      if((g_gSmsPhone[PHONE_ENCTL] & BIT3) && (g_gSmsPhone[PHONE_ENCTL] & BIT4))   //号码4和号码5同时启用
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
                  && tempdata[4] == 'p' && tempdata[5] == 'a' && tempdata[6] == 'r' && tempdata[7] == 'a') //短信写命令提示符
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
      Function：SendRCmdToIHD
          给映瀚通模块发读配置命令
      参数：
      返回：TRUE 成功，FALSE 失败
      日期：2015年8月20 BY zx
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
      case 0:       //读IP地址和端口
          SendSmsFrameHead(READ_SPE_CONFIG);
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x19;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          //TAGContent(tempbuff,4,INHAND_CHAXUN);
          break;
      case 1://读短信功能开启状态
          SendSmsFrameHead(READ_SPE_CONFIG);
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x87;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          //TAGContent(tempbuff,4,INHAND_CHAXUN);
          break;
      case 2:  //APN
      case 3:  //用户名
      case 4:  //密码
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
      Function：SendWCmdToIHD
          给映瀚通模块发写配置命令
      参数：
      返回：TRUE 成功，FALSE 失败
      日期：2015年8月20 BY zx
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
      case 0:       //写IP地址和端口
          
          g_SmsSendBuf[g_SmsWritePtr++] = 0;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x19;
		  OpenTimer(TM_GPRS_RW);//延时2S发送是否进入高低功耗的报文
          if (dataLen > 6)
              return false;
          break;
      case 1://开/关短信功能
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x87;
   
          break;
      case 2:  //APN
      case 3:  //用户名
      case 4:  //密码
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
      
	  //OpenTimer(TM_GPRS_RW);//延时2S发送是否进入高低功耗的报文
      return true;
  }

    /***************************************************************
      Function：Send_GPRS_Power
          发送映瀚通高低功耗
      参数：
      返回：TRUE 成功，FALSE 失败
      日期：2018年4月20 BY zxx
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
      Function：RecIHDPara
          接收映瀚通发送的配置参数確認
      参数：
      返回：TRUE 成功，FALSE 失败
      日期：2015年8月20 BY zx
  ***************************************************************/  
  BOOL CPrtcSms::RecIHDWConf(VFrameAA *pReceiveFrame)
  {
      WORD wTag = MAKEWORD(pReceiveFrame->byContent[1],pReceiveFrame->byContent[0]);//高位在前
     // BYTE secName = 0;
      switch(wTag)
      {
      case 0x0002:   //读APN
        //  secName = 2;
          break;
      case 0x0003:   //读用户名
         // secName = 3;
          break;
      case 0x0004:   //读密码
          //secName = 4;
          break;
      case 0x0019:   //读IP及端口
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
      Function：RecIHDPara
          接收映瀚通发送的配置参数
      参数：
      返回：TRUE 成功，FALSE 失败
      日期：2015年8月20 BY zx
  ***************************************************************/  
  BOOL CPrtcSms::RecIHDPara(VFrameAA *pReceiveFrame)
  {
      BYTE secName;
      WORD wTag = MAKEWORD(pReceiveFrame->byContent[1],pReceiveFrame->byContent[0]);//高位在前

      switch(wTag)
      {
      case 0x0002:   //读APN
          secName = 2;
          break;
      case 0x0003:   //读用户名
          secName = 3;
          break;
      case 0x0004:   //读密码
          secName = 4;
          break;
      case 0x0019:   //读IP及端口
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
      Function：RecIHDParaMes
          接收映瀚通发送的配置参数
      参数：
      返回：TRUE 成功，FALSE 失败
      日期：2015年8月20 BY zx
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
              
              g_SmsCmdCode =g_sRtcManager.m_gRealTimer[RTC_MICROSEC]*1000 + g_sRtcManager.m_gRealTimer[RTC_MICROSEC];  //生成校验码
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
      Function：SendSmsWPConf
      功能：对短信修改命令的回复短信
      输入参数：
      返回：TRUE 成功，FALSE 失败
      日期：2015年11月26 BY zx
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
          if(strcmp((char *)code,(char *)ch) == 0)  //校验码对比
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
      Function：SendSms
      功能    发送短信
      参数：command  0:状态汇报  1：单点遥信  2：双点遥信
      返回：TRUE 成功，FALSE 失败
  ***************************************************************/
  BOOL CPrtcSms::SendSms(unsigned char command)
  {
      BYTE CompID = g_gRunPara[RP_GPRS_COMP];
      BYTE SendType;  //发送数据类型、上报故障OR状态汇报

      WORD YXNo;
      //g_gSmsHeadPt  = g_gSmsStartPtr;   //备份指针赋值发送指针
      g_gSmsHeadPt  = g_gSmsHeadPtrBk;
      WORD RecFaultNum = GetCosNum(g_gSmsHeadPt,g_unSCosBuffTail);
      //g_gSmsHeadPtrBk = g_unSCosBuffTail;
      BYTE temp,i;
      BYTE j = 0;
      BYTE tempbuff[256];
      BYTE *pBuf = null;
      if ((RecFaultNum == 0) && (command == 1))
           return FALSE;
      if(RecFaultNum >= 4)    //一条短信最多报4个故障，由INHAND_B短信格式决定
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
              if(pBuf[SOE_STVAL] > 0)  //检查是否有复归信号
              {
                  for(i=0;i<2;i++)
                  {
                      tempbuff[j++]=((YXNo + ADDR_YX_LO)>>(i*8))&0xff;
                  }        
                  //tempbuff[j++] = pBuf[SOE_STVAL];            
              }
              
          }
          if(j == 0)  //没有故障信息
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
      g_SmsDataLen = j/2; //2个字节点表号一个故障，1个故障转化成4个字节短信长度
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
      g_SmsSendBuf[g_SmsWritePtr++] = 0x55; //包头
      g_SmsSendBuf[g_SmsWritePtr++] = 0xAA;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x55;
      g_SmsSendBuf[g_SmsWritePtr++] = 0xAA;
      g_SmsSendBuf[g_SmsWritePtr++] = Command; //控制字
      g_SmsSendBuf[g_SmsWritePtr++] = 0;
      g_SmsWritePtr++;                 //预留帧长度
      return true;
  }
  
  BOOL CPrtcSms::TAGMesType(BYTE Type)
  {         
      g_SmsSendBuf[g_SmsWritePtr++] = 0x81;//短信类型
      g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
      g_SmsSendBuf[g_SmsWritePtr++] = 0x01;
      g_SmsSendBuf[g_SmsWritePtr++] = Type;   

      return true;
  }
  
  BOOL CPrtcSms::TAGPhoneNum(BYTE *PHONE,BYTE len)
  {
      g_SmsSendBuf[g_SmsWritePtr++] = 0x81; //手机号码
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
      char ch[60];  //用于发送短信用的缓存
     
      if(type == INHAND_GZ_A0GS)
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //数据长度
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
      else if(type == INHAND_GZ_A01)                            //该模式只发短路接地测试
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //数据长度
          BYTE LenthPtr = g_SmsWritePtr;        
          char phaseline = 0;
          //68 72 02       报头      18字节  00 00 00 02    子站地址  24字节
          sprintf((char *)ch,"68 72 02\n 00 00 %02x %02x\n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF));
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }  
          
          //10-09-03 11:52:17   系统时间  36字节
          sprintf((char *)ch,"%02d-%02d-%02d %02d:%02d:%02d\n",g_sRtcManager.m_gRealTimer[RTC_YEAR],g_sRtcManager.m_gRealTimer[RTC_MONTH],
                                                                 g_sRtcManager.m_gRealTimer[RTC_DATE],g_sRtcManager.m_gRealTimer[RTC_HOUR],
                                                                 g_sRtcManager.m_gRealTimer[RTC_MINUT],g_sRtcManager.m_gRealTimer[RTC_SEC]);
                                                                 
          for(BYTE i = 2; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //06 06 06 15 00 02 AA 00 C4    29字节
          
          for(BYTE i = 0; i < num; i++)
          {
              if((g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]]> RI_1A_SHORT) && (g_ucYXAddr[ptr[2*i] - g_gRunPara[RP_SYX_INFADDR]] <= RI_3O_SHORT))//RI_12O_SHORTg_ucYXAddr[]中减一的原因：上报故障的实际点表从1开始，虚拟遥信点表从0开始
              {
                  phaseline = (g_ucYXAddr[ptr[2*i] - 1]) % 4 + ((g_ucYXAddr[ptr[2*i] - 1] >> 2) + 1) * 4;  //计算线盘相偏
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
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x00; //数据长度
          BYTE LenthPtr = g_SmsWritePtr;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x41;   //故障报文头  065 056
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
      else if(type == INHAND_HB_A0GS) //状态汇报A格式 
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //数据长度
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
      else if(type == INHAND_HB_A01)  //状态汇报B格式
      {
          g_SmsSendBuf[g_SmsWritePtr++] = 0x81;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x82;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00;
          g_SmsSendBuf[g_SmsWritePtr++] = 0x00; //数据长度
          BYTE LenthPtr = g_SmsWritePtr;        
          //报文头+子站地址
          sprintf((char *)ch,"68 65 01\n00 00 %02x %02x\n",((g_gRunPara[RP_COMM_ADDR] >> 8) & 0xFF),(g_gRunPara[RP_COMM_ADDR] & 0xFF));
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //系统时间
          sprintf((char *)ch,"%02d-%02d-%02d %02d-%02d-%02d\n",g_sRtcManager.m_gRealTimer[RTC_YEAR],g_sRtcManager.m_gRealTimer[RTC_MONTH],
                                                               g_sRtcManager.m_gRealTimer[RTC_DATE],g_sRtcManager.m_gRealTimer[RTC_HOUR],
                                                               g_sRtcManager.m_gRealTimer[RTC_MINUT],g_sRtcManager.m_gRealTimer[RTC_SEC]);
          for(BYTE i = 2; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //电池、电容电压
          sprintf((char *)ch,"%d\n%d\n25\n3300\n29\n",g_gRmtMeas[RM_CAP_U],g_gRmtMeas[RM_BAT_U]);
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //版本号
          for(BYTE i = 0; i < 5; i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = VerNum[i + 3];
          }
          
          sprintf((char *)ch,"\n1\n0\n");
          for(BYTE i = 0; i < strlen(ch); i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = ch[i];
          }
          //手机号
          //BYTE j = PHONE1;
          for(BYTE i = 0,j = PHONE1; i < g_gSmsPhone[PHONE1-1]; i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = g_gSmsPhone[j++];
          }
          g_SmsSendBuf[g_SmsWritePtr++] = 0x0A;   //回车换行
          for(BYTE i = 0,j = PHONE2; i < g_gSmsPhone[PHONE2-1]; i++)
          {
              g_SmsSendBuf[g_SmsWritePtr++] = g_gSmsPhone[j++];
          }
          g_SmsSendBuf[g_SmsWritePtr++] = 0x0A;   //回车换行
          //预留手机号
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
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x00; //数据长度
          BYTE LenthPtr = g_SmsWritePtr;
          g_SmsSendBuf[g_SmsWritePtr++]  = 0x41;   //故障报文头  065 056
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
          g_SmsSendBuf[g_SmsWritePtr++] = num; //数据长度
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


