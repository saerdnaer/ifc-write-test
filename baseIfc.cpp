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
#include "baseIfc.h"
#include "baseIfcObject.h"
#include "extrudedPolygonIFC.h"
#include "miniExampleDlg.h"

#include "BRepIfc.h"

int     model = 0,
        timeStamp;

int     * aggrRelatedElements;

int     ifcApplicationInstance,
        ifcBuildingInstance,
        ifcBuildingInstancePlacement,
        ifcBuildingStoreyInstance,
        ifcBuildingStoreyInstancePlacement,
        ifcBuildOwnerHistoryInstance,
        ifcConversionBasedUnitInstance,
        ifcDimensionalExponentsInstance,
        ifcGeometricRepresentationContextInstance,
        ifcOrganizationInstance,
        ifcPersonAndOrganizationInstance,
        ifcPersonInstance,
        ifcProjectInstance,
        ifcSiteInstance,
        ifcSiteInstancePlacement,
        ifcUnitAssignmentInstance;


bool    createEmptyIfcFile(char * ifcSchemaName, bool objectsWillBeAdded)
{
    transformationMatrixStruct    matrix;
	
    model = sdaiCreateModelBN(1, NULL, ifcSchemaName);

    if	(model) {
        identityMatrix(&matrix);

        ifcApplicationInstance = 0;
        ifcBuildOwnerHistoryInstance = 0;
        ifcConversionBasedUnitInstance = 0;
        ifcDimensionalExponentsInstance = 0;
        ifcGeometricRepresentationContextInstance = 0;
        ifcOrganizationInstance = 0;
        ifcPersonAndOrganizationInstance = 0;
        ifcPersonInstance = 0;
        ifcUnitAssignmentInstance = 0;

		//
		//	Build standard IFC structures
		//

		//
		//		Build standard IFC objects
		//
		ifcProjectInstance = getProjectInstance();
		ifcSiteInstance = buildSiteInstance(&matrix, NULL, &ifcSiteInstancePlacement);
		ifcBuildingInstance = buildBuildingInstance(&matrix, ifcSiteInstancePlacement, &ifcBuildingInstancePlacement);
		ifcBuildingStoreyInstance = buildBuildingStoreyInstance(&matrix, ifcBuildingInstancePlacement, &ifcBuildingStoreyInstancePlacement);

		//
		//		Build relations
		//
		buildRelAggregatesInstance("BuildingContainer", "BuildingContainer for BuildigStories", ifcBuildingInstance, ifcBuildingStoreyInstance);
		buildRelAggregatesInstance("SiteContainer", "SiteContainer For Buildings", ifcSiteInstance, ifcBuildingInstance);
		buildRelAggregatesInstance("ProjectContainer", "ProjectContainer for Sites", ifcProjectInstance, ifcSiteInstance);

        if  (objectsWillBeAdded) {
		    buildRelContainedInSpatialStructureInstance("BuildingStoreyContainer", "BuildingStoreyContainer for Building Elements", ifcBuildingStoreyInstance, &aggrRelatedElements);
        }

        return  true;
    } else {
        return  false;
    }
}

bool    saveIfcFile(char * ifcFileName)
{
    if	(model) {
        //
        //  Save the created configuration
        //
        sdaiSaveModelBN(model, ifcFileName);

        return  true;
    } else {
        return  false;
    }
}

bool    saveIfcFileAsXml(char * ifcFileName)
{
    if	(model) {
        //
        //  Save the created configuration
        //
        sdaiSaveModelAsXmlBN(model, ifcFileName);

        return  true;
    } else {
        return  false;
    }
}

void	identityMatrix(transformationMatrixStruct * pMatrix)
{
	pMatrix->_11 = 1;
	pMatrix->_12 = 0;
	pMatrix->_13 = 0;
	pMatrix->_21 = 0;
	pMatrix->_22 = 1;
	pMatrix->_23 = 0;
	pMatrix->_31 = 0;
	pMatrix->_32 = 0;
	pMatrix->_33 = 1;
	pMatrix->_41 = 0;
	pMatrix->_42 = 0;
	pMatrix->_43 = 0;
}

void	identityPoint(point3DStruct * pPoint)
{
	pPoint->x = 0;
	pPoint->y = 0;
	pPoint->z = 0;
}

int		* getTimeStamp()
{
	timeStamp = time(0);

	return	&timeStamp;
}


//
//
//	GUID
//
//


