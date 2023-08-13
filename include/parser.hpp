#ifndef R2V_INCLUDE_PARSER_HPP_INCLUDED
#define R2V_INCLUDE_PARSER_HPP_INCLUDED

#include "entity.hpp"
#include "data_source.hpp"

#include <memory>
#include <list>
#include <unordered_map>

namespace r2v {

class IParser {
public:
    using point_t = point2<uint32_t>;
    using data_t = IBitmapSource::data_t;
    using entity_list_t = std::unordered_map<IEntity::symbol_t, std::list<std::unique_ptr<IEntity>>>;
    virtual entity_list_t operator()(std::shared_ptr<IBitmapSource> source) const = 0;
    virtual ~IParser() = default;
};

class parser : public IParser {
public:
    entity_list_t operator()(std::shared_ptr<IBitmapSource> source) const override {
        entity_list_t result{};
        auto data = source->get_data(true);

        auto it = data.begin();
        auto end = data.end();
        while(!data.empty() && it != data.end()) {
            auto entity = entity_builder::build(it->second);
            find_all_entity_points(entity.get(), &data, it->first);
            if(entity->is_valid()) {
                result[entity->get_symbol()].push_back(std::move(entity));
            }
            it = data.begin();
            end = data.end();
        }
        return result;
    }
private:
    static void find_all_entity_points(IEntity* e, IBitmapSource::data_t* d, const point_t& curr) {
        std::set<point_t> possible_neighbours{
            point2{ curr.x == std::numeric_limits<decltype(curr.x)>::max() ? curr.x : curr.x + 1, curr.y },
            point2{ curr.x, curr.y == std::numeric_limits<decltype(curr.y)>::max() ? curr.y : curr.y + 1 },
            point2{ curr.x == 0 ? 0 : curr.x - 1, curr.y },
            point2{ curr.x, curr.y == 0 ? 0 : curr.y - 1 }
        };
        e->add_point(curr);
        d->erase(curr);
        for (const auto &n : possible_neighbours)
        {
            auto found_neighbour = d->find(n);
            if (found_neighbour != d->end())
            {
                const auto found_symb = found_neighbour->second;
                if (e->is_same(found_symb))
                {
                    auto found_point = found_neighbour->first;
                    find_all_entity_points(e, d, found_point);
                }
            }
        }
    }
};

} // namespace r2v

#endif
