#ifndef _DRIVER_RTC

#define _DRIVER_RTC
  #include "..\DataStruct.h"
#endif

#define WRITEADD   0x64
#define READADD    0x65
#define SECADDR    0x00
#define WORDADD1   0xe0
const INT8U Month_Table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//==============================================================================
//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
//==============================================================================
INT8U Is_Leap_Year(INT16U year)

{                     

       //if(year%4==0) //�����ܱ�4����
       if(year&0xFFFC==0) //�����ܱ�4����
       { 

              if(year%100==0) 

              { 

                     if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400����          

                     else return 0;   

              }else return 1;   

       }else return 0; 

}  



//==============================================================================
//  ��������   : RunRTClock
//  ��������   : ʵʱʱ�ӵ��������£�ͨ���ڶ�ʱ�������е��øú�����ʵ�ֶ������Ķ�ʱ����
//  �������   : uint16 AddHour ��λ�̶ȵ�Сʱ���ӣ����ú������õ�����
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//==============================================================================
void RunRTClock(INT16U AddHour)
{
          g_sRtcManager.m_gRealTimerBK[RTC_HOUR]+=AddHour;
          if(g_sRtcManager.m_gRealTimerBK[RTC_HOUR] > 23)//�ۼӴﵽ1day
          {
              g_sRtcManager.m_gRealTimerBK[RTC_HOUR] = 0;
              g_sRtcManager.m_gRealTimerBK[RTC_DATE]++;
              if((g_sRtcManager.m_gRealTimerBK[RTC_MONTH] == 2)&&Is_Leap_Year(g_sRtcManager.m_gRealTimerBK[RTC_YEAR]))//�����2�£����ж�2�£����ж����꣬���ִ��Ч��
              {
                  if(g_sRtcManager.m_gRealTimerBK[RTC_DATE] > 29)//����2��
                  {
                      g_sRtcManager.m_gRealTimerBK[RTC_DATE] = 1;                 
                  }
              }
              else
              {                  
                  if(g_sRtcManager.m_gRealTimerBK[RTC_DATE] > Month_Table[g_sRtcManager.m_gRealTimerBK[RTC_MONTH] - 1])//�������ֱ��ʹ��Ĭ���·�����
                  {
                      g_sRtcManager.m_gRealTimerBK[RTC_DATE] = 1; 
                  }
              }
              if(g_sRtcManager.m_gRealTimerBK[RTC_DATE] == 1)//�����ϳ���������ڴ˴�����Ϊ1��϶���Ӧ�����·ݵ����ӣ�
              {
                  g_sRtcManager.m_gRealTimerBK[RTC_MONTH]++;
                  if(g_sRtcManager.m_gRealTimerBK[RTC_MONTH] > 12)
                  {
                      g_sRtcManager.m_gRealTimerBK[RTC_MONTH] = 1;
                      g_sRtcManager.m_gRealTimerBK[RTC_YEAR]++;
                  }                    
              }
          }
}



INT8U Hex2Bcd(INT8U h)
{
	unsigned char x;
	x=(h/10)*16+h%10;
	return x;
}

void InitRtc(void)
{
   //_DINT();
   WDTCTL = WDTPW + WDTHOLD;               ////��ֹ�ڲ����Ź�
   FEED_WATCH_DOG();                           ////���ⲿ���Ź�
   I2C_start();
   FEED_WATCH_DOG();    
   I2C_senddata(WRITEADD);
   FEED_WATCH_DOG(); 
   I2C_ack();
   FEED_WATCH_DOG(); 
   I2C_senddata(WORDADD1);
   FEED_WATCH_DOG(); 
   I2C_ack();
   FEED_WATCH_DOG(); 
   if(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_START_TIME-16]))
   	I2C_senddata(0x20);         //20����Ϊ24Сʱ��,���жϷ�ʽ
   else
	I2C_senddata(0x23);         //20����Ϊ24Сʱ��,�����������
   FEED_WATCH_DOG(); 
   I2C_ack();
   I2C_stop();
   FEED_WATCH_DOG();
   
   //_EINT();
   return; 
}

