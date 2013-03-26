#include "SimpleMesh.h"

SimpleMesh::SimpleMesh(){
}



	// returns a simple mesh consisting of 3 triangles intersecting at the origin point 0,0,0 and each one lying in on of the natural coordinate frame's planes
	/*
	 *  *                     3 
                             ***
                             ** **
                            * *   *
                            * *    *
                           *  *     *
                          *   *      *
                          *   *       **         t1
                         *    *         *
                         *    *    t4    *
                        * t3  *           *
                        *     *            *
                       *    2 ******        **
                       *      *     ********  *
                      *     **              ****  1
                      *    *    t2       ****
                     *    *          ****
                    *   **       ****
                    *  *     ****
                   *  *  ****
                   ******
                 0 **
	 */

SimpleMesh SimpleMesh::basicTestMesh(){
	SimpleMesh mesh;

	mesh.vertices.push_back(Point3d(0,0,0));
	mesh.vertices.push_back(Point3d(100,0,0));
	mesh.vertices.push_back(Point3d(0,100,0));
	mesh.vertices.push_back(Point3d(0,0,100));

	vector<int> triangle1;
	vector<int> triangle2;
	vector<int> triangle3;
	vector<int> triangle4;

	triangle1.push_back(0); // triangles defined counter-clockwise
	triangle1.push_back(1);
	triangle1.push_back(3);

	triangle2.push_back(0); // triangles defined counter-clockwise
	triangle2.push_back(1);
	triangle2.push_back(2);

	triangle3.push_back(0); // triangles defined counter-clockwise
	triangle3.push_back(2);
	triangle3.push_back(3);

	triangle4.push_back(1); // triangles defined counter-clockwise
	triangle4.push_back(2);
	triangle4.push_back(3);

	mesh.polygons.push_back(triangle1);
	mesh.polygons.push_back(triangle2);
	mesh.polygons.push_back(triangle3);
	mesh.polygons.push_back(triangle4);

	return mesh;
}

bool SimpleMesh::isEmpty() const{
	return vertices.size() == 0;
}
