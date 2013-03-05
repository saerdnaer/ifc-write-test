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
#include "IFCBuilder.h"


void IFCBuilder::createIfcBRepShape(shellStruct* pShell)
{
       sdaiAppend((int) aggrRepresentations, sdaiINSTANCE, (void*) buildShapeRepresentationInstance(pShell));
}


//
//
//		ShapeRepresentation
//
//


int IFCBuilder::buildShapeRepresentationInstance(shellStruct* pShell)
{
	int ifcShapeRepresentationInstance;
	int* aggrItems;

	ifcShapeRepresentationInstance = sdaiCreateInstanceBN(model, "IFCSHAPEREPRESENTATION");

	aggrItems = sdaiCreateAggrBN(ifcShapeRepresentationInstance, "Items");

	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationIdentifier", sdaiSTRING, "Body");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "RepresentationType", sdaiSTRING, "Brep");
	sdaiPutAttrBN(ifcShapeRepresentationInstance, "ContextOfItems", sdaiINSTANCE, (void*) getGeometricRepresentationContextInstance());

    while  (pShell) {
        POLYGON3DSTRUCT  * pPolygon = pShell->pPolygon;
        int     ifcFacetedBrepInstance, ifcClosedShellInstance,* aggrCfsFaces;

	    ifcClosedShellInstance = sdaiCreateInstanceBN(model, "IFCCLOSEDSHELL");
	    aggrCfsFaces = sdaiCreateAggrBN(ifcClosedShellInstance, "CfsFaces");

        while  (pPolygon) {
            VECTOR3DSTRUCT * pVector = pPolygon->pVector;
	        int ifcPolyLoopInstance,* aggrPolygon,
                    ifcFaceOuterBoundInstance,
                    ifcFaceInstance,* aggrBounds;

	        ifcPolyLoopInstance = sdaiCreateInstanceBN(model, "IFCPOLYLOOP");
	        aggrPolygon = sdaiCreateAggrBN(ifcPolyLoopInstance, "Polygon");

            while  (pVector) {
                POINT3DSTRUCT  * pPoint = pVector->pPoint;
                //
                //  Check if point is already written
                //
                if  (! pPoint->ifcCartesianPointInstance) {
                    pPoint->ifcCartesianPointInstance = buildCartesianPointInstance(pPoint->x, pPoint->y, pPoint->z);
                }
                            
                sdaiAppend((int) aggrPolygon, sdaiINSTANCE, (void *) pPoint->ifcCartesianPointInstance);

                pVector = pVector->next;
            }

	        ifcFaceOuterBoundInstance = sdaiCreateInstanceBN(model, "IFCFACEOUTERBOUND");
	        sdaiPutAttrBN(ifcFaceOuterBoundInstance, "Bound", sdaiINSTANCE, (void *) ifcPolyLoopInstance);
            sdaiPutAttrBN(ifcFaceOuterBoundInstance, "Orientation", sdaiENUM, "T");

	        ifcFaceInstance = sdaiCreateInstanceBN(model, "IFCFACE");
	        aggrBounds = sdaiCreateAggrBN(ifcFaceInstance, "Bounds");
	        sdaiAppend((int) aggrBounds, sdaiINSTANCE, (void *) ifcFaceOuterBoundInstance);

	        sdaiAppend((int) aggrCfsFaces, sdaiINSTANCE, (void *) ifcFaceInstance);

            if  (pPolygon->pOpeningVector) {
	            int ifcFaceBoundInstance;

                pVector = pPolygon->pOpeningVector;

	            ifcPolyLoopInstance = sdaiCreateInstanceBN(model, "IFCPOLYLOOP");
	            aggrPolygon = sdaiCreateAggrBN(ifcPolyLoopInstance, "Polygon");

                while  (pVector) {
                    POINT3DSTRUCT  * pPoint = pVector->pPoint;
                    //
                    //  Check if point is already written
                    //
                    if  (! pPoint->ifcCartesianPointInstance) {
                        pPoint->ifcCartesianPointInstance = buildCartesianPointInstance(pPoint->x, pPoint->y, pPoint->z);
                    }
                            
                    sdaiAppend((int) aggrPolygon, sdaiINSTANCE, (void *) pPoint->ifcCartesianPointInstance);

                    pVector = pVector->next;
                }

	            ifcFaceBoundInstance = sdaiCreateInstanceBN(model, "IFCFACEBOUND");
	            sdaiPutAttrBN(ifcFaceBoundInstance, "Bound", sdaiINSTANCE, (void *) ifcPolyLoopInstance);
                sdaiPutAttrBN(ifcFaceBoundInstance, "Orientation", sdaiENUM, "T");

	            sdaiAppend((int) aggrBounds, sdaiINSTANCE, (void *) ifcFaceBoundInstance);
            }

            pPolygon = pPolygon->next;
        }

	    ifcFacetedBrepInstance = sdaiCreateInstanceBN(model, "IFCFACETEDBREP");
	    sdaiPutAttrBN(ifcFacetedBrepInstance, "Outer", sdaiINSTANCE, (void *) ifcClosedShellInstance);
	
        sdaiAppend((int) aggrItems, sdaiINSTANCE, (void*) ifcFacetedBrepInstance);

        pShell = pShell->next;
    }

	return	ifcShapeRepresentationInstance;
}


