#include "user_interface.hpp"

#include <iomanip>
#include <iostream>

#include "norb_strlib.hpp"

using namespace norb::norbs_nook_constants::validation_constants;

namespace norb {
    CommandlineUserInterface::CommandlineUserInterface(std::unique_ptr<norb::BusinessLogicInterface> moved_interface) {
        interface = std::move(moved_interface);
    }

    CommandlineUserInterface::~CommandlineUserInterface() = default;

    void CommandlineUserInterface::Run() {
        std::cerr << "[Hello World] from user_interface.cpp. This is Norb's Nook, up and running." << std::endl;
        // Run the main cycle.
        while (true) {
            if (std::cin.eof()) {
                std::cerr << "Reached end of control without QUIT or EXIT! Quitting..." << '\n';
                break;
            }
            // Read in the next line of action.
            std::string line;
            std::getline(std::cin, line);
            assert(std::cin.good());
            if (line.empty()) {
                continue;
            }
            try {
                if (matchRegex(line, regex_exit)) {
                    throw QuitUtilityException();
                }
                else if (matchRegex(line, regex_su_without_password)) {
                    auto group = groupRegex(line, regex_su_without_password);
                    interface->SwitchUser(group[1]);
                }
                else if (matchRegex(line, regex_su_with_password)) {
                    auto group = groupRegex(line, regex_su_with_password);
                    interface->SwitchUser(group[1], group[2]);
                }
                else if (matchRegex(line, regex_logout)) {
                    interface->Logout();
                }
                else if (matchRegex(line, regex_register)) {
                    auto group = groupRegex(line, regex_register);
                    interface->Register(group[1], group[2], group[3]);
                }
                else if (matchRegex(line, regex_passwd_without_password)) {
                    auto group = groupRegex(line, regex_passwd_without_password);
                    interface->Password(group[1], group[2]);
                }
                else if (matchRegex(line, regex_passwd_with_password)) {
                    auto group = groupRegex(line, regex_passwd_with_password);
                    interface->Password(group[1], group[2], group[3]);
                }
                else if (matchRegex(line, regex_useradd)) {
                    auto group = groupRegex(line, regex_useradd);
                    interface->UserAdd(group[1], group[2], group[3], group[4]);
                }
                else if (matchRegex(line, regex_delete)) {
                    auto group = groupRegex(line, regex_delete);
                    interface->Delete(group[1]);
                }
                else if (matchRegex(line, regex_show_all)) {
                    auto books = interface->FindAll();
                    for (auto i : books) {
                        std::cout << i << '\n';
                    }
                }
                else if (matchRegex(line, regex_show_author)) {
                    auto group = groupRegex(line, regex_show_author);
                    auto books = interface->FindByAuthor(group[1]);
                    for (auto i : books) {
                        std::cout << i << '\n';
                    }
                }
                else if (matchRegex(line, regex_show_name)) {
                    auto group = groupRegex(line, regex_show_name);
                    auto books = interface->FindByName(group[1]);
                    for (auto i : books) {
                        std::cout << i << '\n';
                    }
                }
                else if (matchRegex(line, regex_show_keyword)) {
                    auto group = groupRegex(line, regex_show_keyword);
                    auto books = interface->FindByKeyword(group[1]);
                    for (auto i : books) {
                        std::cout << i << '\n';
                    }
                }
                else if (matchRegex(line, regex_show_name)) {
                    auto group = groupRegex(line, regex_show_name);
                    auto books = interface->FindByName(group[1]);
                    for (auto i : books) {
                        std::cout << i << '\n';
                    }
                }
                else if (matchRegex(line, regex_show_isbn)) {
                    auto group = groupRegex(line, regex_show_isbn);
                    auto books = interface->FindByIsbn(group[1]);
                    for (auto i : books) {
                        std::cout << i << '\n';
                    }
                }
                else if (matchRegex(line, regex_buy)) {
                    auto group = groupRegex(line, regex_buy);
                    interface->Buy(group[1], stringToInteger(group[2]));
                }
                else if (matchRegex(line, regex_select)) {
                    auto group = groupRegex(line, regex_select);
                    interface->Select(group[1]);
                }
                else if (matchRegex(line, regex_modify)) {
                    auto group = groupRegex(line, regex_modify);
                    auto map = extractUniqueKeyValuePairs(group[1]);
                    if (map.empty()) {
                        throw UtilityException("KEY NOT UNIQUE ERROR");
                    }
                    // Load the old book information:
                    Book new_info = interface->GetSelectedBook();
                    if (map.count("ISBN")) {
                        if (!matchRegex(map["ISBN"], regex_valid_isbn)) {
                            throw UtilityException("INVALID ISBN TRAIT");
                        }
                        if (std::string(new_info.isbn) == map["ISBN"]) {
                            throw UtilityException("IDENTICAL ISBN ERROR");
                        }
                        new_info.isbn = map["ISBN"];
                    }
                    if (map.count("name")) {
                        if (!matchRegex(map["name"], regex_valid_book_name)) {
                            throw UtilityException("INVALID BOOK NAME TRAIT");
                        }
                        new_info.name = map["name"];
                    }
                    if (map.count("author")) {
                        if (!matchRegex(map["author"], regex_valid_author)) {
                            throw UtilityException("INVALID AUTHOR TRAIT");
                        }
                        new_info.author = map["author"];
                    }
                    if (map.count("keyword")) {
                        if (!matchRegex(map["keyword"], regex_valid_keyword)) {
                            throw UtilityException("INVALID KEYWORD TRAIT");
                        }
                        new_info.keyword = map["keyword"];
                    }
                    if (map.count("price")) {
                        if (!matchRegex(map["price"], regex_valid_price)) {
                            throw UtilityException("INVALID PRICE TRAIT");
                        }
                        new_info.price = stringToDouble(map["price"]);
                    }
                    interface->Modify(new_info);
                }
                else if (matchRegex(line, regex_import)) {
                    auto group = groupRegex(line, regex_import);
                    interface->Import(stringToInteger(group[1]), stringToDouble(group[2]));
                }
                else if (matchRegex(line, regex_show_finance_all)) {
                    auto finance = interface->GetFinance();
                    std::cout << std::fixed << std::setprecision(2) << "+ " << finance.first << " - " << std::abs(finance.second) << '\n';
                }
                else if (matchRegex(line, regex_show_finance_selective)) {
                    auto group = groupRegex(line, regex_show_finance_selective);
                    const int count = stringToInteger(group[1]);
                    if (count == 0) {
                        std::cout << '\n';
                    }
                    else {
                        auto finance = interface->GetFinance(count);
                        std::cout << std::fixed << std::setprecision(2) << "+ " << finance.first << " - " << std::abs(finance.second) << '\n';
                    }
                }
                else if (matchRegex(line, regex_report_finance)) {
                    // TODO: report finance
                }
                else if (matchRegex(line, regex_report_employee)) {
                    // TODO: report finance
                }
                else if (matchRegex(line, regex_log)) {
                    // TODO: log
                }
                else {
                    throw UtilityException("UNKNOWN LINE ERROR");
                }
             }
            catch (QuitUtilityException &) {
                break;
            }
            catch (UtilityException &exception) {
                std::cerr << exception.what() << '\n';
                std::cout << "Invalid!" << '\n';
                continue;
            }
            catch (...) {
                std::rethrow_exception(std::current_exception());
                // This will directly be passed on to the terminal.
                // It will abort the software and perform all clean-up immediately.
                break;
            }
        }
    }

}
