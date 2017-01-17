#!/bin/bash -vx

MAJOR=2
MINOR=0
SUB=15
echo
echo Building $MAJOR.$MINOR.$SUB
echo

echo = Clean out old builds =
pushd ../../..
./clean.sh
popd

echo = Build ship apps =
rm -r ../bin
pushd ../src
~/Qt/5.7/clang_64/bin/qmake ship-cmd.pro
make -B
popd

echo = Update Qt resources
./update-qt-resources.sh
../bin/update-qt-resources $MAJOR $MINOR $SUB release

echo = Update Qt project
../bin/update-qt-project release

echo = Build Universal Simulator =
pushd ../../..
~/Qt/5.7/clang_64/bin/qmake UniSim2.pro -spec ~/Qt/5.7/clang_64/mkspecs/macx-clang CONFIG+=x86_64
make -B
popd	

echo = Copy base library =
mkdir ~/lib
yes | cp ../../../bin/*.dylib ~/lib

