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
#include "BRepIfc.h"
#include "miniExample.h"
#include "miniExampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define     COORDINATIONVIEW    0
#define     PRESENTATIONVIEW    1
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

extern  char    * ifcSchemaName, * ifcFileName;
bool    saveIfx = false;
int     view;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniExampleDlg dialog

CMiniExampleDlg::CMiniExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniExampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniExampleDlg)
	DDX_Control(pDX, IDC_EDIT_OPENING_Y_OFFSET, m_OpeningZOffset);
	DDX_Control(pDX, IDC_EDIT_WINDOW_THICKNESS, m_WindowThickness);
	DDX_Control(pDX, IDC_EDIT_WINDOW_HEIGHT, m_WindowYOffset);
	DDX_Control(pDX, IDC_EDIT_OPENING_X_OFFSET, m_OpeningXOffset);
	DDX_Control(pDX, IDC_EDIT_OPENING_WIDTH, m_OpeningWidth);
	DDX_Control(pDX, IDC_EDIT_OPENING_HEIGHT, m_OpeningHeight);
	DDX_Control(pDX, IDC_CHECK_WINDOW_BASICREPR, m_WindowBasicRepr);
	DDX_Control(pDX, IDC_CHECK_OPENING_BASICREPR, m_OpeningBasicRepr);
	DDX_Control(pDX, IDC_CHECK_WALL_BASICREPR, m_WallBasicRepr);
	DDX_Control(pDX, IDC_CHECK_WALL, m_Wall);
	DDX_Control(pDX, IDC_CHECK_WINDOW, m_Window);
	DDX_Control(pDX, IDC_CHECK_OPENING, m_Opening);
	DDX_Control(pDX, IDC_EDIT_0_NAME, m_Wall_Name);
	DDX_Control(pDX, IDC_STATIC_0_NAME, m_Static_0_Name);
	DDX_Control(pDX, IDC_STATIC_0_0, m_Static_0_0);
	DDX_Control(pDX, IDC_STATIC_0_1, m_Static_0_1);
	DDX_Control(pDX, IDC_EDIT_1_NAME, m_Opening_Name);
	DDX_Control(pDX, IDC_STATIC_1_NAME, m_Static_1_Name);
	DDX_Control(pDX, IDC_STATIC_1_0, m_Static_1_0);
	DDX_Control(pDX, IDC_STATIC_1_1, m_Static_1_1);
	DDX_Control(pDX, IDC_EDIT_2_NAME, m_Window_Name);
	DDX_Control(pDX, IDC_STATIC_2_NAME, m_Static_2_Name);
	DDX_Control(pDX, IDC_STATIC_2_0, m_Static_2_0);
	DDX_Control(pDX, IDC_STATIC_2_1, m_Static_2_1);
	DDX_Control(pDX, IDC_STATIC_00, m_Static00);
	DDX_Control(pDX, IDC_STATIC_10, m_Static10);
	DDX_Control(pDX, IDC_STATIC_20, m_Static20);
	DDX_Control(pDX, IDC_STATIC_30, m_Static30);
	DDX_Control(pDX, IDC_STATIC_40, m_Static40);
	DDX_Control(pDX, IDC_STATIC_50, m_Static50);
	DDX_Control(pDX, IDC_STATIC_60, m_Static60);
	DDX_Control(pDX, IDC_STATIC_70, m_Static70);
	DDX_Control(pDX, IDC_STATIC_80, m_Static80);
	DDX_Control(pDX, IDC_STATIC_01, m_Static01);
	DDX_Control(pDX, IDC_STATIC_11, m_Static11);
	DDX_Control(pDX, IDC_STATIC_21, m_Static21);
	DDX_Control(pDX, IDC_STATIC_31, m_Static31);
	DDX_Control(pDX, IDC_STATIC_41, m_Static41);
	DDX_Control(pDX, IDC_STATIC_51, m_Static51);
	DDX_Control(pDX, IDC_STATIC_61, m_Static61);
	DDX_Control(pDX, IDC_STATIC_71, m_Static71);
	DDX_Control(pDX, IDC_STATIC_81, m_Static81);
	DDX_Control(pDX, IDC_EDIT_WALL_WIDTH, m_WallWidth);
	DDX_Control(pDX, IDC_EDIT_WALL_THICKNESS, m_WallThickness);
	DDX_Control(pDX, IDC_EDIT_WALL_HEIGHT, m_WallHeight);
	DDX_Control(pDX, IDC_EDIT_SCHEMA_NAME, m_SchemaName);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, m_FileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMiniExampleDlg, CDialog)
	//{{AFX_MSG_MAP(CMiniExampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_WALL, OnCheckWall)
	ON_BN_CLICKED(IDC_CHECK_OPENING, OnCheckOpening)
	ON_BN_CLICKED(IDC_CHECK_WINDOW, OnCheckWindow)
	ON_BN_CLICKED(IDC_RADIO_VIEW_0, OnRadio_View_0)
	ON_BN_CLICKED(IDC_RADIO_VIEW_1, OnRadio_View_1)
	ON_BN_CLICKED(IDC_RADIO_0_1, OnRadio_0_1)
	ON_BN_CLICKED(IDC_RADIO_0_2, OnRadio_0_2)
	ON_BN_CLICKED(IDC_RADIO_1_1, OnRadio_1_1)
	ON_BN_CLICKED(IDC_RADIO_1_2, OnRadio_1_2)
	ON_BN_CLICKED(IDC_RADIO_2_1, OnRadio_2_1)
	ON_BN_CLICKED(IDC_RADIO_2_2, OnRadio_2_2)
	ON_BN_CLICKED(IDC_IFC, OnIfc)
	ON_BN_CLICKED(IDC_IFX, OnIfx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniExampleDlg message handlers

BOOL CMiniExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
		
    m_FileName.SetWindowText(ifcFileName);
    m_SchemaName.SetWindowText(ifcSchemaName);

    m_Wall_Name.SetWindowText("Building A");
    m_Opening_Name.SetWindowText("Opening Element xyz");
    m_Window_Name.SetWindowText("Window xyz");

    m_WallThickness.SetWindowText("5000");
    m_WallWidth.SetWindowText("5000");
    m_WallHeight.SetWindowText("2300");
    m_OpeningHeight.SetWindowText("1400");
    m_OpeningWidth.SetWindowText("750");
    m_OpeningXOffset.SetWindowText("900");
    m_OpeningZOffset.SetWindowText("250");
    m_WindowThickness.SetWindowText("190");
    m_WindowYOffset.SetWindowText("12");

    OnRadio_View_0();

    OnRadio_0_2();
    OnRadio_1_2();
    OnRadio_2_2();

    m_Wall.SetCheck(1);
    m_WallBasicRepr.SetCheck(1);
    //m_Opening.SetCheck(1);
    m_OpeningBasicRepr.SetCheck(1);
    //m_Window.SetCheck(1);
    m_WindowBasicRepr.SetCheck(1);

    OnCheckWindow();
    OnCheckOpening();
    OnCheckWall();
	
	return  true;
}

void CMiniExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMiniExampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMiniExampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//void   localCreatePolygonStructureForSquare

point3DStruct  * create3DPoint(point3DStruct ** pPoint, double x, double y, double z)
{
    (* pPoint) = new point3DStruct();
    (* pPoint)->x = x;
    (* pPoint)->y = y;
    (* pPoint)->z = z;
    (* pPoint)->ifcCartesianPointInstance = 0;

    return  (* pPoint);
}

polygon2DStruct * localCreatePolygonStructureForSquare(double min_x, double min_y, double max_x, double max_y)
{
    polygon2DStruct * pPolygon;

    pPolygon = new polygon2DStruct();
    pPolygon->pPoint = new point2DStruct();
    pPolygon->pPoint->x = min_x;
    pPolygon->pPoint->y = min_y;
    pPolygon->next = new polygon2DStruct();
    pPolygon->next->pPoint = new point2DStruct();
    pPolygon->next->pPoint->x = min_x;
    pPolygon->next->pPoint->y = max_y;
    pPolygon->next->next = new polygon2DStruct();
    pPolygon->next->next->pPoint = new point2DStruct();
    pPolygon->next->next->pPoint->x = max_x;
    pPolygon->next->next->pPoint->y = max_y;
    pPolygon->next->next->next = new polygon2DStruct();
    pPolygon->next->next->next->pPoint = new point2DStruct();
    pPolygon->next->next->next->pPoint->x = max_x;
    pPolygon->next->next->next->pPoint->y = min_y;
    pPolygon->next->next->next->next = 0;

    return  pPolygon;
}

