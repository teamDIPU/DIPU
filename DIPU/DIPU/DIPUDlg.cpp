
// DIPUDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DIPU.h"
#include "DIPUDlg.h"
#include "afxdialogex.h"
#include "comm.h"
#include <math.h>
#include "USB2DY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.1415
#define conv (1024./300)
#define ratio 1.3
int motor_flag = 0;

vector<vector<Point2d>> contours;
DIPU a;

void PrintCommStatus(int CommStatus);
void PrintErrorCode();

int  id[3] = { 1, 2, 3 };
double  Q[3];

// CDIPUDlg 대화 상자

CDIPUDlg::CDIPUDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIPU_DIALOG, pParent)
	, m_nSettingPort(0)
	, m_nSettingBaud(0)
	, m_nSettingData(0)
	, m_nSettingParity(0)
	, m_nSettingFlow(0)
	, m_EditData(_T(""))
	, m_nSettingStop(0)
	, flag_paint(false)
	, rec_x1(0)
	, rec_x2(0)
	, rec_y1(0)
	, rec_y2(0)
	, flag_Ldown(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDIPUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_AVR, m_RadioAVR);
	DDX_Control(pDX, IDC_RADIO_USB2DY, m_RadioUSB2DY);
	DDX_Control(pDX, IDC_PIC, m_stDisplay);
}

BEGIN_MESSAGE_MAP(CDIPUDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_SERIAL, &CDIPUDlg::OnBnClickedBtnOpenSerial)
	ON_BN_CLICKED(IDOK, &CDIPUDlg::OnBnClickedOk)
	ON_MESSAGE(WM_RECEIVEDATA,OnReceiveData)
	ON_BN_CLICKED(IDC_BTN_SEND_SERIAL, &CDIPUDlg::OnBnClickedBtnSendSerial)
	ON_BN_CLICKED(IDCANCEL, &CDIPUDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SEND_MOT_DEG, &CDIPUDlg::OnBnClickedBtnSendMotDeg)
	ON_BN_CLICKED(IDC_BTN_SEND_MOT_POS, &CDIPUDlg::OnBnClickedBtnSendMotPos)
	ON_BN_CLICKED(IDC_BTN_DRAW_LINE, &CDIPUDlg::OnBnClickedBtnDrawLine)
	ON_BN_CLICKED(IDC_BTN_DRAW_CIRCLE, &CDIPUDlg::OnBnClickedBtnDrawCircle)
	ON_BN_CLICKED(IDC_BTN_MOT_INIT, &CDIPUDlg::OnBnClickedBtnMotInit)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_PAINT_ON, &CDIPUDlg::OnBnClickedPaintOn)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_PAINT_INIT, &CDIPUDlg::OnBnClickedPaintInit)
	ON_BN_CLICKED(IDC_IMG_DRAW, &CDIPUDlg::OnBnClickedImgDraw)
	ON_STN_CLICKED(IDC_PIC, &CDIPUDlg::OnStnClickedPic)
	ON_BN_CLICKED(IDC_IMAGPROCESSING, &CDIPUDlg::OnBnClickedImagprocessing)
END_MESSAGE_MAP()


// CDIPUDlg 메시지 처리기

