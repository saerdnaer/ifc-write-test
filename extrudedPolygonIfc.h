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


#include "boundingBoxIfc.h"


typedef struct POINT2DSTRUCT {
	double			x;
	double			y;
}	point2DStruct;

typedef struct POLYGON2DSTRUCT {
	POINT2DSTRUCT   * pPoint;
	POLYGON2DSTRUCT * next;
}	polygon2DStruct;



void    createIfcExtrudedPolygonShape(polygon2DStruct * pPolygon, double depth);
void    createIfcPolylineShape(double p0x, double p0y, double p1x, double p1y);


//
//
//		ShapeRepresentation
//
//


int		buildShapeRepresentationInstance(polygon2DStruct * pPolygon, double depth);
int		buildShapeRepresentationInstance(double p0x, double p0y, double p1x, double p1y);


//
//
//		ArbitraryClosedProfileDef, CartesianPoint(2D), ExtrudedAreaSolid, Polyline
//
//


int		buildArbitraryClosedProfileDefInstance(polygon2DStruct * pPolygon);
int		buildCartesianPointInstance(double x, double y);
int		buildExtrudedAreaSolidInstance(polygon2DStruct * pPolygon, double depth);
int		buildPolylineInstance(polygon2DStruct * pPolygon);
int		buildPolylineInstance(double p0x, double p0y, double p1x, double p1y);
