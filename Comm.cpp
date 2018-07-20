/*------------------------------------------------------------------------
 Module:      Comm.cpp
 Author:      linxueqin
 Project:     ��վƽ̨
 State:       V1.0
 Creation Date:   2014-4-29
 Description:   ���ڳ�ʼ�����������ݣ��������ݣ�RFͨ����ָʾ��ͨ��״̬���
------------------------------------------------------------------------*/

#ifndef _COMM
#define _COMM
  #include "DataStruct.h"
#endif

//==============================================================================
//  ��������   : InitComm
//  ��������   : ͨ��ģ���ʼ���������Ի������ĳ�ʼ��
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��l
//==============================================================================
void InitComm(void)
{
    InitCommProgram();
    InitSCI();
    
}

//==============================================================================
//  ��������   : CrcCount
//  ��������   : ���ݴ��ݽ���������ָ��p�����㳤��Ϊlength�ֵ�CRCУ����
//  �������   : p����У�����ݵ�ͷָ��
//           length����У�����ݵĳ���
//  �������   :<��>
//  ����ֵ     : crcvalue:У���õ���CRCУ����(16λ)
//  ����˵��   : 
//  ����       ��
//==============================================================================
unsigned int CrcCount(unsigned int *p, unsigned int Length)
{
    unsigned char AH = 0xFF, AL = 0xFF;
    unsigned char GH = 0xA0, GL = 0x01;
    unsigned char i, j;
    for(i = 0;i < Length;i++, p++)
    {
        AL = (*p) ^ AL;
        for(j = 0;j < 8;j++)
        {
            if((AL & 0x01) == 1)    // AL���λΪ1
            {
                AL = AL >> 1;
                if(AH & 0x01 == 1)
                    AL = AL | 0x80;
                AH = AH >> 1;
                AH = AH ^ GH;
                AL = AL ^ GL;
            }
            else
            {
                AL = AL >> 1;
                if((AH & 0x01) == 1)
                    AL = AL | 0x80;
                AH = AH >> 1;
            }
        }
    }
    return ((unsigned int)(AL * 0x100 + AH));       //��λ��ǰ����λ�ں�

}
//==============================================================================
//  ��������   : XorChar
//  ��������   : ���ݴ��ݽ���������ָ��p�����㳤��Ϊlength�ļ����У����
//  �������   : p����У�����ݵ�ͷָ��
//           length����У�����ݵĳ���
//  �������   :<��>
//  ����ֵ     : AddValue:У���õ��ļӺ�У����(ȡ8λ�洢)
//  ����˵��   : 
//  ����       ��
//==============================================================================
unsigned char XorChar(unsigned char *p, unsigned int length)
{
    unsigned int AddValue = 0;
    unsigned int i;
    //unsigned char GPS_CRC[2];//GPS_CRCH,GPS_CRCL
    for(i = 0; i < length; i++, p++)
    {
        AddValue ^= *p;
    }
    //GPS_CRC[0]=(GPS_CRC >> 4);//+0x30; 
    //GPS_CRC[1]=(GPS_CRC & 0x0F);//+0x30; 
    
    //AddValue+= TYPE_EQU;
    return (unsigned char)AddValue;    
}
//==============================================================================
//  ��������   : ASCIIToHex
//  ��������   : �����ַ�ת����ʮ������,ֻת�������ֲ���
//  
//  ����       ��
//==============================================================================

unsigned char CharToHex(unsigned char charl,unsigned char charh)
{
	unsigned char bHex;
  	if((charl<=0x39 &&charl>=0x30) &&(charh<=0x39 &&charh>=0x30))
  	{
  		charl -= 0x30;
  		charh -= 0x30;
		bHex = (charl &0x0F) + (charh *10);
  	}
  	else bHex = 0xff;
  	return bHex;
}
//==============================================================================
//  ��������   : ASCIIToHex
//  ��������   : �����ַ�ת����ʮ������,ֻת�������ֲ��������ֽ�
//  
//  ����       ��
//==============================================================================

