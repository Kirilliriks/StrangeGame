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
            defaultOptions();

            try {
                toml::table table = toml::parse_file(R"(..\game\resources\options.toml)");
                useConfiguration = table["use_configuration"].as_boolean()->get();

                if (useConfiguration) {
                    width = table["width"].as_integer()->get();
                    height = table["height"].as_integer()->get();
                    radius = table["radius"].as_integer()->get();
                }
            } catch (const toml::parse_error& err) {
                std::cerr << "Parsing failed:\n" << err << "\n";
            }
        }

        static void defaultOptions() {
            useConfiguration = false;
            width = 2560;
            height = 1440;
            radius = 0;
        }

        static bool useConfiguration;
        static int radius;
        static int width;
        static int height;
    };
}