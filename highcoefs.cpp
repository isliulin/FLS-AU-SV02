
#ifndef CHEBYSHEV_FILTER_GLOBALS

#define CHEBYSHEV_FILTER_GLOBALS
  #include "DataStruct.h"
  #include <math.h> 
  #include "Main.h"
  //#include "highcoefs.h"
#endif

/*#ifndef _CHEBYSHEV_FILTER

#define  _CHEBYSHEV_FILTER
  //#include "highcoefs.h"
 // #include "datatype.h"   
#endif*/


/*const int NL_New = 5;
const float NUM_New[5] = {   //real32_T
     0.5014006562223,   -2.005602624889,    3.008403937334,   -2.005602624889,//�˲���ʵ�ֵ�ϵ����  0.5014   -2.0056    3.0084   -2.0056    0.5014
     0.5014006562223
};//��Ƭ����������*/
const int NUM_New[5] = {5014,-20056,30084,-20056,5014};//real32_T
/*const int DL_New = 5;
const float DEN_New[5] = {
                   1,   -2.809934650895,    3.159824193546,   -1.665149397058,//�˲���ʵ�ֵ�ϵ�� 1.0000   -2.8099    3.1598   -1.6651    0.3664
     0.3663843869895
};*/
//�˲���ʵ�ֵ�ϵ����  0.5014   -2.0056    3.0084   -2.0056    0.5014
const int DEN_New[5] = {10000,-28099,31598,-16651,3664};
//�˲���ʵ�ֵ�ϵ�� 1.0000   -2.8099    3.1598   -1.6651    0.3664
//extern float filterBuf[10];//SAMPLE_MAX_NUM�˲��������
//int  originaBuf[SAMPLE_MAX_NUM];//ԭʼ��������
//float differBuf_V[10];//SAMPLE_MAX_NUM��ѹ�˲�������ݾ����󵼵�����
//float differBuf_I[10];//SAMPLE_MAX_NUM�����˲�������ݾ����󵼵�����

/*typedef struct  {
    long  Real;
    long  Imag;
    long  LowReal;
    long  LowImag;
    unsigned char   Cnt;
}DFT_DATA_EXT;*/


