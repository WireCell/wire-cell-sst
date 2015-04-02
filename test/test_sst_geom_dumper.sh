#!/bin/bash
set -e
wd=$(mktemp -d)
cd $wd 
wget https://raw.githubusercontent.com/BNLIF/wire-cell-event/master/geometry/ChannelWireGeometry.txt
sst-geom-dumper ChannelWireGeometry.txt
rm -rf $wd
