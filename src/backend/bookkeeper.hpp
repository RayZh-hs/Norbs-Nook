#pragma once

#include <constants.hpp>
#include <memory>
#include <stack>
#include "json.hpp"

#include "norb_string.hpp"
#include "norb_blocklist.hpp"

using namespace norb::norbs_nook_constants::bookkeeper_constants;

namespace norb {
    using json = nlohmann::json;

    struct Book {
        string<book_isbn_len> isbn{};
        string<book_name_len> name{};
        string<book_author_len> author{};
        string<book_keyword_len> keyword{};

        int quantity = 0;
        double price = 0;
        double total_cost = 0;

        typedef string<book_isbn_len> isbn_t_;
        typedef lld isbn_hashed_t_;
        typedef string<book_name_len> name_t_;
        typedef string<book_author_len> author_t_;
        typedef string<book_keyword_len> keyword_t_;

        Book(const isbn_t_ &isbn, const name_t_ &name,
             const author_t_ &author, const keyword_t_ &keyword,
             int quantity = 0, double price = 0, double total_cost = 0);

        Book() = default;

        bool operator <(const Book &other) const;

        bool operator <=(const Book &other) const;

        bool operator >(const Book &other) const;

        bool operator >=(const Book &other) const;

        bool operator ==(const Book &other) const;

        friend std::ostream &operator <<(std::ostream &os, const Book &book);
    };

    // The to_json and from_json handles explicit and implicit conversions to and from nlohmann json.
    inline void to_json(json &j, const Book &b) {
        j = json{
            {"isbn", std::string(b.isbn)},
            {"name", std::string(b.name)},
            {"author", std::string(b.author)},
            {"keyword", std::string(b.keyword)},
            {"quantity", b.quantity},
            {"price", b.price},
            {"total_cost", b.total_cost}
        };
    }

    inline void from_json(const json &j, Book &b) {
        b.isbn = j.at("isbn").get<std::string>(); // Use .at() for error handling
        b.name = j.at("name").get<std::string>();
        b.author = j.at("author").get<std::string>();
        b.keyword = j.at("keyword").get<std::string>();
        b.quantity = j.at("quantity").get<int>();
        b.price = j.at("price").get<double>();
        b.total_cost = j.at("total_cost").get<double>();
    }


    class BookManager {
    public:
        BookManager(const std::string &book_list_head_name,
                    const std::string &book_list_body_name,
                    const std::string &book_isbn_head_name,
                    const std::string &book_isbn_body_name,
                    const std::string &book_name_head_name,
                    const std::string &book_name_body_name,
                    const std::string &book_author_head_name,
                    const std::string &book_author_body_name,
                    const std::string &book_keyword_head_name,
                    const std::string &book_keyword_body_name,
                    std::reference_wrapper<std::fstream> f_uid_wrapper,
                    std::reference_wrapper<std::fstream> f_global_wrapper
        );

        explicit BookManager(
            std::reference_wrapper<std::fstream> f_uid_wrapper,
            std::reference_wrapper<std::fstream> f_global_wrapper
        );

        ~BookManager();

        // Returns all books, in order of isbn.
        std::vector<Book> FindAll() const;

        std::vector<Book> FindByIsbn(const string<book_isbn_len> &isbn) const;

        std::vector<Book> FindByName(const string<book_name_len> &name) const;

        std::vector<Book> FindByAuthor(const string<book_author_len> &author) const;

        std::vector<Book> FindByKeyword(const string<book_keyword_len> &keyword) const;

        int GetId(const string<book_isbn_len> &isbn);

        Book GetInfo(int id);

        // Returns npos if it fails.
        double Buy(int id, int quantity);

        bool StackCreate();

        bool StackRemove();

        bool Select(const string<book_isbn_len> &isbn);

        // Note that these errors must be handled before calling this function:
        // 1. no repeated params;
        // 2. changes not null;
        // 3. changing isbn to its original value is forbidden;
        // 4. keyword shouldn't contain identical information;
        bool Modify(Book new_info);

        bool Import(int quantity, double total_cost);

        bool AnySelected();

        int GetSelectedId() {
            return selected.empty() ? npos : selected.top();
        }

        // Several used constexpr vars
        static constexpr int npos = -1;

        inline void DebugPrintBookkeeperInfo() {
            std::cout << "\tbook_list: " << '\n';
            for (auto i: book_list->asDict()) {
                std::cout << i.first << "\t:\t" << i.second << '\n';
            }
            std::cout << "\tisbn_id_list: " << '\n';
            for (auto i: isbn_id_list->asDict()) {
                std::cout << i.first << "\t:\t" << i.second << '\n';
            }
            std::cout << "\tname_id_list: " << '\n';
            for (auto i: name_id_list->asDict()) {
                std::cout << i.first << "\t:\t" << i.second << '\n';
            }
            std::cout << "\tauthor_id_list: " << '\n';
            for (auto i: author_id_list->asDict()) {
                std::cout << i.first << "\t:\t" << i.second << '\n';
            }
            std::cout << "\thashed_keyword_id_list: " << '\n';
            for (auto i: hashed_keyword_id_list->asDict()) {
                std::cout << i.first << "\t:\t" << i.second << '\n';
            }
        }

    private:
        // The f_uid is a reference passed in by global, and managed externally by norb::GlobalVariableManager in global.hpp
        std::fstream &f_uid;
        // UIDs range from 1 to uid_counter. A book's uid is fixed once it has been created.
        int uid_counter = 0;
        std::unique_ptr<algo::FiledBlockList<int, Book> > book_list;
        std::unique_ptr<algo::FiledBlockList<Book::isbn_hashed_t_, int> > isbn_id_list;
        std::unique_ptr<algo::FiledBlockList<Book::name_t_, int> > name_id_list;
        std::unique_ptr<algo::FiledBlockList<Book::author_t_, int> > author_id_list;
        std::unique_ptr<algo::FiledBlockList<lld, int> > hashed_keyword_id_list;

        // The selected ids will be stored in a stack.
        std::stack<int> selected;
    };
}
