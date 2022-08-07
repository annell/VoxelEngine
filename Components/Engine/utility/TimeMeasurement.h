#pragma once

#include "imgui.h"
#include <chrono>
#include <map>
#include <string>
#include <utility>

namespace voxie {
    struct TimeMeasurment {
        static void Start(const std::string &key) {
            measurment.emplace(std::make_pair(key, TimeSpan()));
        }

        static void End(const std::string &key) {
            auto it = measurment.find(key);
            if (it != measurment.end()) {
                it->second.end = std::chrono::steady_clock::now();
            } else {
                assert(!"Key not found! Probably did not start a measurment with the given key.");
            }
        }

        static void RenderResults() {
            std::vector<float> values;
            values.resize(measurment.size());
            std::string label = "Measurments:\n";
            int idx = 0;
            float max = 0;
            for (const auto it : measurment) {
                float val = it.second.count<std::chrono::milliseconds>();
                values.at(idx) = val;
                label += std::to_string(idx++) + " - " + it.first + "\n";
                max = std::max(max, val);
            }
            ImGui::Begin("Performance");

            std::string fpsStr = "FPS: " + std::to_string(fps);
            ImGui::PlotHistogram(label.c_str(), values.data(), values.size(), 0, "[ms]", 0.0f, max + 5);

            int fps = 0;
            if (max != 0) {
                fps = 1 / (max/1000);
            }

            static std::array<float, 1000> renderTimes;
            static int index = 0;
            auto inserter = [&] () {
                renderTimes[index++] = fps;
                if (index >= renderTimes.size()) {
                    index = 0;
                    renderTimes.fill(0);
                }
            };

            inserter();

            ImGui::PlotLines("Loop time\nover 1000 frames", renderTimes.data(), renderTimes.size(), 0, fpsStr.c_str(), 0, fps + 5);

            ImGui::End();
            TimeMeasurment::Clear();
        }

    private:
        static void Clear() {
            measurment.clear();
        }
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
    std::map<std::string, TimeMeasurment::TimeSpan> TimeMeasurment::measurment = {};
}// namespace voxie