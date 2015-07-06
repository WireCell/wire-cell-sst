#include "WireCellSst/GeomWireReader.h"
#include "WireCellData/Units.h"

#include <string>
#include <sstream>
#include <cassert>
#include <fstream>

using namespace units;

WireCellSst::GeomWireReader::GeomWireReader(const char* filename)
    : filename(filename)
{
}


const WireCell::GeomWireSet& WireCellSst::GeomWireReader::get_wires() const
{
    if (wires.size()) {
	return wires;
    }

    std::ifstream geotext(filename);
    this->load(geotext);
    return wires;
}

void WireCellSst::GeomWireReader::load(std::istream& geo) const
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
	int iplane, channel, index;

	iss >> channel >> iplane >> index
	    >> sx >> sy >> sz >> ex >> ey >> ez;
	assert (index >= 0);

	int ident = (iplane+1)*10000 + index;
	WireCell::WirePlaneType_t plane = static_cast<WireCell::WirePlaneType_t>(iplane);

	wires.insert(WireCell::GeomWire(ident, plane, index, channel,
					WireCell::Point(sx*cm,sy*cm,sz*cm),
					WireCell::Point(ex*cm,ey*cm,ez*cm)));
    }
}
	
WireCellSst::GeomWireReader::~GeomWireReader()
{
}

