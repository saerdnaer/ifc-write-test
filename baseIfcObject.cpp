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
#include "baseIfcObject.h"

extern  int     model;

extern  int     * aggrRelatedElements;

extern  int     ifcBuildingInstancePlacement,
                ifcBuildingStoreyInstancePlacement;

int        * aggrRepresentations;

int         ifcOpeningElementInstancePlacement,
            ifcWallInstancePlacement;

int		createIfcWall(char * pWallName, double xOffset, double yOffset, double zOffset)
{
    transformationMatrixStruct  matrix;
    int ifcWallInstance;

    identityMatrix(&matrix);
    matrix._41 = xOffset;
    matrix._42 = yOffset;
    matrix._43 = zOffset;

    //
    //      Build Wall and add it to the BuildingStorey
    //
    ifcWallInstance = buildWallInstance(&matrix, ifcBuildingStoreyInstancePlacement, &ifcWallInstancePlacement, pWallName);
	sdaiAppend((int) aggrRelatedElements, sdaiINSTANCE, (void *) ifcWallInstance);

	return	ifcWallInstance;
}

int		createIfcWallStandardCase(char * pWallName, double xOffset, double yOffset, double zOffset)
{
    transformationMatrixStruct  matrix;
    int ifcWallStandardCaseInstance;

    identityMatrix(&matrix);
    matrix._41 = xOffset;
    matrix._42 = yOffset;
    matrix._43 = zOffset;

    //
    //      Build Wall and add it to the BuildingStorey
    //
    ifcWallStandardCaseInstance = buildWallStandardCaseInstance(&matrix, ifcBuildingStoreyInstancePlacement, &ifcWallInstancePlacement, pWallName);
	sdaiAppend((int) aggrRelatedElements, sdaiINSTANCE, (void *) ifcWallStandardCaseInstance);

	return	ifcWallStandardCaseInstance;
}

int		createIfcOpeningElement(char * pOpeningElementName, double xOffset, double yOffset, double zOffset)
{
    transformationMatrixStruct  matrix;
    int ifcOpeningElementInstance;

    identityMatrix(&matrix);
    matrix._41 = xOffset;
    matrix._42 = yOffset;
    matrix._43 = zOffset;

    //
    //      Build Opening Element
    //
    ifcOpeningElementInstance = buildOpeningElementInstance(&matrix, ifcWallInstancePlacement, &ifcOpeningElementInstancePlacement, pOpeningElementName);

	return	ifcOpeningElementInstance;
}

int		createIfcWindow(char * pWindowName, double xOffset, double yOffset, double zOffset)
{
    transformationMatrixStruct  matrix;
    int ifcWindowInstance, ifcWindowInstancePlacement;

    identityMatrix(&matrix);
    matrix._41 = xOffset;
    matrix._42 = yOffset;
    matrix._43 = zOffset;

    //
    //      Build Window and add it to the BuildingStorey
    //
    ifcWindowInstance = buildWindowInstance(&matrix, ifcBuildingStoreyInstancePlacement, &ifcWindowInstancePlacement, pWindowName);
    sdaiAppend((int) aggrRelatedElements, sdaiINSTANCE, (void *) ifcWindowInstance);

	return	ifcWindowInstance;
}

    
//
//
//		ProductDefinitionShape
//
//


int		buildProductDefinitionShapeInstance()
{
	int		ifcProductDefinitionShapeInstance;

	ifcProductDefinitionShapeInstance = sdaiCreateInstanceBN(model, "IFCPRODUCTDEFINITIONSHAPE");

	aggrRepresentations = sdaiCreateAggrBN(ifcProductDefinitionShapeInstance, "Representations");

	return	ifcProductDefinitionShapeInstance;
}


//
//
//		IfcWall, IfcWallStandardCase, IfcOpeningElement, IfcWindow
//
//


