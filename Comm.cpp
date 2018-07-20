/*------------------------------------------------------------------------
 Module:      Comm.cpp
 Author:      linxueqin
 Project:     子站平台
 State:       V1.0
 Creation Date:   2014-4-29
 Description:   串口初始化，接收数据，发送数据，RF通道各指示器通信状态检测
------------------------------------------------------------------------*/

#ifndef _COMM
#define _COMM
  #include "DataStruct.h"
#endif

//==============================================================================
//  函数名称   : InitComm
//  功能描述   : 通信模块初始化，包括对缓冲区的初始化
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：l
//==============================================================================
void InitComm(void)
{
    InitCommProgram();
    InitSCI();
    
}

//==============================================================================
//  函数名称   : CrcCount
//  功能描述   : 根据传递进来的数组指针p，计算长度为length字的CRC校验码
//  输入参数   : p－需校验数据的头指针
//           length－需校验数据的长度
//  输出参数   :<无>
//  返回值     : crcvalue:校验后得到的CRC校验码(16位)
//  其他说明   : 
//  作者       ：
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
            if((AL & 0x01) == 1)    // AL最低位为1
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
    return ((unsigned int)(AL * 0x100 + AH));       //低位在前，高位在后

}
//==============================================================================
//  函数名称   : XorChar
//  功能描述   : 根据传递进来的数组指针p，计算长度为length的加异或校验码
//  输入参数   : p－需校验数据的头指针
//           length－需校验数据的长度
//  输出参数   :<无>
//  返回值     : AddValue:校验后得到的加和校验码(取8位存储)
//  其他说明   : 
//  作者       ：
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
//  函数名称   : ASCIIToHex
//  功能描述   : 根据字符转换成十六进制,只转换了数字部分
//  
//  作者       ：
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
//  函数名称   : ASCIIToHex
//  功能描述   : 根据字符转换成十六进制,只转换了数字部分三个字节
//  
//  作者       ：
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
//  函数名称   : HexToChar
//  功能描述   : 根据十六进制转换成字符
//  
//  作者       ：
//==============================================================================

unsigned char HexToChar(unsigned char bHex)
{
  if(bHex<=9)
  bHex += 0x30;
  else if((bHex>=10)&&(bHex<=15))//大写字母
  bHex += 0x37;
  else bHex = 0xff;
  return bHex;
}

