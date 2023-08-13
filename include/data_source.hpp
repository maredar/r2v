#ifndef R2V_INCLUDE_DATA_SOURCE_HPP_INCLUDED
#define R2V_INCLUDE_DATA_SOURCE_HPP_INCLUDED

#include <map>
#include <cstdint>
#include <filesystem>
#include <fstream>

#include "point2.hpp"

namespace r2v {

class IBitmapSource {
public:
    using byte_t = char;
    using point_t = point2<uint32_t>;
    using data_t = std::map<point_t, byte_t>;
    virtual data_t get_data(bool ignore_zero) const = 0; 
    virtual ~IBitmapSource() = default;
};

class map_file : public IBitmapSource {
public:
    explicit map_file(const std::filesystem::path& path)
        : _path{path}
    {
        if(!std::filesystem::exists(_path) || !std::filesystem::is_regular_file(_path)) {
            throw std::invalid_argument("Invalid file at path: " + path.string());
        }
    }

    data_t get_data(bool ignore_zero) const override {
        std::basic_ifstream<byte_t> in_stream{};
        in_stream.setf(std::ios::hex);
        in_stream.setf(std::ios::skipws);
        in_stream.exceptions(std::ifstream::failbit | std::ifstream::eofbit | std::ifstream::badbit);

        in_stream.open(_path.c_str(), std::ios::in | std::ios::binary);
        if (!in_stream.is_open()) {
            throw std::runtime_error("Failed to open file: " + _path.string());
        }
        uint32_t width{};
        uint32_t height{};
        data_t result{};
        try {
            in_stream.read(reinterpret_cast<byte_t *>(&width), sizeof(width));
            in_stream.read(reinterpret_cast<byte_t *>(&height), sizeof(height));

            for(uint32_t w = 0; w < width; ++w) {
                for(uint32_t h = 0; h < height; ++h) {
                    byte_t byte{};
                    in_stream.read(reinterpret_cast<byte_t*>(&byte), sizeof(byte));
                    if(byte == 0 && ignore_zero) {
                        continue;
                    }
                    result.insert({ point2{ w, h }, byte });
                }
            }
        }
        catch(const std::exception& ex) {
            in_stream.close();
            throw;
        }
        in_stream.close();
        return result;
    }

private:
    std::filesystem::path _path;
};

} // namespace r2v

#endif