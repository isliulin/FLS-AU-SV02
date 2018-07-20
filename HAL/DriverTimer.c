#ifndef _DRIVER_TIMER

#define _DRIVER_TIMER
  #include "..\DataStruct.h"
  #include "..\Main.h"
#endif

void TimerAInit(void)
{
    TA0CCR0= 327;     // �������10ms
    TA0CTL = TASSEL_1 + MC_1 + TACLR ;//+ TAIETMA0ģʽ���� ACLKʱ��, ������0��������  
    TA0CCTL0 |= CCIE;
   	TA0CCTL1 = CAP +SCS+CM_1 +CCIE;//ccr1�������岶�� 
   	
	TA1CTL = TASSEL_1 + MC_1 + TACLR ;
	TA1CCTL0 = CAP +SCS+CM_1 +CCIE;//ccr0�������岶�� 
    
//    TA1CTL   = TASSEL__ACLK + TACLR + MC_1; //ѡ��ʱ��ԴΪACLK�����ϼ���ģʽ,MC_1:������ģʽ
//    TA1CCR0 = 3277;                      //100ms��ʱ
//    TA1CCTL0 = CCIE;                //����ʱ���ж�
}

void TimerBInit(void)
{//SYS_CLK    =    32768UL
    TB0CTL   = TBSSEL__ACLK + TBCLR + MC_2+ CNTL__16;  //ѡ��ʱ��ԴΪACLK,���ϼ���ģʽ������������
    TB0CCR0= 33;
	//g_ccr0 = 33;
    TBCCTL0 = CCIE;                  //����ʱ���ж�
}

void TimeAStart(void)
{
    TA1CTL |= MC_1 + TACLR;              //�򿪶�ʱ��A
}
void TimeAStop(void)
{
     TA1CTL &= ~MC0;                     //�رն�ʱ��A     
}
void TimeBStart(void)
{
     TBCTL |= MC_1 + TBCLR;              //�򿪶�ʱ��B
}
void TimeBStop(void)
{
     TBCTL &= ~MC0;                  //�رն�ʱ��B
}


