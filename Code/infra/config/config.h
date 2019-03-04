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
    using Description = boost::program_options::options_description;

    class BaseValue {
        friend void parse_args(int argc, char** argv);
    protected:
        static Description description;
        const std::string name;
        const std::string desc;

        BaseValue(const std::string name, const std::string desc) :
            name(name),
            desc(desc)
        
        {}
    };


    template<typename T>
    class RequiredValue : public BaseValue {
    protected:
        T value;
        RequiredValue<T>() = delete;
        void reg();
    public:
        RequiredValue<T>(const std::string name,
                         const std::string desc) :
            BaseValue(name, desc)
        {
            this->reg();
        }
        operator const T&() const { return value; }
    };

    template<typename T>
    class Value : public RequiredValue<T> {
        const T default_value;
        Value<T>() = delete;
        void reg();
    public:
        Value<T>(const std::string name,
                 const T& val,
                 const std::string desc) :
            RequiredValue<T>(name, desc),
            default_value(val)
        {
            this->value = val;
            this->reg();
        }
    };

    void parse_args(int argc, char** argv);
}

#endif

