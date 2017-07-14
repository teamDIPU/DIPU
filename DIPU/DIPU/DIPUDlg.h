
// DIPUDlg.h : 헤더 파일
//

#pragma once
#include "comm.h"
#include "afxwin.h"
#include"ImageProcess.h"
//하하

// CDIPUDlg 대화 상자
class CDIPUDlg : public CDialogEx
{
// 생성입니다.
public:
	CDIPUDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIPU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_nSettingPort;
	int m_nSettingBaud;
	int m_nSettingData;
	int m_nSettingParity;
	int m_nSettingFlow;
	CString m_EditData;
	afx_msg void OnBnClickedBtnOpenSerial();
protected:
	CComm m_Comm;
public:
	TTYSTRUCT Int2TTY();
	int m_nSettingStop;
	TTYSTRUCT LoadSettings();
	void SaveSettings();
	afx_msg void OnBnClickedOk();
	LRESULT OnReceiveData(UINT_PTR WParam, LPARAM a);
	afx_msg void OnBnClickedBtnSendSerial();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnSendMotDeg();
	afx_msg void OnBnClickedBtnSendMotPos();
	void servocom(int *id, double *deg);
	void CalcJoint(double Pos_x, double Pos_y, double Pos_z);
	afx_msg void OnBnClickedBtnDrawLine();
	afx_msg void OnBnClickedBtnDrawCircle();
	CButton m_RadioAVR;
	CButton m_RadioUSB2DY;
	afx_msg void OnBnClickedBtnMotInit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 그림판 페인트 on=1 off=0 기본값 off
	bool flag_paint=0;
	afx_msg void OnBnClickedPaintOn();
	// 사각형전역변수
	int rec_x1;
	int rec_x2;
	int rec_y1;
	int rec_y2;
	double A4_x1;
	double A4_x2;
	double A4_y1;
	double A4_y2;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	bool flag_Ldown = 0;
	// 그림판 to A4 용지비율
	void PainttoA4(int px, int py);
	afx_msg void OnBnClickedPaintInit();
	afx_msg void OnBnClickedImgDraw();
	int pre_z=0;
	afx_msg void OnStnClickedPic();

	void DisplayImage(int IDC_PICURE, Mat targetMat);

	//void DisplayImage(int IDC_PICTURE_TARGET, Mat targetMat);

	afx_msg void OnBnClickedImagprocessing();
	CStatic m_stDisplay;
};
