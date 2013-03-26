/*
 * Adapted for CDP: Andreas Hubel (andreas.hubel@tum.de)
 *
 * This class tries to wrap the TNO ifc dll, for usage with CDP data structures
 * It's based on an C/C++ example available from TNO. This example is more C than 
 * C++, so be aware of dragons ahead! 
 *
 *
 * Now follows the orginal file header
 */

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
#include <iostream>
#include <time.h>
#include <conio.h>
#include "IFCBuilder.h"


IFCBuilder::IFCBuilder() {
	model = 0; 
	saveIfx = false;
	aggrRelatedElements = NULL;
	aggrRepresentations = NULL;

	ifcApplicationInstance = 0,
	ifcBuildingInstance = 0,
	ifcBuildingInstancePlacement = 0,
	ifcBuildingStoreyInstance = 0,
	ifcBuildingStoreyInstancePlacement = 0,
	ifcBuildOwnerHistoryInstance = 0,
	ifcConversionBasedUnitInstance = 0,
	ifcDimensionalExponentsInstance = 0,
	ifcGeometricRepresentationContextInstance = 0,
	ifcOrganizationInstance = 0,
	ifcPersonAndOrganizationInstance = 0,
	ifcPersonInstance = 0,
	ifcProjectInstance = 0,
	ifcSiteInstance = 0,
	ifcSiteInstancePlacement = 0,
	ifcUnitAssignmentInstance = 0;

	char ifcSchemaName[30] = "../IFC2X3_Final.exp";

	if ( !PathFileExists(ifcSchemaName) ) {
		std::cerr << "IFC schema file is not accessible" << std::endl;
	}
	model = sdaiCreateModelBN(1, NULL, ifcSchemaName);

	if	(!model) {
		std::cerr << ("IFC Model could not be instantiated, probably it cannot find the schema file.");
		getch();
		exit(-1);
		return;
	}

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

	ifcProjectInstance = getProjectInstance();
	ifcSiteInstance = buildSiteInstance(&matrix, NULL, &ifcSiteInstancePlacement);
	buildRelAggregatesInstance("ProjectContainer", "ProjectContainer for Sites", ifcProjectInstance, ifcSiteInstance);

	//  CDP: now own function: addBuilding(CDP_Building building) --Andreas
	//ifcBuildingInstance = buildBuildingInstance(&matrix, ifcSiteInstancePlacement, &ifcBuildingInstancePlacement);
	//buildRelAggregatesInstance("SiteContainer", "SiteContainer For Buildings", ifcSiteInstance, ifcBuildingInstance);

	// we don't modell the individual floors --Andreas
	//ifcBuildingStoreyInstance = buildBuildingStoreyInstance(&matrix, ifcBuildingInstancePlacement, &ifcBuildingStoreyInstancePlacement);
	//buildRelAggregatesInstance("BuildingContainer", "BuildingContainer for BuildigStories", ifcBuildingInstance, ifcBuildingStoreyInstance);

	/*
	if  (objectsWillBeAdded) {
		buildRelContainedInSpatialStructureInstance("BuildingStoreyContainer", "BuildingStoreyContainer for Building Elements", ifcBuildingStoreyInstance, &aggrRelatedElements);
	}
	*/

}

