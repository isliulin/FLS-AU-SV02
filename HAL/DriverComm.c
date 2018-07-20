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
      P3SEL  |= (BIT4 + BIT5);	  //���ùܽ�Ϊ�ڶ�����
      UCA0CTL1 |= UCSSEL_1; 
      UCA0CTL0  = 0;
      SetUartBate(BAUD_9600,COM0);	  //���ò�����9600kbit/s
      UCA0CTL1 &= ~UCSWRST;
      UCA0IE   |= UCRXIE + UCTXIE;		  //��USART0�����ж�
}

void Uart1Init(void)
{
      UCA1CTL1 |= UCSWRST;
      
      P5SEL  |= (BIT6 + BIT7);	  //���ùܽ�Ϊ�ڶ�����
      UCA1CTL0  = 0;
      UCA1CTL1 |= UCSSEL_1; 
      SetUartBate(BAUD_9600,COM1);	  //���ò�����9600kbit/s
      UCA1CTL1 &= ~UCSWRST;     
      UCA1IE   |= UCRXIE + UCTXIE;		  //��USART0�����ж�
}
void Uart2Init(void)
{
      UCA2CTL1 |= UCSWRST;
      P9SEL  |= (BIT4 + BIT5);	  //���ùܽ�Ϊ�ڶ�����
      UCA2CTL0  = 0;
      UCA2CTL1 |= UCSSEL_1; 
      SetUartBate(BAUD_9600,COM2);	  //���ò�����9600kbit/s
      UCA2CTL1 &= ~UCSWRST;
      UCA2IE   |= UCRXIE + UCTXIE;		  //��USART0�����ж�
}
void Uart3Init(void)
{
	  P10SEL  |= (BIT4 + BIT5);	  //���ùܽ�Ϊ�ڶ�����
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
      P8SEL  &= ~(BIT0 + BIT1);	          //�趨��ͨ����IO��
      P8OUT  &= ~(BIT0 + BIT1);
      //GPRS_ONOFF_IN();
      
      P5SEL  &= ~(BIT4 + BIT5);	          //�趨��ͨ����IO��
      P5OUT  &= ~(BIT4 + BIT5);
      UCA1IE   &= ~(UCRXIE + UCTXIE);     //�ر��ж�
}
void CloseUart3(void)
{
      //P8SEL  &= ~(BIT0 + BIT1);	          //�趨��ͨ����IO��
      //P8OUT  &= ~(BIT0 + BIT1);
      //GPRS_ONOFF_IN();
      
      P10SEL  &= ~(BIT6 + BIT7);	          //�趨��ͨ����IO��
      P10OUT  &= ~(BIT6 + BIT7);
      UCA3IE   &= ~(UCRXIE + UCTXIE);     //�ر��ж�
}

//==============================================================================
//  ��������   : InitSCI
//  ��������   : ͨ��ģ���ʼ��������SCI���ã��������в������ø�������
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void InitSCI(void)
{
    UCA1IFG &= ~UCTXIFG;                    //�رշ����ж�
    UCA2IFG &= ~UCTXIFG;                    //�رշ����ж�
    UCA3IFG &= ~UCTXIFG;                    //�رշ����ж�  
    Uart0Init();
    Uart1Init();
    Uart2Init();
    Uart3Init();
    
}

//==============================================================================
//  ��������   : InitGPRSBaud
//  ��������   : GPRSģ�鲨���ʳ�ʼ��
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
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
    else                               //115200ΪĬ��
    {
        UCA3BR0 = 0x8a;                // 16MHz 115200 (see User's Guide)
        UCA3BR1 = 0x00;                // 16MHz 115200
    }
}*/

//==============================================================================
//  ��������   : PhysSendByte
//  ��������   :  ������ֽڷ��� 
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void PhysSendByte_RF(unsigned char ucByte)
{
    //unsigned int  i;
    //for(i = 0; i < 1000; i++);                                     //��ʱ
    while((UCA0IFG & UCTXIFG == 0)||(UCA0STAT & UCBUSY == 1));   //�ȴ����ͻ�������
        UCA0TXBUF = ucByte;

}
//==============================================================================
//  ��������   : SendBytes
//  ��������   :  ����һ֡���� 
//  �������   : ucRxdlen�����ݳ���  *Data������
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
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
//  ��������   : PhysSendByte
//  ��������   :  ������ֽڷ��� 
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void PhysSendByte_GPRS(unsigned int ucByte)
{
    unsigned int  i;
    for(i = 0; i < 1000; i++);                                     //��ʱ
    while((UCA1IFG & UCTXIFG == 0)||(UCA1STAT & UCBUSY == 1));   //�ȴ����ͻ�������
        UCA1TXBUF = ucByte;

}