unsigned int Char3ToHex(unsigned char char1,unsigned char char2,unsigned char char3)
{
	unsigned int bHex;
  	if((char1<=0x39 &&char1>=0x30) &&(char2<=0x39 &&char2>=0x30)&&(char3<=0x39 &&char3>=0x30))
  	{
  		char1 -= 0x30;
  		char2 -= 0x30;
		char3 -= 0x30;
		bHex = (char1 &0x0F) + (char2 *10) + (char3 *100);
  	}
  	else bHex = 0xffff;
  	return bHex;
}


//==============================================================================
//  ��������   : HexToChar
//  ��������   : ����ʮ������ת�����ַ�
//  
//  ����       ��
//==============================================================================

unsigned char HexToChar(unsigned char bHex)
{
  if(bHex<=9)
  bHex += 0x30;
  else if((bHex>=10)&&(bHex<=15))//��д��ĸ
  bHex += 0x37;
  else bHex = 0xff;
  return bHex;
}

//==============================================================================
//  ��������   : AddChar
//  ��������   : ���ݴ��ݽ���������ָ��p�����㳤��Ϊlength�ļӺ�У����
//  �������   : p����У�����ݵ�ͷָ��
//           length����У�����ݵĳ���
//  �������   :<��>
//  ����ֵ     : AddValue:У���õ��ļӺ�У����(ȡ8λ�洢)
//  ����˵��   : 
//  ����       ��
//==============================================================================
unsigned char AddChar(unsigned char *p, unsigned int length)
{
    unsigned int AddValue = 0;
    unsigned int i;
    for(i = 0; i < length; i++, p++)
    {
        AddValue += *p;
    }
    AddValue+= TYPE_EQU;
    return (unsigned char)AddValue;    
}

//==============================================================================
//  ��������   : BuffPosition
//  ��������   : ��������λ�õļ���
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
//unsigned int BuffPosition(unsigned int unpointer)
//{
//    // if Rx buff reach its end, restart from the beginning.
//    if( unpointer >= MAX_RXBUFF_LEN)
//        unpointer %= MAX_RXBUFF_LEN;
//
//    return unpointer;
//}


