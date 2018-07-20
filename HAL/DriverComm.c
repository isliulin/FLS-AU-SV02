#ifndef _DRIVER_COMM

#define _DRIVER_COMM
  #include "..\DataStruct.h"
  #include "..\Main.h"
#endif

typedef enum  {BAUD_1200, BAUD_4800, BAUD_9600}BAUD_RATE_EUM;
typedef enum  {COM0, COM1, COM2, COM3}UART_NO_EUM;
void SetUartBate(BAUD_RATE_EUM sel,UART_NO_EUM no)
{
      switch(no)
      {
          case COM0:if(sel == BAUD_1200){
                      UCA0BR0  = 0x1A;UCA0BR1  = 0x00;UCA0MCTL = 0x6D;
                    }else if(sel == BAUD_4800){
                      UCA0BR0  = 0x06;UCA0BR1  = 0x00;UCA0MCTL = 0x6D;
                    }else{
                      UCA0BR0  = 0x03;UCA0BR1  = 0x00;UCA0MCTL = UCBRS_3+UCBRF_0;
                    }
                    break;
          case COM1:if(sel == BAUD_1200){
                      UCA1BR0  = 0x1A;UCA1BR1  = 0x00;UCA1MCTL = 0x6D;
                    }else if(sel == BAUD_4800){
                      UCA1BR0  = 0x06;UCA1BR1  = 0x00;UCA1MCTL = 0x6D;
                    }else{
                      UCA1BR0  = 0x03;UCA1BR1  = 0x00;UCA1MCTL = UCBRS_3+UCBRF_0;
                    }
                    break;
          case COM2:if(sel == BAUD_1200){
                      UCA2BR0  = 0x1A;UCA2BR1  = 0x00;UCA2MCTL = 0x6D;
                    }else if(sel == BAUD_4800){
                      UCA2BR0  = 0x06;UCA2BR1  = 0x00;UCA2MCTL = 0x6D;
                    }else{
                      UCA2BR0  = 0x03;UCA2BR1  = 0x00;UCA2MCTL = UCBRS_3+UCBRF_0;
                    }
                    break;
          case COM3:if(sel == BAUD_1200){
                      UCA3BR0  = 0x1A;UCA3BR1  = 0x00;UCA3MCTL = 0x6D;
                    }else if(sel == BAUD_4800){
                      UCA3BR0  = 0x06;UCA3BR1  = 0x00;UCA3MCTL = UCBRS_7 + UCBRF_0;;
                    }else{                      
                      UCA3BR0  = 0x03;UCA3BR1  = 0x00;UCA3MCTL = UCBRS_3+UCBRF_0;                      
                    }
                    break;                    
      }
}

void Uart0Init(void)
{
      UCA0CTL1 |= UCSWRST;
      P3SEL  |= (BIT4 + BIT5);	  //设置管脚为第二功能
      UCA0CTL1 |= UCSSEL_1; 
      UCA0CTL0  = 0;
      SetUartBate(BAUD_9600,COM0);	  //设置波特率9600kbit/s
      UCA0CTL1 &= ~UCSWRST;
      UCA0IE   |= UCRXIE + UCTXIE;		  //打开USART0接收中断
}

