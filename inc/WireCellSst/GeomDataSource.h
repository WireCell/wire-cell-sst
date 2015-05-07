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
	GeomDataSource(const char* filename=0);
	virtual ~GeomDataSource();

	void load(std::istream& data);


    };

} 

#endif
