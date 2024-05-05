//
// Created by KirilliriK on 28.04.2024.
//

#ifndef OPTIONS_HPP
#define OPTIONS_HPP
#include <fstream>
#include <iostream>
#include <toml++/toml.hpp>

#endif //OPTIONS_HPP

namespace Avox {
    class Options {
    public:
        static void loadOptions() {
            try {
                toml::table table = toml::parse_file(R"(..\game\resources\options.toml)");
                useConfiguration = table["use_configuration"].as_boolean()->get();
                width = table["width"].as_integer()->get();
                height = table["height"].as_integer()->get();
                radius = table["radius"].as_integer()->get();
            } catch (const toml::parse_error& err) {
                useConfiguration = false;
                width = 2560;
                height = 1440;
                radius = 2;
                std::cerr << "Parsing failed:\n" << err << "\n";
            }
        }

        static bool useConfiguration;
        static int radius;
        static int width;
        static int height;
    };
}