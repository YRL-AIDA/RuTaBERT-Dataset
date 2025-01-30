#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <omp.h>
#include <thread>
#include <regex>

#include "functions.h"


const std::string SEP = "⇧";


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
        replace_all(table_row, "\n", " ");
        replace_all(table_row, "\r", "");
        replace_all(table_row, "[править | править код]", "");

        // table_row.erase(std::remove(table_row.begin(), table_row.end(), '\r'), table_row.end());
        // table_row.erase(std::remove(table_row.begin(), table_row.end(), '\n'), table_row.end());
        
        if (count(table_row, '|') < 1) continue;
        
        // headers
        std::vector<std::string> headers = split(table_row, '|');
        
        // columns
        std::vector<std::string> columns(headers.size());
        std::fill(columns.begin(), columns.end(), "");
        
        std::string cells_sep = " << ";
        while (std::getline(table, table_row)) {
            replace_all(table_row, "[править | править код]", "");
            replace_all(table_row, "\n", " ");
            replace_all(table_row, "\r", "");
            replace_all(table_row, "\"", "");

            // table_row.erase(std::remove(table_row.begin(), table_row.end(), '\r'), table_row.end());

            std::vector<std::string> cells = split(table_row, '|');
            if (cells.size() != headers.size()) continue;
            
            for (int i = 0; i < columns.size(); i++) {
                replace_all(table_row, "\n", " ");
                replace_all(table_row, "\r", "");
                // cells[i].erase(std::remove(cells[i].begin(), cells[i].end(), '\r'), cells[i].end());
                // cells[i].erase(std::remove(cells[i].begin(), cells[i].end(), '\n'), cells[i].end());

                // write only non-empty cells
                if (cells[i].find_first_not_of(' ') != std::string::npos) {
                    columns[i] += cells[i] + cells_sep;
                }
            }
        }

        for (int i = 0; i < columns.size(); i++) {
            columns[i] = columns[i].substr(0, columns[i].size() - 4); // remove last separator ` << `;
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
