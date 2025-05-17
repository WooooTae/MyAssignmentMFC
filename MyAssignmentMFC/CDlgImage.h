#pragma once
#include <vector>

// CDlgImage 대화 상자

class CDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImage)

public:
	CDlgImage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgImage();

	CImage m_image;
	int m_nRadius;
	int m_nThickness;
	std::vector<CPoint> m_Points;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgImage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

private:
	void InitImage();
	bool PossibleCircle(CPoint p1, CPoint p2, CPoint p3, double &cx, double &cy, double &nr);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
