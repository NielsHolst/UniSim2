find ./src/plugins -name '*.rc' -delete
find . -name '*.o' -delete
find . -name 'object_script.*.*' -delete
find . -name 'Makefile.*' -delete
find . -name 'UniversalSimulator*-installer.exe' -delete
find ./src/plugins/* -type d -name 'debug' -exec rm -rf {} + 
find ./src/plugins/* -type d -name 'release' -exec rm -rf {} + 
find ..\build-* -type d -exec rm -rf {} + 

rm -r output
rm -r bin
rm -r ship/bin
rm -r ship/cmd/ig/bin

cp UniSim2.pro CopyUniSim2.pro
rm UniSim2.pro.*
rm UniSim2.pro.*.*
cp CopyUniSim2.pro UniSim2.pro
rm CopyUniSim2.pro

cp ig.pro copy_ig.pro
rm ig.pro.*
rm ig.pro.*.*
cp copy_ig.pro ig.pro
rm copy_ig.pro

cp test.pro copy_test.pro
rm test.pro.*
rm test.pro.*.*
cp copy_test.pro test.pro
rm copy_test.pro
