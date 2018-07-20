#ifndef _DRIVER_IO

#define _DRIVER_IO
#include "..\DataStruct.h"

#include "HAL_UCS.h"
#include "HAL_PMM.h"

#endif


#define   ENABLE_P1INT()               P1IFG &=~(BIT0); P1IES|= BIT0;P1IE |= (BIT0);
#define   DISABLE_P1INT()              P1IE &= ~(BIT0 );

int status;
//==============================================================================
//  函数名称   : InitClk
//  功能描述   : 系统用到的时钟进行初始化
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitClk(void)
{ 
//   unsigned int i;  
//    P5SEL |= 0x0C;                            // Port select XT2 
//    UCSCTL0 = 0x1F00;                           // Set lowest possible DCOx, MODx   *注：DCO =31 配合DCO更改为24M
//  
//    UCSCTL1 = DCORSEL_5;                      // Select range for 18MHz operation
//    UCSCTL2 = 732;                            // Set DCO Multiplier for 16MHz
//                                            // (N + 1) * FLLRef = Fdco
//                                            // (733 + 1) * 32768 = 24MHz
//    UCSCTL6 &= ~XT2OFF;                       // Enable XT2 even if not used
// //发现XT1晶振报错，由于晶振未使用，加程序软件关闭XT1  
//    UCSCTL6 |= XT1OFF;                     
//    UCSCTL3 |= SELREF_2;                     // FLLref = REFO
//                                            // Since LFXT1 is not used,
//                                            // sourcing FLL with LFXT1 can cause
//                                            // XT1OFFG flag to set
//    UCSCTL4 |= SELA_3;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO
//    UCSCTL6 |= XT2BYPASS;
//    UCSCTL6 &= ~(XT2DRIVE_2);
//  // Loop until XT2 & DCO stabilize
//    do
//    {
//        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
//                                            // Clear XT2,XT1,DCO fault flags
//        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
//        for(i=0;i<0xFFFF;i++);                  // Delay for Osc to stabilize
//    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
//    UCSCTL4 |= SELS_4 + SELM_4+SELA_5;   
//  
#if 1
 UCSCTL3 = SELREF_2;                                        //FLL = REFCLK  32768kHz
    UCSCTL4 = SELS__DCOCLK + SELA__XT1CLK + SELM__DCOCLK;
    __bis_SR_register(SCG0);
    UCSCTL0 = 0;
    UCSCTL1 = DCORSEL_5;
    UCSCTL2 = FLLD_0 + 243;   
    P7SEL |= 0x03;                            // Select XT1
    UCSCTL6 &= (~XT1OFF);//8MHz
    UCSCTL6 |= XCAP_3; 
    __bic_SR_register(SCG0);   
   do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
    WatchDog();
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive
#endif                                     // strength   
#if 0
  SetVCore(PMMCOREV_0);                     // Set VCore = 0 for 8MHz clock
  
  P7SEL |= BIT0 + BIT1;                     // Port select XT1
    //Startup LFXT1 32 kHz crystal
  do{
    status = LFXT_Start_Timeout(XT1DRIVE_0, 50000);
     WatchDog();
  } while(status == UCS_STATUS_ERROR);
  
   SELECT_ACLK(SELA__XT1CLK);                // Select XT1 as ACLK source
                          
  SELECT_FLLREF(SELREF__REFOCLK);           // Set DCO FLL reference = REFO
 
  Init_FLL_Settle(8000,244); 
  
  
  //SFRIE1 |= OFIE;   
#endif
  
}
void IoInit(void)
{    
    //P1SEL = 0x00;   //1.0-1.7采集单点遥信，全部配置为输入
    //P1DIR = 0x04;
        P1SEL = 0x04;//1.2-GPS秒脉冲输出 1.3-8025固定周期中断1.2-1.4预留 1.51.6-加密芯片相关1.7-GPS控制开关
	P1DIR = 0xE0;
	P1OUT = 0x00;
    P1OUT |= BIT5;
    P1OUT &= ~BIT6;
    
    //P2SEL = 0x00;   //2.0-2.7采集单点遥信，全部配置为输入
    //P2DIR = 0x00;
    P2SEL = 0x02;//2.0-E2片选 2.1-无线输出秒冲到终端2.2-FLASH_WR 2.3-EPROM_WR 2.4-FLASH片选2.5 FLASH_HOLD 2.6与2.7预留
	P2DIR = 0x3D;
	P2OUT = 0x20;
	
    
    
    //P3SEL = 0x3E;  //3.0- flash hold 3.1-SIMO 3.2-SOMI 3.3-CLK;3.4-TX0 3.5-RX0 3.6 预留;3.7 预留
    //P3DIR = 0xDB;
    //P3OUT = 0x01; //flash hold 需为高电平
    P3SEL = 0x3E;  //3.0- 预留 3.1-SIMO 3.2-SOMI 3.3-CLK;3.4-TX0 3.5-RX0 3.6 CS1;3.7 CS0
    P3DIR = 0x1A;
    P3OUT = 0x00; 
    
    //P4SEL = 0x00;  //4.0-RF电源 4.1-RF复位 4.2-RF测试(in) 4.3-GPRS电源  4.4-4.5 DI输入 4.6-BAT充电 4.7-BAT放电
    //P4DIR = 0xCB;
    //P4OUT = 0x00;
	
	P4SEL = 0x00;  //4.0-4.2预留 4.3-RF复位 4.4-RF测试(in) 4.5-同组触发4.6-GPRS电源	4.7-run
	P4DIR = 0xE8;
	P4OUT = 0x00;

	
    //P5SEL = 0xC0;  //P56 TXD1  P57 RXD1 和GPRS通讯  5.4 DI输入 5.5-电池活化输出
    //P5DIR = 0x6F;
    //P5OUT = 0x00;

	P5SEL = 0xC0;  //P56 TXD1  P57 RXD1 P55 -BAT充电 5.4 WDI 5.3-5.0预留
    P5DIR = 0x70;
    P5OUT = 0x00;
    
    //P6SEL = 0x0F;  //P60-P63采集AD量 P60-BATFU-AD0 P61-SAM_C P62-SAM_SUN P63-SAM_BAT P64-复位GPRS模块  P65-GPRS模块配置  P66-G_ON/OFF 打开/关闭 P67 电池负电压测试
    //P6DIR = 0xFF;
    //P6OUT = 0x00;

	P6SEL = 0x0E;  //P60-P63采集AD量 P60-预留 P61-SAM_C P62-SAM_SUN P63-SAM_BAT P64-继电器控制输出 P65-继电器控制输出  P66-架空24V电源输出 P67 预留
    P6DIR = 0x7E;
    P6OUT = 0x30;
     
    //P7SEL = 0x00; //7.2-flash_wp 7.3-eeprom_wp 7.5-WD 7.4-RUN LED
    //P7DIR = 0xFF;
    //P7OUT = 0x00;

	
	P7SEL = 0x00; //7.2-预留 7.3-预留 7.4 7.5 7.6-DIO 7.7-RUN2
	P7DIR = 0xC3;
	P7OUT = 0x00;	 

    //P8SEL = 0x00; //8.0-8.2  8.4-8.5 遥信输入 P8.3 时钟中断 P8.6 硬遥信电源状态监测  P8.7 继电器控制输出
    //P8DIR = 0x80; 
    //P8OUT = 0x80;  
	
	P8SEL = 0x00; //8.0-8.7 预留
	P8DIR = 0x00; 
	P8OUT = 0x00;	
	

    //P9SEL = 0x30; //P94 TXD2  P95 RXD2 9.1-9.2 I2C 9.0 继电器控制输出 9.3 架空24V电源输出
   //P9DIR = 0xDF;
    //P9OUT = 0x01; 
    
	P9SEL = 0x30; //P94 TXD2  P95 RXD2 9.1-9.2 I2C 9.0 9.3 9.6 9.7预留
	P9DIR = 0x16;
	P9OUT = 0x00;	 

	

    //P10SEL = 0x30;  //P10.0-10.2 AD采样控制 P10.3 硬遥信电源控制 P10.4 TXD3  P10.5 RXD3 设置口使用  
    //P10DIR = 0xDF;
    //P10OUT = 0x00;

	P10SEL = 0x30;  //P10.0-10.2 AD采样控制 P10.3 GPRS电源 P10.4 TXD3  P10.5 RXD3 设置口使用  
    P10DIR = 0x1F;
    P10OUT = 0x00;
    
	P11SEL = 0x07;  //P10.0-10.2 AD采样控制 P10.3 硬遥信电源控制 P10.4 TXD3  P10.5 RXD3 设置口使用  
    P11DIR = 0x07;
    P11OUT = 0x00;
	ENABLE_P1INT();
}


