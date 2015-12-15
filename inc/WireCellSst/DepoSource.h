#ifndef WIRECELLSST_DEPOSOURCE
#define WIRECELLSST_DEPOSOURCE

#include "WireCellIface/IDepoSource.h"
#include "WireCellSst/RootSimDepo.h"
#include "WireCellSst/Reader.h"

namespace WireCellSst {

    typedef Reader<RootSimDepo> DepoReader;

    class DepoSource : public WireCell::IDepoSource {
    public:
	DepoSource(const char* filename,
		   const char* treepath = "/Event/Sim");
	virtual ~DepoSource();
	virtual bool operator()(WireCell::IDepo::pointer& depo);	
    private:
	DepoReader* m_reader;
	const RootSimDepo* m_cache;
	int m_index;
	int m_count;
    };


}

#endif
