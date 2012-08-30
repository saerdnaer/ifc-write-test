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


#include "stdafx.h"
#include "miniExample.h"
#include "miniExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniExampleApp

BEGIN_MESSAGE_MAP(CMiniExampleApp, CWinApp)
	//{{AFX_MSG_MAP(CMiniExampleApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

char    * ifcSchemaName, * ifcFileName;

/////////////////////////////////////////////////////////////////////////////
// CMiniExampleApp construction

CMiniExampleApp::CMiniExampleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMiniExampleApp object

CMiniExampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMiniExampleApp initialization

BOOL CMiniExampleApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CMiniExampleDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CMiniExampleApp::InitApplication() 
{
	int	i = strlen(this->m_pszHelpFilePath) - strlen(this->m_pszExeName) - 4;

	ASSERT(ifcFileName == NULL  &&  i > 0);
	ASSERT(ifcSchemaName == NULL  &&  i > 0);

	ifcFileName = new char[i+strlen("example.ifc")+1];
	memcpy(&ifcFileName[0], this->m_pszHelpFilePath, i);
	memcpy(&ifcFileName[i], "example.ifc", strlen("example.ifc")+1);

    ifcSchemaName = new char[i+strlen("IFC2X3_Final.exp")+1];
	memcpy(&ifcSchemaName[0], this->m_pszHelpFilePath, i);
	memcpy(&ifcSchemaName[i], "IFC2X3_Final.exp", strlen("IFC2X3_Final.exp")+1);
    
	return	CWinApp::InitApplication();
}