//==============================================================================
//  ��������   : InitCommProgram
//  ��������   : ͨ�Ų������ݳ�ʼ����������־λ���ʼ����
//  �������   : <��>
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       ��
//==============================================================================
void InitCommProgram(void)
{
    unsigned int i,j;
    for(i=0;i< COMM_PORT_NUM;i++)
    {
        g_sRxBuff[i].m_iBufH = 0;
        g_sRxBuff[i].m_iBufT = 0;
        g_sRxBuff[i].m_iRcvCount = 0;
        g_sRxBuff[i].m_iRcvFlag = OFF;
        g_sTxBuff[i].m_ucTx_Head = 0;
        g_sTxBuff[i].m_ucTx_Tail = 0;
        g_sTxBuff[i].m_unSendFlag = 0;
		
        for(j=0;j < MAX_RXBUFF_LEN ; j++ )
        {
            g_sRxBuff[i].m_gBuf[j] = 0;
            g_sTxBuff[i].m_gBuffer[j] = 0;
        }
     }
		sRx_Oper.m_Operalength =0;
		sRx_Oper.m_OperaBufT=0;
		sRx_Oper.m_Operaflag =0;
     g_CmIdRF   = CMIDINV;
     g_CmIdGPRS = CMIDINV;
     g_CmIdEX  =  CMIDINV; 
     g_CmIdDBG =  CMIDINV; 
     g_CmIdODU1 = CMIDINV;
     g_CmIdODU2 = CMIDINV;
	 g_CmIdGps = CMIDINV;
     g_RstartNum = 0;
}
int CommRcvData(unsigned char *pBuf,int MaxLen,int uartId)
{
    int rtDaLen = 0;
    unsigned int BfH = g_sRxBuff[uartId].m_iBufH;
    unsigned int BfT = g_sRxBuff[uartId].m_iBufT;
    if(uartId >= COMM_PORT_NUM)//�˿ںŴ���
        return 0;
    if((g_sRxBuff[uartId].m_iRcvFlag == ON) || (g_bRecFrameEBFlag[uartId] == 0x55))  //���������ڽ����־λ��ֻ�����EB֡
    {
        if(g_bRecFrameEBFlag[uartId] == 0x55)
        {//��EB֡�õ�EB֡����
            if((g_sRxBuff[uartId].m_gBuf[g_sRxBuff[uartId].m_iBufH] == 0xEB) && (g_sRxBuff[uartId].m_gBuf[g_sRxBuff[uartId].m_iBufH + 3] == 0xEB))
            {                  
                rtDaLen += MAKEWORD(g_sRxBuff[uartId].m_gBuf[g_sRxBuff[uartId].m_iBufH + 2],g_sRxBuff[uartId].m_gBuf[g_sRxBuff[uartId].m_iBufH + 1]);
                rtDaLen += 6;
                //rtDaLen = 0;
            }
        }
        else
        {
            if(BfH <= BfT) 
                 rtDaLen = BfT - BfH;
            else
                rtDaLen = MAX_RXBUFF_LEN - BfH + BfT;
            
        }
        
        //g_sRxBuff[uartId].m_iRcvCount = 0;
        //g_sRxBuff[uartId].m_iRcvFlag = OFF;
        g_bRecFrameEBFlag[uartId] = 0;

        if(rtDaLen == 0) return 0;
        if(rtDaLen >= MaxLen) 
            rtDaLen = MaxLen;
    
        if(rtDaLen < (MAX_RXBUFF_LEN - BfH)) 
        {
            memcpy(pBuf,&(g_sRxBuff[uartId].m_gBuf[g_sRxBuff[uartId].m_iBufH]),rtDaLen);
            g_sRxBuff[uartId].m_iBufH += rtDaLen;
        }  
        else
        {
            memcpy(pBuf,&(g_sRxBuff[uartId].m_gBuf[g_sRxBuff[uartId].m_iBufH]), MAX_RXBUFF_LEN-BfH);
            memcpy(pBuf+(MAX_RXBUFF_LEN-BfH),&(g_sRxBuff[uartId].m_gBuf[0]),rtDaLen - (MAX_RXBUFF_LEN - BfH));
            g_sRxBuff[uartId].m_iBufH = rtDaLen - (MAX_RXBUFF_LEN - BfH);
        }
        if(g_sRxBuff[uartId].m_iBufH == g_sRxBuff[uartId].m_iBufT)
        {
            g_sRxBuff[uartId].m_iBufT = 0;
            g_sRxBuff[uartId].m_iBufH = 0;
        }
        
        if(g_sRxBuff[uartId].m_iBufH >= MAX_RXBUFF_LEN)
        {
            g_sRxBuff[uartId].m_iBufH -= MAX_RXBUFF_LEN;
        }
        //g_sRxBuff[uartId].m_iBufH &= (MAX_RXBUFF_LEN - 1);
        g_sRxBuff[uartId].m_iRcvFlag = OFF;
   }
  return rtDaLen;
}

