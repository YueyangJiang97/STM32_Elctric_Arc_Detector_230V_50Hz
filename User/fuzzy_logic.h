#ifndef __FUZZY_LOGIC_H
#define __FUZZY_LOGIC_H
/*
 *!
 * \file       : fuzzy_logic.h
 * \brief      : 
 * \version    : 
 * \date       : 2021/04/13
 * \author     : Yueyang Jiang
 * Last modified by Yueyang Jiang 2021/04/13
 * Copyright (c) 2021 by Yueyang Jiang. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include <stdio.h>
#include "arm_math.h"  
#include "fatfs_sd.h"
#include "ADC.h"  
#include "kalman.h" 
/* \defgroup FUZZY_LOGIC_Exported_Defines
 * \{
 */
#define MINFAULT_MAX_NUM 			10 /*5*/
#define MAJFAULT_MAX_NUM 			10 /*1*/
#define FLROUND_MAX_NUM       6
/**
 * \}
 */
 
/* \defgroup FUZZY_LOGIC_Exported_TypesDefinitions
 * \{
 */
typedef struct
{
	uint8_t minFault[6];
	uint8_t majFault[6];
	uint8_t SumFault;
	uint8_t SumMajFault;
	uint8_t round;
	uint8_t preMinFalt;
	uint8_t preMajFalt;
	float 	meanDiff3;
	float 	threshold;
}SimpleLogic;

/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Exported_Variables
 * \{
 */
extern volatile uint8_t CircuitOpen_Flag;
static volatile uint8_t flround = 0;
extern float32_t diff3_Buffer[ADC_BUFFER_SIZE];
extern float32_t Res_Buffer[ADC_BUFFER_SIZE];
extern SimpleLogic sl;
/**
 * \}
 */
   
/* \defgroup FUZZY_LOGIC_Exported_Functions
 * \{
 */
 static float Threshold_Update(uint16_t size);
static void Fault_update(uint16_t size);
void Simple_FuzzyLogic_init(void);
void Simple_FuzzyLogic(uint16_t size);
/**
 * \}
 */
#endif 
/************************ (C) COPYRIGHT Yueyang Jiang *****END OF FILE****/

