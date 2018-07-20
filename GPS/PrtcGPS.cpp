/*------------------------------------------------------------------------
 Module:            PrtcGPR.cpp
 Author:            linxueqin
 Project:           ��Լƽ̨
 State:             V1.0
 Creation Date:     2015-3-16
 Description:      GPR��λ����
------------------------------------------------------------------------*/



#ifndef _PRTC_GPS

  #define _PRTC_GPS
  #include "PrtcGPS.h"
#include "..\Main.h"
//���캯��
CPrtcGPS::CPrtcGPS() : CProtocol()
{

}
  void CPrtcGPS::Run(void)
  {
     RcvData(&m_uartId);
     //DoCommSendIdle();
  }
  
  
  /***************************************************************
      Function��Init
          ��Լ��ʼ��
      ������
      ���أ�TRUE �ɹ���FALSE ʧ��
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
      /*delay_1ms(2000);//delayms(3000);//��Դ�ȶ���������GPSģ�飬�����������ò��ɹ�
	  GPS_Config(GPS_PCAS04);
	  GPS_Config(GPS_CCRM0);
	  GPS_Config(GPS_PCAS03);
	  GPS_Config(GPS_PCAS00);
	  //if((g_gRunPara[RP_CFG_KEYH])&(BIT[RP_OFF_GPS-16]))
	 CLOSE_GPS_POWER;*/
      return 1;
  }
  /***************************************************************
      Function��GPS_Config
          GPSģ���Ĭ������
      ��������������
  ***************************************************************/
  /*void CPrtcGPS::GPS_Config(BYTE Type)
  {
	char ph[50];
	unsigned char GPS_CRC=0;
	m_SendBuf.wWritePtr=0;
        BYTE j;
	switch  (Type)
	{
		 case GPS_PCAS00://����������Ϣ
		 	sprintf((char *)ph,"$PCAS00");
		 	for(j = 0; j < strlen(ph); j++)
                        m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		 break;
		 case GPS_PCAS03://����Ҫ�������ֹͣ�����NMEA���
		 	sprintf((char *)ph,"$PCAS03,1,0,0,0,0,0,0,0");//"$PCAS03,0,0,0,0,1,0,0,0");//��RMC
		 	for(j = 0; j < strlen(ph); j++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		break;
		case GPS_PCAS04://���ù���ϵͳGPS BDS GLONAS
			sprintf((char *)ph,"$PCAS04,3");//������GPS˫ģ
		 	for(j = 0; j < strlen(ph); j++)
              m_SendBuf.pBuf[ m_SendBuf.wWritePtr++] = ph[j];
		break;
		case GPS_CCRM0://��ʼ��ֹͣ���NMEA���
			sprintf((char *)ph,"$CCRM0,GGA,3,1");//�ر�ȫ�����
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
      Function��DoReceive
          ���ձ��Ĵ���
      ��������
  
      ���أ�TRUE �ɹ���FALSE ʧ��
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
           {//�����˽�GPS�յ�������ת�������Կ�
                MyPrintfD(&(pReceiveFrame->FrameGPR.Start),pReceiveFrame->FrameGPR.FrameLen);
           }
          }
          continue;
          
 
       }
    }
  
  
  
 
  
  /***************************************************************
      Function��OnTimeOut
          ��ʱ����
      ������TimerID
          TimerID ��ʱ��ID
      ���أ���
  ***************************************************************/
 /* BOOL CPrtcGPR::DoTimeOut(WORD wTimerID)
  {
   return 1;
  }*/
  
  
  /***************************************************************
      Function��SearchOneFrame
          ����һ֡��ȷ�ı���
      ������Buf, Len
          Buf ���ջ�����ͷָ��
          Len ���ջ���������Ч���ݵĳ���
      ���أ�DWORD���ݣ�����
          ���֣�������
            #define FRAME_OK       0x00010000      //��⵽һ��������֡
            #define FRAME_ERR      0x00020000      //��⵽һ��У������֡
            #define FRAME_LESS     0x00030000      //��⵽һ����������֡����δ���룩
          ���֣��Ѿ���������ݳ���
  ***************************************************************/
  DWORD CPrtcGPS::SearchOneFrame(BYTE *Buf, WORD Len)
  {
      unsigned short FrameLen =0;
      BYTE crc[2],*ptr;
      unsigned char GPS_CRC,GPS_CRCH,GPS_CRCL;
      if (Len < 5)
          return FRAME_LESS;
    
      pReceiveFrame = (VIecGprFrame *)Buf; 
	  if(pReceiveFrame->FrameGPR.Start == '$')//��ʼ��$
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
      Function��RecFrameGPR
         ����GPS��Ϣ������GPS����Э��ܶ࣬Ŀǰ���ú�ֻ����GNGGA��Э��
      ������VFrameGPR *pReceiveFrame
       
  ***************************************************************/
  BOOL CPrtcGPS::RecFrameGPR(VFrameGPR *pReceiveFrame)
  { 
    BYTE n_type = 0;
	//INT8U  Time[7]; //Second,Minute,Hour��week;Day,Month,Year
	
	//INT8U  Date[3]; //Day,Month,Year
	INT8U  content_position =0;
	unsigned int MicSecCon ;  //���ʱ
    //if((pReceiveFrame->byContent[0] == 'G') && (pReceiveFrame->byContent[1] == 'N')&&
    //(pReceiveFrame->byContent[2] == 'G')&& (pReceiveFrame->byContent[3] == 'G') && (pReceiveFrame->byContent[4] == 'A')) //$GPRMC 
	
	if((pReceiveFrame->byContent[0] == 'G') && (pReceiveFrame->byContent[1] == 'N')&&
		(pReceiveFrame->byContent[2] == 'R')&& (pReceiveFrame->byContent[3] == 'M') && (pReceiveFrame->byContent[4] == 'C')) //$GPRMC 
	{

      Start_ptr = 0;//��һ�����ݵģ�
      
      for(int i=0;i < pReceiveFrame->FrameLen;i++)
      {
                
        if((pReceiveFrame->byContent[i] == 0x2C)&&(n_type < 16)) //,17��־��������ÿһ�ε�����,��13��
        {
          End_ptr = i+1;
          para_Content_Length[n_type]=(End_ptr-Start_ptr-1);//ÿһ�εĳ��ȷ�������para_Content��
          Start_ptr =End_ptr; 
          n_type++;
		  
          if(n_type ==3)//λ����Ч��־ V ���ջ����� ��������Ч��A ������Ч    //��7�α�ʾ��ǰ��λ����
          {
			 content_position = 1+para_Content_Length[0];
             MicSecCon = Char3ToHex(pReceiveFrame->byContent[content_position+9],pReceiveFrame->byContent[content_position+8],pReceiveFrame->byContent[content_position+7]);

			if((pReceiveFrame->byContent[End_ptr-2] == 'A')&&(MicSecCon ==0))//> 0x30)��0��ʼ������0x2c
			{
				if(pReceiveFrame->flag == ON)//����ʮ�α�����Ч����ȥ����������
				{
					(pReceiveFrame->flag_num)++;
					if(pReceiveFrame->flag_num >10)
						pReceiveFrame->flag = TRUE;
				}
				else 
				{
					pReceiveFrame->flag = ON;
					pReceiveFrame->flag_num =0;//GPS����10�����źź�Ųɼ������壬��Ϊ��оƬ��ǰ�����������п�����ms�����
				}
				
			}
            else
              pReceiveFrame->flag = FALSE;   
          }
		  if(pReceiveFrame->flag == TRUE)//��������ε�λ����Ч���ѵڶ��ε�ʱ����Ϣȡ����
		  {
		  		switch(n_type)
          		{
           			case 0x03://UTCʱ���ڵڶ���
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
					case 0x0A://��10�α�ʾ����
					if(para_Content_Length[10-1] >0)
					{
						content_position = 10-1;//,��ռ�ֽ���
						for(BYTE j=0;j<10-1;j++)
							content_position += para_Content_Length[j];//ǰ��9��������ռ���ֽ���
						//Time[3] = 0;   //����CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position]);
						if(CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position])!=0xFF)
							g_sRtcManager.m_gRealTimerBK[RTC_DATE] = CharToHex(pReceiveFrame->byContent[content_position+1],pReceiveFrame->byContent[content_position]);
						if(CharToHex(pReceiveFrame->byContent[content_position+3],pReceiveFrame->byContent[content_position+2])!=0xFF)

							g_sRtcManager.m_gRealTimerBK[RTC_MONTH] = CharToHex(pReceiveFrame->byContent[content_position+3],pReceiveFrame->byContent[content_position+2]);
						if(CharToHex(pReceiveFrame->byContent[content_position+5],pReceiveFrame->byContent[content_position+4])!=0xFF)
						g_sRtcManager.m_gRealTimerBK[RTC_YEAR] = CharToHex(pReceiveFrame->byContent[content_position+5],pReceiveFrame->byContent[content_position+4])+2000;
						//WriteRealTime(g_sRtcManager.m_gRealTimer);//GPSʱ����µ�RTC��ʱ����
						RunRTClock(8);//UTCʱ���뱱��ʱ��������8Сʱ
					}
					break;
					default:
					break;
						
		  		}
				
		  }
          /*switch(n_type)
          {
           case 0x01://UTCʱ��
           for(j=0;j<9;j++) 
           {
              //BYTE n=i+j+1;
             GPR.Content_Utc[j] = pReceiveFrame->byContent[i+j+1];//UTCʱ��
           } 
           break;
           case 0x02://��λ״̬
              GPR.Content_State = pReceiveFrame->byContent[i+1];
          break;
          case 0x03://γ��
           for(j=0;j<9;j++) 
           {
              GPR.Content_Latitude[j] = pReceiveFrame->byContent[i+j+1];//UTCʱ��
           }    
           break;
          case 0x04://��γ��γ
              GPR.Latitude_Direction = pReceiveFrame->byContent[i+1];//UTCʱ��
            break;
          case 0x05://����
           for(j=0;j<9;j++) 
           {
              GPR.Content_Longitude[j] = pReceiveFrame->byContent[i+j+1];//UTCʱ��
           }  
           break;
         case 0x06://�����򶫾�
              GPR.Longitude_Direction = (pReceiveFrame->byContent[i+1]);//UTCʱ��
            break;
         case 0x09://UTC����
           for(j=0;j<6;j++) 
           {
              GPR.Content_Date[j] = pReceiveFrame->byContent[i+j+1];//UTCʱ��
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
     WORD wFileNa = 0;//�ļ���
      wFileNa = 12;
      if(m_RWPaSrcObj !=null)
      {
        m_RWPaSrcObj->SendReadPa(wFileNa,0);
        m_RWPaSrcObj = null;
           //��GPS�յ�������ת��������ͨ��
            if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF]))
            {
              MyPrintfD(m_RecFrame.pBuf,100);
            }
      }
  }
  
   
}
#endif