void UserTimerInit(void)
{
  int i;
  
  for(i = 0;i < TIMER_NUM;i++)
  {
    g_sTimer[i].m_TmCount = 0;
    g_sTimer[i].m_TmFlag = OFF;
    g_sTimer[i].m_TmCountBk = 0;
  }  
  Time_10ms_T=0;
  //Time_1s_T =0;
  g_EnAdTmCount = 0;
  g_EnAdTmFlag = OFF;
  if(g_gRunPara[Terminal_Type] == CABLE_TERMINAL_ODU || g_gRunPara[Terminal_Type] == CABLE_TERMINAL_DODU )
  	g_OpenAdDelay = 2;//��ODUʱ�ӳ�2*160ms
  else
  	g_OpenAdDelay = 12;//�������豸ʱ�ӳ�12*160ms,ԭ�����ӳ�2s
}
void InitTimer(void)
{     
  TimerAInit();
  TimerBInit();
  UserTimerInit();
  //OpenTimer(TM_ADCOLT);
  OpenTimer(TM_SAVELOAD);
  OpenTimer(TM_ADBAT,1);//�����ӳ�1s�ɼ����е�ѹ(����ز���ʱ��ʵ�Ǵ�����AD�ɼ�)
  #ifdef ZDSENDADDR
  OpenTimer(TM_SENDADDR);  //������ ģ����άģ�鷢����վ��ַ��5���ӷ�һ��
  #endif
}
void OpenTimer(int TmId,unsigned int TmVal)
{ 
    if(TmVal == 0)
    {
        switch(TmId)
        {
            case TM_ADCOLT://AD�ɼ�ʱ����
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_ADCOL_T];
                break;
            case TM_ADBAT://﮵��AD�ɼ�ʱ����
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_ADBAT_T];
                break;
            //case TM_ADCOLTEN:
             //   g_sTimer[TmId].m_TmCount = 2;//����AD�ɼ�λ���1s��ʼ�ɼ�ż���򿪲ɼ���ض˿ڲɼ�ʱ��ǰ����û�вɼ�����
              //  break;
                //case TM_ADBATTEN: 
                //g_sTimer[TmId].m_TmCount = 2;//����AD�ɼ�λ���1s��ʼ�ɼ�
             //   break;
            case TM_CLOSECHARGE:
                g_sTimer[TmId].m_TmCount = 30*60;//30����
                break;
            case TM_BEAT:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_BEAT_T];
                break;
            case TM_SENDYC:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_SENDYC_T];
                break;
            case TM_Volt:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_valt_T];
                break;
            case TM_ODU_call2:
                if(g_open_oducall==0x55)
                {
                    g_sTimer[TmId].m_TmCount = 20;
                    g_open_oducall=0;
                }
                else
                    g_sTimer[TmId].m_TmCount = 1;
                break;
            case TM_ODU_call:
                g_sTimer[TmId].m_TmCount = 30;
                break;
            case TM_GPRSPWOERDOWN:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_CLOSEGPRSPWR_T];
                break;
            case TM_BATPWOERDOWN:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_CLOSEGPRSPWR_T];
                break;
            case TM_SAVELOAD:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_SAVELOAD_T];
                break;
            case TM_SMSSENDALLOW:
                g_sTimer[TmId].m_TmCount = 60;
                break;
            case TM_WAITXINHAO:
                g_sTimer[TmId].m_TmCount = 150;
                break;
            case TM_SENDSTATUSDELAY:
                g_sTimer[TmId].m_TmCount = (g_gRunPara[RP_COMM_ADDR]%120)*30 + 1; //+1���ֹΪ������Ϊ0;
                break;
            case TM_SENDADDR:
                g_sTimer[TmId].m_TmCount = 300;
                break;
            case TM_SENDFAULTDELAY:
                g_sTimer[TmId].m_TmCount = 60;
                break;
            case TM_SENDSTATE:
                g_sTimer[TmId].m_TmCount = 60;
                break;
            case TM_SNTP_TIME:
                g_sTimer[TmId].m_TmCount = g_gRunPara[RP_SNTP_T]*60;//��λ�Ƿ��ӣ���ʱSNTP��ʱ
                break;
            case TM_GPRS_RW:
                g_sTimer[TmId].m_TmCount = 2;
                break;
            case TM_SMS_RW:
                g_sTimer[TmId].m_TmCount = 900;
                break;
	    	case TM_RECORDED:
		        g_sTimer[TmId].m_TmCount = 15;
                break;
			case TM_SHORT_FLAG:
				g_sTimer[TmId].m_TmCount = 15;
                break;
			case TM_GPS_TIME:
				if(GetRIStatus(RI_GPS_ERR))//gps״̬�쳣����ʱ��gps
					g_sTimer[TmId].m_TmCount = 7200;//2*60*60;//2Сʱ��һ��
				else
					g_sTimer[TmId].m_TmCount = g_gRunPara[RP_GPS_T]*60;
				break;
			case TM_OPENGPS_TIME:
				g_sTimer[TmId].m_TmCount = g_gRunPara[RP_OPENGPS_T];
				break;
            case TM_OPENGPRS_RF:
            	g_sTimer[TmId].m_TmCount = 30;
				break; 
        	case TM_OPENDEG_RF:
            	g_sTimer[TmId].m_TmCount = 30;
				break;
			//case TM_YKTRIGGER:
				//g_sTimer[TmId].m_TmCount = 2;
				//break;
            default:
              break;
          }
    }
    else
    {
		g_sTimer[TmId].m_TmCount = TmVal; 
    }
   g_sTimer[TmId].m_TmCountBk = g_sTimer[TmId].m_TmCount;
}

