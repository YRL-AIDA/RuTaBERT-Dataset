#include "rapidjson/document.h" 
#include "rapidjson/filereadstream.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <omp.h>
#include <vector>
#include <regex>
#include <thread>
#include <filesystem>

using namespace rapidjson;


const std::string SEP = "⇧";


void create_files_list(std::vector<std::string> & file_names)
{
    /*
     * Собирает в вектор имена файлов, которые надо посмотреть
     * Результат в векторе file_names. Последовательная ф-ция
     */
    
    std::string path = "../dataset/Dataset/data";

    std::vector<std::string> dir_names;
    for (const auto & dir_name : std::filesystem::directory_iterator(path)) {
        // Check if it is a directory
        const std::filesystem::path path(dir_name);
        std::error_code ec;
        if (!std::filesystem::is_directory(path, ec)) {
            std::cout << "not a dir: " << dir_name << std::endl;
            continue;
        }
           
        dir_names.push_back(dir_name.path().string());
    }

    // TODO: if file exists
    std::ofstream out("filenames");
    for (auto dir_name : dir_names) {
        for (const auto & file : std::filesystem::directory_iterator(dir_name)) {
            std::string file_name = file.path().string();

            bool is_table_meta_info = std::regex_match(
                file_name,
                std::regex("(.)+table_(.)+_meta\.json")
            );

            if (is_table_meta_info) {
                file_names.push_back(file_name);

                // save to file for optimization
                out << file_name << std::endl;
            }
        }
   }
   out.close();
}


void thread_function(const std::vector<std::string> & files_list)
{
    int thread_id = omp_get_thread_num();

    // TODO: write into file directly, no need in this vector
    std::vector<std::string> rows;
    
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < files_list.size(); i++) {
        const std::string & file_name = "../" + files_list[i];
        
        // Check if file is empty
        if (std::filesystem::file_size(file_name) == 0) {
            std::cout << "empty: " << file_name << std::endl;
            continue;
        }

        // Parse json file
        FILE * fp = fopen(file_name.c_str(), "r");
        char readBuffer[65536]; // TODO: decrease buffer size, its too big
        rapidjson::FileReadStream is(
            fp,
            readBuffer,
            sizeof(readBuffer)
        );
        rapidjson::Document d;
        d.ParseStream(is);
        fclose(fp);

        // adds table_id without "_meta.json" and relative to root dir
        std::string table_id = file_name.substr(3, file_name.size() - 10-3);
        if (d["columns"].IsObject()) {
            for (auto & column : d["columns"].GetObject()) {
            
                if (!column.name.IsString()) break;
                std::string column_id = column.name.GetString();

                auto & propertyValue = column.value;
                if (propertyValue.IsObject()) {
                    for (auto & column_properties : propertyValue.GetObject()) {

                        if (!column_properties.name.IsString()) break;
                        std::string column_property_name = column_properties.name.GetString();
                        
                        if (column_property_name == "name") {
                            auto & column_property_value = column_properties.value;

                            // Note: name value could be ARRAY!
                            if (column_property_value.IsString()) {
                                std::string column_name = column_property_value.GetString();
                                rows.push_back(table_id + SEP + column_id + SEP + column_name);
                                
                                // std::cout << "thread_id :" << thread_id << "works" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    // std::cout << "thread_id: " << thread_id << " / done" << std::endl;

    std::ofstream out("columns_headers/headers_" + std::to_string(thread_id) + ".csv");
    out << "table_id" + SEP + "column_id" + SEP + "column_name" << std::endl;
    for (auto i : rows) {
        out << i << std::endl;
    }
    out.close();
}


int main()
{
    std::vector<std::string> files_list;
    if (std::filesystem::exists("filenames")) {
        std::cout << "exists";

        std::ifstream in("filenames");
        std::string file_name;
        while (std::getline(in, file_name)) {
            files_list.push_back(file_name);
        }
    } else {
        std::cout << "doesnt exist";
        create_files_list(files_list);
    }

    #pragma omp parallel 
    {
        thread_function(files_list);
    }
    std::cout << "done";

    return 0;
}
