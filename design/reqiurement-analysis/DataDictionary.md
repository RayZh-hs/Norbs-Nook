# Data Dictionary

The Nook: Data Dictionary of Norb's Bookstore

## Account Management

1. **Account** `class`   
A class that stores all account information, including the name and the role (Visitor, with no name; Customer; Salesperson; Shopkeeper).
2. **current_account** `Account`  
Stores the current logged in account.
3. **account_list** `std::unordered_map<std::string, Account>`  
Maps names to corresponding accounts that are inside the system.

## File IO
1. **FileInterface** `class`  
Reads from and writes to files structured data.

## Data Management

1. **BlockChain** `class`  
A class that handles large data io.
2. **Book** `class`  
A class that contains all information regarding a specific book.
3. **Action** `class`  
Base class of all actions performed by customers/salespersons/shopkeepers.
4. **book_storage** `std::unordered_map<std::string, Book>`  
Maintains all the states of the books in the store.
5. **worker_history** `std::unordered_map<std::string, BlockChain<Action>>`  
Maintains the history of actions of each worker.
6. **action_history** `BlockChain<Action>`  
Maintains the entire history.
7. **profit_history** `BlockChain<ProfitNode>`  
Maintains the history of income and spending of the store. 

## Logging

1. **Logger** `class`
Stores log info of each session, stores to and reads from them on disk.