#pragma vector=PORT1_VECTOR                           
__interrupt void P1_ISR (void)
{
    LPM3_EXIT;
	//unsigned char temp;
  //  INT16U DimLen;
    if((P1IFG & BIT0) == BIT0)
    {
    	//ReadRealTime(1);
       P1IFG &=0xFE;
        //temp = rtcReadRegister();
        
        //delay(20);//读写之间加延时 
		/*if((temp & 0x04) == 0x04)	
		{	 
			temp &= 0xFb;
			
            BYTE byBuf1[2] = {0X5A,0X5A};  //测试使用
            CommSendData(byBuf1,2,g_CmIdDBG);
			
			//OpenTimer(TM_RTCDELAY);
			//g_gwriteRTC = ON;
			g_gRTCtype = temp;
			//WriteRtc(temp);
			//rtcWriteRegister(0xF0, &temp,1);	
		}*/
		
    }
    
           
}
//==============================================================================
//  函数名称   : InitDin
//  功能描述   : 初始化开关量输入扫描
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitDin(void)
{
	#ifdef LOCALYX
    unsigned long i,biti;
    unsigned long ExtIoStat = 0;//当前24个外部DIN输入的状态 
    unsigned int ExtIo1=0,ExtIo2=0;
   
	OPEN_DI_POWER();
     for(i=0;i<1000;i++)
     {
            if((READ_DI_POWER_ST) < 1) 
            {  
              break;
            }
     }
	if((READ_DI_POWER_ST) < 1)
	{
		delay(300);
        P1SEL = 4;    //YX00~YX07
    	P1DIR = 0x00;

    	P2SEL = 0;    //YX08~YX15
    	P2DIR = 0;

		P4SEL &= ~(0x30);    //P2.4~P2.7  YX16~YX19 
    	P4DIR &= ~(0x30);

		P5SEL &= ~(0x10);    //P2.4~P2.7  YX16~YX19 
    	P5DIR &= ~(0x10);
    
    	P8SEL &= ~(0x37);    //P2.4~P2.7  YX20~YX24 
    	P8DIR &= ~(0x37);

    
    //从CPU管脚提取对应DIN的IO管脚电平  
      	ExtIo1 = (P1IN & BIT0) +((P1IN & BIT2)>>1)+((P1IN & BIT4)>>2)+((P1IN & BIT6)>>3)+((P2IN & BIT0)<<4)
      	+((P2IN & BIT2)<<3)+((P2IN & BIT4)<<2)+((P2IN & BIT6)<<1); 
	
		ExtIo2=((P4IN & BIT4)>>4) +((P5IN & BIT4)>>3)+((P8IN & BIT1)<<1)+((P8IN & BIT4)>>1)+((P1IN & BIT1)<<3)
      	+((P1IN & BIT3)<<2)+((P1IN & BIT5)<<1)+(P1IN & BIT7); 
		ExtIo1=ExtIo1+(ExtIo2<<8);
      	ExtIoStat = ((P2IN & BIT1)>>1) +((P2IN & BIT3)>>2)+((P2IN & BIT5)>>3)+((P2IN & BIT7)>>4)+((P4IN & BIT5)>>1)
      	+((P8IN & BIT0)<<5)+((P8IN & BIT2)<<4)+((P8IN & BIT5)<<2); 
      	ExtIoStat = ((ExtIoStat << 16) & 0x00ff0000) + ExtIo1;//把24个DIN的IO状态依次赋值到该变量 
		ExtIoStat = ExtIoStat | 0xff000000;
        g_gExtDinStat = ~ExtIoStat;
		
		
     	
      	g_ucRefYxYcTs |= BIT0;
		for(i = 0; i < EXT_DIN_NUM; i++)
      	{
			if(((g_gRunPara[YX_DSl]+g_gRunPara[YX_DSh]*65536)&((unsigned long)1<<i))>0)//双点
			{
				if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 1 )
				{
								//CreatNewSoe(RI_SD_YX1 + i/2,(2),1);				
					SetRIStatus(RI_ZD_DYX1 +i/2,2);
							
				}
							
				else if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 2 )
            	{
                    		
					SetRIStatus(RI_ZD_DYX1 +i/2, 1);
													
				}//分
            	else if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 0 )
            	{
                    			
					SetRIStatus(RI_ZD_DYX1 +i/2, 0);	
									
								
				}//过渡
            	else 
            	{
					SetRIStatus(RI_ZD_DYX1 +i/2, 3);	
														
				}  //错误
			}
			else
			{
				 biti = 1; //原因是直接biti=1<<i,当i=16时出问题 
          		biti = biti << i;
          		if( (g_gExtDinStat  & biti) == biti)//为产生DIN的电平
          		{
              		SetRIStatus(RI_ZD_YX1 +i,1);
          		}
         	 	else
          		{
              		SetRIStatus(RI_ZD_YX1 +i, 0);
          		}
			}
			
		}
		
      	/*g_ucRefYxYcTs |= BIT0;
     	g_cRMTInfoFlag = 0;
     	g_gRMTInfoBackTail = 0;
     	//g_gRMTInfoBackHead = 0;*/
	}
	CLOSE_DI_POWER();
