/*------------------------------------------------------------------------
 Module:            PrtcOdu.cpp
 Author:            linxueqin
 Project:           ��Լƽ̨
 State:             V1.0
 Creation Date:     2015-3-16
 Description:       ��ODUͨ��--primary
------------------------------------------------------------------------*/




#ifndef _PRTC_ODU

  #define _PRTC_ODU
  #include "PrtcOdu.h"
#include "..\Main.h"
//���캯��
CPrtcOdu::CPrtcOdu() : CProtocol()
{
  m_OduAddr = 0;
  m_open_gprs = 0;
}


void CPrtcOdu::Run(void)
{
    
    RcvData(&m_uartId);
    DoCommSendIdle();
}

/***************************************************************
    Function��Init
        ��Լ��ʼ��
    ������
    ���أ�TRUE �ɹ���FALSE ʧ��
***************************************************************/
BOOL CPrtcOdu::Init(WORD uartId)
{
    BOOL rc;
    m_uartId = uartId;
    if(uartId == g_CmIdODU1)  
      m_OduAddr = 1; 
    else if(uartId == g_CmIdODU2)  
      m_OduAddr = 2;
    rc = CProtocol::Init(1);
    if (!rc)
    {
      return FALSE;
    }
    
    m_ODUCall2Flag = 0;
    m_ODUCallFlag = 0;
    m_WrPaWaitflag = 0;
    m_ReTxNum = 0;
   
    if(m_uartId == g_CmIdODU1)
    {
      m_ODUCall2Bit = BIT0;
      m_ODUCallBit= BIT0;

    }
    else if(m_uartId == g_CmIdODU2)
    { 
      m_ODUCall2Bit = BIT1;
      m_ODUCallBit= BIT1;
    }
    return TRUE;
}

/***************************************************************
    Function��DoReceive
        ���ձ��Ĵ���
    ��������

    ���أ�TRUE �ɹ���FALSE ʧ��
***************************************************************/
  BOOL CPrtcOdu::DoReceive()
  {
     while(1)
     {
        if (SearchFrame() != TRUE)
        return FALSE;
        
        pReceiveFrame = (VIecOduFrame *)m_RecFrame.pBuf;
        if (pReceiveFrame->Frame67.Start1 == 0x67)
        {
             RecFrame67(&pReceiveFrame->Frame67);
             continue;
        }
        if (pReceiveFrame->Frame69.Start1 == 0x69)
        {
          RecFrame69();
          continue;
        }
     
     }
  }
/***************************************************************
    Function��RecFrame67
        ����67֡
    ��������

    ���أ�TRUE �ɹ���FALSE ʧ��
***************************************************************/

BOOL CPrtcOdu::RecFrame67(VFrame67 *pframe67)
{
    unsigned char Type_odu = pframe67->Type_odu;
  //unsigned char Addr_odu = pframe67->Addr_odu;
    static BYTE bJYbk = 0;//ң���б仯��ת��
    BYTE bBuf[2] = {0,0};
    if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF]) && (Type_odu != 0x15) && (Type_odu != 0x64))
    {//�����˽�ODU�յ�������ת�������Կ� ң�ⲻת�� ��Ӧ���ٺͽ������ٲ�ת��
      BYTE bVal = m_OduAddr | 0xD0;
      MyPrintfD(&bVal,1);
      MyPrintfD(&(pReceiveFrame->Frame67.Start1),pReceiveFrame->Frame67.Length1+6);
    }

    if((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_PRINTF]) && (Type_odu == 0x15)&&(bJYbk != m_RecFrame.pBuf[pReceiveFrame->Frame67.Length1+4]))
      {//ң���б仯��ת��
         bJYbk = m_RecFrame.pBuf[pReceiveFrame->Frame67.Length1+4];
         BYTE bVal = m_OduAddr | 0xD0;
         MyPrintfD(&bVal,1);
         MyPrintfD(&(pReceiveFrame->Frame67.Start1),pReceiveFrame->Frame67.Length1+6);
      }
     
    
    switch(Type_odu)
    {
        case 0x01:
          RecODUFault(m_OduAddr);
          break;
        case 0x15://�������յ���ODUң�ⱨ��
              RecODUFault_yaoce(m_OduAddr);
            break;
        case 0x51://ODU��Դ
            RecODUPower_Supply(); 
            break;
         case 0x66://ODU״̬�㱨
            //RecODUPower_Supply();
            bBuf[0] = 0xEF;
            bBuf[1] = 0XEF;
            MyPrintfD(bBuf,2);
            CreatNewSoe(RI_STATE_CYCLE,1);
            OpenTimer(TM_SENDSTATE); //��ʱ1���Ӻ���ң�ű�0����Ϣ
            break;
        case 0x2E://ң��
             if(g_YkOrderFlag ==ON)
              g_YkOrderFlag=TRUE;
            break;
        case 0x7D://д��������
                RecOduWPa();
        case 0x7A://����������
                RecODUPara();
            break;
        default:
            break;
    }
    return TRUE;
}




