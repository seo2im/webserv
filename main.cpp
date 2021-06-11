#include "app.hpp"

int main(int ac, char *av[]) {
    App app;

    try {
        app.init(av[1]);
        std::cout << "init Success\n";
        app.setup();
        std::cout << "setup Success\n";
        app.run();
    } catch (std::exception &e) {

    }
}