polygon3DStruct * create3DPolygonWith4Vectors(polygon3DStruct ** pPolygon, point3DStruct * pPointI, point3DStruct * pPointII, point3DStruct * pPointIII, point3DStruct * pPointIV)
{
    vector3DStruct  * pVector;

    pVector = new vector3DStruct();
    pVector->pPoint = pPointI;
    pVector->next = new vector3DStruct();
    pVector->next->pPoint = pPointII;
    pVector->next->next = new vector3DStruct();
    pVector->next->next->pPoint = pPointIII;
    pVector->next->next->next = new vector3DStruct();
    pVector->next->next->next->pPoint = pPointIV;
    pVector->next->next->next->next = NULL;

    (* pPolygon) = new polygon3DStruct();
    (* pPolygon)->pVector = pVector;
    (* pPolygon)->pOpeningVector = NULL;
    (* pPolygon)->next = NULL;

    return  (* pPolygon);
}

void    add3DPolygonOpeningWith4Vectors(polygon3DStruct * polygon, point3DStruct * pPointI, point3DStruct * pPointII, point3DStruct * pPointIII, point3DStruct * pPointIV)
{
    vector3DStruct  * pVector;

    pVector = new vector3DStruct();
    pVector->pPoint = pPointI;
    pVector->next = new vector3DStruct();
    pVector->next->pPoint = pPointII;
    pVector->next->next = new vector3DStruct();
    pVector->next->next->pPoint = pPointIII;
    pVector->next->next->next = new vector3DStruct();
    pVector->next->next->next->pPoint = pPointIV;
    pVector->next->next->next->next = NULL;

    polygon->pOpeningVector = pVector;
}

shellStruct * localCreateShellStructureForCuboid(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z)
{
    point3DStruct   * pPoints[8];
    polygon3DStruct * pPolygons[6];
    shellStruct     * pShell;

    create3DPoint(&pPoints[0], min_x, min_y, min_z);
    create3DPoint(&pPoints[1], max_x, min_y, min_z);
    create3DPoint(&pPoints[2], min_x, max_y, min_z);
    create3DPoint(&pPoints[3], max_x, max_y, min_z);
    create3DPoint(&pPoints[4], min_x, min_y, max_z);
    create3DPoint(&pPoints[5], max_x, min_y, max_z);
    create3DPoint(&pPoints[6], min_x, max_y, max_z);
    create3DPoint(&pPoints[7], max_x, max_y, max_z);

    pShell = new shellStruct();
    pShell->pPolygon = create3DPolygonWith4Vectors(&pPolygons[0], pPoints[0], pPoints[2], pPoints[3], pPoints[1]);
    pShell->next = NULL;

    pPolygons[0]->next = create3DPolygonWith4Vectors(&pPolygons[1], pPoints[4], pPoints[5], pPoints[7], pPoints[6]);
    pPolygons[1]->next = create3DPolygonWith4Vectors(&pPolygons[2], pPoints[0], pPoints[4], pPoints[6], pPoints[2]);
    pPolygons[2]->next = create3DPolygonWith4Vectors(&pPolygons[3], pPoints[2], pPoints[6], pPoints[7], pPoints[3]);
    pPolygons[3]->next = create3DPolygonWith4Vectors(&pPolygons[4], pPoints[3], pPoints[7], pPoints[5], pPoints[1]);
    pPolygons[4]->next = create3DPolygonWith4Vectors(&pPolygons[5], pPoints[1], pPoints[5], pPoints[4], pPoints[0]);

    return  pShell;
}