//ͨ�����Ϳ��д���
void CPrtcOdu::DoCommSendIdle(void)
{
  if(m_ODUCallFlag == 1)
    {
        m_ODUCallFlag=0;
        SendODUCallFrame();//����
        return;
    }
    if(m_ODUCall2Flag == 1)
    {
          m_ODUCall2Flag=0;
          SendODUFrame();//�ٶ�������
      return;
    }

    //����GPRSʱ����Ϣ��ODU �µĵ�Դ�����в���Ҫ�ٷ�
//        if(m_open_gprs==0x55) 
//          SendWCmdToOdu(0,6);
//        else if(m_open_gprs==0x66)
//          SendWCmdToOdu(0,7);
//        m_open_gprs=0;

    //ODUѯ��ʱ�ظ����ӳٹض�ָ�֮���ն˴�������Ϣ��GPRS�Ѿ��رգ���������ODU���������������
    if(m_RcvCloseZdFlag == ON)
    {
      if((!g_batPowerSt)&&(CheckHaveDataToSd() == FALSE || !g_GprsPowerSt))
      {
        
        BYTE bBuf[3] = {0xDA,0XDA};
        bBuf[2] = m_OduAddr | 0xD0;
        MyPrintfD(bBuf,3);

        SendWCmdToOdu(0x55,3);
        m_RcvCloseZdFlag = OFF;//�ظ�ODU���Թ������˱�ʶ��λ 
      }
    }

    //д�����ط�
    #define MAX_RESENDNUM 3
    if(m_WrPaWaitflag == TRUE && m_WrPaWaitCt == 0)
    {
          if(m_ReTxNum < MAX_RESENDNUM)
          {
            
            m_WrPaWaitCt = 10;
            m_WrPaWaitflag = ON;
            SendOduRetry();
            m_ReTxNum++;
            return;
          }
          else
          {
            m_WrPaWaitflag = OFF;
            m_ReTxNum = 0;
            m_WrPaWaitCt = 0;
          }
      
     }
}


//ODUд�����ش�
void CPrtcOdu::SendOduRetry(void)
{
  CommWrite((char*)m_oduSendBuf,m_oduSendLen);
}
/***************************************************************
    Function��OnTimeOut
        ��ʱ����
    ������TimerID
        TimerID ��ʱ��ID
    ���أ���
***************************************************************/
BOOL CPrtcOdu::DoTimeOut(WORD wTimerID)
{
  if(m_WrPaWaitflag == ON)
  {
    if(m_WrPaWaitCt > 0)
      {//ODUд�����ط���ʱ
          m_WrPaWaitCt--;
          if(m_WrPaWaitCt <= 0)
          {
            m_WrPaWaitflag = TRUE;
            return TRUE;
          }
       }
   }
   
   if(g_sTimer[TM_ODU_call].m_TmFlag & m_ODUCallBit)
   {//����ODU����
     g_sTimer[TM_ODU_call].m_TmFlag &= ~m_ODUCallBit;
      m_ODUCallFlag = 1;
   }
   if(!m_ODUCallFlag)
   {//����һ�뷢�����پͲ����ٶ�������
     if(g_sTimer[TM_ODU_call2].m_TmFlag & m_ODUCall2Bit)
     {//����ODU�ٻ���������
       g_sTimer[TM_ODU_call2].m_TmFlag &= ~m_ODUCall2Bit;
       m_ODUCall2Flag = 1;
     }
  }
  if(g_sTimer[TM_ODU_call2].m_TmCount == 0)
      OpenTimer(TM_ODU_call2);
  if(g_sTimer[TM_ODU_call].m_TmCount == 0)
      OpenTimer(TM_ODU_call);
  if( m_ODUCall2Flag || m_ODUCallFlag)
    return TRUE;
  return FALSE;
}