void Uart1Init(void)
{
      UCA1CTL1 |= UCSWRST;
      
      P5SEL  |= (BIT6 + BIT7);	  //设置管脚为第二功能
      UCA1CTL0  = 0;
      UCA1CTL1 |= UCSSEL_1; 
      SetUartBate(BAUD_9600,COM1);	  //设置波特率9600kbit/s
      UCA1CTL1 &= ~UCSWRST;     
      UCA1IE   |= UCRXIE + UCTXIE;		  //打开USART0接收中断
}
void Uart2Init(void)
{
      UCA2CTL1 |= UCSWRST;
      P9SEL  |= (BIT4 + BIT5);	  //设置管脚为第二功能
      UCA2CTL0  = 0;
      UCA2CTL1 |= UCSSEL_1; 
      SetUartBate(BAUD_9600,COM2);	  //设置波特率9600kbit/s
      UCA2CTL1 &= ~UCSWRST;
      UCA2IE   |= UCRXIE + UCTXIE;		  //打开USART0接收中断
}
void Uart3Init(void)
{
	  P10SEL  |= (BIT4 + BIT5);	  //设置管脚为第二功能
	  UCA3CTL1 |= UCSWRST; 
	  // **Put state machine in reset**
	  if(g_gRunPara[RP_CFG_KEY]& BIT[RP_DEBUG_RATE])
	  {
	  	UCA3CTL1 |= UCSSEL_1; 
		UCA3BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
	  	UCA3BR1 = 0x00;                           //
	  	UCA3MCTL = UCBRS_3+UCBRF_0;      
	  }
	  else
	  {
	  	UCA3CTL1 |= UCSSEL_2;//UCSSEL_1;                     // CLK = ACLK
	  
	  	UCA3BR0 = 0x45;//0x41;                           // 32kHz/9600=3.41 (see User's Guide)
	  	UCA3BR1 = 0;//0x3;                           //
	  	UCA3MCTL = 0x4A;//0;//xEF;//UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0*/
	  }
	  /*UCA3BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
	  UCA3BR1 = 0x00;                           //
	  UCA3MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0*/
	  UCA3CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	  UCA3IE |= UCRXIE + UCTXIE;                         // Enable USCI_A0 RX interrupt     
  
}

void CloseUart1(void)
{
      P8SEL  &= ~(BIT0 + BIT1);	          //设定普通功能IO口
      P8OUT  &= ~(BIT0 + BIT1);
      //GPRS_ONOFF_IN();
      
      P5SEL  &= ~(BIT4 + BIT5);	          //设定普通功能IO口
      P5OUT  &= ~(BIT4 + BIT5);
      UCA1IE   &= ~(UCRXIE + UCTXIE);     //关闭中断
}
void CloseUart3(void)
{
      //P8SEL  &= ~(BIT0 + BIT1);	          //设定普通功能IO口
      //P8OUT  &= ~(BIT0 + BIT1);
      //GPRS_ONOFF_IN();
      
      P10SEL  &= ~(BIT6 + BIT7);	          //设定普通功能IO口
      P10OUT  &= ~(BIT6 + BIT7);
      UCA3IE   &= ~(UCRXIE + UCTXIE);     //关闭中断
}

//==============================================================================
//  函数名称   : InitSCI
//  功能描述   : 通信模块初始化，包括SCI设置，根据运行参数设置各波特率
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void InitSCI(void)
{
    UCA1IFG &= ~UCTXIFG;                    //关闭发送中断
    UCA2IFG &= ~UCTXIFG;                    //关闭发送中断
    UCA3IFG &= ~UCTXIFG;                    //关闭发送中断  
    Uart0Init();
    Uart1Init();
    Uart2Init();
    Uart3Init();
    
}

//==============================================================================
//  函数名称   : InitGPRSBaud
//  功能描述   : GPRS模块波特率初始化
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
/*void InitGPRSBaud(void)
{
     if(g_unBaud_101 == 96)
    {
        UCA3BR0 = 0x82;                // 16MHz 9600 (see User's Guide)
        UCA3BR1 = 0x06;                // 16MHz 9600
    }
    else if(g_unBaud_101 == 192)
    {
        UCA3BR0 = 0x41;                // 16MHz 19200 (see User's Guide)
        UCA3BR1 = 0x03;                // 16MHz 19200
    }
    else if(g_unBaud_101 == 384)
    {
        UCA3BR0 = 0xa0;                // 16MHz 38400 (see User's Guide)
        UCA3BR1 = 0x01;                // 16MHz 38400
    }
    else if(g_unBaud_101 == 576)
    {
        UCA3BR0 = 0x15;                // 16MHz 57600 (see User's Guide)
        UCA3BR1 = 0x01;                // 16MHz 57600
    }
    else                               //115200为默认
    {
        UCA3BR0 = 0x8a;                // 16MHz 115200 (see User's Guide)
        UCA3BR1 = 0x00;                // 16MHz 115200
    }
}*/