/***************************************************************
    Function��chebyshev_filter
       ��ͨ�˲�������Ҫ�˳�50HZ���µ�Ƶ�ʣ������б�ѩ��I���˲������˲����Ľ�����4�ף�����Ƶ��4000HZ��ͨ����ֹƵ��Ϊ250HZ��ͨ�����˥��Ϊ1db,�����ʼƵ��Ϊ50HZ��������˥��Ϊ60db��
    ������originaBuf, SAMPLE_MAX_NUM, filterBuf
        originaBuf �˲�֮ǰ��ԭʼ����
        SAMPLE_MAX_NUM ԭʼ���ݳ��ȣ�ͬʱҲ���˲���ĳ���
      
    ���أ�  filterBuf���˲�������ݣ�SAMPLE_MAX_NUM���˲���ĳ���
***************************************************************/
void chebyshev_filter(WORD sample_num,unsigned char VI_flag,unsigned char line_num,INT8U phase_fault)//int *originaBuf,�Ľ��б�ѩܽ�˲�����ʵ��
{
  
   
   INT16S filterBuf_BK[5];
   unsigned long FWfAddr = 0;
   //FP32 filter_mid =0;
   //FP32 filterBuf[5];//sample_numSAMPLE_MAX_NUM960
   INT16U phasefault_len =0;
   INT16U EEPADD_FaultLEN =0;
  // unsigned long FLbAddrNum = 0;
   //unsigned char ReadBack[256] = {0};
    //unsigned int num=0;
    //unsigned int filter_num=0;
    /* DFT_DATA_EXT DftData;
   INT32S TempRe,data_value;
   INT32S TempIm;
   INT32S Data[2];
   INT8U  cal_num =0;
   INT8U  earth_fault = OFF;*/
   unsigned long FLbAddr = 0;
   int  originaBuf[5] = {0};
   unsigned long FLbAddrA = 0;
   //unsigned int FLbAddrA_Num = 0;
   unsigned long FWfAddrA =0;
   unsigned int FWfAddrA_Num =0;
   unsigned int PageSize;//��ǰҳ��ʣ��ռ�
   
   EEPADD_FaultLEN = EEPADD_ALEN + ((INT16U)phase_fault-1)*2 + line_num*EELEN_LENG;
   CAT_SpiReadWords(EEPADD_FaultLEN, 1, &phasefault_len);
   #ifdef PHASELUBO
   
   if(VI_flag == 0x30)
    {
      //FLbAddr =FADDR_RECORDER_START;
      //FLbAddr =FADDR_RECORDER_START+(long)LINE*(long)FLINEADDR;
      FWfAddr =  FADDR_WAVE_FILTERI+(long)line_num*(long)FPHASEADDR; //�����˲���ʼλ��
      FLbAddr =  FADDR_RECORDER_START+(long)line_num*(long)FLINEADDR+(long)phasefault_len*2+(long)FPHASEADDR*(long)(phase_fault-1);
    }
    else if(VI_flag == 0x31)
    {
      //FLbAddr =FADDR_RECORDER_STARTV;
      FWfAddr =  FADDR_WAVE_FILTERV+(long)line_num*(long)FPHASEADDR; //��ѹ�˲���ʼλ��
      FLbAddr = FADDR_RECORDER_STARTV+(long)line_num*(long)FLINEADDR+(long)phasefault_len*2+(long)FPHASEADDR*(long)(phase_fault-1);
    }
    //phasefault_len = CAT_SpiReadWords(EEPADD_ALEN, 3, offset);
    #else
	
	if(VI_flag == 0x30)
    {
      //FLbAddr =FADDR_RECORDER_START;
      //FLbAddr =FADDR_RECORDER_START+(long)LINE*(long)FLINEADDR;
      FWfAddr =  FADDR_WAVE_FILTERI+(long)line_num*(long)FPHASEADDR; //�����˲���ʼλ��
      FLbAddr =  FADDR_RECORDER_START+(long)line_num*(long)FLINEADDR+(long)FPHASEADDR*3;
    }
    else if(VI_flag == 0x31)
    {
      //FLbAddr =FADDR_RECORDER_STARTV;
      FWfAddr =  FADDR_WAVE_FILTERV+(long)line_num*(long)FPHASEADDR; //��ѹ�˲���ʼλ��
      FLbAddr = FADDR_RECORDER_STARTV+(long)line_num*(long)FLINEADDR+(long)FPHASEADDR*3;
    }
		
    #endif
    //phasefault_len = CAT_SpiReadWords(EEPADD_ALEN, 3, offset);
    
    PageSize = 0x100 -(FLbAddr %0x100);
    if(PageSize < 10)
    {
      Sst26vf064b_Read(FLbAddr,(unsigned char *)originaBuf,PageSize);
      Sst26vf064b_Read((FLbAddr &0xffffff00)+0x100,(unsigned char *)&originaBuf[PageSize>>1],(10-PageSize));
    }
    else
      Sst26vf064b_Read(FLbAddr,(unsigned char *)originaBuf,10);
    
     //FLbAddr+=10;
    //Sst26vf064b_Read(FLbAddr+FPHASEADDR*(phase_fault-1),(unsigned char *)originaBuf,10);//3+i*FLASH_PAGE_SIZE
    Sector_Erase(FWfAddr);//1200*4=4800,��Ҫ�߳������
   //Sector_Erase(FWfAddr);//1200*4=4800,��Ҫ�߳������
   //Sector_Erase(FWfAddr+4096);
   for(unsigned int n=0;n< sample_num;n++) //m=0;�˲���ʵ�֣��˲����������µݹ���ʽ������������̣������˲���ϵ���ó��˲��Ĺ�ʽ
   {
   /*
      FP32 filter_mid =0;
     FP32 filterBuf[5];//sample_numSAMPLE_MAX_NUM960
   		//�Ľ��˲�
        if (n==0)
        {
          filter_mid = (FP32)NUM_New[0]*originaBuf[0];
          filterBuf[0]=  filter_mid/10000;
          //filterBuf[0]= ((long)(NUM_New[0]*originaBuf[0]))/10000;
          //filterBuf[m]= (NUM_New[0]*originaBuf[0])/10000;
        }
        else if(n==1)
        {
            filter_mid = (FP32)NUM_New[0]*originaBuf[1];
            filter_mid += (FP32)NUM_New[1]*originaBuf[0];
            filter_mid -= (FP32)DEN_New[1]*filterBuf[0];
            filterBuf[1]=  filter_mid/10000;
            //filterBuf[1]= (NUM_New[0]*originaBuf[1]+NUM_New[1]*originaBuf[0]-DEN_New[1]*filterBuf[0])/10000;
            //filterBuf[m+1]= (NUM_New[0]*originaBuf[1]+NUM_New[1]*originaBuf[0]-DEN_New[1]*filterBuf[0])/10000;
        }
        else if(n==2)
        {
          
            filter_mid = (FP32)NUM_New[0]*originaBuf[2];
            filter_mid += (FP32)NUM_New[1]*originaBuf[1];
            filter_mid += (FP32)NUM_New[2]*originaBuf[0];
            filter_mid -= (FP32)DEN_New[1]*filterBuf[1];
            filter_mid -= (FP32)DEN_New[2]*filterBuf[0];
            filterBuf[2]=  filter_mid/10000;
            //filterBuf[2]= (NUM_New[0]*originaBuf[2]+NUM_New[1]*originaBuf[1]+NUM_New[2]*originaBuf[0]-DEN_New[1]*filterBuf[1]-DEN_New[2]*filterBuf[0])/10000; 
             //filterBuf[m+2]= (NUM_New[0]*originaBuf[1]+NUM_New[1]*originaBuf[0]-DEN_New[1]*filterBuf[0])/10000;
        }
        else if(n==3)
        {
          
            filter_mid = (FP32)NUM_New[0]*originaBuf[3];
            filter_mid +=(FP32)NUM_New[1]*originaBuf[2];
            filter_mid +=(FP32)NUM_New[2]*originaBuf[1];
            filter_mid +=(FP32)NUM_New[3]*originaBuf[0];
            filter_mid -=(FP32)DEN_New[1]*filterBuf[2];
            filter_mid -=(FP32)DEN_New[2]*filterBuf[1];
            filter_mid -=(FP32)DEN_New[3]*filterBuf[0];
            filterBuf[3]=  filter_mid/10000;
            //filterBuf[3]= (NUM_New[0]*originaBuf[3]+NUM_New[1]*originaBuf[2]+NUM_New[2]*originaBuf[1]+NUM_New[3]*originaBuf[0]-DEN_New[1]*filterBuf[2]-DEN_New[2]*filterBuf[1]-DEN_New[3]*filterBuf[0])/10000;
            //filterBuf[m+3]= (NUM_New[0]*originaBuf[3]+NUM_New[1]*originaBuf[2]+NUM_New[2]*originaBuf[1]+NUM_New[3]*originaBuf[0]-DEN_New[1]*filterBuf[2]-DEN_New[2]*filterBuf[1]-DEN_New[3]*filterBuf[0])/10000;
        }
        else
        {

             filter_mid = (FP32)NUM_New[0]*originaBuf[4];
             filter_mid += (FP32)NUM_New[1]*originaBuf[3];
             filter_mid += (FP32)NUM_New[2]*originaBuf[2];
             filter_mid += (FP32)NUM_New[3]*originaBuf[1];
             filter_mid += (FP32)NUM_New[4]*originaBuf[0];
             filter_mid -= (FP32)DEN_New[1]*filterBuf[3];
             filter_mid -= (FP32)DEN_New[2]*filterBuf[2];
             filter_mid -= (FP32)DEN_New[3]*filterBuf[1];
             filter_mid -= (FP32)DEN_New[4]*filterBuf[0];
             filterBuf[4]=  filter_mid/10000;///10000;
             //filterBuf[4]=(NUM_New[0]*originaBuf[4]+NUM_New[1]*originaBuf[3]+NUM_New[2]*originaBuf[2]+NUM_New[3]*originaBuf[1]+NUM_New[4]*originaBuf[0]-DEN_New[1]*filterBuf[3]-DEN_New[2]*filterBuf[2]-DEN_New[3]*filterBuf[1]-DEN_New[4]*filterBuf[0])/10000;
             //filterBuf[m+4]=(NUM_New[0]*originaBuf[m+4]+NUM_New[1]*originaBuf[m+3]+NUM_New[2]*originaBuf[m+2]+NUM_New[3]*originaBuf[m+1]+NUM_New[4]*originaBuf[m]-DEN_New[1]*filterBuf[m+3]-DEN_New[2]*filterBuf[m+2]-DEN_New[3]*filterBuf[m+1]-DEN_New[4]*filterBuf[m])/10000;
*/
		//�����˲� 200Hz��ͨ
		#define BF1_B0 (27226)
		#define BF1_B1 (-54452)
		#define BF1_B2 (27226)
		#define BF1_A1 (-53508)
		#define BF1_A2 (22629)
		INT32S filter_mid =0;
                INT32S filterBuf[5];//sample_numSAMPLE_MAX_NUM960
		if (n==0)
        {
              filter_mid = (INT32S)BF1_B0*originaBuf[0];//fbuf[0] = sbuf[0];
				filterBuf[0]=(filter_mid >> 15);
			    
				
          //filter_mid = (FP32)NUM_New[0]*originaBuf[0];
          //filterBuf[0]=  filter_mid/10000;
          //filterBuf[0]= ((long)(NUM_New[0]*originaBuf[0]))/10000;
          //filterBuf[m]= (NUM_New[0]*originaBuf[0])/10000;
        }
        else if(n==1)
        {
        	filter_mid = (INT32S)BF1_B0*originaBuf[1]+(INT32S)BF1_B1*originaBuf[0]-(INT32S)BF1_A1*filterBuf[0];//fbuf[1] = sbuf[1];
			filterBuf[1]=(filter_mid >> 15);
			
            //filter_mid = (FP32)NUM_New[0]*originaBuf[1];
            //filter_mid += (FP32)NUM_New[1]*originaBuf[0];
            //filter_mid -= (FP32)DEN_New[1]*filterBuf[0];
            //filterBuf[1]=  filter_mid/10000;
            //filterBuf[1]= (NUM_New[0]*originaBuf[1]+NUM_New[1]*originaBuf[0]-DEN_New[1]*filterBuf[0])/10000;
            //filterBuf[m+1]= (NUM_New[0]*originaBuf[1]+NUM_New[1]*originaBuf[0]-DEN_New[1]*filterBuf[0])/10000;
        }
        else if(n==2)
        {
        	filter_mid = (INT32S)BF1_B0*(originaBuf[2]+originaBuf[0])+(INT32S)BF1_B1*originaBuf[1]-
            		(INT32S)BF1_A1*filterBuf[1]-(INT32S)BF1_A2*filterBuf[0];
        	filterBuf[2] =(filter_mid >> 15);
          
            //filter_mid = (FP32)NUM_New[0]*originaBuf[2];
            //filter_mid += (FP32)NUM_New[1]*originaBuf[1];
            //filter_mid += (FP32)NUM_New[2]*originaBuf[0];
            //filter_mid -= (FP32)DEN_New[1]*filterBuf[1];
            //filter_mid -= (FP32)DEN_New[2]*filterBuf[0];
            //filterBuf[2]=  filter_mid/10000;
            //filterBuf[2]= (NUM_New[0]*originaBuf[2]+NUM_New[1]*originaBuf[1]+NUM_New[2]*originaBuf[0]-DEN_New[1]*filterBuf[1]-DEN_New[2]*filterBuf[0])/10000; 
             //filterBuf[m+2]= (NUM_New[0]*originaBuf[1]+NUM_New[1]*originaBuf[0]-DEN_New[1]*filterBuf[0])/10000;
        }
        else if(n==3)
        {

		  filter_mid = (INT32S)BF1_B0*(originaBuf[3]+originaBuf[1])+(INT32S)BF1_B1*originaBuf[2]-
            		(INT32S)BF1_A1*filterBuf[2]-(INT32S)BF1_A2*filterBuf[1];
		  filterBuf[3] =(filter_mid >> 15);
                  
        	//filterBuf[j] =(filter_mid >> 15);
            //filter_mid = (FP32)NUM_New[0]*originaBuf[3];
            //filter_mid +=(FP32)NUM_New[1]*originaBuf[2];
           // filter_mid +=(FP32)NUM_New[2]*originaBuf[1];
           // filter_mid +=(FP32)NUM_New[3]*originaBuf[0];
           // filter_mid -=(FP32)DEN_New[1]*filterBuf[2];
           // filter_mid -=(FP32)DEN_New[2]*filterBuf[1];
           // filter_mid -=(FP32)DEN_New[3]*filterBuf[0];
           // filterBuf[3]=  filter_mid/10000;
            //filterBuf[3]= (NUM_New[0]*originaBuf[3]+NUM_New[1]*originaBuf[2]+NUM_New[2]*originaBuf[1]+NUM_New[3]*originaBuf[0]-DEN_New[1]*filterBuf[2]-DEN_New[2]*filterBuf[1]-DEN_New[3]*filterBuf[0])/10000;
            //filterBuf[m+3]= (NUM_New[0]*originaBuf[3]+NUM_New[1]*originaBuf[2]+NUM_New[2]*originaBuf[1]+NUM_New[3]*originaBuf[0]-DEN_New[1]*filterBuf[2]-DEN_New[2]*filterBuf[1]-DEN_New[3]*filterBuf[0])/10000;
        }
        else
        {
			filter_mid = (INT32S)BF1_B0*(originaBuf[4]+originaBuf[2])+(INT32S)BF1_B1*originaBuf[3]-
            		(INT32S)BF1_A1*filterBuf[3]-(INT32S)BF1_A2*filterBuf[2];
			filterBuf[4] =(filter_mid >> 15);
                        
             //filter_mid = (FP32)NUM_New[0]*originaBuf[4];
            // filter_mid += (FP32)NUM_New[1]*originaBuf[3];
            // filter_mid += (FP32)NUM_New[2]*originaBuf[2];
            // filter_mid += (FP32)NUM_New[3]*originaBuf[1];
            // filter_mid += (FP32)NUM_New[4]*originaBuf[0];
            // filter_mid -= (FP32)DEN_New[1]*filterBuf[3];
            // filter_mid -= (FP32)DEN_New[2]*filterBuf[2];
            // filter_mid -= (FP32)DEN_New[3]*filterBuf[1];
            // filter_mid -= (FP32)DEN_New[4]*filterBuf[0];
            // filterBuf[4]=  filter_mid/10000;///10000;
             //filterBuf[4]=(NUM_New[0]*originaBuf[4]+NUM_New[1]*originaBuf[3]+NUM_New[2]*originaBuf[2]+NUM_New[3]*originaBuf[1]+NUM_New[4]*originaBuf[0]-DEN_New[1]*filterBuf[3]-DEN_New[2]*filterBuf[2]-DEN_New[3]*filterBuf[1]-DEN_New[4]*filterBuf[0])/10000;
             //filterBuf[m+4]=(NUM_New[0]*originaBuf[m+4]+NUM_New[1]*originaBuf[m+3]+NUM_New[2]*originaBuf[m+2]+NUM_New[3]*originaBuf[m+1]+NUM_New[4]*originaBuf[m]-DEN_New[1]*filterBuf[m+3]-DEN_New[2]*filterBuf[m+2]-DEN_New[3]*filterBuf[m+1]-DEN_New[4]*filterBuf[m])/10000;
             if(n == 4)
             {
               
               for(int i =0;i<5;i++)
                filterBuf_BK[i] = (INT16S)filterBuf[i];
               Sst26vf064b_Page_WriteBytes(FWfAddr, (unsigned char *)filterBuf_BK,10);
             }
             else
             {
                FWfAddrA =n*2-FWfAddrA_Num*FLASH_PAGE_SIZE;//WRITEPECORDER;
        
                if(FWfAddrA >= FLASH_PAGE_SIZE)//WRITEPECORDER
                {
                  FWfAddrA_Num++;
                  filterBuf_BK[4] = (INT16S)filterBuf[4];
                  Sst26vf064b_Page_WriteBytes(FWfAddr+FWfAddrA_Num*FLASH_PAGE_SIZE,(unsigned char *)&filterBuf_BK[4],2);//+FLbAddrA
                }
                else
                {
                  filterBuf_BK[4] = (INT16S)filterBuf[4];
                  Sst26vf064b_Page_WriteBytes(FWfAddr+FWfAddrA+FWfAddrA_Num*FLASH_PAGE_SIZE,(unsigned char *)&filterBuf_BK[4],2);
                }
             }
             
             
             
             filterBuf[0] = filterBuf[1];
             filterBuf[1] = filterBuf[2];
             filterBuf[2] = filterBuf[3];
             filterBuf[3] = filterBuf[4];
             
             originaBuf[0] = originaBuf[1];
             originaBuf[1] = originaBuf[2];
             originaBuf[2] = originaBuf[3];
             originaBuf[3] = originaBuf[4];
             
             
             
             
            // m =0;
             
             //FLbAddrA =(n+1)*2+(FLbAddr % 100)-FLbAddrA_Num*FLASH_PAGE_SIZE;//WRITEPECORDER;
             //FLbAddr +=  (n+1)*2;
             FLbAddrA = FLbAddr+(unsigned long)(n+1)*(unsigned long)2;
             /*PageSize = 0x100 -(FLbAddrA %0x100);
             if(PageSize < 2)
             {
                //Sst26vf064b_Read(FLbAddr,(unsigned char *)&originaBuf[4],PageSize);
                Sst26vf064b_Read((FLbAddrA &0xffffff00)+0x100,(unsigned char *)&originaBuf[4],2);
             }
              else*/
                Sst26vf064b_Read(FLbAddrA,(unsigned char *)&originaBuf[4],2);
            /*if(FLbAddrA >= FLASH_PAGE_SIZE)//WRITEPECORDER
            {
              FLbAddrA_Num++;
              Sst26vf064b_Read(FLbAddr+FPHASEADDR*(phase_fault-1)+FLbAddrA_Num*FLASH_PAGE_SIZE,(unsigned char *)&originaBuf[4],2);//+FLbAddrA
            }
            else
              Sst26vf064b_Read(FLbAddr+FPHASEADDR*(phase_fault-1)+FLbAddrA+FLbAddrA_Num*FLASH_PAGE_SIZE,(unsigned char *)&originaBuf[4],2);*/
           
              
        }
        
        
   }
   
  
     if(VI_flag == 0x31) //&&(earth_fault  == ON))
     {
          //differential_filter(sample_num,phase_fault,line_num);
          g_step_earth = 4;//float *filterBuf_V,float *filterBuf_Ig_fault_flag,
     }
   
}

