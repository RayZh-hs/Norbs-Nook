#include "user_interface.hpp"

#include <iomanip>
#include <iostream>

#include "norb_strlib.hpp"
#include "norb_con.hpp"
#include "json.hpp"     // Used for GUI, since all inputs and outputs are json strings.

using json = nlohmann::json;
using namespace norb::norbs_nook_constants::validation_constants;

namespace norb {

    // The Commandline User Interface is the one that the OJ accepts.
    // All the inputs and outputs strictly follow the requirements.

    CommandlineUserInterface::CommandlineUserInterface(std::unique_ptr<norb::BusinessLogicInterface> moved_interface) {
        interface = std::move(moved_interface);
    }

    CommandlineUserInterface::~CommandlineUserInterface() = default;

    void CommandlineUserInterface::Run() {
        std::cerr << "[Hello World] from user_interface.cpp. This is Norb's Nook, up and running." << std::endl;
        // Run the main cycle.
        //! int line = 118;
        //! int line = 1;
        //! freopen("output_capture.wrong.out.roaming", "w", stdout);
        while (true) {
            // Read in the next line of action.
            //! std::cout << "LINE " << line++ << '\n';
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::cerr << "Reached end of control without QUIT or EXIT! Quitting..." << '\n';
                break;
            }
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
                    if (books.empty()) {
                        std::cout << '\n';
                    } else {
                        for (auto i : books) {
                            std::cout << i;
                        }
                    }
                }
                else if (matchRegex(line, regex_show_author)) {
                    auto group = groupRegex(line, regex_show_author);
                    auto books = interface->FindByAuthor(group[1]);
                    if (books.empty()) {
                        std::cout << '\n';
                    } else {
                        for (auto i : books) {
                            std::cout << i;
                        }
                    }
                }
                else if (matchRegex(line, regex_show_name)) {
                    auto group = groupRegex(line, regex_show_name);
                    auto books = interface->FindByName(group[1]);
                    if (books.empty()) {
                        std::cout << '\n';
                    } else {
                        for (auto i : books) {
                            std::cout << i;
                        }
                    }
                }
                else if (matchRegex(line, regex_show_keyword)) {
                    auto group = groupRegex(line, regex_show_keyword);
                    auto books = interface->FindByKeyword(group[1]);
                    if (books.empty()) {
                        std::cout << '\n';
                    } else {
                        for (auto i : books) {
                            std::cout << i;
                        }
                    }
                }
                else if (matchRegex(line, regex_show_isbn)) {
                    auto group = groupRegex(line, regex_show_isbn);
                    auto books = interface->FindByIsbn(group[1]);
                    if (books.empty()) {
                        std::cout << '\n';
                    } else {
                        for (auto i : books) {
                            std::cout << i;
                        }
                    }
                }
                else if (matchRegex(line, regex_buy)) {
                    auto group = groupRegex(line, regex_buy);
                    auto money = interface->Buy(group[1], stringToInteger(group[2]));
                    std::cout << std::fixed << std::setprecision(2) << money << '\n';
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
                        std::string text = map["ISBN"];
                        if (!matchRegex(text, regex_valid_isbn)) {
                            throw UtilityException("INVALID ISBN TRAIT");
                        }
                        if (std::string(new_info.isbn) == text) {
                            throw UtilityException("IDENTICAL ISBN ERROR");
                        }
                        if (interface->IsbnIsOccupied(text)) {
                            throw UtilityException("ISBN IS OCCUPIED ERROR");
                        }
                        new_info.isbn = text;
                    }
                    if (map.count("name")) {
                        std::string text = map["name"];
                        if (!unquote(text) || !matchRegex(text, regex_valid_book_name)) {
                            throw UtilityException("INVALID BOOK NAME TRAIT");
                        }
                        new_info.name = text;
                    }
                    if (map.count("author")) {
                        std::string text = map["author"];
                        if (!unquote(text) || !matchRegex(text, regex_valid_author)) {
                            throw UtilityException("INVALID AUTHOR TRAIT");
                        }
                        new_info.author = text;
                    }
                    if (map.count("keyword")) {
                        std::string text = map["keyword"];
                        if (!unquote(text) || !matchRegex(text, regex_valid_keyword)) {
                            throw UtilityException("INVALID KEYWORD TRAIT");
                        }
                        new_info.keyword = text;
                        // Search for repetition in keywords here.
                        auto parsed_ = new_info.keyword.split_and_hash(book_keyword_separator);
                        std::set<lld> parsed_dump_;
                        for (auto i: parsed_) {
                            if (parsed_dump_.count(i)) {
                                throw UtilityException("REPETITIVE KEYWORD TRAIT");
                            } else {
                                parsed_dump_.insert(i);
                            }
                        }
                        // Reaching here means succeeding in the repetitive assertion.
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
                    auto finance_info = interface->ReportFinance();
                    double net_revenue = 0;
                    std::cout << std::fixed << std::setprecision(2);
                    for (int i = 0; i < finance_info.size(); i++) {
                        auto fin = finance_info[i];
                        std::cout << i + 1 << "#:\t " << (fin > 0 ? con::boldGreen : con::boldRed) << (fin > 0 ? "+" : "-") << std::abs(fin) << con::clear;
                        std::cout << '\t' << "Tot = ";
                        net_revenue += fin;
                        if (fin >= 0) {
                            std::cout << con::underlineGreen << '+' << net_revenue << con::clear << '\n';
                        }
                        else {
                            std::cout << con::underlineRed << net_revenue << con::clear << '\n';
                        }
                    }
                    std::cout << con::boldYellow << "- Total Revenue: " << net_revenue << con::clear << '\n';
                }
                else if (matchRegex(line, regex_report_employee)) {
                    // TODO: report finance
                    auto actions = interface->ReportEmployee();
                    for (auto i : actions) {
                        std::cout << i;
                    }
                }
                else if (matchRegex(line, regex_log)) {
                    // TODO: log
                    std::cout << con::clear;
                    for (const auto& i : interface->GetLog()) {
                        auto type = Logger::WhichLevel(i);
                        switch (type) {
                            case Logger::level::DEBUG:
                                std::cout << con::colorCyan;    break;
                            case Logger::level::INFO:
                                std::cout << con::colorGreen;   break;
                            case Logger::level::WARNING:
                                std::cout << con::colorYellow;  break;
                            case Logger::level::ERROR:
                                std::cout << con::colorRed;     break;
                            default:
                                std::cout << con::clear;        break;
                        }
                        std::cout << i;
                        std::cout << con::clear << '\n';
                    }
                }
                //! TODO For Debug purposes. Remove in final version !
                // else if (matchRegex(line, regex_debug_account)) {
                //     std::cout << "\t ACCOUNTS:\n";
                //     // auto accounts = interface->GetAccounts();
                //     // for (auto i : accounts) {
                //     //     std::cout << i << '\n';
                //     // }
                //     interface->DebugPrintAccountInfo();
                // }
                // else if (matchRegex(line, regex_debug_book)) {
                //     std::cout << "\t BOOKKEEPER:\n";
                //     interface->DebugPrintBookkeeperInfo();
                // }
                else {
                    throw UtilityException("UNKNOWN LINE ERROR");
                }
             }
            catch (QuitUtilityException &) {
                break;
            }
            catch (UtilityException &exception) {
                //! std::cerr << exception.what() << '\n';
                std::cout << "Invalid" << '\n';
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

    // The Graphical User Interface is used for local GUI builds.
    // All inputs and outputs are formatted as JSON objects so that flask can parse it and/or pass it more easily.

    GraphicalUserInterface::GraphicalUserInterface(std::unique_ptr<norb::BusinessLogicInterface> moved_interface) {
        interface = std::move(moved_interface);
    }

    GraphicalUserInterface::~GraphicalUserInterface() = default;

    void GraphicalUserInterface::Run() {
        freopen("./generated/logs/cerr-capture.log", "w", stderr);
        while(true) {
            try {
                json input;
                std::cin >> input;
                std::cerr << "[INFO] Received JSON: " << input << '\n';
                assert(std::cin.good());
                std::cerr << "[DEBUG] std::cin is still good" << '\n';
                std::string mode = input["mode"];
                if (mode == "login") {
                    try {
                        interface->SwitchUser(input["userid"], input["password"]);
                        std::cout << json::object({
                            {"status", "success"},
                            {"message", "successfully logged in"}
                        }) << '\n';
                        std::cerr << "[INFO] Login Successful" << '\n';
                    }
                    catch (UtilityException &) {
                        std::cout << json::object({
                            {"status", "failure"},
                            {"message", "wrong userid-password combination"}
                        }) << '\n';
                        std::cerr << "[WARNING] Login Failed" << '\n';
                    }
                }
                else if (mode == "register") {
                    try {
                        interface->Register(input["userid"], input["password"], input["username"]);
                        std::cout << json::object({
                            {"status", "success"},
                            {"message", "successful registration"}
                        }) << '\n';
                        std::cerr << "[INFO] Registration Successful" << '\n';
                    }
                    catch (UtilityException &) {
                        std::cout << json::object({
                            {"status", "failure"},
                            {"message", "please select another user-id"}
                        }) << '\n';
                        std::cerr << "[WARNING] Registration Failed" << '\n';
                    }
                }
            }
            catch (QuitUtilityException &) {
                return;
            }
            catch (...) {
                std::rethrow_exception(std::current_exception());
            }
        }
    }


}
