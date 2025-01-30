// #include <boost/algorithm/string.hpp>
// #include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <omp.h>
#include <thread>

#include <functions.h>


const std::string SEP = "⇧";

void create_files_list(std::vector<std::string> & file_names, std::string labelled_headers_filename)
{
    /*
     * Create vector of file names.csv
     *
     */
    // std::ifstream in("../labelling/labelled/threshold_0.78.csv");
    std::ifstream in(labelled_headers_filename);
    std::string file_name;

    // skip header
    std::getline(in, file_name);
    while(std::getline(in, file_name)) {
        int table_id_end_idx = file_name.find(";");
        std::string table_id = file_name.substr(0, table_id_end_idx);

        std::string file_path_str = "../" + table_id + ".csv";
        const std::filesystem::path file_path(file_path_str);
        if (std::filesystem::exists(file_path)) {
            int col_id_end_idx = file_name.find(";", table_id_end_idx + 1);
            std::string col_id = file_name.substr(
                table_id_end_idx + 1,
                col_id_end_idx - table_id_end_idx - 1
            );
        
            file_names.push_back(table_id + ";" + col_id);
        }
    }
    std::cout << "created" << std::endl;
}

void collect(const std::vector<std::string> & file_names)
{
    int thread_id = omp_get_thread_num();

    std::ofstream output("columns_data/data_" + std::to_string(thread_id) + ".csv");
    output << "table_id" + SEP + "column_id" + SEP + "column_data" << std::endl;
    std::string rwt_path = "";

    #pragma omp for schedule(dynamic)
    for (int i = 0; i < file_names.size(); i++) {
        std::vector<std::string> temp = split(file_names[i], ';');
        const std::string table_id = temp[0];
        const std::string col_id = temp[1];

        // todo: why ../ ???
        const std::string & file_name = "../" + temp[0] + ".csv";

        std::ifstream table(file_name);
        std::string table_row;

        // skip header    
        // TODO: improve somehow, dont like
        std::getline(table, table_row);
        replace_all(table_row, "[править | править код]", "");
        replace_all(table_row, "\"", "");
        if (std::count(table_row.begin(), table_row.end(), '|') < 1) {
            std::getline(table, table_row);
        }

        std::string column_data = "";
        while (std::getline(table, table_row)) {
            // TODO: if empty skip do not add to data
            replace_all(table_row, "[править | править код]", "");
            if (std::count(table_row.begin(), table_row.end(), '|') < 1) {
                continue;
            }
                
            std::vector<std::string> cells = split(table_row, '|');

            std::string cur_cell = cells[std::stoi(col_id)];
            cur_cell = trim(cur_cell);

            if (!cur_cell.empty() and cur_cell != " " and cur_cell != "\n" and cur_cell.find_first_not_of(' ') != std::string::npos) {
                // TODO: add length check
                bool ok = false;
                for (auto cell: cells) {
                    cell = trim(cell);
                    if (cell != cur_cell) {
                        ok = true;
                        break;
                    }
                }
                    
                if (ok) {
                    // column_data += cur_cell + ";";
                    column_data += cur_cell + " ";
                }
            }
        }
        output << table_id << SEP << col_id << SEP << column_data << std::endl;
    }
}

int main()
{
    std::vector<std::string> file_names;
    create_files_list(file_names, "../labelling/labelled/threshold_0.78.csv");

    #pragma omp parallel
    {
        collect(file_names);
    }

    return 0;
}