/*void fft_current_val(int *filterBuf,WORD sample_num)//(int *originaBuf,WORD sample_num,
{
  
  
        unsigned int filter_num=0;
        DFT_DATA_EXT DftData;
        INT32S TempRe,data_value;
        INT32S TempIm;
        INT32S Data[2];
          INT8U  cal_num =0;
         //INT8U  earth_fault = OFF;    
      for(int j=0;j< RECORDER_FILTER_LENGTH+20;j++)
          {
            DftCalc(filterBuf[filter_num*80+j],&DftData,&TempRe,&TempIm);
            cal_num++;
            if(cal_num > 2) cal_num =1;
          
          }
          Data[cal_num-1] = DataDeal(TempRe,TempIm);
          Data[cal_num-1]= (INT32S)sqrt((float)Data[cal_num-1]);
          if(cal_num >= 2)
          {
            
            if(Data[1] >= Data[0])
              data_value = Data[1] -Data[0];
            else
              data_value = Data[0] -Data[1];
            if(data_value > 10)//�жϽӵع��ϵĵ�һ����������ͻ��
            {
            
             //earth_fault = ON;
             g_fault_flag = filter_num*80;
             
            }
          }
          filter_num ++; 
        
       
}*/
/***************************************************************
    Function��DftCalc
       ����Ҷ������Чֵ
   
       
      
    ���أ� TRUE���е���ӵع��Ϸ����� FALSE��û�е���ӵع��Ϸ���
***************************************************************/
/*INT8U DftCalc(INT32S buf,DFT_DATA_EXT * DftDate,INT32S* TempRe,INT32S* TempIm)
{
     INT32S temp1,temp2;
   //  static INT8U DelayT=0;
//   DelayT++;
  // if((DelayT>16)||(SysInfoFlag.SysFlagBit.ElectDownFlag==TRUE)||(SysInfoFlag.SysFlagBit.FatultFlag==TRUE))
   {
       //temp1 = buf * CosTab[DftDate->Cnt];      
       //temp2 = buf * SinTab[DftDate->Cnt];       
       DftDate->Real    += (temp1 >> 16);             
       DftDate->Imag    += (temp2 >> 16);
       DftDate->LowReal += (temp1 & 0xFFFF);
       DftDate->LowImag += (temp2 & 0xFFFF);
       DftDate->Cnt++;          
       if((DftDate->Cnt + 1)> RECORDER_FILTER_LENGTH+20)
       {
            *TempRe = ((DftDate->Real >> 2) + (DftDate->LowReal >> 18));  
            *TempIm = ((DftDate->Imag >> 2) + (DftDate->LowImag >> 18));
            DftDate->Real = 0;            
            DftDate->Imag = 0;            
       
            DftDate->LowReal = 0;
            DftDate->LowImag = 0; 
            DftDate->Cnt = 0;
    //    DelayT=0;
            return TRUE;
            
       }
   }
   return FALSE;
}
INT32S DataDeal(INT32S TempRe,INT32S TempIm)
{
     INT32S  val;   
  
     val = (INT32S)((FP32)TempRe * TempRe + (FP32)TempIm * TempIm);
     if(val > 36695396)         
       val = 36695396;
     return val;
}*/

