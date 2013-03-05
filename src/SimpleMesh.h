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
	vector<vector<int> > polygons;

	SimpleMesh();
    typedef boost::shared_ptr<SimpleMesh> Ptr;
    typedef boost::shared_ptr<const SimpleMesh> ConstPtr;

	bool isEmpty() const;
	// returns a simple mesh consisting of 3 triangles intersecting at the origin point 0,0,0 and each one lying in on of the natural coordinate frame's planes
	/*
	 *  *
                             ***
                             ** **
                            * *   *
                            * *    *
                           *  *     *
                          *   *      *
                          *   *       **
                         *    *         *
                         *    *          *
                        *     *           *
                        *     *            *
                       *      ******        **
                       *      *     ********  *
                      *     **              ****
                      *    *             ****
                     *    *          ****
                    *   **       ****
                    *  *     ****
                   *  *  ****
                   ******
                   **
	 */
	static SimpleMesh basicTestMesh();
};


#endif
