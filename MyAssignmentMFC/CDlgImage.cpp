// CDlgImage.cpp: 구현 파일
//

#include "pch.h"
#include "MyAssignmentMFC.h"
#include "CDlgImage.h"
#include "afxdialogex.h"


// CDlgImage 대화 상자

IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgImage, pParent)
{

}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgImage 메시지 처리기


BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, 640, 480);

	InitImage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	Gdiplus::Graphics g(dc.GetSafeHdc());
	if (m_image)
	{
		m_image.Draw(dc, 0, 0);

		Gdiplus::SolidBrush dotBrush(Gdiplus::Color::Black);
		float x = m_image.GetWidth() - 150.f;
		float y = 30.f;
		for (int i = 0; i < m_Points.size(); i++)
		{
			REAL r = static_cast<REAL>(m_nRadius);
			g.FillEllipse(&dotBrush,m_Points[i].x - r,m_Points[i].y - r,2 * r, 2 * r);

			WCHAR buf[32];
			swprintf(buf, 32, L"(%d: %d, %d)", i + 1,m_Points[i].x, m_Points[i].y);

			Gdiplus::PointF ptText((REAL)x, (REAL)y);
			Gdiplus::Font       font(L"Arial", 12);
			g.DrawString(buf,-1,&font,Gdiplus::PointF(x,y + i * 30.0f),&dotBrush);
		}

		if (m_Points.size() == 3)
		{
			double cx, cy, nr;
			if (PossibleCircle(m_Points[0], m_Points[1], m_Points[2], cx, cy, nr))
			{
				Gdiplus::Pen pen(Gdiplus::Color::Black, m_nThickness);
				g.DrawEllipse(&pen, (Gdiplus::REAL)(cx - nr), (Gdiplus::REAL)(cy - nr),
					(Gdiplus::REAL)(2 * nr), (Gdiplus::REAL)(2 * nr));
			}
		}

		//Gdiplus::Font       font(L"Arial", 12);
		//Gdiplus::PointF     pt(10, 10);   // X=10, Y=10 에서 시작

		//wchar_t buf[64];
		//for (size_t i = 0; i < m_Points.size(); ++i)
		//{
		//	swprintf(buf, _countof(buf), L"P%zu: (%d, %d)",
		//		i + 1, m_Points[i].x, m_Points[i].y);

		//	// 화면 상단부터 아래로 한 줄씩 내려가며
		//	g.DrawString(buf, -1, &font, pt, &dotBrush);
		//	pt.Y += 20;   // 다음 텍스트는 20px 아래에
		//}
	}
}

void CDlgImage::InitImage()
{
	int nWidth = 1280;
	int nHeight = 700;
	int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char *fm = (unsigned char*)m_image.GetBits();

	memset(fm, 0xff, nWidth * nHeight);
}

bool CDlgImage::PossibleCircle(CPoint p1, CPoint p2, CPoint p3, double & cx, double & cy, double & nr)
{
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	// 세 점이 일직선인지 확인 (행렬식 이용)
	double det = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	if (det == 0) return false;

	//중심 계산
	double A = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
	double B = (x1*x1 + y1 * y1)*(y3 - y2) + (x2*x2 + y2 * y2)*(y1 - y3) + (x3*x3 + y3 * y3)*(y2 - y1);
	double C = (x1*x1 + y1 * y1)*(x2 - x3) + (x2*x2 + y2 * y2)*(x3 - x1) + (x3*x3 + y3 * y3)*(x1 - x2);

	cx = -B / (2 * A);
	cy = -C / (2 * A);
	nr = sqrt((cx - x1)*(cx - x1) + (cy - y1)*(cy - y1));

	return true;
}


void CDlgImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);

	if (m_Points.size() < 3)
	{
		m_Points.push_back(point);
		Invalidate();
	}

	for (int i = 0; i < m_Points.size(); i++)
	{
		// 클릭한 지점과 두점 사이의 거리
		double distance = sqrt((point.x - m_Points[i].x) * (point.x - m_Points[i].x) + 
			(point.y - m_Points[i].y) * (point.y - m_Points[i].y));

		if (distance <= (double)m_nRadius)
		{
			m_bIsDrag = true;
			m_nDragIndex = i;
			SetCapture();
			break;
		}
	}
}


void CDlgImage::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bIsDrag && m_nDragIndex >= 0)
	{
		m_Points[m_nDragIndex] = point;

		Invalidate();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bIsDrag)
	{
		m_bIsDrag = false;
		m_nDragIndex = -1;
		ReleaseCapture();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}
