#include "config.hpp"
#include <iostream>

namespace config {
    template<>
    void RequiredValue<bool>::init() {
        bd().add_options() (
            name.c_str(),
            po::bool_switch(&value),
            description.c_str()
        );
    }

    template<>
    void Value<bool>::init() {
        bd().add_options() (
            name.c_str(),
            po::bool_switch(&value)->default_value(default_value),
            description.c_str()
        );
    }

    template<typename T>
    void RequiredValue<T>::init() {
        bd().add_options() (
            name.c_str(),
            po::value<T>(&value)->required(),
            description.c_str()
        );
    }

    template<typename T>
    void Value<T>::init() {
        bd().add_options()(
            name.c_str(),
            po::value<T>(&value)->default_value(default_value),
            description.c_str()
        );
    }

    template class RequiredValue<std::string>;
    template class RequiredValue<uint64>;
    template class Value<uint64>;
    template class Value<bool>;

    void parse_args(int argc, char** argv) {
        po::variables_map vm;
        auto& desc = BaseValue::bd();

        try {
            po::store(
                po::parse_command_line(argc, argv, desc),
                vm
            );

            if (vm.count("help")) {
                std::cout << "Usage:"
                          << std::endl
                          << std::endl
                          << desc
                          << std::endl;
                std::exit(EXIT_SUCCESS);
            }

            po::notify(vm);
        }
        catch (const std::exception& e) {
            std::cerr << argv[0] << ": " << e.what()
                      << std::endl
                      << std::endl
                      << desc
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

