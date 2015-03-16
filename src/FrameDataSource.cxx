#include "WireCellSst/FrameDataSource.h"

WireCellSst::FrameDataSource::FrameDataSource(TTree& ttree, const char *name)
    : WireCellNav::FrameDataSource()
    , tree(&ttree)
    , event(0)
    , index(-1)
{
    tree->SetBranchAddress("event", &event);
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

int WireCellSst::FrameDataSource::get(WireCellData::Frame& frame) const
{
    return -1;
}


