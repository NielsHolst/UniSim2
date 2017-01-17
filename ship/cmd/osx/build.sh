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
sudo cp ../../../bin/*.dylib /usr/lib

echo = Create App bundle =
target=../../../bin/unisim
target_app=$target.app
~/Qt/5.7/clang_64/bin/macdeployqt $target_app -always-overwrite
base=libuniversal_simulator_base.1.dylib
install_name_tool -change @executable_path/../Frameworks/$base /usr/lib/$base $target_app/Contents/MacOS/unisim
rm $target_app/Contents/Frameworks/*.dylib

echo = Copy UniSim plugins =
mkdir $target_app/Contents/MacOS/plugins
cp ../../../bin/plugins/*.dylib $target_app/Contents/MacOS/plugins
otool -L  $target_app/Contents/MacOS/unisim$suffix

echo = Set installer version number =
../bin/update-installer-project $MAJOR $MINOR $SUB

echo = Restore to debug version =
../bin/update-qt-project debug

#!/bin/bash +vx
echo "END"

