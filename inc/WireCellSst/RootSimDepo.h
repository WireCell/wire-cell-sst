#ifndef WIRECELLSST_ROOTSIMDEPO
#define WIRECELLSST_ROOTSIMDEPO

#include <vector>

class TTree;

namespace  WireCellSst {

    /// Match structure of the SST sim truth
    struct RootSimDepo {

	std::vector<float> *x;
	std::vector<float> *y;
	std::vector<float> *z;
	std::vector<float> *q;


	RootSimDepo();
	~RootSimDepo();

	void attach(TTree& tree);

    };


}

#endif
