// czt268View.h : interface of the CCzt268View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT268VIEW_H__C2136536_9ECE_4D64_B31F_46FE352D160B__INCLUDED_)
#define AFX_CZT268VIEW_H__C2136536_9ECE_4D64_B31F_46FE352D160B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<math.h>



class CCzt268View : public CScrollView
{
protected: // create from serialization only
	CCzt268View();
	DECLARE_DYNCREATE(CCzt268View)

// Attributes
public:
	CCzt268Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCzt268View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCzt268View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCzt268View)
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistogramdlg();
	afx_msg void OnUpdateHistogramdlg(CCmdUI* pCmdUI);
	afx_msg void OnEqual();
	afx_msg void OnUpdateEqual(CCmdUI* pCmdUI);
	afx_msg void OnLineTrans();
	afx_msg void OnUpdateLineTrans(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// 自定义数据
private:
	int isOpenFile;
	int imgWidth, imgHeight, imgBitCount,biSize;
	byte *imgData;

	BITMAPINFO* bmpInfo;
};

#ifndef _DEBUG  // debug version in czt268View.cpp
inline CCzt268Doc* CCzt268View::GetDocument()
   { return (CCzt268Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CZT268VIEW_H__C2136536_9ECE_4D64_B31F_46FE352D160B__INCLUDED_)
