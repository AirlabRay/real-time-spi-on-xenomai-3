#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi.h"

/* RTDM header */
#include <rtdm/rtdm.h>

/* BCM283x SPI RTDM driver header */
#include "spi-bcm283x-rtdm.h"

// open rtdm spi device
static const char *device = "/dev/rtdm/spidev0.0";

/** Transmit buffer */
uint8_t tx_buffer[]={
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95,
	};

/** Receive buffer */
uint8_t rx_buffer[]={0, };

/** Handle to the bcm283x rtdm spi driver instance. */
static int device_handle = -1;

/**
 * Open and configure device
 * @return 0 in case of success, a negative value otherwise.
 */
int open_device() {

    int res;
    int value;

    /* Open device */
    res = open(device, O_RDWR);
    if (res < 0) {
        printf("%s: Could not open spi device, open has failed with %d (%s)\n.", __FUNCTION__, errno, strerror(errno));
        return -1;
    } else {
        printf("%s: Device opened. \n", __FUNCTION__);
        device_handle = res;
    }

    /* Configure device */
    value = BCM283X_SPI_BIT_ORDER_MSBFIRST;
    res = ioctl(device_handle, BCM283X_SPI_SET_BIT_ORDER, &value);
    if (res < 0) {
        printf("%s: Could not configure bit order, ioctl has failed with %d (%s). \n", __FUNCTION__, errno, strerror(errno));
        device_handle = 0;
        return -1;
    }
    value = BCM283X_SPI_DATA_MODE_0;
    res = ioctl(device_handle, BCM283X_SPI_SET_DATA_MODE, &value);
    if (res < 0) {
        printf("%s: Could not configure data mode, ioctl has failed with %d (%s).", __FUNCTION__, errno, strerror(errno));
        device_handle = 0;
        return -1;
    }
    value = BCM283X_SPI_SPEED_8MHz;
    res = ioctl(device_handle, BCM283X_SPI_SET_SPEED, &value);
    if (res < 0) {
        printf("%s: Could not configure bus speed, ioctl has failed with %d (%s).", __FUNCTION__, errno, strerror(errno));
        device_handle = 0;
        return -1;
    }
    value = BCM283X_SPI_CS_POL_LOW;
    res = ioctl(device_handle, BCM283X_SPI_SET_CS_POLARITY, &value);
    if (res < 0) {
        printf("%s: Could not configure chip select polarity, ioctl has failed with %d (%s).", __FUNCTION__, errno, strerror(errno));
        device_handle = 0;
        return -1;
    }

    printf("%s: Device sucessfully configured.\n", __FUNCTION__);
    return 0;

}

int spi_test(int counter) {

    printf("spi_test starting...\n");
    int res;
    int spi_frame_size;
    int i = 1;
    
    /* Open device */
//     res = open_device();
//     if (res < 0) {
//         printf("%s: Could not open device, exiting. \n", __FUNCTION__);
//         return -1;
//     }
    
    sleep(3);
    
    while (i < counter) {
    ssize_t size;
    spi_frame_size=sizeof(tx_buffer);
    memset(rx_buffer, 0, spi_frame_size);

    //printf("h: %d \n",device_handle);

    printf("%d \n", i);
    /* Write from tx buffer */
    size = write(device_handle, tx_buffer, spi_frame_size);
    printf("send: ");
    for(int i=0;i<spi_frame_size;i++)		
    printf("0x%02x ", tx_buffer[i]);
    printf("\n");

    /* Receive to rx buffer */
    size = read(device_handle, rx_buffer, spi_frame_size); 
    printf("read: ");
    for(int i=0;i<spi_frame_size;i++)		
    printf("0x%02x ", rx_buffer[i]);
    printf("\n");
  
    i++;
    usleep(1000);
    }
    return -1;
}

void spi() {

    //printf("spi_test starting...\n");
    ssize_t size;
    int spi_frame_size=sizeof(tx_buffer);
    memset(rx_buffer, 0, spi_frame_size);

    /* Write from tx buffer */
    size = write(device_handle, tx_buffer, spi_frame_size);
//     printf("send: ");
//     for(int i=0;i<spi_frame_size;i++)		
//     printf("0x%02x ", tx_buffer[i]);
//     printf("\n");

    /* Receive to rx buffer */
    size = read(device_handle, rx_buffer, spi_frame_size); 
//     printf("read: ");
//     for(int i=0;i<spi_frame_size;i++)		
//     printf("0x%02x ", rx_buffer[i]);
//     printf("\n");
    
    //printf("write and read success!");
}