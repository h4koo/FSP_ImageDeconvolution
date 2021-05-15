#include "ImageLoader.hpp"

namespace AppLogic
{
    using namespace cimg_library;
    namespace fs = std::experimental::filesystem;

    VecImage ImageLoader::loadSingleImage(const char *filename)
    {
        // check if file exists
       
            string full_path = fs::canonical(filename);

            // VecImage returns empty picture if not able to load
            VecImage result(full_path);

            
        this->source_directory = fs::path(full_path).parent_path();
        this->loaded_file_paths.push_back(full_path);
                            this->loaded_file_names.push_back(fs::path(full_path).filename());
        
        return result;
      
        
    }

    vector<VecImage> ImageLoader::loadImagesFromFolder(const char *folder_path)
    {

        this->clearLoadedData();
        vector<VecImage> result;

        size_t first_image_col = 0;
        size_t first_image_row = 0;
        bool first_image = true;

        try
        {
            fs::directory_iterator it = fs::directory_iterator(fs::canonical(folder_path));
            for (const auto &entry : it)
            {
                if (isSupportedImageFormat(entry.path()))
                {
                    string full_path = fs::canonical(entry);
                    VecImage ld_image(full_path);
                    if (ld_image.num_cols() != 0)
                    {
                        if (first_image)
                        {
                            first_image_col = ld_image.num_cols();
                            first_image_row = ld_image.num_rows();
                            first_image = false;
                            this->source_directory = entry.path().parent_path();
                        }

                        if (first_image_col == ld_image.num_cols() && first_image_row == ld_image.num_rows())
                        {
                            this->loaded_file_paths.push_back(full_path);
                            this->loaded_file_names.push_back(entry.path().filename());
                            result.push_back(ld_image);
                        }
                        else
                        {
                            this->not_loaded_files.push_back(full_path);
                        }
                    }
                }
            }
            return result;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            result.clear();
            return result;
        }
    }

    vector<FilterInfo> ImageLoader::loadFilterInfo(const char *folder_path)
    {
        this->clearLoadedData();
        vector<FilterInfo> result;

        try
        {
            for (const auto &entry : fs::directory_iterator(fs::canonical(folder_path)))
            {

                if (entry.path().extension() == FILTER_INFO_EXTENSION)
                {
                    fstream fi_binary_file(entry.path(), std::ios::in | std::ios::binary | std::ios::app);
                    FilterInfo fi;
                    if (fi_binary_file.is_open())
                    {
                        fi_binary_file >> fi;
                        fi_binary_file.close();

                        result.push_back(fi);
                        this->loaded_file_names.push_back(entry.path().filename());
                        this->loaded_file_paths.push_back(fs::canonical(entry));
                    }
                }
            }

            return result;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            result.clear();
            return result;
        }
    }

    vector<string> ImageLoader::getLoadedImageNames()
    {
        return this->loaded_file_names;
    }

    vector<string> ImageLoader::getLoadedImagePaths()
    {
        return this->loaded_file_paths;
    }
    vector<string> ImageLoader::getNotLoadedImagePaths()
    {
        return this->not_loaded_files;
    }
    string ImageLoader::getSourceDirectory()
    {
        return this->source_directory;
    }
}

// +loadImagesFromFolder(folder_path) : VecImage[]
// +loadImagesFromZip(file_path) : VecImage[]
// +loadSingleImage(file_path : string) : VecImage
// +saveImageToFile(file_path : string, image : VecImage) : bool
// +saveMultipleImagesToFile(file_path : string, image : VecImage) : bool
