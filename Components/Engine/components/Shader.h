//
// Created by Stefan Annell on 2021-03-21.
//
#pragma once
#include "JsonUtil.h"
#include <glm/glm.hpp>
#include <map>
#include <string>

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

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec3Array(const std::string &name, const glm::vec3 value[], int size) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w) const;
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

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