shellStruct * localCreateShellStructureForCuboidWithOpening(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z, double min_x_opening, double min_z_opening, double max_x_opening, double max_z_opening)
{
    point3DStruct   * pPoints[16];
    polygon3DStruct * pPolygons[10];
    shellStruct     * pShell;

    create3DPoint(&pPoints[0], min_x, min_y, min_z);
    create3DPoint(&pPoints[1], max_x, min_y, min_z);
    create3DPoint(&pPoints[2], min_x, max_y, min_z);
    create3DPoint(&pPoints[3], max_x, max_y, min_z);
    create3DPoint(&pPoints[4], min_x, min_y, max_z);
    create3DPoint(&pPoints[5], max_x, min_y, max_z);
    create3DPoint(&pPoints[6], min_x, max_y, max_z);
    create3DPoint(&pPoints[7], max_x, max_y, max_z);
    create3DPoint(&pPoints[8], min_x_opening, min_y, min_z_opening);
    create3DPoint(&pPoints[9], max_x_opening, min_y, min_z_opening);
    create3DPoint(&pPoints[10], min_x_opening, max_y, min_z_opening);
    create3DPoint(&pPoints[11], max_x_opening, max_y, min_z_opening);
    create3DPoint(&pPoints[12], min_x_opening, min_y, max_z_opening);
    create3DPoint(&pPoints[13], max_x_opening, min_y, max_z_opening);
    create3DPoint(&pPoints[14], min_x_opening, max_y, max_z_opening);
    create3DPoint(&pPoints[15], max_x_opening, max_y, max_z_opening);

    pShell = new shellStruct();
    pShell->pPolygon = create3DPolygonWith4Vectors(&pPolygons[0], pPoints[0], pPoints[2], pPoints[3], pPoints[1]);
    pShell->next = NULL;
    pPolygons[0]->next = create3DPolygonWith4Vectors(&pPolygons[1], pPoints[4], pPoints[5], pPoints[7], pPoints[6]);
    pPolygons[1]->next = create3DPolygonWith4Vectors(&pPolygons[2], pPoints[0], pPoints[4], pPoints[6], pPoints[2]);
    pPolygons[2]->next = create3DPolygonWith4Vectors(&pPolygons[3], pPoints[2], pPoints[6], pPoints[7], pPoints[3]);
    pPolygons[3]->next = create3DPolygonWith4Vectors(&pPolygons[4], pPoints[3], pPoints[7], pPoints[5], pPoints[1]);
    pPolygons[4]->next = create3DPolygonWith4Vectors(&pPolygons[5], pPoints[1], pPoints[5], pPoints[4], pPoints[0]);

    pPolygons[5]->next = create3DPolygonWith4Vectors(&pPolygons[6], pPoints[0+8], pPoints[1+8], pPoints[3+8], pPoints[2+8]);
    pPolygons[6]->next = create3DPolygonWith4Vectors(&pPolygons[7], pPoints[4+8], pPoints[6+8], pPoints[7+8], pPoints[5+8]);
    pPolygons[7]->next = create3DPolygonWith4Vectors(&pPolygons[8], pPoints[0+8], pPoints[2+8], pPoints[6+8], pPoints[4+8]);
    pPolygons[8]->next = create3DPolygonWith4Vectors(&pPolygons[9], pPoints[3+8], pPoints[1+8], pPoints[5+8], pPoints[7+8]);

    add3DPolygonOpeningWith4Vectors(pPolygons[3], pPoints[2+8], pPoints[3+8], pPoints[7+8], pPoints[6+8]);
    add3DPolygonOpeningWith4Vectors(pPolygons[5], pPoints[1+8], pPoints[0+8], pPoints[4+8], pPoints[5+8]);

    return  pShell;
}

