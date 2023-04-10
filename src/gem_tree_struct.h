#ifndef GEM_TREE_STRUCT_H
#define GEM_TREE_STRUCT_H

/*
 * a helper class to help fill gem tree
 */

#include <vector>
#include <iostream>
#include <iomanip>

#define MAXCLUSTERS 1000
#define MAXCLUSTERSIZE 100
#define MAXAPV 100
#define DEF_VAL -99999

using std::unordered_map;
using std::vector;

struct GEMTreeStruct
{
    int event_number;
    int nCluster;
    vector<int> Plane, Prod, Module, Axis, Size;
    vector<float> Adc, Pos;

    // reorganize 2d array into 1d array for easier TTree treatment
    // data organization:
    // [<....cluste_1 strips....><....cluste_2 strips....> ...... <....cluste_N strips....>]
    vector<int> StripNo;
    vector<float> StripAdc;

    vector<float> StripTs0, StripTs1, StripTs2,
        StripTs3, StripTs4, StripTs5;

    // dynamic programming to reduce time complexity
    unordered_map<int, int> dp_table;
    vector<bool> position_check;

    // tracking data
    int fNtracks_found, fNhitsOnTrack;
    float fXtrack, fYtrack, fXptrack, fYptrack, fChi2Track;
    vector<int> fHitLayer;
    vector<float> fHitXlocal, fHitYlocal;
    vector<float> fHitXprojected, fHitYprojected;
    vector<float> fHitResidU, fHitResidV;
    vector<float> fHitUADC, fHitVADC;
    vector<float> fHitIsampMaxUstrip, fHitIsampMaxVstrip;

    int nAPV;
    vector<int> apv_crate_id, apv_mpd_id, apv_adc_ch;

    GEMTreeStruct()
    {
        Clear();
    }

    void Clear()
    {
        event_number = -1, nCluster = 0;

        Plane.resize(MAXCLUSTERS, DEF_VAL), Prod.resize(MAXCLUSTERS, DEF_VAL),
            Module.resize(MAXCLUSTERS, DEF_VAL), Axis.resize(MAXCLUSTERS, DEF_VAL),
            Size.resize(MAXCLUSTERS, DEF_VAL), Adc.resize(MAXCLUSTERS, DEF_VAL),
            Pos.resize(MAXCLUSTERS, DEF_VAL);

        StripNo.clear(), StripAdc.clear(), StripTs0.clear(), StripTs1.clear(),
            StripTs2.clear(), StripTs3.clear(), StripTs4.clear(), StripTs5.clear();

        dp_table.clear(); position_check.resize(MAXCLUSTERS, false);

        fNtracks_found = 0, fNhitsOnTrack = 0;
        fXtrack = DEF_VAL, fYtrack = DEF_VAL, fXptrack = DEF_VAL, fYptrack = DEF_VAL, fChi2Track = DEF_VAL;
        fHitLayer.clear(); fHitXlocal.clear(); fHitYlocal.clear();
        fHitXprojected.clear(); fHitYprojected.clear();
        fHitResidU.clear(); fHitResidV.clear();
        fHitUADC.clear(); fHitVADC.clear();
        fHitIsampMaxUstrip.clear(); fHitIsampMaxVstrip.clear();

        nAPV = 0;
        apv_crate_id.resize(MAXAPV, DEF_VAL), apv_mpd_id.resize(MAXAPV, DEF_VAL),
            apv_adc_ch.resize(MAXAPV, DEF_VAL);
    }

    // get strip no. array for a given cluster
    vector<int> GetStripNoArrayForCluster(int i)
    {
        vector<int> res;
        if (i > nCluster || i < 0)
            return res;

        if (i == 0)
        {
            dp_table[i] = 0;
            position_check[i] = true;
        }
        else
        {
            if (position_check[i - 1])
            {
                dp_table[i] = dp_table[i - 1] + Size[i - 1];
                position_check[i] = true;
            }
            else
                GetStripNoArrayForCluster(i - 1);
        }

        for (int j = 0; j < Size[i]; j++)
        {
            res.push_back(StripNo[dp_table[i] + j]);
        }

        return res;
    }

    // get strip adc array for a given cluster
    vector<float> GetStripAdcArrayForCluster(int i)
    {
        vector<float> res;
        if (i > nCluster || i < 0)
            return res;

        if (i == 0)
        {
            dp_table[i] = 0;
            position_check[i] = true;
        }
        else
        {
            if (position_check[i - 1])
            {
                dp_table[i] = dp_table[i - 1] + Size[i - 1];
                position_check[i] = true;
            }
            else
                GetStripNoArrayForCluster(i - 1);
        }

        for (int j = 0; j < Size[i]; j++)
        {
            res.push_back(StripAdc[dp_table[i] + j]);
        }

        return res;
    }

    // get strip ts_adc 2-d array for a given cluster
    vector<vector<float>> GetStripTsArrayForCluster(int i)
    {
        vector<vector<float>> res;
        if (i > nCluster || i < 0)
            return res;

        if (i == 0)
        {
            dp_table[i] = 0;
            position_check[i] = true;
        }
        else
        {
            if (position_check[i - 1])
            {
                dp_table[i] = dp_table[i - 1] + Size[i - 1];
                position_check[i] = true;
            }
            else
                GetStripNoArrayForCluster(i - 1);
        }

        for (int j = 0; j < Size[i]; j++)
        {
            vector<float> temp;
            temp.push_back(StripTs0[dp_table[i] + j]);
            temp.push_back(StripTs1[dp_table[i] + j]);
            temp.push_back(StripTs2[dp_table[i] + j]);
            temp.push_back(StripTs3[dp_table[i] + j]);
            temp.push_back(StripTs4[dp_table[i] + j]);
            temp.push_back(StripTs5[dp_table[i] + j]);
            res.push_back(temp);
        }
        return res;
    }
};

#endif
