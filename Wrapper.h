#ifndef WRAPPER_H
#define WRAPPER_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <any>
#include <vector>
#include <stdexcept>
using DefinedArg = std::unordered_map<std::string, std::any>;

class CommandWrapper {
public:
    virtual ~CommandWrapper() = default;
    virtual void execute(const DefinedArg& args) = 0;
};


template <typename T, typename Func, typename... Args>
class Wrapper : public CommandWrapper {
private:
    T* obj;
    Func (T::*method)(Args...);  

public:
    Wrapper(T* obj, Func (T::*method)(Args...), DefinedArg defaultArgs)
        : obj(obj), method(method) {}

    void execute(const DefinedArg & args) override {
        auto convertedArgs = convertArguments(args);
        invoke(convertedArgs, std::index_sequence_for<Args...>());
    }

private:
    std::tuple<Args...> convertArguments(const DefinedArg& args) {
        return collectArguments(args, std::index_sequence_for<Args...>());
    }

    template <std::size_t... Is>
    std::tuple<Args...> collectArguments(const DefinedArg& args, std::index_sequence<Is...>) {
        return std::make_tuple(getArgument<Args>(args, Is)...);
    }

    template <typename Arg>
    Arg getArgument(const DefinedArg& args, std::size_t index) {
        auto it = args.begin();
        std::advance(it, index);
        if (it != args.end()) {
            try {
                return std::any_cast<Arg>(it->second);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("Invalid type for argument: " + it->first);
            }
        }
        throw std::invalid_argument("Argument not found: " + it->first);
    }
    template <std::size_t... Is>
    void invoke(const std::tuple<Args...>& args, std::index_sequence<Is...>) {
        (obj->*method)(std::get<Is>(args)...);
    }
};

#endif