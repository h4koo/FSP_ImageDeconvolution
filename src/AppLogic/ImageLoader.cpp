#include "ImageLoader.hpp"

namespace AppLogic
{
    bool AppLogic::ImageLoader::_is_canceled = false;
    const string ImageLoader::FILTER_SAVE_LOCATION = string(getenv("HOME")) + "/.fsp_imgdcnv/";

    using namespace cimg_library;

    void ImageLoader::addImageFileInfo(const string &full_path)
    {
        this->loaded_file_paths.push_back(full_path);
        string only_filename = fs::path(full_path).filename();
        size_t last_index = only_filename.find_last_of(".");
        only_filename = only_filename.substr(0, last_index);
        this->loaded_file_names.push_back(only_filename);
    }

    void ImageLoader::cancelOperation(bool status)
    {
        _is_canceled = status;
    }

    VecImage ImageLoader::loadSingleImage(const char *filename)
    {

        string full_path = fs::canonical(filename);

        // VecImage returns empty picture if not able to load
        VecImage result(full_path);

        this->source_directory = fs::path(full_path).parent_path();
        this->addImageFileInfo(full_path);

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
                if (_is_canceled)
                {
                    _is_canceled = false;
                    return result;
                }
                if (isSupportedImageFormat(entry.path()))
                {
                    string full_path = fs::canonical(entry);
                    VecImage ld_image(full_path);
                    if (ld_image.numCols() != 0)
                    {
                        if (first_image)
                        {
                            first_image_col = ld_image.numCols();
                            first_image_row = ld_image.numRows();
                            first_image = false;
                            this->source_directory = entry.path().parent_path();
                        }

                        if (first_image_col == ld_image.numCols() && first_image_row == ld_image.numRows())
                        {
                            this->addImageFileInfo(full_path);
                            result.push_back(ld_image);
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

    vector<FilterInfo> ImageLoader::loadFilterInfo()
    {
        this->clearLoadedData();
        vector<FilterInfo> result;
        try
        {
            for (const auto &entry : fs::directory_iterator(fs::canonical(FILTER_SAVE_LOCATION)))
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

    bool ImageLoader::deleteFilter(const string filter_name)
    {

        // remove filter info file
        string path = FILTER_SAVE_LOCATION + filter_name + FILTER_INFO_EXTENSION;
        string full_path = fs::canonical(path);
        if (remove(full_path.c_str()) != 0)
            return false;

        //remove the filter file
        path = FILTER_SAVE_LOCATION + filter_name + FILTER_FILE_EXTENSION;
        full_path = fs::canonical(path);
        if (remove(full_path.c_str()) != 0)
            return false;

        return true;
    }

    bool ImageLoader::existsNameFilter(string filter_name)
    {

        try
        {
            string full_filter_name = FILTER_SAVE_LOCATION + filter_name + FILTER_INFO_EXTENSION;
            return fs::exists(full_filter_name);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    bool ImageLoader::createFileFromZipFile(string filename, fstream &stream, zip_file *zip_file, size_t buff_size)
    {

        stream.open(filename.c_str(), std::ios::trunc | std::ios::out | std::ios::in);
        if (stream.bad())
            return false;

        char file_buff[ZIP_FILE_BUFF_SIZE];
        int read_bytes;
        try{
        while ((read_bytes = zip_fread(zip_file, file_buff, ZIP_FILE_BUFF_SIZE)) > 0)
        {
            if (_is_canceled)
            {
                _is_canceled = false;
                stream.close();
                zip_fclose(zip_file);
                remove(filename.c_str());
                return false;
            }
            stream.write(file_buff, read_bytes);
        }
        stream.close();
        zip_fclose(zip_file);
        return true;
        }

        catch (...){
            stream.close();
            zip_fclose(zip_file);
            remove(filename.c_str());
            return false;
        }
    }

    vector<VecImage> ImageLoader::loadImagesFromZip(string file_path)
    {
        this->clearLoadedData();
        vector<VecImage> result;

        struct zip *z_arch;
        struct zip_file *z_fl;
        struct zip_stat z_fstat;

        int err;

        if ((z_arch = zip_open(file_path.c_str(), 0, &err)) == NULL)
        {
            // unable to open zip archive
            return result;
        }

        size_t first_image_col = 0;
        size_t first_image_row = 0;
        bool first_image = true;

        fstream temp_file;
        string temp_filename = FILTER_SAVE_LOCATION + string(TEMP_FILE_NAME);
        int num_entries = zip_get_num_entries(z_arch, 0);
        for (int i = 0; i < num_entries; ++i)
        {
            if (_is_canceled)
            {
                _is_canceled = false;
                return result;
            }
            if (zip_stat_index(z_arch, i, 0, &z_fstat) != 0)
                continue;

            if (!isSupportedImageFormat(z_fstat.name))
                continue;

            z_fl = zip_fopen_index(z_arch, i, 0);
            if (!z_fl)
            {
                printf("Could not open zip file: %s", z_fstat.name);
                continue;
            }

            if (!createFileFromZipFile(temp_filename, temp_file, z_fl, z_fstat.size))
            {
                printf("Couldn't load temp file");
                continue;
            }

            VecImage ld_image(temp_filename);
            if (ld_image.numCols() == 0)
                continue;

            if (first_image)
            {
                first_image_col = ld_image.numCols();
                first_image_row = ld_image.numRows();
                first_image = false;
                this->source_directory = file_path;
            }

            if (first_image_col == ld_image.numCols() && first_image_row == ld_image.numRows())
            {
                this->addImageFileInfo(z_fstat.name);
                result.push_back(ld_image);
            }
        }
        zip_close(z_arch);
        return result;
    }

    bool ImageLoader::createExportZip(string export_name, string filter_name)
    {
        if (!existsNameFilter(filter_name))
            return false;

        int errorp;
        zip_t *zipper = zip_open(export_name.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
        if (zipper == nullptr)
        {
            zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
            printf("Failed to open output file %s : %s", export_name.c_str(), zip_error_strerror(&ziperror));
            return false;
        }

        // write file_info file
        string filename = FILTER_SAVE_LOCATION + filter_name + FILTER_INFO_EXTENSION;
        zip_source_t *source = zip_source_file(zipper, filename.c_str(), 0, 0);
        if (source == nullptr)
        {
            return false;
        }

        if (zip_file_add(zipper, (filter_name + FILTER_INFO_EXTENSION).c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
        {
            zip_source_free(source);
            zip_discard(zipper);
            return false;
        }
        if (_is_canceled)
        {
            _is_canceled = false;
            zip_discard(zipper);
            return false;
        }

        // write filter_matrix file to archive
        filename = FILTER_SAVE_LOCATION + filter_name + FILTER_FILE_EXTENSION;
        source = zip_source_file(zipper, filename.c_str(), 0, 0);
        if (source == nullptr)
        {
            return false;
        }
        if (zip_file_add(zipper, (filter_name + FILTER_FILE_EXTENSION).c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
        {
            zip_source_free(source);
            zip_discard(zipper);
            return false;
        }

        if (zip_close(zipper) < -1)
        {
            printf("Failed to close zipfile %s : %s", export_name.c_str(), zip_strerror(zipper));
            return false;
        };

        return true;
    }

    bool ImageLoader::importFilterFromZip(string import_filename)
    {
        struct zip *z_arch;
        struct zip_stat z_fstat;
        int zfile_finfo_index = -1;
        int zfile_fdata_index = -1;

        string finfo_filename = "";
        size_t finfo_filesize;
        string fdata_filename = "";
        size_t fdata_filesize;
        int err;

        if ((z_arch = zip_open(import_filename.c_str(), 0, &err)) == NULL)
        {
            // unable to open zip archive
            return false;
        }

        int num_entries = zip_get_num_entries(z_arch, 0);
        for (int i = 0; i < num_entries; ++i)
        {
            if (zip_stat_index(z_arch, i, 0, &z_fstat) != 0)
                continue;
            if (zfile_fdata_index != -1 && zfile_finfo_index != -1)
                break;
            if (endsWith(z_fstat.name, FILTER_INFO_EXTENSION) && zfile_finfo_index == -1)
            {
                zfile_finfo_index = i;
                finfo_filename = stripExtensionFromFilename(z_fstat.name);
                finfo_filesize = z_fstat.size;
            }

            if (endsWith(z_fstat.name, FILTER_FILE_EXTENSION) && zfile_fdata_index == -1)
            {
                zfile_fdata_index = i;
                fdata_filename = stripExtensionFromFilename(z_fstat.name);
                fdata_filesize = z_fstat.size;
            }
        }

        // filter_info file or filter_matrix file were not found
        if (zfile_fdata_index == -1 || zfile_finfo_index == -1)
        {
            zip_close(z_arch);
            return false;
        }
        // the names of the files are different
        if (fdata_filename.compare(finfo_filename) != 0)
        {
            zip_close(z_arch);
            return false;
        }
        // a filter with tha name already exists
        if (existsNameFilter(fdata_filename))
        {
            zip_close(z_arch);
            return false;
        }
        if (_is_canceled)
        {
            _is_canceled = false;
            zip_close(z_arch);
            return false;
        }

        fstream fout;
        string filename_finfo = FILTER_SAVE_LOCATION + finfo_filename + FILTER_INFO_EXTENSION;
        if (!ImageLoader::createFileFromZipFile(filename_finfo, fout, zip_fopen_index(z_arch, zfile_finfo_index, 0), finfo_filesize))
        {
            zip_close(z_arch);
            return false;
        }
        if (_is_canceled)
        {
            _is_canceled = false;
            zip_close(z_arch);
            remove(filename_finfo.c_str());
            return false;
        }
        string filename_fdata = FILTER_SAVE_LOCATION + fdata_filename + FILTER_FILE_EXTENSION;
        if (!ImageLoader::createFileFromZipFile(filename_fdata, fout, zip_fopen_index(z_arch, zfile_fdata_index, 0), fdata_filesize))
        {
            remove(filename_finfo.c_str());
            zip_close(z_arch);
            return false;
        }
        zip_close(z_arch);
        return true;
    }

}
