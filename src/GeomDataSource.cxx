#include "WireCellSst/GeomDataSource.h"
#include "WireCellData/Units.h"

#include <string>
#include <sstream>
#include <cassert>
#include <fstream>

using namespace units;


WireCellSst::GeomDataSource::GeomDataSource(const char* filename)
    : WireCell::GeomDataSource()
{
    if (filename) {
	std::ifstream geotext(filename);
	this->load(geotext);
    }
}

void WireCellSst::GeomDataSource::load(std::istream& geo)
{

    std::string line;
    while (std::getline(geo, line)) {
	if (! line.size()) {
	    continue;
	}
	if (line[0] == '#') {
	    continue;
	}

	std::istringstream iss(line);

	float sx, sy, sz, ex, ey, ez;
	int plane;
	WireCell::Wire wire;
	iss >> wire.channel >> plane >> wire.index
	    >> sx >> sy >> sz >> ex >> ey >> ez;

	assert (wire.index >= 0);

	wire.ident = (plane+1)*10000 + wire.index;
	wire.plane = static_cast<WireCell::WirePlaneType_t>(plane);
	wire.point1 = WireCell::Point(sx*cm,sy*cm,sz*cm);
	wire.point2 = WireCell::Point(ex*cm,ey*cm,ez*cm);
	this->add_wire(wire);
    }
}
	
WireCellSst::GeomDataSource::~GeomDataSource()
{
}

