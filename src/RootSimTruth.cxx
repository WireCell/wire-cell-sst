#include "WireCellSst/RootSimTruth.h"


WireCellSst::RootSimTruth::RootSimTruth()
    : size(0)
    , channelIdY(new std::vector<int>)
    , trackId(new std::vector<int>)
    , tdc(new std::vector<int>)
    , x(new std::vector<float>)
    , y(new std::vector<float>)
    , z(new std::vector<float>)
    , q(new std::vector<float>)
{

}

WireCellSst::RootSimTruth::~RootSimTruth()
{
    delete channelIdY;
    delete trackId;
    delete tdc;
    delete x;
    delete y;
    delete z;
    delete q;
}
void WireCellSst::RootSimTruth::clear()
{
    channelIdY->clear();
    trackId->clear();
    tdc->clear();
    x->clear();
    y->clear();
    z->clear();
    q->clear();
}
