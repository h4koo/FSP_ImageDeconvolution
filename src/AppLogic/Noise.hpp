#ifndef NOISE_H
#define NOISE_H

#include "VecImage.hpp"

namespace AppLogic
{

    typedef enum
    {
        MULT,
        GAUSS,
        SLT_PP
    } noise_type_t;

    class Noise
    {

    public:
        virtual void applyNoise(VecImage &img) = 0;
    };

    class RandomNoise : public Noise
    {

    public:
        void applyNoise(VecImage &img){
            // //generate random vector
            // for (cimg_library::CImg<unsigned double>::iterator pix = data.begin(); pix < data.end(); ++pix)
            // {
            //     double a = ((rand() % 100) + 1) / 101;
            //     char s = char(img_data[vec_it] * 255);
            //     *pix = s;
            //     ++vec_it;
            // }

            // for (size_t i = 0; i < img->size(); ++i)
            // {
            //     double a = ((rand() % 100) + 1);
            //     a /= 100;
            //     distorted_vecim.push_back(og_vecimg[i] + a);
            // }
        };
    };

}

#endif

// +applyNoise(image : char[])

// -noise_type : int
// -noise_name : string
// -noise_value : double

// MultiplicativeNoise

// GaussianNoise

// SaltPepperNoise
