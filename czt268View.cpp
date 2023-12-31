// czt268View.cpp : implementation of the CCzt268View class
//

#include "stdafx.h"
#include "czt268.h"

#include "czt268Doc.h"
#include "czt268View.h"
#include "HistogramDlg.h"
#include "LinearTrans.h"

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
	ON_COMMAND(ID_HISTOGRAMDLG, OnHistogramdlg)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAMDLG, OnUpdateHistogramdlg)
	ON_COMMAND(ID_EQUAL, OnEqual)
	ON_UPDATE_COMMAND_UI(ID_EQUAL, OnUpdateEqual)
	ON_COMMAND(ID_LINE_TRANS, OnLineTrans)
	ON_UPDATE_COMMAND_UI(ID_LINE_TRANS, OnUpdateLineTrans)
	ON_COMMAND(ID_FT, OnFt)
	ON_UPDATE_COMMAND_UI(ID_FT, OnUpdateFt)
	ON_COMMAND(ID_IFT, OnIft)
	ON_UPDATE_COMMAND_UI(ID_IFT, OnUpdateIft)
	ON_COMMAND(ID_MEANFILTER, OnMeanfilter)
	ON_UPDATE_COMMAND_UI(ID_MEANFILTER, OnUpdateMeanfilter)
	ON_COMMAND(ID_LAPLACIAN, OnLaplacian)
	ON_UPDATE_COMMAND_UI(ID_LAPLACIAN, OnUpdateLaplacian)
	ON_COMMAND(ID_MIDFILTER, OnMidfilter)
	ON_UPDATE_COMMAND_UI(ID_MIDFILTER, OnUpdateMidfilter)
	ON_COMMAND(ID_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(ID_FFT, OnUpdateFft)
	ON_COMMAND(ID_IFFT, OnIfft)
	ON_UPDATE_COMMAND_UI(ID_IFFT, OnUpdateIfft)
	ON_COMMAND(ID_GRADIENT_SHARPEN, OnGradientSharpen)
	ON_UPDATE_COMMAND_UI(ID_GRADIENT_SHARPEN, OnUpdateGradientSharpen)
	ON_COMMAND(ID_IDEALLOWPASS, OnIdeallowpass)
	ON_UPDATE_COMMAND_UI(ID_IDEALLOWPASS, OnUpdateIdeallowpass)
	ON_COMMAND(ID_BUTTERWORTHLOWPASS, OnButterworthlowpass)
	ON_UPDATE_COMMAND_UI(ID_BUTTERWORTHLOWPASS, OnUpdateButterworthlowpass)
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
	}
	if(pDoc->isFT){
		StretchDIBits(
			pDC->GetSafeHdc(),
			pDoc->bmpInfo->bmiHeader.biWidth,0,
			pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
			0,0,
			pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
			pDoc->ftSpectrum,
			pDoc->bmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}
	if(pDoc->isIFT){
		StretchDIBits(
			pDC->GetSafeHdc(),
			0,pDoc->bmpInfo->bmiHeader.biHeight,
			pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
			0,0,
			pDoc->bmpInfo->bmiHeader.biWidth, pDoc->bmpInfo->bmiHeader.biHeight,
			pDoc->iftImgData,
			pDoc->bmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}
}

void CCzt268View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 1200;
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

bool Gray(BITMAPINFO* orilpbmi, byte* orilpBits,BITMAPINFO* &graylpbmi, byte* &graylpBits);
void CCzt268View::OnGray() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	BITMAPINFO* grayBmpInfo;
	byte* grayImgData;
	if(pDoc->isLoad && Gray(pDoc->bmpInfo, pDoc->imgData, grayBmpInfo, grayImgData)){
		pDoc->bmpInfo = grayBmpInfo;
		pDoc->imgData = grayImgData;
	}
	
	Invalidate();
	
}

void CCzt268View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad);
	
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
		strcat(xy, rgb);
	}
	
	((CFrameWnd*)GetParent())->SetMessageText(xy);
	
	CScrollView::OnMouseMove(nFlags, point);

}

void CCzt268View::OnHistogramdlg() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	CHistogramDlg dlg(NULL, pDoc->bmpInfo, pDoc->imgData);
	dlg.DoModal();

	
}

void CCzt268View::OnUpdateHistogramdlg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad);

	
}
bool GetGrayHistogram(BITMAPINFO* bmpInfo, byte* imgData,int* &grayHistogram);
bool EqualHistogram(BITMAPINFO* &bmpInfo, byte* &imgData, int* grayHistogram);
bool LinearPointCalculate(BITMAPINFO* &bmpInfo, byte* &imgData, int a1, int a0);
void CCzt268View::OnEqual() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	
	int* grayHistogram;
	if(GetGrayHistogram(pDoc->bmpInfo, pDoc->imgData, grayHistogram)){
		EqualHistogram(pDoc->bmpInfo, pDoc->imgData, grayHistogram);
	}
	Invalidate();
	
}

void CCzt268View::OnUpdateEqual(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad);
	
}