char * CreateCompressedGuidString()
{
	char	* buf = (char *) malloc(23);
	GUID	guid = GUID_NULL;

	//
	// Call to the function from Microsoft
	//
	CoCreateGuid(&guid);

	if (memcmp(&GUID_NULL, &guid, sizeof (GUID)) == 0) {
		return NULL;
	}

	return getString64FromGuid (&guid, buf);
}

char * getString64FromGuid(const GUID *pGuid, char * buf )
{
    unsigned long   num[6];
    char            str[6][5];
    int             i, n;

    //
    // Creation of six 32 Bit integers from the components of the GUID structure
    //
    num[0] = (unsigned long) (pGuid->Data1 / 16777216);                                                 //    16. byte  (pGuid->Data1 / 16777216) is the same as (pGuid->Data1 >> 24)
    num[1] = (unsigned long) (pGuid->Data1 % 16777216);                                                 // 15-13. bytes (pGuid->Data1 % 16777216) is the same as (pGuid->Data1 & 0xFFFFFF)
    num[2] = (unsigned long) (pGuid->Data2 * 256 + pGuid->Data3 / 256);                                 // 12-10. bytes
    num[3] = (unsigned long) ((pGuid->Data3 % 256) * 65536 + pGuid->Data4[0] * 256 + pGuid->Data4[1]);  // 09-07. bytes
    num[4] = (unsigned long) (pGuid->Data4[2] * 65536 + pGuid->Data4[3] * 256 + pGuid->Data4[4]);       // 06-04. bytes
    num[5] = (unsigned long) (pGuid->Data4[5] * 65536 + pGuid->Data4[6] * 256 + pGuid->Data4[7]);       // 03-01. bytes
    //
    // Conversion of the numbers into a system using a base of 64
    //
    buf[0]='\0';
    n = 3;
    for (i = 0; i < 6; i++) {
        if (!cv_to_64(num[i], str[i], n)) {
            return NULL;
        }
        strcat(buf, str[i]);
        n = 5;
    }
    return buf;
}

BOOL cv_to_64(const unsigned long number, char *code, int len)
{
    unsigned long   act;
    int             iDigit, nDigits;
    char            result[5];

    if (len > 5)
        return FALSE;

    act = number;
    nDigits = len - 1;

    for (iDigit = 0; iDigit < nDigits; iDigit++) {
        result[nDigits - iDigit - 1] = cConversionTable64[(int) (act % 64)];
        act /= 64;
    }
    result[len - 1] = '\0';

    if (act != 0)
        return FALSE;

    strcpy(code, result);
    return TRUE;
}


//
//
//		Application, Organization, Person (OwnerHistory, PersonAndOrganization)
//
//


int		getApplicationInstance()
{
	if	(!ifcApplicationInstance) {
		ifcApplicationInstance = sdaiCreateInstanceBN(model, "IFCAPPLICATION");

		sdaiPutAttrBN(ifcApplicationInstance, "ApplicationDeveloper", sdaiINSTANCE, (void*) getOrganizationInstance());
		sdaiPutAttrBN(ifcApplicationInstance, "Version", sdaiSTRING, "0.10");
		sdaiPutAttrBN(ifcApplicationInstance, "ApplicationFullName", sdaiSTRING, "Test Application");
		sdaiPutAttrBN(ifcApplicationInstance, "ApplicationIdentifier", sdaiSTRING, "TA 1001");
	}

	return	ifcApplicationInstance;
}

int		getOrganizationInstance()
{
	if	(!ifcOrganizationInstance) {
		ifcOrganizationInstance = sdaiCreateInstanceBN(model, "IFCORGANIZATION");

		sdaiPutAttrBN(ifcOrganizationInstance, "Name", sdaiSTRING, "TNO");
		sdaiPutAttrBN(ifcOrganizationInstance, "Description", sdaiSTRING, "TNO Building Innovation");
	}

	return	ifcOrganizationInstance;
}

int		getOwnerHistoryInstance()
{
	if	(!ifcBuildOwnerHistoryInstance) {
		ifcBuildOwnerHistoryInstance = sdaiCreateInstanceBN(model, "IFCOWNERHISTORY");

		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance, "OwningUser", sdaiINSTANCE, (void*) getPersonAndOrganizationInstance());
		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance, "OwningApplication", sdaiINSTANCE, (void*) getApplicationInstance());
		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance, "ChangeAction", sdaiENUM, "ADDED");
		sdaiPutAttrBN(ifcBuildOwnerHistoryInstance, "CreationDate", sdaiINTEGER, (void*) getTimeStamp());
	}

	return	ifcBuildOwnerHistoryInstance;
}

