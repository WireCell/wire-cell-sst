#include "WireCellSst/GeomWireReader.h"
#include "WireCellNav/GeomDataSource.h" // fixme, break dependency on nav
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    WireCellSst::GeomWireReader reader(argv[1]);
    WireCell::GeomDataSource gds;
    gds.use_wires(reader);

    std::vector<double> ex = gds.extent();
    cerr << "Extent: "
	 << " x:" << ex[0]/units::mm << " mm"
	 << " y:" << ex[1]/units::m << " m"
	 << " z:" << ex[2]/units::m << " m"
	 << endl;

    return 0;
}
