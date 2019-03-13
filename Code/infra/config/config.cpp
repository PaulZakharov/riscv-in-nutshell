#include "config.h"
#include <iostream>
#include <boost/program_options.hpp>

namespace config {
    BoostDescription BaseValue::boost_description("Allowed options");

    template<>
    void RequiredValue<bool>::init() {
        boost_description.add_options() (
            name.c_str(),
            po::bool_switch(&value),
            description.c_str()
        );
        std::cout << "Registered Required bool" << name << std::endl;
    }

    template<>
    void Value<bool>::init() {
        boost_description.add_options() (
            name.c_str(),
            po::bool_switch(&value)->default_value(default_value),
            description.c_str()
        );
        std::cout << "Registered bool" << name << std::endl;
    }

    template<typename T>
    void RequiredValue<T>::init() {
        boost_description.add_options() (
            name.c_str(),
            po::value<T>(&value)->required(),
            description.c_str()
        );
        std::cout << "Registered Required " << name << std::endl;
    }

    template<typename T>
    void Value<T>::init() {
        boost_description.add_options()(
            name.c_str(),
            po::value<T>(&value)->default_value(default_value),
            description.c_str()
        );
        std::cout << "Registered " << name << std::endl;
    }

    template class RequiredValue<std::string>;
    template class RequiredValue<uint64>;
    template class RequiredValue<uint32>;
    template class RequiredValue<int32>;
    template class Value<std::string>;
    template class Value<uint64>;
    template class Value<uint32>;
    template class Value<int32>;

    void parse_args(int argc, char** argv) {
        po::variables_map vm;
        std::cout << "Num arguments: " << argc << std::endl;

        try {
            po::store(
                po::command_line_parser(argc, argv)
                .options(BaseValue::boost_description).run(),
                vm
            );

            std::cout << "Parsed options!" << std::endl;

            if (vm.count("help") != 0u) {
                std::cout << "Usage:"
                          << std::endl << std::endl
                          << BaseValue::boost_description << std::endl;
                std::exit(EXIT_SUCCESS);
            }

            po::notify(vm);
        }
        catch ( const std::exception& e) {
            std::cerr << argv[0] << ": " << e.what()
                      << std::endl << std::endl
                      << BaseValue::boost_description << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

