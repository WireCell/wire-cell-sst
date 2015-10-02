#include "WireCellSst/FrameSource.h"
#include "WireCellUtil/Testing.h"
#include "WireCellUtil/Units.h"

#include <iostream>
using namespace WireCell;
using namespace std;

int count_nonzero(const ITrace::ChargeSequence& charges)
{
    int nonzero = 0;
    for (auto q : charges) {
	if (q > 0.0) ++nonzero;
    }
    return nonzero;
}

void dump(const IFrame::pointer &frame)
{
    ITrace::shared_vector traces = frame->traces();

    cerr << "Frame " << frame->ident() << " @ t=" << frame->time()/units::second << "s with " << traces->size() << endl;

    for (auto trace : *traces) {
	const ITrace::ChargeSequence& charges = trace->charge();
	int nonzero = count_nonzero(charges);
	if (!nonzero) { continue; }
	cerr << " chid:" << trace->channel()
	     << " tbin:" << trace->tbin()
	     << " #bins:" << charges.size()
	     << " nonzero:" << nonzero
	     << endl;
    }
}
    

int main(int argc, char** argv)
{
    if (argc < 1) {
	cerr << "usage: test_dump celltree_file.root [TDirectory/path]" << endl;
	return 1;
    }
    const char* root_file_name = argv[1];

    const char* tpath = "/Event/Sim";
    if (argc > 2) {
	tpath = argv[2];
    }

    WireCellSst::FrameSource fs(root_file_name, tpath);
    cerr << "Loaded: " << root_file_name << " " << tpath << endl;

    while (true) {
	IFrame::pointer frame;
	Assert(fs.extract(frame));
	if (!frame) {
	    break;
	}
	dump(frame);
    }

    return 0;
}
