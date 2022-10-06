#!/bin/csh

source setup.csh
mkdir -p $HallCBeamtestDir
cmake -S. -B$HallCBeamtestDir/build
cmake --build $HallCBeamtestDir/build
cmake --install $HallCBeamtestDir/build --prefix $HallCBeamtestDir
cp -f ./bin/*csh ${HallCBeamtestDir}/bin/
cp -fr ./database and ./config ${HallCBeamtestDir}/
cp -f rootlogon.C ${HallCBeamtestDir}/