//==============================================================================
//  函数名称   : PhysSendByte
//  功能描述   :  物理层字节发送 
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void PhysSendByte_RF(unsigned char ucByte)
{
    //unsigned int  i;
    //for(i = 0; i < 1000; i++);                                     //延时
    while((UCA0IFG & UCTXIFG == 0)||(UCA0STAT & UCBUSY == 1));   //等待发送缓冲区空
        UCA0TXBUF = ucByte;

}
//==============================================================================
//  函数名称   : SendBytes
//  功能描述   :  发送一帧数据 
//  输入参数   : ucRxdlen：数据长度  *Data：数据
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void SendBytes_RF(unsigned char *Data,int ucRxdlen)
{    
  int i = 0;
  for(i = 0;i < ucRxdlen;i++)
    PhysSendByte_RF(Data[i]);  
  
  //g_ucTxNUM_RF = ucRxdlen;
    //UCA0TXBUF = g_unRFTX_Buff[0]; 
    //g_unRFTX_Buff[0] = 0;
    //g_ucTxlen_RF = 1;
}
//==============================================================================
//  函数名称   : PhysSendByte
//  功能描述   :  物理层字节发送 
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void PhysSendByte_GPRS(unsigned int ucByte)
{
    unsigned int  i;
    for(i = 0; i < 1000; i++);                                     //延时
    while((UCA1IFG & UCTXIFG == 0)||(UCA1STAT & UCBUSY == 1));   //等待发送缓冲区空
        UCA1TXBUF = ucByte;

}

//==============================================================================
//  函数名称   : SendBytes
//  功能描述   :  发送一帧数据 
//  输入参数   : ucRxdlen：数据长度  *Data：数据
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void SendBytes_GPRS(unsigned char *Data,int ucRxdlen)
{
  int i = 0;
  for(i = 0;i < ucRxdlen;i++)
    PhysSendByte_GPRS(Data[i]); 
}
//==============================================================================
//  函数名称   : PhysSendByte
//  功能描述   :  物理层字节发送 
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void PhysSendByte_UART2(unsigned char ucByte)
{
    //unsigned int  i;
    //for(i = 0; i < 1000; i++);                                     //延时
    while((UCA2IFG & UCTXIFG == 0)||(UCA2STAT & UCBUSY == 1));   //等待发送缓冲区空
        UCA2TXBUF = ucByte;

}
//==============================================================================
//  函数名称   : SendBytes
//  功能描述   :  发送一帧数据 
//  输入参数   : ucRxdlen：数据长度  *Data：数据
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void SendBytes_UART2(unsigned char *Data,int ucRxdlen)
{
  int i = 0;
  for(i = 0;i < ucRxdlen;i++)
    PhysSendByte_UART2(Data[i]);
}
//==============================================================================
//  函数名称   : PhysSendByte
//  功能描述   :  物理层字节发送 
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void PhysSendByte_232(unsigned char ucByte)
{
    //unsigned int  i;
    //for(i = 0; i < 1000; i++);                                     //延时
    while((UCA3IFG & UCTXIFG == 0)||(UCA3STAT & UCBUSY == 1));   //等待发送缓冲区空
        UCA3TXBUF = ucByte;

}
//==============================================================================
//  函数名称   : SendBytes
//  功能描述   :  发送一帧数据 
//  输入参数   : ucRxdlen：数据长度  *Data：数据
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
//==============================================================================
void SendBytes_232(unsigned char *Data,int ucRxdlen)
{
  int i = 0;
  for(i = 0;i < ucRxdlen;i++)
    PhysSendByte_232(Data[i]);
}