//==============================================================================
//  ��������   : WriteRTCTime
//  ��������   : ��վ�·���ʱ����󣬵ȵ���MS=1000ʱ��дRTCоƬ��ʱ�䣬�������Ա�֤RTCоƬ��ʱ������վ��ʱ������ܴ�
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void WriteRTCTime(void)
{
	unsigned int unHYM[7];
      
    unHYM[0] = g_sRtcManager.m_gRealTimer[RTC_YEAR]-2000;
    unHYM[1] = g_sRtcManager.m_gRealTimer[RTC_MONTH];
    unHYM[2] = g_sRtcManager.m_gRealTimer[RTC_DATE];
    unHYM[3] = g_sRtcManager.m_gRealTimer[RTC_WEEK];
    unHYM[4] = g_sRtcManager.m_gRealTimer[RTC_HOUR];  
    unHYM[5] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
    unHYM[6] = g_sRtcManager.m_gRealTimer[RTC_SEC];
      //unHYM[7] = MSecond%1000;
    
     WriteRealTime(unHYM);  //�޸�ʱ��
     if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
	 {
        	BYTE bBuf[7] = {0xAB,0XAB,unHYM[4],unHYM[5],unHYM[6],g_sRtcManager.m_gRealTimer[RTC_MICROSEC],g_sRtcManager.m_gRealTimer[RTC_MICROSEC]>>8};
        	MyPrintfD(bBuf,7);
	 }
}
//==============================================================================
//  ��������   : ReadRealTime
//  ��������   : ��ʱ��оƬ�ж�ȡϵͳʱ��
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
unsigned int* ReadRealTime(unsigned char type)
{
  INT8U  temp;//,a,b;
    INT8U  Time[7]; //Second,Minute,Hour,Week,Day,Month,Year

    unsigned int  unHYM[8];
    if((!(g_gRunPara[RP_CFG_KEYH] & BIT[RP_set_TIME-16]))||(GetRIStatus(RI_GPS_ERR))||(type ==2)) //gps��rtc�Զ��л�ÿ4���Ӵ�ʱ��оƬ��ȡһ��ʱ�䣬�ñ�־�ڶ�ʱ���ж��м������־
    {
	//_DINT();
    WDTCTL = WDTPW + WDTHOLD;               ////��ֹ�ڲ����Ź�
    FEED_WATCH_DOG();                            ////���ⲿ���Ź�
    I2C_start();
    FEED_WATCH_DOG(); 
    I2C_senddata(WRITEADD);I2C_ack();
    FEED_WATCH_DOG(); 
    I2C_senddata(0x04);I2C_ack();
    FEED_WATCH_DOG();         
    temp = (I2C_receivedata() & 0x7F);
    Time[0]=(temp >> 4) * 10 + (temp & 0x0f);//Second 

    I2C_sendack();
    FEED_WATCH_DOG(); 
    temp = (I2C_receivedata() & 0x7F);

    Time[1]=(temp >> 4) * 10 + (temp & 0x0F);I2C_sendack();	// Minute   
    FEED_WATCH_DOG(); 
    temp = (I2C_receivedata() & 0x3F);//

    Time[2] =(temp >> 4) * 10 + (temp & 0x0F);I2C_sendack(); //  Hour
    
    FEED_WATCH_DOG(); 
    temp = (I2C_receivedata() & 0x07);//;

    Time[3] =(temp >> 4) * 10 + (temp & 0x0F);I2C_sendack();	   
    
    FEED_WATCH_DOG();    
    temp = (I2C_receivedata()&0x3f);// 

    Time[4] =(temp >> 4) * 10 + (temp & 0x0F);I2C_sendack();   
    
    FEED_WATCH_DOG();     
    temp = (I2C_receivedata()&0x1f);//;

    Time[5] =(temp >> 4) * 10 + (temp & 0x0F);I2C_sendack();	 
    
    FEED_WATCH_DOG(); 
    temp = (I2C_receivedata() & 0xFF);//;

    Time[6] =(temp >> 4) * 10 + (temp & 0x0F); 

    I2C_sendnotack();
    FEED_WATCH_DOG(); 
    I2C_stop();
    FEED_WATCH_DOG();  
    //_EINT();
    if((Time[0] > 59) || (Time[1] > 59) || (Time[2] > 23)  \
      || (Time[5] < 1) || (Time[5] > 12) || (Time[6] > 40) || (Time[6] < 6))
    {
            InitRtc();
            g_sRtcManager.m_gRealTimer[RTC_SEC]   = 7;
            g_sRtcManager.m_gRealTimer[RTC_MINUT] = 8;
            g_sRtcManager.m_gRealTimer[RTC_HOUR]  = 9;
            g_sRtcManager.m_gRealTimer[RTC_WEEK] = 3;
            g_sRtcManager.m_gRealTimer[RTC_DATE]  = 10;
            g_sRtcManager.m_gRealTimer[RTC_MONTH] = 11;
            g_sRtcManager.m_gRealTimer[RTC_YEAR]  = 12+2000;
            unHYM[0] = g_sRtcManager.m_gRealTimer[RTC_YEAR]-2000;
            unHYM[1] = g_sRtcManager.m_gRealTimer[RTC_MONTH];
            unHYM[2] = g_sRtcManager.m_gRealTimer[RTC_DATE];
            unHYM[3] = g_sRtcManager.m_gRealTimer[RTC_WEEK];
            unHYM[4] = g_sRtcManager.m_gRealTimer[RTC_HOUR];  
            unHYM[5] = g_sRtcManager.m_gRealTimer[RTC_MINUT];
            unHYM[6] = g_sRtcManager.m_gRealTimer[RTC_SEC];
            unHYM[7] = g_sRtcManager.m_gRealTimer[RTC_MICROSEC];
            WriteRealTime(unHYM);
    }
    else
    {
        g_sRtcManager.m_gRealTimer[RTC_SEC]   =Time[0];
        g_sRtcManager.m_gRealTimer[RTC_MINUT] =Time[1];
        g_sRtcManager.m_gRealTimer[RTC_HOUR]  =Time[2];
        g_sRtcManager.m_gRealTimer[RTC_WEEK]  =Time[3];
        g_sRtcManager.m_gRealTimer[RTC_DATE]  =Time[4]; 
        g_sRtcManager.m_gRealTimer[RTC_MONTH] =Time[5];
        g_sRtcManager.m_gRealTimer[RTC_YEAR]  =Time[6]+2000;
		if(type &&(g_sRtcManager.m_gRealTimer[RTC_HOUR]==0)&&(g_sRtcManager.m_gRealTimer[RTC_MINUT] ==0))
		{
			g_sRtcManager.m_FixYCFlag = ON;
		}
		else if(type && (g_sRtcManager.m_gRealTimer[RTC_MINUT] % g_gRunPara[RP_SAVELOAD_T] == 0))
		{
			g_sRtcManager.m_FixYCFlag =YES;
		}
        if(type && (g_StatusTime != g_sRtcManager.m_gRealTimer[RTC_HOUR]))
        {
            g_StatusTime = g_sRtcManager.m_gRealTimer[RTC_HOUR];
            if((g_StatusTime == g_gRunPara[RP_STATUSRP_TIME]) && g_DateCount)
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
	if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
	 {
        	BYTE bBuf[7] = {0xAC,0XAC,Time[2],Time[1],Time[0],g_sRtcManager.m_gRealTimer[RTC_MICROSEC], g_sRtcManager.m_gRealTimer[RTC_MICROSEC]>>8};
        	MyPrintfD(bBuf,7);
	 }
    return g_sRtcManager.m_gRealTimer;
}

//==============================================================================
//  ��������   : WriteRealTime
//  ��������   : ��ʱ��оƬд��ϵͳʱ��
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================

void WriteRealTime(unsigned int *RealTime)
{
  INT8U mixbcd,tt;
   if((!(g_gRunPara[RP_CFG_KEYH] & (BIT[RP_set_TIME-16])))||(GetRIStatus(RI_GPS_ERR)))//gpsʧЧ����վ��ʱû�йر�ʱ��������վ��ʱ
   {
    //_DINT();
    WDTCTL = WDTPW + WDTHOLD;               ////��ֹ�ڲ����Ź�
   FEED_WATCH_DOG();                             ////���ⲿ���Ź�
	//WatchDogClose();
    I2C_start();
    
    I2C_senddata(WRITEADD);
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
    I2C_senddata(SECADDR);
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();

	//WatchDogClose();
    tt=RealTime[6];
    mixbcd=Hex2Bcd(tt);//& 0x7f
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);	    //SysTime.Second	��
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
  
	//WatchDogClose();
    mixbcd=Hex2Bcd(RealTime[5]);//Hex2Bcd()& 0x7f;
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
    
    mixbcd=Hex2Bcd(RealTime[4]);  //Hour Hex2Bcd	 & 0x3f
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);		   
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
    
    mixbcd=Hex2Bcd(RealTime[3]);//Week Hex2Bcd & 0x07
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);//3
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();

	//WatchDogClose();
    mixbcd=Hex2Bcd(RealTime[2]) ;//Day Hex2Bcd& 0x3f		   
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);//		   
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
    mixbcd=Hex2Bcd(RealTime[1]);//Month Hex2Bcd	 & 0x1f	   
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
    
    mixbcd=Hex2Bcd(RealTime[0]& 0xff);//Year Hex2Bcd & 0xff
    FEED_WATCH_DOG();
    I2C_senddata(mixbcd);	//	��
    FEED_WATCH_DOG();
    I2C_ack();
    FEED_WATCH_DOG();
    I2C_stop();
    FEED_WATCH_DOG();    
    //_EINT();
   	}
}
//==============================================================================
//  ��������   : TimeAdd
//  ��������   : ʱ��ӷ��������û�����ms���ʱ��
//  �������   : pRtClock: ��ǰʱ��
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void TimeAdd(WORD *pRtClock,WORD wMs,BYTE bySec,BYTE byMin,BYTE byHour,BYTE byDay,BYTE byMonth,WORD wYear)
{
        BYTE byDayNext;//����λ���·ݶ�����
        pRtClock[RTC_MICROSEC] += wMs;
        if(pRtClock[RTC_MICROSEC] >= 1000)//�ۼӴﵽ1s
        {
            pRtClock[RTC_MICROSEC] = pRtClock[RTC_MICROSEC] - 1000;
            bySec++;
        }
        
        pRtClock[RTC_SEC] += bySec;//ȡ��
        if(pRtClock[RTC_SEC] > 59)//�ﵽ1m
        {
            pRtClock[RTC_SEC] = pRtClock[RTC_SEC] - 60;
            byMin++;
        }
        
        pRtClock[RTC_MINUT] += byMin;
        if(pRtClock[RTC_MINUT] > 59)//�ۼӴﵽ1hour
        {
            pRtClock[RTC_MINUT] = pRtClock[RTC_MINUT] - 60;
            byHour++;
        }
        
        pRtClock[RTC_HOUR] += byHour;
        if(pRtClock[RTC_HOUR] > 23)//�ۼӴﵽ1day
        {
            pRtClock[RTC_HOUR] = pRtClock[RTC_HOUR] - 24;
            byDay++;
        }
        
        pRtClock[RTC_DATE] += byDay;
        if((pRtClock[RTC_MONTH] == 2)&&Is_Leap_Year(pRtClock[RTC_YEAR]))//�����2�£����ж�2�£����ж����꣬���ִ��Ч��
        {
            byDayNext = 29;
        }
        else
        {   
            byDayNext = Month_Table[pRtClock[RTC_MONTH] - 1];
        }
        if(pRtClock[RTC_DATE] > byDayNext)
        {
            pRtClock[RTC_DATE] = pRtClock[RTC_DATE] - byDayNext; 
            byMonth++; 
        }
        
        pRtClock[RTC_MONTH] += byMonth;
        if(pRtClock[RTC_MONTH] > 12)
        {
              pRtClock[RTC_MONTH] = pRtClock[RTC_MONTH] - 12;
              wYear++;  
        }  
        pRtClock[RTC_YEAR] += wYear;
}
//==============================================================================
//  ��������   : TimeSub
//  ��������   : ʱ������������û�����ms���ʱ��
//  �������   : pRtClock: ��ǰʱ��
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void TimeSub(WORD *pRtClock,WORD wMs,BYTE bySec,BYTE byMin,BYTE byHour,BYTE byDay,BYTE byMonth,WORD wYear)
{
        BYTE byDayLast;//������·ݶ�����
        if(pRtClock[RTC_MICROSEC] < wMs)//��ǰms��������ȡs
        {
            pRtClock[RTC_MICROSEC] = pRtClock[RTC_MICROSEC] + 1000 - wMs;
            bySec++;//��+1�൱����ǰ�����1
        }
        else
        {
            pRtClock[RTC_MICROSEC] -= wMs;
        }
        
        if(pRtClock[RTC_SEC] < bySec)//��ǰ�벻������ֽ�
        {
            pRtClock[RTC_SEC] = pRtClock[RTC_SEC] + 60 - bySec;
            byMin++;
        }
        else
        {
            pRtClock[RTC_SEC] -= bySec;
        }

        if(pRtClock[RTC_MINUT] < byMin)//��ǰ�ֲ�������Сʱ��
        {
            pRtClock[RTC_MINUT] = pRtClock[RTC_MINUT] + 60 - byMin;
            byHour++;
        }
        else
        {
            pRtClock[RTC_MINUT] -= byMin;
        }

        if(pRtClock[RTC_HOUR] < byHour)//��ǰСʱ�����������ڽ�
        {
            pRtClock[RTC_HOUR] = pRtClock[RTC_HOUR] + 24 - byHour;
            byDay++;
        }
        else
        {
            pRtClock[RTC_HOUR] -= byHour;
        }

        if(pRtClock[RTC_DATE] <= byDay)//DAY������ day= 1->28/30/31
        {
            if(Is_Leap_Year(pRtClock[RTC_YEAR]))
            {
                if(pRtClock[RTC_MONTH] - 1 == 2)//����2��
                {
                    byDayLast = 29;
                }
            }
            else
            {
                byDayLast = Month_Table[pRtClock[RTC_MONTH] - 1];
            }
            pRtClock[RTC_DATE] = pRtClock[RTC_DATE] + byDayLast - byDay;
            byMonth++;
        }
        else
        {
            pRtClock[RTC_DATE] -= byDay;
        }

        if(pRtClock[RTC_MONTH] <= byMonth)//�·ݲ�����
        {
            pRtClock[RTC_MONTH] = pRtClock[RTC_MONTH] + 12 - byMonth;
            wYear++;
        }
        else
        {
            pRtClock[RTC_MONTH] -= byMonth;
        }
        
        if(pRtClock[RTC_YEAR] > wYear)
            pRtClock[RTC_YEAR] -= wYear;
}


