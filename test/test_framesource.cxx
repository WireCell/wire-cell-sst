#include "WireCellSst/FrameSource.h"
#include "WireCellUtil/Testing.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace WireCell;
using namespace std;

const string url_alpha = "http://www.phy.bnl.gov/wire-cell/examples/celltree/3.0/celltree_alpha-new.root";
const string url_positron = "http://www.phy.bnl.gov/wire-cell/examples/celltree/3.0/celltree-e.root";

int main()
{
    WireCellSst::FrameSource fs;
    auto cfg = fs.default_configuration();
    put(cfg, "filename", url_alpha);
    cerr << cfg << endl;
    fs.configure(cfg);

    IFrame::pointer frame;
    while (fs(frame)) {
	if (!frame) {
	    cerr << "EOS" << endl;
	    continue;
	}
	    

	auto traces = frame->traces();

	cerr << "Got frame: #" << frame->ident()
	     << " t=" << frame->time()
	     << " #traces:" << traces->size()
	     << endl;
    }

    return 0;
}
