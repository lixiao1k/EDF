/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            WIN32 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE    128
#define  TASK_START_PRIO    5

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK        AppStartTaskStk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
//MY Modification
static  void  periodicTask(void  *p_arg);

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(INT8U rx_data);
#endif

//MY Modification

static OS_STK stack1[APP_TASK_STK_SIZE];
static OS_STK stack2[APP_TASK_STK_SIZE];
static OS_STK stack3[APP_TASK_STK_SIZE];

// compTime,deadline,timeout waiting time
INT32S limits[][3] = {
        { 0, 0, 0 },
        { 100, 300, 0 },
        { 300,600,0},
        { 400, 900, 0 }
};

/*
*********************************************************************************************************
*                                                _tmain()
*
* Description : This is the standard entry point for C++ WIN32 code.  
* Arguments   : none
*********************************************************************************************************
*/

void main(int argc, char *argv[])
{
	INT8U  err;


#if 0
    BSP_IntDisAll();                       /* For an embedded target, disable all interrupts until we are ready to accept them */
#endif

    OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"                                      */

	
	//MY Modification

    // OSTaskCreateExt(AppStartTask,
    //                 (void *)0,
    //                 (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
    //                 TASK_START_PRIO,
    //                 TASK_START_PRIO,
    //                 (OS_STK *)&AppStartTaskStk[0],
    //                 TASK_STK_SIZE,
    //                 (void *)0,
    //                 OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(periodicTask,
        (void *)0,
        stack1 + (APP_TASK_STK_SIZE - 1),
        (INT8U)(1),
        (INT16U)(1),
        stack1,
        (INT32U)APP_TASK_STK_SIZE,
        (void *)limits[1],
        (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

     OSTaskCreateExt(periodicTask,
         (void *)0,
         stack2 + (APP_TASK_STK_SIZE - 1),
         (INT8U)(2),
         (INT16U)(2),
         stack2,
         (INT32U)APP_TASK_STK_SIZE,
         (void *)limits[2],
         (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

     OSTaskCreateExt(periodicTask,
         (void *)0,
         stack3 + (APP_TASK_STK_SIZE - 1),
         (INT8U)(3),
         (INT16U)(3),
         stack3,
         (INT32U)APP_TASK_STK_SIZE,
         (void *)limits[3],
         (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &err);
#endif

#if OS_TASK_NAME_SIZE > 14
    OSTaskNameSet(OS_IDLE_PRIO, (INT8U *)"uC/OS-II Idle", &err);
#endif

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_STAT_PRIO, "uC/OS-II Stat", &err);
#endif

     OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)                               */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/


//MY Modification
static void periodicTask(void *p_arg)
{
    INT32S start;//the start time 
    INT32S end;//the end time
    INT32S toDelay;
    start = 0;

    while (1)
    {
        while (OSTCBCur->compTime > 0)//C ticks
        {
            //Do nothing
        }
        end = OSTimeGet();//end time 
		toDelay = OSTCBCur->period - (end - start)/* % OSTCBCur->period;*/;
		if (toDelay <= 0) {
			toDelay = 0;
		}
        start += (OSTCBCur->period);
        OSTCBCur->compTime = OSTCBCur->fullCompTime;
        OSTimeDly(toDelay);
    }
}