BOOL Operation_Rxbuf(unsigned char RxBuf)
{
	
	if((RxBuf == 0x55) &&(sRx_Oper.m_Operaflag ==0))
	{
	//sRx_Oper.m_OperaBuf[sRx_Oper.m_OperaBufT++] = RxBuf;
		sRx_Oper.m_OperaBufT =0;
		sRx_Oper.m_Operaflag =1;
	}
			
	else if((sRx_Oper.m_Operaflag ==1))
	{
		if(RxBuf == 0xAA)
		{
			//sRx_Oper.m_OperaBuf[sRx_Oper.m_OperaBufT++] = RxBuf;
			sRx_Oper.m_Operaflag =2;
		}
		else
		{
			sRx_Oper.m_Operalength =0;
			sRx_Oper.m_OperaBufT=0;
			sRx_Oper.m_Operaflag =0;
		}
			
				
	}
	else if(sRx_Oper.m_Operaflag ==2)
	{
		sRx_Oper.m_Operaflag =3;
		//sRx_Oper.m_OperaBuf[sRx_Oper.m_OperaBufT++] = RxBuf;
		sRx_Oper.m_Operalength = RxBuf;
	}
	if(sRx_Oper.m_OperaBufT < MAX_RXBUFF_LEN)
		sRx_Oper.m_OperaBufT++;//sRx_Oper.m_OperaBuf[sRx_Oper.m_OperaBufT++] = RxBuf;
	else
	{
		sRx_Oper.m_Operalength =0;
		sRx_Oper.m_OperaBufT=0;
		sRx_Oper.m_Operaflag =0;
	}
	if(sRx_Oper.m_Operaflag ==3)
	{
		if(sRx_Oper.m_OperaBufT == sRx_Oper.m_Operalength )
		{
			if(RxBuf == 0x16)//sRx_Oper.m_OperaBuf[sRx_Oper.m_OperaBufT-1]
				return TRUE;
		        else
		        {
			  sRx_Oper.m_Operalength =0;
			  sRx_Oper.m_OperaBufT=0;
			  sRx_Oper.m_Operaflag =0;
				//return FALSE;
			}
		}
	}
	return FALSE;
	
}


