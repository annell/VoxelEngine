#pragma once

#include "imgui.h"
#include <chrono>
#include <map>
#include <string>

namespace voxie {
    struct TimeMeasurment {
        static void Start(const std::string &key);
        static void End(const std::string &key);
        static void RenderResults();

    private:
        static void Clear();
        
        struct TimeSpan {
            TimeSpan() : start(std::chrono::steady_clock::now()), end(std::chrono::steady_clock::now()) {}
            std::chrono::steady_clock::time_point start;
            std::chrono::steady_clock::time_point end;
            template<typename T>
            int count() const {
                return std::chrono::duration_cast<T>(end - start).count();
            }
        };
        static std::map<std::string, TimeSpan> measurment;
    };
}// namespace voxie