
// DIPUDlg.h : ��� ����
#pragma once
#include "comm.h"
#include "afxwin.h"
#include"ImageProcess.h"

#define HakDebug 0
#define Mute 0

// CDIPUDlg ��ȭ ����
class CDIPUDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDIPUDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIPU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
	// �׸��� ����Ʈ on=1 off=0 �⺻�� off
	bool flag_paint=0;
	afx_msg void OnBnClickedPaintOn();
	// �簢����������
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
	// �׸��� to A4 ��������
	void PainttoA4(int px, int py);
	afx_msg void OnBnClickedPaintInit();
	afx_msg void OnBnClickedImgDraw();
	int pre_z=0;
	afx_msg void OnStnClickedPic();
	void DisplayImage(int IDC_PICURE, Mat targetMat);
	//void DisplayImage(int IDC_PICTURE_TARGET, Mat targetMat);
	afx_msg void OnBnClickedImagprocessing();
	CStatic m_stDisplay;
	static UINT ThreadVoice(LPVOID pParam);
	LPCWSTR m_pVoicetext;
};
