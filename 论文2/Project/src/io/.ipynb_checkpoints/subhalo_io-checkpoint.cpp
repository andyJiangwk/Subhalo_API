#include "subhalo_io.h"

hid_t Subhalo_Catalog::createSubDatatype_in() {
    hsize_t dims_xyz[1] = {3}; // For 1D arrays of size 3
    hsize_t dims_matrix[2] = {3, 3}; // For 2D matrices of size 3x3

    hid_t H5T_SubhaloInMem = H5Tcreate(H5T_COMPOUND, sizeof(Subhalo_input));

    // Insert all fields
    H5Tinsert(H5T_SubhaloInMem, "TrackId", HOFFSET(Subhalo_input, TrackId), H5T_NATIVE_LONG);
    H5Tinsert(H5T_SubhaloInMem, "Nbound", HOFFSET(Subhalo_input, Nbound), H5T_NATIVE_LONG);
    H5Tinsert(H5T_SubhaloInMem, "Mbound", HOFFSET(Subhalo_input, Mbound), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "HostHaloId", HOFFSET(Subhalo_input, HostHaloId), H5T_NATIVE_LONG);
    H5Tinsert(H5T_SubhaloInMem, "Rank", HOFFSET(Subhalo_input, Rank), H5T_NATIVE_LONG);
    H5Tinsert(H5T_SubhaloInMem, "Depth", HOFFSET(Subhalo_input, Depth), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "LastMaxMass", HOFFSET(Subhalo_input, LastMaxMass), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "SnapshotIndexOfLastMaxMass", HOFFSET(Subhalo_input, SnapshotIndexOfLastMaxMass), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "SnapshotIndexOfLastIsolation", HOFFSET(Subhalo_input, SnapshotIndexOfLastIsolation), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "SnapshotIndexOfBirth", HOFFSET(Subhalo_input, SnapshotIndexOfBirth), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "SnapshotIndexOfDeath", HOFFSET(Subhalo_input, SnapshotIndexOfDeath), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "SnapshotIndexOfSink", HOFFSET(Subhalo_input, SnapshotIndexOfSink), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "RmaxComoving", HOFFSET(Subhalo_input, RmaxComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "VmaxPhysical", HOFFSET(Subhalo_input, VmaxPhysical), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "LastMaxVmaxPhysical", HOFFSET(Subhalo_input, LastMaxVmaxPhysical), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "SnapshotIndexOfLastMaxVmax", HOFFSET(Subhalo_input, SnapshotIndexOfLastMaxVmax), H5T_NATIVE_INT);
    H5Tinsert(H5T_SubhaloInMem, "R2SigmaComoving", HOFFSET(Subhalo_input, R2SigmaComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "RHalfComoving", HOFFSET(Subhalo_input, RHalfComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "BoundR200CritComoving", HOFFSET(Subhalo_input, BoundR200CritComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "BoundM200Crit", HOFFSET(Subhalo_input, BoundM200Crit), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "SpecificSelfPotentialEnergy", HOFFSET(Subhalo_input, SpecificSelfPotentialEnergy), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_SubhaloInMem, "SpecificSelfKineticEnergy", HOFFSET(Subhalo_input, SpecificSelfKineticEnergy), H5T_NATIVE_FLOAT);

    // For 3-element arrays
    hid_t array3type = H5Tarray_create2(H5T_NATIVE_FLOAT, 1, dims_xyz);
    H5Tinsert(H5T_SubhaloInMem, "SpecificAngularMomentum", HOFFSET(Subhalo_input, SpecificAngularMomentum), array3type);
    H5Tinsert(H5T_SubhaloInMem, "ComovingAveragePosition", HOFFSET(Subhalo_input, ComovingAveragePosition), array3type);
    H5Tinsert(H5T_SubhaloInMem, "PhysicalAverageVelocity", HOFFSET(Subhalo_input, PhysicalAverageVelocity), array3type);
    H5Tinsert(H5T_SubhaloInMem, "ComovingMostBoundPosition", HOFFSET(Subhalo_input, ComovingMostBoundPosition), array3type);
    H5Tinsert(H5T_SubhaloInMem, "PhysicalMostBoundVelocity", HOFFSET(Subhalo_input, PhysicalMostBoundVelocity), array3type);

    // For 3x3 matrices
    hid_t matrix3x3type = H5Tarray_create2(H5T_NATIVE_FLOAT, 2, dims_matrix);
    H5Tinsert(H5T_SubhaloInMem, "InertialEigenVector", HOFFSET(Subhalo_input, InertialEigenVector), matrix3x3type);
    H5Tinsert(H5T_SubhaloInMem, "InertialEigenVectorWeighted", HOFFSET(Subhalo_input, InertialEigenVectorWeighted), matrix3x3type);

    // InertialTensor and InertialTensorWeighted as 1D arrays
    hsize_t dims_tensor[1] = {6};
    hid_t tensorType = H5Tarray_create2(H5T_NATIVE_FLOAT, 1, dims_tensor);
    
    H5Tinsert(H5T_SubhaloInMem, "InertialTensor", HOFFSET(Subhalo_input, InertialTensor), tensorType);
    H5Tinsert(H5T_SubhaloInMem, "InertialTensorWeighted", HOFFSET(Subhalo_input, InertialTensorWeighted), tensorType);

    H5Tinsert(H5T_SubhaloInMem, "MostBoundParticleId", HOFFSET(Subhalo_input, MostBoundParticleId), H5T_NATIVE_LONG);
    H5Tinsert(H5T_SubhaloInMem, "SinkTrackId", HOFFSET(Subhalo_input, SinkTrackId), H5T_NATIVE_LONG);

    return H5T_SubhaloInMem; // Return the created datatype
}

int Subhalo_Catalog::readSubhalo(int isnap) {

    char filename[100];
    //Read hdf5 first file
    sprintf(filename,"/home/cossim/CosmicGrowth/6610/subcat2/SubSnap_%03d.hdf5",isnap);
    hid_t dset,file = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    herr_t status;
    dset=H5Dopen2(file, "Subhalos", H5P_DEFAULT);
    if (dset < 0) {
        std::cerr << "Error opening dataset: Subhalos" << std::endl;
        H5Fclose(file);
        return 1;
    }

    hsize_t dims[1];
    GetDatasetDims(dset, dims);
    nsubhaloes=dims[0];
    Subhalos.resize(nsubhaloes);
    
    //create datatype
    hid_t H5T_SubhaloInMem = createSubDatatype_in();
    if(nsubhaloes)	H5Dread(dset, H5T_SubhaloInMem, H5S_ALL, H5S_ALL, H5P_DEFAULT, Subhalos.data());

    H5Tclose(H5T_SubhaloInMem);
    H5Dclose(dset);
    H5Fclose(file);
    return 0;
}

void Subhalo_Catalog::Clear() {
    nsubhaloes = 0;
    Subhalos.resize(nsubhaloes);
}