#include "WireCellSst/GeomDataSource.h"

#include <fstream>
#include <string>
#include <sstream>

WireCellSst::GeomDataSource::GeomDataSource(const char* geometry_filename)
{
    std::ifstream geo(geometry_filename);
    if (!geo.is_open()) { 
	return; 
    }

    WireCell::Wire wire;
    std::string line;
    while (std::getline(geo, line)) {
	if (line[0] == '#') {
	    continue;
	}

	std::istringstream iss(line);

	float sx, sy, sz, ex, ey, ez;
	int plane;
	iss >> wire.channel >> plane >> wire.index
	    >> sx >> sy >> sz >> ex >> ey >> ez;
	wire.plane = static_cast<WireCell::WirePlaneType_t>(plane);
	wire.point1 = WireCell::Point(sx,sy,sz);
	wire.point2 = WireCell::Point(ex,ey,ez);
	this->add_wire(wire);
    }
}
	
WireCellSst::GeomDataSource::~GeomDataSource()
{
}

