// czt268Doc.h : interface of the CCzt268Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT268DOC_H__03C0C845_CACC_4F1D_9C60_86ECE9CBFBA6__INCLUDED_)
#define AFX_CZT268DOC_H__03C0C845_CACC_4F1D_9C60_86ECE9CBFBA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCzt268Doc : public CDocument
{
protected: // create from serialization only
	CCzt268Doc();
	DECLARE_DYNCREATE(CCzt268Doc)

// Attributes
public:
	BITMAPINFO* bmpInfo;
	byte* imgData;

	BITMAPINFO* grayBmpInfo;
	byte* grayImgData;

	byte* mirrorImgData;

	bool isLoad;
	bool isTrueColor2Gray;
	bool isMirror;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCzt268Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCzt268Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCzt268Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CZT268DOC_H__03C0C845_CACC_4F1D_9C60_86ECE9CBFBA6__INCLUDED_)