/*���в�����ʱ�����б仯ʱ���¶�ʱ������ֵ��ʹ��������Ч*/
void RefreshTmCount(unsigned int TmId)
{
    int iRpTmId = 0;
    int iTime = 0;
    
    if(TmId == TM_ADCOLT)
    {
        iRpTmId = RP_ADCOL_T;
    }
    else if(TmId == TM_BEAT)
    {
        iRpTmId = RP_BEAT_T;
    }
    else if(TmId == TM_SENDYC)
    {
        iRpTmId = RP_SENDYC_T;
    }
    else if(TmId == TM_SAVELOAD)
    {
        iRpTmId = RP_SAVELOAD_T;
    }
    else if(TmId == TM_ADBAT)
    {
        iRpTmId = RP_ADBAT_T;
    }
    
    else if(TmId == TM_Volt)
    {
        iRpTmId = RP_valt_T;
    }
    else if(TmId == TM_SNTP_TIME)
    {
        iRpTmId = RP_SNTP_T ;
    }
	else if(TmId ==TM_GPS_TIME)
	{
        iRpTmId = RP_GPS_T ;
    }
	else if(TmId ==TM_OPENGPS_TIME)
	{
		iRpTmId = RP_OPENGPS_T;
	}
    else
        return;
    if(g_gRunPara[iRpTmId] != g_sTimer[TmId].m_TmCountBk)
    {//��ʱ����б仯ʱ���¶�ʱ��ֵ��ʹ��������Ч
        iTime = g_gRunPara[iRpTmId] - (g_sTimer[TmId].m_TmCountBk - g_sTimer[TmId].m_TmCount);
        if(iTime < 0)
            iTime = 1;
        OpenTimer(TmId,iTime);
    }
}
#pragma vector= TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    memcpy(g_RF003_timeInfo.RF003_absolute_time[g_RF003_timeInfo.RF003_num], g_sRtcManager.m_gRealTimer,RTC_NUM*2);//�յ����ߵĶ�ʱ�򱣴浱ǰ�ľ���ʱ��  //memcpy(Recorder_cfg.comtrade_time,g_recorder.absolute_time,RTC_NUM*2);
	if(g_RF003_timeInfo.RF003_num > 0)
	{
		if(g_RF003_timeInfo.RF003_mini_num[g_RF003_timeInfo.RF003_num] < 65534)
	
	    	g_RF003_timeInfo.RF003_mini_num[g_RF003_timeInfo.RF003_num] = g_RF003_timeInfo.RF003_mini_num[g_RF003_timeInfo.RF003_num-1]+1;
	}
	else if(g_RF003_timeInfo.RF003_num == 0)
	{
		if(g_RF003_timeInfo.RF003_mini_num[2] < 65534)
		{
			if(g_RF003_timeInfo.RF003_time_flag ==OFF)
				g_RF003_timeInfo.RF003_mini_num[g_RF003_timeInfo.RF003_num]++;
			else if(g_RF003_timeInfo.RF003_time_flag ==ON)
	    		g_RF003_timeInfo.RF003_mini_num[g_RF003_timeInfo.RF003_num] = g_RF003_timeInfo.RF003_mini_num[2]+1;
		}
	}
	/*g_RF003_timeInfo.RF003_num ++;
	if(g_RF003_timeInfo.RF003_num >2)
		g_RF003_timeInfo.RF003_num =0;*/
	g_RF003_timeInfo.RF003_time_flag =OFF;
	g_OutputInfo.Interrupt_flag1= ON;
	g_OutputInfo.g_buf1[0] = 0xEA;
        //BYTE * p = (BYTE *)g_recorder.absolute_time;
	memcpy(&g_OutputInfo.g_buf1[1],(BYTE *)g_RF003_timeInfo.RF003_mini_num,6);
	memcpy(&g_OutputInfo.g_buf1[7],(BYTE *)&g_RF003_timeInfo.RF003_num,1);	
	memcpy(&g_OutputInfo.g_buf1[8],(BYTE *)g_RF003_timeInfo.RF003_absolute_time[g_RF003_timeInfo.RF003_num],RTC_NUM*2);
	g_RF_pps =0;
    CreatNewSoe(RI_RF_ERR,0);//RF��ʱ�쳣�ָ�
    if(g_RF_Reset == TRUE)
	g_RF_Reset = OFF;
}

