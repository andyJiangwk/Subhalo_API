#include "halo_io.h"
hid_t Halo_Catalog::createHaloDatatype_in() {
    hsize_t dims_xyz[1] = {3};  // For the HBTxyz array (size 3)
    
    // Create compound datatype for the Halo_input structure
    hid_t H5T_HaloInMem = H5Tcreate(H5T_COMPOUND, sizeof(Halo_input));

    // Insert all fields
    H5Tinsert(H5T_HaloInMem, "HaloId", HOFFSET(Halo_input, HaloId), H5T_NATIVE_INT);
    
    // For HBTxyz (3-element array)
    hid_t array3type = H5Tarray_create2(H5T_NATIVE_FLOAT, 1, dims_xyz);
    H5Tinsert(H5T_HaloInMem, "CenterComoving", HOFFSET(Halo_input, CenterComoving), array3type);
    
    // Insert the remaining float fields
    H5Tinsert(H5T_HaloInMem, "R200CritComoving", HOFFSET(Halo_input, R200CritComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "R200MeanComoving", HOFFSET(Halo_input, R200MeanComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "RVirComoving", HOFFSET(Halo_input, RVirComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "M200Crit", HOFFSET(Halo_input, M200Crit), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "M200Mean", HOFFSET(Halo_input, M200Mean), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "MVir", HOFFSET(Halo_input, MVir), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "RmaxComoving", HOFFSET(Halo_input, RmaxComoving), H5T_NATIVE_FLOAT);
    H5Tinsert(H5T_HaloInMem, "VmaxPhysical", HOFFSET(Halo_input, VmaxPhysical), H5T_NATIVE_FLOAT);

    return H5T_HaloInMem;  // Return the created datatype
}

int Halo_Catalog::readHalo(int isnap) {
    char filename[100];
    sprintf(filename,"/home/cossim/CosmicGrowth/6610/subcat2/HaloSize/HaloSize_%d.hdf5",isnap);
    
    hid_t file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        std::cerr << "Error opening file." << std::endl;\
    }

    // Open the dataset
    hid_t dataset_id = H5Dopen(file_id, "HostHalos", H5P_DEFAULT);
    if (dataset_id < 0) {
        std::cerr << "Error opening dataset." << std::endl;
        H5Fclose(file_id);
    }

    // Get the dataspace
    hid_t dataspace_id = H5Dget_space(dataset_id);
    hsize_t nhalos_this;
    H5Sget_simple_extent_dims(dataspace_id, &nhalos_this, nullptr);

    // Resize the vector to hold all halos
    nhalos = nhalos_this;
    Halos.resize(nhalos);
    
    // Create Datatype
    hid_t halo_mem_type = createHaloDatatype_in();
    // Read the data from the dataset
    H5Dread(dataset_id, halo_mem_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, Halos.data());

    // Cleanup
    H5Tclose(halo_mem_type);
    H5Sclose(dataspace_id);
    H5Dclose(dataset_id);
    H5Fclose(file_id);
    return 0;
}

void Halo_Catalog::Clear() {
    nhalos=0;
    Halos.resize(nhalos);
}