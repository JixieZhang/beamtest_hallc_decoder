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

if ($#argv < 2) then
    echo "error: you need to provide at least 2 arguments"
    echo "usage: $0 <'replay_option'> <File_List>"
    echo "example 1: $0:t '-n -1' hallc_fadc_ssp_3049.evio.?"
    echo "example 2: $0:t '-m $decoderdir/database/modules/mapmt_module_2FADC_beamtest.json -n 5000' hallc_fadc_ssp_3049.evio.2 hallc_fadc_ssp_3049.evio.3"
    echo "replayoption must be single-quoted, here is a list of them:"
    echo "usage of build/src/analyze:"
    echo "build/src/analyze [-n NEV] [-m, --module MODULE] [-r RES] [-t THRES] [-p NPEDS] [-f FLAT] RAW_DATA ROOT_FILE"
    echo "         -h, --help: print help message"
    echo "         -n [NEV (default: -1)]: number of events to process (< 0 means all)"
    echo "         -m, --module [MODULE (default: database/modules/mapmt_module.json)]: json file for module configuration"
    echo "         -r [RES (default: 3)]: resolution for waveform analysis"
    echo "         -t [THRES (default: 20.000000)]: peak threshold for waveform analysis"
    echo "         -p [NPEDS (default: 8)]: sample window width for pedestal searching"
    echo "         -f [FLAT (default: 1.000000)]: flatness requirement for pedestal searching"
    $DEBUG exit 0
endif

set jsonfile = "-m $decoderdir/database/modules/mapmt_module_2FADC_beamtest.json "
set replayoption = ("$jsonfile $1")
echo "replayoption='$replayoption'"

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

 
foreach infile ($argv[2-$#argv])
    if !(-f $infile)  then
        echo "file $infile not exist, skipped ..."
        continue
    endif
    #set the file to proper group
    $DEBUG chown jixie.12gev_solid $infile
    
    set run = (`echo $infile:t:r:r | awk -F'ssp_' '{print $2}'`)
    if ("$run" == "") then
        set run = (`echo $infile:t:r:r | awk -F'ti_' '{print $2}'`)
    endif
    set subrun = (${infile:e})
    set outfile = $replaydir/beamtest_hallc_${run}_${subrun}.root
    #echo "start replaying $infile  --> $outfile"
    #remove the output file if exist
    $DEBUG rm -f $outfile
    $DEBUG $decoderdir/build/src/analyze $replayoption $infile $outfile

     #extract pedestal and then create level1 tree
     if (-f $outfile) then
         $DEBUG root -b -q $outfile $decoderdir/run.C+
     endif
end #end of loop

