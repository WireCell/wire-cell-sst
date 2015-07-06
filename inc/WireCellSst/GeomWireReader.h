#ifndef WIRECELLSST_GEOMWIREREADER
#define WIRECELLSST_GEOMWIREREADER

#include "WireCellIface/IWireGeometry.h"

#include <istream>

namespace WireCellSst {

    /**
       WireCellSst::GeomWireReader - read in a Channel Wire Geometry database.
     */


    class GeomWireReader : public WireCell::IWireGeometry {
	const char* filename;
        mutable WireCell::GeomWireSet wires;
    public:
	/// Read from an input stream containing content from ChannelWireGeometry.txt
	GeomWireReader(const char* filename=0);

	/// Load from some text stream instead of a filename
	void load(std::istream& geo) const;

	virtual ~GeomWireReader();

        virtual const WireCell::GeomWireSet& get_wires() const;
    };
} 

#endif
