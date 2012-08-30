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


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "IFCEngine.h"


typedef struct TRANSFORMATIONMATRIXSTRUCT {
	double			_11;
	double			_12;
	double			_13;
	double			_21;
	double			_22;
	double			_23;
	double			_31;
	double			_32;
	double			_33;
	double			_41;
	double			_42;
	double			_43;
}	transformationMatrixStruct;

typedef struct POINT3DSTRUCT {
	double			x;
	double			y;
	double			z;
    int             ifcCartesianPointInstance;
}	point3DStruct;


void	identityMatrix(transformationMatrixStruct * pMatrix);
void	identityPoint(point3DStruct * pPoint);
int		* getTimeStamp();


//
//
//	GUID
//
//


static const char *cConversionTable64 =
 "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_$";

char * CreateCompressedGuidString();
char * getString64FromGuid(const GUID *pGuid, char * buf);
BOOL cv_to_64(const unsigned long number, char *code, int len);


bool    createEmptyIfcFile(char * ifcSchemaName, bool objectsWillBeAdded);
bool    saveIfcFile(char * ifcFileName);
bool    saveIfcFileAsXml(char * ifcFileName);


//
//
//		Application, Organization, Person (OwnerHistory, PersonAndOrganization)
//
//


int		getApplicationInstance();
int		getOrganizationInstance();
int		getOwnerHistoryInstance();
int		getPersonAndOrganizationInstance();
int		getPersonInstance();


//
//
//		CartesianPoint, Direction, LocalPlacement (Axis2Placement)
//
//


int		buildAxis2Placement3DInstance(transformationMatrixStruct * pMatrix);
int		buildCartesianPointInstance(point3DStruct * pPoint);
int		buildDirectionInstance(point3DStruct * pPoint);
int		buildLocalPlacementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo);


//
//
//		ConversionBasedUnit, DimensionalExponents, MeasureWithUnit, SIUnit, UnitAssignment
//
//


int		getConversionBasedUnitInstance();
int		getDimensionalExponentsInstance();
int		buildMeasureWithUnitInstance();
int		buildSIUnitInstance(char * UnitType, char * Prefix, char * Name);
int		getUnitAssignmentInstance();

	
//
//
//		RelAggregates, RelContainedInSpatialStructure
//
//


int		buildRelAggregatesInstance(char * name, char * description, int ifcRelatingObjectInstance, int ifcRelatedObjectInstance);
int		buildRelContainedInSpatialStructureInstance(char * name, char * description, int ifcRelatingStructureInstance, int ** aggrRelatedElements);


//
//
//		Building, BuildingStorey, Project, Site
//
//


int		buildBuildingInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingInstancePlacement);
int		buildBuildingStoreyInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingStoreyInstancePlacement);
int		getProjectInstance();
int		buildSiteInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcSiteInstancePlacement);


//
//
//		WorldCoordinateSystem, GeometricRepresentationContext
//
//


int     getWorldCoordinateSystemInstance();
int     getGeometricRepresentationContextInstance();
