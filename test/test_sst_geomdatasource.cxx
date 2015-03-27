#include "WireCellSst/GeomDataSource.h"

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
# channel plane wire sx sy sz ex ey ez
0    0 0 -6.34706e-14  117.021 0.206489   -6.33987e-14 117.36 0.793511
1    0 1 -6.34706e-14  116.675 0.206489   -6.33252e-14 117.36 1.39351
2    0 2 -6.34706e-14  116.328 0.206489   -6.32517e-14 117.36 1.99351
3    0 3 -6.34706e-14  115.982 0.206489   -6.31782e-14 117.36 2.59351
4    0 4 -6.34706e-14  115.636 0.206489   -6.31048e-14 117.36 3.19351
5    0 5 -6.34706e-14  115.289 0.206489   -6.30313e-14 117.36 3.79351
6    0 6 -6.34706e-14  114.943 0.206489   -6.29578e-14 117.36 4.39351
7    0 7 -6.34706e-14  114.596 0.206489   -6.28843e-14 117.36 4.99351
8    0 8 -6.34706e-14  114.25  0.206489   -6.28109e-14 117.36 5.59351
9    0 9 -6.34706e-14  113.903 0.206489   -6.27374e-14 117.36 6.19351 
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
4798 2 0         -0.6 -115.53  0.3        -0.6 117.47 0.3
4799 2 1         -0.6 -115.53  0.6        -0.6 117.47 0.6
4800 2 2         -0.6 -115.53  0.9        -0.6 117.47 0.9
4801 2 3         -0.6 -115.53  1.2        -0.6 117.47 1.2
4802 2 4         -0.6 -115.53  1.5        -0.6 117.47 1.5
4803 2 5         -0.6 -115.53  1.8        -0.6 117.47 1.8
4804 2 6         -0.6 -115.53  2.1        -0.6 117.47 2.1
4805 2 7         -0.6 -115.53  2.4        -0.6 117.47 2.4
4806 2 8         -0.6 -115.53  2.7        -0.6 117.47 2.7
4807 2 9         -0.6 -115.53  3          -0.6 117.47 3)";

    istringstream iss(data);
    WireCellSst::GeomDataSource gds(iss);

    int errors = 0;
    
    for (int iplane = 0; iplane < 3; ++iplane) {
	WirePlaneType_t plane = static_cast<WirePlaneType_t>(iplane);
	WireSelection ws = gds.wires_in_plane(plane);
	if (! ws.size()) {
	    cerr << "Got empty wire selection." << endl;
	    ++errors;
	}
	sort_by_planeindex(ws);
	for (size_t ind=0; ind<ws.size(); ++ind) {
	    const Wire& wire = *ws[ind];
	    if (wire.plane != plane) {
		cerr << "Got wrong plane" << endl;
		++errors;
	    }
	    if (wire.ident < 0) {
		cerr << "Bogus wire ID, id=" << wire.ident << " plane=" << wire.plane << " index=" << wire.index << endl;
		++errors;
	    }
	    if (wire.index < 0) {
		cerr << "Bogus wire index, id=" << wire.ident << " plane=" << wire.plane << " index=" << wire.index << endl;
		++errors;
	    }
	    cout << "Wire: id=" << wire.ident << " plane=" << wire.plane << " index=" << wire.index << endl;


	}

	float pitch = gds.pitch(plane);
	float angle = gds.angle(plane);
	cout << "pitch=" << pitch/units::mm << " mm"
	     <<" angle=" << angle/units::degree << " degree"
	     << endl;
	if (isnan(pitch)) {
	    cerr << "Got NaN for pitch for plane " << plane << endl;
	    ++errors;
	}
	if (std::abs(pitch-3*units::mm) > 0.01*units::mm) {
	    cerr << "Got wrong pitch for plane " << plane << ": " << pitch << endl;
	    ++errors;
	}
	double wantang[3] = {60.0*units::degree, 120.0*units::degree, 0.0*units::degree};
	double want = wantang[iplane];
	double epsdegree = 0.01*units::degree;
	if (std::abs(angle - want) > epsdegree) {
	    cerr << "Got wrong angle for plane " << plane << ": " << angle/units::degree << " degree"
		 << " wanted: " << want/units::degree
		 << endl;
	    ++errors;
	}

	for (int mmind=0; mmind<3; ++mmind) {
	    std::pair<float,float> minmax = gds.minmax(mmind, plane);
	    cerr << mmind << " min=" << minmax.first << " max=" << minmax.second << endl;
	    if (minmax.first == minmax.second && minmax.first == 0.0) {
		cerr << "Failed to find mimmax!" << endl;
		++errors;
	    }
	    if (minmax.first > minmax.second) {
		cerr << "Min bigger than max!" << endl;
		++errors;
	    }
	}


    }

    std::vector<float> ex = gds.extent();
    cerr << "Extent: "
	 << " x:" << ex[0]/units::mm << " mm"
	 << " y:" << ex[1]/units::m << " m"
	 << " z:" << ex[2]/units::m << " m"
	 << endl;
    // note we are using only a portion of wires, which explains these weird numbers
    float wantext[] = {6.0*units::mm, 2.33*units::m, 0.0619*units::m};
    for (int ind=0; ind<3; ++ind) {
	float normed = std::abs(ex[ind] - wantext[ind]) / wantext[ind];
	cerr << "Extent dim#" << ind << ": " << ex[ind] << " - " << wantext[ind] << " normed: " << normed << endl;
	if (normed > 0.001) {
	    cerr << "Got unexpected extent for dimension " << ind << ": " << normed << endl;
	    ++errors;
	}

    }



    if (errors) {
	cerr << "Got " << errors << " errors" << endl;
	exit(1);
    }

    return 0;
}
