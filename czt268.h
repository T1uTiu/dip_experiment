// czt268.h : main header file for the CZT268 application
//

#if !defined(AFX_CZT268_H__86C25758_863B_48A3_83B4_64ED48756169__INCLUDED_)
#define AFX_CZT268_H__86C25758_863B_48A3_83B4_64ED48756169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCzt268App:
// See czt268.cpp for the implementation of this class
//

class CCzt268App : public CWinApp
{
public:
	CCzt268App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCzt268App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCzt268App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CZT268_H__86C25758_863B_48A3_83B4_64ED48756169__INCLUDED_)