//����д�����ɹ�
void CPrtcOdu::SendOWrPaSuc()
{
    m_SendBuf.wReadPtr = 0;
     m_SendBuf.wWritePtr=0;
    pSendFrame = (VIecOduFrame *)m_SendBuf.pBuf;
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame67.Start1,(pReceiveFrame->Frame67.Length1)+6);
    m_SendBuf.wWritePtr += (pReceiveFrame->Frame67.Length1)+6;
    
    if(m_RWPaSrcObj !=null)
      CommSendData(m_SendBuf.pBuf,m_SendBuf.wWritePtr,m_RWPaSrcObj->m_uartId);
    return;

}
//�����յ���д�ļ�ȷ�ϱ���
void CPrtcOdu::RecOduWPa(void)
{
  if(m_ReadPaTp == 3 || m_ReadPaTp == 6 || m_ReadPaTp == 7)
  {
    m_WrPaWaitflag = OFF;//�յ�д������ȷ�ϱ��ĺ�ر��ش�
    m_ReTxNum = 0;
    m_WrPaWaitCt = 0;
    SendOWrPaSuc();
  }
  else
  {
    if(m_RWPaSrcObj !=null)
      m_RWPaSrcObj->SendWrPaSuc(); 
  }

  
}

//ODU�ٶ�������
void CPrtcOdu::SendODUFrame(void)
{
    m_SendBuf.wReadPtr = m_SendBuf.wWritePtr = 0;

    #define ODUCALL2LEN 5
    BYTE bySdFrame[ODUCALL2LEN]={0x10,0x5B,0x01,0x5c,0x16};
  //BYTE bySdFrame[ODUCALL2LEN]={0x10,0x5B,0x02,0x5d,0x16};//ODU2    
  // bySdFrame[2] = m_OduAddr;

    memcpy(m_SendBuf.pBuf,bySdFrame,ODUCALL2LEN);
    m_SendBuf.wWritePtr += ODUCALL2LEN;
    CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,1);
}
//ODU����
void CPrtcOdu::SendODUCallFrame(void)
{
    m_SendBuf.wReadPtr = 0;
    m_SendBuf.wWritePtr=0;
    
    #define ODUCALLLEN 15
    BYTE bySdFrame[ODUCALLLEN]={0x67,0x09,0x09,0x67,0x53,0x01,0x64,0x01,0x06,0x01,0x00,0x00,0x14,0xD4,0x16};
  //BYTE bySdFrame[ODUCALLLEN]={0x67,0x09,0x09,0x67,0x53,0x02,0x64,0x01,0x06,0x02,0x00,0x00,0x14,0xD6,0x16};//ODU2 
  //  bySdFrame[5] = m_OduAddr;
  //  bySdFrame[9] = m_OduAddr;
    memcpy(m_SendBuf.pBuf,bySdFrame,ODUCALLLEN);
     
     m_SendBuf.wWritePtr +=ODUCALLLEN;
     CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,1);  
}


BOOL CPrtcOdu::SendFrameHead_ODU(BYTE Style, BYTE Reason,BYTE OduPara1, BYTE OduPara2, BYTE OduPara)
{
      m_SendBuf.wReadPtr = 0;
      m_SendBuf.wWritePtr=0;
      pSendFrame = (VIecOduFrame *)m_SendBuf.pBuf;
     
      //pSendFrame->Frame67.Start1  = pSendFrame->Frame67.Start2 = 0x67;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr]=0x67;
      m_SendBuf.wWritePtr+=3;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0x67;
    
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0x48;
      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]= 1;//m_OduAddr;

      m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=Style;
      if(Style == 0x2e)
      {
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=6;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0x24;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=0;
      }
      else
      {
          pSendFrame->Frame67.VSQ_odu=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=Reason;
      //if(OduPara==10)
          //m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=2;
      //else
         m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=OduPara1;
          m_SendBuf.pBuf[ m_SendBuf.wWritePtr++]=OduPara2;
      }

    return TRUE;
}


