﻿
// MyAssignmentMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyAssignmentMFC.h"
#include "MyAssignmentMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyAssignmentMFCDlg 대화 상자



CMyAssignmentMFCDlg::CMyAssignmentMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYASSIGNMENTMFC_DIALOG, pParent)
	, m_nEditValue(0)
	, m_nEditValue2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyAssignmentMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_nEditValue);
	DDX_Text(pDX, IDC_EDIT_THICKNESS, m_nEditValue2);
}

BEGIN_MESSAGE_MAP(CMyAssignmentMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_CONFIRM, &CMyAssignmentMFCDlg::OnBnClickedBtnConfirm)
	ON_BN_CLICKED(IDC_BTN_RESET, &CMyAssignmentMFCDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMyAssignmentMFCDlg::OnBnClickedBtnRandom)
	ON_MESSAGE(WM_MY_CUSTOM_MSG,&CMyAssignmentMFCDlg::OnMyCustomMsg)
END_MESSAGE_MAP()


// CMyAssignmentMFCDlg 메시지 처리기

BOOL CMyAssignmentMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, 1280, 800);
	m_pDlgImage = new CDlgImage;
	m_pDlgImage->Create(IDD_CDlgImage, this);
	m_pDlgImage->ShowWindow(SW_SHOW);
	CRect clientRect;
	GetClientRect(&clientRect);

	const int subWidth = 1280;
	const int subHeight = 700;

	// 5) 가운데 위치 계산
	int left = (clientRect.Width() - subWidth) / 2;
	int top = (clientRect.Height() - subHeight) / 2 + 20;

	// 6) 가운데 위치로 SetWindowPos
	m_pDlgImage->SetWindowPos(
		nullptr,
		left, top,
		subWidth, subHeight,
		SWP_NOZORDER
	);

	srand(static_cast<unsigned int>(time(nullptr)));

	GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyAssignmentMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyAssignmentMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyAssignmentMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyAssignmentMFCDlg::OnBnClickedBtnConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_pDlgImage->m_nRadius = m_nEditValue;
	m_pDlgImage->m_nThickness = m_nEditValue2;
	m_pDlgImage->Invalidate();
}


void CMyAssignmentMFCDlg::OnBnClickedBtnReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pDlgImage) 
	{
		m_pDlgImage->ResetImage(); // 자식 다이얼로그 초기화
	}
	m_nEditValue = 3;    // 반지름 입력 필드 초기화
	m_nEditValue2 = 2;    // 두께 입력 필드 초기화
	UpdateData(FALSE);    // UI에 반영
}


void CMyAssignmentMFCDlg::OnBnClickedBtnRandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(FALSE); //중복 방지

	AfxBeginThread(RandomMoveThread, this);
}

UINT CMyAssignmentMFCDlg::RandomMoveThread(LPVOID pParam)
{
	CMyAssignmentMFCDlg* pDlg = (CMyAssignmentMFCDlg*)pParam;
	CDlgImage* pImgeDlg = pDlg->m_pDlgImage;

	for (int i = 0; i < 10; ++i)
	{
		pImgeDlg->RandomPoints();

		pImgeDlg->Invalidate();

		Sleep(500);
	}

	pDlg->GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(TRUE);
	return 0;
}

LRESULT CMyAssignmentMFCDlg::OnMyCustomMsg(WPARAM wParam, LPARAM lParam)
{
	BOOL bEnable = (BOOL)wParam;
	GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(bEnable);
	return 0;
}
