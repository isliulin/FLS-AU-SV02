/**********************************************************************************
  Copyright (C) 1988-2007, CREAT Company 
  ģ����       : ��������
  �ļ���       : .c
  ����ļ�     : DataStruct.h FuncDef.h Constant.h
  ʵ�ֹ���     : 
  ����         : ����һ
  �汾         : 1.0
  
  --------------------------------------------------------------------------------
  ��ע         : -
  --------------------------------------------------------------------------------
  
  �޸ļ�¼     : 
    ��  ��        �汾        �޸���        �޸����� 
    2010/06/01    1.0         ����һ        ����

************************************************************************************/
#ifndef _DRIVER_DMA

#define _DRIVER_DMA
  #include "DataStruct.h"
#endif

//==============================================================================
//  ��������   : InitDMA
//  ��������   : DMA0��ΪGPRS���ݽ���
//               DMA1��Ϊ232���ݽ���
//               DMA1��Ϊ232��GPRS���ݷ��͹��ã��ڷ��ͺ����г�ʼ��
//  �������   : <��>
//  �������   : <��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����     :
//==============================================================================
void InitDMA(void)
{    
    DMACTL0 |= DMA0TSEL_16;
    DMA0SA =  (unsigned int*)UCA0RXBUF_ ;                        //  Start block address  
    //DMA0DA =  g_sRxBuff[COMM_INDEX_RF].m_gBuffer;               // Destination block address0
    DMA0DA =  g_sRxBuff[COMM_INDEX_GPRS].m_gBuffer;
    DMA0SZ =  128;                                               // Block size
    DMA0CTL = DMADT_4 + DMASRCINCR_0 +DMADSTINCR_3 + DMASBDB  + DMALEVEL ; // Rpt, inc DST   
    DMA0CTL |= DMAEN;

    DMACTL0 |= DMA1TSEL_20;
    DMA1SA =  (unsigned int*)UCA1RXBUF_ ;                        //  Start block address  
    DMA1DA =  g_sRxBuff[COMM_INDEX_232].m_gBuffer;               // Destination block address0
    DMA1SZ =  128;                                               // Block size
    DMA1CTL = DMADT_4 + DMASRCINCR_0 + DMADSTINCR_3 + DMASBDB   + DMALEVEL ; // Rpt, inc DST    
    
    DMA2SA = g_unTX_Buff;                 // Start block address
//    DMA2SA = g_sTxBuff[COMM_INDEX_232].m_gBuffer;                 // Start block address
    DMA2DA = (unsigned int*)UCA1TXBUF_;                          // Destination block address
    DMA2SZ = 7;                                                  // Block size
    DMA2CTL = DMADT_1 + DMASRCINCR_3 + DMADSTINCR_0 + DMASBDB + DMALEVEL;  // once, inc SRC 

    DMACTL1 |= DMA2TSEL_21;
}