BOOL CPrtcOdu::SendFrameTail_ODU(BYTE Num,BYTE OduPara)
{
    pSendFrame->Frame67.Length1 = pSendFrame->Frame67.Length2 = m_SendBuf.wWritePtr - 4;
    if(Num==0)
      pSendFrame->Frame67.Control =0x53;
  else
    pSendFrame->Frame67.Control =0x48;
    

    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = (BYTE)ChkSum((BYTE *)&pSendFrame->Frame67.Control, pSendFrame->Frame67.Length1);
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x16;

    CommWrite((char *)m_SendBuf.pBuf,m_SendBuf.wWritePtr);
    return TRUE;
}


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
DWORD CPrtcOdu::SearchOneFrame(BYTE *Buf, WORD Len)
{
    unsigned short FrameLen;
    //WORD wLinkAddress;
    if (Len < 5)
        return FRAME_LESS;

    pReceiveFrame = (VIecOduFrame *)Buf;
    switch(pReceiveFrame->Frame67.Start1)
    {
        case 0x69:
            if (pReceiveFrame->Frame69.Length1 != pReceiveFrame->Frame69.Length2)
                return FRAME_ERR|1;
            if (pReceiveFrame->Frame69.Start2 != 0x69)
                return FRAME_ERR|1;
            FrameLen=pReceiveFrame->Frame69.Length1+6;
            if (FrameLen > Len)
                return FRAME_LESS;
            if (Buf[FrameLen-1] != 0x16)
                return FRAME_ERR|1;
            if (Buf[FrameLen-2] != (BYTE)ChkSum((BYTE *)&pReceiveFrame->Frame69.Control,pReceiveFrame->Frame69.Length1))
                return FRAME_ERR|1;
            return FRAME_OK|FrameLen;
            
         case 0x67:
            if (pReceiveFrame->Frame67.Length1 != pReceiveFrame->Frame67.Length2)
                return FRAME_ERR|1;
            if (pReceiveFrame->Frame67.Start2 != 0x67)
                return FRAME_ERR|1;
            FrameLen=pReceiveFrame->Frame67.Length1+6;
            if (FrameLen > Len)
                return FRAME_ERR|1;
            if (Buf[FrameLen-1] != 0x16)
                return FRAME_ERR|1;
            if (Buf[FrameLen-2] != (BYTE)ChkSum((BYTE *)&pReceiveFrame->Frame67.Control,pReceiveFrame->Frame67.Length1))
                return FRAME_ERR|1;
            return FRAME_OK|FrameLen;
        default:
            return FRAME_ERR|1;
    }
}
/***************************************************************
    Function��RecODUFault
        ����ODU���ϱ���
    ������bOduAddr ֻ��Ϊ1��2(��pOdu���ڵõ���������Ϊ��ǰ6·��pOdu1���ڵõ�������Ϊ��6·)
    ���أ�δʹ��
***************************************************************/

