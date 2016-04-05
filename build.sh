#!/bin/sh

if [ ! -d "build" ];
then
	mkdir "build"
fi

# rm -rf ./packages/ontgen/data/ontgen

# if [ ! -d "packages/ontgen/data/ontgen" ];
# then
# 	mkdir "packages/ontgen/data/ontgen"
# 	mkdir "packages/ontgen/data/ontgen/platforms"
# 	mkdir "packages/ontgen/data/ontgen/libs"
# fi

# /home/silvana/qt/qtbase/bin/qmake -makefile -spec linux-g++ -o build/Makefile
# cd build
# make
# cp interface ../packages/ontgen/data/ontgen
# cp -r ./help ../packages/ontgen/data/ontgen
# cd ..

cp libqxcb.so packages/ontgen/data/ontgen/platforms
cd packages/ontgen/data/ontgen
for dep in `ldd ./interface | awk '{print $3}' | grep -v "("`
do
	cp $dep ./libs
done

cd ../../../../

binarycreator -c config/config.xml -p packages/ --offline-only ontgen