int		buildWallInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWallInstancePlacement, char * pWallName)
{
	int		ifcWallInstance = sdaiCreateInstanceBN(model, "IFCWALL");

	sdaiPutAttrBN(ifcWallInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcWallInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWallInstance, "Name", sdaiSTRING, pWallName);
	sdaiPutAttrBN(ifcWallInstance, "Description", sdaiSTRING, "Description of Wall");

	(* ifcWallInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcWallInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcWallInstancePlacement));
	sdaiPutAttrBN(ifcWallInstance, "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	return	ifcWallInstance;
}

int		buildWallStandardCaseInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWallInstancePlacement, char * pWallName)
{
	int		ifcWallInstance = sdaiCreateInstanceBN(model, "IFCWALLSTANDARDCASE");

	sdaiPutAttrBN(ifcWallInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcWallInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWallInstance, "Name", sdaiSTRING, pWallName);
	sdaiPutAttrBN(ifcWallInstance, "Description", sdaiSTRING, "Description of Wall");

	(* ifcWallInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcWallInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcWallInstancePlacement));
	sdaiPutAttrBN(ifcWallInstance, "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	return	ifcWallInstance;
}

int		buildOpeningElementInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcOpeningElementInstancePlacement, char * pOpeningElementName)
{
	int		ifcOpeningElementInstance = sdaiCreateInstanceBN(model, "IFCOPENINGELEMENT");

	sdaiPutAttrBN(ifcOpeningElementInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcOpeningElementInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcOpeningElementInstance, "Name", sdaiSTRING, pOpeningElementName);
	sdaiPutAttrBN(ifcOpeningElementInstance, "Description", sdaiSTRING, "Description of Opening");

	(* ifcOpeningElementInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcOpeningElementInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcOpeningElementInstancePlacement));
	sdaiPutAttrBN(ifcOpeningElementInstance, "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	return	ifcOpeningElementInstance;
}

int		buildWindowInstance(transformationMatrixStruct * pMatrix, int ifcPlacementRelativeTo, int * ifcWindowInstancePlacement, char * pWindowName)
{
	int		ifcWindowInstance = sdaiCreateInstanceBN(model, "IFCWINDOW");

	sdaiPutAttrBN(ifcWindowInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcWindowInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());
	sdaiPutAttrBN(ifcWindowInstance, "Name", sdaiSTRING, pWindowName);
	sdaiPutAttrBN(ifcWindowInstance, "Description", sdaiSTRING, "Description of Window");

	(* ifcWindowInstancePlacement) = buildLocalPlacementInstance(pMatrix, ifcPlacementRelativeTo);
	sdaiPutAttrBN(ifcWindowInstance, "ObjectPlacement", sdaiINSTANCE, (void*) (* ifcWindowInstancePlacement));
	sdaiPutAttrBN(ifcWindowInstance, "Representation", sdaiINSTANCE, (void*) buildProductDefinitionShapeInstance());

	return	ifcWindowInstance;
}


//
//
//		RelVoidsElement, RelFillsElement
//
//


