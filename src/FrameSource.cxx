#include "WireCellSst/FrameSource.h"
#include "WireCellIface/SimpleFrame.h"
#include "WireCellIface/SimpleTrace.h"
#include "WireCellUtil/Units.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include <iostream>

using namespace std;
using namespace WireCell;

WireCellSst::FrameSource::FrameSource(const char* filename, const char* tpath)
    : m_tfile(0)
    , m_tree(0)
    , m_entry(0)
{
    m_tfile = TFile::Open(filename, "READONLY");
    m_tree = dynamic_cast<TTree*>(m_tfile->Get(tpath));

    m_tree->SetBranchAddress("eventNo" , &m_event.number);
    m_tree->SetBranchAddress("runNo"   , &m_event.run);
    m_tree->SetBranchAddress("subRunNo", &m_event.subrun);

    m_tree->SetBranchAddress("calib_nChannel", &m_event.nchannels);
    m_tree->SetBranchAddress("calib_channelId", &m_event.channelid);
    m_tree->SetBranchAddress("calib_wf", &m_event.signal);
}
WireCellSst::FrameSource::~FrameSource()
{
    if (!m_tfile) {
	return;
    }
    m_tfile->Close();
    delete m_tfile;
    m_tfile = 0;
    m_tree = 0;
}

bool WireCellSst::FrameSource::extract(IFrame::pointer& frame)
{
    if (m_entry < 0) {		// past EOS
	cerr << "FrameSource past EOS";
	if (m_tfile) {
	    cerr << " in: " << m_tfile->GetName();
	}
	cerr << endl;
	return false;
    }

    int nentries = m_tree->GetEntries();
    if (m_entry >= nentries) { // hit EOS
	cerr << "FrameSource hit EOS at entry " << m_entry << " >= " << nentries;
	if (m_tfile) {
	    cerr << " in: " << m_tfile->GetName();
	}
	cerr << endl;
	m_entry = -1;
	frame = nullptr;
	return true;
    }

    int frame_number = m_entry;
    int siz = m_tree->GetEntry(m_entry++);
    if (siz <= 0 ) {
	cerr << "FrameSource got bad entry at: " << m_entry-1;
	if (m_tfile) {
	    cerr << " in: " << m_tfile->GetName();
	}
	cerr << endl;
	return false;
    }

    ITrace::vector traces;

    // load into frame
    int nchannels = m_event.channelid->size();
    for (size_t ind=0; ind < nchannels; ++ind) {
	TH1F* signal = dynamic_cast<TH1F*>(m_event.signal->At(ind));
	if (!signal) {
	    cerr << "FrameSource failed to get TH1F from event at entry=" << m_entry-1 << " index=" << ind;
	    if (m_tfile) {
		cerr << " in: " << m_tfile->GetName();
	    }
	    cerr << endl;
	    return false;
	}

	int chid = m_event.channelid->at(ind);
	ITrace::ChargeSequence charge;
	for (int ibin=1; ibin <= signal->GetNbinsX(); ++ibin) {
	    charge.push_back(signal->GetBinContent(ibin));
	    //std::cerr << ibin << " " << signal->GetBinContent(ibin) << std::endl;
	}
	SimpleTrace *trace = new SimpleTrace(chid, 0, charge);
	traces.push_back(ITrace::pointer(trace));

    }

    double time = (1+frame_number)*units::second;
    SimpleFrame* sframe = new SimpleFrame(frame_number, time, traces);
    frame = IFrame::pointer(sframe);
    return true;
}




