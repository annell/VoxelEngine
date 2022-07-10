#include "Shader.h"

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>

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

    unsigned int GetGLShaderFromShaderType(voxie::ShaderType type) {
        switch (type) {
            case voxie::ShaderType::VERTEX:
                return GL_VERTEX_SHADER;
            case voxie::ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;
        }
        return 0;
    }
}// namespace

namespace voxie {
    Shader::Shader(const std::map<ShaderDefinition, ShaderType> &shaders) {
        shaderCode = shaders;
        ID = glCreateProgram();
        std::vector<unsigned int> compiledShaders;
        for (const auto &pair : shaders) {
            const auto &path = pair.first;
            const unsigned int &type = GetGLShaderFromShaderType(pair.second);
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

    void Shader::encode(YAML::Node &node) const {
    }

    bool Shader::decode(const YAML::Node &node) {
        return true;
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void Shader::use() const {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void Shader::setVec3Array(const std::string &name, const glm::vec3 value[], int size) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), size, &(*value)[0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}// namespace voxie