void CCzt268View::OnLineTrans() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	int a0 = 1, a1 = 0;
	LinearTrans dlg(&a0, &a1);
	dlg.DoModal();
	LinearPointCalculate(pDoc->bmpInfo, pDoc->imgData, a0, a1);
	Invalidate();
}

void CCzt268View::OnUpdateLineTrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad);
	
}
bool Fourier(BITMAPINFO* bmpInfo, byte* imgData, complex<double>* &res);
bool GetFTSpectrum(BITMAPINFO* bmpInfo, complex<double>* ftData, byte*& res);
void CCzt268View::OnFt() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	
	Fourier(pDoc->bmpInfo, pDoc->imgData, pDoc->ftData);
	pDoc->isFT = GetFTSpectrum(pDoc->bmpInfo, pDoc->ftData, pDoc->ftSpectrum);
	pDoc->isIFT = false;
	Invalidate();
	
}

void CCzt268View::OnUpdateFt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad && pDoc->isGrayImage());
	
}
bool IFourier(BITMAPINFO* bmpInfo, complex<double>* ftData, byte* &res);

void CCzt268View::OnIft() 
{
	// TODO: Add your command handler code here
	
	CCzt268Doc* pDoc = GetDocument();
	if(pDoc->isIFT = IFourier(pDoc->bmpInfo, pDoc->ftData, pDoc->iftImgData)){
		Invalidate();
	}
	
}

void CCzt268View::OnUpdateIft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isFT && ! pDoc->isIFT);
	
}
bool Conv2D(BITMAPINFO* bmpInfo, byte*& imgData, int* kernel, int kernelSize);
void CCzt268View::OnMeanfilter() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	int kernel[] = {1,1,1,1,1,1,1,1,1};
	if(Conv2D(pDoc->bmpInfo, pDoc->imgData, kernel, 3)){
		Invalidate();
	}
}

void CCzt268View::OnUpdateMeanfilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad && pDoc->isGrayImage());
	
}

void CCzt268View::OnLaplacian() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	int kernel[] = {-1,-1,-1,-1,9,-1,-1,-1,-1};
	if(Conv2D(pDoc->bmpInfo, pDoc->imgData, kernel, 3)){
		Invalidate();
	}
}

void CCzt268View::OnUpdateLaplacian(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad && pDoc->isGrayImage());
	
}

bool MidPool2D(BITMAPINFO* bmpInfo, byte*& imgData, int kernelSize);
void CCzt268View::OnMidfilter() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	if(MidPool2D(pDoc->bmpInfo, pDoc->imgData, 3)){
		Invalidate();
	}
}

void CCzt268View::OnUpdateMidfilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad && pDoc->isGrayImage());
}

bool FFourier(BITMAPINFO* bmpInfo, byte* imgData, complex<double>* &res);
void CCzt268View::OnFft() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	FFourier(pDoc->bmpInfo, pDoc->imgData, pDoc->ftData);
	pDoc->isFT = GetFTSpectrum(pDoc->bmpInfo, pDoc->ftData, pDoc->ftSpectrum);
	pDoc->isIFT = false;
	Invalidate();
}

void CCzt268View::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad && pDoc->isGrayImage());
}
bool IFFTourier(BITMAPINFO* bmpInfo, complex<double>* ftData, byte* &res);
void CCzt268View::OnIfft() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	if(pDoc->isIFT = IFFTourier(pDoc->bmpInfo, pDoc->ftData, pDoc->iftImgData)){
		Invalidate();
	}
}

void CCzt268View::OnUpdateIfft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isFT && ! pDoc->isIFT);
	
}

bool GradientSharpen(BITMAPINFO* bmpInfo, byte*& imgData);
void CCzt268View::OnGradientSharpen() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	if(GradientSharpen(pDoc->bmpInfo, pDoc->imgData)){
		Invalidate();
	}
}

void CCzt268View::OnUpdateGradientSharpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isLoad && pDoc->isGrayImage());
}
bool IdealFilter(BITMAPINFO* bmpInfo, complex<double>* &ftData, int d0, int method=0);
void CCzt268View::OnIdeallowpass() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	if(IdealFilter(pDoc->bmpInfo, pDoc->ftData, 50)){
		pDoc->isFT = GetFTSpectrum(pDoc->bmpInfo, pDoc->ftData, pDoc->ftSpectrum);
		pDoc->isIFT = false;
		this->OnIfft();
		Invalidate();
	}
}

void CCzt268View::OnUpdateIdeallowpass(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isFT);
}
bool ButterworthFilter(BITMAPINFO* bmpInfo, complex<double>* &ftData, int d0, int n=2, int method=0);
void CCzt268View::OnButterworthlowpass() 
{
	// TODO: Add your command handler code here
	CCzt268Doc* pDoc = GetDocument();
	if(ButterworthFilter(pDoc->bmpInfo, pDoc->ftData, 50,2,1)){
		pDoc->isFT = GetFTSpectrum(pDoc->bmpInfo, pDoc->ftData, pDoc->ftSpectrum);
		pDoc->isIFT = false;
		this->OnIfft();
		// Invalidate();
	}
}

void CCzt268View::OnUpdateButterworthlowpass(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CCzt268Doc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isFT);
}