void IFCBuilder::addBuilding(CDP_Building* building) {

	ifcBuildingInstance = buildBuildingInstance(&matrix, ifcSiteInstancePlacement, &ifcBuildingInstancePlacement);
	buildRelAggregatesInstance("SiteContainer", "SiteContainer For Buildings", ifcSiteInstance, ifcBuildingInstance);

	// Add geometry of building
	sdaiPutAttrBN(ifcBuildingInstance, "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	double buildingWidth = 5000, buildingLength = 5000, buildingHeight = 2300;


    buildRelAssociatesMaterial(ifcBuildingInstance, buildingLength);
    createIfcPolylineShape(0, buildingLength/2, buildingWidth, buildingLength/2);

	//poly
	if ( false ) {
		polygon2DStruct* pPolygon;
        pPolygon = localCreatePolygonStructureForSquare(0, 0, buildingWidth, buildingLength);
        createIfcExtrudedPolygonShape(pPolygon, buildingHeight);
	}
	//brep
	if (false) { 	
	    shellStruct* pShell;
        pShell = localCreateShellStructureForCuboid(0, 0, 0, buildingWidth, buildingLength, buildingHeight);
        createIfcBRepShape(pShell);
	}
	createIfcBRepShape(building->mesh);



	// add Bounding Box
	if  (false) {
        createIfcBoundingBoxShape(buildingWidth, buildingLength, buildingHeight, "Box");
	}
}


void IFCBuilder::createIfcBRepShape(SimpleMesh* mesh)
{
       sdaiAppend((int) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(mesh));
}



int IFCBuilder::buildShapeRepresentationInstance(SimpleMesh* mesh)
{
	// this function is based on buildShapeRepresentationInstance(shellStruct* pShell) --Andreas
	
	int ifcShapeRepresentationInstance;
	int* aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");

	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, "Body");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, "Brep");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());

    //while  (pShell) {
    //POLYGON3DSTRUCT  * pPolygon = pShell->pPolygon;
    int     ifcFacetedBrepInstance, ifcClosedShellInstance,* aggrCfsFaces;

	ifcClosedShellInstance = sdaiCreateInstanceBN(model, "IFCCLOSEDSHELL");
	aggrCfsFaces = sdaiCreateAggrBN(ifcClosedShellInstance, "CfsFaces");

	int ifcPolyLoopInstance,* aggrPolygon,
		ifcFaceOuterBoundInstance,
		ifcFaceInstance,* aggrBounds;

	vector<int> ifcPointInstances(mesh->vertices.size(), 0);

	for(vector<SimpleMesh::polygon_type>::iterator polygon = mesh->polygons.begin(); 
		polygon != mesh->polygons.end(); 
		++polygon) {


	    ifcPolyLoopInstance = sdaiCreateInstanceBN(model, "IFCPOLYLOOP");
	    aggrPolygon = sdaiCreateAggrBN(ifcPolyLoopInstance, "Polygon");

		for(SimpleMesh::polygon_type::iterator point_id = polygon->begin(); 
		point_id != polygon->end(); 
		++point_id) {
            //  Check if point is already written
            if  (ifcPointInstances[*point_id] == 0) {
				ifcPointInstances[*point_id] = buildCartesianPointInstance(mesh->vertices[*point_id]);
            }
                            
            sdaiAppend((int) aggrPolygon, sdaiINSTANCE, (void *) ifcPointInstances[*point_id]);
        }

	    ifcFaceOuterBoundInstance = sdaiCreateInstanceBN(model, "IFCFACEOUTERBOUND");
	    sdaiPutAttrBN(ifcFaceOuterBoundInstance, "Bound", sdaiINSTANCE, (void *) ifcPolyLoopInstance);
        sdaiPutAttrBN(ifcFaceOuterBoundInstance, "Orientation", sdaiENUM, "T");

	    ifcFaceInstance = sdaiCreateInstanceBN(model, "IFCFACE");
	    aggrBounds = sdaiCreateAggrBN(ifcFaceInstance, "Bounds");
	    sdaiAppend((int) aggrBounds, sdaiINSTANCE, (void *) ifcFaceOuterBoundInstance);

	    sdaiAppend((int) aggrCfsFaces, sdaiINSTANCE, (void *) ifcFaceInstance);

        // in the CDP our buildings don't support holes in buildings so we can skip the inner bound face
		// if you want to add this feature have a look at buildShapeRepresentationInstance(shellStruct* pShell)
    }

	ifcFacetedBrepInstance = sdaiCreateInstanceBN(model, "IFCFACETEDBREP");
	sdaiPutAttrBN(ifcFacetedBrepInstance, "Outer", sdaiINSTANCE, (void *) ifcClosedShellInstance);
	
    sdaiAppend((int) aggrItems, sdaiINSTANCE, (void*) ifcFacetedBrepInstance);

	//}

	return	ifcShapeRepresentationInstance;
}

int	IFCBuilder::buildCartesianPointInstance(Point3d point)
{
	return buildCartesianPointInstance(point.x, point.y, point.z);
}



