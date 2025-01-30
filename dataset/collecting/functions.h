#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>


void create_files_list(std::vector<std::string> & file_names, const std::string cached_filenames)
{
    /**
     * Collect filenames into vector `file_names`, that contain tables.
     */
    
    if (std::filesystem::exists(cached_filenames)) {
        std::ifstream in(cached_filenames);
        std::string file_name;

        while(std::getline(in, file_name)) {
            file_names.push_back(file_name);
        }
        std::cout << "created" << std::endl;
    } else {
        std::string path = "../../../ru-wiki-tables-dataset/Dataset/data/";

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

        std::ofstream out(cached_filenames);
        for (auto dir_name : dir_names) {
            for (const auto & file : std::filesystem::directory_iterator(dir_name)) {
                std::string file_name = file.path().string();

                bool is_table_meta_info = std::regex_match(
                    file_name,
                    std::regex("(.)+table_(.)+\\.csv")
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
}


std::vector<std::string> split(const std::string & s, const char delimiter) {
    /**
     * Works well (tested)
     */
    std::string tmp; 
    std::stringstream ss(s);
    std::vector<std::string> tokens;
    while(getline(ss, tmp, delimiter)){
        tokens.push_back(tmp);
    }

    return tokens;
}


void replace_all(std::string & s, const std::string & search, const std::string & replace ) {
    /**
     * Works well (tested)
     */
    for (size_t pos = 0; ; pos += replace.length()) {
        // Locate the substring to replace
        pos = s.find(search, pos);
        if (pos == std::string::npos) break;
        // Replace by erasing and inserting
        s.erase(pos, search.length());
        s.insert(pos, replace);
    }
}


size_t count (const std::string & table_row, const char delimiter) {
    /**
     * Works well (tested)
     */
    size_t cnt_delimiter = 0;
    for (int i = 0; i < table_row.size(); i++) {
        if (table_row[i] == delimiter) {
            cnt_delimiter++;
        }
    }
    return cnt_delimiter;
}

std::string trim (std::string s) {
    int start_id = 0;
    int end_id = s.size() - 1;

    while (true) {
        if (isspace(s[start_id])) {
            start_id++;
        }
        if (isspace(s[end_id])) {
            end_id--;
        }
        if (isspace(s[start_id]) && isspace(s[end_id])) break;
    }
    return s.substr(start_id, end_id - start_id + 1);
}
