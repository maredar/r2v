#ifndef R2V_INCLUDE_CONNECTIONS_HPP_INCLUDED
#define R2V_INCLUDE_CONNECTIONS_HPP_INCLUDED

#include "data_source.hpp"
#include "entity.hpp"
#include "parser.hpp"

#include <memory>
#include <unordered_map>
#include <list>
#include <utility>
#include <numeric>

namespace r2v {

class connected_map {
    using entity_list_t = IParser::entity_list_t;
    using houses_list_t = std::list<std::pair<double, const house* const>>;
    using connections_list_t = std::unordered_map<std::string, houses_list_t>;
public:
    explicit connected_map(std::shared_ptr<IBitmapSource> source, std::shared_ptr<IParser> parse_algorithm) 
        : _entities{ parse_algorithm->operator()(std::move(source)) }
        , _connections{}
    {
        for(const auto& h : _entities[1]) {
            std::pair<double, std::string_view> min_distance{ std::numeric_limits<double>::infinity(), {} };
            for(const auto& s : _entities[2]) {
                auto d = distance(h->center(), s->center());
                if(d < min_distance.first) {
                    min_distance.first = d;
                    min_distance.second = s->name();
                }
            }
            if(!min_distance.second.empty()) {
                _connections[std::string(min_distance.second)].push_back({ min_distance.first, dynamic_cast<house*>(h.get()) });
            }
        }
    }

    void print_all_entities(std::ostream& out) const {
        const auto& houses = _entities.at(1);
        std::cout << std::setw(10) << "Дома:";
        std::cout << "\t<" << std::setw(23) << "Название  " << ">";
        std::cout << "\t<" << std::setw(28) << "Центр       " << ">";
        std::cout << "\t<" << std::setw(21) << "Размер   " << ">\n";
        for(const auto& h : houses) {
            std::cout << "\t<"
                      << std::setw(15) << h->name() << ">"
                      << "\t<"
                      << std::setw(10) << std::fixed << std::setprecision(0) << h->center().x
                      << " : "
                      << std::setw(10) << std::fixed << std::setprecision(0) << h->center().y
                      << ">"
                      << "\t<"
                      << std::setw(6) << h->size().x
                      << " : "
                      << std::setw(6) << h->size().y
                      << ">\n";
        }

        const auto& stations = _entities.at(2);       
        std::cout << std::endl;
        std::cout << std::setw(10) << "Станц:";
        std::cout << "\t<" << std::setw(23) << "Название  " << ">";
        std::cout << "\t<" << std::setw(28) << "Центр       " << ">\n";
        for(const auto& s : stations) {
            std::cout << "\t<"
                      << std::setw(15) << s->name() << ">"
                      << "\t<"
                      << std::setw(10) << std::fixed << std::setprecision(0) << s->center().x
                      << " : "
                      << std::setw(10) << std::fixed << std::setprecision(0) << s->center().y
                      << ">\n";
        }
    }

    void print_all_connections(std::ostream& out) const {
        for(const auto& [station_name, connected] : _connections) {
            out << station_name << "\t:";
            for(const auto& [dist, h] : connected) {
                out << h->name() << "( " << std::setprecision(5) << dist << " ), ";
            }
            out << "\n";
        }
    }

    houses_list_t get_connections(const std::string& station_name) const {
        if(_connections.contains(station_name)) {
            return _connections.at(station_name);
        }
        return {};
    }

private:
    entity_list_t _entities;
    connections_list_t _connections;
};

} // namespace r2v

#endif