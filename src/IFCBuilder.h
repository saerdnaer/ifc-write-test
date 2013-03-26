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

#include "stdafx.h"
#include "IFCEngine.h"
#include "Point3d.h"
#include "CDP_Building.h"


#define     COORDINATIONVIEW    0
#define     PRESENTATIONVIEW    1
#define 	NULL 0

static const char *cConversionTable64 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_$";

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

	typedef struct POINT2DSTRUCT {
	double			x;
	double			y;
}	point2DStruct;

// from extrudedPolygon.h

typedef struct POLYGON2DSTRUCT {
	POINT2DSTRUCT   * pPoint;
	POLYGON2DSTRUCT * next;
}	polygon2DStruct;



// from BRepIFC.h

	
typedef struct VECTOR3DSTRUCT {
	POINT3DSTRUCT   * pPoint;
	VECTOR3DSTRUCT  * next;
}	vector3DStruct;

typedef struct POLYGON3DSTRUCT {
	VECTOR3DSTRUCT  * pVector;
	VECTOR3DSTRUCT  * pOpeningVector;
	POLYGON3DSTRUCT * next;
}	polygon3DStruct;

typedef struct SHELLSTRUCT {
	POLYGON3DSTRUCT * pPolygon;
	SHELLSTRUCT     * next;
}	shellStruct;



class IFCBuilder {
private:
	char* ifcSchemaName;
	bool saveIfx;
	int view;

	
	int model;
	int timeStamp;

	int* aggrRelatedElements;
	int* aggrRepresentations;

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
	
transformationMatrixStruct matrix;

	int ifcOpeningElementInstancePlacement;
    int ifcWallInstancePlacement;

public:
	IFCBuilder();
	void addBuilding(CDP_Building* building);
	void setHeader(char* ifcFileName);
	void saveIfcFile(char* ifcFileName);
	void saveIfcFileAsXml(char* ifcFileName);

	void createIfcBRepShape(SimpleMesh* mesh);
	int buildShapeRepresentationInstance(SimpleMesh* mesh);
	int buildCartesianPointInstance(Point3d point);


	static void	identityMatrix(transformationMatrixStruct * pMatrix);
	static void	identityPoint(point3DStruct * pPoint);
	int		* getTimeStamp();

	//	GUID
	char* CreateCompressedGuidString();
	char* getString64FromGuid(const GUID *pGuid, char * buf);
	BOOL cv_to_64(const unsigned long number, char *code, int len);




	//		Application, Organization, Person (OwnerHistory, PersonAndOrganization)
	int		getApplicationInstance();
	int		getOrganizationInstance();
	int		getOwnerHistoryInstance();
	int		getPersonAndOrganizationInstance();
	int		getPersonInstance();

	//		CartesianPoint, Direction, LocalPlacement (Axis2Placement)
	int		buildAxis2Placement3DInstance(transformationMatrixStruct * pMatrix);
	int		buildCartesianPointInstance(point3DStruct * pPoint);
	int		buildDirectionInstance(point3DStruct * pPoint);
	int		buildLocalPlacementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo);

	//		ConversionBasedUnit, DimensionalExponents, MeasureWithUnit, SIUnit, UnitAssignment
	int		getConversionBasedUnitInstance();
	int		getDimensionalExponentsInstance();
	int		buildMeasureWithUnitInstance();
	int		buildSIUnitInstance(char * UnitType, char * Prefix, char * Name);
	int		getUnitAssignmentInstance();

	//		RelAggregates, RelContainedInSpatialStructure
	int		buildRelAggregatesInstance(char * name, char * description, int ifcRelatingObjectInstance, int ifcRelatedObjectInstance);
	int		buildRelContainedInSpatialStructureInstance(char * name, char * description, int ifcRelatingStructureInstance, int ** aggrRelatedElements);

	//		Building, BuildingStorey, Project, Site
	int		buildBuildingInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingInstancePlacement);
	int		buildBuildingStoreyInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcBuildingStoreyInstancePlacement);
	int		getProjectInstance();
	int		buildSiteInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcSiteInstancePlacement);


	// Helper functions for example data, formerly als local functions minExampleDlg.cpp 
	// --Andreas
	point3DStruct* create3DPoint(point3DStruct ** pPoint, double x, double y, double z);
	polygon2DStruct* localCreatePolygonStructureForSquare(double min_x, double min_y, double max_x, double max_y);

	polygon3DStruct* create3DPolygonWith4Vectors(polygon3DStruct** pPolygon, point3DStruct* pPointI, point3DStruct* pPointII, point3DStruct* pPointIII, point3DStruct* pPointIV);
	void add3DPolygonOpeningWith4Vectors(polygon3DStruct * polygon, point3DStruct * pPointI, point3DStruct * pPointII, point3DStruct * pPointIII, point3DStruct * pPointIV);
	shellStruct * localCreateShellStructureForCuboid(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);
	shellStruct * localCreateShellStructureForCuboidWithOpening(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z, double min_x_opening, double min_z_opening, double max_x_opening, double max_z_opening);
	

	//		WorldCoordinateSystem, GeometricRepresentationContext
	int     getWorldCoordinateSystemInstance();
	int     getGeometricRepresentationContextInstance();


