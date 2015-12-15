#include "WireCellSst/WireSource.h"

#include "WireCellIface/SimpleWire.h"
#include "WireCellIface/WirePlaneId.h"

#include "WireCellUtil/Units.h"

#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

using namespace WireCell;

WireCellSst::WireSource::WireSource(const char* filename)
{
    if (filename) this->load(filename);
}
void WireCellSst::WireSource::load(const char* filename)
{
    std::ifstream geotext(filename);
    this->load(geotext);
}
void WireCellSst::WireSource::load(std::istream& geotext)
{

    const double cm = units::cm;
    
    IWire::vector* wires = new IWire::vector;

    std::string line;
    while (std::getline(geotext, line)) {
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
        assert (iplane >= 0 && iplane <= 3);

	// make up some id scheme
        int ident = (iplane+1)*10000 + index;

	WirePlaneLayer_t layer = iplane2layer[iplane];
	int face = 0, apa = 0, segment = 0; // for now?
	WirePlaneId wpid(layer, face, apa);
	Ray ray(Point(sx*cm,sy*cm,sz*cm),
		Point(ex*cm,ey*cm,ez*cm));

	SimpleWire* wire = new SimpleWire(wpid, ident, index, channel, ray, segment);
	wires->push_back(IWire::pointer(wire));
    }
    m_wires = output_pointer(wires);
}

WireCellSst::WireSource::~WireSource()
{
}

bool WireCellSst::WireSource::operator()(output_pointer& wires)
{
    wires = m_wires;
    return true;
}

