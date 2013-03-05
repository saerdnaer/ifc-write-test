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
#include "IFCExport.h"


IFCExport::IFCExport() {
	builder = new IFCBuilder(); 
	saveIfx = false;
}

// based on CMiniExampleDlg::OnOK() --Andreas
void IFCExport::export_ifc() 
{
    char ifcFileName[] = "building_test.ifc", 
		ifcSchemaName[] = "IFC2X3_Final.exp";

    if  (!builder->createIfcFile(ifcSchemaName, false)) 
	{
        std::cout << ("IFC Model could not be instantiated, probably it cannot find the schema file.");
    }

    //
    //  Update header
    //

    char description[512], timeStamp[512];

    if  (view == COORDINATIONVIEW) {
        memcpy(description, "ViewDefinition [CoordinationView]", sizeof("ViewDefinition [CoordinationView]"));
    } else {
        //ASSERT(view == PRESENTATIONVIEW);
        memcpy(description, "ViewDefinition [PresentationView]", sizeof("ViewDefinition [PresentationView]"));
    }

	builder->genIfcTimestamp(timeStamp);

    int i = 0, j = 0;
    while  (ifcFileName[i]) {
        if  (ifcFileName[i++] == '\\') {
            j = i;
        }
    }

    SetSPFFHeader(
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

    if  (saveIfx) {
        builder->saveIfcFileAsXml(ifcFileName);
    } else {
        builder->saveIfcFile(ifcFileName);
    }
}




