#include "WireCellSst/RootSimDepo.h"

#include "TTree.h"

using namespace WireCellSst;


RootSimDepo::RootSimDepo()
    : x(new std::vector<float>)
    , y(new std::vector<float>)
    , z(new std::vector<float>)
    , q(new std::vector<float>)
{
}
RootSimDepo::~RootSimDepo()
{
    delete x;
    delete y;
    delete z;
    delete q;
}


void RootSimDepo::attach(TTree& tree)
{
    tree.SetBranchStatus("simide_x", 1);
    tree.SetBranchAddress("simide_x", &x);

    tree.SetBranchStatus("simide_y", 1);
    tree.SetBranchAddress("simide_y", &y);

    tree.SetBranchStatus("simide_z", 1);
    tree.SetBranchAddress("simide_z", &z);

    tree.SetBranchStatus("simide_numElectrons", 1);
    tree.SetBranchAddress("simide_numElectrons", &q);
}
