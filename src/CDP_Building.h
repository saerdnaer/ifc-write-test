#pragma once
//#include <QtCore/qobject.h>
#include <boost/shared_ptr.hpp>
//#include "sBuilding.h"
//#include "Building.hpp"
#include "SimpleMesh.h"

class CDP_Building // : public QObject
{
	//Q_OBJECT

public:
	typedef boost::shared_ptr<CDP_Building> ptr;

	//boost::shared_ptr<sBuilding> simple;
	//boost::shared_ptr<PrsBuilding> presentation;

	CDP_Building();
	~CDP_Building();

	int id;
	int getId();

	float height;
	float getHeight();

	SimpleMesh* mesh;
	SimpleMesh* getMesh();

	Point3d getPosition();

};