int		getPersonAndOrganizationInstance()
{
	if	(!ifcPersonAndOrganizationInstance) {
		ifcPersonAndOrganizationInstance = sdaiCreateInstanceBN(model, "IFCPERSONANDORGANIZATION");

		sdaiPutAttrBN(ifcPersonAndOrganizationInstance, "ThePerson", sdaiINSTANCE, (void*) getPersonInstance());
		sdaiPutAttrBN(ifcPersonAndOrganizationInstance, "TheOrganization", sdaiINSTANCE, (void*) getOrganizationInstance());
	}

	return	ifcPersonAndOrganizationInstance;
}

int		getPersonInstance()
{
	if	(!ifcPersonInstance) {
		ifcPersonInstance = sdaiCreateInstanceBN(model, "IFCPERSON");

		sdaiPutAttrBN(ifcPersonInstance, "GivenName", sdaiSTRING, "Peter");
		sdaiPutAttrBN(ifcPersonInstance, "Id", sdaiSTRING, "ID001");
		sdaiPutAttrBN(ifcPersonInstance, "FamilyName", sdaiSTRING, "Bonsma");
	}

	return	ifcPersonInstance;
}


//
//
//		CartesianPoint, Direction, LocalPlacement (Axis2Placement)
//
//


int		buildAxis2Placement3DInstance(transformationMatrixStruct * pMatrix)
{
	int		ifcAxis2Placement3DInstance;

	ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT3D");

	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Location", sdaiINSTANCE, (void*) buildCartesianPointInstance((point3DStruct *) &pMatrix->_41));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Axis", sdaiINSTANCE, (void*) buildDirectionInstance((point3DStruct *) &pMatrix->_31));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "RefDirection", sdaiINSTANCE, (void*) buildDirectionInstance((point3DStruct *) &pMatrix->_11));

	return	ifcAxis2Placement3DInstance;
}

int		buildCartesianPointInstance(point3DStruct * pPoint)
{
	int		ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &pPoint->x);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &pPoint->y);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &pPoint->z);

	return	ifcCartesianPointInstance;
}

int		buildDirectionInstance(point3DStruct * pPoint)
{
	int		ifcDirectionInstance, * aggrDirectionRatios;
	double	_null = 0, _one = 1;

	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");
	sdaiAppend((int) aggrDirectionRatios, sdaiREAL, &pPoint->x);
	sdaiAppend((int) aggrDirectionRatios, sdaiREAL, &pPoint->y);
	sdaiAppend((int) aggrDirectionRatios, sdaiREAL, &pPoint->z);

	return	ifcDirectionInstance;
}

int		buildLocalPlacementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo)
{
	int		ifcLocalPlacementInstance;

	ifcLocalPlacementInstance = sdaiCreateInstanceBN(model, "IFCLOCALPLACEMENT");

	if	(ifcPlacementRelativeTo) {
		sdaiPutAttrBN(ifcLocalPlacementInstance, "PlacementRelTo", sdaiINSTANCE, (void*) ifcPlacementRelativeTo);
	}
	sdaiPutAttrBN(ifcLocalPlacementInstance, "RelativePlacement", sdaiINSTANCE, (void*) buildAxis2Placement3DInstance(pMatrix));

	return	ifcLocalPlacementInstance;
}


//
//
//		ConversionBasedUnit, DimensionalExponents, MeasureWithUnit, SIUnit, UnitAssignment
//
//


int		getConversionBasedUnitInstance()
{
	if	(!ifcConversionBasedUnitInstance) {
		ifcConversionBasedUnitInstance = sdaiCreateInstanceBN(model, "IFCCONVERSIONBASEDUNIT");

		sdaiPutAttrBN(ifcConversionBasedUnitInstance, "Dimensions", sdaiINSTANCE, (void*) getDimensionalExponentsInstance());
		sdaiPutAttrBN(ifcConversionBasedUnitInstance, "UnitType", sdaiENUM, "PLANEANGLEUNIT");
		sdaiPutAttrBN(ifcConversionBasedUnitInstance, "Name", sdaiSTRING, "DEGREE");
		sdaiPutAttrBN(ifcConversionBasedUnitInstance, "ConversionFactor", sdaiINSTANCE, (void*) buildMeasureWithUnitInstance());
	}

	return	ifcConversionBasedUnitInstance;
}

