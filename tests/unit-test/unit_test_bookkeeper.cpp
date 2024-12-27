#include <iostream>

#include "bookkeeper.hpp"

int main() {
    std::fstream f_uid, f_global;
    norb::utils::fAssert("test-uid.bin");
    norb::utils::fAssert("test-global.bin");
    const auto mode = std::ios::binary | std::ios::in | std::ios::out;
    f_uid.open("test-uid.bin", mode);
    f_global.open("test-global.bin", mode);
    int _ = 0;
    norb::utils::bWrite(f_uid, _);
    f_uid.seekg(0);
    f_global.seekg(0);
    {
        norb::BookManager manager(f_uid, f_global);

        // Test Sel-Mod
        // TODO: check whether this behaviour is okay
        manager.StackCreate();          // ? Observation: without this line, the program will RE !
        manager.Select("001");
        manager.Modify({
            "a", "Minecraft Handbook", "norb", "mc|gaming|tutorial"
        });
    }

    // manager.Select("888");
    // manager.Modify({
    //     "b", "Redstone Engineering", "norb", "mc|redstone|intro"
    // });
    // manager.Select("000");
    // manager.Modify({
    //     "c", "Mining", "norb again", "mc|survival|tutorial"
    // });
    // manager.Select("000");
    // manager.Modify({
    //     "d", "Survival Engineering", "by norb", "mc|redstone|tutorial"
    // });
    // manager.Select("000");
    // manager.Modify({
    //     "e", "Botania!!!", "norb", "mc|intro|mods"
    // });
    //
    // // Test Find
    // std::cout << "Find\n";
    // std::cout << ' ' <<  manager.FindAll() << '\n';
    // std::cout << ' ' <<  manager.FindByAuthor("norb") << '\n';
    // std::cout << ' ' <<  manager.FindByKeyword("mc") << '\n';
    // std::cout << ' ' <<  manager.FindByKeyword("redstone") << '\n';
    // std::cout << ' ' <<  manager.FindByKeyword("survival|mc") << '\n';
    // std::cout << ' ' <<  manager.FindByIsbn("e");
    // std::cout << ' ' <<  manager.FindByName("Botania!!!");
    //
    // // Test Buy/Import
    // std::cout << "Buy/Import\n";
    // std::cout << manager.Buy(manager.GetId("e"), 1) << '\n';
    // manager.Select("b");
    // std::cout << manager.Import(100, 15.72354121) << '\n';
    // std::cout << manager.Buy(manager.GetId("b"), 5) << '\n';
    // std::cout << "after: " << manager.GetInfo(manager.GetId("b")) << '\n';
    //
    // // Test delete
    // manager.StackCreate();
    // manager.Select("user-1-sel");
    // manager.StackCreate();
    // manager.Select("user-2-sel");
    // manager.StackCreate();
    // manager.Select("user-3");
    // manager.Modify({
    //     "user3", "How to make a computer", "Batbattwings", "mc|redstone|tutorial", 1
    // });
    // manager.StackRemove();
    // manager.Modify({
    //     "user2", "The Hunter and the Hunted", "Dream", "mc|survival"
    // });
    // manager.StackRemove();
    // manager.Modify({
    //     "user1", "Redstoned?", "Mumbo Jumbo", "mc|redstone|fun"
    // });
    // manager.StackRemove();
    // std::cout << ' ' << manager.FindAll() << '\n';
    // std::cout << ' ' << manager.FindByKeyword("tutorial") << '\n';

    return 0;
}