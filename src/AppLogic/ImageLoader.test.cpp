
// #include "ImageLoader.hpp"
// #include "CImg.h"
// using namespace cimg_library;
// using namespace AppLogic;

// int main() {
//   const unsigned char purple[] = { 255,0,255 };
//   CImg<unsigned char>(640,400,1,3,0).draw_text(100,100,"Hello World",purple).display("My first CImg code");
//   return 0;
// }

#include "ImageLoader.hpp"
#include <vector>

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    string filename;
    if (argc < 2)
        filename = "./test/images/";
    else
        filename = argv[1];
    AppLogic::ImageLoader Il;
    AppLogic::VecImage img = Il.loadSingleImage(filename.c_str());
    img.display();

    vector<AppLogic::VecImage> files_in_folder = Il.loadImagesFromFolder(filename.c_str());
    vector<string> filenames = Il.getLoadedImageNames();
    size_t it = 0;
    for (auto file : files_in_folder)
    {
        // img = AppLogic::VecImage(file);
        file.display();
        cout << "filename is: " << filenames[it] << endl;
        ++it;
    }
    {
        /* code */
    }

    return 0;
}