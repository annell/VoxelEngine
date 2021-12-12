//
// Created by Stefan Annell on 2021-03-21.
//
#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "Core.h"

namespace {
    void checkCompileErrors(GLuint shader, const std::string &type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}// namespace

namespace voxie {
    class Shader {
    public:
        using ShaderDefinition = std::string;
        using ShaderType = unsigned int;
        unsigned int ID;
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        explicit Shader(const std::map<ShaderDefinition, ShaderType> &shaders) {
            shaderCode = shaders;
            ID = glCreateProgram();
            std::vector<unsigned int> compiledShaders;
            for (const auto &pair : shaders) {
                const auto &path = pair.first;
                const unsigned int &type = pair.second;
                std::string code;
                std::ifstream shaderFile;
                shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                try {
                    shaderFile.open(path.c_str());
                    std::stringstream shaderStream;
                    shaderStream << shaderFile.rdbuf();
                    shaderFile.close();
                    code = shaderStream.str();
                } catch (std::ifstream::failure &e) {
                    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
                }
                unsigned int shader;
                shader = glCreateShader(type);
                const char *codeConst = code.c_str();
                glShaderSource(shader, 1, &codeConst, nullptr);
                glCompileShader(shader);
                compiledShaders.push_back(shader);

                checkCompileErrors(shader, "");
                glAttachShader(ID, shader);
            }
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
            for (auto shader : compiledShaders) {
                glDeleteShader(shader);
            }
        }

        void encode(YAML::Node &node) const {
            node["shaderCode"] = shaderCode;
        }

        bool decode(const YAML::Node &node) {
            return true;
        }

        // activate the shader
        // ------------------------------------------------------------------------
        void use() const {
            glUseProgram(ID);
        }
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string &name, float x, float y) const {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string &name, float x, float y, float z) const {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }
        void setVec3Array(const std::string &name, const glm::vec3 value[], int size) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), size, &(*value)[0]);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string &name, float x, float y, float z, float w) const {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        std::map<ShaderDefinition, ShaderType> shaderCode;
    };

}// namespace voxie
