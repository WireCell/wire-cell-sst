#include "WireCellNav/FrameDataSource.h"
#include "WireCellNav/SliceDataSource.h"
#include "WireCellNav/SimDataSource.h"
#include "WireCellNav/GenerativeFDS.h"
#include "WireCellNav/SimTruthDepositor.h"

#include "WireCellSst/Util.h"
#include "WireCellSst/GeomDataSource.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2) {
	std::cerr << "usage: sst-depo-example geom_file.txt celltree_file.root" << std::endl;
	return 1;
    }
    const char* geom_file_name = argv[1];
    WireCellSst::GeomDataSource gds(geom_file_name);

    const char* root_file_name = argv[2];
    WireCell::FrameDataSource* fds1 = WireCellSst::make_fds(root_file_name);
    if (!fds1) {
	std::cerr << "ERROR: failed to get FDS from " << root_file_name << std::endl;
	return 1;
    }
    WireCell::SimDataSource* sim = dynamic_cast<WireCell::SimDataSource*>(fds1);
    if (!sim) {
	std::cerr << "ERROR: the FDS is not also an SDS " << std::endl;
	return 2;
    }

    WireCell::SimTruthDepositor std(*sim);
    WireCell::GenerativeFDS *fds2 = new WireCell::GenerativeFDS(std, gds, 9600);
    
    WireCell::SliceDataSource sds1(*fds1);
    WireCell::SliceDataSource sds2(*fds2);


    fds1->jump(0);
    fds2->jump(0);

    if (sds1.size() != sds2.size()) {
	cerr << "Size mismatch:" << endl
	     << "sds1 size=" << sds1.size() << endl
	     << "sds2 size=" << sds2.size() << endl;
	return 1;
    }

    for (int ind=0; ind<sds1.size(); ++ind) {
	if (sds1.jump(ind) < 0) {
	    cerr << "sds1(" << ind << ") failed" << endl;
	    return 2;
	}
	if (sds2.jump(ind) < 0) {
	    cerr << "sds2(" << ind << ") failed" << endl;
	    return 2;
	}

	const WireCell::Slice& slice1 = sds1.get();
	const WireCell::Slice& slice2 = sds2.get();

	if (slice1.tbin() != slice2.tbin()) {
	    cerr << "Slice time bin mismatch:" << endl
		 << "sds1 @ " << slice1.tbin() << endl
		 << "sds2 @ " << slice2.tbin() << endl;
	    return 3;
	}

	// vector<pair<int,float>>
	const WireCell::Channel::Group& cg1 = slice1.group();
	const WireCell::Channel::Group& cg2 = slice2.group();

	if (cg1.size() != cg2.size()) {
	    cerr << "Channel::Group size mismatch:" << endl
		 << "sds1 has " << cg1.size() << endl
		 << "sds2 has " << cg2.size() << endl;
	    return 4;
	}


	float qtot1=0, qtot2=0;
	for (int ich=0; ich<cg1.size(); ++ich) {
	    qtot1 += cg1[ich].second;
	    qtot2 += cg2[ich].second;
	}

	if (fabs(qtot1-qtot2) > 0.001) {
	    cerr << "Total charge mismatch:" << endl
		 << "qtot1=" << qtot1 << endl
		 << "qtot2=" << qtot2 << endl;
	    return 5;
	}

	cerr << "slice " << ind << " has " << cg1.size() << " hit wires with charge " << qtot1 << endl;

    }

    return 0;
}

