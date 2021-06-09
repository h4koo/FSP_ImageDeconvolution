#ifndef FILTERLOGIC_H
#define FILTERLOGIC_H

#include "Filter.hpp"
#include <pthread.h>

// #define FILTER_SAVE_LOCATION "../../test/"

namespace AppLogic
{
    using std::string;
    using std::vector;


    class FilterLogic
    {
    private:
        Filter new_filter;
        noise_type_t last_noise;
        double last_noise_value;
        pthread_t filter_calc_thread;
        bool is_canceled = false;

    public:
        FilterLogic();
        vector<string> loadImagesFromFolder(string folder_path);
        vector<string> loadImagesFromZip(string file_path);

        string addImageToWorkingImages(string file_path);
        void clearWorkingImages();
        string getImagePath(const size_t index);
        VecImage * getLoadedImage(const size_t index);
        
        VecImage applyNoiseToImage(int image_id, noise_type_t noise, double noise_value);
        bool createFilter(string name, int rank, calc_method_t calc_method);
        void cancelFilterCreation();

        void setNoise(noise_type_t n_type){
            this->last_noise = n_type;
        };
        void setNoiseValue(double val){
            this->last_noise_value = val;
        };

        bool saveAllDirtyImages(string folder_path);
        void cancelSaveDirtyImages();
        bool saveDirtyImage(size_t image_id, string folder_path);

        FilterInfo * getFilterInfo();
    };

}
#endif

/// +loadImagesFromFolder(folder_path : string) : string[]
// +loadImagesFromZip(file_path : string) : string[]
// +createFilter(name, rank : unsigned int, calc_method : enum) : bool

// +deleteFilter(filter_id : int) : bool
// +exportFilter() : bool
// +importFilter(file_path : string) : bool
// +applyNoiseToImage(image_id : int, noise_type, value : double) : string
// +applyNoiseToAllImages(noise_type, value : double) : string[]
// +getNoisyImage(image_id : int)
// +saveDirtyImage(image_id : int, folder_path : string) : bool
// +saveAllDirtyImages(folder_path : string) : bool

// -new_filter : Filter
// hello
