#ifndef R2V_INCLUDE_ENTITY_HPP_INCLUDED
#define R2V_INCLUDE_ENTITY_HPP_INCLUDED

#include "point2.hpp"

#include <set>
#include <cstdint>
#include <optional>
#include <memory>
#include <string_view>
#include <string>
#include <iostream>

namespace r2v {

class IEntity {
public:
    using symbol_t = unsigned char;
    using point_t = point2<uint32_t>;
    using dpoint_t = point2<double>;
    using pos_container_t = std::set<point_t>;

    explicit IEntity(symbol_t symbol) : ID{ 0 }, SYMBOL{ symbol } {}
    virtual ~IEntity() noexcept = default;

    virtual pos_container_t get_points() const = 0;
    virtual void add_point(const point_t& p) = 0;
    virtual bool is_valid() const = 0;
    virtual symbol_t get_symbol() const {
        return this->SYMBOL;
    }
    virtual bool is_same(symbol_t s) const {
        return s == SYMBOL;
    } 
    virtual dpoint_t center() const = 0;
    virtual std::string_view name() const = 0;
    virtual point_t size() const = 0;

protected:
    std::size_t ID;

private:
    const symbol_t SYMBOL;
};

class house : public IEntity {
public:
    house()
        : IEntity(1)
        , _center{}
        , _top_left{ nullptr }
        , _bot_left{ nullptr }
        , _top_right{ nullptr }
        , _bot_right{ nullptr }
        , _name{} 
    {
        static std::size_t COUNTER = 1;
        _name = "H#" + std::to_string(COUNTER++);
    }
    ~house() noexcept override = default;

    pos_container_t get_points() const override {
        return _points;
    }

    void add_point(const point_t& p) override {
        _points.insert(p);

        _top_left = &*(_points.begin());
        _bot_right = &*(_points.rbegin());

        auto top_right_search = _points.find(point_t{ _top_left->x, _bot_right->y });
        _top_right = top_right_search == _points.end() ? nullptr : &*top_right_search;

        auto bot_left_search = _points.find(point_t{ _bot_right->x, _top_left->y });
        _bot_left = bot_left_search == _points.end() ? nullptr : &*bot_left_search;

        auto d1center = dpoint_t{ (_bot_right->x + _top_left->x) / 2.0, (_bot_right->y + _top_left->y) / 2.0 };
        _center = d1center;
    }

    bool is_valid() const override {
        if(_top_left == nullptr || _top_right == nullptr || _bot_right == nullptr || _bot_left == nullptr) {
            return false;
        }
        if(auto rec_size = size(); _points.size() < 9 || rec_size.x < 3 || rec_size.y < 3) {
            return false;
        } 
        if(_top_left->y != 0) {
            for(int x = _top_left->x; x <= _top_right->x; ++x) {
                auto found_over_top = _points.find(point_t{ x, _top_left->y - 1 });
                if(found_over_top != _points.end()) {
                    return false;
                }
            }
        }
        if(_bot_right->y != std::numeric_limits<decltype(point_t::y)>::max()) {
            for(int x = _bot_right->x; x >= _bot_left->x; --x) {
                auto found_over_bot = _points.find(point_t{ x, _bot_right->y + 1 });
                if(found_over_bot != _points.end()) {
                    return false;
                }
            }
        }
        if(area() != _points.size()) {
            return false;
        }
        return true;
    }

    std::optional<point_t> top_left() const {
        return _top_left == nullptr ? std::nullopt : std::optional(*_top_left);
    }

    std::optional<point_t> bot_right() const {
        return _bot_right == nullptr ? std::nullopt : std::optional(*_top_left);
    }

    std::optional<point_t> bot_left() const {
        return _bot_left == nullptr ? std::nullopt : std::optional(*_bot_left);
    }

    std::optional<point_t> top_right() const {
        return _top_right == nullptr ? std::nullopt : std::optional(*_top_right);
    }

    dpoint_t center() const override {
        return _center;
    }

    std::string_view name() const override {
        return _name;
    }

    virtual point_t size() const override {
        auto w = _bot_right->x - _top_left->x + 1;
        auto h = _bot_right->y - _top_left->y + 1;
        return point_t{ w, h };
    }

private:
    uint64_t area() const {
        auto sz = size();
        return sz.x * sz.y;
    }

private:
    pos_container_t _points;
    dpoint_t _center;
    const point_t* _top_left;
    const point_t* _bot_left;
    const point_t* _top_right;
    const point_t* _bot_right;

    std::string _name;
};

class station : public IEntity {
public:
    station()
        : IEntity(2)
        , _points{}
        , _name{}
    {
        static std::size_t COUNTER = 1;
        _name = "S#" + std::to_string(COUNTER++);
    }

    pos_container_t get_points() const override {
        return _points;
    }

    void add_point(const point_t& p) override {
        _points.insert(p);
    }

    bool is_valid() const override {
        return _points.size() == 1; 
    }

    dpoint_t center() const override {
        auto center = *_points.begin();
        return dpoint_t{ static_cast<double>(center.x), static_cast<double>(center.y) };
    }

    std::string_view name() const override {
        return _name;
    }
    
    point_t size() const override {
        return point_t{1, 1};
    }

private:
    pos_container_t _points;
    std::string _name;
};

class entity_builder {
    using symbol_t = IEntity::symbol_t;
public:
    static std::unique_ptr<IEntity> build(symbol_t symbol) {
        switch(symbol) {
            case 1:
                return std::make_unique<house>();
            case 2:
                return std::make_unique<station>();
            default:
                throw std::invalid_argument("Unknown IEntity symbol identifier: " + std::to_string(symbol));
        }
    }
private:
    entity_builder() = default;
};

} // namespace r2v

#endif