#pragma vector= TIMER0_A0_VECTOR     //(53 * 2u)0xFFEA Timer0_A5 CC1-4, TA 
__interrupt void TIMER0_A0_ISR(void)
{//10ms��ʱ��
  int i;
  _EINT();//�����ж�
  Time_10ms_T++;

  if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_ZDDI_MODE]))
  {
	  if(ScanDin() == TRUE)
	    LPM3_EXIT;
  }
  if(g_RF_Reset == ON)//��ʱ10MS��λ
  {
      g_RF_Reset = TRUE;
      WIRE_LESS_RST_H() ;
  }
  if((Time_10ms_T & 0x0f) == 0)
  {//160ms
    LPM3_EXIT; //for clear WatchDog
    if(g_EnAdTmCount > 0)
  	{//ʹ��AD�ɼ�ʱ�䵥λ����Ϊ1s����˷ŵ�160ms��
	    g_EnAdTmCount--;
		if(g_EnAdTmCount == 0)
			g_EnAdTmFlag = ON;
		}
      }
      if(Time_10ms_T>=100)
      {//1s
        Time_10ms_T=0;
		if(g_RF_pps < 310)//RFС��5min�����ǵ������ߵľ������ȣ��˴������10S
    		g_RF_pps ++;
		else	
        {
		  CreatNewSoe(RI_RF_ERR,1);//RF��ʱ�쳣
		  if(g_RF_Reset == OFF)
		  	{
                WIRE_LESS_RST_L(); //��λ����ģ��
                g_RF_Reset = ON;
		  	}
         }
        for(i = 0;i < TIMER_NUM;i++)
        {
            if(g_sTimer[i].m_TmCount > 0)
            {
                g_sTimer[i].m_TmCount--;
                if(g_sTimer[i].m_TmCount == 0)
                {
                    g_sTimer[i].m_TmFlag = ON;
                    LPM3_EXIT;      
                }
            }
        } 
        if(TimeOut(0) == TRUE)
          LPM3_EXIT;
      	//Time_1s_T ++;
	//if(Time_1s_T>=10)//���ϱ����Ӹ�λʱ����S��Ϊ10S
	//{
	  //Time_1s_T =0;
	  	CheckRmtLock();//�жϹ��ϱ���
	//}
        if(g_gRunPara[Terminal_Type] == AERIAL_TERMINAL)  //���ε����ϱ��ܿ�ͨ������
        {
            CheckCommSt();//�ж�ָʾ��ͨ��״̬
        }
        
#if 0
        //for test====
        for(i = 0;i < RM_LOADI_NUM;i++)
        {
          g_gRmtMeas[i+ RM_1A_LOAD_I] = i+g_sRtcManager.m_gRealTimer[RTC_MINUT];
          
        }
        g_ucRefYxYcTs |= BIT1;
        //====
#endif
        
  } 
}

