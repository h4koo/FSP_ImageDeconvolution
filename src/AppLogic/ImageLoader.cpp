#include "ImageLoader.hpp"

#include <experimental/filesystem>

namespace AppLogic
{
    using std::string;
    using std::vector;
    using namespace cimg_library;
    namespace fs = std::experimental::filesystem;

    struct AcceptedFileFormats
    {
        /* data */
    };

    static bool endsWith(const string &str, const string &suffix);
    static bool isSupportedImageFormat(const string &str);

    VecImage ImageLoader::loadSingleImage(const char *filename)
    {
        // check if file exists

        // VecImage result(filename);
        return VecImage(filename);
    }

    vector<VecImage> ImageLoader::loadImagesFromFolder(const char *folder_path)
    {

        vector<VecImage> result;
        this->loaded_images.clear();
        // VecImage ld_image;
        // std::string path = "/path/to/directory";
        // this->loaded_images.push_back(fs::canonical(folder_path));
        size_t first_image_size = 0;
        bool first_image = true;
        for (const auto &entry : fs::directory_iterator(folder_path))
        {

            // std::cout << entry.path() << std::endl;
            // string filename =
            if (isSupportedImageFormat(entry.path()))
            {
                VecImage ld_image = VecImage(fs::canonical(entry));
                if (ld_image.num_cols() != 0)
                {
                    if (first_image)
                    {
                        first_image_size = ld_image.num_cols() * ld_image.num_rows();
                        first_image = false;
                    }

                    if (first_image_size == ld_image.num_cols() * ld_image.num_rows())
                    {
                        this->loaded_images.push_back(fs::canonical(entry));
                        result.push_back(ld_image);
                    }
                    else
                    {
                        this->unloaded_images.push_back(fs::canonical(entry));
                    }
                }
            }
        }
        return result;
    }

    static bool isSupportedImageFormat(const string &str)
    {
        return endsWith(str, ".jpg") || endsWith(str, ".bmp") || endsWith(str, ".png");
    }

    static bool endsWith(const std::string &str, const std::string &suffix)
    {
        return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
    }

    vector<string> ImageLoader::getLoadedImageNames()
    {
        return this->loaded_images;
    }
}

// +loadImagesFromFolder(folder_path) : VecImage[]
// +loadImagesFromZip(file_path) : VecImage[]
// +loadSingleImage(file_path : string) : VecImage
// +saveImageToFile(file_path : string, image : VecImage) : bool
// +saveMultipleImagesToFile(file_path : string, image : VecImage) : bool
