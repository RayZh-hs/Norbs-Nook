#include "bookkeeper.hpp"

#include <iomanip>

#include "constants.hpp"

namespace norb {
    Book::Book(const string<book_isbn_len> &isbn,
               const string<book_name_len> &name,
               const string<book_author_len> &author,
               const string<book_keyword_len> &keyword,
               const int quantity, const double price, const double total_cost)
        : isbn(isbn), name(name), author(author), keyword(keyword),
          quantity(quantity), price(price), total_cost(total_cost) {
    }

    bool Book::operator < (const Book &other) const {
        return isbn < other.isbn;
    }

    bool Book::operator <= (const Book &other) const {
        return isbn <= other.isbn;
    }

    bool Book::operator > (const Book &other) const {
        return isbn > other.isbn;
    }

    bool Book::operator >= (const Book &other) const {
        return isbn >= other.isbn;
    }

    // TODO: figure out whether comparing isbn in this context is enough
    bool Book::operator==(const Book &other) const {
        return isbn == other.isbn;
    }

    std::ostream &operator << (std::ostream &os, const Book& book) {
        os << book.isbn     << '\t'
           << book.name     << '\t'
           << book.author   << '\t'
           << book.keyword  << '\t'
           << std::fixed    << std::setprecision(2)
           << book.price    << '\t'
           << book.quantity << '\n';
        return os;
    }

    BookManager::BookManager(const std::string &book_list_head_name, const std::string &book_list_body_name,
                             const std::string &book_isbn_head_name, const std::string &book_isbn_body_name,
                             const std::string &book_name_head_name, const std::string &book_name_body_name,
                             const std::string &book_author_head_name, const std::string &book_author_body_name,
                             const std::string &book_keyword_head_name, const std::string &book_keyword_body_name,
                             const std::reference_wrapper<std::fstream> f_uid_wrapper,
                             std::reference_wrapper<std::fstream> f_global_wrapper)
        : book_list(std::make_unique<algo::FiledBlockList<int, Book> >
              (book_list_head_name, book_list_body_name, f_global_wrapper)),
          isbn_id_list(std::make_unique<algo::FiledBlockList<Book::isbn_hashed_t_, int> >
              (book_isbn_head_name, book_isbn_body_name, f_global_wrapper)),
          name_id_list(std::make_unique<algo::FiledBlockList<Book::name_t_, int> >
              (book_name_head_name, book_name_body_name, f_global_wrapper)),
          author_id_list(std::make_unique<algo::FiledBlockList<Book::author_t_, int> >
              (book_author_head_name, book_author_body_name, f_global_wrapper)),
          hashed_keyword_id_list(std::make_unique<algo::FiledBlockList<lld, int> >
              (book_keyword_head_name, book_keyword_body_name, f_global_wrapper)),
          f_uid(f_uid_wrapper) {
        // Import the uid count from the f_uid_wrapper.
        // Prior to the reading, GlobalVariableManager should set the pointer to begin and ensure a number (0 if need be) is within the file.
        assert(f_uid.good());
        utils::bRead(f_uid, uid_counter);
        assert(f_uid.good());
    }

    BookManager::BookManager(const std::reference_wrapper<std::fstream> f_uid_wrapper,
                             const std::reference_wrapper<std::fstream> f_global_wrapper)
        : BookManager(book_list_head_name, book_list_body_name,
                      book_isbn_head_name,
                      book_isbn_body_name,
                      book_name_head_name,
                      book_name_body_name,
                      book_author_head_name,
                      book_author_body_name,
                      book_keyword_head_name,
                      book_keyword_body_name,
                      f_uid_wrapper,
                      f_global_wrapper) {
    }

    BookManager::~BookManager() {
        assert(f_uid.good());
        utils::bWrite(f_uid, uid_counter);
        assert(f_uid.good());
    }

    std::vector<Book> BookManager::FindAll() const {
        // Get all the books from the list.
        std::vector<Book> ret = std::move(book_list->valAsVector());
        std::sort(ret.begin(), ret.end());
        return ret;
    }