//LZY ������̬�ӵص���ر����ͺ���
INT16U g_sTrEarthPara[3];//��̬�ӵ��жϲ���	0��ʾ���� 1��ʾ��ѹ 2 ��ֵ���õ�������̬λ��ƫ�����
INT16U g_EarthAcuLock;	//��̬�ӵ�ǰ�ĵ糡���ݼ�¼����
#define EARTH_CHAN_I 0	//�ӵز��ε���ͨ��
#define EARTH_CHAN_U 1	//�ӵز��ε�ѹͨ��
#define EARTH_DIFF_DOT 2	//��ֵ���õ�������̬λ��ƫ�����
void TrEarthParaInit(void)
{
	//����Ϊ���������ã�Ĭ��ֵ����30����ֵ�൱��15A���ҷ�ֵ����ѹ40����ֵ
	//��ѹĬ��ֵΪ50����Ե糡��600���ϵ�ϵͳ���ԽϺ��ж���̬������糡ϵͳƫС�������ʵ��µ��糡ֵ������糡ϵͳƫ����Ա����ϵ��糡ֵ
    g_sTrEarthPara[EARTH_CHAN_I] = g_gRunPara[RP_ABSOLUTE_CURREN];	//15A
    //g_sTrEarthPara[EARTH_CHAN_U] = g_gRunPara[RP_ABSOLUTE_VOLTAGE];	//15A
	//����ָʾ���ĵ糡ֵ�ܻ����仯�ϴ� ���Ҫ���ݴ���¼��ʱ�������糡ֵ��һ����̬ͻ���ż�ֵ�Ļ��� 70����ֵ��Ŷ�Ӧ1000�ĵ糡��
    //g_EarthAcuLock = 1024;      //������¼��ָʾ�����жϵ�ʱ�������ѹ����˸ù���ǰ�ĵ糡ֵӦ����ȡ�����ѹ����ǰ�ĵ糡ֵ����Ҫ���һ�ȡ
    g_sTrEarthPara[EARTH_CHAN_U] = g_gRunPara[RP_ABSOLUTE_VOLTAGE] * g_EarthAcuLock >> 10;	
	if(g_sTrEarthPara[EARTH_CHAN_U] < 15)
	{
		g_sTrEarthPara[EARTH_CHAN_U] = 15;
	}
	
	g_sTrEarthPara[EARTH_DIFF_DOT] = g_gRunPara[RP_diff_NUM];
}

