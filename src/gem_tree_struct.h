#ifndef GEM_TREE_STRUCT_H
#define GEM_TREE_STRUCT_H

/*
 * a helper class to help fill gem tree
 */

#include <vector>

#define MAXCLUSTERS 1000
#define MAXCLUSTERSIZE 100
#define MAXAPV 100

using std::vector;

struct GEMTreeStruct 
{
    int event_number;
    int nCluster;
    vector<int> Plane, Prod, Module, Axis, Size;
    vector<float> Adc, Pos;
    vector<vector<int>> StripNo;
    vector<vector<float>> StripAdc;

    vector<vector<float>> StripTs0;
    vector<vector<float>> StripTs1;
    vector<vector<float>> StripTs2;
    vector<vector<float>> StripTs3;
    vector<vector<float>> StripTs4;
    vector<vector<float>> StripTs5;

    int nAPV;
    vector<int> apv_crate_id, apv_mpd_id, apv_adc_ch;

    GEMTreeStruct()
        : event_number(0), nCluster(0), Plane(MAXCLUSTERS), Prod(MAXCLUSTERS), 
        Module(MAXCLUSTERS), Axis(MAXCLUSTERS), Size(MAXCLUSTERS),
        Adc(MAXCLUSTERS), Pos(MAXCLUSTERS),
        StripNo(MAXCLUSTERS, vector<int>(MAXCLUSTERSIZE)),
        StripAdc(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)), 
        StripTs0(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)),
        StripTs1(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)),
        StripTs2(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)),
        StripTs3(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)),
        StripTs4(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)),
        StripTs5(MAXCLUSTERS, vector<float>(MAXCLUSTERSIZE)),
        nAPV(0), apv_crate_id(MAXAPV), apv_mpd_id(MAXAPV), apv_adc_ch(MAXAPV)
    {
    }

    void Clear() { event_number = 0; nCluster = 0; nAPV = 0;}

};

#endif
