#include "ImageCleaningLogic.hpp"



namespace AppLogic{
        vector<string> ImageCleaningLogic::getFilterNames(){
            this->filters = this->deconv.getLoadedFilters();
            vector<string> result;
            for (auto &&filt : this->filters)
            {
                result.push_back(filt.filter_name);
            }
            
            return result;
        }


        vector<string> ImageCleaningLogic::loadSingleImage(string file_path){
            return this->deconv.loadSingleImage(file_path);
        }


        vector<string> ImageCleaningLogic::loadImagesFromFolder(string folder_path){

            return this->deconv.loadImagesFromFolder(folder_path);
        }


        bool ImageCleaningLogic::loadFilter(int filter_id){
            return this->deconv.loadFilter(filter_id);
        }

        VecImage ImageCleaningLogic::applyFilterToImage(int image_id){
            return this->deconv.applyFilterToImage(image_id);
        }

        FilterInfo * ImageCleaningLogic::getFilterInfo(size_t indx){
            if(indx > this->filters.size()) return NULL;
            return &(this->filters[indx]);
        }
}