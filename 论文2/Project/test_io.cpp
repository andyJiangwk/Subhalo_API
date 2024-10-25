#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <omp.h>
#include "./src/io/subhalo_io.h"

using namespace std;

int main(int argc, char *argv[]) {
    int snapshot_start=127,snapshot_end=10;
    long nsub_tot=0,nsub_this=0;
    
    if (argc == 2) {
        snapshot_start = atoi(argv[1]);
    }else if (argc == 3){
        snapshot_start = atoi(argv[1]);
        snapshot_end = atoi(argv[2]);
    }
    cout<<snapshot_start<<" "<<snapshot_end<<endl;

    Subhalo_Catalogs Subhalos_in;
    Subhalos_in.readSubhalo(snapshot_start);
    for (int isub=0;isub<5;isub++)
        cout<<Subhalos_in.Subhalos[isub].TrackId<<endl;
    return 0;
}