BOOL CDIPUDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//통신
	m_Comm.SetHwnd(this->m_hWnd);
	LoadSettings();
	UpdateData(FALSE);

	//Radio
	m_RadioAVR.SetCheck(BST_CHECKED);
	
	
	//A4초기화
	A4_x1 = 186;
	A4_x2 = A4_x1 + 210 ;
	A4_y1 = -148.5;
	A4_y2 = A4_y1 + 297 ;


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDIPUDlg::OnPaint()
{
	// 그리기를 위한 디바이스 컨텍스트입니다.
	CPaintDC dc(this);
	if (IsIconic())
	{
		 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		//추가
		int x1 = 600;
		int y1 = 220;
		int x2, y2;
		x2 = x1 + 297* ratio;
		y2 = y1 + 210* ratio;
		dc.Rectangle(x1, y1, x2, y2);

		//전역변수 이동
		rec_x1 = x1;
		rec_x2 = x2;
		rec_y1 = y1;
		rec_y2 = y2;
		CDialogEx::OnPaint();
		
		

	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDIPUDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CDIPUDlg::OnBnClickedBtnOpenSerial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_BTN_OPEN_SERIAL))->GetCheck();

	if (bCheck) // 통신 시작 명령
	{
		if (m_Comm.OpenCommPort(&Int2TTY()) != TRUE)
		{
			CString str;
			str.Format("COM%d 이 이미 사용중인지 확인하세요", m_nSettingPort + 1);
			AfxMessageBox(str);

			((CButton*)GetDlgItem(IDC_BTN_OPEN_SERIAL))->SetCheck(!bCheck);
			return;
		}
		GetDlgItem(IDC_BTN_OPEN_SERIAL)->SetWindowTextA("통신종료");

		GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(FALSE);
	}
	else // 통신 종료 명령
	{
		m_Comm.CloseConnection();

		GetDlgItem(IDC_BTN_OPEN_SERIAL)->SetWindowTextA("통신시작");

		GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BAUD)->EnableWindow(TRUE);
	}

}


TTYSTRUCT CDIPUDlg::Int2TTY()
{
	TTYSTRUCT tty;
	ZERO_MEMORY(tty);

	
	tty.byCommPort = (BYTE)m_nSettingPort + 1;
	tty.byXonXoff = FALSE;
	tty.byByteSize = (BYTE)_nDataValues[m_nSettingData];
	tty.byFlowCtrl = (BYTE)_nFlow[m_nSettingFlow];
	tty.byParity = (BYTE)m_nSettingParity;
	tty.byStopBits = (BYTE)_nStopBits[m_nSettingStop];
	tty.dwBaudRate = (DWORD)_nBaudRates[m_nSettingBaud];

	return tty;
}



TTYSTRUCT CDIPUDlg::LoadSettings()
{
	CWinApp *pApp = AfxGetApp();
	m_nSettingPort = pApp->GetProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_PORT, 2);
	m_nSettingParity = 0;
	m_nSettingBaud = pApp->GetProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_BAUD, 0);
	m_nSettingData = 1;
	m_nSettingStop = 0;
	m_nSettingFlow = 0;
	return Int2TTY();
}


void CDIPUDlg::SaveSettings()
{
	CWinApp *pApp = AfxGetApp();
	pApp->WriteProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_PORT, m_nSettingPort);
	pApp->WriteProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_PARITY, m_nSettingParity);
	pApp->WriteProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_BAUD, m_nSettingBaud);
	pApp->WriteProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_DATABITS, m_nSettingData);
	pApp->WriteProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_STOPBITS, m_nSettingStop);
	pApp->WriteProfileInt(CS_REGKEY_SETTINGS, CS_REGENTRY_FLOW, m_nSettingFlow);
}


void CDIPUDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SaveSettings();
	m_Comm.CloseConnection();
	dxl_terminate();
	CDialogEx::OnOK();
}


LRESULT CDIPUDlg::OnReceiveData(UINT_PTR WParam, LPARAM a)
{
	int i;
	int nLen = WParam;
	CString str;

	UpdateData(TRUE);
	BYTE ch;


	for (i = 0; i<nLen; i++)
	{
		ch = (int)m_Comm.abIn[i];
		str.Format("%c", ch);
		m_EditData += str;
	}
	SetDlgItemTextA(IDC_EDIT1, str);
	printf(str);
	if(str=='f')
	{
		motor_flag = 1;
	}
	printf("\n");
	UpdateData(FALSE);
	return TRUE;
}


void CDIPUDlg::OnBnClickedBtnSendSerial()
{
	int a, b, c;
	a = 1024;
	b = 1024;
	c = 1024;

	char start[] = {'a'};
	m_Comm.WriteCommBlock(start, 1);
}


void CDIPUDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	dxl_terminate();
	m_Comm.CloseConnection();
	CDialogEx::OnCancel();
}