void CMiniExampleDlg::OnOK() 
{
    polygon2DStruct * pPolygon;
    shellStruct     * pShell;
    int     ifcWallInstance, ifcOpeningElementInstance, ifcWindowInstance,
            wallHeight, wallWidth, wallThickness,
            openingHeight, openingWidth, openingXOffset, openingZOffset,
            windowThickness, windowYOffset;
    char    ifcFileName[512], ifcSchemaName[512], buffer[512],
            wallName[512], openingName[512], windowName[512];
    bool    objectsWillBeAdded;
    
    m_FileName.GetWindowText(ifcFileName, 512);
    m_SchemaName.GetWindowText(ifcSchemaName, 512);

    m_Wall_Name.GetWindowText(wallName, 512);
    m_Opening_Name.GetWindowText(openingName, 512);
    m_Window_Name.GetWindowText(windowName, 512);

    m_WallHeight.GetWindowText(buffer, 512);
    wallHeight = atoi(buffer);
    m_WallWidth.GetWindowText(buffer, 512);
    wallWidth = atoi(buffer);
    m_WallThickness.GetWindowText(buffer, 512);
    wallThickness = atoi(buffer);
    m_OpeningHeight.GetWindowText(buffer, 512);
    openingHeight = atoi(buffer);
    m_OpeningWidth.GetWindowText(buffer, 512);
    openingWidth = atoi(buffer);
    m_OpeningXOffset.GetWindowText(buffer, 512);
    openingXOffset = atoi(buffer);
    m_OpeningZOffset.GetWindowText(buffer, 512);
    openingZOffset = atoi(buffer);
    m_WindowThickness.GetWindowText(buffer, 512);
    windowThickness = atoi(buffer);
    m_WindowYOffset.GetWindowText(buffer, 512);
    windowYOffset = atoi(buffer);

    if  ( ( (m_Wall.IsWindowEnabled())  &&
            (m_Wall.GetCheck()) ) ||
          ( (m_Window.IsWindowEnabled())  &&
            (m_Window.GetCheck()) ) ) {
        objectsWillBeAdded = true;
    } else {
        objectsWillBeAdded = false;
    }

    if  (createEmptyIfcFile(ifcSchemaName, objectsWillBeAdded)) 
	{
        switch  (GetCheckedRadioButton(IDC_RADIO_0_1, IDC_RADIO_0_2)) {
            case  IDC_RADIO_0_1:
                ifcWallInstance = createIfcWallStandardCase(wallName, 0, 0, 0);

                buildRelAssociatesMaterial(ifcWallInstance, wallThickness);
                createIfcPolylineShape(0, wallThickness/2, wallWidth, wallThickness/2);

                pPolygon = localCreatePolygonStructureForSquare(0, 0, wallWidth, wallThickness);
                createIfcExtrudedPolygonShape(pPolygon, wallHeight);
                break;
            case  IDC_RADIO_0_2:
                ifcWallInstance = createIfcWall(wallName, 0, 0, 0);

                if  ( (view == PRESENTATIONVIEW)  &&  (m_Opening.IsWindowEnabled()) ) {
                    pShell = localCreateShellStructureForCuboidWithOpening(0, 0, 0, wallWidth, wallThickness, wallHeight, openingXOffset, openingZOffset, openingXOffset + openingWidth, openingZOffset + openingHeight);
                } else {
                    pShell = localCreateShellStructureForCuboid(0, 0, 0, wallWidth, wallThickness, wallHeight);
                }
                createIfcBRepShape(pShell);
                break;
            default:
                MessageBox("Unknown selected type");
                break;
		}

        if  (m_WallBasicRepr.GetCheck()) {
            createIfcBoundingBoxShape(wallWidth, wallThickness, wallHeight, "Box");
		}
        

        //
        //  Update header
        //

        char    description[512], timeStamp[512];
        time_t  t;
        struct tm   * tInfo;

        time ( &t );
        tInfo = localtime ( &t );

        if  (view == COORDINATIONVIEW) {
            memcpy(description, "ViewDefinition [CoordinationView]", sizeof("ViewDefinition [CoordinationView]"));
        } else {
            ASSERT(view == PRESENTATIONVIEW);
            memcpy(description, "ViewDefinition [PresentationView]", sizeof("ViewDefinition [PresentationView]"));
        }

        int i = 0, j = 0;
        while  (ifcFileName[i]) {
            if  (ifcFileName[i++] == '\\') {
                j = i;
            }
        }

        itoa(1900 + tInfo->tm_year, &timeStamp[0], 10);
        itoa(100 + 1 + tInfo->tm_mon, &timeStamp[4], 10);
        itoa(100 + tInfo->tm_mday, &timeStamp[7], 10);
        timeStamp[4] = '-';
        timeStamp[7] = '-';
        itoa(100 + tInfo->tm_hour, &timeStamp[10], 10);
        itoa(100 + tInfo->tm_min, &timeStamp[13], 10);
        itoa(100 + tInfo->tm_sec, &timeStamp[16], 10);
        timeStamp[10] = 'T';
        timeStamp[13] = ':';
        timeStamp[16] = ':';
        timeStamp[19] = 0;
        

        //int hour = tm.tm_hour();


        SetSPFFHeader(
                description,                        //  description
                "2;1",                              //  implementationLevel
                &ifcFileName[j],                    //  name
                &timeStamp[0],                      //  timeStamp
                "Architect",                        //  author
                "Building Designer Office",         //  organization
                "IFC Engine DLL version 1.02 beta", //  preprocessorVersion
                "IFC Engine DLL version 1.02 beta", //  originatingSystem
                "The authorising person",           //  authorization
                "IFC2X3"                            //  fileSchema
            );

        if  (saveIfx) {
            saveIfcFileAsXml(ifcFileName);
        } else {
            saveIfcFile(ifcFileName);
        }
	} else {
        MessageBox("Model could not be instantiated, probably it cannot find the schema file.");
    }
	
	CDialog::OnOK();
}

