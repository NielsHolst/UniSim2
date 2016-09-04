# File suffix (debug or release)
suffix=d
target=bin/unisim$suffix.app
echo $suffix

# Clean target bin and create bin sub-folders
rm -rf bin
mkdir bin
cp -R ../bin/unisim$suffix.app $target

# Create bundle
sudo cp ../bin/libbase$suffix.1.dylib /usr/lib
~/Qt/5.7/clang_64/bin/macdeployqt $target -always-overwrite
# Note: macdeployqt looks in /usr/lib for base library

# Copy UniSim plugins
mkdir $target/Contents/MacOS/plugins
cp ../bin/plugins/lib*.1.0.0.dylib $target/Contents/MacOS/plugins

echo "END"

