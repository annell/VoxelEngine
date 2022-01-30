//
// Created by Stefan Annell on 2021-05-09.
//

#pragma once

#include <functional>
#include <map>

namespace voxie {


    template<typename... Values>
    class Delegate {
        using Function = std::function<void(Values...)>;

    public:
        int Bind(Function);
        void Unbind(int);

        void Broadcast(Values...);

    private:
        std::map<int, Function> callbacks;
        int id = 0;
    };

    template<typename... Values>
    int Delegate<Values...>::Bind(Delegate::Function function) {
        id++;
        callbacks[id] = function;
        return id;
    }

    template<typename... Values>
    void Delegate<Values...>::Unbind(int callbackId) {
        callbacks.erase(callbackId);
    }

    template<typename... Values>
    void Delegate<Values...>::Broadcast(Values... args) {
        for (auto pair : callbacks) {
            auto &callback = pair.second;
            callback(std::forward<Values>(args)...);
        }
    }

}// namespace voxie
