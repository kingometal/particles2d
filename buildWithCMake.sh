#!/bin/bash
BUILD_FOLDER=build
if [[ "$1" == "clean" ]]; then
	rm -rf $BUILD_FOLDER || exit -1
	exit 0
elif [[ "$1" == "rebuild" ]]; then
	rm -rf $BUILD_FOLDER || exit -1
fi

if [[ ! -d $BUILD_FOLDER ]]; then
	mkdir ${BUILD_FOLDER} || exit -1
fi

cd $BUILD_FOLDER || exit -1
cmake .. || exit -1
make || exit -1
cd ..
./particles2d.x
