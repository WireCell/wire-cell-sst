#ifndef WIRECELLSST_FRAMESOURCE
#define WIRECELLSST_FRAMESOURCE

#include "WireCellIface/IFrameSource.h"
#include "WireCellSst/RootEvent.h"

class TFile;
class TTree;

namespace WireCellSst {

    class FrameSource : public WireCell::IFrameSource {
    public:
	FrameSource(const char* filename,
		    const char* treepath = "/Event/Sim");
	virtual ~FrameSource();
	virtual bool operator()(WireCell::IFrame::pointer& frame);	
    private:
	WireCellSst::RootEvent m_event;
	TFile* m_tfile;
	TTree* m_tree;
	int m_entry;
    };


}

#endif
