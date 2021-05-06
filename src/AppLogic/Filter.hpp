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

        // VecImage applyNoiseToImage(int image_id, noise_type_t noise_type, double value);

        void applyNoiseToWorkingImages();

        mat &getFmatrix()
        {
            return this->F_matrix;
        };

        vector<string> loadImagesFromFolder(string folder_path);
        vector<string> loadSingleImage(string folder_path);

        vector<string> loadImagesFromZip(string file_path);

        bool calculateFilter(size_t rank);

        bool saveToFile(string file_path);

        bool loadFmatrixFromFile(string file_path);

        bool saveAllDirtyImages(string folder_path);
        bool saveDirtyImage(int image_id, string folder_path);
        bool saveFilterInfo();
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
