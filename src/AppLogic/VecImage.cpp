
#include "VecImage.hpp"

#define VECIMG_NORM 255

namespace AppLogic
{
    VecImage::VecImage(){
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
    
    VecImage::VecImage(CImg<unsigned char> &cImg){
        this->data= *cImg;
        this->cols = cImg.width();
        this->rows = cImg.height();
        this->source_file_location="";

    }

    VecImage::VecImage(const VecImage &vImg){
        this->data= vImg.data;
        this->cols = vImg.cols;
        this->rows = vImg.rows;
        this->source_file_location=vImg.source_file_location;

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

        // Algorithm for calculating value between a scale of [0-255]
        // ranged_value = 255 / (max-min) * (X - min)
        // where max and min are the maximun and minimun values present in the vector
        // and X is the value of each item in the vector

        double min = img_data.min();
        double mult_const = VECIMG_NORM / (img_data.max() - min);

        for (cimg_library::CImg<unsigned char>::iterator pix = data.begin(); pix < data.end(); ++pix)
        {
            // char s = img_data[vec_it] > 0 ? char(img_data[vec_it] * VECIMG_NORM) : char(img_data[vec_it] * -VECIMG_NORM);
            char ranged_value = char(round(mult_const * (img_data(vec_it) - min)));
            *pix = ranged_value;
            ++vec_it;
        }
        this->source_file_location = "internal";
        this->cols = width;
        this->rows = height;
    }

    std::vector<double> VecImage::get_double_data()
    {
        std::vector<double> result(this->cols * this->rows);
        for (size_t y_coord = 0; y_coord < this->rows; y_coord++)
        {
            for (size_t x_coord = 0; x_coord < this->cols; x_coord++)
            {
                // unsigne char d = data.at(i, j);
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
                // unsigne char d = data.at(i, j);
                result[y_coord * this->cols + x_coord] = double(data(x_coord, y_coord)) / VECIMG_NORM;
            }
        }
        return result;
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
        // mat N(this->cols, this->rows, arma::fill::randn);
        // mat im_data = this->getMatDouble();
        // mat D = 0.05 * N + im_data;

        // this->setImgDataMatDouble(D);
        switch (noise)
        {
        case noise_type_t::GAUSSIAN :
            this->data.noise(noise_value);
            break;
        case noise_type_t::SALT_PEPPER :
            this->data.noise(noise_value,noise_type_t::SALT_PEPPER);
            break;
        
        default:
            break;
        }
         // = this->data.get_noise(10);
    }

    void VecImage::display()
    {
        this->data.display(source_file_location.c_str());
    }

    size_t VecImage::num_rows() { return this->rows; }
    size_t VecImage::num_cols() { return this->cols; }
    string VecImage::getSourceFileLocation() {return this->source_file_location;}

}