void CDIPUDlg::OnBnClickedBtnSendMotDeg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Q1, Q2, Q3;
	int size;
	

	Q1 = GetDlgItemInt(IDC_EDIT_SEND_MOT_DEG1, 0, 1);
	Q2 = GetDlgItemInt(IDC_EDIT_SEND_MOT_DEG2, 0, 1);
	Q3 = GetDlgItemInt(IDC_EDIT_SEND_MOT_DEG3, 0, 1);
	
	//Q[0] = (-Q1 + 150)*conv;
	//Q[1] = (Q2 + 150)*conv;
	//Q[2] = 150*conv;


	Q[0] = (-Q1 + 180)*(4096. / 360);
	Q[1] = (Q2 + 180)*(4096. / 360);
	Q[2] = 180 * conv;



	servocom(id,Q);



}


void CDIPUDlg::OnBnClickedBtnSendMotPos()
{
	int px, py, pz;
	px = GetDlgItemInt(IDC_EDIT_SEND_MOT_POS1, 0, 1);
	py = GetDlgItemInt(IDC_EDIT_SEND_MOT_POS2, 0, 1);
	pz = GetDlgItemInt(IDC_EDIT_SEND_MOT_POS3, 0, 1);

	/*if (100 <= px & 320 >= px&py <= 148.5 & py >= -148.5)*/
		CalcJoint(px, py, pz);
}

void CDIPUDlg::OnBnClickedBtnMotInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int result;

	result = dxl_initialize(DEFAULT_PORTNUM, DEFAULT_BAUDNUM);

	if (result == 1)
	{
		// Succeed to open USB2Dynamixel 
		printf("Motor succeed\n");
	}

	else if (result == 0)

	{
		printf("Motor fail\n");
		// Failed to open USB2Dynamixel

	}
	dxl_write_word(1, P_GOAL_SPEED_L, 20);
	dxl_write_word(2, P_GOAL_SPEED_L, 5);
	//DIP
	dxl_write_byte(1, 26, 1);
	dxl_write_byte(1, 27, 6);
	dxl_write_byte(1, 28, 35);
	dxl_write_byte(1, 73, 0);


	//DIP
	dxl_write_byte(2, 26, 1);
	dxl_write_byte(2, 27, 10);
	dxl_write_byte(2, 28, 100);
	dxl_write_byte(2, 73, 0);
	/*dxl_write_word(BROADCAST_ID, CW_Compliance_Slope, 128);
	dxl_write_word(BROADCAST_ID, CCW_Compliance_Slope, 128);*/


}