int		getDimensionalExponentsInstance()
{
	int		LengthExponent = 0,
			MassExponent = 0,
			TimeExponent = 0,
			ElectricCurrentExponent = 0,
			ThermodynamicTemperatureExponent = 0,
			AmountOfSubstanceExponent = 0,
			LuminousIntensityExponent = 0;

	if	(!ifcDimensionalExponentsInstance) {
		ifcDimensionalExponentsInstance = sdaiCreateInstanceBN(model, "IFCDIMENSIONALEXPONENTS");

		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "LengthExponent", sdaiINTEGER, &LengthExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "MassExponent", sdaiINTEGER, &MassExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "TimeExponent", sdaiINTEGER, &TimeExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "ElectricCurrentExponent", sdaiINTEGER, &ElectricCurrentExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "ThermodynamicTemperatureExponent", sdaiINTEGER, &ThermodynamicTemperatureExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "AmountOfSubstanceExponent", sdaiINTEGER, &AmountOfSubstanceExponent);
		sdaiPutAttrBN(ifcDimensionalExponentsInstance, "LuminousIntensityExponent", sdaiINTEGER, &LuminousIntensityExponent);
	}

	return	ifcDimensionalExponentsInstance;
}

int		buildMeasureWithUnitInstance()
{
	int		ifcMeasureWithUnitInstance;
	void	* valueComponentADB;
	double	valueComponent= 0.01745;

	ifcMeasureWithUnitInstance = sdaiCreateInstanceBN(model, "IFCMEASUREWITHUNIT");

	valueComponentADB = sdaiCreateADB(sdaiREAL, &valueComponent);
	sdaiPutADBTypePath(valueComponentADB, 1, "IFCPLANEANGLEMEASURE"); 
	sdaiPutAttrBN(ifcMeasureWithUnitInstance, "ValueComponent", sdaiADB, (void*) valueComponentADB);

	sdaiPutAttrBN(ifcMeasureWithUnitInstance, "UnitComponent", sdaiINSTANCE, (void*) buildSIUnitInstance("PLANEANGLEUNIT", NULL, "RADIAN"));

	return	ifcMeasureWithUnitInstance;
}

int		buildSIUnitInstance(char * UnitType, char * Prefix, char * Name)
{
	int		ifcSIUnitInstance;

	ifcSIUnitInstance = sdaiCreateInstanceBN(model, "IFCSIUNIT");

	sdaiPutAttrBN(ifcSIUnitInstance, "Dimensions", sdaiINTEGER, NULL);
	sdaiPutAttrBN(ifcSIUnitInstance, "UnitType", sdaiENUM, UnitType);
	if	(Prefix) {
		sdaiPutAttrBN(ifcSIUnitInstance, "Prefix", sdaiENUM, Prefix);
	}
	sdaiPutAttrBN(ifcSIUnitInstance, "Name", sdaiENUM, Name);

	return	ifcSIUnitInstance;
}

