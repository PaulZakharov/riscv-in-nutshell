#ifndef CONFIG_H
#define CONFIG_H

#include "../types.hpp"
#include <iostream>
#include <string>
#include <map>
#include <boost/program_options.hpp>


namespace config {
    namespace po = boost::program_options;
    using BoostDescription = po::options_description;

    void parse_args(int argc, char** argv);

    class BaseValue {
    private:
        friend void parse_args(int argc, char** argv);
    protected:
        static BoostDescription& bd() {
            static BoostDescription desc("Allowed options");
            return desc;
        }
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
