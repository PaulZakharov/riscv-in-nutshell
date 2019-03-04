#include "config.h"
#include <iostream>
#include <boost/program_options.hpp>

namespace config {
    namespace po = boost::program_options;
    Description BaseValue::description("Allowed options");

    template<>
    void RequiredValue<bool>::reg() {
        description.add_options()(
            name.c_str(),
            po::bool_switch(&value),
            desc.c_str());
            std::cout << "Regsitered " << name << std::endl;
    }

    template<>
    void Value<bool>::reg() {
        description.add_options()(
            name.c_str(),
            po::bool_switch(&value)->default_value(default_value),
            desc.c_str());
    }

    template<typename T>
    void RequiredValue<T>::reg() {
        description.add_options()(
            this->name.c_str(),
            po::value<T>(&value)->required(),
            desc.c_str());
    }

    template<typename T>
    void Value<T>::reg() {
        this->description.add_options()(
            this->name.c_str(),
            po::value<T>(&this->value)->default_value(this->default_value),
            this->desc.c_str());
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
        std::cout << argc << std::endl;
        try {
            po::store(
                po::command_line_parser(argc, argv)
                .options(BaseValue::description).run(),
                vm
            );

            if (vm.count("help") != 0u) {
                std::cout << "Usage:"
                          << std::endl << std::endl
                          << BaseValue::description << std::endl;
                std::exit(EXIT_SUCCESS);
            }

            po::notify(vm);
        }
        catch ( const std::exception& e) {
            std::cerr << argv[0] << ": " << e.what()
                      << std::endl << std::endl
                      << BaseValue::description << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

