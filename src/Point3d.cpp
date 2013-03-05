#include "Point3d.h"

Point3d::Point3d():x(0.0),y(0.0),z(0.0){
}

Point3d::Point3d(double x_, double y_, double z_):x(x_),y(y_),z(z_){
}

Point3d Point3d::operator + (const Point3d& r) const{
	return Point3d(this->x + r.x, this->y + r.y, this->z + r.z);
}

Point3d Point3d::operator - (const Point3d& r) const{
	return Point3d(this->x - r.x, this->y - r.y, this->z - r.z);
}

Point3d Point3d::operator - (void) const{
	return Point3d(-this->x, -this->y, -this->z);
}

Point3d Point3d::operator * (const double& r) const{
	return Point3d(this->x * r, this->y * r, this->z * r);
}

Point3d Point3d::operator / (const double& r) const{
	return Point3d(this->x / r, this->y / r, this->z / r);
}

bool Point3d::operator == (const Point3d& r) const{
	return this->x == r.x && this->y == r.y && this->z == r.z;
}

double Point3d::operator() (const int& x) const{
	return this->vec[x];
}

double& Point3d::operator() (const int& x){
	return this->vec[x];
}

std::ostream& operator<<(std::ostream& os, const Point3d& p)
{
    os << "( " << p(0) << ", " << p(1) << ", " << p(2) << " )";
    return os;
}
