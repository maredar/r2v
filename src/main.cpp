#include "app.hpp"

void print_usage() {
    std::cout << "R2V: Необходимо указать путь к .dat файлу\n";
}

int main(int argc, char **argv) {
    if(argc != 2) {
        print_usage();
        return EXIT_FAILURE;
    }
    try {
        r2v::application a{argv[1]};
        a.run(); 
    } catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}