int		getUnitAssignmentInstance()
{
	int		* aggrUnits;

	if	(!ifcUnitAssignmentInstance) {
		ifcUnitAssignmentInstance = sdaiCreateInstanceBN(model, "IFCUNITASSIGNMENT");

		aggrUnits = sdaiCreateAggrBN(ifcUnitAssignmentInstance, "Units");
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("LENGTHUNIT", "MILLI", "METRE"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("AREAUNIT", NULL, "SQUARE_METRE"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("VOLUMEUNIT", NULL, "CUBIC_METRE"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) getConversionBasedUnitInstance());
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("SOLIDANGLEUNIT", NULL, "STERADIAN"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("MASSUNIT", NULL, "GRAM"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("TIMEUNIT", NULL, "SECOND"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("THERMODYNAMICTEMPERATUREUNIT", NULL, "DEGREE_CELSIUS"));
		sdaiAppend((int) aggrUnits, sdaiINSTANCE, (void*) buildSIUnitInstance("LUMINOUSINTENSITYUNIT", NULL, "LUMEN"));
	}

	return	ifcUnitAssignmentInstance;
}


//
//
//		RelAggregates, RelContainedInSpatialStructure
//
//


int		buildRelAggregatesInstance(char * name, char * description, int ifcRelatingObjectInstance, int ifcRelatedObjectInstance)
{
	int		ifcRelAggregatesInstance, * aggrRelatedObjects;

	ifcRelAggregatesInstance = sdaiCreateInstanceBN(model, "IFCRELAGGREGATES");

	sdaiPutAttrBN(ifcRelAggregatesInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcRelAggregatesInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcRelAggregatesInstance, "Name", sdaiSTRING, name);
	sdaiPutAttrBN(ifcRelAggregatesInstance, "Description", sdaiSTRING, description);
	sdaiPutAttrBN(ifcRelAggregatesInstance, "RelatingObject", sdaiINSTANCE, (void*) ifcRelatingObjectInstance);
	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelAggregatesInstance, "RelatedObjects");
	sdaiAppend((int) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcRelatedObjectInstance);

	return	ifcRelAggregatesInstance;
}

int		buildRelContainedInSpatialStructureInstance(char * name, char * description, int ifcRelatingStructureInstance, int ** aggrRelatedElements)
{
	int		ifcRelContainedInSpatialStructureInstance;

	ifcRelContainedInSpatialStructureInstance = sdaiCreateInstanceBN(model, "IFCRELCONTAINEDINSPATIALSTRUCTURE");

	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "Name", sdaiSTRING, "Default Building");
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "Description", sdaiSTRING, "Description of Default Building");
	(* aggrRelatedElements) = sdaiCreateAggrBN(ifcRelContainedInSpatialStructureInstance, "RelatedElements");
	sdaiPutAttrBN(ifcRelContainedInSpatialStructureInstance, "RelatingStructure", sdaiINSTANCE, (void*) ifcRelatingStructureInstance);

	return	ifcRelContainedInSpatialStructureInstance;
}


//
//
//		Building, BuildingStorey, Project, Site
//
//

// Workaround ... This functions are implemented in minExampleDlg.cpp as local functions and not mention in the header file
// to use them here, we have to add the prototypes
polygon2DStruct* localCreatePolygonStructureForSquare(double min_x, double min_y, double max_x, double max_y);
shellStruct* localCreateShellStructureForCuboid(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);
shellStruct* localCreateShellStructureForCuboidWithOpening(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z, double min_x_opening, double min_z_opening, double max_x_opening, double max_z_opening);



int		buildBuildingInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingInstancePlacement)
{
	int		ifcBuildingInstance;

	ifcBuildingInstance = sdaiCreateInstanceBN(model, "IFCBUILDING");

	sdaiPutAttrBN(ifcBuildingInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcBuildingInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcBuildingInstance, "Name", sdaiSTRING, "Default Building");
	sdaiPutAttrBN(ifcBuildingInstance, "Description", sdaiSTRING, "Description of Default Building");

	(* ifcBuildingInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcBuildingInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcBuildingInstancePlacement));
	sdaiPutAttrBN(ifcBuildingInstance, "CompositionType", sdaiENUM, "ELEMENT");

	// Add geomertry of building 
	sdaiPutAttrBN(ifcBuildingInstance, "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	double buildingWidth = 5000, buildingLength = 5000, buildingHeight = 2300;
	

	polygon2DStruct * pPolygon;
    shellStruct     * pShell;
 
	//poly 
	if ( true ) {

        buildRelAssociatesMaterial(ifcBuildingInstance, buildingLength);
        createIfcPolylineShape(0, buildingLength/2, buildingWidth, buildingLength/2);

        pPolygon = localCreatePolygonStructureForSquare(0, 0, buildingWidth, buildingLength);
        createIfcExtrudedPolygonShape(pPolygon, buildingHeight);
	}
	//brep
	else {
        pShell = localCreateShellStructureForCuboid(0, 0, 0, buildingWidth, buildingLength, buildingHeight);
        createIfcBRepShape(pShell);
	}

	if  (false) {
        createIfcBoundingBoxShape(buildingWidth, buildingLength, buildingHeight, "Box");
	}


	return	ifcBuildingInstance;
}

int		buildBuildingStoreyInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingStoreyInstancePlacement)
{
	int		ifcBuildingStoreyInstance;
	double	elevation = 0;

	ifcBuildingStoreyInstance = sdaiCreateInstanceBN(model, "IFCBUILDINGSTOREY");

	sdaiPutAttrBN(ifcBuildingStoreyInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcBuildingStoreyInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcBuildingStoreyInstance, "Name", sdaiSTRING, "Default Building Storey");
	sdaiPutAttrBN(ifcBuildingStoreyInstance, "Description", sdaiSTRING, "Description of Default Building Storey");

	(* ifcBuildingStoreyInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcBuildingStoreyInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcBuildingStoreyInstancePlacement));
	sdaiPutAttrBN(ifcBuildingStoreyInstance, "CompositionType", sdaiENUM, "ELEMENT");
	sdaiPutAttrBN(ifcBuildingStoreyInstance, "Elevation", sdaiREAL, &elevation);

	return	ifcBuildingStoreyInstance;
}

