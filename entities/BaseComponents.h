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
    Position(float x, float y, float z)
            : x(x), y(y), z(z), model(glm::mat4(1.0f)), pos({x, y, z}) {}

    Position(const Position& p)
            : Position(p.x, p.y, p.z) {}

    Position(const glm::vec3& position)
            : Position(position[0], position[1], position[2]) {}

    bool operator<(const Position& pos) const {
        return std::tie(x, y, z) < std::tie(pos.x, pos.y, pos.z);
    }

    void SetPosition(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
        pos = {x, y, z};
        this->model = glm::translate(glm::mat4(1.0f), pos);
    }

    float x;
    float y;
    float z;
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
