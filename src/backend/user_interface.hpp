#pragma once

#include <regex>

#include "business_logic.hpp"
#include "constants.hpp"

namespace norb {
    class CommandlineUserInterface {
    public:
        explicit CommandlineUserInterface(std::unique_ptr<norb::BusinessLogicInterface>);
        ~CommandlineUserInterface();

        void Run();

    private:
        std::unique_ptr<norb::BusinessLogicInterface> interface;
    };
}