//==============================================================================
//  ��������   : SendBytes
//  ��������   :  ����һ֡���� 
//  �������   : ucRxdlen�����ݳ���  *Data������
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void SendBytes_GPRS(unsigned char *Data,int ucRxdlen)
{
  int i = 0;
  for(i = 0;i < ucRxdlen;i++)
    PhysSendByte_GPRS(Data[i]); 
}
//==============================================================================
//  ��������   : PhysSendByte
//  ��������   :  ������ֽڷ��� 
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void PhysSendByte_UART2(unsigned char ucByte)
{
    //unsigned int  i;
    //for(i = 0; i < 1000; i++);                                     //��ʱ
    while((UCA2IFG & UCTXIFG == 0)||(UCA2STAT & UCBUSY == 1));   //�ȴ����ͻ�������
        UCA2TXBUF = ucByte;

}
//==============================================================================
//  ��������   : SendBytes
//  ��������   :  ����һ֡���� 
//  �������   : ucRxdlen�����ݳ���  *Data������
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void SendBytes_UART2(unsigned char *Data,int ucRxdlen)
{
  int i = 0;
  for(i = 0;i < ucRxdlen;i++)
    PhysSendByte_UART2(Data[i]);
}
//==============================================================================
//  ��������   : PhysSendByte
//  ��������   :  ������ֽڷ��� 
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void PhysSendByte_232(unsigned char ucByte)
{
    //unsigned int  i;
    //for(i = 0; i < 1000; i++);                                     //��ʱ
    while((UCA3IFG & UCTXIFG == 0)||(UCA3STAT & UCBUSY == 1));   //�ȴ����ͻ�������
        UCA3TXBUF = ucByte;

}
//==============================================================================
//  ��������   : SendBytes
//  ��������   :  ����һ֡���� 
//  �������   : ucRxdlen�����ݳ���  *Data������
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
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
// **                                   ���� 0
// ** �̾������� RF001
// **�Ͷ̾�����ģ��ͨѶ
//
//
//**********************************************************************************
#pragma vector=USCI_A0_VECTOR//�ξ�������
__interrupt void UsartA0_ISR()            //��������
{
 unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 0;
  
 switch(__even_in_range(UCA0IV,4))
   {
    case 0x04://����
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
     case 0x02://����
            ucRxdReg = UCA0RXBUF;
            if(((CBJ101S *)pGprs)->m_tra_FI == ON)//�ն˽���ת��ģʽ
            {
                UCA1TXBUF = ucRxdReg;
                //OpenTimer(TM_OPENGPRS_RF);
            }
            else if(((CBJ101S *)pDbg)->m_tra_FI == ON)//�ն˽���ת��ģʽ
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
              g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
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
// **                                   ���� 1
// ** GPRSͨѶ
//**********************************************************************************
#pragma vector=USCI_A1_VECTOR           
__interrupt void UsartA1_ISR()          //��������
{    
unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 1;
  
 switch(__even_in_range(UCA1IV,4))
   {
    case 0x04://���� 
    	
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
     case 0x02://����
            if((g_CmIdGPRS == cCmId) && !g_GprsPowerSt)//for test ģ��GPRS�ر�״̬���ر���վӦ���ղ�������
              break;
            ucRxdReg = UCA1RXBUF;
            if(((CBJ101S *)pGprs)->m_tra_FI == ON)//�ն˽���ת��ģʽ
            {
                UCA0TXBUF = ucRxdReg;
                OpenTimer(TM_OPENGPRS_RF);
            }
            else
            {
              g_sRxBuff[cCmId].m_iRcvCount = 10;
             /* g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
              g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
              g_sRxBuff[cCmId].m_iBufT++;*/
			   if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
               g_sRxBuff[cCmId].m_iBufT -=  MAX_RXBUFF_LEN;
            //g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
          g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
          g_sRxBuff[cCmId].m_iBufT++;
        //if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
            //g_sRxBuff[cCmId].m_iBufT -= MAX_RXBUFF_LEN;
        //g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
        //g_sRxBuff[cCmId].m_iBufT++;
        if(g_bCommFlag[cCmId])
        {
            g_wCheckEBByte[cCmId]++;
        }
        if((ucRxdReg == 0xEB) && g_bCommFlag[cCmId] == 0x00)  //��һ��EB
        {
            g_bCommFlag[cCmId] = 0x01;
            g_bCheckEBFrameBuf[cCmId][0]  = ucRxdReg;
        }
        else if(g_bCommFlag[cCmId] == 0x01)
        {
            g_bCheckEBFrameBuf[cCmId][g_wCheckEBByte[cCmId]]  = ucRxdReg;
            if(g_wCheckEBByte[cCmId] == 3)   //�ڶ���EB
            {
                if(g_bCheckEBFrameBuf[cCmId][3] == 0xEB)
                {
                    g_bCommFlag[cCmId] = 0x02;
                    g_wEBFrameLen[cCmId] = MAKEWORD(g_bCheckEBFrameBuf[cCmId][2],g_bCheckEBFrameBuf[cCmId][1]);  //�������EB֡��
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
                if(ucRxdReg == 0xD7)  //�ҵ�֡β��������������
                {
                    g_bRecFrameEBFlag[cCmId] = 0x55;
                    LPM3_EXIT;
                }
                g_bCommFlag[cCmId] = 0;       //������б�־λ��׼��������һ֡
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
// **                                   ���� 2
// ** Ԥ����չ
//**********************************************************************************
#pragma vector=USCI_A2_VECTOR
__interrupt void UsartA2_ISR()          //��������
{
  unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 2;
  
 switch(__even_in_range(UCA2IV,4))
   {
    case 0x04://����
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
     case 0x02://����
            ucRxdReg = UCA2RXBUF;
            g_sRxBuff[cCmId].m_iRcvCount = 50;
            // g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
			  if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
               g_sRxBuff[cCmId].m_iBufT -=  MAX_RXBUFF_LEN;
            g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
            g_sRxBuff[cCmId].m_iBufT++;
           break;
   default:break;
   }        
}


//**********************************************************************************
// **                                   ���� 3
// ** ����/����
//**********************************************************************************
#pragma vector=USCI_A3_VECTOR
__interrupt void UsartA3_ISR()          //��������
{ 
  unsigned char  ucRxdReg;
  unsigned char cCmId;
  cCmId = 3;
  
 switch(__even_in_range(UCA3IV,4))
   {
    case 0x04://����  
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
     case 0x02://����
            ucRxdReg = UCA3RXBUF;
            if(((CBJ101S *)pDbg)->m_tra_FI == ON)//�ն˽���ת��ģʽ
            {
                UCA0TXBUF = ucRxdReg;
                OpenTimer(TM_OPENDEG_RF);
            }
            else
            {
              g_sRxBuff[cCmId].m_iRcvCount = 10;
             /* g_sRxBuff[cCmId].m_iBufT &= (MAX_RXBUFF_LEN - 1);
              g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
              g_sRxBuff[cCmId].m_iBufT++;*/
	if(g_sRxBuff[cCmId].m_iBufT >= MAX_RXBUFF_LEN)
            g_sRxBuff[cCmId].m_iBufT -= MAX_RXBUFF_LEN;
        g_sRxBuff[cCmId].m_gBuf[g_sRxBuff[cCmId].m_iBufT] = ucRxdReg;   //�������ݵ����ջ����� 
        g_sRxBuff[cCmId].m_iBufT++;
        if(g_bCommFlag[cCmId])
        {
            g_wCheckEBByte[cCmId]++;
        }
        if((ucRxdReg == 0xEB) && g_bCommFlag[cCmId] == 0x00)  //��һ��EB
        {
            g_bCommFlag[cCmId] = 0x01;
            g_bCheckEBFrameBuf[cCmId][0]  = ucRxdReg;
        }
        else if(g_bCommFlag[cCmId] == 0x01)
        {
            g_bCheckEBFrameBuf[cCmId][g_wCheckEBByte[cCmId]]  = ucRxdReg;
            if(g_wCheckEBByte[cCmId] == 3)   //�ڶ���EB
            {
                if(g_bCheckEBFrameBuf[cCmId][3] == 0xEB)
                {
                    g_bCommFlag[cCmId] = 0x02;
                    g_wEBFrameLen[cCmId] = MAKEWORD(g_bCheckEBFrameBuf[cCmId][2],g_bCheckEBFrameBuf[cCmId][1]);  //�������EB֡��
                }
                else
                {
                    g_bCommFlag[cCmId] = 0;
                    g_wCheckEBByte[cCmId] = 0;                    
                }
            }       //ZX�޸� 2017050
        }
        else if(g_bCommFlag[cCmId] == 0x02)
        {
            if(g_wCheckEBByte[cCmId] == g_wEBFrameLen[cCmId] + 5)
            {
                if(ucRxdReg == 0xD7)  //�ҵ�֡β��������������
                {
                    g_bRecFrameEBFlag[cCmId] = 0x55;    
                    LPM3_EXIT;
                }
                g_bCommFlag[cCmId] = 0;       //������б�־λ��׼��������һ֡
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
