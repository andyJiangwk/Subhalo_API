#ifndef HALO_INPUT_H
#define HALO_INPUT_H

#include <cstring>
#include "../datatype.h"
#include "../hdf5_wrapper.h"

struct Halo_input {
    int HaloId;             
    HBTxyz CenterComoving;  
    float R200CritComoving;                
    float R200MeanComoving;                
    float RVirComoving;                    
    float M200Crit;                        
    float M200Mean;                        
    float MVir;                            
    float RmaxComoving;                    
    float VmaxPhysical;                    
    Halo_input()
    : HaloId(-1),
    CenterComoving({-1.0,-1.0,-1.0}),
    R200CritComoving(0.0),
    R200MeanComoving(0.0),                
    RVirComoving(0.0),                    
    M200Crit(0.0),                        
    M200Mean(0.0),                        
    MVir(0.0),                            
    RmaxComoving(0.0),                    
    VmaxPhysical(0.0) 
    {}
};

class Halo_Catalog {
    public:
        vector <Halo_input> Halos;
        long nhalos;
        
        hid_t createHaloDatatype_in();
        int readHalo(int isnap);
        void Clear();

    Halo_Catalog() {
        int zeros=0;
        Halos.resize(zeros);
        nhalos=zeros;
    }
    
    Halo_Catalog(long nhalo) {
        Halos.resize(nhalo);
        nhalos=nhalo;
    }
    
};

#endif