//**********************************************************************************
// **                                   串口 0
// ** 短距离无线 RF001
// **和短距无线模块通讯
//
//
//**********************************************************************************
#pragma vector=USCI_A0_VECTOR//段距离无线
__interrupt void UsartA0_ISR()            //发送数据
{
 unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 0;
  
 switch(__even_in_range(UCA0IV,4))
   {
    case 0x04://发送
	    if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
	    {
			g_sTxBuff[cCmId].m_unSendFlag = 1;
			g_sTxBuff[cCmId].m_ucTx_Head++;
	        g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_TXBUFF_LEN - 1);
			if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
				UCA0TXBUF = g_sTxBuff[cCmId].m_gBuffer[g_sTxBuff[cCmId].m_ucTx_Head];
			else
				g_sTxBuff[cCmId].m_unSendFlag = 0;
			//g_sTxBuff[cCmId].m_ucTx_Head++;
	        //g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_RXBUFF_LEN - 1);
		}
		else
		{
			g_sTxBuff[cCmId].m_unSendFlag = 0;
		}
         break;
     case 0x02://接收
            ucRxdReg = UCA0RXBUF;
            if(((CBJ101S *)pGprs)->m_tra_FI == ON)//终端进入转发模式
            {
                UCA1TXBUF = ucRxdReg;
                //OpenTimer(TM_OPENGPRS_RF);
            }
            else if(((CBJ101S *)pDbg)->m_tra_FI == ON)//终端进入转发模式
            {
                UCA3TXBUF = ucRxdReg;
               // OpenTimer(TM_OPENDEG_RF);
            }
            else
            {
            //UCA3TXBUF = UCA0RXBUF;
              g_sRxBuff[cCmId].m_iRcvCount = 200;//80
              //g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
			  if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
               g_sRxBuff[cCmId].m_iBufT -=  MAX_RXBUFF_LEN; 
              g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
              g_sRxBuff[cCmId].m_iBufT++;
              if(gRecorder_flagall == ON)
              {
	        if(Operation_Rxbuf(ucRxdReg))
		  g_sRxBuff[cCmId].m_iRcvCount = 2;
              }
            }
           break;
   default:break;
   }
}
//**********************************************************************************
// **                                   串口 1
// ** GPRS通讯
//**********************************************************************************
#pragma vector=USCI_A1_VECTOR           
__interrupt void UsartA1_ISR()          //发送数据
{    
unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 1;
  
 switch(__even_in_range(UCA1IV,4))
   {
    case 0x04://发送 
    	
	    if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
	    {
			g_sTxBuff[cCmId].m_unSendFlag = 1;
			g_sTxBuff[cCmId].m_ucTx_Head++;
	        g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_TXBUFF_LEN - 1);
			if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
				UCA1TXBUF = g_sTxBuff[cCmId].m_gBuffer[g_sTxBuff[cCmId].m_ucTx_Head];
			else
				g_sTxBuff[cCmId].m_unSendFlag = 0;
			//g_sTxBuff[cCmId].m_ucTx_Head++;
	        //g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_RXBUFF_LEN - 1);
		}
		else
		{
			g_sTxBuff[cCmId].m_unSendFlag = 0;
			
		}
         break;
     case 0x02://接收
            if((g_CmIdGPRS == cCmId) && !g_GprsPowerSt)//for test 模拟GPRS关闭状态，关闭子站应该收不到数据
              break;
            ucRxdReg = UCA1RXBUF;
            if(((CBJ101S *)pGprs)->m_tra_FI == ON)//终端进入转发模式
            {
                UCA0TXBUF = ucRxdReg;
                OpenTimer(TM_OPENGPRS_RF);
            }
            else
            {
              g_sRxBuff[cCmId].m_iRcvCount = 10;
             /* g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
              g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
              g_sRxBuff[cCmId].m_iBufT++;*/
			   if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
               g_sRxBuff[cCmId].m_iBufT -=  MAX_RXBUFF_LEN;
            //g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
          g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
          g_sRxBuff[cCmId].m_iBufT++;
        //if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
            //g_sRxBuff[cCmId].m_iBufT -= MAX_RXBUFF_LEN;
        //g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
        //g_sRxBuff[cCmId].m_iBufT++;
        if(g_bCommFlag[cCmId])
        {
            g_wCheckEBByte[cCmId]++;
        }
        if((ucRxdReg == 0xEB) && g_bCommFlag[cCmId] == 0x00)  //第一个EB
        {
            g_bCommFlag[cCmId] = 0x01;
            g_bCheckEBFrameBuf[cCmId][0]  = ucRxdReg;
        }
        else if(g_bCommFlag[cCmId] == 0x01)
        {
            g_bCheckEBFrameBuf[cCmId][g_wCheckEBByte[cCmId]]  = ucRxdReg;
            if(g_wCheckEBByte[cCmId] == 3)   //第二个EB
            {
                if(g_bCheckEBFrameBuf[cCmId][3] == 0xEB)
                {
                    g_bCommFlag[cCmId] = 0x02;
                    g_wEBFrameLen[cCmId] = MAKEWORD(g_bCheckEBFrameBuf[cCmId][2],g_bCheckEBFrameBuf[cCmId][1]);  //计算加密EB帧长
                }
                else
                {
                    g_bCommFlag[cCmId] = 0;
                    g_wCheckEBByte[cCmId] = 0;                    
                }
            } 
        }
        else if(g_bCommFlag[cCmId] == 0x02)
        {
            if(g_wCheckEBByte[cCmId] == g_wEBFrameLen[cCmId] + 5)
            {
                if(ucRxdReg == 0xD7)  //找到帧尾，启动解析报文
                {
                    g_bRecFrameEBFlag[cCmId] = 0x55;
                    LPM3_EXIT;
                }
                g_bCommFlag[cCmId] = 0;       //清楚所有标志位，准备接收下一帧
                g_wCheckEBByte[cCmId] = 0;
            }
        }
        else if(g_wEBFrameLen[cCmId] > MAX_RXBUFF_LEN)
        {
            g_bCommFlag[cCmId] = 0;
            g_wCheckEBByte[cCmId] = 0;
        }

     }
           break;
   default:break;
   }        
}

//**********************************************************************************
// **                                   串口 2
// ** 预留扩展
//**********************************************************************************
#pragma vector=USCI_A2_VECTOR
__interrupt void UsartA2_ISR()          //发送数据
{
  unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 2;
  
 switch(__even_in_range(UCA2IV,4))
   {
    case 0x04://发送
    	if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
	    {
			g_sTxBuff[cCmId].m_unSendFlag = 1;
			g_sTxBuff[cCmId].m_ucTx_Head++;
	        g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_TXBUFF_LEN - 1);
			if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
				UCA2TXBUF = g_sTxBuff[cCmId].m_gBuffer[g_sTxBuff[cCmId].m_ucTx_Head];
			else
				g_sTxBuff[cCmId].m_unSendFlag = 0;
			//g_sTxBuff[cCmId].m_ucTx_Head++;
	        //g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_RXBUFF_LEN - 1);
		}
		else
		{
			g_sTxBuff[cCmId].m_unSendFlag = 0;
		}
         break;
     case 0x02://接收
            ucRxdReg = UCA2RXBUF;
            g_sRxBuff[cCmId].m_iRcvCount = 50;
            // g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
			  if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
               g_sRxBuff[cCmId].m_iBufT -=  MAX_RXBUFF_LEN;
            g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
            g_sRxBuff[cCmId].m_iBufT++;
           break;
   default:break;
   }        
}


