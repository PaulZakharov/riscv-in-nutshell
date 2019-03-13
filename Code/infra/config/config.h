#ifndef CONFIG_H
#define CONFIG_H

#include "../types.h"
#include <iostream>
#include <string>
#include <map>

// include boost only in CPP file
namespace boost {
    namespace program_options {
        class options_description;
    }
}

namespace config {
    namespace po = boost::program_options;
    using BoostDescription = po::options_description;

    void parse_args(int argc, char** argv);

    class BaseValue {
        friend void parse_args(int argc, char** argv);
    protected:
        static BoostDescription boost_description;
        const std::string name;
        const std::string description;

        BaseValue(const std::string name, const std::string description) :
            name(name),
            description(description)
        {}
    };

    template<typename T>
    class RequiredValue final : public BaseValue {
    private:
        T value;
        void init();
    public:
        RequiredValue<T>(const std::string name,
                         const std::string description) :
            BaseValue(name, description)
        { init(); }
        operator const T&() const { return value; }
    };

    template<typename T>
    class Value final : public BaseValue {
    private:
        const T default_value;
        T value;
        void init();
    public:
        Value<T>(const std::string name,
                 const std::string description,
                 const T& default_value) :
            BaseValue(name, description),
            default_value(default_value),
            value(default_value)
        { init(); }
        operator const T&() const { return value; }
    };
}

#endif
