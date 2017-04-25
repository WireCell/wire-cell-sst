#include "WireCellSst/FrameSource.h"
#include "WireCellUtil/Testing.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>

using namespace WireCell;
using namespace std;

const string url_alpha = "http://www.phy.bnl.gov/wire-cell/examples/celltree/3.0/celltree_alpha-new.root";
const string url_positron = "http://www.phy.bnl.gov/wire-cell/examples/celltree/3.0/celltree-e.root";

int main(int argc, char* argv[])
{
    string url = url_alpha;	// small file
    if (argc > 1) {
	url = argv[1];
    }

    WireCellSst::FrameSource fs;
    auto cfg = fs.default_configuration();
    put(cfg, "filename", url);
    //put(cfg, "source", "raw");
    cerr << cfg << endl;
    fs.configure(cfg);

    IFrame::pointer frame;
    while (fs(frame)) {
	if (!frame) {
	    cerr << "Hist end of stream, bye." << endl;
	    break;
	}

	auto traces = frame->traces();
	cerr << "Got frame: #" << frame->ident()
	     << " t=" << frame->time()
	     << " #traces:" << traces->size()
	     << endl;

	for (auto trace : *traces.get()) {
	    //int tbin = trace->tbin();
	    int ch = trace->channel();
	    auto charges = trace->charge();
	    double qsum=0, qsum2=0;
	    int n=charges.size();
	    if (!n) { continue; }
	    for (auto q : charges) {
		qsum += q;
		qsum2 += q*q;
	    }	    

	    double qmean = qsum/n;
	    cerr << "\tch="<<ch<<" <q>="<<qmean<<" +/- " << sqrt(qsum2/n - qmean*qmean) << endl;
	}
    }

    return 0;
}