BOOL CPrtcOdu::RecODUFault(BYTE bOduAddr)
{
  BYTE InfoNum;//���ϸ���
  BYTE bFltType;//��������
  ODUFTDA *pFaultCon;
  BYTE phase = 0;
  BYTE line = 0;
  int j = 0;
  int YcIndex = 0;
  int YcTIndex = 0;
  WORD wFltYxIndex = 0;//������� 
  WORD wComIndex = 0;//����ͨ������ң�����
  WORD wAllComIndex = 0;//��ͨ��״̬ң�����
  static WORD wComFltBk[2][2] = {0};//��������������24����ͨ������,��bit�洢
  BYTE bAllComFltBk[12] = {0};//����������ͨ������,����Ҫ���� ��6��Ϊ12
  BYTE *pCom;//ָ��ͨ�����ϱ�ʶλ
  BYTE ComFltFlag = 0;//ͨ�����ϱ�ʶ
  BYTE bComValTmp = 0;
  
  
  BYTE bAddrOffset = 0;//ODU��ַƫ����
  if(bOduAddr == 1 || bOduAddr == 2) 
    bAddrOffset = bOduAddr - 1;
  else 
   return 0;
      
  InfoNum = (pReceiveFrame->Frame67.VSQ_odu & 0x7f);
  bFltType = pReceiveFrame->Frame67.COT_odu;//����ԭ��������ʶ��������
  
  if(pReceiveFrame->Frame67.VSQ_odu > 0x80)//�ṹ�޶�����������ͨ�����Ϻ���������
  {//ͨ������
    if(InfoNum == 0x4c)
    {//a
        for(j=0;j<24;j++)//��ͨ�����ϵ�24��ң��������g_gRmtInfo�����У�ͬʱ��֮ǰ�Ĺ���״̬����
        {//b
          
              pCom = &(pReceiveFrame->Frame67.Data[3]);
              phase = j&3;
              line = (j>>2)+(bAddrOffset*6);//���ݵ�ŵõ���Ӧ����·�ţ���0��ʼ
              if(phase == 0) ComFltFlag = 0;
              if(pCom[j]) ComFltFlag = 1;
              
              //��Ӧ��ң�����ó���Ч
              if(pCom[j] && ( !(GETBIT(wComFltBk[bAddrOffset],j)) ) && ((g_gRunPara[RP_CFG_KEY] & BIT[RPCFG_COMMFLTVAL_MODE]) == 0))//ͨ������ʱң��Ϊ��Чֵ
              {
                   wComIndex = RI_1A_ZSQST + j + (bAddrOffset*PHASE_NUM*6);
                   //��Ӧ��ң�����ó���Ч
                    YcIndex = YxIdToYcId(wComIndex);
                    YcTIndex = YxIdToYcId(wComIndex,1);
                    if(YcIndex != 255) 
                    {
                      g_gRmtMeas[YcIndex + RM_1A_FAULT_I] = INVALIDYC;
                      g_gRmtMeas[YcIndex + RM_1A_LOAD_I] = INVALIDYC;
                      g_ucRefYxYcTs |= BIT1;
                    }
                    //�¶����ó���Ч
                    if(YcTIndex != 255)
                    {
                      //g_gRmtMeas[YcTIndex + RM_1A_CABLE_T] = INVALIDYC;
                      g_gRmtMeas[YcTIndex + RM_1A_CABLE_T_INT] = INVALIDYC;
                      g_ucRefYxYcTs |= BIT1;
                    }
              }
              
              //���ݷ���ͨ������
              BYTE fLag = 0;
              if(g_LineFlag[line] == 1)//�ϳ�ʱ�ĵ�һ·�������ڶ�·ʱһ��������
                fLag = 1;
              else if(g_LineFlag[line] == 2)
              {
                if( ((1&&pCom[j]) == GETBIT(wComFltBk[bAddrOffset],j)) && ((1&&pCom[j-4]) == GETBIT(wComFltBk[bAddrOffset],(j-4))) )
                {
                  fLag = 1;
                }
                
                SETBIT(wComFltBk[bAddrOffset],j-4,(pCom[j-4]&&1));
                SETBIT(wComFltBk[bAddrOffset],j,(pCom[j]&&1));
              }
              else if(g_LineFlag[line] == 0)
              {
                  if((1&&pCom[j]) == GETBIT(wComFltBk[bAddrOffset],j))
                     fLag = 1;
                  SETBIT(wComFltBk[bAddrOffset],j,(pCom[j]&&1));
              }
              
              //SETBIT(wComFltBk[bAddrOffset],j,(pCom[j]&&1));
              
              if(!fLag)
              {
                //����ͨ�����ϵĺϳ�
                if(g_LineFlag[line] == 2)//�ں�һ·���кϳɲ�����SOE���ϳɵķ��ڵ�һ·��λ��
                {
                    wComIndex = RI_1A_ZSQST + j - 4 + (bAddrOffset*PHASE_NUM*6);
                    bComValTmp = (pCom[j] || pCom[j-4])&&1;
                    CreatNewSoe(wComIndex,bComValTmp);
                }
                else if(g_LineFlag[line] == 0)
                {//���ϲ���ֱ������SOE
        
                    wComIndex = RI_1A_ZSQST + j + (bAddrOffset*PHASE_NUM*6);
                    CreatNewSoe(wComIndex,pCom[j]);
                } 
              }
               //������ͨ������===============
              if(phase == 3)
              {
                if(ComFltFlag) 
                  bAllComFltBk[line] = 1;
                else
                 bAllComFltBk[line] = 0;
                //��ͨ�����ϵĺϳ�
                if(g_LineFlag[line] == 2)//�ں�һ·���кϳɲ�����SOE
                {
                  wAllComIndex = line - 1 + RI_1ZSQST; //+ bAddrOffset*6;//�ܹ������
                  bComValTmp = (bAllComFltBk[line] || bAllComFltBk[line-1])&&1;
                  CreatNewSoe(wAllComIndex,bComValTmp);
                }
                else if(g_LineFlag[line] == 0)
                {//���ϲ���ֱ������SOE
                  wAllComIndex = line+ RI_1ZSQST; //+ bAddrOffset*6;//�ܹ������
                  CreatNewSoe(wAllComIndex,bAllComFltBk[line]);
                }
              }
              //==================================
        }//b
     }//a
  }
  else//��·�ӵ��¶ȹ���
  {
    BYTE iFtType;
    if(bFltType == 0x05) iFtType = 0;//��·��ӵع���
    else if(bFltType == 0x06) iFtType = 3;//�¶ȹ���
    else return 0;//���ʹ����򲻽���
    
    pFaultCon = (ODUFTDA *)&(pReceiveFrame->Frame67.Data[1]);
    for(int i = 0; i < InfoNum;i++)
    {
        if((pFaultCon->byFaultVal && 1) != 1)  //��λΪ1ʱ���������Ч
         continue;
 
        line = pFaultCon->byLinePhase;
        line = (line >> 2) & 0x3F;//ȡ����·ƫ�� ��6λ  ��0��ʼ
        phase = pFaultCon->byLinePhase & 0x03;//ȡ����λƫ��
        if(phase == 0) 
        {
          phase = 4;//���⴦�� O��
          line -= 1;//�ӱ����п���0����������·�ű����������·�Ŵ�1
          if(bFltType == 0x05) iFtType = 1;//�����жϽӵع��ϣ���Ϊ��·,�ӵص����ͱ�ʶ������0x05��
        }
        else
        {
            if(bFltType == 0x05) iFtType = 0;
            else if(bFltType == 0x06)iFtType = 3;//�¶ȹ���
        }
        line += bAddrOffset*6;
        
        //ȡ������ң��
        if(g_LineFlag[line] == 2) //��ǰ·Ϊ�ϳ�ʱ�ĺ�һ·����ȡǰһ·��Ӧ�Ĺ��ϵ��
          line = line - 1;
        wFltYxIndex = LPFToYxId(line +1,phase,iFtType);
        
        if(RI_IN_DAERR == wFltYxIndex)  
        {
          SetRIStatus(RI_IN_DAERR, 1);
          continue;
        }
        if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
        {
          g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
          CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);              
        }  
       pFaultCon++;     
    }
  }
  return TRUE; 
}
//����ODU����������
BOOL CPrtcOdu::RecODUPara()
  {
     BYTE *pData,*pData1;
     WORD wFileNa = 0;//�ļ���
     BYTE bSecNa = 0;//����
     if(m_OduAddr == 1)
        wFileNa = 9;
     else if(m_OduAddr == 2)
        wFileNa = 10;
    
      pData = &pReceiveFrame->Frame67.Data[3];
      pData1= &pReceiveFrame->Frame67.Data[5];
      switch((*pData))
      {
            case 0x04://ң��
              bSecNa = 0;
              break;
            case 0x05://�ٻ�ĳһ������
              bSecNa = 1;
              break;
            case 0x09:
              if((*pData1)==1)      bSecNa = 2;
              else if((*pData1)==2) bSecNa = 3;
              break;
            case 0x01:
               bSecNa = 4;
               break;
            default:
               break;
       }
      if(m_RWPaSrcObj !=null)
        m_RWPaSrcObj->SendReadPa(wFileNa,bSecNa);
    return TRUE;    
  }
