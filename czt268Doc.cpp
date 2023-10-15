// czt268Doc.cpp : implementation of the CCzt268Doc class
//

#include "stdafx.h"
#include "czt268.h"


#include "czt268Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCzt268Doc

IMPLEMENT_DYNCREATE(CCzt268Doc, CDocument)

BEGIN_MESSAGE_MAP(CCzt268Doc, CDocument)
	//{{AFX_MSG_MAP(CCzt268Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCzt268Doc construction/destruction

CCzt268Doc::CCzt268Doc()
{
	// TODO: add one-time construction code here
	isLoad = false;
	isTrueColor2Gray = false;

	bmpInfo=NULL;
	imgData=NULL;

	grayBmpInfo=NULL;
	grayImgData=NULL;


}

CCzt268Doc::~CCzt268Doc()
{
}

BOOL CCzt268Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCzt268Doc serialization

void CCzt268Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCzt268Doc diagnostics

#ifdef _DEBUG
void CCzt268Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCzt268Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCzt268Doc commands

bool LoadBmpFile(char* path, BITMAPINFO*& bmpInfo, byte*& imgData);
bool TrueColor2Gray(BITMAPINFO* orilpbmi, byte* orilpBits,BITMAPINFO* &graylpbmi, byte* &graylpBits);
bool Mirror(BITMAPINFO* bmpInfo, byte* imgData, int direction, byte* &newImgData);
BOOL CCzt268Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	isLoad = LoadBmpFile((char*)lpszPathName,bmpInfo,imgData);
	isTrueColor2Gray = false;
	return TRUE;
}
