#!/bin/csh -fb
#  script to run the decoder
#
set DEBUG = ( echo ); 
set DEBUG = (""); 

if !($?HallCBeamtestDir) set decoderdir = /work/halla/solid/jixie/ecal_beamtest_hallc/decoder
else set decoderdir = ${HallCBeamtestDir}
set projdir = /cache/halla/solid/subsystem/ec/ecal_beamtest_hallc
set datadir = $projdir/raw
#set replaydir = $projdir/replay
set replaydir = $decoderdir/../ROOTFILE

if ($#argv < 1) then
    echo "error: you need to provide at least 1 argument"
    echo "usage: $0 <startrun> [endrun=0] [overwrite=0] [outputdir=$replaydir]"
    echo "example 1: '$0 1000'  will replay for run number 1000"
    echo "example 2: '$0 1000 1005'  will replay for run number from 1000 to 1005(included)"
    echo "example 3: '$0 1000 1005 ./outdir'  will replay for run number from 1000 to 1005(included) and output file is at ./outdir"
    $DEBUG exit 0
endif


set startrun = ($1)
set endrun = ($1)
if ($#argv > 1) set endrun = ($2)
if ( $endrun < $startrun ) set endrun = $startrun

set overwrite = (0)
if ($#argv >= 3) set overwrite = ($3)

if ($#argv >= 4) set replaydir = ($4)
mkdir -p $replaydir/.check
#check for write permission
if !(-d $replaydir/.check) then
  echo "You have no write permission in $replaydir, I quit ..."
  exit 0
else
  rm -fr $replaydir/.check
endif


###################################################################################
#build/src/analyze -m database/modules/mapmt_module_2FADC.json  \
#  /cache/halla/solid/subsystem/hgc/2020benchtest/coda/solidhgc_${run}.evio.0 \
#  /w/halla-scshelf2102/solid/cc_pro/benchtest/coda/root/solidhgc_${run}.root
#
# First argument is reads the json file for the FADC slot to be decoded
# IN above examle we have 2 slots 6 and slot 7 "database/modules/mapmt_module_2FADC.json" 
# The other argument gives the raw evio file to be decoded:
#"/cache/halla/solid/subsystem/hgc/2020benchtest/coda/solidhgc_${run}.evio.0"
#
#3) Gives the path where the decoded root file is kept 
#"/w/halla-scshelf2102/solid/cc_pro/benchtest/coda/root/solidhgc_${run}.root"
###################################################################################

cd $decoderdir

@ run = $startrun - 1
while ($run < $endrun)

  @ run = $run + 1
  
  set nfile = (0)
  
  (ls -1 $datadir/hallc_fadc_ssp_${run}.evio.*| wc | awk '{print " " $1}' >! ~/.tmp_$$) >&! /dev/null  
  set nfile = (`cat  ~/.tmp_$$`)
  rm -fr ~/.tmp_$$  >&! /dev/null   
  if ($nfile < 1) continue
  
  foreach infile ($datadir/hallc_fadc_ssp_${run}.evio.*)
    #set the file to proper group
    $DEBUG chown jixie.12gev_solid $infile
  
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
      $DEBUG root -b -q $outfile $decoderdir/run.C+
    endif
  end #end of for loop
  
end #end of while loop

