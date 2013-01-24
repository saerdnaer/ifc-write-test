#if _MSC_VER > 1000
#pragma once
#endif 



#include "baseIfc.h"



class IFCExport
{
public:
	IFCExport();
	~IFCExport();
	void export_ifc();

private:
	IFCBuilder* builder;
	char* ifcSchemaName, * ifcFileName;
	bool saveIfx;
	int view;
};

