//
// Created by Stefan Annell on 2021-03-21.
//
#pragma once
#include <glm/glm.hpp>
#include <map>
#include <string>

namespace YAML {
    class Node;
}

namespace voxie {
    class Shader {
    public:
        using ShaderDefinition = std::string;
        using ShaderType = unsigned int;
        unsigned int ID;
        explicit Shader(const std::map<ShaderDefinition, ShaderType> &shaders);

        void encode(YAML::Node &node) const;
        bool decode(const YAML::Node &node);

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
