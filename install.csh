#!/bin/csh

source setup.csh

if !($?HallCBeamtestDir) then
  setenv HallCBeamtestDir /work/halla/solid/jixie/ecal_beamtest_hallc/decoder
  mkdir -p $HallCBeamtestDir
endif

if !(-d $HallCBeamtestDir) then
  echo '$HallCBeamtestDir  does not exit, I quit ...';
  exit 0;
endif

mkdir -p $HallCBeamtestDir
cmake -S. -B$HallCBeamtestDir/build
cmake --build $HallCBeamtestDir/build
cmake --install $HallCBeamtestDir/build --prefix $HallCBeamtestDir
cp -f ./bin/*csh ${HallCBeamtestDir}/bin/
cp -fr ./database ./config ${HallCBeamtestDir}/
cp -f rootlogon.C ${HallCBeamtestDir}/
