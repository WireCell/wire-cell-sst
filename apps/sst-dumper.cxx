#include "WireCellSst/FrameSource.h"
#include "WireCellSst/DepoSource.h"
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

    double qtot = 0.0;
    int ntot = 0;
    for (auto trace : *traces) {
	const ITrace::ChargeSequence& charges = trace->charge();
	for (auto q : charges) {
	    qtot += q;
	    if (q > 0.0) {
		++ntot;
	    }
	}
	// 	int nonzero = count_nonzero(charges);
	// 	if (!nonzero) { continue; }
	// 	cerr << " chid:" << trace->channel()
	// 	     << " tbin:" << trace->tbin()
	// 	     << " #bins:" << charges.size()
	// 	     << " nonzero:" << nonzero
	// 	     << endl;
	// }
    }

    cerr << "Frame " << frame->ident() << " @ t=" << frame->time()/units::second << "s with " << traces->size() << " traces, 3*qtot=" << qtot << ", " << ntot << " q>0" << endl;
}

void dump(const IDepo::pointer& depo)
{
    cerr << "Depo t=" << depo->time() << " q=" << depo->charge() << " pos=" << depo->pos() << endl;
}

void dump_frame(const char* root_file_name, const char* tpath)
{
    WireCellSst::FrameSource fs(root_file_name, tpath);
    cerr << "FrameSource loaded: " << root_file_name << " " << tpath << endl;

    while (true) {
	IFrame::pointer frame;
	Assert(fs.extract(frame));
	if (!frame) {
	    break;
	}
	dump(frame);
    }
}

void dump_depos(const char* root_file_name, const char* tpath)
{
    WireCellSst::DepoSource ds(root_file_name, tpath);
    cerr << "DepoSource loaded: " << root_file_name << " " << tpath << endl;
    
    while (true) {
	IDepo::pointer depo;
	Assert(ds.extract(depo));
	if (!depo) {
	    break;
	}
	dump(depo);
    }
}

int main(int argc, char** argv)
{
    if (argc < 3) {
	cerr << "usage: test_dump what|all celltree_file.root [TDirectory/path]" << endl;
	return 1;
    }
    string what(argv[1]);
    const char* root_file_name = argv[2];

    const char* tpath = "/Event/Sim";
    if (argc > 3) {
	tpath = argv[3];
    }

    if (what == "all" || what == "frame") {
	dump_frame(root_file_name, tpath);
    }
    if (what == "all" || what == "depo") {
	dump_depos(root_file_name, tpath);
    }


    return 0;
}