void CDIPUDlg::servocom(int *id, double *deg)
{
	CString str;
	int CommStatus;

	// Make syncwrite packet
	//0.114rpm 8배
	//0.916rpm
	double dif_1, dif_2;
	double presentdeg1, presentdeg2;
	double load1, load2;
	presentdeg1 = dxl_read_word(1, 36);
	presentdeg2 = dxl_read_word(2, 36);
	load1 = dxl_read_word(1, 40);
	if (load1 > 1023)
		load1 = load1-1024;
	load2 = dxl_read_word(2, 40);
	if (load2 > 1023)
		load2 = load2-1024;
	printf("   load1=%.3f , load2=%.3f", load1, load2);

	//printf("p1=%f , p2=%f\n", presentdeg1, presentdeg2);
	dif_1 = ((long)(deg[0]-presentdeg1));
	dif_2 = ((long)(deg[1]-presentdeg2));
	
	printf("   dif1=%.3f , dif2=%.3f\n", dif_1, dif_2);

	int speed[3];
	if (deg[2] == 590)
	{
		speed[0] = 8;
		speed[1] = 1;
		speed[2] = 100; 
	}
	else
	{
		speed[0] = 80;
		speed[1] = 10;
		speed[2] = 100;
		
	}
	/*
	if (dif_1 > 8)
	{
		dxl_write_word(1, 27, 5);
		dxl_write_word(1, 28, 15);
	}
	else
	{	
		dxl_write_word(1, 27, 1);
		dxl_write_word(1, 28, 30);
	}
	if (dif_2 > 8)
	{
		dxl_write_word(2, 27, 5);
		dxl_write_word(2, 28, 100);
	}
	else
	{
		dxl_write_word(2, 27, 1);
		dxl_write_word(2, 28, 200);
	}*/

	/*
	if ((load1 > 50) )
	{
		dxl_write_byte(1, 27, 1);
		dxl_write_byte(1, 28, 15);
	}
	else
	{
		dxl_write_byte(1, 26, 1);
		dxl_write_byte(1, 27, 1);
		dxl_write_byte(1, 28, 30);

	}
	if ((load2 > 50))
	{
		dxl_write_byte(2, 27, 1);
		dxl_write_byte(2, 28, 100);
	}
	else
	{
		dxl_write_byte(2, 26, 100);
		dxl_write_byte(2, 27, 1);
		dxl_write_byte(2, 28, 200);
	}*/


	/*
	if ((dif_2==0||dif_1==0))
		speed[0] = 40;
	else
		speed[0] = (dif_2 / dif_1) * speed[1] * 8;
	if (speed[0] <= 0)
		speed[0] = 5;
	if (speed[0] > 40)
		speed[0] = 40;
	printf("%d\n",speed[0]);
	*/

	//deg[1] = 2048;
	dxl_set_txpacket_id(BROADCAST_ID);
	dxl_set_txpacket_instruction(INST_SYNC_WRITE);
	dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
	dxl_set_txpacket_parameter(1, 4);
	for (int i = 0; i<NUM_ACTUATOR; i++)
	{
		dxl_set_txpacket_parameter(2 + 5 * i, id[i]);
		dxl_set_txpacket_parameter(2 + 5 * i + 1, dxl_get_lowbyte(deg[i]));
		dxl_set_txpacket_parameter(2 + 5 * i + 2, dxl_get_highbyte(deg[i]));
		dxl_set_txpacket_parameter(2 + 5 * i + 3, dxl_get_lowbyte(speed[i]));
		dxl_set_txpacket_parameter(2 + 5 * i + 4, dxl_get_highbyte(speed[i]));
	}
	dxl_set_txpacket_length((4 + 1)*NUM_ACTUATOR + 4);

	dxl_txrx_packet();

	CommStatus = dxl_get_result();
	if (CommStatus == COMM_RXSUCCESS)
	{
		PrintErrorCode();
	}
	else
	{
		PrintCommStatus(CommStatus);
	}

	int count = 0;
	int accer = 50;
	while ((dxl_read_byte(id[0], P_MOVING) == 1) || (dxl_read_byte(id[1], P_MOVING) == 1) || (dxl_read_byte(id[2], P_MOVING) == 1))
	{
		/*if (count > 1000)
		{
			
			if (accer = 254)
				accer=0;
			dxl_write_byte(1, 73, accer);
			dxl_write_byte(2, 73, accer);
			accer++;
		}
			
		count++;
		printf("a");*/
		
	}
	/*printf("\n");
	printf("end\n");*/


}


void CDIPUDlg::CalcJoint(double Pos_x, double Pos_y, double Pos_z)
{

	double  Q1, Q2, Q3;
	double	r11, r12, r13;
	double	r21, r22, r23;
	double	r31, r32, r33;
	double	px, py, pz;
	double  Pose[4][4];
	double l1, l2, l3;
	double xi, b, d;
	double c2;
	l1 = 196;
	//l2 = 166.184;
	l2 = 165+60;

	r11 = Pose[0][0];		r12 = Pose[0][1]; 	r13 = Pose[0][2];	px = Pose[0][3];
	r21 = Pose[1][0];		r22 = Pose[1][1]; 	r23 = Pose[1][2];	py = Pose[1][3];
	r31 = Pose[2][0];		r32 = Pose[2][1]; 	r33 = Pose[2][2];	pz = Pose[2][3];
	Pose[3][0] = 0;			Pose[3][1] = 0;		Pose[3][2] = 0;		Pose[3][3] = 1;

	px = Pos_x + A4_x1;
	py = -Pos_y + A4_y2;
	pz = Pos_z;

	printf("px=%.3f, py=%03.3f    ", Pos_x, Pos_y);
	//-------------------------------------------------------------------------------------------------------------- 

	d = sqrt(px*px + py*py);
	c2 = (d*d - l2*l2 - l1*l1) / (2 * l1*l2);
	Q2 = atan2(sqrt(1 - c2*c2), c2);


	//------------------------------------------------------------------------------------------------------------- 	
	xi = acos((l1*l1 + d*d - l2*l2) / (2 * l1*d));
	b = atan2(py, px);

	if (Q2 > 0)
		Q1 = b - xi;
	else if (Q2 < 0)
		Q1 = b + xi;

	//--------------------------------------------------------------------------------------------------------------
	/*Q[0] = (-Q1* (180 / pi) + 150)*conv;
	Q[1] = (Q2* (180 / pi) + 150)*conv;
	Q[2] = 600;
	*/

	Q[0] = (-Q1* (180 / pi) + 180)*(4096./360);
	Q[1] = (Q2* (180 / pi) + 180)*(4096./ 360);
	if (Pos_z == 0)
	{
		if (pre_z == 1)
		{
			dxl_write_word(id[2], P_GOAL_POSITION_L, 630);
			while (dxl_read_byte(id[2], P_MOVING) == 1);
		}
		Q[2] = 630;

	}
	else if (Pos_z == 1)
	{
		Q[2] = 590;
		
	}
	pre_z = Pos_z;
	printf("Q1=%.3f, Q2=%.3f", Q[0], Q[1]);
	servocom(id,Q);

}


