#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include "spi.h"

#define TASK_STKSZ 0 //default stack size
#define TASK_PRIO 99 //0-99
#define TASK_MODE 0  //no flags

RT_TASK demo_task;

void demo(void *arg){
  
  RTIME now, previous, intial, error;
  int i = 0;
  int looptime = 10000;
  double latency = 0;
  
  /*
  * Arguments: &task (NULL=self),start time, period (here: 1 ms)
  */
  rt_task_set_periodic(NULL, TM_NOW, 1000000);
  previous = rt_timer_read();
  intial = rt_timer_read();
  
  while (i < looptime) {
    
    rt_task_wait_period(NULL);
    spi();
    now = rt_timer_read();
    
    /*
    * NOTE: printf may have unexpected impact on the timing of
    *       your program. It is used here in the critical loop
    *       only for demonstration purposes.
    */
    printf("[%f] ",(double)(now-intial) / 1000000);
    printf(" %ld.%06ld ms ",
    (long)(now - previous) / 1000000,
    (long)(now - previous) % 1000000);
    
    error = 1000000-(now - previous);
    
    printf("  [latency] %.03f us \n", (double)abs(error)/1000);
    latency += (double)abs(error)/1000;
    previous = now;
    i++;
    }
    
   latency = latency/looptime;
   printf("----------------------------------------------- \n");
   printf("[avg.latency]  %f us \n", latency);
   printf("----------------------------------------------- \n");
   
   alarm(1);
}




int main(int argc, char** argv) {
  
   //open rtdm spi
   open_device();
   
   //test spi write & read
   //int counter=100;
   //spi_test(counter);
   
   /* Avoids memory swapping for this program */
   mlockall(MCL_CURRENT|MCL_FUTURE);

   /*
    * Arguments: &task,
    *            name,
    *            stack size (0=default),
    *            priority,
    *            mode (FPU, start suspended, ...)
    */
   rt_task_create(&demo_task, "RT-spi", TASK_STKSZ, TASK_PRIO, TASK_MODE);

   /*
    * Arguments: &task,
    *            task function,
    *            function argument
    */
   rt_task_start(&demo_task, &demo, NULL);

   pause();

   rt_task_delete(&demo_task);
  
}
