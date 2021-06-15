#include "app.hpp"

int main(int ac, char *av[]) {
    App app;

    try {
        app.init(av[1]);
        std::cout << "CONFIG SETTING CLEAR!!\n"; 
        app.setup();
        std::cout << "SERVER SETTING CLEAR!!\n"; 
        app.run();
    } catch (std::exception &e) {

    }
}