void IFCBuilder::setHeader(char* ifcFileName)
{

    char description[512], timeStamp[512];


	if  (view == COORDINATIONVIEW) {
        memcpy(description, "ViewDefinition [CoordinationView]", sizeof("ViewDefinition [CoordinationView]"));
    } else {
        //ASSERT(view == PRESENTATIONVIEW);
        memcpy(description, "ViewDefinition [PresentationView]", sizeof("ViewDefinition [PresentationView]"));
    }

	genIfcTimestamp(timeStamp);



    int i = 0, j = 0;
    while  (ifcFileName[i]) {
        if  (ifcFileName[i++] == '\\') {
            j = i;
        }
    }

    SetSPFFHeader(
			model,
            description,                        //  description
            "2;1",                              //  implementationLevel
            &ifcFileName[j],                    //  name
            timeStamp,							//  timeStamp
            "Architect",                        //  author
            "Building Designer Office",         //  organization
            "IFC Engine DLL version 1.02 beta", //  preprocessorVersion
            "IFC Engine DLL version 1.02 beta", //  originatingSystem
            "The authorising person",           //  authorization
            "IFC2X3"                            //  fileSchema
    );

}


//  Save the created configuration
void IFCBuilder::saveIfcFile(char* ifcFileName)
{
	setHeader(ifcFileName);
	sdaiSaveModelBN(model, ifcFileName);
}

void IFCBuilder::saveIfcFileAsXml(char* ifcFileName)
{
	setHeader(ifcFileName);
	sdaiSaveModelAsXmlBN(model, ifcFileName);
}

void IFCBuilder::identityMatrix(transformationMatrixStruct * pMatrix)
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

void IFCBuilder::identityPoint(point3DStruct * pPoint)
{
	pPoint->x = 0;
	pPoint->y = 0;
	pPoint->z = 0;
}

int* IFCBuilder::getTimeStamp()
{
	timeStamp = time(0);

	return	&timeStamp;
}


//
//
//	GUID
//
//


char* IFCBuilder::CreateCompressedGuidString()
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

char* IFCBuilder::getString64FromGuid(const GUID *pGuid, char * buf )
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

BOOL IFCBuilder::cv_to_64(const unsigned long number, char *code, int len)
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


int	IFCBuilder::getApplicationInstance()
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

int	IFCBuilder::getOwnerHistoryInstance()
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

int	IFCBuilder::getPersonAndOrganizationInstance()
{
	if	(!ifcPersonAndOrganizationInstance) {
		ifcPersonAndOrganizationInstance = sdaiCreateInstanceBN(model, "IFCPERSONANDORGANIZATION");

		sdaiPutAttrBN(ifcPersonAndOrganizationInstance, "ThePerson", sdaiINSTANCE, (void*) getPersonInstance());
		sdaiPutAttrBN(ifcPersonAndOrganizationInstance, "TheOrganization", sdaiINSTANCE, (void*) getOrganizationInstance());
	}

	return	ifcPersonAndOrganizationInstance;
}

int	IFCBuilder::getPersonInstance()
{
	if	(!ifcPersonInstance) {
		ifcPersonInstance = sdaiCreateInstanceBN(model, "IFCPERSON");

		sdaiPutAttrBN(ifcPersonInstance, "GivenName", sdaiSTRING, "Hans");
		sdaiPutAttrBN(ifcPersonInstance, "Id", sdaiSTRING, "ID001");
		sdaiPutAttrBN(ifcPersonInstance, "FamilyName", sdaiSTRING, "Muster");
	}

	return	ifcPersonInstance;
}

int	IFCBuilder::getOrganizationInstance()
{
	if	(!ifcOrganizationInstance) {
		ifcOrganizationInstance = sdaiCreateInstanceBN(model, "IFCORGANIZATION");

		sdaiPutAttrBN(ifcOrganizationInstance, "Name", sdaiSTRING, "TUM");
		sdaiPutAttrBN(ifcOrganizationInstance, "Description", sdaiSTRING, "Technische Universitaet Muenchen");
	}

	return	ifcOrganizationInstance;
}

//
//
//		CartesianPoint, Direction, LocalPlacement (Axis2Placement)
//
//


