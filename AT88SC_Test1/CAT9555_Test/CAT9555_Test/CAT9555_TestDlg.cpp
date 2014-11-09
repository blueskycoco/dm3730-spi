// CAT9555_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CAT9555_Test.h"
#include "CAT9555_TestDlg.h"
#include "AT88SCLib.h"
#include <windows.h>
#include <commctrl.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CCAT9555_TestDlg dialog
AT88SC cat;
CCAT9555_TestDlg::CCAT9555_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCAT9555_TestDlg::IDD, pParent)
	, nPort_No(0)
	, nPort_Value(0)
	, Cs_ID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCAT9555_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NO, nPort_No);
	DDV_MinMaxUInt(pDX, nPort_No, 0, 15);
	DDX_Text(pDX, IDC_EDIT_LEVEL, nPort_Value);
	DDV_MinMaxUInt(pDX, nPort_Value, 0, 1);
	//DDX_Text(pDX, IDC_EDIT_LEVEL2, nDevice);
	//DDV_MinMaxUInt(pDX, nDevice, 0, 1);
	DDX_Text(pDX, IDC_EDIT_LEVEL2, Cs_ID);
	DDV_MaxChars(pDX, Cs_ID, 20);
}

BEGIN_MESSAGE_MAP(CCAT9555_TestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCAT9555_TestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCAT9555_TestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCAT9555_TestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCAT9555_TestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CCAT9555_TestDlg message handlers

BOOL CCAT9555_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
int i;
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	unsigned char G[8];
	unsigned char pw[3];
	for(i=0;i<8;i++)
		G[i]=i;
	for(i=0;i<3;i++)
		pw[i]=i;
    cat.ConfigKey(G,pw);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CCAT9555_TestDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_CAT9555_TEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_CAT9555_TEST_DIALOG));
	}
}
#endif


void CCAT9555_TestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	unsigned char i;	
	UpdateData(TRUE);
	
	unsigned char *data=(unsigned char *)malloc(nPort_Value);
	for(i=0;i<nPort_Value;i++)
		data[i]=i;
	cat.Write(nPort_No,data,nPort_Value);
	printf("AT88SC Write: ");
	for(i=0;i<nPort_Value;i++)
		printf("%x ",data[i]);
}

void CCAT9555_TestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	
	//unsigned char pOutBuf;
	UpdateData(TRUE);
	//AT88SC cat;
	unsigned char i;
	unsigned char *data=(unsigned char *)malloc(nPort_Value);
	for(i=0;i<nPort_Value;i++)
		data[i]=0xff;
	cat.Read(nPort_No,data,nPort_Value);
	printf("AT88SC Read: ");
	for(i=0;i<nPort_Value;i++)
		printf("%x ",data[i]);
	UpdateData(FALSE);
}

void CCAT9555_TestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//AT88SC cat;
	unsigned char i;
	unsigned char *data=(unsigned char *)malloc(8);
	for(i=0;i<nPort_Value;i++)
		data[i]=0xff;
	cat.ReadID(data);
	Cs_ID.Format(L"%x %x %x %x %x %x %x %x ",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
	UpdateData(FALSE);
}

void CCAT9555_TestDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	exit(0);
}
