#include "IFCBuilder.h"



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