//**********************************************************************************
// **                                   串口 3
// ** 调试/配置
//**********************************************************************************
#pragma vector=USCI_A3_VECTOR
__interrupt void UsartA3_ISR()          //发送数据
{ 
  unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 3;
  
 switch(__even_in_range(UCA3IV,4))
   {
    case 0x04://发送  
    	if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
	    {
			g_sTxBuff[cCmId].m_unSendFlag = 1;
			g_sTxBuff[cCmId].m_ucTx_Head++;
			g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_TXBUFF_LEN - 1);
			if(g_sTxBuff[cCmId].m_ucTx_Head != g_sTxBuff[cCmId].m_ucTx_Tail)
				UCA3TXBUF = g_sTxBuff[cCmId].m_gBuffer[g_sTxBuff[cCmId].m_ucTx_Head];
            else
				g_sTxBuff[cCmId].m_unSendFlag = 0;
                       
			//g_sTxBuff[cCmId].m_ucTx_Head++;
	        //g_sTxBuff[cCmId].m_ucTx_Head &= (MAX_RXBUFF_LEN - 1);
		}
		else
		{
			g_sTxBuff[cCmId].m_unSendFlag = 0;
			
		}
         break;
     case 0x02://接收
            ucRxdReg = UCA3RXBUF;
            if(((CBJ101S *)pDbg)->m_tra_FI == ON)//终端进入转发模式
            {
                UCA0TXBUF = ucRxdReg;
                OpenTimer(TM_OPENDEG_RF);
            }
            else
            {
              g_sRxBuff[cCmId].m_iRcvCount = 10;
             /* g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
              g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
              g_sRxBuff[cCmId].m_iBufT++;*/
	if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
            g_sRxBuff[cCmId].m_iBufT -= MAX_RXBUFF_LEN;
        g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //保存数据到接收缓冲器 
        g_sRxBuff[cCmId].m_iBufT++;
        if(g_bCommFlag[cCmId])
        {
            g_wCheckEBByte[cCmId]++;
        }
        if((ucRxdReg == 0xEB) && g_bCommFlag[cCmId] == 0x00)  //第一个EB
        {
            g_bCommFlag[cCmId] = 0x01;
            g_bCheckEBFrameBuf[cCmId][0]  = ucRxdReg;
        }
        else if(g_bCommFlag[cCmId] == 0x01)
        {
            g_bCheckEBFrameBuf[cCmId][g_wCheckEBByte[cCmId]]  = ucRxdReg;
            if(g_wCheckEBByte[cCmId] == 3)   //第二个EB
            {
                if(g_bCheckEBFrameBuf[cCmId][3] == 0xEB)
                {
                    g_bCommFlag[cCmId] = 0x02;
                    g_wEBFrameLen[cCmId] = MAKEWORD(g_bCheckEBFrameBuf[cCmId][2],g_bCheckEBFrameBuf[cCmId][1]);  //计算加密EB帧长
                }
                else
                {
                    g_bCommFlag[cCmId] = 0;
                    g_wCheckEBByte[cCmId] = 0;                    
                }
            }       //ZX修改 2017050
        }
        else if(g_bCommFlag[cCmId] == 0x02)
        {
            if(g_wCheckEBByte[cCmId] == g_wEBFrameLen[cCmId] + 5)
            {
                if(ucRxdReg == 0xD7)  //找到帧尾，启动解析报文
                {
                    g_bRecFrameEBFlag[cCmId] = 0x55;    
                    LPM3_EXIT;
                }
                g_bCommFlag[cCmId] = 0;       //清楚所有标志位，准备接收下一帧
                g_wCheckEBByte[cCmId] = 0;
            }
        }
        else if(g_wEBFrameLen[cCmId] > MAX_RXBUFF_LEN)
        {
            g_bCommFlag[cCmId] = 0;
            g_wCheckEBByte[cCmId] = 0;
        }
 
 		}
           break;
   default:break;
   }
}