// from extrudedPolygon.h
	void    createIfcExtrudedPolygonShape(polygon2DStruct * pPolygon, double depth);
	void    createIfcPolylineShape(double p0x, double p0y, double p1x, double p1y);


	//		ShapeRepresentation
	int		buildShapeRepresentationInstance(polygon2DStruct * pPolygon, double depth); // from extrudedPolygon.h
	int		buildShapeRepresentationInstance(double p0x, double p0y, double p1x, double p1y); // from extrudedPolygon.h
	int		buildShapeRepresentationInstance(shellStruct * pShell); // from BRepIFC.h
	int		buildShapeRepresentationInstance(double width, double thickness, double height, char * representationIdentifier); // from boundingBoxIfc.h

	//		ArbitraryClosedProfileDef, CartesianPoint(2D), ExtrudedAreaSolid, Polyline
	int		buildArbitraryClosedProfileDefInstance(polygon2DStruct * pPolygon);
	int		buildCartesianPointInstance(double x, double y);
	int		buildExtrudedAreaSolidInstance(polygon2DStruct * pPolygon, double depth);
	int		buildPolylineInstance(polygon2DStruct * pPolygon);
	int		buildPolylineInstance(double p0x, double p0y, double p1x, double p1y);

// from BRepIFC.h

	
	void    createIfcBRepShape(shellStruct * pShell);
	//		ShapeRepresentation

	// helper function extracted from miniExampleDlg --Andreas
	void genIfcTimestamp(char *timeStamp);


// from boundingBoxIfc.h

	void    createIfcBoundingBoxShape(double width, double thickness, double height, char * representationIdentifier);

	//		BoundingBox, ShapeRepresentation
	int		buildBoundingBoxInstance(double width, double thickness, double height); // from boundingBoxIfc.h

	//      CartesianPoint(3D)
	int		buildCartesianPointInstance(double x, double y, double z);

// from baseIfcObject.h

	
	//      RelAssociatesMaterial, MaterialLayerSetUsage, MaterialLayerSet, MaterialLayer
	int		buildRelAssociatesMaterial(int ifcBuildingElementInstance, double thickness);
	int     buildMaterialLayerSetUsage(double thickness);
	int     buildMaterialLayerSet(double thickness);
	int     buildMaterialLayer(double thickness);
	int     buildMaterial();


	// Elements
	int		createIfcWall(char * pWallName, double xOffset, double yOffset, double zOffset);
	int		createIfcWallStandardCase(char * pWallName, double xOffset, double yOffset, double zOffset);
	int		createIfcOpeningElement(char * pOpeningElementName, double xOffset, double yOffset, double zOffset);
	int		createIfcWindow(char * pWindowName, double xOffset, double yOffset, double zOffset);

	//		ProductDefinitionShape
	int		buildProductDefinitionShapeInstance();

	//		IfcWall, IfcWallStandardCase, IfcOpeningElement, IfcWindow
	int		buildWallInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWallInstancePlacement, char * pWallName);
	int		buildWallStandardCaseInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWallInstancePlacement, char * pWallName);
	int		buildOpeningElementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcOpeningElementInstancePlacement, char * pOpeningElementName);
	int		buildWindowInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWindowInstancePlacement, char * pWindowName);

	//		RelVoidsElement, RelFillsElement
	int		buildRelVoidsElementInstance(int ifcBuildingElementInstance, int ifcOpeningElementInstance);
	int     buildRelFillsElementInstance(int ifcOpeningElementInstance, int ifcBuildingElementInstance);

};