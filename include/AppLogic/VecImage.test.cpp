#include  "VecImage.hpp"

int main()
{
    AppLogic::VecImage A("/mnt/c/Users/JorgeAgueroZamora/Documents/TEC/ProyectoDiseno/FroSigProImageDeconvolution/FSP_ImageDeconvolution/test/images/lena.jpg");
    std::vector<double> inner_data;
    A.display();
    inner_data = A.get_double_data();
    AppLogic::VecImage B(inner_data, A.num_cols(),A.num_rows());
    B.display();

    return 0;
}