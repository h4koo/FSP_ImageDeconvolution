#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include <string>
#include <armadillo>
#include <time.h>
#include <chrono>

#include "FilterInfo.hpp"
#include "VecImage.hpp"
#include "ImageLoader.hpp"
#include "libfrcima.hpp"

namespace AppLogic
{
    using arma::mat;
    using std::string;
    using std::vector;
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::seconds;

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
        vector<string> loaded_file_paths;
        vector<string> loaded_file_names;
        noise_type_t last_used_noise_type;
        size_t last_used_noise_value;
        bool is_canceled;
        mat F_matrix;

        mat getWorkingImagesMat();

    public:
        Filter();

        VecImage applyNoiseToImage(size_t image_id, size_t noise_value, noise_type_t noise_type = noise_type_t::GAUSSIAN);
        void applyNoiseToWorkingImages(size_t noise_value, noise_type_t noise_type = noise_type_t::GAUSSIAN);
        inline void setFilterName(string name)
        {
            this->calc_info.filter_name = name;
        }

        inline mat &getFmatrix()
        {
            return this->F_matrix;
        };
        inline FilterInfo *getFilterInfo()
        {
            return &this->calc_info;
        };

        inline void clearWorkingImages()
        {
            this->working_images.clear();
            this->loaded_file_paths.clear();
            this->loaded_file_names.clear();
        }

        vector<string> loadImagesFromFolder(string folder_path);
        string loadSingleImage(string folder_path);
        vector<string> loadImagesFromZip(string file_path);
        string getImagePath(size_t index);
        VecImage *getLoadedImage(const size_t index);
        bool calculateFilter(size_t rank, calc_method_t calc_method);
        bool saveToFile(string file_path = ImageLoader::FILTER_SAVE_LOCATION);
        bool loadFmatrixFromFile(string file_path);
        bool saveAllDirtyImages(string folder_path);
        bool saveDirtyImage(size_t image_id, string folder_path);
        bool saveFilterInfo(string folder_path = ImageLoader::FILTER_SAVE_LOCATION);
        bool exportFilter();
        bool importFilter(string file_path);
        void cancelSaveDirtyImages();
        static string getCalculationMethodName(calc_method_t calc_method);
    };

}

#endif
