#include "GEMModule.h"
#include "HCTracking.h"
#include <iostream>
#include <TDatime.h>
#include "EventWrapper.h"
#include <string>

int main(int argc, char *argv[])
{
    std::cout << "hello, world!\n";

    EventWrapper evio_event_wrapper;

    HCTracking *tracking = new HCTracking();
    tracking->LoadConfig(TDatime("2022-11-03 16:35:00"));
    tracking->CompleteInitialization();
    tracking -> Begin(9999);

    // raw data
    evc::EvChannel evchan;
    //std::string dpath = "/home/xinzhan/evio_data/genii_data/e1209016_2562.evio.0.3";
    std::string dpath = "/home/xinzhan/test/hallc_setup_2022/analysis_frame_work/data/hallc_fadc_ssp_3340.evio.21";
    if (evchan.Open(dpath) != evc::status::success)
    {
        std::cout << "Cannot open evchannel at " << dpath << std::endl;
        return 0;
    }

    int nev = 10000; // event limit
    int count = 1;
    int PROGRESS_COUNT = 10000;

    while ((evchan.Read() == evc::status::success) && (nev-- != 0))
    {
        if ((count % PROGRESS_COUNT) == 0)
        {
            std::cout << "Processed events - " << count << "\r" << std::flush;
        }
        count++;
        switch (evchan.GetEvHeader().tag)
        {
        case CODA_PHY1:
        case CODA_PHY2:
            break;
        case CODA_EPICS:
        {
            // fill_epics_event(evchan.GetRawBuffer(), &epic_sys, count, epics_tree);
        }
            continue;
        case CODA_PRST:
        case CODA_GO:
        default:
            continue;
        }

        // evchan.ScanBanks(dbanks);
        //  get block level
        //  int blvl = evchan.GetEvBuffer(ref.crate, ref.bank, ref.slot).size();

        const uint32_t *dbuf;
        size_t buflen;
        int blvl = 0;
        std::cout << "event number: " << count << std::endl;
        //getchar();
        std::vector<uint32_t> banks = {10};
        evchan.ScanBanks(banks);

        // only get gem: crate=3; bank=10; slot=20
        // int blvl = evchan.GetEvBuffer(ref.crate, ref.bank, ref.slot).size();
        uint32_t tmp_crate = 3, tmp_bank = 10, tmp_slot = 20;

        // this is for GEn-II setup
        //uint32_t tmp_crate = 20, tmp_bank = 3561, tmp_slot = 11;

        try
        {
            blvl = evchan.GetEvBuffer(tmp_crate, tmp_bank, tmp_slot).size();
            //std::cout << "buffer level: " << blvl << std::endl;
            //getchar();
        }
        catch (std::exception &e)
        {
            std::cout << "XBDEBUG:: warning: " << e.what() << std::endl;
            continue;
        }

        /*
        for (int ii = 0; ii < blvl; ii++)
        {
            try
            {
                dbuf = evchan.GetEvBuffer(tmp_crate, tmp_bank, tmp_slot, ii, buflen);
            }
            catch (std::exception &e)
            {
                std::cout << "warning: " << e.what() << "\n";
                continue;
            }

            evio_event_wrapper.LoadEvent(dbuf, buflen);

            // std::vector<int> ivec{mod.bank, mod.crate};
            // gem_decoder.Decode(dbuf, buflen, ivec);
            // auto event = static_cast<GEMTreeStruct *>(mod.event);
            // extract_gem_cluster(&gem_system, &gem_decoder, *event, count);

            tracking->Decode(evio_event_wrapper);
            tracking->find_tracks();
        }
        */

        auto raw_vec = evchan.GetRawBufferVec();
        evio_event_wrapper.LoadEvent(raw_vec.data(), raw_vec.size());
        tracking->Decode(evio_event_wrapper);
        tracking -> find_tracks();
    }
    evchan.Close();

    tracking -> End(9999);

    return 0;
}
