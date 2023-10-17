// czt268View.cpp : implementation of the CCzt268View class
//

#include "stdafx.h"
#include "czt268.h"

#include "czt268Doc.h"
#include "czt268View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCzt268View

IMPLEMENT_DYNCREATE(CCzt268View, CScrollView)

BEGIN_MESSAGE_MAP(CCzt268View, CScrollView)
	//{{AFX_MSG_MAP(CCzt268View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCzt268View construction/destruction

CCzt268View::CCzt268View()
{
	// TODO: add construction code here
	isOpenFile = 0;

}

CCzt268View::~CCzt268View()
{
}

BOOL CCzt268View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCzt268View drawing

void CCzt268View::OnDraw(CDC* pDC)
{
	CCzt268Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->isLoad){
		StretchDIBits(
			pDC->GetSafeHdc(),
			0,0,
			pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
			0,0,
			pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
			pDoc->imgData,
			pDoc->bmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
		if(pDoc->isTrueColor2Gray){
			StretchDIBits(
				pDC->GetSafeHdc(),
				0,0,
				pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
				0,0,
				pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
				pDoc->grayImgData,
				pDoc->grayBmpInfo,
				DIB_RGB_COLORS,
				SRCCOPY
			);
		}
		
	}
}

void CCzt268View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CCzt268View printing

BOOL CCzt268View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCzt268View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCzt268View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCzt268View diagnostics

#ifdef _DEBUG
void CCzt268View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCzt268View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCzt268Doc* CCzt268View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCzt268Doc)));
	return (CCzt268Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCzt268View message handlers

bool TrueColor2Gray(BITMAPINFO* orilpbmi, byte* orilpBits,BITMAPINFO* &graylpbmi, byte* &graylpBits);
void CCzt268View::OnGray() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	if(pDoc->isLoad && pDoc->bmpInfo->bmiHeader.biBitCount==24){
		pDoc->isTrueColor2Gray = TrueColor2Gray(pDoc->bmpInfo, pDoc->imgData, pDoc->grayBmpInfo, pDoc->grayImgData);
	}else{
		pDoc->isTrueColor2Gray = false;
	
	}
	Invalidate();
	
}

void CCzt268View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
bool GetRGBValue(BITMAPINFO* bmpInfo, byte* imgData,int i, int j,char* &val);
void CCzt268View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CCzt268Doc* pDoc = GetDocument();
	char xy[100];
	memset(xy, 0, 100);
	sprintf(xy, "x:%d, y:%d ", point.x, point.y);
	char* rgb = (char*)malloc(sizeof(char)*100);
	
	if(GetRGBValue(pDoc->bmpInfo, pDoc->imgData ,point.y, point.x, rgb)){
		// sprintf(xy, "(x:%d, y:%d) RGB(%d,%d,%d)", point.x, point.y, r, g, b);
		strcat(xy, rgb);
	}
	
	((CFrameWnd*)GetParent())->SetMessageText(xy);
	
	CScrollView::OnMouseMove(nFlags, point);

}