void CMiniExampleDlg::EnableProperties()
{
    if  ( (m_Wall.IsWindowEnabled())  &&
          (m_Wall.GetCheck()) ) {
        m_Static00.EnableWindow(true);
        m_Static10.EnableWindow(true);
        m_Static20.EnableWindow(true);

        m_WallWidth.EnableWindow(true);
        m_WallThickness.EnableWindow(true);
        m_WallHeight.EnableWindow(true);

        m_Static01.EnableWindow(true);
        m_Static11.EnableWindow(true);
        m_Static21.EnableWindow(true);
    } else {
        m_Static00.EnableWindow(false);
        m_Static10.EnableWindow(false);
        m_Static20.EnableWindow(false);

        m_WallWidth.EnableWindow(false);
        m_WallThickness.EnableWindow(false);
        m_WallHeight.EnableWindow(false);

        m_Static01.EnableWindow(false);
        m_Static11.EnableWindow(false);
        m_Static21.EnableWindow(false);
    }

    if  ( (m_Opening.IsWindowEnabled())  &&
          (m_Opening.GetCheck()) ) {
        m_Static30.EnableWindow(true);
        m_Static40.EnableWindow(true);
        m_Static50.EnableWindow(true);
        m_Static60.EnableWindow(true);

        m_OpeningWidth.EnableWindow(true);
        m_OpeningHeight.EnableWindow(true);
        m_OpeningXOffset.EnableWindow(true);
        m_OpeningZOffset.EnableWindow(true);

        m_Static31.EnableWindow(true);
        m_Static41.EnableWindow(true);
        m_Static51.EnableWindow(true);
        m_Static61.EnableWindow(true);
    } else {
        m_Static30.EnableWindow(false);
        m_Static40.EnableWindow(false);
        m_Static50.EnableWindow(false);
        m_Static60.EnableWindow(false);

        m_OpeningWidth.EnableWindow(false);
        m_OpeningHeight.EnableWindow(false);
        m_OpeningXOffset.EnableWindow(false);
        m_OpeningZOffset.EnableWindow(false);

        m_Static31.EnableWindow(false);
        m_Static41.EnableWindow(false);
        m_Static51.EnableWindow(false);
        m_Static61.EnableWindow(false);
    }

    if  ( (m_Window.IsWindowEnabled())  &&
          (m_Window.GetCheck()) ) {
        m_Static50.EnableWindow(true);
        m_Static60.EnableWindow(true);
        m_Static70.EnableWindow(true);
        m_Static80.EnableWindow(true);

        m_OpeningWidth.EnableWindow(true);
        m_WindowThickness.EnableWindow(true);
        m_OpeningHeight.EnableWindow(true);
        m_WindowYOffset.EnableWindow(true);

        m_Static51.EnableWindow(true);
        m_Static61.EnableWindow(true);
        m_Static71.EnableWindow(true);
        m_Static81.EnableWindow(true);
    } else {
        m_Static70.EnableWindow(false);
        m_Static80.EnableWindow(false);

        m_WindowThickness.EnableWindow(false);
        m_WindowYOffset.EnableWindow(false);

        m_Static71.EnableWindow(false);
        m_Static81.EnableWindow(false);
    }
}

