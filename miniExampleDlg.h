////////////////////////////////////////////////////////////////////////
//  Author:  Peter Bonsma
//  Date:    11 July 2008
//  Project: IFC Engine Series (example using DLL)
//
//  This code may be used and edited freely,
//  also for commercial projects in open and closed source software
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@tno.nl
//
//  more info for using the IFC Engine DLL in other languages: 
//  see other examples or contact:  pim.vandenhelm@tno.nl
////////////////////////////////////////////////////////////////////////

#include "resource.h"

#if !defined(AFX_MINIEXAMPLEDLG_H__E673F826_8678_41CC_92D0_C897178873FC__INCLUDED_)
#define AFX_MINIEXAMPLEDLG_H__E673F826_8678_41CC_92D0_C897178873FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMiniExampleDlg dialog

class CMiniExampleDlg : public CDialog
{
// Construction
public:
	CMiniExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMiniExampleDlg)
	enum { IDD = IDD_MINIEXAMPLE_DIALOG };
	CEdit	m_OpeningZOffset;
	CEdit	m_WindowThickness;
	CEdit	m_WindowYOffset;
	CEdit	m_OpeningXOffset;
	CEdit	m_OpeningWidth;
	CEdit	m_OpeningHeight;
	CButton	m_WindowBasicRepr;
	CButton	m_OpeningBasicRepr;
	CButton	m_WallBasicRepr;
	CButton	m_Wall;
	CButton	m_Window;
	CButton	m_Opening;
	CEdit	m_FloorThickness;
	CEdit	m_Wall_Name;
	CStatic	m_Static_0_Name;
	CStatic	m_Static_0_0;
	CStatic	m_Static_0_1;
	CEdit	m_Opening_Name;
	CStatic	m_Static_1_Name;
	CStatic	m_Static_1_0;
	CStatic	m_Static_1_1;
	CEdit	m_Window_Name;
	CStatic	m_Static_2_Name;
	CStatic	m_Static_2_0;
	CStatic	m_Static_2_1;
	CStatic	m_Static00;
	CStatic	m_Static10;
	CStatic	m_Static20;
	CStatic	m_Static30;
	CStatic	m_Static40;
	CStatic	m_Static50;
	CStatic	m_Static60;
	CStatic	m_Static70;
	CStatic	m_Static80;
	CStatic	m_Static01;
	CStatic	m_Static11;
	CStatic	m_Static21;
	CStatic	m_Static31;
	CStatic	m_Static41;
	CStatic	m_Static51;
	CStatic	m_Static61;
	CStatic	m_Static71;
	CStatic	m_Static81;
	CEdit	m_WallWidth;
	CEdit	m_WallThickness;
	CEdit	m_WallHeight;
	CEdit	m_SchemaName;
	CEdit	m_FileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniExampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMiniExampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void EnableProperties();
	afx_msg void OnCheckWall();
	afx_msg void OnCheckOpening();
	afx_msg void OnCheckWindow();
	afx_msg void OnRadio_View_0();
	afx_msg void OnRadio_View_1();
	afx_msg void OnRadio_0_0();
	afx_msg void OnRadio_0_1();
	afx_msg void OnRadio_0_2();
	afx_msg void OnRadio_1_0();
	afx_msg void OnRadio_1_1();
	afx_msg void OnRadio_1_2();
	afx_msg void OnRadio_2_0();
	afx_msg void OnRadio_2_1();
	afx_msg void OnRadio_2_2();
    afx_msg bool CheckWallAndWindowMeasuresNotSupported();
	afx_msg void OnIfc();
	afx_msg void OnIfx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIEXAMPLEDLG_H__E673F826_8678_41CC_92D0_C897178873FC__INCLUDED_)
