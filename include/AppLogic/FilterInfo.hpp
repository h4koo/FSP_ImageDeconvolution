/**
 * @file FilterInfo.hpp
 * @author Jorge Agüero Zamora 
 * @brief Contains struct to store filter calculation information and the members to save this information into files
 * @version 0.1
 * @date 2021-06-13
 * 
 */
#ifndef FILTERINFO_H
#define FILTERINFO_H

#include <string>

namespace AppLogic
{
    /**
     * @brief struct to hold the the information of the calculated filter
     * 
     */
    typedef struct
    {
        std::string filter_name; //!< name of the filter
        std::string file_image_source; //!< name of the directory of the source images
        int image_calc_amount; //!< amount of images used in the calculation
        float calc_time_seconds; //!< amount of time taken to calculate the filter
        int rank; //!< rank used in the calculation
        int f_matrix_num_col; //!< number of columns of the filter matrix
        int f_matrix_num_row; //!< number of rows of the filter matrix
        std::string noise_type; //!< noise type used in the calculation
        double noise_value; //!< noise value used (this is a percentage from 0 to 99)
        std::string calculation_method; //!< method used to calculate the filter (either RCIMA of Fast-RCIMA)
    } FilterInfo;

    /**
     * @brief outputs filter information to a stream. Used to save FilterInfo to a file.
     * 
     * @param stream where the filter information is written
     * @param data filter information to write to stream
     * @return std::ostream& returns stream with the filter information written
     */
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

    /**
     * @brief operator to read filter information from a stream. used to read FilterInfo from file
     * 
     * @param stream stream to read filter information from
     * @param data FilterInfo struct where the read information is written
     * @return std::istream& returns stream with the information read
     */
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
