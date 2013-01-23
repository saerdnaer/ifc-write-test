#if _MSC_VER > 1000
#pragma once
#endif 



#include "BRepIfc.h"

class IFCExport
{
public:
	IFCExport();
	~IFCExport();
	void export_ifc();
	void genTimestamp(char *timeStamp);

private:
	BaseIFC* baseIFC;
	char* ifcSchemaName, * ifcFileName;
	bool saveIfx;
	int view;
}

#define     COORDINATIONVIEW    0
#define     PRESENTATIONVIEW    1