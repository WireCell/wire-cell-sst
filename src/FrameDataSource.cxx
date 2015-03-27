#include "WireCellSst/FrameDataSource.h"

#include "TClonesArray.h"
#include "TH1F.h"

WireCellSst::FrameDataSource::FrameDataSource(TTree& ttree)
    : WireCell::FrameDataSource()
    , tree(&ttree)
    , event()
    , index(-1)
{
    // sigh, we can't do things this simply because the ttree does not
    // have a single branch.  
    // tree->SetBranchAddress(name, &event);

    tree->SetBranchAddress("eventNo" , &event.number);
    tree->SetBranchAddress("runNo"   , &event.run);
    tree->SetBranchAddress("subRunNo", &event.subrun);

    tree->SetBranchAddress("calib_nChannel", &event.nchannels);
    tree->SetBranchAddress("calib_channelId", &event.channelid);
    tree->SetBranchAddress("calib_wf", &event.signal);

}

WireCellSst::FrameDataSource::~FrameDataSource()
{
}

int WireCellSst::FrameDataSource::size() const
{
    return tree->GetEntries();
}

int WireCellSst::FrameDataSource::jump(int frame_number)
{
    if (index == frame_number) {
	return frame_number;
    }

    int siz = tree->GetEntry(frame_number);
    if (siz>0) {
	index = frame_number;
	return frame_number;
    }
    return -1;
}

int WireCellSst::FrameDataSource::next()
{
    index += 1;
    return jump(index);
}

int WireCellSst::FrameDataSource::get(WireCell::Frame& frame) const
{
    for (size_t ind=0; ind<event.channelid->size(); ++ind) {
	int chid = event.channelid->at(ind);
	TH1F* signal = dynamic_cast<TH1F*>(event.signal->At(ind));
	if (!signal) {
	    return -1;
	}

	WireCell::Trace trace;
	trace.chid = chid;
	trace.tbin = 0;		// full readout, if zero suppress this would be non-zero
	for (int ibin=1; ibin <= signal->GetNbinsX(); ++ibin) {
	    trace.charge.push_back(signal->GetBinContent(ibin));
	}
	frame.push_back(trace);
    }
    return index;
}


