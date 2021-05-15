#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <string>
#include <armadillo>

#include "FilterInfo.hpp"
#include "VecImage.hpp"
// #include "Noise.hpp"
#include "ImageLoader.hpp"
#include "libfrcima.hpp"

namespace AppLogic
{
    using arma::mat;
    using std::string;
    using std::vector;

    typedef enum
    {
        RCIMA_METHOD,
        FAST_RCIMA_METHOD
    } calc_method_t;


    class Filter
    {
    private:
        FilterInfo calc_info;
        vector<VecImage> working_images;
        mat image_set;
        mat F_matrix;

        mat getWorkingImagesMat();

    public:
        Filter();

        VecImage applyNoiseToImage(size_t image_id, double noise_value, noise_type_t noise_type = noise_type_t::GAUSSIAN);

        void applyNoiseToWorkingImages(double noise_value, noise_type_t noise_type = noise_type_t::GAUSSIAN);
        inline void setFilterName(string name){
            this->calc_info.filter_name = name;
        }

        mat &getFmatrix()
        {
            return this->F_matrix;
        };
        FilterInfo getFilterInfo(){
            return this->calc_info;
        };

        vector<string> loadImagesFromFolder(string folder_path);
        vector<string> loadSingleImage(string folder_path);

        vector<string> loadImagesFromZip(string file_path);

        bool calculateFilter(size_t rank, calc_method_t calc_method);

        bool saveToFile(string file_path = FILTER_SAVE_LOCATION);

        bool loadFmatrixFromFile(string file_path);

        bool saveAllDirtyImages(string folder_path);
        bool saveDirtyImage(size_t image_id, string folder_path);
        bool saveFilterInfo(string folder_path = FILTER_SAVE_LOCATION);
        bool exportFilter();
        bool importFilter(string file_path);
    };

}

#endif

// +applyNoiseToImage(image_id : int, noise_type, value : double) : VecImage
// +applyNoiseToWorkingImages() : VecImage[]
// +loadImagesFromFolder(folder_path : string) : string[]
// +loadImagesFromZip(file_path : string) : string[]
// +getNoisyImage(image_id : int)
// +calculateFilter(rank : unsigned int, calc_method : enum)
// +saveToFile(name : string, file_path : string)
// +saveAllDirtyImages(folder_path : string) : bool
// +saveDirtyImage(image_id : int, folder_path : string) : bool
// -saveFilterInfo()
// +exportFilter() : bool
// +importFilter(file_path : string) : bool

// -calc_info : FilterInfo
// -working_images : VecImage[]
// -working_noise : Noise
// -image_set :double[][]
// -f_matrix : double[][]
