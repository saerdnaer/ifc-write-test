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


#include "baseIfc.h"


int		createIfcWall(char * pWallName, double xOffset, double yOffset, double zOffset);
int		createIfcWallStandardCase(char * pWallName, double xOffset, double yOffset, double zOffset);
int		createIfcOpeningElement(char * pOpeningElementName, double xOffset, double yOffset, double zOffset);
int		createIfcWindow(char * pWindowName, double xOffset, double yOffset, double zOffset);


//
//
//		ProductDefinitionShape
//
//


int		buildProductDefinitionShapeInstance();


//
//
//		IfcWall, IfcWallStandardCase, IfcOpeningElement, IfcWindow
//
//


int		buildWallInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWallInstancePlacement, char * pWallName);
int		buildWallStandardCaseInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWallInstancePlacement, char * pWallName);
int		buildOpeningElementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcOpeningElementInstancePlacement, char * pOpeningElementName);
int		buildWindowInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWindowInstancePlacement, char * pWindowName);


//
//
//		RelVoidsElement, RelFillsElement
//
//


int		buildRelVoidsElementInstance(int ifcBuildingElementInstance, int ifcOpeningElementInstance);
int     buildRelFillsElementInstance(int ifcOpeningElementInstance, int ifcBuildingElementInstance);


//
//
//      RelAssociatesMaterial, MaterialLayerSetUsage, MaterialLayerSet, MaterialLayer
//
//


int		buildRelAssociatesMaterial(int ifcBuildingElementInstance, double thickness);
int     buildMaterialLayerSetUsage(double thickness);
int     buildMaterialLayerSet(double thickness);
int     buildMaterialLayer(double thickness);
int     buildMaterial();