BOOL CPrtcOdu::RecODUPower_Supply()
{
    BYTE *pData;
    pData = &pReceiveFrame->Frame67.COT_odu;
   
    if((*pData == 4) && (pGprs != null))
    {
        m_RcvCloseZdFlag = ON;
        if((!g_batPowerSt)&&(CheckHaveDataToSd() == FALSE || !g_GprsPowerSt))
        {
          
          if(g_GprsPowerSt)
          {
            BYTE bBuf[2] = {0xD2,0XD2};
            MyPrintfD(bBuf,2);
            SendWCmdToOdu(0x02,3);
            CloseGPRS(0);
            OpenTimer(TM_Volt);
            g_SunHighNum = 0;
            g_gRunPara[RP_POWER_MODE]=QUASI_REALTIME;
           }
          else
          {
            BYTE bBuf[2] = {0xDB,0XDB};
            MyPrintfD(bBuf,2);
            SendWCmdToOdu(0x55,3);
          }
          m_RcvCloseZdFlag = OFF;//�ظ�ODU���Թ������˱�ʶ��λ
          
        }
        else
        {
          SendWCmdToOdu(0x0A,3);//��ODU��ʱ10����
          
        }
//          if(pGprs->m_reSendDelay > 0 || pGprs->m_resendflag)
//          {//GPRSͨ�����ڹ����ط���ʱ�л���Ҫ�ط����ϣ�����ODU�ӳ�10���ӹص�Դ
//            SendWCmdToOdu(0x10,3);
//          }
//          else if((g_ucRefYxYcTs & BIT1) == BIT1)
//          {//��ң��Ҫ���ͣ�����ODU�ӳ�3���ӹص�Դ
//            SendWCmdToOdu(0,3);
//          }
//          else
//          {
//            SendWCmdToOdu(0x55,3);
//            m_RcvCloseZdFlag = OFF;//�ظ�ODU���Թ������˱�ʶ��λ
//          } 
    }
    return TRUE; 
}
BOOL CPrtcOdu::RecODUFault_yaoce(BYTE bOduAddr)
{
    BYTE InfoNum = 0;
    ODUYCDA *pOduYcCon;
    BYTE line,phase;//����ң�ⱨ�ĵĳ��ȣ��ߺ�
    BYTE line_NUM;//ȡֵ1-6�������¶�ң��
    WORD wComIndex = 0; //��ǰͨ����� 
    WORD wYcIndex = 0;
    WORD wTmpIndex = 0;
    WORD wCurInfoOfset = 0;//��ǰ��Ϣƫ����
    WORD wCurTmInfoOfset = 0;//��ǰ�¶���Ϣƫ����
    int iTmp1,iTmp2;
    BYTE bYcFlag[24]={0};//0=δ�յ���Ч���� 1=���յ���Ч����
    BYTE bAddrOffset = 0;
     if(bOduAddr == 1 || bOduAddr == 2) //bOduAddrֻ��Ϊ1��2(��pOdu���ڵõ���������Ϊ��ǰ6·��pOdu1���ڵõ�������Ϊ��6·)
       bAddrOffset = bOduAddr - 1;
     else 
      return 0;
     InfoNum = pReceiveFrame->Frame67.VSQ_odu & 0x7f;
     pOduYcCon = (ODUYCDA *)&(pReceiveFrame->Frame67.Data[3]);

     
     for(int i = 0; i < InfoNum;i++)
     {
        if((pOduYcCon->byZsqType == 0x66) || (pOduYcCon->byZsqType == 0x55))//0x55=������3C1Oָʾ����ֵ,0x66=������3C1O-Tָʾ����ֵ
        {//a
          if(MAKEWORD(pOduYcCon->byIL,pOduYcCon->byIH) == ODUINITYC)//�������ĵ���ֵΪODU��ʼ����ֵ�򲻽��н���
            continue; 
            
          //line = i>>2;//���ݵ�ŵõ���Ӧ����·�ţ���0��ʼ
          line = (i>>2)+(bAddrOffset*6);
          line_NUM=(i>>2);
          phase = i&3;
          wCurInfoOfset = i + bAddrOffset*PHASE_NUM*6;

          if(g_LineFlag[line] == 2)
          {
            wComIndex = RI_1A_ZSQST + wCurInfoOfset - 4;//��ǰΪ�ϳɵĵ�2·����ͨ��״̬Ӧ�ж�ǰһ·
          }
          else
            wComIndex = RI_1A_ZSQST + wCurInfoOfset;
          if(GetRIStatus(wComIndex))//�жϺϳɺ����·�Ƿ���ͨ�����ϣ����ϳɵ���·�κ�һ����ͨ�����ϣ����Ӧ��һ·�ĸ���ң��Ҳ������
            continue;
          
            //������������
            wYcIndex = RM_1A_LOAD_I + wCurInfoOfset;//���ɵ����ĵ��
            g_gRmtMeas[wYcIndex] = MAKEWORD(pOduYcCon->byIL,pOduYcCon->byIH);

           
            g_gRmtMeas[wYcIndex] &= 0x7fff;
            if(pOduYcCon->byIH >= 0x80)//����ָʾ�����ĵĽ�����>0x80ʱ��ң��Ŵ���10��
              g_gRmtMeas[wYcIndex] = (g_gRmtMeas[wYcIndex]/10);
            bYcFlag[i] = 1;
          

             if(g_LineFlag[line] == 2)//�ڵڶ�·���кϳ�
             {
                if(bYcFlag[i-4] == 1)//��һ·������Ч���ϴ���·�Ӻ�ֵ
                {
                  g_gRmtMeas[wYcIndex-4] += g_gRmtMeas[wYcIndex];  
                }
                else//��һ·Ϊ��ʼ��ֵ(δ�յ���Ч����)���ϴ��ڶ�·��ң��
                {
                  g_gRmtMeas[wYcIndex-4] = g_gRmtMeas[wYcIndex];
                }
             }

          if((pOduYcCon->byZsqType == 0x66) && (phase != 3))//���¶� 0��û���¶�
          {
              wCurTmInfoOfset = line_NUM*(PHASE_NUM-1) + phase+ bAddrOffset*TEMP_PHASE_NUM*6;
              //�������½�������===========
              for(int k = 0;k < 2;k++)
              {
                  if(k == 0)
                  {
                   // wTmpIndex = RM_1A_CABLE_T + wCurTmInfoOfset;
                    //g_gRmtMeas[wTmpIndex] = pOduYcCon->byExtTmpt;
                  }
                  else
                  {
                    wTmpIndex = RM_1A_CABLE_T_INT + wCurTmInfoOfset;
                    g_gRmtMeas[wTmpIndex] = pOduYcCon->byIntTmpt;
                  }
                  if((g_gRmtMeas[wTmpIndex])>0x80)
                  {
                     g_gRmtMeas[wTmpIndex] |= 0xFF00;
                  }
                  
                  if(g_LineFlag[line] == 2)//�ڵڶ�·���кϳ�
                  {
                    if(bYcFlag[i - 4] == 1)//��һ·������Ч
                    {
                      if((g_gRmtMeas[wTmpIndex - 3] != 0x80)&&(g_gRmtMeas[wTmpIndex] != 0x80))
                      {//��·���ݶ�������0x80���ϴ���·�нϴ���¶�ֵ��0x80��ʾ����̽ͷû���ϡ�
                        iTmp1 = g_gRmtMeas[wTmpIndex - 3];
                        iTmp2 = g_gRmtMeas[wTmpIndex];
                        if(iTmp1 < iTmp2)
                          g_gRmtMeas[wTmpIndex - 3] = g_gRmtMeas[wTmpIndex];
                      }
                      else if(g_gRmtMeas[wTmpIndex - 3] == 0x80)
                      {
                        g_gRmtMeas[wTmpIndex - 3] = g_gRmtMeas[wTmpIndex];
                      }
                      
                    }
                    else//��һ·Ϊ��ʼ��ֵ(δ�յ���Ч����)���ϴ��ڶ�·��ң��
                    {
                     g_gRmtMeas[wTmpIndex - 3] = g_gRmtMeas[wTmpIndex];
                    }
                  }
              }
             //======================
          }
          g_ucRefYxYcTs |= BIT1;
        }//a
        pOduYcCon++;
     }
   return TRUE; 
}