#pragma vector=TIMER0_A1_VECTOR    
__interrupt void TIMER0_A1_ISR(void)//ccr0�Ĳ���
{//10ms��ʱ��
  //int i
  	//_EINT();//�����ж�
  	DWORD TBO_init;
    switch(TA0IV)
    {
      case 0x02:
	  	
  		if((((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.flag == TRUE)//13,+6GPRMC
  		{
			
            if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG]) &&(g_OutputInfo.Interrupt_flag == OFF))//����ʹ��
			{
        		  BYTE bBuf[7] = {0xE6,g_ccr0,g_ccr0>>8,g_sRtcManager.m_gRealTimer[RTC_MICROSEC],g_sRtcManager.m_gRealTimer[RTC_MICROSEC]>>8,g_sRtcManager.m_gRealTimer[RTC_SEC],g_sRtcManager.m_gRealTimer[RTC_MINUT]};
				  memcpy(g_OutputInfo.g_buf,bBuf,6);
				  g_OutputInfo.Interrupt_flag = ON;
				  //MyPrintfD(bBuf,7);
				
			}
			
			if(((g_sRtcManager.m_gRealTimer[RTC_MICROSEC] >10)&&(g_sRtcManager.m_gRealTimer[RTC_MICROSEC] < 990))&&(g_CrystalError.Compen_cry_total >0))//������MS����10����Ϊ����ƫ���
			{
				g_CrystalError.diff_value_num ++;
				if(g_CrystalError.diff_value_num >=3)//����3���ն˵ļ�ʱƫ��ϴ�,���¿�ʼ��ʱҲ���������ˣ����ݼ����ֵ���²���
				{
					g_CrystalError.diff_value_num =0;
					g_CrystalError.Crystal_start_flag=0;
					g_CrystalError.Compen_cry_total =0;
					
				}
			
			}
			else
				g_CrystalError.diff_value_num =0;
			
			CLOSE_GPS_POWER;

			if(g_CrystalError.diff_value_num ==0)//��������Ч��ǿ����
			{
				g_sRtcManager.m_gRealTimerBK[RTC_MICROSEC] = 999;
				memcpy(g_sRtcManager.m_gRealTimer,g_sRtcManager.m_gRealTimerBK,RTC_NUM*2);//g_sRtcManager.m_gRealTimerBK[RTC_YEAR]
				//if(g_ccr0 == 32768)//Ϊ�˱�֤GPS��ʱ��
            	                        {
			  	 	//CLOSE_GPS_POWER;
					//g_gpsPowerSt=0;//zxx
					if(GetRIStatus(RI_GPS_ERR))//gps״̬�쳣��ָ���״̬
					{
						CreatNewSoe(RI_GPS_ERR,0);
						OpenTimer(TM_GPS_TIME);//gps�򿪵�ʱ�����ظ���������
        				OpenTimer(TM_OPENGPS_TIME);//gps�򿪺��������ʱ���ر�gps
					}
					g_GPS_READ_NUM =0; 
			
					if(TB0CCR0 >=TB0R)
						TBO_init = (DWORD)(TB0CCR0-TB0R);
                	                else if((TB0R >60000)&&(TB0CCR0 <35))
                    	                    TBO_init = (DWORD)(TB0CCR0+0x10000-TB0R);
					else//���Ǹ��ж��붨ʱ��B�ж�ͬʱ����ʱ�����
				    	TBO_init = 0;
					//g_sRtcManager.m_gRealTimer[RTC_MICROSEC] = 999;
					if(g_CrystalError.Crystal_start_flag == 1)
					{
						g_CrystalError.Crystal_start_num-=TBO_init;//û�е�ms�ж���������ʱ��Ҫȥ������û���ߵľ������
						g_CrystalError.Crystal_end_num = g_CrystalError.Crystal_start_num;
						g_CrystalError.end_RealTime[0] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
						g_CrystalError.end_RealTime[1] = g_sRtcManager.m_gRealTimer[RTC_SEC];
						g_CrystalError.end_RealTime[2] =  g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
						//g_CrystalError.Crystal_start_flag =0;
						g_CrystalError.Crystal_end_flag = ON;
						g_CrystalError.Crystal_start_num =TBO_init;//0;//���ǴӸ��жϵ�MS�жϵľ������
						g_CrystalError.Compen_cry_num = 0;
						//g_CrystalError.Crystal_start_flag =2;
				  		//BYTE bBuf1[9] = {0xE7,g_CrystalError.Crystal_end_num,g_CrystalError.Crystal_end_num>>8,g_CrystalError.Crystal_end_num>>16,g_CrystalError.Crystal_end_num>>24,g_CrystalError.end_RealTime[2],g_CrystalError.end_RealTime[2]>>8,g_CrystalError.end_RealTime[1],g_CrystalError.end_RealTime[0]};
				 		//memcpy(g_OutputInfo.g_buf1,bBuf1,9);
						//MyPrintfD(bBuf1,9);
					}

					if(g_CrystalError.Crystal_start_flag ==0)
					{
						g_CrystalError.Crystal_start_flag = 1;//���񵽵�һ���������ſ�ʼ�ۼӾ������
						g_CrystalError.Crystal_start_num = TBO_init;//0;
						g_CrystalError.start_RealTime[0] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
						g_CrystalError.start_RealTime[1] = g_sRtcManager.m_gRealTimer[RTC_SEC];
						g_CrystalError.start_RealTime[2] =  g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
						//BYTE bBuf2[5] = {0xE8,g_CrystalError.start_RealTime[2],g_CrystalError.start_RealTime[2]>>8,g_CrystalError.start_RealTime[1],g_CrystalError.start_RealTime[0]};
						//memcpy(g_OutputInfo.g_buf2,bBuf2,5);
					   // MyPrintfD(bBuf2,5);
					}
				}
            	(((CPrtcGPS *)pGps)->pReceiveFrame)->FrameGPR.flag = FALSE;
			}
			//
                      
			//g_ccr0 =0;
			//g_buchangnum =0;
			//g_buchangnum2 =0;
  		}
      break;
      
      default:
      break;
    }
	
}


