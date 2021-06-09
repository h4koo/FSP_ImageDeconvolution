#ifndef VECIMAGE_H
#define VECIMAGE_H

#define cimg_use_jpeg
#define cimg_use_png
#define cimg_display 0 // !! Important: Disables X11 libs so project can build with QT !!

#include <string>
#include <vector>
#include <armadillo>
#include <math.h>
// #include "Noise.hpp"

#include "CImg.h"

namespace AppLogic
{
    using arma::mat;
    using arma::vec;
    using std::string;
    using std::vector;
    using cimg_library::CImg;


     typedef enum
    {
        GAUSSIAN,
        SALT_PEPPER,
        UNIFORM,        
        POISSON,
        RICIAN
    } noise_type_t;




    class VecImage
    {
    private:
        string source_file_location;
        size_t cols;
        size_t rows;
        cimg_library::CImg<unsigned char> data;

    public:
        VecImage();
        VecImage(string filename);      
        VecImage(vector<double> &img_data, size_t width, size_t height);
        VecImage(const vec &img_data, size_t width, size_t height);
        VecImage(CImg<unsigned char> &cImg);
        VecImage(const VecImage &vImg);


        size_t num_rows(); // { return rows; };
        size_t num_cols(); // { return cols; };
        string getSourceFileLocation();

        vector<double> get_double_data();
    
        vec getVecDoubleData();
        mat getMatDouble();
        void applyNoise( double noise_value,noise_type_t noise= noise_type_t::GAUSSIAN);
        void display();
        void setImgDataMatDouble(const mat &new_data);
        const  unsigned char* getRawImageData(){
            return this->data._data;
        };

        static string getNoiseName(noise_type_t noise_type);

        bool save(const string filename);
    };
}
#endif
