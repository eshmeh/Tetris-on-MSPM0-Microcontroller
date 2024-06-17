/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#define PA12INDEX 33
#define PA24INDEX 53
#define PA28INDEX 2
#define PA31INDEX 5
#define initinput 0x00050081
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    IOMUX->SECCFG.PINCM[PA12INDEX] = initinput;
    IOMUX->SECCFG.PINCM[PA24INDEX] = initinput;
    IOMUX->SECCFG.PINCM[PA28INDEX] = initinput;
    IOMUX->SECCFG.PINCM[PA31INDEX] = initinput;
    GPIOA->DOE31_0 = GPIOA->DOE31_0 | (1<<12) | (1<<24) | (1<<28) | (1<<31);
}
// return current state of switches
uint32_t Switch_In(void){
    uint32_t input = GPIOA->DIN31_0;
    uint32_t mask12 = 0x1000;
    uint32_t mask24 = 0x1000000;
    uint32_t mask28 = 0x10000000;
    uint32_t mask31 = 0x80000000;
    uint32_t in12 = input & mask12;
    uint32_t in24 = input & mask24;
    uint32_t in28 = input & mask28;
    uint32_t in31 = input & mask31;
    in12 = in12 >> 12;
    in24 = in24 >> 23;
    in28 = in28 >> 26;
    in31 = in31 >> 28;
    input = in12 | in24 | in28 | in31;
    return input;
}
