/*
 * IFCBuilderTest.cpp
 *
 *  Created on: Jan 24, 2013
 *      Author: cdp
 */

#include "IFCBuilderTest.h"
#include "IFCBuilder.h"
#include "SimpleMesh.h"
#include "CDP_Building.h"
#include <conio.h>

int main () {

	IFCBuilder* builder = new IFCBuilder();
	CDP_Building* building = new CDP_Building();
	SimpleMesh mesh = SimpleMesh::basicTestMesh();
	building->mesh = &mesh;
	builder->addBuilding(building);
	builder->saveIfcFile("test1.ifc");
	builder->~IFCBuilder();
	std::cout << "wrote ifc data to test1.ifc" << std::endl;
	getch();
}



IFCBuilderTest::IFCBuilderTest() {
	// TODO Auto-generated constructor stub

}

IFCBuilderTest::~IFCBuilderTest() {
	// TODO Auto-generated destructor stub
}

