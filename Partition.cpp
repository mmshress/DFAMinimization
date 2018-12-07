#include "Partition.h"
#include <vector>

Partition::Partition(int max)
{
    //Initiailize the partition data structure as outlined in Valmari-Lehtinen's paper

    sets = 1; // only one set initially

    elems = std::vector<int>(max, 0);
    loc = std::vector<int>(max, 0);
    sidx = std::vector<int>(max, 0);
    for (int i = 0; i < max; i++)
    {
        elems[i] = i;
        loc[i] = i;
    }

    first = std::vector<int>(1, 0); //set 0 (0-indexing) starts at index 0 in elems
    mid = std::vector<int>(1, 0);
    end = std::vector<int>(max, 0);
}

int Partition::size(int s){
    return end[s] - first[s];
}

int Partition::set(int e){
    return sidx[e];
}

int Partition::get_first(int s){
    return elems[first[s]];
}

int Partition::get_next(int e){
    if ((loc[e] + 1) >= end[sidx[e]]) return -1;
    return elems[loc[e] + 1];
}

int Partition::mark(int e){
    int s = sidx[e];
    int l = loc[e];
    int m = mid[s];
    if(l >= m){ //if e is in the unmarked half, swap with element at position m
        elems[l] = elems[m];
        loc[elems[l]] = l;
        elems[m] = e;
        loc[e] = m;
        mid[s] = m + 1;
    }
}

int Partition::split(int s){
    if(mid[s] == end[s]) mid[s] = first[s];
    if(mid[s] == first[s]) return 0;
    else{
        first.push_back(first[s]);
        mid.push_back(first[s]);
        end.push_back(mid[s]);
        first[s] = mid[s];
        for(int i = first[sets]; i < end[sets]; i++){
            sidx[elems[i]] = sets;
        }
        sets++;
        return end.back() - first.back();        
    }
}

bool Partition::no_marks(int s){
    return mid[s] == first[s];
}