int	IFCBuilder::buildAxis2Placement3DInstance(transformationMatrixStruct * pMatrix)
{
	int ifcAxis2Placement3DInstance;

	ifcAxis2Placement3DInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT3D");

	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Location", sdaiINSTANCE, (void*) buildCartesianPointInstance((point3DStruct *) &pMatrix->_41));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "Axis", sdaiINSTANCE, (void*) buildDirectionInstance((point3DStruct *) &pMatrix->_31));
	sdaiPutAttrBN(ifcAxis2Placement3DInstance, "RefDirection", sdaiINSTANCE, (void*) buildDirectionInstance((point3DStruct *) &pMatrix->_11));

	return	ifcAxis2Placement3DInstance;
}

int	IFCBuilder::buildCartesianPointInstance(point3DStruct * pPoint)
{
	int ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &pPoint->x);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &pPoint->y);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &pPoint->z);

	return	ifcCartesianPointInstance;
}

int	IFCBuilder::buildDirectionInstance(point3DStruct * pPoint)
{
	int ifcDirectionInstance, * aggrDirectionRatios;
	double	_null = 0, _one = 1;

	ifcDirectionInstance = sdaiCreateInstanceBN(model, "IFCDIRECTION");

	aggrDirectionRatios = sdaiCreateAggrBN(ifcDirectionInstance, "DirectionRatios");
	sdaiAppend((int) aggrDirectionRatios, sdaiREAL, &pPoint->x);
	sdaiAppend((int) aggrDirectionRatios, sdaiREAL, &pPoint->y);
	sdaiAppend((int) aggrDirectionRatios, sdaiREAL, &pPoint->z);

	return	ifcDirectionInstance;
}

