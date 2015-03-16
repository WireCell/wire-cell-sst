#ifndef WIRECELLSST_ROOTEVENT_H
#define WIRECELLSST_ROOTEVENT_H

#include <vector>
class TClonesArray;

namespace WireCellSst{

    struct RootEvent {
	int number;
	int run;
	int subrun;
	int nchannels;
	std::vector<int> *channelid;
	TClonesArray* signal;
    };
}

#endif