    std::vector<Book> BookManager::FindByAuthor(const string<book_author_len> &author) const {
        const std::vector<int> ids = std::move(author_id_list->find(author));
        std::vector<Book> ret;
        for (const auto i : ids) {
            ret.push_back(book_list->findFirst(i));
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }

    std::vector<Book> BookManager::FindByIsbn(const string<book_isbn_len> &isbn) const {
        const std::vector<int> ids = std::move(isbn_id_list->find(hash(isbn)));
        std::vector<Book> ret;
        for (const auto i : ids) {
            ret.push_back(book_list->findFirst(i));
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }

    std::vector<Book> BookManager::FindByName(const string<book_name_len> &name) const {
        const std::vector<int> ids = std::move(name_id_list->find(name));
        std::vector<Book> ret;
        for (const auto i : ids) {
            ret.push_back(book_list->findFirst(i));
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }

    std::vector<Book> BookManager::FindByKeyword(const string<book_keyword_len> &keyword) const {
        // Split the string with terminator
        auto keys_parsed = keyword.split_and_hash(book_keyword_separator);
        if (keys_parsed.empty()) {
            return {};
        }
        const auto ids = hashed_keyword_id_list->find(keys_parsed[0]);
        std::vector<Book> ret;
        for (auto id : ids) {
            Book book = book_list->findFirst(id);
            if (utils::isSubsetOf(keys_parsed, book.keyword.split_and_hash(book_keyword_separator))) {
                ret.push_back(book);
            }
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }

    double BookManager::Buy(int id, int quantity) {
        if (!book_list->count(id))
            return npos;
        auto book = book_list->findFirst(id);
        if (quantity <= 0 || book.quantity < quantity) {
            return npos;
        }
        book_list->del(id, book);
        book.quantity -= quantity;
        book_list->insert(id, book);
        return quantity * book.price;
    }

    bool BookManager::Import(const int quantity, const double total_cost) {
        if (selected.empty())
            return false;
        int id = selected.top();
        if (id == npos)
            return false;
        if (quantity <= 0 || total_cost <= 0) {
            return false;
        }
        Book book = book_list->findFirst(id);
        book_list->del(id, book);
        book.quantity += quantity;
        book_list->insert(id, book);
        return true;
    }

    bool BookManager::Modify(Book new_info) {
        if (selected.empty())
            return false;
        const auto id = selected.top();
        if (id == npos) {
            return false;
        }
        Book old_info = book_list->findFirst(id);
        assert(book_list->del(id, old_info));
        assert(book_list->insert(id, new_info));
        // Change ISBN
        if (new_info.isbn != old_info.isbn) {
            assert(isbn_id_list->del(hash(old_info.isbn), id));
            isbn_id_list->insert(hash(new_info.isbn), id);
        }
        // Change Name
        if (new_info.name != old_info.name) {
            assert(name_id_list->del(old_info.name, id));
            name_id_list->insert(new_info.name, id);
        }
        // Change Author
        if (new_info.author != old_info.author) {
            assert(author_id_list->del(old_info.author, id));
            author_id_list->insert(new_info.author, id);
        }
        // Change Keyword
        if (new_info.keyword != old_info.keyword) {
            auto old_parsed = old_info.keyword.split_and_hash(book_keyword_separator);
            auto new_parsed = new_info.keyword.split_and_hash(book_keyword_separator);
            for (auto i : old_parsed) {
                assert(hashed_keyword_id_list->del(i, id));
            }
            for (auto i : new_parsed) {
                hashed_keyword_id_list->insert(i, id);
            }
        }
        // Change Price
        if (new_info.price != old_info.price) {}    // Nothing needs to be changed because no FiledBlockList tracks this.
        return true;
    }

    bool BookManager::Select(const string<book_isbn_len> &isbn) {
        int id;
        lld hashed_isbn = hash(isbn);
        const bool is_new_book = !isbn_id_list->count(hashed_isbn);
        id = is_new_book ? (++uid_counter) : isbn_id_list->findFirst(hashed_isbn);
        if (is_new_book) {
            // TODO: (although it seems unlikely) test whether multiple new books will collide!
            Book book {};
            book.isbn = isbn;
            book_list->insert(id, book);
            isbn_id_list->insert(hashed_isbn, id);
            author_id_list->insert(book.author, id);
            name_id_list->insert(book.name, id);
            hashed_keyword_id_list->insert(0, id);
        }
        selected.pop();
        selected.push(id);
        return true;
    }

    bool BookManager::AnySelected() {
        return !selected.empty() &&  selected.top() != npos;
    }

    int BookManager::GetId(const string<book_isbn_len> &isbn) {
        const lld hashed_isbn = hash(isbn);
        return isbn_id_list->findFirst(hashed_isbn);
    }

    Book BookManager::GetInfo(int id) {
        return book_list->findFirst(id);
    }

    bool BookManager::StackCreate() {
        selected.push(npos);
        return true;
    }

    bool BookManager::StackRemove() {
        if (selected.empty())
            return false;
        selected.pop();
        return true;
    }

}
