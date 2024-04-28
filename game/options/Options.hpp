//
// Created by KirilliriK on 28.04.2024.
//

#ifndef OPTIONS_HPP
#define OPTIONS_HPP
#include <fstream>
#include <iostream>
#include <exception>
#include <toml++/toml.hpp>

#endif //OPTIONS_HPP

namespace Avox {
    class Options {
    public:
        static void loadOptions() {
            try {
                toml::table table = toml::parse_file(R"(..\game\resources\options.toml)");
                width = table["width"].as_integer()->get();
                height = table["height"].as_integer()->get();
                radius = table["radius"].as_integer()->get();
            } catch (const std::exception& err) {
                width = 2560;
                height = 1440;
                radius = 2;
                std::cerr << "Parsing failed:\n" << err.what() << "\n";
            }
        }

        static int radius;
        static int width;
        static int height;
    private:
    };
}