int CommSendData(unsigned char *pBuf,int BufLen,int uartId)
{
    if(uartId >= COMM_PORT_NUM)//�������͵���Ч���� 
      return 0;

    if((uartId == g_CmIdGPRS) && !g_GprsPowerSt)//for test ģ��GPRS�ر�״̬���ر�ʱ��վӦ�ò�������
        return 0;
              
    //���ǵ�ǰ�������ڷ����ݣ��ȴ��䷢���ٽ����ݷ��뷢������
   if(g_sTxBuff[uartId].m_unSendFlag)
   {
      while(1)
      {
        if(!g_sTxBuff[uartId].m_unSendFlag)//����ǰδ�������ݷ��ͻ����ݷ������
          break;
      }
    }
    for(int i = 0;i<BufLen;i++)
    {
            g_sTxBuff[uartId].m_gBuffer[g_sTxBuff[uartId].m_ucTx_Tail] = pBuf[i];
            g_sTxBuff[uartId].m_ucTx_Tail++;
            g_sTxBuff[uartId].m_ucTx_Tail &= (MAX_TXBUFF_LEN - 1);
           if(g_sTxBuff[uartId].m_ucTx_Tail == g_sTxBuff[uartId].m_ucTx_Head) 
           {
             g_sTxBuff[uartId].m_ucTx_Head++;//��βָ��׷��ͷָ�룬��ͷָ���������
             g_sTxBuff[uartId].m_ucTx_Head &= (MAX_TXBUFF_LEN - 1);
           }
     }
     if(g_sTxBuff[uartId].m_unSendFlag == 0)
     {
       g_sTxBuff[uartId].m_unSendFlag = 2;
       if(uartId == CMID0)
          UCA0TXBUF = g_sTxBuff[uartId].m_gBuffer[g_sTxBuff[uartId].m_ucTx_Head];
       else if(uartId == CMID1)
          UCA1TXBUF = g_sTxBuff[uartId].m_gBuffer[g_sTxBuff[uartId].m_ucTx_Head];
       else if(uartId == CMID2)
          UCA2TXBUF = g_sTxBuff[uartId].m_gBuffer[g_sTxBuff[uartId].m_ucTx_Head];
       else if(uartId == CMID3)
          UCA3TXBUF = g_sTxBuff[uartId].m_gBuffer[g_sTxBuff[uartId].m_ucTx_Head];
        //g_sTxBuff[uartId].m_ucTx_Head++;//�˴������⣬���ڴ����ж���++,�����ʸ�ʱ���׳�����
        //g_sTxBuff[uartId].m_ucTx_Head &= (MAX_RXBUFF_LEN - 1);
     }  
     
  //��RF�ڻ�ODU�ڷ���������ת�������Կ�
  if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])
  {
    if((uartId == g_CmIdRF) ||(uartId == g_CmIdGps))
        CommSendData(pBuf,BufLen,g_CmIdDBG);
    
    if((uartId == g_CmIdODU1 || uartId == g_CmIdODU2) && (BufLen > 5))
        CommSendData(pBuf,BufLen,g_CmIdDBG);//ODU 1S��һ���ٶ������ݣ�������Ĳ�ת��

    //if(uartId == g_CmIdGPRS)
      //CommSendData(pBuf,BufLen,g_CmIdDBG);
  }


  /*switch(uartId)
  {
     case CMID0:
      SendBytes_RF(pBuf,BufLen);    
      #ifdef RF_TEST
        CommSendData(pBuf,BufLen,RF_TEST_CMID);
      #endif
      break;
     case CMID1:
        SendBytes_GPRS(pBuf,BufLen);
      break;
     case CMID2:
      SendBytes_UART2(pBuf,BufLen);
      break;
    case CMID3:
      SendBytes_232(pBuf,BufLen);
      break;
  default:
    break;
  }*/

  return 1;
}
//==============================================================================
//  ��������   : CheckCommSt
//  ��������   : ���ָʾ��ͨ��״̬
//  �������   : 
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================

  void CheckCommSt(void)
  {//�ж�ָʾ��ͨ��״̬
    int i;
    int YxIndex = 0;
    int YcIndex = 0;
    int line = 0;//��·��
    int iTime = 0;
    for(i = 0; i < RMT_ZSQST_NUM;i++)
    {
        YxIndex = i+RI_1A_ZSQST;
        line = i/PHASE_NUM;
         if(g_sCommStatCount[i] == g_gRunPara[RP_ZSQCST_T] && GetRIStatus(YxIndex) == 1)
         {//ͨ�����ϻָ�
              CreatNewSoe(YxIndex,0);
              //�ж��ܹ���״̬�ָ������з���״̬�ָ�����״̬�Żָ�
              int FltFlag = 0;
              if(GetRIStatus(line+ RI_1ZSQST) == 1)
              {
                 for(int j = 0; j < PHASE_NUM;j++)
                 {
                   if(GetRIStatus(line*PHASE_NUM + j + RI_1A_ZSQST) ==1)
                   {
                      FltFlag = 1;
                      break;
                   }
                 }
                 if(FltFlag == 0)
                    CreatNewSoe(line+ RI_1ZSQST,0);
              }   
         }
         else if(g_sCommStatCount[i] > 0)
         {
              if(g_gRunParaZsqStTBk != g_gRunPara[RP_ZSQCST_T])
              {//ָʾ��ͨ��״̬������ڷ����仯
                  iTime = g_gRunPara[RP_ZSQCST_T] - (g_gRunParaZsqStTBk - g_sCommStatCount[i]);
                if(iTime < 0)  iTime = 1;
                g_sCommStatCount[i] = iTime;
              }
              g_sCommStatCount[i]--;
              if(g_sCommStatCount[i] == 0 && GetRIStatus(YxIndex) == 0)
              {//ͨ������
                  CreatNewSoe(YxIndex,1);
                  //�ж��ܹ���״̬����һ��������ϣ�����״̬Ϊ����
                  if(GetRIStatus(line+ RI_1ZSQST) == 0)
                    CreatNewSoe(line+ RI_1ZSQST,1);
                    if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_COMMFLTVAL_MODE]))
                    {
                      //��Ӧ��ң�����ó���Ч
                      YcIndex = YxIdToYcId(YxIndex);
                      if(YcIndex == 255) 
                        continue;
                      g_gRmtMeas[YcIndex + RM_1A_FAULT_I] = INVALIDYC;
                      g_gRmtMeas[YcIndex + RM_1A_LOAD_I] = INVALIDYC;				  
					  g_gRmtMeas[YcIndex + RM_1A_LUBO] = INVALIDYC;
                      g_ucRefYxYcTs |= BIT1;
                    }
              }
         }
    }//end for
    if(g_gRunParaZsqStTBk != g_gRunPara[RP_ZSQCST_T]) 
      g_gRunParaZsqStTBk = g_gRunPara[RP_ZSQCST_T];
  }