void CMiniExampleDlg::OnCheckWall() 
{
    if  (m_Wall.GetCheck()) {
        m_Wall_Name.EnableWindow(true);
        m_Static_0_Name.EnableWindow(true);
        m_Static_0_0.EnableWindow(true);
        if  (view == COORDINATIONVIEW) {
            GetDlgItem(IDC_RADIO_0_1)->EnableWindow(true);     
            GetDlgItem(IDC_RADIO_0_2)->EnableWindow(true);
        } else {
            ASSERT(view == PRESENTATIONVIEW);
            GetDlgItem(IDC_RADIO_0_1)->EnableWindow(false);     
            GetDlgItem(IDC_RADIO_0_2)->EnableWindow(true);
            OnRadio_0_2();
        }

        if  (!GetCheckedRadioButton(IDC_RADIO_0_1, IDC_RADIO_0_1)) {
            m_Static_0_1.EnableWindow(true);
            m_WallBasicRepr.EnableWindow(true);
        }

        m_Opening.EnableWindow(true);
        OnCheckOpening();
	}
    EnableProperties();
}

void CMiniExampleDlg::OnCheckOpening() 
{
}

void CMiniExampleDlg::OnCheckWindow() 
{
}

void CMiniExampleDlg::OnRadio_View_0() 
{
    view = COORDINATIONVIEW;
    CheckRadioButton(IDC_RADIO_VIEW_0, IDC_RADIO_VIEW_1, IDC_RADIO_VIEW_0);

    OnCheckWall();
    OnCheckOpening();
    OnCheckWindow();
}

void CMiniExampleDlg::OnRadio_View_1() 
{
    view = PRESENTATIONVIEW;
    CheckRadioButton(IDC_RADIO_VIEW_0, IDC_RADIO_VIEW_1, IDC_RADIO_VIEW_1);

    OnCheckWall();
    OnCheckOpening();
    OnCheckWindow();
}

void CMiniExampleDlg::OnRadio_0_1() 
{
    CheckRadioButton(IDC_RADIO_0_1, IDC_RADIO_0_2, IDC_RADIO_0_1);
    m_Static_0_1.EnableWindow(true);
    m_WallBasicRepr.EnableWindow(true);
}

void CMiniExampleDlg::OnRadio_0_2() 
{    CheckRadioButton(IDC_RADIO_0_1, IDC_RADIO_0_2, IDC_RADIO_0_2);
    m_Static_0_1.EnableWindow(true);
    m_WallBasicRepr.EnableWindow(true);
}

