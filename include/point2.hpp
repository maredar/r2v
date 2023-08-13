#ifndef R2V_INCLUDE_POINT2_HPP_INCLUDED
#define R2V_INCLUDE_POINT2_HPP_INCLUDED

#include <cmath>
#include <iostream>

namespace r2v {

template<typename T>
struct point2 {
    T x, y;
    explicit point2(T x = 0, T y = 0)
        : x{ x }, y{ y }
    {
        //empty
    }
    inline bool operator<(const point2 &rhs) const {
        return x < rhs.x || (x == rhs.x && y < rhs.y);
    }
};

template<typename T>
inline double distance(const point2<T>& from, const point2<T>& to) {
    auto x_dist = (to.x > from.x) ? to.x - from.x : from.x - to.x;
    auto y_dist = (to.y > from.y) ? to.y - from.y : from.y - to.y;

    double x_dist_squared = std::pow(x_dist, 2);
    double y_dist_squared = std::pow(y_dist, 2);
    return std::sqrt( x_dist_squared + y_dist_squared );
}

} // namespace r2v
#endif