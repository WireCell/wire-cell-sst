#include "WireCellSst/FrameDataSource.h"
#include "WireCellData/Trace.h"

#include "TFile.h"
#include "TTree.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 1) {
	std::cerr << "usage: test_dump celltree_file.root [TDirectory/path]" << std::endl;
	return 1;
    }
    const char* root_file = argv[1];

    const char* tpath = "/Event/Sim";
    if (argc > 2) {
	tpath = argv[2];
    }

    std::cerr << "Dumping " << tpath << " from " << root_file << std::endl;


    TFile tfile(root_file,"read");
    TTree* sst = dynamic_cast<TTree*>(tfile.Get(tpath));
    if (!sst) {
	std::cerr << "Failed to get TTree at " << tpath << std::endl;
	return 2;
    }
    WireCellSst::FrameDataSource fds(*sst);
    std::cerr << "Got " << fds.size() 
	      << " frames from " << tpath 
	      << " in " << root_file << std::endl;

    for (size_t iframe=0; iframe<fds.size(); ++iframe) {

	if (fds.jump(iframe) < 0) {
	    std::cerr << "Failed to go to frame " << iframe << std::endl;
	    return 3;
	}

	const WireCell::Frame& frame = fds.get();
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
	
    }


    return 0;

}