//
//==============================================================================
//  ��������   : CfmBreakPoint
//  ��������   :ȷ���Ƿ�Ϊ�յ㣬�յ��������������ֵ�ķֽ�㣬�ҹ�ȥ��������ֵ������������ֵ֮���������ֵ
//  �������   : pDiffValue--�ĸ���ֵ��
//							 DiffIndex---���²�ֵ�����ڵ�Index
//				ParaChang--��������߸������������ֵ֮��ֵ
//  �������   ��
//  ����ֵ     : 0----��ʾδ�ҵ��յ�   -1---��ʾ�ҵ�������  1---��ʾ�ҵ�������
//  ����˵��   : 
//  ����       ������һ
//==============================================================================
INT8S CfmBreakPoint(INT16S *pDiffValue, INT8U DiffIndex, INT16S ParaChang)
{
	INT16S FuPara,LtPara,TwoPara;
	LtPara = (ParaChang - 2 - (ParaChang >> 3));

	FuPara = 0-LtPara;
	TwoPara = (ParaChang << 1);
	
	INT16S DiffValue[4];
	INT16S ValSum01,ValSum23;
	DiffValue[0] = pDiffValue[DiffIndex];
	DiffValue[1] = pDiffValue[(DiffIndex - 1) & 0x3];
	DiffValue[2] = pDiffValue[(DiffIndex - 2) & 0x3];
	DiffValue[3] = pDiffValue[(DiffIndex - 3) & 0x3];
	ValSum01 = DiffValue[0] + DiffValue[1];
	ValSum23 = DiffValue[2] + DiffValue[3];
	
    //��������10�������ܺͱ����������ParaChang
	if(DiffValue[0] >= 0 && DiffValue[1] >= 0 && (ValSum01) >= LtPara
			&& DiffValue[2] < 0 && DiffValue[3] < 0 && (ValSum23) < FuPara
			&& (ValSum01 - ValSum23) >= TwoPara
			)
		{
			
			return  -1;	//������
		}
		if(DiffValue[0] < 0 && DiffValue[1] < 0 && (ValSum01) < FuPara
			&& DiffValue[2] >= 0 && DiffValue[3] >= 0 && (ValSum23) >= (LtPara)
			&& (ValSum23 - ValSum01) >= TwoPara
			)
		{
			return 1;	//������
		}

		return 0;
}

