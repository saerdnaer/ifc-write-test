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


#include "extrudedPolygonIfc.h"


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


void    createIfcBRepShape(shellStruct * pShell);


//
//
//		ShapeRepresentation
//
//


int		buildShapeRepresentationInstance(shellStruct * pShell);