//����дָ�ODU
void CPrtcOdu::SendWCmdToOdu(WORD OduPara,BYTE ParaName, BYTE *pDa,CProtocol *pDaSrc)
{//pDa[0]��ʾ��������
    BYTE Style = 125;
    BYTE Reason = 0X0D;
  BYTE ODU_para1 = 0Xea;
  BYTE ODU_para2 = 0X0c;
    SendFrameHead_ODU(Style, Reason,ODU_para1,ODU_para2,OduPara);
    
    if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
    switch(ParaName)
    {
      case 0://д�������        
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 4;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 24;
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],pDa+1,pDa[0]);
      m_SendBuf.wWritePtr +=24;
        break;
     case 1://д��ʱ����
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 5;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 4;
        memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],pDa+1,pDa[0]);//�޸�
        m_SendBuf.wWritePtr +=13;
        break;
   case 4://д��ʱ����
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
    memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],pDa+1,1);
      m_SendBuf.wWritePtr +=1;
        break;
   case 3://
    if(OduPara==0x55)
    {
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x4;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x55;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      //memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame68.Data[m_dwasdu.Infooff+6],4);
        //m_SendBuf.wWritePtr +=4;
    }
    else if(OduPara==0x10)
    {
    
    //m_reSendodu = 5;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x4;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x0A;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      //memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame68.Data[m_dwasdu.Infooff+6],4);
        //m_SendBuf.wWritePtr +=4;
    }
    else if(OduPara==0)
    {
    
    //m_reSendodu = 5;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x4;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 3;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      //memcpy(&m_SendBuf.pBuf[m_SendBuf.wWritePtr],&pReceiveFrame->Frame68.Data[m_dwasdu.Infooff+6],4);
        //m_SendBuf.wWritePtr +=4;
    }
     break;
  case 6://��GPRS��Դ
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 7;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0x55;
     break;
  case 7://�ضϪGPRS��Դ
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 7;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;
    m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
      break;  
     
     }
    SendFrameTail_ODU(1,OduPara);
   if((ParaName==3)||(ParaName==6)||(ParaName==7))
   {
    memcpy(&m_oduSendBuf[0],&m_SendBuf.pBuf[0],m_SendBuf.wWritePtr);
    m_oduSendLen = m_SendBuf.wWritePtr;
    m_WrPaWaitflag = ON;
    m_WrPaWaitCt = 10;
    m_ReTxNum = 0;
   }
   m_ReadPaTp = ParaName;
     return ;
}
//��λODU
void CPrtcOdu::RstOdu(void)
{
        m_SendBuf.wReadPtr = 0;
        #define RSTODU_LEN  15
        BYTE bySdFrame[RSTODU_LEN]={0x67,0x09,0x09,0x67,0x53,0x01,0x2E,0x06,0x01,0x01,0x00,0x24,0x00,0xAE,0x16};
        memcpy(m_SendBuf.pBuf,bySdFrame,RSTODU_LEN);
        m_SendBuf.wWritePtr += RSTODU_LEN;
        CommWrite((char *)(m_SendBuf.pBuf+m_SendBuf.wReadPtr),m_SendBuf.wWritePtr-m_SendBuf.wReadPtr,1);    
        return;
}

void CPrtcOdu::SendRCmdToOdu(WORD OduPara,BYTE ParaName,CProtocol *pDaSrc)
{
      BYTE Style = 122;
      BYTE Reason = 0X0D;
      BYTE ODU_para1 = 0Xea;
      BYTE ODU_para2 = 0X0c;
      if(pDaSrc != null ) m_RWPaSrcObj = pDaSrc;
          
       SendFrameHead_ODU(Style, Reason,ODU_para1,ODU_para2,OduPara);
       switch(ParaName)
        {
          case 0://���������
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 4;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;
            break;
         case 1://����ʱ����
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 5;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
       case 2://������149�汾��
          m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 9;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
         case 3://���Ӱ�2247�汾��
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 9;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 2;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
      case 4://��ODU��ַ
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 1;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
            m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 0;
        m_SendBuf.pBuf[m_SendBuf.wWritePtr++] = 6;  
            break;
  
         }
      SendFrameTail_ODU(0,OduPara);
      return;
}

#endif