//==============================================================================
//  函数名称   : AddChar
//  功能描述   : 根据传递进来的数组指针p，计算长度为length的加和校验码
//  输入参数   : p－需校验数据的头指针
//           length－需校验数据的长度
//  输出参数   :<无>
//  返回值     : AddValue:校验后得到的加和校验码(取8位存储)
//  其他说明   : 
//  作者       ：
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
//  函数名称   : BuffPosition
//  功能描述   : 缓冲区中位置的计算
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
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
//  函数名称   : InitCommProgram
//  功能描述   : 通信部分数据初始化，包括标志位与初始数据
//  输入参数   : <无>
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       ：
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
    if(uartId >= COMM_PORT_NUM)//端口号错误
        return 0;
    if((g_sRxBuff[uartId].m_iRcvFlag == ON) || (g_bRecFrameEBFlag[uartId] == 0x55))  //增加驱动内解包标志位，只解加密EB帧
    {
        if(g_bRecFrameEBFlag[uartId] == 0x55)
        {//解EB帧得到EB帧长度
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
    if(uartId >= COMM_PORT_NUM)//不允许发送到无效串口 
      return 0;

    if((uartId == g_CmIdGPRS) && !g_GprsPowerSt)//for test 模拟GPRS关闭状态，关闭时子站应该不发数据
        return 0;
              
    //若是当前串口正在发数据，等待其发完再将数据放入发送链表
   if(g_sTxBuff[uartId].m_unSendFlag)
   {
      while(1)
      {
        if(!g_sTxBuff[uartId].m_unSendFlag)//若当前未启动数据发送或数据发送完成
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
             g_sTxBuff[uartId].m_ucTx_Head++;//若尾指针追上头指针，则头指针向后推移
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
        //g_sTxBuff[uartId].m_ucTx_Head++;//此处有问题，需在串口中断中++,波特率高时容易出问题
        //g_sTxBuff[uartId].m_ucTx_Head &= (MAX_RXBUFF_LEN - 1);
     }  
     
  //将RF口或ODU口发出的数据转发到调试口
  if(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF])
  {
    if((uartId == g_CmIdRF) ||(uartId == g_CmIdGps))
        CommSendData(pBuf,BufLen,g_CmIdDBG);
    
    if((uartId == g_CmIdODU1 || uartId == g_CmIdODU2) && (BufLen > 5))
        CommSendData(pBuf,BufLen,g_CmIdDBG);//ODU 1S发一次召二级数据，这个报文不转发

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
//  函数名称   : CheckCommSt
//  功能描述   : 检测指示器通信状态
//  输入参数   : 
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
//==============================================================================

  void CheckCommSt(void)
  {//判断指示器通道状态
    int i;
    int YxIndex = 0;
    int YcIndex = 0;
    int line = 0;//线路号
    int iTime = 0;
    for(i = 0; i < RMT_ZSQST_NUM;i++)
    {
        YxIndex = i+RI_1A_ZSQST;
        line = i/PHASE_NUM;
         if(g_sCommStatCount[i] == g_gRunPara[RP_ZSQCST_T] && GetRIStatus(YxIndex) == 1)
         {//通道故障恢复
              CreatNewSoe(YxIndex,0);
              //判断总故障状态恢复，所有分相状态恢复，总状态才恢复
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
              {//指示器通道状态检测周期发生变化
                  iTime = g_gRunPara[RP_ZSQCST_T] - (g_gRunParaZsqStTBk - g_sCommStatCount[i]);
                if(iTime < 0)  iTime = 1;
                g_sCommStatCount[i] = iTime;
              }
              g_sCommStatCount[i]--;
              if(g_sCommStatCount[i] == 0 && GetRIStatus(YxIndex) == 0)
              {//通道故障
                  CreatNewSoe(YxIndex,1);
                  //判断总故障状态，有一个分相故障，则总状态为故障
                  if(GetRIStatus(line+ RI_1ZSQST) == 0)
                    CreatNewSoe(line+ RI_1ZSQST,1);
                    if(!(g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_COMMFLTVAL_MODE]))
                    {
                      //对应的遥测设置成无效
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
//根据设备通道状态的遥信序号得到遥测序号 IorT标识需要转化的为电流还是温度
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
    if(IorT == 1)//若是温度
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
//根据遥测序号得到设备通道状态的遥信序号 
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
    
    //if(IorT == 1)//若是温度
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
//  函数名称   : LPFToYxId
//  功能描述   : 根据线偏，相偏，故障编号得到对应的遥信点在遥信总表中的序号
//  输入参数   : bLine和bPhase最小值=1 ，bFtType最小值=0
//  输出参数   ：<无>
//  返回值     : 遥信在总表中的序号
//  其他说明   : bLine:1-12指示器线路号，报文中解析所得
//               bPhase:1-4分别表示A,B,C,O;
//               bFtType:0-5分别表示短路，接地，低压(指示器)，温度，有电(电场)，备用
//  作者       
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
//  函数名称   : SetAllYxStatus
//  功能描述   : 设置线路总遥信状态以及相故障，如，线路1总短路，线路1总接地
//  输入参数   : 分相遥信点，如，1A短路，1A接地
//  输出参数   ：<无>
//  返回值     : <无>
//  其他说明   : 
//  作者       
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

    //线总计算
    wYxIndex = Line - 1 + LINE_NUM*Fault + RI_1SHORT;
    wFirstYx = LPFToYxId(Line,1,Fault);//同一类故障的A,B,C,O连续排列，因此取出A相对应的点号
    bYxVal = GetRIStatus(wFirstYx) || GetRIStatus(wFirstYx + 1)
               ||GetRIStatus(wFirstYx + 2) ||GetRIStatus(wFirstYx + 3);
    CreatNewSoe(wYxIndex,bYxVal);

     if(Fault == 0 || Fault == 1|| Fault == 5)//遥信点为短路或接地时才合成故障遥信还有永久性短路
     {
      //分相故障计算
      wYxIndex = (Line - 1)*PHASE_NUM + (Phase -1) + RI_1A_FLT;
      bYxVal = GetRIStatus(LPFToYxId(Line,Phase,0)) || GetRIStatus(LPFToYxId(Line,Phase,1)) || GetRIStatus(LPFToYxId(Line,Phase,5));
      CreatNewSoe(wYxIndex,bYxVal);
      
      //线故障计算
      wYxIndex = Line - 1 + RI_1FLT;
      wFirstYx = RI_1A_FLT + (Line - 1)*PHASE_NUM;//同一类故障的A,B,C,O连续排列，因此取出A相对应的点号
      bYxVal = GetRIStatus(wFirstYx) || GetRIStatus(wFirstYx + 1)
                    ||GetRIStatus(wFirstYx + 2) || GetRIStatus(wFirstYx + 3);
      CreatNewSoe(wYxIndex,bYxVal);

      //事故总
      wYxIndex = RI_TOTAL;
      //wFirstYx = RI_1FLT + (Line - 1);//同一类故障的A,B,C,O连续排列，因此取出A相对应的点号
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
       Line = (RYxNo - RI_1A_BATPOW_EN)/PHASE_NUM;  //从0开始计算
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