int		getProjectInstance()
{
    int * aggrRepresentationContexts = NULL;

	if	(! ifcProjectInstance) {
		ifcProjectInstance = sdaiCreateInstanceBN(model, "IFCPROJECT");

		sdaiPutAttrBN(ifcProjectInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
		sdaiPutAttrBN(ifcProjectInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
		sdaiPutAttrBN(ifcProjectInstance, "Name", sdaiSTRING, "Default Project");
		sdaiPutAttrBN(ifcProjectInstance, "Description", sdaiSTRING, "Description of Default Project");
		sdaiPutAttrBN(ifcProjectInstance, "UnitsInContext", sdaiINSTANCE, (void*) getUnitAssignmentInstance());
	    aggrRepresentationContexts = sdaiCreateAggrBN(ifcProjectInstance, "RepresentationContexts");
    	sdaiAppend((int) aggrRepresentationContexts, sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());
	}

	return	ifcProjectInstance;
}

int		buildSiteInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcSiteInstancePlacement)
{
	int		ifcSiteInstance, * aggrRefLatitude, * aggrRefLongitude,
			refLat_x = 24, refLat_y = 28, refLat_z = 0, refLong_x = 54, refLong_y = 25, refLong_z = 0;

	ifcSiteInstance = sdaiCreateInstanceBN(model, "IFCSITE");

	sdaiPutAttrBN(ifcSiteInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcSiteInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcSiteInstance, "Name", sdaiSTRING, "Default Site");
	sdaiPutAttrBN(ifcSiteInstance, "Description", sdaiSTRING, "Description of Default Site");

	(* ifcSiteInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcSiteInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcSiteInstancePlacement));
	sdaiPutAttrBN(ifcSiteInstance, "CompositionType", sdaiENUM, "ELEMENT");

	aggrRefLatitude = sdaiCreateAggrBN(ifcSiteInstance, "RefLatitude");
	sdaiAppend((int) aggrRefLatitude, sdaiINTEGER, &refLat_x);
	sdaiAppend((int) aggrRefLatitude, sdaiINTEGER, &refLat_y);
	sdaiAppend((int) aggrRefLatitude, sdaiINTEGER, &refLat_z);

	aggrRefLongitude = sdaiCreateAggrBN(ifcSiteInstance, "RefLongitude");
	sdaiAppend((int) aggrRefLongitude, sdaiINTEGER, &refLong_x);
	sdaiAppend((int) aggrRefLongitude, sdaiINTEGER, &refLong_y);
	sdaiAppend((int) aggrRefLongitude, sdaiINTEGER, &refLong_z);

	return	ifcSiteInstance;
}


//
//
//		WorldCoordinateSystem, GeometricRepresentationContext
//
//


int     getWorldCoordinateSystemInstance()
{
    point3DStruct   point;

    identityPoint(&point);
	int ifcWorldCoordinateSystemInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT3D");
    sdaiPutAttrBN(ifcWorldCoordinateSystemInstance, "Location", sdaiINSTANCE, (void*) buildCartesianPointInstance(&point));

    return  ifcWorldCoordinateSystemInstance;
}

int     getGeometricRepresentationContextInstance()
{
    if  (! ifcGeometricRepresentationContextInstance) {
        double  precision = 0.00001;
        int     coordinateSpaceDimension = 3;

		ifcGeometricRepresentationContextInstance = sdaiCreateInstanceBN(model, "IFCGEOMETRICREPRESENTATIONCONTEXT");

		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "ContextType", sdaiSTRING, "Model");
		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "CoordinateSpaceDimension", sdaiINTEGER, &coordinateSpaceDimension);
		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "Precision", sdaiREAL, &precision);
		sdaiPutAttrBN(ifcGeometricRepresentationContextInstance, "WorldCoordinateSystem", sdaiINSTANCE, (void*) getWorldCoordinateSystemInstance());
    }

    return  ifcGeometricRepresentationContextInstance;
}
