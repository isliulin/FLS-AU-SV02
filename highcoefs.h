/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 *
 * Generated by MATLAB(R) 7.10 and the Signal Processing Toolbox 6.13.
 *
 * Generated on: 14-Jul-2016 09:54:04
 *
 */

/*
 * Discrete-Time IIR Filter (real)
 * -------------------------------
 * Filter Structure    : Direct-Form II
 * Numerator Length    : 5
 * Denominator Length  : 5
 * Stable              : Yes
 * Linear Phase        : No
 */

/* General type conversion for MATLAB generated C-code  */
//#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * D:\Program Files\MATLAB\R2010a\extern\include\tmwtypes.h 
 */
//#ifndef _RECORDER_WAVE_H_
//#define _RECORDER_WAVE_H_

#ifdef  CHEBYSHEV_FILTER_GLOBALS
#define CHEBYSHEV_FILTER_EXT
#else
#define CHEBYSHEV_FILTER_EXT  extern
#endif

/*const int NL_New = 5;
const float NUM_New[5] = {   //real32_T
     0.5014006562223,   -2.005602624889,    3.008403937334,   -2.005602624889,//滤波器实现的系数，  0.5014   -2.0056    3.0084   -2.0056    0.5014
     0.5014006562223
};//单片机处理单精度
const int DL_New = 5;
const float DEN_New[5] = {
                   1,   -2.809934650895,    3.159824193546,   -1.665149397058,//滤波器实现的系数 1.0000   -2.8099    3.1598   -1.6651    0.3664
     0.3663843869895
};*/



#define SAMPLE_MAX_NUM       1200 //采样点的个数
#define FILTER_TYPE_V       0X55 //电压滤波
#define FILTER_TYPE_I       0XAA //电流滤波

#define CURRENT_ACTION_THRESHOLD   30 //电流动作阀值
#define VOLTAGE_ACTION_THRESHOLD   30 //电压动作阀值

/*const INT16S CosTab[SAMPLE_TIME_NUM]= { 32767,   30273,   23170,   12539,	    
                                            0,  -12539,  -23170,  -30273,
				       -32768,  -30273,  -23170,  -12539,
                                            0,   12539,   23170,   30273};         
const INT16S SinTab[SAMPLE_TIME_NUM]= {     0,   12539,   23170,   30273,  
                                        32767,   30273,   23170,   12539,
                                            0,  -12539,  -23170,  -30273, 
                                       -32768,  -30273,  -23170,  -12539};*/



//typedef 

/*struct DFT_DATA_EXT
{
    long  Real;
    long  Imag;
    long  LowReal;
    long  LowImag;
    unsigned char Cnt;
};*/
//CHEBYSHEV_FILTER_EXT   INT16U g_fault_flag;
//INT8U DftCalc(INT32S buf,DFT_DATA_EXT * DftDate,INT32S* TempRe,INT32S* TempIm);
//CHEBYSHEV_FILTER_EXT struct DFT_DATA_EXT gRecorder_data;  //用来标识是否在读录波数据

CHEBYSHEV_FILTER_EXT void chebyshev_filter(WORD sample_num,unsigned char VI_flag,unsigned char line_num,INT8U phase_fault);//int *originaBuf,
CHEBYSHEV_FILTER_EXT void fft_current_val  (int *filterBuf,WORD sample_num);//,unsigned char VI_flag,unsigned char line_num);
CHEBYSHEV_FILTER_EXT BOOL differential_filter(INT16U sample_num,INT8U phase_fault,unsigned char line_num);//float *filterBuf_V,float *filterBuf_I)INT16U filterBuf,
INT16U max(INT16U x,INT16U y);