//�����豸ͨ��״̬��ң����ŵõ�ң����� IorT��ʶ��Ҫת����Ϊ���������¶�
unsigned int YxIdToYcId(unsigned int RYxNo,unsigned int IorT)
{
   unsigned int wYcIndex = 255;
   unsigned char Line,Phase;
   if(RYxNo >= RI_1A_ZSQST && RYxNo <= RI_ZSQST_H)
   {
    Line = (RYxNo - RI_1A_ZSQST)/PHASE_NUM;
    Phase = (RYxNo - RI_1A_ZSQST - Line * PHASE_NUM)%PHASE_NUM;
    Line += 1;
    Phase += 1;
    if(IorT == 1)//�����¶�
     {
       if(Phase!=4)
        wYcIndex = (Line-1)*(PHASE_NUM-1) + (Phase-1);
     }
    else
      wYcIndex = (Line-1)*PHASE_NUM + (Phase-1);
   }
//       else if(RYxNo >= RI_1A_SHORT && RYxNo <= RI_3O_LVD)
//       {
//        Line = (RYxNo - RI_1A_SHORT)/(PHASE_NUM * FTTPYE_NUM);
//        Phase = (RYxNo - RI_1A_SHORT - Line * PHASE_NUM * FTTPYE_NUM)/FTTPYE_NUM;
//        Line += 1;
//        Phase += 1;
//       }
    
   return wYcIndex;
}
//����ң����ŵõ��豸ͨ��״̬��ң����� 
unsigned int YcIdToYxId(unsigned int RYxNo)
{
   unsigned int wYxIndex = 255;
   unsigned char Line,Phase;
   if(RYxNo >= RM_1A_FAULT_I && RYxNo < RMT_MEAS_NUM)
   {
    if(RYxNo >= RM_1A_FAULT_I && RYxNo <= RM_3O_FAULT_I)//RM_12O_FAULT_I
    {
     Line = (RYxNo - RM_1A_FAULT_I)/PHASE_NUM;
     Phase = (RYxNo - RM_1A_FAULT_I - Line * PHASE_NUM)%PHASE_NUM;
    }
    else if(RYxNo >= RM_1A_LOAD_I && RYxNo <= RM_3O_LOAD_I)//RM_12O_LOAD_I
    {
     Line = (RYxNo - RM_1A_LOAD_I)/PHASE_NUM;
     Phase = (RYxNo - RM_1A_LOAD_I - Line * PHASE_NUM)%PHASE_NUM;
    }
	
	if(RYxNo >= RM_1A_LUBO && RYxNo <= RM_3C_LUBO)//RM_12O_FAULT_I
    {
     Line = (RYxNo - RM_1A_LUBO)/PHASE_NUM;
     Phase = (RYxNo - RM_1A_LUBO - Line * PHASE_NUM)%PHASE_NUM;
    }	
   /* else if(RYxNo >= RM_1A_CABLE_T && RYxNo <= RM_3C_CABLE_T)//RM_12C_CABLE_T)
    {
     Line = (RYxNo - RM_1A_CABLE_T)/TEMP_PHASE_NUM;
     Phase = (RYxNo - RM_1A_CABLE_T - Line * TEMP_PHASE_NUM)%TEMP_PHASE_NUM;
    }*/
    else if(RYxNo >= RM_1A_CABLE_T_INT && RYxNo <= RM_3C_CABLE_T_INT)//RM_12C_CABLE_T_INT)
    {
     Line = (RYxNo - RM_1A_CABLE_T_INT)/TEMP_PHASE_NUM;
     Phase = (RYxNo - RM_1A_CABLE_T_INT - Line * TEMP_PHASE_NUM)%TEMP_PHASE_NUM;
    }
    Line += 1;
    Phase += 1;
    //if(Line < 1 || Phase < 1) return RI_IN_DAERR;
    wYxIndex = RI_1A_ZSQST + (Line - 1)*PHASE_NUM + (Phase -1);
    
    //if(IorT == 1)//�����¶�
       //wYcIndex = (Line-1)*(PHASE_NUM-1) + (Phase-1);
    //else
      //wYcIndex = (Line-1)*PHASE_NUM + (Phase-1);
   }
//       else if(RYxNo >= RI_1A_SHORT && RYxNo <= RI_3O_LVD)
//       {
//        Line = (RYxNo - RI_1A_SHORT)/(PHASE_NUM * FTTPYE_NUM);
//        Phase = (RYxNo - RI_1A_SHORT - Line * PHASE_NUM * FTTPYE_NUM)/FTTPYE_NUM;
//        Line += 1;
//        Phase += 1;
//       }
    
   return wYxIndex;
}

