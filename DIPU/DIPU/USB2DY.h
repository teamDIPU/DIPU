#pragma once
#include <stdio.h>
#include <conio.h>
#include <dynamixel.h>
#include <windows.h>

#pragma comment(lib, "dynamixel.lib")

#define DEFAULT_PORTNUM			4
#define DEFAULT_BAUDNUM			1
#define DEFAULT_ID				4
#define P_GOAL_POSITION_L		30
#define P_PRESENT_POSITION_L	30
#define P_MOVING				46
#define NUM_ACTUATOR			3 // Number of actuator
#define P_GOAL_POSITION_L		30
#define P_GOAL_SPEED_L			32
#define P_GOAL_SPEED_H			33
#define CW_Compliance_Slope		28
#define CCW_Compliance_Slope	29
void PrintErrorCode();
void PrintCommStatus(int CommStatus);