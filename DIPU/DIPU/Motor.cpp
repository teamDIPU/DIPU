//#include <stdio.h>
//#include <conio.h>
//#include <dynamixel.h>
//#include <windows.h>
//
//#pragma comment(lib, "dynamixel.lib")
//
//#define DEFAULT_PORTNUM			4
//#define DEFAULT_BAUDNUM			1
//#define DEFAULT_ID				4
//#define P_GOAL_POSITION_L		30
//#define P_PRESENT_POSITION_L	30
//#define P_MOVING				46
//
//void PrintErrorCode();
//void PrintCommStatus(int CommStatus);
//
//
//void main()
//{
//	int result;
//	int GoalPos[5] = { 45,90,150,180,300 };
//	int PresentPos;
//	int CommStatus;
//	int Moving;
//	result = dxl_initialize(DEFAULT_PORTNUM, DEFAULT_BAUDNUM);
//
//	if (result == 1)
//	{
//		// Succeed to open USB2Dynamixel 
//		printf("suc\n");
//	}
//
//	else if (result == 0)
//
//	{
//		printf("fail\n");
//		// Failed to open USB2Dynamixel
//
//	}
//
//	// Write goal position
//	for (int index = 0; index < 5; index++)
//	{
//		dxl_write_word(DEFAULT_ID, P_GOAL_POSITION_L, GoalPos[index]);
//		Sleep(1000);
//
//		if (index == 4)
//			index = 0;
//	}
//
//
//	// Read present position
//
//	PresentPos = dxl_read_word(DEFAULT_ID, P_PRESENT_POSITION_L);
//
//	CommStatus = dxl_get_result();
//
//	if (CommStatus == COMM_RXSUCCESS)
//
//	{
//
//		PrintErrorCode();
//
//	}
//
//	else
//
//	{
//
//		PrintCommStatus(CommStatus);
//
//	}
//	// Check moving done
//
//	Moving = dxl_read_byte(DEFAULT_ID, P_MOVING);
//
//
//
//	// Close device
//
//	dxl_terminate();
//
//}
//
//
//void PrintErrorCode()
//
//{
//
//	// Method 2. using function
//
//	if (dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
//
//		printf("Input voltage error!\n");
//
//	if (dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
//
//		printf("Angle limit error!\n");
//
//	if (dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
//
//		printf("Overheat error!\n");
//
//	if (dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
//
//		printf("Out of range error!\n");
//
//	if (dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
//
//		printf("Checksum error!\n");
//
//	if (dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
//
//		printf("Overload error!\n");
//
//	if (dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
//
//		printf("Instruction code error!\n");
//
//}
//
//void PrintCommStatus(int CommStatus)
//
//{
//
//	switch (CommStatus)
//
//	{
//
//	case COMM_TXFAIL:
//
//		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
//
//		break;
//
//	case COMM_TXERROR:
//
//		printf("COMM_TXERROR: Incorrect instruction packet!\n");
//
//		break;
//
//	case COMM_RXFAIL:
//
//		printf("COMM_RXFAIL: Failed get status packet from device!\n");
//
//		break;
//
//	case COMM_RXWAITING:
//
//		printf("COMM_RXWAITING: Now recieving status packet!\n");
//
//		break;
//
//	case COMM_RXTIMEOUT:
//
//		printf("COMM_RXTIMEOUT: There is no status packet!\n");
//
//		break;
//
//	case COMM_RXCORRUPT:
//
//		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
//
//		break;
//
//	default:
//
//		printf("This is unknown error code!\n");
//
//		break;
//
//	}
//
//}
