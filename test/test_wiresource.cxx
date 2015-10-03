#include "WireCellSst/WireSource.h"
#include "WireCellUtil/Testing.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace WireCell;
using namespace std;

int main()
{
    // R"()" is C++-0x
    // The tests below bake-in various things about this test data so don't go changing it willy-nilly.
    std::string data = R"(
# This file is used to translate between channel ID, wire ID, and geometry.
#
# channel: channel ID
# plane: plane number (0 == U, 1 == V, 2 == Z)
# wire: wire ID on that plane
# sx, sy, sz: starting (x,y,z) position of the wire in cm
# ex, ey, ez: ending (x,y,z) position of the wire in cm
#
# ch pl wire       sx     sy    sz          ex     ey   ez
0    0 0          0.0  117.021 0.206489    0.0  117.36 0.793511
1    0 1          0.0  116.675 0.206489    0.0  117.36 1.39351
2    0 2          0.0  116.328 0.206489    0.0  117.36 1.99351
3    0 3          0.0  115.982 0.206489    0.0  117.36 2.59351
4    0 4          0.0  115.636 0.206489    0.0  117.36 3.19351
5    0 5          0.0  115.289 0.206489    0.0  117.36 3.79351
6    0 6          0.0  114.943 0.206489    0.0  117.36 4.39351
7    0 7          0.0  114.596 0.206489    0.0  117.36 4.99351
8    0 8          0.0  114.25  0.206489    0.0  117.36 5.59351
9    0 9          0.0  113.903 0.206489    0.0  117.36 6.19351 
2399 1 0         -0.3 -115.081 0.00648934 -0.3 -115.42 0.593511
2400 1 1         -0.3 -114.735 0.00648934 -0.3 -115.42 1.19351
2401 1 2         -0.3 -114.388 0.00648934 -0.3 -115.42 1.79351
2402 1 3         -0.3 -114.042 0.00648934 -0.3 -115.42 2.39351
2403 1 4         -0.3 -113.696 0.00648934 -0.3 -115.42 2.99351
2404 1 5         -0.3 -113.349 0.00648934 -0.3 -115.42 3.59351
2405 1 6         -0.3 -113.003 0.00648934 -0.3 -115.42 4.19351
2406 1 7         -0.3 -112.656 0.00648934 -0.3 -115.42 4.79351
2407 1 8         -0.3 -112.31  0.00648934 -0.3 -115.42 5.39351
2408 1 9         -0.3 -111.963 0.00648934 -0.3 -115.42 5.99351 
4798 2 0         -0.6 -115.53  0.3        -0.6  117.47 0.3
4799 2 1         -0.6 -115.53  0.6        -0.6  117.47 0.6
4800 2 2         -0.6 -115.53  0.9        -0.6  117.47 0.9
4801 2 3         -0.6 -115.53  1.2        -0.6  117.47 1.2
4802 2 4         -0.6 -115.53  1.5        -0.6  117.47 1.5
4803 2 5         -0.6 -115.53  1.8        -0.6  117.47 1.8
4804 2 6         -0.6 -115.53  2.1        -0.6  117.47 2.1
4805 2 7         -0.6 -115.53  2.4        -0.6  117.47 2.4
4806 2 8         -0.6 -115.53  2.6        -0.6  117.47 2.8
4807 2 9         -0.6 -115.53  3          -0.6  117.47 3)";

    istringstream iss(data);
    WireCellSst::WireSource ws;
    ws.load(iss);
    IWire::shared_vector wires;
    Assert(ws.extract(wires));
    Assert(wires);
    Assert(!wires->empty());

    for (auto wire : *wires) {
	cerr << " " << wire->ident()
	     << " " << wire->planeid()
	     << " " << wire->index()	     
	     << " " << wire->channel()
	     << " " << wire->segment()
	     << " " << wire->ray()
	     << endl;
    }
    return 0;
}
