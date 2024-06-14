# RWT-RuTaBERT

Dataset based on [Russian Web Tables](https://arxiv.org/abs/2210.06353) (RWT), which is a corpus of Russian language tables from Wikipedia.

Only relational tables were chosen from _RWT_ with headers matching selected **170** DBpedia semantic types.

Dataset contains `1.441.349` columns, and has fixed train / test split. 

| Split | Columns   | Tables  | Avg. columns per table |
|-------|-----------| ------- | ---------------------- |
| Test  | 115 448   | 55 080  | 2.096                  |
| Train | 1 325 901 | 633 426 | 2.093                  |

## Table of contents
- [RWT-RuTaBERT](#rwt-rutabert)
  * [Train statistics](#train-statistics)
      - [Most frequent column sizes](#most-frequent-column-sizes)
      - [Least frequent column sizes](#least-frequent-column-sizes)
      - [Most frequent labels](#most-frequent-labels)
      - [Least frequent labels](#least-frequent-labels)
  * [Test statistics](#test-statistics)
      - [Most frequent column sizes](#most-frequent-column-sizes-1)
      - [Least frequent column sizes](#least-frequent-column-sizes-1)
      - [Most frequent labels](#most-frequent-labels-1)
      - [Least frequent labels](#least-frequent-labels-1)
  * [Instruction to reproduce dataset](#instruction-to-reproduce-dataset)


## Train statistics

#### Most frequent column sizes
| Column size | Occurances  |
| -------     | --------    |
| 1           | 257890      |
| 2           | 172414      |
| 3           | 124635      |
| 4           | 54886       |
| 5           | 18532       |
| 6           | 3404        |
| 7           | 733         |
| 8           | 254         |
| 9           | 234         |
| 18          | 221         |

#### Least frequent column sizes
| Column size  | Occurances  |
| -------      | --------    |
| 19           | 6           |
| 40           | 6           |
| 16           | 5           |
| 38           | 5           |
| 29           | 4           |
| 20           | 4           |
| 21           | 4           |
| 37           | 2           |
| 39           | 2           |
| 17           | 2           |

#### Most frequent labels
| Label        | Occurances  |
| -------      | --------    |
| год          | 230016      |
| название     | 170812      |
| место        | 103986      |
| дата         | 97228       |
| команда      | 75032       |
| результат    | 52730       |
| примечание   | 48635       |
| актер        | 38959       |
| страна       | 36754       |
| турнир       | 33175       |

#### Least frequent labels
| Label        | Occurances  |
| -------      | --------    |
| континент    | 92          |
| роман        | 89          |
| закон        | 89          |
| борец        | 88          |
| колледж      | 87          |
| музей        | 86          |
| фирма        | 85          |
| дорога       | 83          |
| префектура   | 83          |
| цитата       | 76          |



## Test statistics

#### Most frequent column sizes
| Column size | Occurances  |
| -------     | --------    |
| 1           | 22491      |
| 2           | 14923      |
| 3           | 10798      |
| 4           | 4801       |
| 5           | 1614       |
| 6           | 299        |
| 7           | 69         |
| 18          | 21         |
| 8           | 19         |
| 9           | 18         |

#### Least frequent column sizes
| Column size  | Occurances  |
| -------      | --------    |
| 13           | 3           |
| 36           | 2           |
| 20           | 1           |
| 16           | 1           |
| 21           | 1           |
| 14           | 1           |
| 39           | 1           |
| 37           | 1           |
| 38           | 1           |
| 11           | 1           |
  
#### Most frequent labels
| Label        | Occurances  |
| -------      | --------    |
| год          | 19854       |
| название     | 14748       |
| место        | 9004        |
| дата         | 8408        |
| команда      | 6653        |
| результат    | 4653        |
| примечание   | 4203        |
| актер        | 3435        |
| страна       | 3217        |
| турнир       | 2911        |


#### Least frequent labels
| Label        | Occurances  |
| -------      | --------    |
| цитата       | 7           |
| дорога       | 6           |
| статья       | 6           |
| фирма        | 6           |
| сообщество   | 5           |
| колледж      | 5           |
| борец        | 5           |
| музей        | 4           |
| банк         | 4           |
| камера       | 4           |



## Instruction to reproduce dataset
Make sure your PC satisfies these requirements:
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
7. Run all cells in `./dataset/cta_dataset/create_cta_dataset.ipynb`. Output train/test splits will be in `./dataset/cta_dataset/train[test]` directories.
