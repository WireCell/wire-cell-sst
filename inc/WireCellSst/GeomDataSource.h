#ifndef WIRECELLSST_GEOMETRYDATASOURCE
#define WIRECELLSST_GEOMETRYDATASOURCE

#include "WireCellNav/GeomDataSource.h"

#include <istream>

namespace WireCellSst {

    /**
       WireCellSst::GeomDataSource - read in a Channel Wire Geometry database.

       
     */

    class GeomDataSource : public WireCell::GeomDataSource {
    public:
	/// Read from an input stream containing content from ChannelWireGeometry.txt
	GeomDataSource(std::istream& data);
	virtual ~GeomDataSource();
    };

} 

#endif
