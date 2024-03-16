# RuTaBERT-Dataset

Dataset besed on [Russian Web Tables](https://arxiv.org/abs/2210.06353), which is a corpus of Russian language tables from Wikipedia.

Only relational tables were chosen from RuWebTables with headers matching selected 170 DBpedia semantic types.

Dataset contains `1.441.349` columns, and has fixed train / test split. 

| Split | Columns |
|--------------|---------|
| Test   | 72.067   |
| Train  | 1.369.282 |


## Instruction to reproduce dataset
Make sure your pc satisfies these requirements:

- C++ compiler (supports C++17 and OpenMP)
- Make
- [RapidJSON](https://rapidjson.org/index.html)
- [Boost](https://www.boost.org/) library

---

1. Download and decompress [ru-wiki-tables-datset](https://gitlab.com/unidata-labs/ru-wiki-tables-dataset) into `./dataset/` directory.
2. Run `make` command from `./dataset/collecting/` directory to compile collecting files.
3. Run `./dataset/collecting/collect_columns_from_dataset` to collect column headers from dataset. Output will be in `./dataset/collecting/columns_headers/`.
4. Run all cells in `./dataset/research/research.ipynb`.
5. Run all cells in `./dataset/labelling/labelling.ipynb`.
6. Run `./dataset/collecting/collect_columns_data` to collect column data from dataset. Output will be in `./dataset/collecting/columns_data/`.
7. Run all cells in `./dataset/cta_dataset/create_cta_dataset.ipynb`. Output train / test split will be in `./dataset/cta_dataset/train[test]` directories.
