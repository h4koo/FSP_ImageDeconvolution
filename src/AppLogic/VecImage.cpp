
#include "VecImage.hpp"

#define VECIMG_NORM 255

namespace AppLogic
{
    VecImage::VecImage()
    {
        this->cols = 0;
        this->rows = 0;
        this->source_file_location = "empty";
        this->data.clear();
    }

    VecImage::VecImage(std::string filename)
    {
        try
        {
            this->data = cimg_library::CImg<unsigned char>(filename.c_str());
            this->cols = data.width();
            this->rows = data.height();
            this->source_file_location = filename;
        }
        catch (...)
        {
            VecImage();
        }
    }

    VecImage::VecImage(CImg<unsigned char> &cImg)
    {
        this->data = *cImg;
        this->cols = cImg.width();
        this->rows = cImg.height();
        this->source_file_location = "";
    }

    VecImage::VecImage(const VecImage &vImg)
    {
        this->data = vImg.data;
        this->cols = vImg.cols;
        this->rows = vImg.rows;
        this->source_file_location = vImg.source_file_location;
    }

    VecImage::VecImage(std::vector<double> &img_data, size_t width, size_t height)
    {
        this->data = cimg_library::CImg<unsigned char>(width, height, 1, 1); //construct grayscale image

        size_t vec_it = 0;
        for (cimg_library::CImg<unsigned char>::iterator pix = this->data.begin(); pix < this->data.end(); ++pix)
        {
            char s = char(img_data[vec_it] * VECIMG_NORM);
            *pix = s;
            ++vec_it;
        }
        this->source_file_location = "internal";
        this->cols = width;
        this->rows = height;
    }

    VecImage::VecImage(const vec &img_data, size_t width, size_t height)
    {
        this->data = cimg_library::CImg<unsigned char>(width, height, 1, 1); //construct grayscale image

        size_t vec_it = 0;

        // Algorithm for calculating a value between a scale of [0-255]
        // ranged_value = 255 / (max-min) * (X - min)
        // where max and min are the maximun and minimun values present in the vector
        // and X is the value of each item in the vector

        double min = img_data.min();
        double mult_const = VECIMG_NORM / (img_data.max() - min);

        for (cimg_library::CImg<unsigned char>::iterator pix = data.begin(); pix < data.end(); ++pix)
        {
            char ranged_value = char(round(mult_const * (img_data(vec_it) - min)));
            *pix = ranged_value;
            ++vec_it;
        }
        this->source_file_location = "internal";
        this->cols = width;
        this->rows = height;
    }

    VecImage &VecImage::operator=(const VecImage &image)
    {
        // self-assignment guard
        if (this == &image)
            return *this;
        this->cols = image.cols;
        this->rows = image.rows;
        this->source_file_location = image.source_file_location;
        this->data = image.data;
        return *this;
    }

    const unsigned char *VecImage::getRawImageData()
    {
        return this->data._data;
    }

    std::vector<double> VecImage::getDoubleData()
    {
        std::vector<double> result(this->cols * this->rows);
        for (size_t y_coord = 0; y_coord < this->rows; y_coord++)
        {
            for (size_t x_coord = 0; x_coord < this->cols; x_coord++)
            {
                result[y_coord * this->cols + x_coord] = double(this->data(x_coord, y_coord)) / VECIMG_NORM;
            }
        }
        return result;
    }

    vec VecImage::getVecDoubleData()
    {
        vec result(this->cols * this->rows);
        for (size_t y_coord = 0; y_coord < this->rows; y_coord++)
        {
            for (size_t x_coord = 0; x_coord < this->cols; x_coord++)
            {
                result[y_coord * this->cols + x_coord] = double(data(x_coord, y_coord)) / VECIMG_NORM;
            }
        }
        return result;
    }

    // **NOTE** don't add whitespace to the names since it breaks FilterInfo loading
    string VecImage::getNoiseName(noise_type_t noise_type)
    {
        switch (noise_type)
        {
        case noise_type_t::GAUSSIAN:
            return "Gaussian";
        case noise_type_t::POISSON:
            return "Poisson";
        case noise_type_t::RICIAN:
            return "Rician";
        case noise_type_t::SALT_PEPPER:
            return "SaltAndPepper";
        case noise_type_t::UNIFORM:
            return "Uniform";
        default:
            return "NoiseName";
        }
    }

    mat VecImage::getMatDouble()
    {
        mat result(this->cols, this->rows);
        for (size_t col = 0; col < this->cols; ++col)
        {
            for (size_t row = 0; row < this->rows; ++row)
            {
                result(col, row) = double(this->data(col, row)) / VECIMG_NORM;
            }
        }
        return result;
    }

    void VecImage::setImgDataMatDouble(const mat &new_data)
    {
        for (size_t col = 0; col < this->cols; ++col)
        {
            for (size_t row = 0; row < this->rows; ++row)
            {
                this->data(col, row) = char(new_data(col, row) * VECIMG_NORM);
            }
        }
    }

    void VecImage::applyNoise(double noise_value, noise_type_t noise)
    {
        this->data.noise(noise_value, noise);
    }

    bool VecImage::save(const string filename)
    {
        try
        {
            this->data.save_png(filename.c_str());
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }

    size_t VecImage::numRows() { return this->rows; }

    size_t VecImage::numCols() { return this->cols; }

    string VecImage::getSourceFileLocation() { return this->source_file_location; }

}
