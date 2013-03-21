/*
 * IFCBuilderTest.cpp
 *
 *  Created on: Jan 24, 2013
 *      Author: cdp
 */

#include "IFCBuilderTest.h"
#include "IFCBuilder.h"
#include "SimpleMesh.h"
#include "CDP_Building.hpp"

int main () {

	IFCBuilder* builder = new IFCBuilder();
	CDP_Building building = new CDP_Building();
	building->mesh = SimpleMesh::basicTestMesh();
	builder->addBuilding(building);
	builder->addHeader();
	builder->saveIfcFile("test1.ifc");
	builder->~IFCBuilder();
}



IFCBuilderTest::IFCBuilderTest() {
	// TODO Auto-generated constructor stub

}

IFCBuilderTest::~IFCBuilderTest() {
	// TODO Auto-generated destructor stub
}

