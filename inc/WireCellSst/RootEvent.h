#ifndef WIRECELLSST_ROOTEVENT_H
#define WIRECELLSST_ROOTEVENT_H

#include "TTree.h"
#include "TClonesArray.h"
#include <vector>

namespace WireCellSst{

    struct RootEvent {
	RootEvent();
	~RootEvent();
	

	int number;
	int run;
	int subrun;
	int nchannels;
	std::vector<int> *channelid;
	TClonesArray* signal;

	void clear();
	void graphf(TTree& tree);
	
    };
}

#endif
