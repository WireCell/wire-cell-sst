#ifndef WIRECELLSST_FRAMESOURCE
#define WIRECELLSST_FRAMESOURCE

#include "WireCellIface/IFrameSource.h"
#include "WireCellIface/IConfigurable.h"
#include "WireCellSst/RootEvent.h"

class TFile;
class TTree;

namespace WireCellSst {

    class FrameSource : public WireCell::IFrameSource, public WireCell::IConfigurable {
    public:
	FrameSource();
	FrameSource(const char* filename,
		    const char* treepath = "/Event/Sim",
		    const char* source = "calib");// or "raw"
	virtual ~FrameSource();

	void open(const char* filename,
		  const char* treepath = "/Event/Sim",
		  const char* source = "calib");
	void close();

	virtual bool operator()(WireCell::IFrame::pointer& frame);	

	/// WireCell::IConfigurable interface.
	virtual void configure(const WireCell::Configuration& config);
	virtual WireCell::Configuration default_configuration() const;

    private:
	WireCellSst::RootEvent m_event;
	TFile* m_tfile;
	TTree* m_tree;
	int m_entry;
    };


}

#endif
