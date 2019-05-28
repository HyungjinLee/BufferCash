// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simple.h"
#include "TestDlg.h"
#include "afxdialogex.h"

// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	
}

CTestDlg::CTestDlg(CChildView* child /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, child)
{
	mychildview = child;
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CTestDlg message handlers


void CTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	mychildview->app->scenarios(1);
}


void CTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	mychildview->app->scenarios(2);
}


void CTestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	mychildview->app->scenarios(3);
}


void CTestDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	mychildview->app->scenarios(4);
}


void CTestDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	mychildview->app->scenarios(5);
}
