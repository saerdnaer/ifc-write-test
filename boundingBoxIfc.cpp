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


#include "IFCBuilder.h"

/*
extern  int     * aggrRelatedElements,
                * aggrRepresentations;

extern  int     ifcBuildingStoreyInstancePlacement;
*/

void    IFCBuilder::createIfcBoundingBoxShape(double width, double thickness, double height, char * representationIdentifier)
{
    sdaiAppend((int) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(width, thickness, height, representationIdentifier));
}


//
//
//		BoundingBox, ShapeRepresentation
//
//


int IFCBuilder::buildBoundingBoxInstance(double width, double thickness, double height)
{
	int ifcBoundingBoxInstance;

	ifcBoundingBoxInstance = sdaiCreateInstanceBN(model, "IFCBOUNDINGBOX");

	sdaiPutAttrBN(ifcBoundingBoxInstance, "Corner", sdaiINSTANCE, (void*) buildCartesianPointInstance(0, 0, 0));
	sdaiPutAttrBN(ifcBoundingBoxInstance, "XDim", sdaiREAL, &width);
	sdaiPutAttrBN(ifcBoundingBoxInstance, "YDim", sdaiREAL, &thickness);
	sdaiPutAttrBN(ifcBoundingBoxInstance, "ZDim", sdaiREAL, &height);

	return	ifcBoundingBoxInstance;
}

int IFCBuilder::buildShapeRepresentationInstance(double width, double thickness, double height, char * representationIdentifier)
{
	int ifcShapeRepresentationInstance, * aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

	sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());
	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, representationIdentifier);
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, "BoundingBox");
    sdaiAppend((int) aggrItems, sdaiINSTANCE, (void*) buildBoundingBoxInstance(width, thickness, height));

	return	ifcShapeRepresentationInstance;
}


//
//
//      CartesianPoint(3D)
//
//



int IFCBuilder::buildCartesianPointInstance(double x, double y, double z)
{
	int ifcCartesianPointInstance, * aggrCoordinates;

	ifcCartesianPointInstance = sdaiCreateInstanceBN(model, "IFCCARTESIANPOINT");

	aggrCoordinates = sdaiCreateAggrBN(ifcCartesianPointInstance, "Coordinates");
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &x);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &y);
	sdaiAppend((int) aggrCoordinates, sdaiREAL, &z);

	return	ifcCartesianPointInstance;
}