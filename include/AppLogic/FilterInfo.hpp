#ifndef FILTERINFO_H
#define FILTERINFO_H

#include <string>

namespace AppLogic
{

    typedef struct
    {
        std::string filter_name;
        std::string file_image_source;
        int image_calc_amount;
        float calc_time_seconds;
        int rank;
        int f_matrix_num_col;
        int f_matrix_num_row;
        std::string noise_type;
        double noise_value;
        std::string calculation_method;
    } FilterInfo;

    inline std::ostream &operator<<(std::ostream &stream, FilterInfo const &data)
    {
        // NOTE:  string values cannot be multiple words because whitespace is used to separate data
        return stream << data.filter_name << " "
                      << data.file_image_source << " "
                      << data.image_calc_amount << " "
                      << data.rank << " "
                      << data.f_matrix_num_col << " "
                      << data.f_matrix_num_row << " "
                      << data.noise_type << " "
                      << data.noise_value << " "
                      << data.calculation_method << " "
                      << data.calc_time_seconds;
    }

    inline std::istream &operator>>(std::istream &stream, FilterInfo &data)
    {
        return stream >> data.filter_name 
                        >> data.file_image_source 
                        >> data.image_calc_amount 
                        >> data.rank 
                        >> data.f_matrix_num_col
                        >> data.f_matrix_num_row 
                        >> data.noise_type 
                        >> data.noise_value 
                        >> data.calculation_method 
                        >> data.calc_time_seconds;
    }
}

#endif
