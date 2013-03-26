/*
 * Point3d.h
 *
 *      Author: Sebastian Riedel
 */

#ifndef __SIMPLEMESH_H__
#define __SIMPLEMESH_H__

#include <vector>
#include "Point3d.h"
#include <boost/shared_ptr.hpp>

using namespace std;

/*
 * Simple mesh class based on a polygon list
 */

class SimpleMesh
{
public:
	vector<Point3d> vertices;
	typedef vector<int> polygon_type;
	vector<polygon_type> polygons;

	SimpleMesh();
    typedef boost::shared_ptr<SimpleMesh> Ptr;
    typedef boost::shared_ptr<const SimpleMesh> ConstPtr;

	bool isEmpty() const;

	static SimpleMesh basicTestMesh();
};


#endif