void CDIPUDlg::OnBnClickedBtnDrawLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double iter;
	double len;
	double px, py, pz;
	iter = 100; 
	len = 100;
	//y=x
	

	for (int j = 0; j <= iter; j++)
	{
		for (int i = 0; i <= iter; i++)
		{

			px = (len / iter)*j;
			py = (len / iter)*i;


			if(i==0)
			CalcJoint(px, py, 0);

			CalcJoint(px, py, 1);
			//printf("px=%f, py=%f\n", px, py);
		}
		
	}


}


void CDIPUDlg::OnBnClickedBtnDrawCircle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double iter;
	double r;
	double px, py, pz;
	double cen_x, cen_y;
	iter = 100;
	r = 50;
	cen_x = 210/2;
	cen_y = 297/2-50;

	for (int i = 0; i <= iter; i++)
	{
		px = r*cos(i*(2 * pi / iter)) + cen_x;
		py = r*sin(i*(2 * pi / iter)) + cen_y;
		CalcJoint(px, py, 1);
	}

	
	//for (int i = 0; i <= iter; i++)
	//{
	//	while (!i|!motor_flag);
	//	px = r*cos(i*(2 * pi / iter)) + cen_x;
	//	py = r*sin(i*(2 * pi / iter)) + cen_y;
	//	CalcJoint(px, py, 0);
	//}
}

void PrintErrorCode()

{

	// Method 2. using function

	if (dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)

		printf("Input voltage error!\n");

	if (dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)

		printf("Angle limit error!\n");

	if (dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)

		printf("Overheat error!\n");

	if (dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)

		printf("Out of range error!\n");

	if (dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)

		printf("Checksum error!\n");

	if (dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)

		printf("Overload error!\n");

	if (dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)

		printf("Instruction code error!\n");

}

void PrintCommStatus(int CommStatus)

{

	switch (CommStatus)

	{

	case COMM_TXFAIL:

		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");

		break;

	case COMM_TXERROR:

		printf("COMM_TXERROR: Incorrect instruction packet!\n");

		break;

	case COMM_RXFAIL:

		printf("COMM_RXFAIL: Failed get status packet from device!\n");

		break;

	case COMM_RXWAITING:

		printf("COMM_RXWAITING: Now recieving status packet!\n");

		break;

	case COMM_RXTIMEOUT:

		printf("COMM_RXTIMEOUT: There is no status packet!\n");

		break;

	case COMM_RXCORRUPT:

		printf("COMM_RXCORRUPT: Incorrect status packet!\n");

		break;

	default:

		printf("This is unknown error code!\n");

		break;

	}

}





void CDIPUDlg::OnBnClickedPaintOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (flag_paint == false)
	{
		flag_paint = 1;
		GetDlgItem(IDC_PAINT_ON)->SetWindowText("그림판종료");
	}
	else
	{
		flag_paint = 0;
		GetDlgItem(IDC_PAINT_ON)->SetWindowText("그림판활성화");
	}
	
	
	
}

void CDIPUDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (point.x > rec_x1 && point.x < rec_x2 && point.y>rec_y1 && point.y < rec_y2)
		flag_Ldown = true;
	printf("X=%d , Y=%d\n", point.x, point.y);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDIPUDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if (point.x > rec_x1 && point.x < rec_x2 && point.y>rec_y1 && point.y < rec_y2)
		flag_Ldown = false;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDIPUDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((flag_Ldown==1)&(flag_paint == 1))
	{
		if (point.x > rec_x1 && point.x < rec_x2 && point.y>rec_y1 && point.y < rec_y2)
		{
			CClientDC dc(this);
			dc.SetPixel(point.x, point.y, RGB(0, 0, 0));
			PainttoA4(point.x, point.y);
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


// 그림판 to A4 용지비율
void CDIPUDlg::PainttoA4(int px, int py)
{
	double temp_x, temp_y;
	double pos_x, pos_y;
	temp_y=(px - rec_x1) / ratio;
	temp_x=(py - rec_y1) / ratio;
	
	pos_x = 210-temp_x;
	pos_y = temp_y;
	printf("pos_x=%.3f ,pos_y=%.3f\n", pos_x, pos_y);
	CalcJoint(pos_x, pos_y, 1);
	
}


void CDIPUDlg::OnBnClickedPaintInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDC dc(this);
	dc.Rectangle(rec_x1, rec_y1, rec_x2, rec_y2);
}


void CDIPUDlg::OnBnClickedImgDraw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double iter;
	double len;
	double px, py, pz;
	iter = 100;
	len = 50;

	
	for (int i = 0; i < contours.size(); ) {
		cout << "\nDraw contour " << i + 1 << endl;

		for (int j = 0; j < contours[i].size(); j++) {

			if (i!=0 &j == 0) CalcJoint(px, py, 0);

			px = contours[i][j].y ;
			py = contours[i][j].x ;
			CalcJoint(px, py, 1);

			if (j == contours[i].size() - 1) CalcJoint(px, py, 0);

			printf("px=%.3f, py=%03.3f    ", px, py);

		}

		//한 획 씩 테스트하려면 수정 필요.
		i++;
	}
}

////////////////////////////////////////////////////////////////

void CDIPUDlg::OnStnClickedPic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


}


void CDIPUDlg::DisplayImage(int IDC_PICURE, Mat targetMat)
{

	IplImage* targetIplImage = new IplImage(targetMat);

	if (targetIplImage != nullptr) {
		CWnd* pWnd_ImageTraget = GetDlgItem(IDC_PIC);
		CClientDC dcImageTraget(pWnd_ImageTraget);
		RECT rcImageTraget;
		pWnd_ImageTraget->GetClientRect(&rcImageTraget);

		BITMAPINFO bitmapInfo;
		memset(&bitmapInfo, 0, sizeof(bitmapInfo));
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biWidth = targetIplImage->width;
		bitmapInfo.bmiHeader.biHeight = -targetIplImage->height;

		IplImage *tempImage = nullptr;

		if (targetIplImage->nChannels == 1)
		{
			tempImage = cvCreateImage(cvSize(targetIplImage->width, targetIplImage->height), IPL_DEPTH_8U, 3);
			cvCvtColor(targetIplImage, tempImage, CV_GRAY2BGR);
		}
		else if (targetIplImage->nChannels == 3)
		{
			tempImage = cvCloneImage(targetIplImage);
		}

		bitmapInfo.bmiHeader.biBitCount = tempImage->depth * tempImage->nChannels;

		dcImageTraget.SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(dcImageTraget.GetSafeHdc(), rcImageTraget.left, rcImageTraget.top, rcImageTraget.right, rcImageTraget.bottom,
			0, 0, tempImage->width, tempImage->height, tempImage->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&tempImage);
	}

}


void CDIPUDlg::OnBnClickedImagprocessing()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



	//Mat src = imread(FILE);

	//DisplayImage(IDC_PICTURE, src);


	//CDialogEx::OnOK();



	DIPU a;
	contours = a.ImageProcess();
	Mat targetmat = a.getTargetMat();
	DisplayImage(IDC_PIC, targetmat);
}
