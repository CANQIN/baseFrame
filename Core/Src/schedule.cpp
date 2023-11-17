#include "schedule.h"
#include "tim.h"

#include "virtualTask.h"

/*初始化完成标志*/
uint8_t Init_OK;
/*机器人使能标志*/
uint8_t RobotEnable;

void boardALLInit(void)
{
//	// CAN初始化
//	can.init(&hfdcan1);
//	can.init(&hfdcan2);
//	can.init(&hfdcan3);
//    Motor::canMsgInit();
//    
//	// 遥控器初始化
//	rc.init();
	// 任务初始化
	for (uint8_t i = 0; i < VirtualTask::taskNum; i++)
		VirtualTask::taskList[i]->init();

	// 看门狗初始化
	//  iwdg.init();

	/*初始化完成*/
	Init_OK = 1;
}

void TDT_Loop_1000Hz(void) // 1ms执行一次
{
}

void TDT_Loop_500Hz(void) // 2ms执行一次
{
    

	// 任务调度
	for (uint8_t i = 0; i < VirtualTask::taskNum; i++)
	{
		// 调用运行态任务的主函数
		if (RobotEnable == ENABLE && VirtualTask::taskList[i]->status == taskStateRun)
			VirtualTask::taskList[i]->run();
		else if (VirtualTask::taskList[i]->alwaysRunFlag == true)
			VirtualTask::taskList[i]->run();
	}

//	// 通讯处理
//	if (RobotEnable == DISABLE)
//		can.setAllZero();
//    can.sendMsg();
//    
//	// AllOnlineObject离线检查
//	for (uint8_t i = 0; i < onlineNum; i++)
//		onlinePtrList[i]->check();

//	// 遥控器离线失能
//	if (rc.online.isOffLine())
//		RobotEnable = DISABLE;
//    
//    //复位
//    if(rc.key.SW2 == RCS::Down && rc.sw1Tick == RCS::Mid_Down)
//        softwareReset();
}

void TDT_Loop_200Hz(void) // 5ms执行一次
{
}


void TDT_Loop_100Hz(void) // 10ms执行一次
{
}

void TDT_Loop_50Hz(void) // 20ms执行一次
{
}

void TDT_Loop_20Hz(void) // 50ms执行一次
{
}

void TDT_Loop_10Hz(void) // 100ms执行一次
{
}

void TDT_Loop_2Hz(void) // 500ms执行一次
{
}

void TDT_Loop_1Hz(void) // 1000ms执行一次
{
}

/**
 * @ingroup TDT_Frame
 * @defgroup TDT_SCHEDULE_API schedule相关接口
 * @brief 该模块展示了schedule的相关接口
 */

void TDT_Loop(struct _Schedule *robotSchedule)
{
	uint64_t startTimeStamp = getSysTimeUs();
	TDT_Loop_1000Hz();
	robotSchedule->runTime_1ms = getSysTimeUs() - startTimeStamp;

	if (robotSchedule->cnt_2ms >= 2)
	{
		robotSchedule->cnt_2ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_500Hz();
		robotSchedule->runTime_2ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_5ms >= 5)
	{
		robotSchedule->cnt_5ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_200Hz();
		robotSchedule->runTime_5ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_10ms >= 10)
	{
		robotSchedule->cnt_10ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_100Hz();
		robotSchedule->runTime_10ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_20ms >= 20)
	{
		robotSchedule->cnt_20ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_50Hz();
		robotSchedule->runTime_20ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_50ms >= 50)
	{
		robotSchedule->cnt_50ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_20Hz();
		robotSchedule->runTime_50ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_100ms >= 100)
	{
		robotSchedule->cnt_100ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_10Hz();
		robotSchedule->runTime_100ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_500ms >= 500)
	{
		robotSchedule->cnt_500ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_2Hz();
		robotSchedule->runTime_500ms = getSysTimeUs() - startTimeStamp;
	}
	if (robotSchedule->cnt_1000ms >= 1000)
	{
		robotSchedule->cnt_1000ms = 0;
		uint64_t startTimeStamp = getSysTimeUs();
		TDT_Loop_1Hz();
		robotSchedule->runTime_1000ms = getSysTimeUs() - startTimeStamp;
	}
	robotSchedule->CPU_usage = (robotSchedule->runTime_1ms * 1000 + robotSchedule->runTime_2ms * 500 + robotSchedule->runTime_5ms * 200 + robotSchedule->runTime_10ms * 100 + robotSchedule->runTime_20ms * 50 + robotSchedule->runTime_50ms * 20 + robotSchedule->runTime_100ms * 10 + robotSchedule->runTime_500ms * 2 + robotSchedule->runTime_1000ms) / 1e6f;
}

volatile uint32_t tim4Uptime = 0;
volatile uint32_t tim4UptimeUs = 0;
Schedule scheduleLoop;

/**
 * @brief 获取us级时间
 * @return    单片机从运行到此时的时间，单位us
 * @note 滴答定时器分频在此函数处理好像会损失精度
 */
uint64_t getSysTimeUs(void)
{
    uint64_t ms;
    uint64_t value;
//    ms = tim4Uptime;
//    value = (ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD);
    return value;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim4)
	{
		tim4Uptime++;
		if (Init_OK)
		{
			//1ms不需要计数
			scheduleLoop.cnt_2ms++;
			scheduleLoop.cnt_5ms++;
			scheduleLoop.cnt_10ms++;
			scheduleLoop.cnt_20ms++;
			scheduleLoop.cnt_50ms++;
			scheduleLoop.cnt_100ms++;
			scheduleLoop.cnt_500ms++;
			scheduleLoop.cnt_1000ms++;
			
			TDT_Loop(&scheduleLoop);
		}
	}
}
