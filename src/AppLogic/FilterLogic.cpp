#include "FilterLogic.hpp"

namespace AppLogic{

    FilterLogic::FilterLogic(){

    }

        vector<string> FilterLogic::loadImagesFromFolder(string folder_path){

            return this->new_filter.loadImagesFromFolder(folder_path);

        }

        VecImage FilterLogic::applyNoiseToImage(int image_id, noise_type_t noise_type, double noise_value){
            this->last_noise = noise_type;
            this->last_noise_value = noise_value;
            return this->new_filter.applyNoiseToImage(image_id, noise_value, noise_type);

        }


        bool FilterLogic::createFilter(string name, int rank, calc_method_t calc_method){

            
            this->new_filter.applyNoiseToWorkingImages(this->last_noise_value, this->last_noise);

            
            if(!this->new_filter.calculateFilter(rank, calc_method)) return false;
            
            this->new_filter.setFilterName(name);
            this->new_filter.saveToFile(FILTER_SAVE_LOCATION);
            
            this->new_filter.saveFilterInfo(FILTER_SAVE_LOCATION);

            return true;


            


        }
}