//==============================================================================
//  ��������   : LPFToYxId
//  ��������   : ������ƫ����ƫ�����ϱ�ŵõ���Ӧ��ң�ŵ���ң���ܱ��е����
//  �������   : bLine��bPhase��Сֵ=1 ��bFtType��Сֵ=0
//  �������   ��<��>
//  ����ֵ     : ң�����ܱ��е����
//  ����˵��   : bLine:1-12ָʾ����·�ţ������н�������
//               bPhase:1-4�ֱ��ʾA,B,C,O;
//               bFtType:0-5�ֱ��ʾ��·���ӵأ���ѹ(ָʾ��)���¶ȣ��е�(�糡)������
//  ����       
//==============================================================================
WORD LPFToYxId(BYTE bLine,BYTE bPhase,BYTE bFtType)
{
   WORD wYxId = 0;
    if(bLine < 1 || bPhase < 1) return RI_IN_DAERR;
    ////if(bFtType == 3)
        ////wYxId = RI_1A_STATUS + (bLine - 1)*PHASE_NUM + (bPhase -1);
    ////else
        wYxId = bFtType *(LINE_NUM * PHASE_NUM) + (bLine - 1)*PHASE_NUM + (bPhase -1);
    
    //if((wYxId > RI_12O_RSV) && (wYxId < RI_1A_STATUS))
      //wYxId = RI_IN_DAERR;
   return wYxId;
}

//==============================================================================
//  ��������   : SetAllYxStatus
//  ��������   : ������·��ң��״̬�Լ�����ϣ��磬��·1�ܶ�·����·1�ܽӵ�
//  �������   : ����ң�ŵ㣬�磬1A��·��1A�ӵ�
//  �������   ��<��>
//  ����ֵ     : <��>
//  ����˵��   : 
//  ����       
//==============================================================================