//INT16S FilterDataI[640];	//��̬�˲�������ݻ���
//INT16S FilterDataU[640];	//��̬�˲�������ݻ���
/***************************************************************
    Function��differential_filter
       ���˲�������ݽ���1�ײ��(����)����,���ж��Ƿ�������ӵع���
    ������
        filterBuf_V ��ѹ�˲�֮�������
        filterBuf_I �����˲�֮�������
       SAMPLE_MAX_NUM �˲���ĳ���
       
      
    ���أ� TRUE���е���ӵع��Ϸ����� FALSE��û�е���ӵع��Ϸ���
***************************************************************/
BOOL  differential_filter(INT16U sample_num,INT8U phase_fault,unsigned char line_num)//float *filterBuf_V,float *filterBuf_I)INT16U filterBuf,
{
        unsigned int i,j=0;
        INT32U FWfAddrI = 0;
        INT32U FWfAddrV = 0;
        INT16S filterBufI[10];//960sample_numSAMPLE_MAX_NUMsample_num
        INT16S filterBufV[10];//sample_numSAMPLE_MAX_NUMsample_num
        //INT16S differBuf_V;//[RECORDER_FILTER_LENGTH+20];//sample_numSAMPLE_MAX_NUM
        //INT16S differBuf_I;//[RECORDER_FILTER_LENGTH+20];//sample_numSAMPLE_MAX_NUM
        //INT16U  differI_flag =0;
        //INT16U  differV_flag =0;
        g_differI_weizhi =0;
        g_differV_weizhi=0;
        g_faultI_Valse=0;
        g_faultV_Valse=0;
        INT16U  differmax_flag =0;
        unsigned long FLbAddr_offset = 0;
        //INT16U  differBuf_V_abs =0;
        //INT16U  differBuf_I_abs=0;
        WORD wFltYxIndex = 0;//�������
        unsigned int PageSize;//��ǰҳ��ʣ��ռ�
        INT16U differ_flag = OFF;
        

        //lzy �޸���̬�ӵ��㷨
        INT8U DiffIndex = 0;
        INT8S result;
        INT16S DiffValue1[4],DiffValue2[4],DiffValue3[4];
        for(j = 0; j < 4; j++)
        {
            DiffValue1[j] = 0;
            DiffValue2[j] = 0;
            DiffValue3[j] = 0;
        }
        TrEarthParaInit();	//�ȳ�ʼ����̬�жϵĶ�ֵ

		
        FWfAddrI =  FADDR_WAVE_FILTERI+(long)line_num*(long)FPHASEADDR +160;//40;//�˲����ǰ20���㲻�������+ filterBuf*80*2; //������ַ
        FWfAddrV =  FADDR_WAVE_FILTERV+(long)line_num*(long)FPHASEADDR +160;//40;//+ filterBuf*80*2; //������ַ
        
        Sst26vf064b_Read(FWfAddrI,(unsigned char *)filterBufI,20); //sample_numRECORDER_FILTER_LENGTH+20
        Sst26vf064b_Read(FWfAddrV,(unsigned char *)filterBufV,20); //sample_numRECORDER_FILTER_LENGTH+20
        FLbAddr_offset =20;
        j =0;
        if((sample_num-5) < 80) return FALSE;
        for(i=0;i<(sample_num-5-80);i++)//��ֺ������3RECORDER_FILTER_LENGTH+20
        {
            
            //differBuf_V= (filterBufV[i+5]-filterBufV[i]);//[i]
            if((i%5 ==0) &&(i > 0))
            {
              filterBufI[0] = filterBufI[5];
              filterBufI[1] = filterBufI[6];
              filterBufI[2] = filterBufI[7];
              filterBufI[3] = filterBufI[8];
              filterBufI[4] = filterBufI[9];
              
                FWfAddrI =FADDR_WAVE_FILTERI+(long)line_num*(long)FPHASEADDR +FLbAddr_offset+ 160;//40;
                PageSize = 0x100 - (FWfAddrI%0x100);//��ǰҳ��ʣ��ռ�
                if(PageSize < 10)//WRITEPECORDER��ǰҳ��ʣ��ռ�С��Ҫд��������
                {
                  Sst26vf064b_Read(FWfAddrI,(unsigned char *)&filterBufI[5],PageSize);//pLbBuffÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����                    
                  Sst26vf064b_Read(((FWfAddrI&0xffffff00) + 0x100),(unsigned char *)&filterBufI[5+(PageSize>>1)],(10-PageSize)); //��һҳ����ʼ��ַ               
                  //Sst26vf064b_Read((FWfAddrI&0xffffff00),ReadBack,256);
                  //Sst26vf064b_Read(((FWfAddrI&0xffffff00) + 0x100),ReadBack,256);//����ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����            
                 //Sst26vf064b_Read((FLbAddr&0xffffff00) + 0x100,&ReadBack[PageSize],Data_length1-PageSize);
                }
                else
                {
                  Sst26vf064b_Read(FWfAddrI,(unsigned char *)&filterBufI[5],10);//FLASH_PAGE_SIZE
                  //Sst26vf064b_Read(FWfAddrI,ReadBack,256);//����ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����
                }
                FLbAddr_offset +=10;
                j=0;
            }
            /*
            g_faultI_Valse = (filterBufI[j+5]-filterBufI[j]);//[i]
            if(g_faultI_Valse < 0) 
              differBuf_I_abs = 0-g_faultI_Valse;
            else
              differBuf_I_abs = g_faultI_Valse;
            j++;
            if(differBuf_I_abs >= g_gRunPara[RP_ABSOLUTE_CURREN])//CURRENT_ACTION_THRESHOLD(||(differBuf_V>= VOLTAGE_ACTION_THRESHOLD))////�жϽӵع��ϵĵڶ�����������ͻ��[i][i]
            {
                 
              //differI_flag = i;
                  
              differ_flag = ON; 
              g_differI_weizhi = i;
              break;
            }*/

            //�޸Ľӵ��ж��㷨lzy  ������̬λ���ж�
            DiffIndex++;
            DiffIndex &= 3;
			//FilterDataI[i] = filterBufI[j+5];
						DiffValue1[DiffIndex] = filterBufI[j+5] - filterBufI[j+4];
						result = CfmBreakPoint(DiffValue1, DiffIndex, g_sTrEarthPara[EARTH_CHAN_I]);
						
						if(result != 0)
						{
							g_faultI_Valse = result;	//������̬�������廹�Ǹ�����
							g_differI_weizhi = i;
							differ_flag = ON;
							break;
						}
						DiffValue2[DiffIndex] = filterBufI[j+5] - filterBufI[j+3];
						result = CfmBreakPoint(DiffValue2, DiffIndex, g_sTrEarthPara[EARTH_CHAN_I]);
						if(result != 0)
						{
							g_faultI_Valse = result;	//������̬�������廹�Ǹ�����
							g_differI_weizhi = i - 1;
							differ_flag = ON;
							break;
						}
						
						DiffValue3[DiffIndex] = filterBufI[j+5] - filterBufI[j+2];
						result = CfmBreakPoint(DiffValue3, DiffIndex, g_sTrEarthPara[EARTH_CHAN_I]);
						if(result != 0)
						{
							g_faultI_Valse = result;	//������̬�������廹�Ǹ�����
							g_differI_weizhi = i - 2;
							differ_flag = ON;
							break;
						}
                                                j++;

      
        }
        FLbAddr_offset =20;//20;
        j =0;
        if((sample_num-5) < 80) return FALSE;
        for(i=0;i<(sample_num-5-80);i++)//��ֺ������3RECORDER_FILTER_LENGTH+20
        {
            if((i%5 ==0) &&(i > 0))
            {
              filterBufV[0] = filterBufV[5];
              filterBufV[1] = filterBufV[6];
              filterBufV[2] = filterBufV[7];
              filterBufV[3] = filterBufV[8];
              filterBufV[4] = filterBufV[9];
              
                //FWfAddrV +=FLbAddr_offset;
                FWfAddrV = FADDR_WAVE_FILTERV+(long)line_num*(long)FPHASEADDR+FLbAddr_offset +160;//40;
                PageSize = 0x100 - (FWfAddrV%0x100);//��ǰҳ��ʣ��ռ�
                
                if(PageSize < 10)//WRITEPECORDER��ǰҳ��ʣ��ռ�С��Ҫд��������
                {
                  Sst26vf064b_Read(FWfAddrV,(unsigned char *)&filterBufV[5],PageSize);//pLbBuffÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����                    
                  Sst26vf064b_Read(((FWfAddrV&0xffffff00) + 0x100),(unsigned char *)&filterBufV[5+(PageSize>>1)],(10-PageSize)); //��һҳ����ʼ��ַ               
                  //Sst26vf064b_Read((FWfAddrI&0xffffff00),ReadBack,256);
                  //Sst26vf064b_Read(((FWfAddrI&0xffffff00) + 0x100),ReadBack,256);//����ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����            
                 //Sst26vf064b_Read((FLbAddr&0xffffff00) + 0x100,&ReadBack[PageSize],Data_length1-PageSize);
                }
                else
                {
                  Sst26vf064b_Read(FWfAddrV,(unsigned char *)&filterBufV[5],10);//FLASH_PAGE_SIZE
                  //Sst26vf064b_Read(FWfAddrI,ReadBack,256);//����ÿ�������ȶ���Ϊ��ͬ��δ�ﵽ���ȵİ���ʹ�ð����ȣ��ڶ�ȡʱ������Ч����
                }
                FLbAddr_offset +=10;
                j=0;
            }
            /*
            g_faultV_Valse = (filterBufV[j+5]-filterBufV[j]);//[i]
            if(g_faultV_Valse < 0) 
              differBuf_V_abs = 0-g_faultV_Valse;
            else
              differBuf_V_abs = g_faultV_Valse;
            //differBuf_I= (filterBufI[i+5]-filterBufI[i]);//[i]
             j++;
            if(differBuf_V_abs >= g_gRunPara[RP_ABSOLUTE_VOLTAGE])//VOLTAGE_ACTION_THRESHOLD(||(differBuf_V>= VOLTAGE_ACTION_THRESHOLD))////�жϽӵع��ϵĵڶ�����������ͻ��[i][i]
            {
              g_differV_weizhi = i;
              if(differ_flag == ON)
                differ_flag = TRUE;
              break;
        
            }
            */
            
            //�޸Ľӵ��ж��㷨lzy
            DiffIndex++;
            DiffIndex &= 3;

			//FilterDataU[i] = filterBufV[j+5];
						DiffValue1[DiffIndex] = filterBufV[j+5] - filterBufV[j+4];
						result = CfmBreakPoint(DiffValue1, DiffIndex, g_sTrEarthPara[EARTH_CHAN_U]);
						
						if(result != 0)
						{
							g_faultV_Valse = result;	//������̬�������廹�Ǹ�����
							g_differV_weizhi = i;
							if(differ_flag == ON)
                                                          differ_flag = TRUE;
							break;
						}
						DiffValue2[DiffIndex] = filterBufV[j+5] - filterBufV[j+3];
						result = CfmBreakPoint(DiffValue2, DiffIndex, g_sTrEarthPara[EARTH_CHAN_U]);
						if(result != 0)
						{
							g_faultV_Valse = result;	//������̬�������廹�Ǹ�����
							g_differV_weizhi = i - 1;
							if(differ_flag == ON)
                                                          differ_flag = TRUE;
							break;
						}
						
						DiffValue3[DiffIndex] = filterBufV[j+5] - filterBufV[j+2];
						result = CfmBreakPoint(DiffValue3, DiffIndex, g_sTrEarthPara[EARTH_CHAN_U]);
						if(result != 0)
						{
							g_faultV_Valse = result;	//������̬�������廹�Ǹ�����
							g_differV_weizhi = i - 2;
							if(differ_flag == ON)
                                                          differ_flag = TRUE;
							break;
						}
                                                j++;
        }
        if((((g_faultV_Valse == 1)&&(g_faultI_Valse == (char)-1))||((g_faultV_Valse == (char)-1)&&(g_faultI_Valse == 1))) &&(differ_flag == TRUE))
        {
          //differmax_flag = max(g_differV_weizhi,g_differI_weizhi);
          //if(((differmax_flag>=g_differV_weizhi)&&(differmax_flag - g_differV_weizhi < g_gRunPara[RP_diff_NUM]))||((differmax_flag>=g_differI_weizhi)&&(differmax_flag - g_differI_weizhi <g_gRunPara[RP_diff_NUM])))
            if(g_differV_weizhi >= g_differI_weizhi)
            {
               differmax_flag =  g_differV_weizhi - g_differI_weizhi;
            }
            else
            {
                differmax_flag = g_differI_weizhi - g_differV_weizhi;
            }
            if(differmax_flag <= g_sTrEarthPara[EARTH_DIFF_DOT])
          { 
                wFltYxIndex = LPFToYxId((g_Current_line>>2)&0xFF,phase_fault,1);//group + 1�ӵ�
                if(RI_IN_DAERR == wFltYxIndex)  
                {
                    SetRIStatus(RI_IN_DAERR, 1);
                    //continue;
                }
                
                if(g_gRmtLock[wFltYxIndex + RI_1A_SHORT] == 0)
                {
                    g_gRmtLock[wFltYxIndex + RI_1A_SHORT] = g_gRunPara[RP_FLTLK_T];
                    CreatNewSoe(wFltYxIndex + RI_1A_SHORT,1);//=(g_Current_line>>2)&0xFF;
                    //unsigned char chYkPa  = (1 << 2) + phase_fault;
                    unsigned char chYkPa  = ((pRf->Recorder.Current_line_phase) & 0xFC) + phase_fault;
                    unsigned char Val =1;
                    if(pRf != null) ((CPrtcRF *)pRf)->SendWCmdToRf001(chYkPa,29,&Val);
                }
        
                
          }
          return TRUE;         
        }
          return FALSE;   
}
INT16U max(INT16U x,INT16U y)
{    
  INT16U max;    
  if(x<y)    max=y;    
  else max=x;    
        //if(max<z)    max=z; 
        
  return max;
}