int	IFCBuilder::buildLocalPlacementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo)
{
	int ifcLocalPlacementInstance;

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


int	IFCBuilder::getConversionBasedUnitInstance()
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

int	IFCBuilder::getDimensionalExponentsInstance()
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

int	IFCBuilder::buildMeasureWithUnitInstance()
{
	int ifcMeasureWithUnitInstance;
	void	* valueComponentADB;
	double	valueComponent= 0.01745;

	ifcMeasureWithUnitInstance = sdaiCreateInstanceBN(model, "IFCMEASUREWITHUNIT");

	valueComponentADB = sdaiCreateADB(sdaiREAL, &valueComponent);
	sdaiPutADBTypePath(valueComponentADB, 1, "IFCPLANEANGLEMEASURE"); 
	sdaiPutAttrBN(ifcMeasureWithUnitInstance, "ValueComponent", sdaiADB, (void*) valueComponentADB);

	sdaiPutAttrBN(ifcMeasureWithUnitInstance, "UnitComponent", sdaiINSTANCE, (void*) buildSIUnitInstance("PLANEANGLEUNIT", NULL, "RADIAN"));

	return	ifcMeasureWithUnitInstance;
}

int	IFCBuilder::buildSIUnitInstance(char * UnitType, char * Prefix, char * Name)
{
	int ifcSIUnitInstance;

	ifcSIUnitInstance = sdaiCreateInstanceBN(model, "IFCSIUNIT");

	sdaiPutAttrBN(ifcSIUnitInstance, "Dimensions", sdaiINTEGER, NULL);
	sdaiPutAttrBN(ifcSIUnitInstance, "UnitType", sdaiENUM, UnitType);
	if	(Prefix) {
		sdaiPutAttrBN(ifcSIUnitInstance, "Prefix", sdaiENUM, Prefix);
	}
	sdaiPutAttrBN(ifcSIUnitInstance, "Name", sdaiENUM, Name);

	return	ifcSIUnitInstance;
}

int	IFCBuilder::getUnitAssignmentInstance()
{
	int	* aggrUnits;

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


int	IFCBuilder::buildRelAggregatesInstance(char * name, char * description, int ifcRelatingObjectInstance, int ifcRelatedObjectInstance)
{
	int ifcRelAggregatesInstance, * aggrRelatedObjects;

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

int	IFCBuilder::buildRelContainedInSpatialStructureInstance(char * name, char * description, int ifcRelatingStructureInstance, int ** aggrRelatedElements)
{
	int ifcRelContainedInSpatialStructureInstance;

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

int	IFCBuilder::getProjectInstance()
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

int	IFCBuilder::buildSiteInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcSiteInstancePlacement)
{
	int ifcSiteInstance, * aggrRefLatitude, * aggrRefLongitude,
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


int	IFCBuilder::buildBuildingInstance(transformationMatrixStruct* pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingInstancePlacement)
{
	int ifcBuildingInstance;

	ifcBuildingInstance = sdaiCreateInstanceBN(model, "IFCBUILDING");

	sdaiPutAttrBN(ifcBuildingInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcBuildingInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcBuildingInstance, "Name", sdaiSTRING, "Default Building");
	sdaiPutAttrBN(ifcBuildingInstance, "Description", sdaiSTRING, "Description of Default Building");

	(* ifcBuildingInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcBuildingInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcBuildingInstancePlacement));
	sdaiPutAttrBN(ifcBuildingInstance, "CompositionType", sdaiENUM, "ELEMENT");

	return	ifcBuildingInstance;
}

int	IFCBuilder::buildBuildingStoreyInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingStoreyInstancePlacement)
{
	int ifcBuildingStoreyInstance;
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


//
//
// Helper functions for example data, formerly als local functions minExampleDlg.cpp 
// --Andreas
//
//
point3DStruct* IFCBuilder::create3DPoint(point3DStruct ** pPoint, double x, double y, double z)
{
    (* pPoint) = new point3DStruct();
    (* pPoint)->x = x;
    (* pPoint)->y = y;
    (* pPoint)->z = z;
    (* pPoint)->ifcCartesianPointInstance = 0;

    return  (* pPoint);
}

polygon2DStruct * IFCBuilder::localCreatePolygonStructureForSquare(double min_x, double min_y, double max_x, double max_y)
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

polygon3DStruct * IFCBuilder::create3DPolygonWith4Vectors(polygon3DStruct ** pPolygon, point3DStruct * pPointI, point3DStruct * pPointII, point3DStruct * pPointIII, point3DStruct * pPointIV)
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

void IFCBuilder::add3DPolygonOpeningWith4Vectors(polygon3DStruct * polygon, point3DStruct * pPointI, point3DStruct * pPointII, point3DStruct * pPointIII, point3DStruct * pPointIV)
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

shellStruct * IFCBuilder::localCreateShellStructureForCuboid(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z)
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

shellStruct * IFCBuilder::localCreateShellStructureForCuboidWithOpening(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z, double min_x_opening, double min_z_opening, double max_x_opening, double max_z_opening)
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

//
//
//		WorldCoordinateSystem, GeometricRepresentationContext
//
//


int IFCBuilder::getWorldCoordinateSystemInstance()
{
    point3DStruct   point;

    identityPoint(&point);
	int ifcWorldCoordinateSystemInstance = sdaiCreateInstanceBN(model, "IFCAXIS2PLACEMENT3D");
    sdaiPutAttrBN(ifcWorldCoordinateSystemInstance, "Location", sdaiINSTANCE, (void*) buildCartesianPointInstance(&point));

    return  ifcWorldCoordinateSystemInstance;
}

int IFCBuilder::getGeometricRepresentationContextInstance()
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

// helper function extracted from miniExampleDlg --Andreas
void IFCBuilder::genIfcTimestamp(char *timeStamp) {
	 
	time_t t;
    struct tm * tInfo;

    time ( &t );
    tInfo = localtime ( &t );

	_itoa(1900 + tInfo->tm_year, &timeStamp[0], 10);
    _itoa(100 + 1 + tInfo->tm_mon, &timeStamp[4], 10);
    _itoa(100 + tInfo->tm_mday, &timeStamp[7], 10);
    timeStamp[4] = '-';
    timeStamp[7] = '-';
    _itoa(100 + tInfo->tm_hour, &timeStamp[10], 10);
    _itoa(100 + tInfo->tm_min, &timeStamp[13], 10);
    _itoa(100 + tInfo->tm_sec, &timeStamp[16], 10);
    timeStamp[10] = 'T';
    timeStamp[13] = ':';
    timeStamp[16] = ':';
    timeStamp[19] = 0;
        
    //int hour = tm.tm_hour();
}

