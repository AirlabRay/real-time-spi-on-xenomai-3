// RTDM SPI on Raspberry Pi3
// Testing program
// 2017/03/03
// NTUST AIRLab Ray

#ifndef SPI_H
#define SPI_H

/* BCM283x SPI RTDM driver header */
#include "spi-bcm283x-rtdm.h"

//function
int open_device();
int spi_test();
void spi();

#endif 