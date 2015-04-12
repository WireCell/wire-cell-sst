#include "WireCellSst/ToyuBooNEFrameDataSource.h"

#include "TClonesArray.h"
#include "TH1F.h"

WireCellSst::ToyuBooNEFrameDataSource::ToyuBooNEFrameDataSource(TTree& ttree)
    : WireCell::FrameDataSource()
    , tree(&ttree)
    , event()
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

WireCellSst::ToyuBooNEFrameDataSource::~ToyuBooNEFrameDataSource()
{
}

int WireCellSst::ToyuBooNEFrameDataSource::size() const
{
    return tree->GetEntries();
}

int WireCellSst::ToyuBooNEFrameDataSource::jump(int frame_number)
{
    if (frame.index == frame_number) {
	return frame_number;
    }

    frame.clear();		// win or lose, we start anew

    if (frame_number < 0) {	// underflow
	return frame_number;
    }

    int siz = tree->GetEntry(frame_number);
    if (siz <= 0 ) {
	return -1;
    }

    if (frame_number >= siz) {
	return -1;
    }

    // load into frame
    int nchannels = event.channelid->size();
    for (size_t ind=0; ind < nchannels; ++ind) {
	TH1F* signal = dynamic_cast<TH1F*>(event.signal->At(ind));
	if (!signal) {
	    return -1;
	}

	WireCell::Trace trace;
	trace.chid = event.channelid->at(ind);

	trace.tbin = 0;		// full readout, if zero suppress this would be non-zero
	for (int ibin=1; ibin <= signal->GetNbinsX()/4.; ++ibin) {
	  trace.charge.push_back(signal->GetBinContent(4*(ibin-1)+1)+
				 signal->GetBinContent(4*(ibin-1)+2)+
				 signal->GetBinContent(4*(ibin-1)+3)+
				 signal->GetBinContent(4*(ibin-1)+4)
				   );
	}
	frame.traces.push_back(trace);
    }
    frame.index = frame_number;
    return frame.index;
}


