
/*
 *!
 * \file       :fuzzy_logic.c
 * \brief      : 
 * \version    : 
 * \date       : 2021/04/13
 * \author     : Yueyang Jiang
 *Last modified by Yueyang Jiang 2021/04/13
 *Copyright (c) 2021 by Yueyang Jiang. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "fuzzy_logic.h"   
/* \defgroup FUZZY_LOGIC_Private_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Private_Defines
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Private_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Private_Variables
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Private_FunctionPrototypes
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Private_Functions
 * \{
 */
/*Simple logic initiation*/
void Simple_FuzzyLogic_init(void)
{
	sl.round = 0;
	sl.majFault[sl.round] = 0;
	sl.minFault[sl.round] = 0;
	sl.threshold = 250;
	sl.SumMajFault = 0;
	sl.SumFault = 0;
}

/*Update threshold*/
static float Threshold_Update(uint16_t size)
{
	float threshold;
	float ResMax;	
	float ResMean;
	arm_max_no_idx_f32(Res_Buffer, size+3, &ResMax);
	arm_mean_f32(Res_Buffer, size+3, &ResMean);
	
	if(ResMean < 0.3f)
	{
		if(ResMax > 6*ResMean)
		{	
			threshold *= 4;
			threshold = threshold < 150 ? 150 : threshold;
			threshold = threshold > 200 ? 200 : threshold;
			
			return threshold;
		}
		else return 250;
	}
	else if(ResMean > 0.3f && ResMean < 0.9f)
	{
		arm_mean_f32(diff3_Buffer, size, &threshold);
		if(ResMax > 3*ResMean)
		{	
			threshold *= 6.5f;
			threshold = threshold < 180 ? 180 : threshold;
			threshold = threshold > 220 ? 220 : threshold;
			return threshold;
		}
		else return 250;
	}
	else return 400;
}

/*Update fault symptome*/
static void Fault_update(uint16_t size)
{
	uint8_t *pFalt; 
	uint8_t *pPreFalt; 
	
	if(sl.threshold >= 250)
	{
		pFalt = sl.majFault;
		pPreFalt = &sl.preMajFalt;
	}
	else
	{
		pFalt = sl.minFault;
		pPreFalt = &sl.preMinFalt;
	}
	
	for(int i = 0; i < size; i++)
	{
		if((diff3_Buffer[i] > sl.threshold) && !(*pPreFalt))
		{
			*pPreFalt = 1;
			pFalt[sl.round]++;
		}
		else *pPreFalt = 0;
	}
}

/*Simple logic decision*/
void Simple_FuzzyLogic(uint16_t size)
{ 
	sl.threshold = Threshold_Update(size);
	Fault_update(size);
	
	sl.SumMajFault = 0;
	sl.SumFault = 0;
	for(int i = 0; i < FLROUND_MAX_NUM; i++)
	{
		sl.SumMajFault += sl.majFault[i];	
		sl.SumFault += (sl.minFault[i] + sl.majFault[i]);	
	}
	
	if(sl.SumMajFault > MAJFAULT_MAX_NUM || sl.SumFault > MINFAULT_MAX_NUM)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0, GPIO_PIN_SET);
		CircuitOpen_Flag = 1;
		return;
	}

	sl.round++;
	sl.round = sl.round%FLROUND_MAX_NUM;
	sl.majFault[sl.round] = 0;
	sl.minFault[sl.round] = 0;	
}
/**
 * \}
 */
   
/* \addtogroup FUZZY_LOGIC_Exported_Functions
 * \{
 */
     
/**
 * \}
 */
  
/************************ (C) COPYRIGHT Yueyang Jiang *****END OF FILE****/