#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMEB0_ISR(void)
{//1ms
    static unsigned int MicSecCount = 0;  //���ʱ
    static unsigned int RunLedCount = 3000; 
    unsigned  int tmp =0;
    //WDTCTL = WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2;   //����WDT

    //���е�
    if(RunLedCount > 0)
    {
      RunLedCount--;
      if(RunLedCount == 0)
      {
        LIGHT_RUN_UP();
		LIGHT_RUN2_UP();
      }
    }
    //�жϴ��ڱ��ķְ�
    int iCmId = 0;
    for(iCmId = CMID0;iCmId <= CMID3;iCmId++)
    {
      if(g_sRxBuff[iCmId].m_iRcvCount > 0)
      {
        g_sRxBuff[iCmId].m_iRcvCount--;
        if(g_sRxBuff[iCmId].m_iRcvCount == 0)
        {
          g_sRxBuff[iCmId].m_iRcvFlag = ON;
		  sRx_Oper.m_Operalength =0;
		  sRx_Oper.m_OperaBufT=0;
		  sRx_Oper.m_Operaflag =0;
          //printf("%d,%d\n",iCmId,g_sRxBuff[iCmId].m_iBufT);
          LPM3_EXIT;
        }
      }
    }
    
    //����ʵʱʱ��
    g_sRtcManager.m_gRealTimer[RTC_MICROSEC]++;  //ϵͳʵʱʱ�Ӻ����ۼ�
    MicSecCount = g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
	if(MicSecCount >=1000)
	{
		P7OUT ^= BIT6;
		g_buchangnum =0;
		g_buchangnum2 =0;
		
		//MicSecCount = 0;
		g_ccr0 =0;
		
	}
		g_buchangnum ++;
		if(g_buchangnum >= 4)
		{
		 	g_buchangnum2++;
		 	if(g_buchangnum2 >=14)
		 	{
		 		tmp = 33;
				g_ccr0 +=33;
				g_buchangnum2 =0;
				
		 	}
		 	else
		 	{
                tmp = 32;
				g_ccr0 +=32;
				//g_buchangnum2 =0;
		 	}
                       
		 	g_buchangnum =0;
		}
		else
		{
			tmp = 33;
			g_ccr0 +=33;
		}
		if(MicSecCount == 999)
		{
			if(g_sRtcManager.m_gRealTimerFlag ==ON)//��վ�����ʱ����ʱ�������ǿ��
			{
				tmp = 33;
			}
			else
			{
				if(g_ccr0 + tmp > 32768)	
                        {
				tmp += 1;//
                                g_ccr0 +=1;//(32768 - g_ccr0);                    						
                        }
				else
                        {
			    tmp+= (32768 - g_ccr0);//tmp=tmp;
                            g_ccr0 +=(32768 - g_ccr0); 
                        }
              }        
		}
		
		if(g_CrystalError.Compen_cry_total >0)
		{
			g_CrystalError.Compen_cry_num++;//����ಹ��һ������
			if(g_CrystalError.Compen_cry_num >= g_CrystalError.Compen_cry_total)
			{
				g_CrystalError.Compen_cry_num =0;
				if(g_CrystalError.Compen_cry_flag == ON)
					tmp--;
				else if(g_CrystalError.Compen_cry_flag == OFF)
					tmp++;
					
			}
			
                      
		}
                  TB0CCR0 +=tmp;
		
		if(g_CrystalError.Crystal_start_flag == 1)//=0,��һ��������=1�����Լ�������������=2��֮��ʼ����
		{
		  g_CrystalError.Crystal_start_num+=tmp;
		}
		//g_ccr0 +=TB0CCR0;
    //MicSecCount = g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
    if(MicSecCount >= 1000)   //���ʱ
    {
        MicSecCount = 0;
        if(RunLedCount==0)
        { 
            RunLedCount = 100;
            LIGHT_RUN_DOWN();
			LIGHT_RUN2_DOWN();
        }
        g_sRtcManager.m_gRealTimer[RTC_SEC]++;
        g_sRtcManager.m_gRealTimer[RTC_MICROSEC] = 0;

        if(g_sRtcManager.m_gRealTimer[RTC_SEC] >= 60)     //���Ӽ�ʱ
        {
            g_sRtcManager.m_gRealTimer[RTC_SEC] = 0;
            g_sRtcManager.m_gRealTimer[RTC_MINUT]++;
			//g_validity

			
            if((g_sRtcManager.m_gRealTimer[RTC_MINUT] & 3) == 0)  //Ϊ�˾����ܼ��ٶ�ʱʱ���ϵĲ����ʱʱ����5���Ӹ�Ϊ4����
            {
                g_sRtcManager.m_ucRtcSynFlag = YES;
            }
			if(((g_sRtcManager.m_gRealTimer[RTC_MINUT] % 15) == 0)&&(g_sRtcManager.m_gRealTimer[RTC_MINUT] < 60))
			{
				g_sRtcManager.m_FixYCFlag =YES;
			}
            if(g_sRtcManager.m_gRealTimer[RTC_MINUT] >= 60)
            {
                g_sRtcManager.m_gRealTimer[RTC_MINUT] = 0;
                g_sRtcManager.m_gRealTimer[RTC_HOUR]++;
				if(g_sRtcManager.m_gRealTimer[RTC_HOUR] >=24)
				{
					g_sRtcManager.m_gRealTimer[RTC_HOUR] =0;
					g_sRtcManager.m_gRealTimer[RTC_DATE]++;
				}
				if((g_sRtcManager.m_gRealTimer[RTC_HOUR] < 24) &&(g_sRtcManager.m_gRealTimer[RTC_HOUR] > 0))
					g_sRtcManager.m_FixYCFlag =YES;
				else
					g_sRtcManager.m_FixYCFlag =ON;//Сʱ����24Сʱ�������Ӽӣ�ͬʱ��ȡң���¼����
                g_StatusTime = g_sRtcManager.m_gRealTimer[RTC_HOUR];
                if(g_StatusTime == g_gRunPara[RP_STATUSRP_TIME])
                {
                    if(g_DateCount)
                    {
                        g_DateCount--;
                        if(!g_DateCount)
                        {
                            OpenTimer(TM_SENDSTATUSDELAY);
                            g_DateCount = g_gRunPara[RP_STATUSRP_INTERVAL];
                        }
                    }
                }
            }
        }
		if(g_sRtcManager.m_gRealTimerFlag ==ON)
		{
		    g_sRtcManager.m_gRealTimerFlag =TRUE;//��ѭ����дRTCоƬ��ʱ��
			
		}
        
    }
    
          
}