int		buildRelVoidsElementInstance(int ifcBuildingElementInstance, int ifcOpeningElementInstance)
{
	int		ifcRelVoidsElementInstance;

	ifcRelVoidsElementInstance = sdaiCreateInstanceBN(model, "IFCRELVOIDSELEMENT");

	sdaiPutAttrBN(ifcRelVoidsElementInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcRelVoidsElementInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	sdaiPutAttrBN(ifcRelVoidsElementInstance, "RelatingBuildingElement", sdaiINSTANCE, (void*) ifcBuildingElementInstance);
	sdaiPutAttrBN(ifcRelVoidsElementInstance, "RelatedOpeningElement", sdaiINSTANCE, (void*) ifcOpeningElementInstance);

	return	ifcRelVoidsElementInstance;
}

int     buildRelFillsElementInstance(int ifcOpeningElementInstance, int ifcBuildingElementInstance)
{
	int		ifcRelFillsElementInstance;

	ifcRelFillsElementInstance = sdaiCreateInstanceBN(model, "IFCRELFILLSELEMENT");

	sdaiPutAttrBN(ifcRelFillsElementInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcRelFillsElementInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	sdaiPutAttrBN(ifcRelFillsElementInstance, "RelatingOpeningElement", sdaiINSTANCE, (void*) ifcOpeningElementInstance);
	sdaiPutAttrBN(ifcRelFillsElementInstance, "RelatedBuildingElement", sdaiINSTANCE, (void*) ifcBuildingElementInstance);

	return	ifcRelFillsElementInstance;
}


//
//
//      RelAssociatesMaterial, MaterialLayerSetUsage, MaterialLayerSet, MaterialLayer
//
//


int		buildRelAssociatesMaterial(int ifcBuildingElementInstance, double thickness)
{
	int		ifcRelAssociatesMaterialInstance, * aggrRelatedObjects;

	ifcRelAssociatesMaterialInstance = sdaiCreateInstanceBN(model, "IFCRELASSOCIATESMATERIAL");

	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance, "GlobalId", sdaiSTRING, (void*) CreateCompressedGuidString());
	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance, "OwnerHistory", sdaiINSTANCE, (void*) getOwnerHistoryInstance());

	aggrRelatedObjects = sdaiCreateAggrBN(ifcRelAssociatesMaterialInstance, "RelatedObjects");
    sdaiAppend((int) aggrRelatedObjects, sdaiINSTANCE, (void*) ifcBuildingElementInstance);
	sdaiPutAttrBN(ifcRelAssociatesMaterialInstance, "RelatingMaterial", sdaiINSTANCE, (void*) buildMaterialLayerSetUsage(thickness));

	return	ifcRelAssociatesMaterialInstance;
}

int     buildMaterialLayerSetUsage(double thickness)
{
	int		ifcMaterialLayerSetUsageInstance;
    double  offsetFromReferenceLine = -thickness/2;

	ifcMaterialLayerSetUsageInstance = sdaiCreateInstanceBN(model, "IFCMATERIALLAYERSETUSAGE");

	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance, "ForLayerSet", sdaiINSTANCE, (void*) buildMaterialLayerSet(thickness));
	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance, "LayerSetDirection", sdaiENUM, "AXIS2");
	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance, "DirectionSense", sdaiENUM, "POSITIVE");
	sdaiPutAttrBN(ifcMaterialLayerSetUsageInstance, "OffsetFromReferenceLine", sdaiREAL, &offsetFromReferenceLine);

    return  ifcMaterialLayerSetUsageInstance;
}

int     buildMaterialLayerSet(double thickness)
{
	int		ifcMaterialLayerSetInstance, * aggrMaterialLayers;

	ifcMaterialLayerSetInstance = sdaiCreateInstanceBN(model, "IFCMATERIALLAYERSET");

	aggrMaterialLayers = sdaiCreateAggrBN(ifcMaterialLayerSetInstance, "MaterialLayers");
    sdaiAppend((int) aggrMaterialLayers, sdaiINSTANCE, (void*) buildMaterialLayer(thickness));

    return  ifcMaterialLayerSetInstance;
}

int     buildMaterialLayer(double thickness)
{
	int		ifcMaterialLayerInstance;

	ifcMaterialLayerInstance = sdaiCreateInstanceBN(model, "IFCMATERIALLAYER");

	sdaiPutAttrBN(ifcMaterialLayerInstance, "Material", sdaiINSTANCE, (void*) buildMaterial());
	sdaiPutAttrBN(ifcMaterialLayerInstance, "LayerThickness", sdaiREAL, &thickness);

    return  ifcMaterialLayerInstance;
}

int     buildMaterial()
{
	int		ifcMaterialInstance;

	ifcMaterialInstance = sdaiCreateInstanceBN(model, "IFCMATERIAL");

	sdaiPutAttrBN(ifcMaterialInstance, "Name", sdaiSTRING, (void*) "Name of the material used for the wall");

    return  ifcMaterialInstance;
}
