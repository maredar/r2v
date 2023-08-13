#ifndef R2V_INCLUDE_APP_HPP_INCLUDED
#define R2V_INCLUDE_APP_HPP_INCLUDED

#include "connections.hpp"

#include <memory>
#include <filesystem>
#include <iostream>

namespace r2v {

class application {
public:
    explicit application(const std::filesystem::path& data_file_path)
        : _data{ std::make_shared<map_file>(data_file_path) } 
        , _map{ _data, std::make_shared<parser>() }
    {

    }

    void run() {
        std::string token{};
        print_token_usage();
        while(true) {
            std::cout << " r2v > ";
            std::cin >> token;
            if (token == "q") {
                break;
            }
            if(auto found = token.find("S#"); found != std::string::npos) {
                std::cout << token << " : ";
                auto connected = _map.get_connections(token);
                for(const auto& [dist, h] : connected) {
                    std::cout << h->name() << " ( " << std::setprecision(2) << dist << " ), ";
                }
                std::cout << "\n";
            }
            else if(token == "e") {
                _map.print_all_entities(std::cout);
            }
            else if(token == "all") {
                _map.print_all_connections(std::cout);
            }
            else {
                std::cout << "Не распознанная команда: " << token << std::endl;
                print_token_usage();
            }
        }
    }

private:
    void print_token_usage() {
        std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
        std::cout << "\t- \"Имя станции\" \t - отображение подключенных к станции\n";
        std::cout << "\t- all\t\t - отображение всех подключений всех станций\n";
        std::cout << "\t- e\t\t - отображение всех объектов\n";
        std::cout << "\t- q\t\t - выход из программы\n";
    }

private:
    std::shared_ptr<IBitmapSource> _data;
    connected_map _map;
};

} // namespace r2v

#endif