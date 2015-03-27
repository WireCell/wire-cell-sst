#ifndef WIRECELLSST_GEOMETRYDATASOURCE
#define WIRECELLSST_GEOMETRYDATASOURCE

#include "WireCellNav/GeomDataSource.h"

namespace WireCellSst {

    class GeomDataSource : public WireCell::GeomDataSource {
    public:
	GeomDataSource(const char* geometry_filename);
	virtual ~GeomDataSource();
    };

} 

#endif
