#include "Filter.hpp"

#include <iostream>

using namespace AppLogic;
using arma::mat;
using arma::vec;
using std::vector;

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    string filename;
    if (argc < 2)
        filename = "./test/images/saturno.jpg";
    else
        filename = argv[1];

    ImageLoader il;

    Filter filter;

    filter.loadImagesFromFolder("./test/images/imagen_nasa_saturno");
    // filter.loadSingleImage(filename);

    filter.applyNoiseToWorkingImages(0.5,noise_type_t::GAUSSIAN);

    filter.calculateFilter(1000,calc_method_t::FAST_RCIMA_METHOD);

    // filter.saveToFile("./test/fmatrix.bin");
    // filter.loadFmatrixFromFile("./test/fmatrix.bin");

    mat filter_mat = filter.getFmatrix();

    VecImage og_img = il.loadSingleImage(filename.c_str());

    og_img.display();

    og_img.applyNoise(0.5, noise_type_t::GAUSSIAN);

    og_img.display();

    vec og_vec = og_img.getVecDoubleData();
    // og_vec.print();
    vec filtered_vec = filter_mat * og_vec;

    VecImage filtered_image(filtered_vec, og_img.num_cols(), og_img.num_rows());
    // VecImage filtered_image(og_vec, og_img.num_cols(), og_img.num_rows());

    filtered_image.display();

    // filtered_vec.print();

    // cout << "Tamaño de un double: " << sizeof(double) << endl;
    // cout << "Tamaño de un float: " << sizeof(float) << endl;

    // vector<double> og_vecimg, distorted_vecim;
    // og_vecimg = og_img.get_double_data();

    // //generate random vector
    // for (size_t i = 0; i < og_vecimg.size(); ++i)
    // {
    //     double a = ((rand() % 100) + 1);
    //     a /= 100;
    //     distorted_vecim.push_back(og_vecimg[i] + a);
    // }

    // VecImage dirty_img(distorted_vecim, og_img.num_cols(), og_img.num_rows());

    // dirty_img.display();

    // vector<vector<double>> t_x, t_c;
    // t_c.push_back(distorted_vecim);
    // t_x.push_back(og_vecimg);

    // mat F_rcima = frcima::rcima(t_x, t_c, 1);

    // cout << "Error for RCIMA method: " << frcima::error << endl;

    // arma::vec dirty_vec_img(distorted_vecim);

    // arma::vec filtered = F_rcima * dirty_vec_img;

    // vector<double> filtered_image_vec;

    // for (size_t i = 0; i < filtered.size(); i++)
    //     filtered_image_vec.push_back(filtered(i));

    // VecImage filtered_img_rcima(filtered_image_vec, og_img.num_cols(), og_img.num_rows());
    // filtered_img_rcima.display();
    // F_rcima.reset();

    // F_rcima = frcima::fast_rcima(t_x, t_c, 1);

    // cout << "Error for fast-RCIMA method: " << frcima::error << endl;

    // filtered = F_rcima * dirty_vec_img;
    // filtered_image_vec.clear();

    // for (size_t i = 0; i < filtered.size(); i++)
    // {
    //     filtered_image_vec.push_back(filtered(i));
    // }

    // VecImage filtered_img_frcima(filtered_image_vec, og_img.num_cols(), og_img.num_rows());
    // filtered_img_frcima.display();

    return 0;
}
