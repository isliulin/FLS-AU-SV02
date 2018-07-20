/*------------------------------------------------------------------------
 Module:            PrtcGPR.cpp
 Author:            linxueqin
 Project:           规约平台
 State:             V1.0
 Creation Date:     2015-3-16
 Description:      GPR定位保卫
------------------------------------------------------------------------*/



#ifndef _PRTC_GPS

  #define _PRTC_GPS
  #include "PrtcGPS.h"
#include "..\Main.h"
//构造函数
CPrtcGPS::CPrtcGPS() : CProtocol()
{

}
  void CPrtcGPS::Run(void)
  {
     RcvData(&m_uartId);
     //DoCommSendIdle();
  }
  
  
  /***************************************************************
      Function：Init
          规约初始化
      参数：
      返回：TRUE 成功，FALSE 失败
  ***************************************************************/
  BOOL CPrtcGPS::Init(WORD uartId)
  {
      BOOL rc;
      //OPEN_GPS_POWER;
      m_uartId = uartId;
      rc = CProtocol::Init(1);
      if (!rc)
      {
        return FALSE;
      }
      m_Len = 0;
      End_ptr=0;
      Start_ptr=0;
      /*delay_1ms(2000);//delayms(3000);//电源稳定后再配置GPS模块，否则容易配置不成功
	  GPS_Config(GPS_PCAS04);
	  GPS_Config(GPS_CCRM0);
	  GPS_Config(GPS_PCAS03);
	  GPS_Config(GPS_PCAS00);
	  //if((g_gRunPara[RP_CFG_KEYH])&(BIT[RP_OFF_GPS-16]))
	 CLOSE_GPS_POWER;*/
      return 1;
  }
  /***************************************************************
      Function：GPS_Config
          GPS模块的默认配置
      参数：参数类型
  ***************************************************************/
  /*void CPrtcGPS::GPS_Config(BYTE Type)
  {
	char ph[50];
	unsigned char GPS_CRC=0;
	m_SendBuf.wWritePtr=0;
        BYTE j;
	switch  (Type)
	{
		 case GPS_PCAS00://保存配置信息
		 	sprintf((char *)ph,"$PCAS00");
		 	for(j = 0; j < strlen(ph); j++)
                        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		 break;
		 case GPS_PCAS03://设置要求输出或停止输出的NMEA语句
		 	sprintf((char *)ph,"$PCAS03,1,0,0,0,0,0,0,0");//"$PCAS03,0,0,0,0,1,0,0,0");//打开RMC
		 	for(j = 0; j < strlen(ph); j++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		break;
		case GPS_PCAS04://配置工作系统GPS BDS GLONAS
			sprintf((char *)ph,"$PCAS04,3");//北斗和GPS双模
		 	for(j = 0; j < strlen(ph); j++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		break;
		case GPS_CCRM0://开始或停止输出NMEA语句
			sprintf((char *)ph,"$CCRM0,GGA,3,1");//关闭全部语句
		 	for(j = 0; j < strlen(ph); j++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		break;
		default:
            break;
	}
		GPS_CRC = XorChar(&m_SendBuf.pBuf[1], strlen(ph)-1);
		m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]  = 0x2A;
		
		m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]= HexToChar(GPS_CRC >> 4);
        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = HexToChar(GPS_CRC & 0x0F);
		m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]= 0X0D;
		m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]= 0X0A;
		CommSendData((char *)m_SendBuf.pBuf,m_SendBuf.wWritePtr,g_CmIdGps);
      	//CommWrite((char *)m_SendBuf.pBuf,m_SendBuf.wWritePtr);
        delay_1ms(1000);
  }*/
  /***************************************************************
      Function：DoReceive
          接收报文处理
      参数：无
  
      返回：TRUE 成功，FALSE 失败
  ***************************************************************/
    BOOL CPrtcGPS::DoReceive()
    {
       while(1)
       {
          if (SearchFrame() != TRUE)
          return FALSE;
          //unsigned short FrameLen;
          pReceiveFrame = (VIecGprFrame *)m_RecFrame.pBuf;
         // GprFrame=m_RecFrame.pBuf
          //memcpy(FrameGPR.byContent,m_RecFrame.pBuf,255);
          //if(g_gpsPowerSt ==1)//zxx
          {
          	RecFrameGPR(&pReceiveFrame->FrameGPR);
          if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])
           {//设置了将GPS收到的数据转发到调试口
                MyPrintfD(&(pReceiveFrame->FrameGPR.Start),pReceiveFrame->FrameGPR.FrameLen);
           }
          }
          continue;
          
 
       }
    }
  
  
  
 
  
  /***************************************************************
      Function：OnTimeOut
          定时处理
      参数：TimerID
          TimerID 定时器ID
      返回：无
  ***************************************************************/
 /* BOOL CPrtcGPR::DoTimeOut(WORD wTimerID)
  {
   return 1;
  }*/
  
  
  /***************************************************************
      Function：SearchOneFrame
          搜索一帧正确的报文
      参数：Buf, Len
          Buf 接收缓冲区头指针
          Len 接收缓冲区内有效数据的长度
      返回：DWORD数据，其中
          高字：处理结果
            #define FRAME_OK       0x00010000      //检测到一个完整的帧
            #define FRAME_ERR      0x00020000      //检测到一个校验错误的帧
            #define FRAME_LESS     0x00030000      //检测到一个不完整的帧（还未收齐）
          低字：已经处理的数据长度
  ***************************************************************/
  DWORD CPrtcGPS::SearchOneFrame(BYTE *Buf, WORD Len)
  {
      unsigned short FrameLen =0;
      BYTE crc[2],*ptr;
      unsigned char GPS_CRC,GPS_CRCH,GPS_CRCL;
      if (Len < 5)
          return FRAME_LESS;
    
      pReceiveFrame = (VIecGprFrame *)Buf; 
	  if(pReceiveFrame->FrameGPR.Start == '$')//起始符$
	  {
	  		//FrameLen =0;
        	for(int i=0;i < Len;i++)
        	{
                
         	  if(pReceiveFrame->FrameGPR.byContent[i] == '*') //*
            	  {
             		FrameLen= i;
                	break;
            	  }     
         	}
		if(FrameLen <= 5)
                  return FRAME_LESS;
	    ptr = (BYTE*)pReceiveFrame->FrameGPR.byContent;
            crc[0] = ptr[FrameLen + 1];
            crc[1] = ptr[FrameLen + 2];
	    	GPS_CRC = XorChar(ptr, FrameLen);
            GPS_CRCH = HexToChar(GPS_CRC >> 4);
            GPS_CRCL = HexToChar(GPS_CRC & 0x0F); 
              
             if((crc[0] != GPS_CRCH) || (crc[1] != GPS_CRCL))
             {
                return FRAME_ERR|1;
             }
	      if((ptr[FrameLen + 3] != 0x0D) ||(ptr[FrameLen + 4] != 0x0A))
	      {
			return FRAME_ERR|1;
	      }
	      pReceiveFrame->FrameGPR.FrameLen = FrameLen+6;
	      return FRAME_OK|(FrameLen+6);
	  }
      else
	  return FRAME_ERR|1;
  }
  
  
  //GNGGA
   /***************************************************************
      Function：RecFrameGPR
         接收GPS信息，由于GPS报文协议很多，目前配置后只发送GNGGA的协议
      参数：VFrameGPR *pReceiveFrame
       
  ***************************************************************/
  BOOL CPrtcGPS::RecFrameGPR(VFrameGPR *pReceiveFrame)
  { 
    BYTE n_type = 0;
	//INT8U  Time[7]; //Second,Minute,Hour，week;Day,Month,Year
	
	//INT8U  Date[3]; //Day,Month,Year
	INT8U  content_position =0;
	unsigned int MicSecCon ;  //秒计时
    //if((pReceiveFrame->byContent[0] == 'G') && (pReceiveFrame->byContent[1] == 'N')&&
    //(pReceiveFrame->byContent[2] == 'G')&& (pReceiveFrame->byContent[3] == 'G') && (pReceiveFrame->byContent[4] == 'A')) //$GPRMC 
	
	if((pReceiveFrame->byContent[0] == 'G') && (pReceiveFrame->byContent[1] == 'N')&&
		(pReceiveFrame->byContent[2] == 'R')&& (pReceiveFrame->byContent[3] == 'M') && (pReceiveFrame->byContent[4] == 'C')) //$GPRMC 
	{

      Start_ptr = 0;//第一段内容的，
      
      for(int i=0;i < pReceiveFrame->FrameLen;i++)
      {
                
        if((pReceiveFrame->byContent[i] == 0x2C)&&(n_type < 16)) //,17标志用来区分每一段的内容,共13段
        {
          End_ptr = i+1;
          para_Content_Length[n_type]=(End_ptr-Start_ptr-1);//每一段的长度放在数组para_Content中
          Start_ptr =End_ptr; 
          n_type++;
		  
          if(n_type ==3)//位置有效标志 V 接收机警告 ，数据无效，A 数据有效    //第7段表示当前定位质量
          {
			 content_position = 1+para_Content_Length[0];
             MicSecCon = Char3ToHex(pReceiveFrame->byContent[content_position+9],pReceiveFrame->byContent[content_position+8],pReceiveFrame->byContent[content_position+7]);

			if((pReceiveFrame->byContent[End_ptr-2] == 'A')&&(MicSecCon ==0))//> 0x30)从0开始，还有0x2c
			{
				if(pReceiveFrame->flag == ON)//连续十次报文有效后再去解析秒脉冲
				{
					(pReceiveFrame->flag_num)++;
					if(pReceiveFrame->flag_num >10)
						pReceiveFrame->flag = TRUE;
				}
				else 
				{
					pReceiveFrame->flag = ON;
					pReceiveFrame->flag_num =0;//GPS连续10次有信号后才采集秒脉冲，因为该芯片的前几次秒脉冲有可能有ms级误差
				}
				
			}
            else
              pReceiveFrame->flag = FALSE;   
          }
		  if(pReceiveFrame->flag == TRUE)//如果第三段的位置有效，把第二段的时间信息取出来
		  {
		  		switch(n_type)
          		{
           			case 0x03://UTC时间在第二段
					if(para_Content_Length[1] >0)
					{
						content_position = 1+para_Content_Length[0];
						if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
						{
        		  			BYTE bBuf[6] = {0xE5,g_ccr0,g_ccr0>>8,g_sRtcManager.m_gRealTimer[RTC_MICROSEC],g_sRtcManager.m_gRealTimer[RTC_MICROSEC]>>8,g_sRtcManager.m_gRealTimer[RTC_SEC]};
        		  			MyPrintfD(bBuf,6);
				
						}
                          if(CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position]) !=0xFF)
							g_sRtcManager.m_gRealTimerBK[RTC_HOUR] = CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position]);
						if(CharToHex(pReceiveFrame->byContent[content_position+3],pReceiveFrame->byContent[content_position+2])!=0xFF)
							g_sRtcManager.m_gRealTimerBK[RTC_MINUT] = CharToHex(pReceiveFrame->byContent[content_position+3],pReceiveFrame->byContent[content_position+2]);
						if(CharToHex(pReceiveFrame->byContent[content_position+5],pReceiveFrame->byContent[content_position+4])!=0xFF)
							g_sRtcManager.m_gRealTimerBK[RTC_SEC]  = CharToHex(pReceiveFrame->byContent[content_position+5],pReceiveFrame->byContent[content_position+4]);
						/*if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
						{
        		  			BYTE bBuf[5] = {0xE5,g_ccr0,g_ccr0>>8,g_sRtcManager.m_gRealTimer[RTC_MICROSEC],g_sRtcManager.m_gRealTimer[RTC_MICROSEC]>>8};
        		  			MyPrintfD(bBuf,5);
				
						}*/
						//g_sRtcManager.m_gRealTimer[RTC_MICROSEC]=0;
                	}
					break;
					case 0x0A://第10段表示日期
					if(para_Content_Length[10-1] >0)
					{
						content_position = 10-1;//,所占字节数
						for(BYTE j=0;j<10-1;j++)
							content_position += para_Content_Length[j];//前面9段内容所占的字节数
						//Time[3] = 0;   //星期CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position]);
						if(CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position])!=0xFF)
							g_sRtcManager.m_gRealTimerBK[RTC_DATE] = CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position]);
						if(CharToHex(pReceiveFrame->byContent[content_position+3],pReceiveFrame->byContent[content_position+2])!=0xFF)

							g_sRtcManager.m_gRealTimerBK[RTC_MONTH] = CharToHex(pReceiveFrame->byContent[content_position+3],pReceiveFrame->byContent[content_position+2]);
						if(CharToHex(pReceiveFrame->byContent[content_position+5],pReceiveFrame->byContent[content_position+4])!=0xFF)
						g_sRtcManager.m_gRealTimerBK[RTC_YEAR] = CharToHex(pReceiveFrame->byContent[content_position+5],pReceiveFrame->byContent[content_position+4])+2000;
						//WriteRealTime(g_sRtcManager.m_gRealTimer);//GPS时间更新到RTC的时间上
						RunRTClock(8);//UTC时间与北京时间的误差是8小时
					}
					break;
					default:
					break;
						
		  		}
				
		  }
          /*switch(n_type)
          {
           case 0x01://UTC时间
           for(j=0;j<9;j++) 
           {
              //BYTE n=i+j+1;
             GPR.Content_Utc[j] = pReceiveFrame->byContent[i+j+1];//UTC时间
           } 
           break;
           case 0x02://定位状态
              GPR.Content_State = pReceiveFrame->byContent[i+1];
          break;
          case 0x03://纬度
           for(j=0;j<9;j++) 
           {
              GPR.Content_Latitude[j] = pReceiveFrame->byContent[i+j+1];//UTC时间
           }    
           break;
          case 0x04://南纬或北纬
              GPR.Latitude_Direction = pReceiveFrame->byContent[i+1];//UTC时间
            break;
          case 0x05://经度
           for(j=0;j<9;j++) 
           {
              GPR.Content_Longitude[j] = pReceiveFrame->byContent[i+j+1];//UTC时间
           }  
           break;
         case 0x06://西经或东经
              GPR.Longitude_Direction = (pReceiveFrame->byContent[i+1]);//UTC时间
            break;
         case 0x09://UTC日期
           for(j=0;j<6;j++) 
           {
              GPR.Content_Date[j] = pReceiveFrame->byContent[i+j+1];//UTC时间
           }    
           break;
           
         default:
           break;
        }*/
                
      }
    }
  }
  return TRUE;
}
void CPrtcGPS::SendRCmdToGPR(WORD GprPara,BYTE ParaName,CProtocol *pDaSrc)
{
  if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;

  {//RMC
     WORD wFileNa = 0;//文件名
      wFileNa = 12;
      if(m_RWPaSrcObj !=null)
      {
        m_RWPaSrcObj->SendReadPa(wFileNa,0);
        m_RWPaSrcObj = null;
           //将GPS收到的数据转发到调试通道
            if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF]))
            {
              MyPrintfD(m_RecFrame.pBuf,100);
            }
      }
  }
  
   
}
#endif



