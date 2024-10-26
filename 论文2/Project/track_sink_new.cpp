#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <omp.h>

#include "./src/io/subhalo_io.h"
#include "./src/io/halo_io.h"
using namespace std;

class Subhalo_input2
{
    public:
        long TrackId;
        long SinkTrackId;
        int SnapshotIndexOfSink;

        // Constructor
        Subhalo_input2()
        : TrackId(-1),
          SinkTrackId(-1),
          SnapshotIndexOfSink(-1)
        {}
};


class Subhalo_output
{
    public:
        long TrackId;
        long HostHaloId;
        long SinkTrackId;

        // Sink Moment
        float LastMaxMassOfSubhalo;

        HBTxyz SubhaloSinkPos;
        HBTxyz ParentSinkPos;
        HBTxyz HostSinkPos;

        int SubhaloSinkDepth;
        int ParentSinkDepth;

        float MassOfHost;
        float RadiusOfHost;

        int SnapshotIndexOfFirstAcc;
        int SnapshotIndexOfSink;

        // Pre-sink information
        float LastMaxMassOfParentBefore;
        float MboundOfSubhaloBefore;
        float MboundOfParentBefore;

        int SubhaloSinkDepthBefore;
        int ParentSinkDepthBefore;

        // Constructor
        Subhalo_output()
        : TrackId(-1),
          HostHaloId(-1),
          SinkTrackId(-1),
          
          // Info related to sink moment
          LastMaxMassOfSubhalo(0.0),
    
          SubhaloSinkPos({0, 0, 0}),
          ParentSinkPos({0, 0, 0}),
          HostSinkPos({0, 0, 0}),
    
          SubhaloSinkDepth(-1),
          ParentSinkDepth(-1),

          MassOfHost(0.0),
          RadiusOfHost(0.0),

          SnapshotIndexOfFirstAcc(-1),
          SnapshotIndexOfSink(-1),
    
          // Pre-sink information
          LastMaxMassOfParentBefore(0.0),
          MboundOfSubhaloBefore(0.0),
          MboundOfParentBefore(0.0),
          SubhaloSinkDepthBefore(-1),
          ParentSinkDepthBefore(-1)
        {}
};

class Subhalo_Catalog2 {
    public:
        vector <Subhalo_input2> Subhalos;
        long nsubhaloes;
    hid_t createSubDatatype_in();
    int readSubhalo(int isnap);
    
    Subhalo_Catalog2() {
        nsubhaloes=0;
        Subhalos.resize(nsubhaloes);
    }
    
    Subhalo_Catalog2(long nsub) {
        nsubhaloes=nsub;
        Subhalos.resize(nsubhaloes);
    }
};


hid_t Subhalo_Catalog2::createSubDatatype_in() {
    hid_t subhaloType = H5Tcreate(H5T_COMPOUND, sizeof(Subhalo_input2));

    // Add each member to the compound type
    H5Tinsert(subhaloType, "TrackId", HOFFSET(Subhalo_input2, TrackId), H5T_NATIVE_LONG);
    H5Tinsert(subhaloType, "SinkTrackId", HOFFSET(Subhalo_input2, SinkTrackId), H5T_NATIVE_LONG);
    H5Tinsert(subhaloType, "SnapshotIndexOfSink", HOFFSET(Subhalo_input2, SnapshotIndexOfSink), H5T_NATIVE_INT);
    return subhaloType;
}

int Subhalo_Catalog2::readSubhalo(int isnap) {
    char filename[100];
    //Read hdf5 first file
    sprintf(filename,"/home/wenkang/论文2/Part1/data/consistent_track/SubSnap_%03d.hdf5",isnap);
    cout<<"t.0"<<endl;
    hid_t dset,file = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    herr_t status;
    dset=H5Dopen2(file, "Subhalos", H5P_DEFAULT);
    if (dset < 0) {
        std::cerr << "Error opening dataset: Subhalos" << std::endl;
        H5Fclose(file);
        return 1;
    }
    cout<<"t.1"<<endl;
    hsize_t dims[1];
    GetDatasetDims(dset, dims);
    nsubhaloes=dims[0];
    Subhalos.resize(nsubhaloes);
    cout<<nsubhaloes<<" t.2"<<endl;
    //create datatype
    hid_t H5T_SubhaloInMem = createSubDatatype_in();
    if(nsubhaloes)	H5Dread(dset, H5T_SubhaloInMem, H5S_ALL, H5S_ALL, H5P_DEFAULT, Subhalos.data());
    cout<<"t.3"<<endl;
    H5Tclose(H5T_SubhaloInMem);
    H5Dclose(dset);
    H5Fclose(file);
    return 0;
}

