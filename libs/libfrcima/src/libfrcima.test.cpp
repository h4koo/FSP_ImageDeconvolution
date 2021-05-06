#include "libfrcima.hpp"

#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <iostream>

using namespace std;

int main()
{

    std::vector<std::vector<double>> a;
    std::vector<std::vector<double>> c;

    /* initialize random seed: */
    srand(time(NULL));


    for (size_t i = 0; i < 200; i++)
    {
        std::vector<double> p;
        std::vector<double> p2;
        
        for (size_t j = 0; j < 100; j++)
        {
            double a = (rand()%100) + 1;
            double b = (rand()%100) + 1;
            p.push_back(a/(a+rand()%100));
            p2.push_back(b / (b +rand()%100));
        }
        a.push_back(p);
        c.push_back(p2);
    }

    cout << a.size() << " " << endl;
    cout << a[0].size() << " " << endl;

    // for (size_t i = 0; i < 10; i++)
    // {
    //     for (size_t j = 0; j < 10; j++)
    //     {
    //         cout << a[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    arma::mat res = frcima::rcima(a, c, 50);

    cout << "Error en cálculo RCIMA: " << frcima::error << endl;

    res = frcima::fast_rcima(a, c, 50);

    cout << "Error en cálculo fast-RCIMA: " << frcima::error << endl;


    // res.print();
    // for (size_t i = 0; i < 10; i++)
    // {

    //     for (size_t j = 0; i < 5; i++)
    //     {

    //     }
    //     cout << endl;
    // }

    return 0;
}