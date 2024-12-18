# Functional Manual

The Nook: Functional Manual of Norb's Bookstore

## Account Management

`account.hpp acount.cpp`

1. add_user:
   - **Name**: add_user;
   - **Input**: name `std::string`, category `enum`, time `int`
   - **Output**: None;
   - **Description**: Adds user to system;
2. del_user:
    - **Name**: del_user;
    - **Input**: name `std::string`;
    - **Output**: success `bool`;
    - **Description**: Deletes user if it exists;
3. has_user:
4. chmod_user:
    - **Name**: del_user;
    - **Input**: name `std::string`, category `enum`;
    - **Output**: success `bool`;
    - **Description**: Changes the category of a user if it exists;


## File IO

`file.hpp file.cpp`

Standard File IO functionality. Similar to MemoryRiver.


## Data Management

`blockchain.hpp blockchain.cpp`
`action.hpp action.cpp`
`storage.hpp storage.cpp`
`history.hpp history.cpp`

1. BlockChain: insert, edit, delete, optimise
    - **Name**: insert, edit, delete, optimise;
    - **Description**: Edits the BlockChain so that new data is stored within;
2. BlockChain: get
    - **Name**: get;
    - **Input**: serial `int`;
    - **Description**: Seeks and retrieves the data from the disk into memory;
3. BlockChain: dump
    - **Name**: dump;
    - **Input**: serial `int`;
    - **Description**: Saves the data from the memory into the disk;
4. perform_action
    - **Name**: perform_action;
    - **Input**: action `Action`, time `int`;
    - **Description**: Performs the action and saves it into history;
5. query_book
    - **Name**: query_book;
    - **Input**: name `std::string`;
    - **Output**: book_info `Book`;
    - **Description**: Finds all info related to a book;
6. query_worker
    - **Name**: query_worker;
    - - **Input**: name `std::string`;
    - **Output**: actions `BlockChain<Action>`;
    - **Description**: Finds all info related to a worker;
    - - **Input**: name `std::string`, begin `int`, end `int`;
    - **Output**: actions `BlockChain<Action>`;
    - **Description**: Finds all info related to a worker in a given period of time;
7. query_action
    - **Name**: query_action;
    - - **Input**: name `std::string`;
    - **Output**: actions `int`;
    - **Description**: Finds all actions;
    - - **Input**: name `std::string`, begin `int`, end `int`;
    - **Output**: actions `BlockChain<Action>`;
    - **Description**: Finds all actions in a given period of time;
8. query_profit
    - **Name**: query_profit
    - **Input**: begin `int`, end `int`;
    - **Output**: profit `int`;
    - **Description**: Calculates profit in a given period of time;


## Logging

1. set_logger_mode
    - **Name**: set_logger_mode
    - **Input**: mode `enum`;
    - **Description**: Set the screen logging standard;
2. log
    - **Name**: log
    - **Input**: status `enum`, scope `std::string`, msg `std::string`, time `int`;
    - **Description**: Saves the log to disk and prints it the screen;
3. print_log_within
    - **Name**: print_log_within
    - **Input**: status `enum`, begin `int`, end `int`;
    - **Description**: Prints on the screen all log info on/above status, within the given timespan;
4. clear_all_logs
   - **Name**: clear_all_logs
    - **Input**: begin `int`, end `int`;
    - **Description**: Clears all log files within the given timespan;