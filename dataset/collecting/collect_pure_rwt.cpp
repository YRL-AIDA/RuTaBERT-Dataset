#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <omp.h>
#include <thread>
// #include <sstream>
#include <regex>

#include "functions.h"


const std::string SEP = "⇧";

// void create_files_list(std::vector<std::string> & file_names, const std::string cached_filenames)
// {
//     /**
//      * Collect filenames into vector `file_names`, that contain tables.
//      */
    
//     if (std::filesystem::exists(cached_filenames)) {
//         std::ifstream in(cached_filenames);
//         std::string file_name;

//         while(std::getline(in, file_name)) {
//             file_names.push_back(file_name);
//         }
//         std::cout << "created" << std::endl;
//     } else {
//         std::string path = "../../../ru-wiki-tables-dataset/Dataset/data/";

//         std::vector<std::string> dir_names;
//         for (const auto & dir_name : std::filesystem::directory_iterator(path)) {
//             // Check if it is a directory
//             const std::filesystem::path path(dir_name);
//             std::error_code ec;
//             if (!std::filesystem::is_directory(path, ec)) {
//                 std::cout << "not a dir: " << dir_name << std::endl;
//                 continue;
//             }

//             dir_names.push_back(dir_name.path().string());
//         }

//         std::ofstream out(cached_filenames);
//         for (auto dir_name : dir_names) {
//             for (const auto & file : std::filesystem::directory_iterator(dir_name)) {
//                 std::string file_name = file.path().string();

//                 bool is_table_meta_info = std::regex_match(
//                     file_name,
//                     std::regex("(.)+table_(.)+\\.csv")
//                 );

//                 if (is_table_meta_info) {
//                     file_names.push_back(file_name);

//                     // save to file for optimization
//                     out << file_name << std::endl;
//                 }
//             }
//         }
//         out.close();
//     }
// }


// std::vector<std::string> split(const std::string & s, const char delimiter) {
//     /**
//      * Works well (tested)
//      */
//     std::string tmp; 
//     std::stringstream ss(s);
//     std::vector<std::string> tokens;
//     while(getline(ss, tmp, delimiter)){
//         tokens.push_back(tmp);
//     }

//     return tokens;
// }


// void replace_all(std::string & s, const std::string & search, const std::string & replace ) {
//     /**
//      * Works well (tested)
//      */
//     for (size_t pos = 0; ; pos += replace.length()) {
//         // Locate the substring to replace
//         pos = s.find(search, pos);
//         if (pos == std::string::npos) break;
//         // Replace by erasing and inserting
//         s.erase(pos, search.length());
//         s.insert(pos, replace);
//     }
// }


// size_t count (const std::string & table_row, const char delimiter) {
//     /**
//      * Works well (tested)
//      */
//     size_t cnt_delimiter = 0;
//     for (int i = 0; i < table_row.size(); i++) {
//         if (table_row[i] == delimiter) {
//             cnt_delimiter++;
//         }
//     }
//     return cnt_delimiter;
// }


void collect(const std::vector<std::string> & file_names)
{
    int thread_id = omp_get_thread_num();

    std::ofstream output("data/pure_rwt/data_" + std::to_string(thread_id) + ".csv");
    output << "table_id" + SEP + "column_id" + SEP + "column_header" + SEP + "column_data" << std::endl;

    #pragma omp for schedule(dynamic)
    for (int i = 0; i < file_names.size(); i++) {
        const std::string & file_name = file_names[i];
        const std::string table_id = file_name;

        std::ifstream table(file_name);
        
        std::string table_row;
        std::getline(table, table_row);
        
        replace_all(table_row, "\"", "");
        replace_all(table_row, "\n", "");
        replace_all(table_row, "[править | править код]", "");
        
        if (count(table_row, '|') < 1) continue;
        
        // headers
        std::vector<std::string> headers = split(table_row, '|');
        
        // columns
        std::vector<std::string> columns(headers.size());
        std::fill(columns.begin(), columns.end(), "");
        
        std::string cells_sep = " >> ";
        while (std::getline(table, table_row)) {
            replace_all(table_row, "[править | править код]", "");
            replace_all(table_row, "\n", "");
            replace_all(table_row, "\"", "");

            // avoid `.mw-parser`
            // std::regex re ("\\.mw-parser-output\\s\\.ts.+\\{.+\\}");
            if (table_row.find("mw-parser-output") != std::string::npos) {
                continue;
                // table_row = std::regex_replace(table_row, re, "");
            }

            std::vector<std::string> cells = split(table_row, '|');
            if (cells.size() != headers.size()) continue;
            
            for (int i = 0; i < columns.size(); i++) {
                columns[i] += cells[i] + cells_sep;
            }
        }

        for (int i = 0; i < columns.size(); i++) {
            output << table_id << SEP << i << SEP << headers[i] << SEP << columns[i] << std::endl;
        }
    }
}

int main()
{
    std::vector<std::string> file_names;
    create_files_list(file_names, "csv_filenames");

    #pragma omp parallel
    {
        collect(file_names);
    }

    return 0;
}