void CMiniExampleDlg::OnRadio_1_1() 
{
    CheckRadioButton(IDC_RADIO_1_1, IDC_RADIO_1_2, IDC_RADIO_1_1);
    m_Static_1_1.EnableWindow(true);
    m_OpeningBasicRepr.EnableWindow(true);
}

void CMiniExampleDlg::OnRadio_1_2() 
{
    CheckRadioButton(IDC_RADIO_1_1, IDC_RADIO_1_2, IDC_RADIO_1_2);
    m_Static_1_1.EnableWindow(true);
    m_OpeningBasicRepr.EnableWindow(true);
}

void CMiniExampleDlg::OnRadio_2_1() 
{
    CheckRadioButton(IDC_RADIO_2_1, IDC_RADIO_2_2, IDC_RADIO_2_1);
    m_Static_2_1.EnableWindow(true);
    m_WindowBasicRepr.EnableWindow(true);
}

void CMiniExampleDlg::OnRadio_2_2() 
{
    CheckRadioButton(IDC_RADIO_2_1, IDC_RADIO_2_2, IDC_RADIO_2_2);
    m_Static_2_1.EnableWindow(true);
    m_WindowBasicRepr.EnableWindow(true);
}

bool CMiniExampleDlg::CheckWallAndWindowMeasuresNotSupported() 
{
    int     wallHeight, wallWidth,
            openingHeight, openingWidth, openingXOffset, openingZOffset;
    char    buffer[512];
    
    m_WallHeight.GetWindowText(buffer, 512);
    wallHeight = atoi(buffer);
    m_WallWidth.GetWindowText(buffer, 512);
    wallWidth = atoi(buffer);
    m_OpeningHeight.GetWindowText(buffer, 512);
    openingHeight = atoi(buffer);
    m_OpeningWidth.GetWindowText(buffer, 512);
    openingWidth = atoi(buffer);
    m_OpeningXOffset.GetWindowText(buffer, 512);
    openingXOffset = atoi(buffer);
    m_OpeningZOffset.GetWindowText(buffer, 512);
    openingZOffset = atoi(buffer);

    if  ( (openingXOffset <= 0)  ||
          (openingXOffset + openingWidth >= wallWidth)  ||
          (openingZOffset <= 0)  ||
          (openingZOffset + openingHeight >= wallHeight) ) {
        MessageBox("The opening is not placed totaly within the wall, this variant is not supported by this software in Presentation View. Please change values.", "Program incompatibility error", IDOK);
        return  true;
    }

    return  false;
}

void CMiniExampleDlg::OnIfc() 
{
    if  ( (view == PRESENTATIONVIEW)  &&  (m_Opening.IsWindowEnabled()) ) {
        if  (CheckWallAndWindowMeasuresNotSupported()) {
            return;
        }
    }

    OnOK();
}

void CMiniExampleDlg::OnIfx() 
{
    //
    //  Check if the opening is easy enough for this program to generate in Presentation View
    //
    if  ( (view == PRESENTATIONVIEW)  &&  (m_Opening.IsWindowEnabled()) ) {
        if  (CheckWallAndWindowMeasuresNotSupported()) {
            return;
        }
    }

    saveIfx = true;

    m_FileName.GetWindowText(ifcFileName, 512);
    int i = strlen(ifcFileName);

    if  ( (ifcFileName[i-4] == '.')  &&
          (ifcFileName[i-3] == 'i'  ||  ifcFileName[i-3] == 'I')  &&
          (ifcFileName[i-2] == 'f'  ||  ifcFileName[i-2] == 'F')  &&
          (ifcFileName[i-1] == 'c'  ||  ifcFileName[i-1] == 'C') ) {
        ifcFileName[i] = ifcFileName[i-1] + 'x' - 'c';
        ifcFileName[i+1] = ifcFileName[i-1] + 'm' - 'c';
        ifcFileName[i+2] = ifcFileName[i-1] + 'l' - 'c';
        ifcFileName[i+3] = 0;
        m_FileName.SetWindowText(ifcFileName);
    }

    OnOK();
}
