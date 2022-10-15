//
// Created by Stefan Annell on 2021-03-21.
//
#pragma once
#include "JsonUtil.h"
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <variant>

namespace YAML {
    class Node;
}

namespace voxie {

    enum ShaderType {
        VERTEX,
        FRAGMENT,
    };

    struct Shader {
    public:
        using ShaderDefinition = std::string;
        unsigned int ID;
        explicit Shader(const std::map<ShaderDefinition, ShaderType> &shaders);

        void use() const;

        void setBool(const std::string &name, bool value);
        void setInt(const std::string &name, int value);
        void setFloat(const std::string &name, float value);
        void setVec2(const std::string &name, const glm::vec2 &value);
        void setVec2(const std::string &name, float x, float y);
        void setVec3(const std::string &name, const glm::vec3 &value);
        void setVec3(const std::string &name, float x, float y, float z);
        void setVec3Array(const std::string &name, const glm::vec3 value[], int size);
        void setVec4(const std::string &name, const glm::vec4 &value);
        void setVec4(const std::string &name, float x, float y, float z, float w);
        void setMat2(const std::string &name, const glm::mat2 &mat);
        void setMat3(const std::string &name, const glm::mat3 &mat);
        void setMat4(const std::string &name, const glm::mat4 &mat);

        using ShaderValueTypes = std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat2, glm::mat3, glm::mat4>;
        std::map<std::string, ShaderValueTypes> ShaderValues;

    private:
        std::map<ShaderDefinition, ShaderType> shaderCode;
    };

}// namespace voxie

namespace YAML {
    template<>
    struct convert<voxie::Shader> {
        static Node encode(const voxie::Shader &rhs) {
            Node node;
            return node;
        }

        static bool decode(const Node &node, voxie::Shader &rhs) {
            return true;
        }
    };
}// namespace YAML