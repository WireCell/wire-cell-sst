#ifndef WIRECELLSST_WIRESOURCE
#define WIRECELLSST_WIRESOURCE

#include "WireCellIface/IWireSource.h"
#include "WireCellSst/RootEvent.h"

#include <istream>

class TFile;
class TTree;

namespace WireCellSst {

    class WireSource : public WireCell::IWireSource {
    public:
	WireSource(const char* filename=0);
	void load(const char* filename);
	void load(std::istream& geotext);
	virtual ~WireSource();
	virtual bool extract(output_type& wires);	
    private:
	output_type m_wires;
    };


}

#endif
