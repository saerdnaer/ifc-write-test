/*
 * Point3d.h
 *
 *      Author: Sebastian Riedel
 */

#ifndef __POINT3D_H__
#define __POINT3D_H__

#include <ostream>

/*
 * Simple point class representing 3d point and providing basic arithmetic operations
 */
class Point3d
{
public:
	union
	    {
	        double vec[3];
	        struct
	        {
	             double x;
	             double y;
	             double z;
	        };
	    };

	Point3d();
	Point3d(double x_, double y_, double z_);

	// overloaded operators
	// element access
	// +
	// - and negation
	// * with scalar
	// / with scalar
	// ==
	// <<

	Point3d operator + (const Point3d& r) const;
	Point3d operator - (const Point3d& r) const;
	Point3d operator - (void) const;
	Point3d operator * (const double& r) const;
	Point3d operator / (const double& r) const;
	bool operator == (const Point3d& r) const;

	double operator() (const int& x) const;
	double& operator() (const int& x);

	friend std::ostream& operator<<(std::ostream& os, const Point3d& p);

};




#endif
