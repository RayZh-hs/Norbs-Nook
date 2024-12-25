#include "business_logic.hpp"
#include "user_interface.hpp"

int main() {
    // All the business logic are captured within the BLI classes.
    std::unique_ptr<norb::BusinessLogicInterface> bli = std::make_unique<norb::BusinessLogicImplement>();
    // All the user actions are captured within the UI classes.
    // The ownership of BLI will be transferred into CUI.
    norb::CommandlineUserInterface cui((std::move(bli)));
    cui.Run();
}   // According to stack allocation principles, CUI will be destructed first, then BLI.