#endif
}


//==============================================================================
//  函数名称   : ScanDin
//  功能描述   : 开关量输入扫描函数，并根据扫描的结果产生对应的SOE和遥信变位信息
//  输出参数   : 
//  返回值     : 1=有变位 0=无变位
//  其他说明   : 
//  作者       ：
//==============================================================================
bool ScanDin(void)
{
	#ifdef LOCALYX
    unsigned long i,m,n,ExtIoStat = 0;//当前24个外部DIN输入的状态
    volatile unsigned int ExtIo1=0,ExtIo2=0,j;
    unsigned int f1_cnt = 0xff;
   static unsigned long lExtDinStat_new;
   unsigned char ucReadDiPowerSt = 0;
   unsigned long biti;
	
   //从CPU管脚提取对应DIN的IO管脚电平
	  OPEN_DI_POWER();
   
   	  //delay(3000);
        if (g_gRunPara[Terminal_Type]==AERIAL_TERMINAL)
        {//架空型不进行硬遥信电源状态监测
          ucReadDiPowerSt = 1;
        }
        else
        {
	    for(m=0;m<700;m++)//1000
            {
              if((READ_DI_POWER_ST) < 1) 
              {
                ucReadDiPowerSt = 1;
                break;
              }
            }
        }
        if(ucReadDiPowerSt == 1)
        {
          	delay(1500);
           
            ExtIo1 = (P1IN & BIT0) +((P1IN & BIT2)>>1)+((P1IN & BIT4)>>2)+((P1IN & BIT6)>>3)+((P2IN & BIT0)<<4)
      	    +((P2IN & BIT2)<<3)+((P2IN & BIT4)<<2)+((P2IN & BIT6)<<1); 
	
		    ExtIo2=((P4IN & BIT4)>>4) +((P5IN & BIT4)>>3)+((P8IN & BIT1)<<1)+((P8IN & BIT4)>>1)+((P1IN & BIT1)<<3)
      	    +((P1IN & BIT3)<<2)+((P1IN & BIT5)<<1)+(P1IN & BIT7); 
            
		    ExtIo1=ExtIo1+(ExtIo2<<8);
            
      	    ExtIoStat = ((P2IN & BIT1)>>1) +((P2IN & BIT3)>>2)+((P2IN & BIT5)>>3)+((P2IN & BIT7)>>4)+((P4IN & BIT5)>>1)
      	    +((P8IN & BIT0)<<5)+((P8IN & BIT2)<<4)+((P8IN & BIT5)<<2); 
            
      	    ExtIoStat = ((ExtIoStat << 16) & 0x00ff0000) | ExtIo1;//?
            ExtIoStat = ExtIoStat | 0xff000000; 
            lExtDinStat_new = ~ExtIoStat;
            
		    CLOSE_DI_POWER();
            //delay(20);
     	if(lExtDinStat_new  != g_gExtDinStat)
        {
		    mtemp=lExtDinStat_new^g_gExtDinStat;
			YX_delay_clear_flag=1;
		    for(i=0;i<24;i++)
		    {
			    if(mtemp%2==1)
			    {
				    g_gExtDinYesTimer[i]++;
			    }
			    else
			    {
				    g_gExtDinYesTimer[i]=0;
			    }
			    if(g_gExtDinYesTimer[i] == 1)
			    {
				    for(j=0;j<8;j++)
				    {
					    g_gDinRtcLock[i][j]=g_sRtcManager.m_gRealTimer[j];
				    }
			    }
			    if(((g_gRunPara[YX_DSl]+g_gRunPara[YX_DSh]*65536)&((unsigned long)1<<i)) > 0 )//// 双点其中一个变位时，如果另一个已经处在变位延时中，则两个都置1，重新延时
			    {
				    if(g_gExtDinYesTimer[i]==1)
				    {
					    if(i%2==0)
					    {
						    if(g_gExtDinYesTimer[i+1]>0)
							    g_gExtDinYesTimer[i+1]=1;
					    }
					    else
					    {
						    if(g_gExtDinYesTimer[i-1]>0)
							    g_gExtDinYesTimer[i-1]=1;
					    }
				    }
			    }
			    if(g_gExtDinYesTimer[i]>g_gRunPara[YX_delay])
			    {
				    if(((g_gRunPara[YX_DSl]+g_gRunPara[YX_DSh]*65536)&((unsigned long)1<<i))>0)//双点
				    {
					    if(i%2==0)
					    {
						    if((g_gExtDinYesTimer[i+1]>g_gRunPara[YX_delay]-1)||(g_gExtDinYesTimer[i+1]==0))
						    {
							    f1_cnt=0x80+i;
							    g_gExtDinYesTimer[i]=0;
							
							    g_gExtDinStat^=((unsigned long)1<<i);
							
        					
							    if(g_gExtDinYesTimer[i+1]>0)
							    {
								    g_gExtDinYesTimer[i+1]=0;
								
								    g_gExtDinStat^=((unsigned long)1<<(i+1));
							    }
							
							    if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 1 )
							    {
							
							    	CreatNewSoe(RI_ZD_DYX1 + (i/2),(2),1);	
							    }
							    else if(((g_gExtDinStat >> 2*(i/2)) & 0x03) == 2 )
                    		    {
                    				CreatNewSoe(RI_ZD_DYX1 + (i/2),(1),1);		
							    }//分
                    		    else if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 0 )
                    		    {
								    CreatNewSoe(RI_ZD_DYX1 + (i/2),(0),1);		
							    }//过渡
                    		    else 
                    		    {
								    CreatNewSoe(RI_ZD_DYX1 + (i/2),(3),1);	
							    }  //错误
								
						    }
						    else
						    {
							    g_gExtDinYesTimer[i]=0;
							    g_gExtDinYesTimer[i+1]=0;
						    }
					    }
					    else
					    {
						    if((g_gExtDinYesTimer[i-1]>g_gRunPara[YX_delay]-1)||(g_gExtDinYesTimer[i-1]==0))
						    {
							    f1_cnt=0x80+i-1;
							    g_gExtDinYesTimer[i]=0;
							    g_gExtDinStat^=((unsigned long)1<<i);
							
							    if(g_gExtDinYesTimer[i-1]>0)
							    {
								    g_gExtDinYesTimer[i-1]=0;
								   g_gExtDinStat^=((unsigned long)1<<(i-1));
        						
							    }
							    if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 1 )
							    {						
									CreatNewSoe(RI_ZD_DYX1 + (i/2),(2),1);	
								}
							
							    else if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 2 )
                    		    {
									CreatNewSoe(RI_ZD_DYX1 + (i/2),(1),1);		
								}//分
                    		    else if(((g_gExtDinStat  >> 2*(i/2)) & 0x03) == 0 )
                    		    {
									CreatNewSoe(RI_ZD_DYX1 + (i/2),(0),1);																		
							    }//过渡
                    		    else 
                    		    {                    			
									CreatNewSoe(RI_ZD_DYX1 + (i/2),(3),1);	
									
							    }  //错误
								
						    }
						    else
						    {
							    g_gExtDinYesTimer[i]=0;
							    g_gExtDinYesTimer[i-1]=0;
						    }
					    }
				    }
				    else  //单点
				    {
					    g_gExtDinYesTimer[i]=0;
					    g_gExtDinStat ^=((unsigned long)1<<i);
 			
					  if(((g_gRunPara[YX_MODEl]+g_gRunPara[YX_MODEh]*65536)& ((unsigned long)1<<i) ) > 0 )// 脉冲模式
                      {
                        // 如果脉冲屏蔽时间没到，或者遥信从 1 -> 0 ，不触发故障
                      	if(g_gDIMcLock[i] > 0)
                        {
							f1_cnt = 0xff;
                        }
                        else if(( g_gExtDinStat & ((unsigned long)1 << i)) != ((unsigned long)1 << i)) 
                        {
                          f1_cnt = 0xff;//待测试
                        }
                        
                        else
                        { 
					        f1_cnt = 0x40 + i;
							CreatNewSoe(RI_ZD_YX1  + i,(1),0);
                            g_gDIMcLock[i]=g_gRunPara[RP_FLTLK_T];
                        }    
                     }
                     else
                     { 
                       f1_cnt = i;
          				biti = 1; //原因是直接biti=1<<i,当i=16时出问题 
          		        biti = biti << i;
          		        if( (g_gExtDinStat  & biti) == biti)//为产生DIN的电平
          		        {
              		       CreatNewSoe(RI_ZD_YX1 + i,1,1);
          		        }
         	 	        else
          		        {
              		        CreatNewSoe(RI_ZD_YX1 + i,0,1);
          		        }
                     }	
				}
			}
			
			mtemp >>= 1;			
        }
	    	
	    }
		else
		{
			if(YX_delay_clear_flag > 0)
        	{
            	for(n=0;n<24;n++)
                
            	g_gExtDinYesTimer[n] = 0;
            
            	YX_delay_clear_flag = 0;    //防止在 中断里重复 清空延时计数
        	}
		}
        //CLOSE_DI_POWER();
     }
     if((f1_cnt<0xFF))
	    {
		    return 1;
	    }
        else
#endif
            return 0;
}

//==============================================================================
//  函数名称   : WatchWire
//  功能描述   : 监视无线模块的喂狗信号，3秒之内无变位则重启
//  输出参数   : 
//  返回值     : 1=重启信号
//  其他说明   : 
//  作者       ：zx
//==============================================================================
//bool WatchWire(void)
//{
//    g_WireStatus = WIRETESTIN();
//    if(g_WireStatusOld == g_WireStatus)
//    {
//        g_WatchWireTime++;
//        if(g_WatchWireTime >= WATCHWIRE_3S)
//        {
//            g_WatchWireTime = 0;
//            return 1;
//        }
//        else
//        {
//            return 0;    
//        }
//    }
//    else
//    {
//        g_WatchWireTime = 0;
//        g_WireStatusOld = g_WireStatus;
//        return 0;
//    }
//}


//==============================================================================
//  函数名称   : ActOut
//  功能描述   : 对具体硬件对象的操作，根据操作类型，产生出口
//  输入参数   : OutType-操作出口号
//                 OutValue-动作值，ON或OFF
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================

void ActOut(unsigned int OutType, unsigned int OutValue)
{

}






