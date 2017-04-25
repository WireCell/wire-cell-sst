#include "WireCellSst/FrameSource.h"
#include "WireCellIface/SimpleFrame.h"
#include "WireCellIface/SimpleTrace.h"
#include "WireCellUtil/Units.h"
#include "WireCellUtil/Persist.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include <iostream>

using namespace std;
using namespace WireCell;

WireCellSst::FrameSource::FrameSource()
    : m_tfile(0)
    , m_tree(0)
    , m_entry(0)
{
}
WireCellSst::FrameSource::FrameSource(const char* filename, const char* tpath, const char* source)
    : m_tfile(0)
    , m_tree(0)
    , m_entry(0)
{
    this->open(filename, tpath, source);
}

WireCellSst::FrameSource::~FrameSource()
{
    this->close();
}

void WireCellSst::FrameSource::open(const char* filename, const char* tpath, const char* source)
{
    if (m_tfile) {
	this->close();
    }

    // fixme, should check version consistency
    // https://github.com/BNLIF/wire-cell-celltree/blob/master/data/versions.md

    m_tfile = TFile::Open(filename, "READONLY");
    m_tree = dynamic_cast<TTree*>(m_tfile->Get(tpath));

    m_tree->SetBranchAddress("eventNo" , &m_event.number);
    m_tree->SetBranchAddress("runNo"   , &m_event.run);
    m_tree->SetBranchAddress("subRunNo", &m_event.subrun);

    m_tree->SetBranchAddress(Form("%s_nChannel", source), &m_event.nchannels);
    m_tree->SetBranchAddress(Form("%s_channelId", source), &m_event.channelid);
    m_tree->SetBranchAddress(Form("%s_wf", source), &m_event.signal);
}

void WireCellSst::FrameSource::close()
{
    if (m_tfile) {
	delete m_tfile;
    }
    m_tfile = 0;
    m_tree = 0;
}

Configuration WireCellSst::FrameSource::default_configuration() const
{
    stringstream ss;
    ss << "{\n"
       << "\"filename\":\"\",\n"
       << "\"treepath\":\"/Event/Sim\",\n"
       << "\"source\":\"calib\"\n"
       << "}\n";
    return Persist::loads(ss.str());
}

void WireCellSst::FrameSource::configure(const Configuration& cfg)
{
    auto fname = get<std::string>(cfg, "filename", "");
    auto tpath = get<std::string>(cfg, "treepath", "/Event/Sim");
    auto source = get<std::string>(cfg, "source", "calib");
    this->open(fname.c_str(), tpath.c_str(), source.c_str());
}


bool WireCellSst::FrameSource::operator()(IFrame::pointer& frame)
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
    size_t nchannels = m_event.channelid->size();
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

    // This data is too stupid and wasteful to encode an absolute time
    // or a tick length so we make up some crap.
    double time = (1+frame_number)*units::second;
    double tick = 0.5*units::microsecond;
    SimpleFrame* sframe = new SimpleFrame(frame_number, time, traces, tick);
    frame = IFrame::pointer(sframe);
    return true;
}




