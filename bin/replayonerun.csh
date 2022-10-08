#!/bin/csh -fb
#  script to run the fadc250 decoder
#  remember to source env setup file $decoderdir/env/setup_env.csh before execute this script
#
#set DEBUG = ( echo ); 
set DEBUG = (""); 

if($#argv < 1) then
    echo "error: you need to provide at least 1 argument"
    echo "usage: $0 <run> [overwrite=0]"
    $DEBUG exit 0
endif

set run = ($1)

set overwrite = (0)
if ($#argv >= 2) set overwrite = ($2)

if (! $?HallCBeamtestDir) setenv HallCBeamtestDir /work/halla/solid/jixie/ecal_beamtest_hallc/decoder
set decoderdir = ${HallCBeamtestDir}
set projdir = /cache/halla/solid/subsystem/ec/ecal_beamtest_hallc
set datadir = $projdir/raw
#set replaydir = $projdir/replay
set replaydir = $decoderdir/../ROOTFILE


#set the file to proper group
$DEBUG chown jixie.12gev_solid $datadir/hallc_fadc_ssp_${run}.evio.*

cd $decoderdir
###################################################################################
#build/src/analyze -m database/modules/mapmt_module_2FADC.json  /cache/halla/solid/subsystem/hgc/2020benchtest/coda/solidhgc_${run}.evio.0 /w/halla-scshelf2102/solid/cc_pro/benchtest/coda/root/solidhgc_${run}.root
#
# First argument is reads the json file for the FADC slot to be decoded
# IN above examle we have 2 slots 6 and slot 7 "database/modules/mapmt_module_2FADC.json" 
# The other argument gives the raw evio file to be decoded:
#"/cache/halla/solid/subsystem/hgc/2020benchtest/coda/solidhgc_${run}.evio.0"
#
#3) Gives the path where the decoded root file is kept 
#"/w/halla-scshelf2102/solid/cc_pro/benchtest/coda/root/solidhgc_${run}.root"
###################################################################################


foreach infile ($datadir/hallc_fadc_ssp_${run}.evio.*)
  set subrun = (${infile:e})
  set outfile = $replaydir/beamtest_hallc_${run}_${subrun}.root

  if($overwrite == 0 && -f $outfile) then
    echo "$outfile exist, skip replaying this file ......"
    continue
  endif
    
  #echo "start replaying $infile  --> $outfile"
  #remove the output file if exists
  $DEBUG rm -f $outfile
  $DEBUG build/src/analyze -m database/modules/mapmt_module_2FADC_beamtest.json $infile $outfile
  
  #extract pedestal and create level1 tree
  if (-f $outfile) then
  $DEBUG root -b -q $outfile $decoderdir/CreateLevel1Tree/run.C+
  endif
end


