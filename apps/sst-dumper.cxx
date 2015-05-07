#include "WireCellNav/FrameDataSource.h"
#include "WireCellNav/SimDataSource.h"
#include "WireCellSst/Util.h"
#include "WireCellData/Trace.h"
#include "WireCellData/SimTruth.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 1) {
	std::cerr << "usage: test_dump celltree_file.root [TDirectory/path]" << std::endl;
	return 1;
    }
    const char* root_file_name = argv[1];

    const char* tpath = "/Event/Sim";
    if (argc > 2) {
	tpath = argv[2];
    }

    // Note, we could cheat and just make an SST version of the FDS
    // and then get direct access to its SDS mixing facet, but let's
    // pretend to do things proper.
    //WireCellSst::FrameDataSource* fds = 0;

    //
    WireCell::FrameDataSource* fds = 0;

    fds = WireCellSst::make_fds(root_file_name);
    if (!fds) {
	std::cerr << "ERROR: failed to get FDS from " << root_file_name << std::endl;
	return 1;
    }

    WireCell::SimDataSource* sds = dynamic_cast<WireCell::SimDataSource*>(fds);
    if (!sds) {
	std::cerr << "ERROR: the FDS is not also an SDS " << std::endl;
	return 2;
    }

    std::cerr << "Got " << fds->size() 
	      << " frames from " << tpath 
	      << " in " << root_file_name << std::endl;

    for (size_t iframe=0; iframe<fds->size(); ++iframe) {

	if (fds->jump(iframe) < 0) {
	    std::cerr << "Failed to go to frame " << iframe << std::endl;
	    return 3;
	}

	const WireCell::Frame& frame = fds->get();
	std::cerr << "Frame " << iframe << " has " << frame.traces.size() << " traces" << std::endl;

	for (size_t itrace=0; itrace < frame.traces.size(); ++itrace) {
	    const WireCell::Trace& trace = frame.traces[itrace];

	    std::cerr << "Frame: " << iframe
		      << " Trace:" << itrace
		      << " chid:" << trace.chid
		      << " tbin:" << trace.tbin
		      << " #bins:" << trace.charge.size()
		      << std::endl;

	}
	
	WireCell::SimTruthSelection sts = sds->truth();
	std::cerr << "Got " << sts.size() << " true hits" << std::endl;
	for (size_t itruth = 0; itruth < sts.size(); ++itruth) {
	    const WireCell::SimTruth* st = sts[itruth];
	    std::cerr << "Hit: "
		      << " @ (" << st->x() << " " << st->y() << " " << st->z() << ")"
		      << " q=" << st->charge()
		      << " tdc=" << st->tdc()
		      << std::endl;
	}

    } // frames


    return 0;

}
