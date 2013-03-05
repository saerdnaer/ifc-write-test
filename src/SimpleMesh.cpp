#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(){
}

SimpleMesh SimpleMesh::basicTestMesh(){
	SimpleMesh mesh;

	mesh.vertices.push_back(Point3d(0,0,0));
	mesh.vertices.push_back(Point3d(1,0,0));
	mesh.vertices.push_back(Point3d(0,1,0));
	mesh.vertices.push_back(Point3d(0,0,1));

	vector<int> triangle1;
	vector<int> triangle2;
	vector<int> triangle3;

	triangle1.push_back(0); // triangles defined counter-clockwise
	triangle1.push_back(1);
	triangle1.push_back(3);

	triangle2.push_back(0); // triangles defined counter-clockwise
	triangle2.push_back(1);
	triangle2.push_back(2);

	triangle3.push_back(0); // triangles defined counter-clockwise
	triangle3.push_back(2);
	triangle3.push_back(3);

	mesh.polygons.push_back(triangle1);
	mesh.polygons.push_back(triangle2);
	mesh.polygons.push_back(triangle3);

	return mesh;
}

bool SimpleMesh::isEmpty() const{
	return vertices.size() == 0;
}
