MAJOR=2
MINOR=0
SUB=3
CONFIG=release
echo Building $MAJOR.$MINOR.$SUB $CONFIG

makeops=-B

echo = Build ship apps =
del /S /Q ..\bin
pushd ../src
~/Qt/5.7/clang_64/bin/qmake ship-cmd.pro
make
popd

echo = Update Qt resources
./update-qt-resources.sh

echo = Update Qt project
../bin/update-qt-project $CONFIG

echo = Delete target bin folder
rm -r ../../../bin

echo = Build Universal Simulator =
pushd ../../..
~/Qt/5.7/clang_64/bin/qmake UniSim2.pro -spec ~/Qt/5.7/clang_64/mkspecs/macx-clang CONFIG+=x86_64
make $makeops
popd	

echo = Set suffix =
suffix=d
RELEASE=release
if [ “$CONFIG” == “$RELEASE” ]
then
  suffix=
fi
target=../../../bin/unisim$suffix.app

echo = Create App bundle =
sudo cp ../../../bin/*.dylib /usr/lib
~/Qt/5.7/clang_64/bin/macdeployqt $target -always-overwrite
install_name_tool -change @executable_path/../Frameworks/libbase$suffix.1.dylib /usr/lib/libbase$suffix.1.dylib ../../../bin/unisim.app/Contents/MacOS/unisim

echo = Copy UniSim plugins =
mkdir $target/Contents/MacOS/plugins
cp ../../../bin/plugins/*.dylib $target/Contents/MacOS/plugins
otool -L  $target/Contents/MacOS/unisim$suffix

echo = Set installer version number =
../bin/update-installer-project $MAJOR $MINOR $SUB

echo "END"

