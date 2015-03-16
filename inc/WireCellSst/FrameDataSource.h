#ifndef WIRECELLSST_FRAMEDATASOURCE_H
#define  WIRECELLSST_FRAMEDATASOURCE_H

#include "WireCellNav/FrameDataSource.h"
#include "WireCellSst/RootEvent.h"

#include "TTree.h"

namespace WireCellSst {

    /**
       
     */
    class FrameDataSource : public WireCellNav::FrameDataSource {
	mutable TTree* tree;	// or TChain
	WireCellSst::RootEvent event;
	int index;

      public:
	FrameDataSource(TTree& tree);
	virtual ~FrameDataSource();

	/// Return the number of frames this data source knows about.  Return -1 if unlimited.
	virtual int size() const;

	/// Explicitly set the "frame" (event) to process.  Frame number returned or -1 on error.
	virtual int jump(int frame_number);

	/// Advance to next frame, return frame number or -1 on error, -2 on end of frames
	virtual int next();

	/// Fill full frame with current frame data, returning it's number
	virtual int get(WireCellData::Frame& frame) const;

    };

}

#endif