class Output_Catalog {
    public:
        vector <Subhalo_output> Subhalos;
        long nsubhaloes;
    hid_t createSubDatatype_out();
    int SaveSubhalos(int isnap);

    Output_Catalog() {
        nsubhaloes=0;
        Subhalos.resize(nsubhaloes);
    }
    Output_Catalog(long nsub) {
        nsubhaloes=nsub;
        Subhalos.resize(nsubhaloes);
    }
};

int Output_Catalog::SaveSubhalos(int isnap) {
    //Output temporary array in test run
    char filename_out[100];
    sprintf(filename_out,"/home/wenkang/论文2/Part1/data/consistent_track/SubSnap_re_%03d.hdf5",isnap);
    hid_t file_out=H5Fcreate(filename_out, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    //create datatype
    hid_t subhalo_type = createSubDatatype_out();
    //Write data
    hsize_t dim_sub[1] = {nsubhaloes,};
    hid_t dataspace = H5Screate_simple(1, dim_sub, NULL);

    // Create a dataset using the compound datatype
    hid_t dataset = H5Dcreate(file_out, "Subhalos", subhalo_type, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if (dataset < 0) {
        std::cerr << "Error creating dataset." << std::endl;
        H5Fclose(file_out);
        return 1;
    }

    // Write the data to the dataset
    herr_t status = H5Dwrite(dataset, subhalo_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, Subhalos.data());
    if (status < 0) {
        std::cerr << "Error writing data to dataset." << std::endl;
    }

    // Clean up resources
    H5Dclose(dataset);
    H5Sclose(dataspace);
    H5Tclose(subhalo_type);
    H5Fclose(file_out);
    return 0;
}

inline hid_t Output_Catalog::createSubDatatype_out() {
    hid_t subhaloType = H5Tcreate(H5T_COMPOUND, sizeof(Subhalo_output));

    // Add each member to the compound type
    H5Tinsert(subhaloType, "TrackId", HOFFSET(Subhalo_output, TrackId), H5T_NATIVE_LONG);
    H5Tinsert(subhaloType, "HostHaloId", HOFFSET(Subhalo_output, HostHaloId), H5T_NATIVE_LONG);
    H5Tinsert(subhaloType, "SinkTrackId", HOFFSET(Subhalo_output, SinkTrackId), H5T_NATIVE_LONG);

    H5Tinsert(subhaloType, "LastMaxMassOfSubhalo", HOFFSET(Subhalo_output, LastMaxMassOfSubhalo), H5T_NATIVE_FLOAT);
    
    hsize_t dims_xyz[1] = {3};
    hid_t vec3Type = H5Tarray_create(H5T_NATIVE_FLOAT, 1, dims_xyz);
    H5Tinsert(subhaloType, "SubhaloSinkPos", HOFFSET(Subhalo_output, SubhaloSinkPos), vec3Type);
    H5Tinsert(subhaloType, "ParentSinkPos", HOFFSET(Subhalo_output, ParentSinkPos), vec3Type);
    H5Tinsert(subhaloType, "HostSinkPos", HOFFSET(Subhalo_output, HostSinkPos), vec3Type);

    H5Tinsert(subhaloType, "SubhaloSinkDepth", HOFFSET(Subhalo_output, SubhaloSinkDepth), H5T_NATIVE_INT);
    H5Tinsert(subhaloType, "ParentSinkDepth", HOFFSET(Subhalo_output, ParentSinkDepth), H5T_NATIVE_INT);

    H5Tinsert(subhaloType, "MassOfHost", HOFFSET(Subhalo_output, MassOfHost), H5T_NATIVE_FLOAT);
    H5Tinsert(subhaloType, "RadiusOfHost", HOFFSET(Subhalo_output, RadiusOfHost), H5T_NATIVE_FLOAT);

    H5Tinsert(subhaloType, "SnapshotIndexOfFirstAcc", HOFFSET(Subhalo_output, SnapshotIndexOfFirstAcc), H5T_NATIVE_INT);
    H5Tinsert(subhaloType, "SnapshotIndexOfSink", HOFFSET(Subhalo_output, SnapshotIndexOfSink), H5T_NATIVE_INT);

    H5Tinsert(subhaloType, "LastMaxMassOfParentBefore", HOFFSET(Subhalo_output, LastMaxMassOfParentBefore), H5T_NATIVE_FLOAT);
    H5Tinsert(subhaloType, "MboundOfSubhaloBefore", HOFFSET(Subhalo_output, MboundOfSubhaloBefore), H5T_NATIVE_FLOAT);
    H5Tinsert(subhaloType, "MboundOfParentBefore", HOFFSET(Subhalo_output, MboundOfParentBefore), H5T_NATIVE_FLOAT);

    H5Tinsert(subhaloType, "SubhaloSinkDepthBefore", HOFFSET(Subhalo_output, SubhaloSinkDepthBefore), H5T_NATIVE_INT);
    H5Tinsert(subhaloType, "ParentSinkDepthBefore", HOFFSET(Subhalo_output, ParentSinkDepthBefore), H5T_NATIVE_INT);

    H5Tclose(vec3Type);

    return subhaloType;
}

long findMaxIndex(const vector<long>& vec, long value) {
    auto it = upper_bound(vec.begin(), vec.end(), value);

    if (it == vec.begin()) {
        return -1; 
    }
    return distance(vec.begin(), it) - 1;
}

int main(int argc, char *argv[]) {
    int snapshot_start=127,snapshot_end=10; 
    long nsub_tot=0,nsub_this=0,nguess;
    
    if (argc == 2) {
        snapshot_start = atoi(argv[1]);
    }else if (argc == 3){
        snapshot_start = atoi(argv[1]);
        snapshot_end = atoi(argv[2]);
    }
    cout<<snapshot_start<<" "<<snapshot_end<<endl;

    Subhalo_Catalog Subhalos_in1;
    Subhalo_Catalog2 Subhalos_in2;
    
    Halo_Catalog Halos_in1;
    cout<<"1.0.0"<<endl;
    Subhalos_in1.readSubhalo(snapshot_start);
    nsub_tot = Subhalos_in1.nsubhaloes;
    cout<<nsub_tot<<endl;
    
    Subhalos_in2.readSubhalo(snapshot_start);
    cout<<"1.0.1"<<endl;
    Output_Catalog Subhalos_out(nsub_tot);
    cout<<"1.1"<<endl;
    
    nguess = nsub_tot*0.2;
    vector <long> Sink_Subhalo;
    Sink_Subhalo.reserve(nguess);
    cout<<nsub_tot<<Sink_Subhalo.size()<<endl;
    
    for (int isub=0;isub<nsub_tot;isub++) {
        Subhalos_out.Subhalos[isub].TrackId = isub;
        Subhalos_out.Subhalos[isub].SnapshotIndexOfSink = Subhalos_in2.Subhalos[isub].SnapshotIndexOfSink;
        Subhalos_out.Subhalos[isub].SinkTrackId = Subhalos_in2.Subhalos[isub].SinkTrackId;
        if (Subhalos_out.Subhalos[isub].SnapshotIndexOfSink!=-1) Sink_Subhalo.push_back(isub);
    }
    cout<<"1.2"<<endl;
        
    for (int isub=0;isub<5;isub++) {
        cout<<Subhalos_out.Subhalos[Sink_Subhalo[isub]].SnapshotIndexOfSink<<Subhalos_out.Subhalos[Sink_Subhalo[isub]].SinkTrackId<<endl;
    }
    ;
    for (int isnap_t=snapshot_start;isnap_t>=snapshot_end;isnap_t--) {
        
        long host_id=-1,parent_id=-1;
        Subhalos_in1.Clear();
        Halos_in1.Clear();
        
        //Read Present Catalog
        Subhalos_in1.readSubhalo(isnap_t);
        Halos_in1.readHalo(isnap_t);
        
        cout<<"1.3.1"<<endl;
        nsub_this = Subhalos_in1.nsubhaloes;
        long max_index_t = findMaxIndex(Sink_Subhalo,nsub_this);
        if (max_index_t<0) return -1;
        cout<<"1.3.2"<<endl;
        long nhalos=Halos_in1.nhalos;
        vector <long> host_cen;
        host_cen.resize(nhalos);
        #pragma omp parallel for
        for (int isub=0;isub<nsub_this;isub++) {
            if ((Subhalos_in1.Subhalos[isub].HostHaloId!=-1)&(Subhalos_in1.Subhalos[isub].Depth==0))
                host_cen[Subhalos_in1.Subhalos[isub].HostHaloId]=isub;
        }
        cout<<"1.3.3"<<endl;
        #pragma omp parallel private(parent_id,host_id)
        {
            #pragma omp for
            for (long isub_index=0;isub_index<=max_index_t;isub_index++) {
                long isub = Sink_Subhalo[isub_index];
                if (Subhalos_out.Subhalos[isub].SnapshotIndexOfSink==isnap_t) {
                    //Note that Hid[sub] can be possibly -1 which can cause segmentation fault
                    long HaloOfSub1 = Subhalos_in1.Subhalos[isub].HostHaloId;
                    if (HaloOfSub1==-1) cout<<"Sink in field halo"<<isub<<" "<<HaloOfSub1<<endl;
                    parent_id = Subhalos_out.Subhalos[isub].SinkTrackId;
                    host_id = host_cen[HaloOfSub1];//For the sink moment HostHaloId won't be -1
                    
                    Subhalos_out.Subhalos[isub].HostHaloId = HaloOfSub1;
                    Subhalos_out.Subhalos[isub].LastMaxMassOfSubhalo = Subhalos_in1.Subhalos[isub].LastMaxMass;

                    Subhalos_out.Subhalos[isub].SubhaloSinkPos = Subhalos_in1.Subhalos[isub].ComovingMostBoundPosition;
                    Subhalos_out.Subhalos[isub].ParentSinkPos = Subhalos_in1.Subhalos[parent_id].ComovingMostBoundPosition;
                    Subhalos_out.Subhalos[isub].HostSinkPos = Subhalos_in1.Subhalos[host_id].ComovingMostBoundPosition;                    

                    Subhalos_out.Subhalos[isub].SubhaloSinkDepth = Subhalos_in1.Subhalos[isub].Depth;
                    Subhalos_out.Subhalos[isub].ParentSinkDepth = Subhalos_in1.Subhalos[parent_id].Depth;
                    

                    Subhalos_out.Subhalos[isub].MassOfHost = Halos_in1.Halos[HaloOfSub1].M200Crit;
                    Subhalos_out.Subhalos[isub].RadiusOfHost = Halos_in1.Halos[HaloOfSub1].R200CritComoving;

                    Subhalos_out.Subhalos[isub].SnapshotIndexOfFirstAcc = Subhalos_in1.Subhalos[isub].SnapshotIndexOfLastIsolation;
                }
                 if (Subhalos_out.Subhalos[isub].SnapshotIndexOfSink==isnap_t+1) {
                    // For the moment just before sink, both Hid[sub] and Hid[parent] can possibly be -1 
                    // Be careful about the treatments to these issues
                    parent_id = Subhalos_out.Subhalos[isub].SinkTrackId;

                    Subhalos_out.Subhalos[isub].LastMaxMassOfParentBefore = Subhalos_in1.Subhalos[parent_id].LastMaxMass;
                    Subhalos_out.Subhalos[isub].MboundOfSubhaloBefore = Subhalos_in1.Subhalos[isub].Mbound;
                    Subhalos_out.Subhalos[isub].MboundOfParentBefore = Subhalos_in1.Subhalos[parent_id].Mbound;

                    Subhalos_out.Subhalos[isub].SubhaloSinkDepthBefore = Subhalos_in1.Subhalos[isub].Depth;
                    Subhalos_out.Subhalos[isub].ParentSinkDepthBefore = Subhalos_in1.Subhalos[parent_id].Depth;
                 }
            }
        }
        Subhalos_out.SaveSubhalos(snapshot_start);
        cout<<" The Snapshot "<<isnap_t<<" has been processed, Total count of subhaloes "<<nsub_this<<endl;
    }
}