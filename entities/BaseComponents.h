//
// Created by Stefan Annell on 2021-05-17.
//

#pragma once

namespace voxie {

struct Material {
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Color {
    Color(glm::vec3 color) : color(color) { }
    glm::vec3 color;

    void SetColor(float r, float g, float b) {
        color = {r, g, b};
    }
};

struct Position {
    Position(const glm::vec3& position) {
        SetPosition(position);
    }

    Position(float x, float y, float z)
        : Position(glm::vec3{x, y, z}) {}

    Position(const Position& p)
        : Position(p.pos) {}

    bool operator<(const Position& pos) const {
        return std::tie(pos.pos[0], pos.pos[1], pos.pos[2]) < std::tie(pos.pos[0], pos.pos[1], pos.pos[2]);
    }

    void SetPosition(float x, float y, float z) {
        SetPosition({x, y, z});
    }

    void SetPosition(const glm::vec3& pos) {
        this->pos = pos;
        this->model = glm::translate(glm::mat4(1.0f), pos);
    }

    glm::mat4 model;
    glm::vec3 pos;
};

struct ChunkPosition {
    int x;
    int y;
    int z;

    bool operator<(const ChunkPosition& pos) const {
        return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
    }
};

struct Dimensions {
    float width;
    float height;
    float depth;
};

}