void SetAllYxStatus(WORD RYxNo)
{
   unsigned char Line,Phase,Fault;
   WORD wYxIndex;
   BYTE bYxVal;
   WORD wFirstYx;
   
   WORD wYxNoOffset;
   if((int)RYxNo >= RI_1A_SHORT && RYxNo <= RI_3O_POWERCUT)//RI_3O_RSVRI_12O_RSV
   {
    wYxNoOffset = RYxNo - RI_1A_SHORT;
    Line  =  (wYxNoOffset%(LINE_NUM*PHASE_NUM))/PHASE_NUM + 1;
    Phase = wYxNoOffset%PHASE_NUM + 1;
    Fault = wYxNoOffset/(PHASE_NUM*LINE_NUM);

    //���ܼ���
    wYxIndex = Line - 1 + LINE_NUM*Fault + RI_1SHORT;
    wFirstYx = LPFToYxId(Line,1,Fault);//ͬһ����ϵ�A,B,C,O�������У����ȡ��A���Ӧ�ĵ��
    bYxVal = GetRIStatus(wFirstYx) || GetRIStatus(wFirstYx + 1)
               ||GetRIStatus(wFirstYx + 2) ||GetRIStatus(wFirstYx + 3);
    CreatNewSoe(wYxIndex,bYxVal);

     if(Fault == 0 || Fault == 1|| Fault == 5)//ң�ŵ�Ϊ��·��ӵ�ʱ�źϳɹ���ң�Ż��������Զ�·
     {
      //������ϼ���
      wYxIndex = (Line - 1)*PHASE_NUM + (Phase -1) + RI_1A_FLT;
      bYxVal = GetRIStatus(LPFToYxId(Line,Phase,0)) || GetRIStatus(LPFToYxId(Line,Phase,1)) || GetRIStatus(LPFToYxId(Line,Phase,5));
      CreatNewSoe(wYxIndex,bYxVal);
      
      //�߹��ϼ���
      wYxIndex = Line - 1 + RI_1FLT;
      wFirstYx = RI_1A_FLT + (Line - 1)*PHASE_NUM;//ͬһ����ϵ�A,B,C,O�������У����ȡ��A���Ӧ�ĵ��
      bYxVal = GetRIStatus(wFirstYx) || GetRIStatus(wFirstYx + 1)
                    ||GetRIStatus(wFirstYx + 2) || GetRIStatus(wFirstYx + 3);
      CreatNewSoe(wYxIndex,bYxVal);

      //�¹���
      wYxIndex = RI_TOTAL;
      //wFirstYx = RI_1FLT + (Line - 1);//ͬһ����ϵ�A,B,C,O�������У����ȡ��A���Ӧ�ĵ��
      bYxVal=0;
      for(int i = 0; i < LINE_NUM;i++)
      {
        bYxVal=(bYxVal || GetRIStatus(RI_1FLT + i));
      }
      
      CreatNewSoe(wYxIndex,bYxVal);

      
    }
   }
   else if((int)RYxNo >= RI_1A_BATPOW_EN && RYxNo <= RI_3O_BATPOW_EN)
   {
       Line = (RYxNo - RI_1A_BATPOW_EN)/PHASE_NUM;  //��0��ʼ����
       wFirstYx = RI_1A_BATPOW_EN + Line*PHASE_NUM;
       bYxVal = GetRIStatus(wFirstYx) || GetRIStatus(wFirstYx + 1)
               ||GetRIStatus(wFirstYx + 2) ||GetRIStatus(wFirstYx + 3); 
       CreatNewSoe(RI_1_BATPOW_EN+Line,bYxVal);
       bYxVal = 0;
       for(int i = 0; i < LINE_NUM;i++)
       {
          bYxVal=(bYxVal || GetRIStatus(RI_1_BATPOW_EN + i));
       }
       CreatNewSoe(RI_BATPOW_EN,bYxVal);       
   }
}



unsigned short Fcs16(unsigned short fcs, unsigned char *cp, unsigned short len)
{
    while (len--)
    {
        fcs = (fcs >> 8) ^ fcsTab[(fcs ^ *cp++) & 0xff];
    }
    return (fcs);
}

unsigned short ByAddCrc16(unsigned char *cp, unsigned short len)
{
    unsigned short trialfcs;

    /* add on output */
    trialfcs = Fcs16( 0xFFFF, cp, len );
    trialfcs ^= 0xffff;                 /* complement */
    cp[len] = (trialfcs & 0x00ff);      /* least significant byte first */
    cp[len+1] = ((trialfcs >> 8) & 0x00ff);
    return 0;
}
void MyPrintf(char *ch)
{
#if 0
  WORD wLen = strlen(ch);
  CommSendData((unsigned char *)ch,wLen,g_CmIdDBG);
#endif
}
void MyPrintfD(BYTE *p,WORD wLen)
{
  if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTDUG])
    CommSendData((unsigned char *)p,wLen,g_CmIdDBG);
}


