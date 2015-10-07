#ifndef WIRECELLSST_ROOTSIMTRUTH_H
#define WIRECELLSST_ROOTSIMTRUTH_H

#include "TTree.h"
#include <vector>


namespace WireCellSst{

    /// Match structure of the SST sim truth
    struct RootSimTruth {

	int size;
	std::vector<int>   *channelIdY;
	std::vector<int>   *trackId;
	std::vector<int>   *tdc;
	std::vector<float> *x;
	std::vector<float> *y;
	std::vector<float> *z;
	std::vector<float> *q;


	RootSimTruth();
	~RootSimTruth();

	void clear();
	void graphf(TTree& tree);

    };
}

#endif




