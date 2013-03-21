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

#include "IFCBuilder.h"


void IFCBuilder::createIfcExtrudedPolygonShape(polygon2DStruct * pPolygon, double depth)
{
    sdaiAppend((int) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(pPolygon, depth));
}

void IFCBuilder::createIfcPolylineShape(double p0x, double p0y, double p1x, double p1y)
{
    sdaiAppend((int) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(p0x, p0y, p1x, p1y));
}


//
//
//		ShapeRepresentation
//
//


int IFCBuilder::buildShapeRepresentationInstance(polygon2DStruct * pPolygon, double depth)
{
	int ifcShapeRepresentationInstance, * aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
	sdaiAppend((int) aggrItems, sdaiINSTANCE, (void*) buildExtrudedAreaSolidInstance(pPolygon, depth));

	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, "Body");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, "SweptSolid");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());

	return	ifcShapeRepresentationInstance;
}

int IFCBuilder::buildShapeRepresentationInstance(double p0x, double p0y, double p1x, double p1y)
{
	int ifcShapeRepresentationInstance, * aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
	sdaiAppend((int) aggrItems, sdaiINSTANCE, (void*) buildPolylineInstance(p0x, p0y, p1x, p1y));

	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, "Axis");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, "Curve2D");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());

	return	ifcShapeRepresentationInstance;
}


//
//
//		ArbitraryClosedProfileDef, CartesianPoint(2D), ExtrudedAreaSolid, Polyline
//
//


int IFCBuilder::buildArbitraryClosedProfileDefInstance(polygon2DStruct * pPolygon)
{
	int ifcArbitraryClosedProfileDefInstance;

	ifcArbitraryClosedProfileDefInstance = sdaiCreateInstanceBN(model, "IFCARBITRARYCLOSEDPROFILEDEF");

	sdaiPutAttrBN(ifcArbitraryClosedProfileDefInstance, "ProfileType", sdaiENUM, "AREA");
	sdaiPutAttrBN(ifcArbitraryClosedProfileDefInstance, "OuterCurve", sdaiINSTANCE, (void*) buildPolylineInstance(pPolygon));

	return	ifcArbitraryClosedProfileDefInstance;
}

int IFCBuilder::buildCartesianPointInstance(double x, double y)
{
	int ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &x);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &y);

	return	ifcCartesianPointInstance;
}

int IFCBuilder::buildExtrudedAreaSolidInstance(polygon2DStruct * pPolygon, double depth)
{
	transformationMatrixStruct  matrix;
	int ifcExtrudedAreaSolidInstance;

	IFCBuilder::identityMatrix(&matrix);

	ifcExtrudedAreaSolidInstance = sdaiCreateInstanceBN(model, "IFCEXTRUDEDAREASOLID");

	sdaiPutAttrBN(ifcExtrudedAreaSolidInstance, "SweptArea", sdaiINSTANCE, (void*) buildArbitraryClosedProfileDefInstance(pPolygon));
	sdaiPutAttrBN(ifcExtrudedAreaSolidInstance, "Position", sdaiINSTANCE, (void*) buildAxis2Placement3DInstance(&matrix));
	sdaiPutAttrBN(ifcExtrudedAreaSolidInstance, "ExtrudedDirection", sdaiINSTANCE, (void*) buildDirectionInstance((point3DStruct *) &matrix._31));
	sdaiPutAttrBN(ifcExtrudedAreaSolidInstance, "Depth", sdaiREAL, (void *) &depth);

	return	ifcExtrudedAreaSolidInstance;
}

int IFCBuilder::buildPolylineInstance(polygon2DStruct * pPolygon)
{
	int ifcPolylineInstance, * aggrPoints;

	ifcPolylineInstance = sdaiCreateInstanceBN(model, "IFCPOLYLINE");

	aggrPoints = sdaiCreateAggrBN(ifcPolylineInstance, "Points");
    double  x = pPolygon->pPoint->x,
            y = pPolygon->pPoint->y;
    while  (pPolygon) {
	    sdaiAppend((int) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(pPolygon->pPoint->x, pPolygon->pPoint->y));

        pPolygon = pPolygon->next;
    }
	sdaiAppend((int) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(x, y));

	return	ifcPolylineInstance;
}

int IFCBuilder::buildPolylineInstance(double p0x, double p0y, double p1x, double p1y)
{
	int ifcPolylineInstance, * aggrPoints;

	ifcPolylineInstance = sdaiCreateInstanceBN(model, "IFCPOLYLINE");

	aggrPoints = sdaiCreateAggrBN(ifcPolylineInstance, "Points");
	sdaiAppend((int) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(p0x, p0y));
	sdaiAppend((int) aggrPoints, sdaiINSTANCE, (void*) buildCartesianPointInstance(p1x, p1y));

	return	ifcPolylineInstance;
}
