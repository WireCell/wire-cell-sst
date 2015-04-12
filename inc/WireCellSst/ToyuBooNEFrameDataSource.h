#ifndef WIRECELLSST_TOYUBOONEFRAMEDATASOURCE_H
#define  WIRECELLSST_TOYUBOONEFRAMEDATASOURCE_H

#include "WireCellNav/FrameDataSource.h"
#include "WireCellSst/RootEvent.h"

#include "TTree.h"

namespace WireCellSst {

    /**
       
     */
    class ToyuBooNEFrameDataSource : public WireCell::FrameDataSource {
	mutable TTree* tree;	// or TChain
	WireCellSst::RootEvent event;

      public:
	ToyuBooNEFrameDataSource(TTree& tree);
	virtual ~ToyuBooNEFrameDataSource();

	/// Return the number of frames this data source knows about.  Return -1 if unlimited.
	virtual int size() const;

	/// Explicitly set the "frame" (event) to process.  Frame number returned or -1 on error.
	virtual int jump(int frame_number);

    };

}

#endif
