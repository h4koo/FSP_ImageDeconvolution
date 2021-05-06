#ifndef FILTERLOGIC_H
#define FILTERLOGIC_H

#include "../../src/AppLogic/Filter.hpp"

class FilterLogic{
private:
    AppLogic::Filter new_filter; 
public:
    FilterLogic();
    std::string loadImagesFromFolder(std::string folder_path);
};

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

