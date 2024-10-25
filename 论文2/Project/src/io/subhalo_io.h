#ifndef SUBHALO_INPUT_H
#define SUBHALO_INPUT_H

#include <cstring>
#include "../datatype.h"
#include "../hdf5_wrapper.h"

struct Subhalo_input{
    long TrackId;
    long Nbound;
    float Mbound;
    long HostHaloId;
    long Rank;
    int Depth;
    float LastMaxMass;
    int SnapshotIndexOfLastMaxMass;
    int SnapshotIndexOfLastIsolation;
    int SnapshotIndexOfBirth;
    int SnapshotIndexOfDeath;
    int SnapshotIndexOfSink;
    float RmaxComoving;
    float VmaxPhysical;
    float LastMaxVmaxPhysical;
    int SnapshotIndexOfLastMaxVmax;
    float R2SigmaComoving;
    float RHalfComoving;
    float BoundR200CritComoving;
    float BoundM200Crit;
    float SpecificSelfPotentialEnergy;
    float SpecificSelfKineticEnergy;
    HBTxyz SpecificAngularMomentum; // Using HBTxyz
    HBTxyz InertialEigenVector[3]; // 3x3 matrix as an array of HBTxyz
    HBTxyz InertialEigenVectorWeighted[3]; // 3x3 matrix as an array of HBTxyz
    HBTtensor InertialTensor; // Array of size 6
    HBTtensor InertialTensorWeighted; // Array of size 6
    HBTxyz ComovingAveragePosition; // Using HBTxyz
    HBTxyz PhysicalAverageVelocity; // Using HBTxyz
    HBTxyz ComovingMostBoundPosition; // Using HBTxyz
    HBTxyz PhysicalMostBoundVelocity; // Using HBTxyz
    long MostBoundParticleId;
    long SinkTrackId;
};


class Subhalo_Catalog {
    public:
        vector <Subhalo_input> Subhalos;
        long nsubhaloes;
        
        hid_t createSubDatatype_in();
        int readSubhalo(int isnap);
    
        void Clear();
    Subhalo_Catalog() {
        long zeros=0;
        Subhalos.resize(zeros);
        nsubhaloes=zeros;
    }
    
    Subhalo_Catalog(long nsub_tot) {
        Subhalos.resize(nsub_tot);
        nsubhaloes=nsub_tot;
    }
};
#endif