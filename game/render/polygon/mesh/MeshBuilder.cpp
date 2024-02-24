//
// Created by KirilliriK on 17.02.2024.
//

#include "MeshBuilder.hpp"

#include <vector>
#include <glm/core/type.hpp>

MeshBuilder::MeshBuilder() {
    attruibutes.push_back(3); //XYZ
    attruibutes.push_back(4); //RGBA
}

void MeshBuilder::cube(const glm::vec3& pos, const glm::vec4& color, const float& size) {
    const auto leftDownBack = glm::vec3(pos.x - size, pos.y - size, pos.z - size);
    const auto leftUpBack = glm::vec3(pos.x - size, pos.y + size, pos.z - size);
    const auto leftDownFront = glm::vec3(pos.x - size, pos.y - size, pos.z + size);
    const auto leftUpFront = glm::vec3(pos.x - size, pos.y + size, pos.z + size);

    const auto rightDownBack = glm::vec3(pos.x + size, pos.y - size, pos.z - size);
    const auto rightUpBack = glm::vec3(pos.x + size, pos.y + size, pos.z - size);
    const auto rightDownFront = glm::vec3(pos.x + size, pos.y - size, pos.z + size);
    const auto rightUpFront = glm::vec3(pos.x + size, pos.y + size, pos.z + size);

    quad(leftDownBack, leftDownFront, leftUpFront, leftUpBack, color); // LeftQuad
    quad(rightDownFront, rightDownBack, rightUpBack, rightUpFront, color); // RightQuad

    quad(leftUpBack, rightUpBack, rightDownBack, leftDownBack, color); // BackQuad
    quad(leftDownFront, rightDownFront, rightUpFront, leftUpFront, color); // FrontQuad

    quad(leftDownBack, rightDownBack, rightDownFront, leftDownFront, color); // DownQuad
    quad(rightUpBack, leftUpBack, leftUpFront, rightUpFront, color); // UpQuad

    // quad(leftDownBack, leftDownFront, leftUpFront, leftUpBack, glm::vec4(1, 0, 0, 1)); // LeftQuad
    // quad(rightDownFront, rightDownBack, rightUpBack, rightUpFront, glm::vec4(1, 1, 0, 1)); // RightQuad
    //
    // quad(leftUpBack, rightUpBack, rightDownBack, leftDownBack, glm::vec4(0, 1, 0, 1)); // BackQuad
    // quad(leftDownFront, rightDownFront, rightUpFront, leftUpFront, glm::vec4(0, 1, 1, 1)); // FrontQuad
    //
    // quad(leftDownBack, rightDownBack, rightDownFront, leftDownFront, glm::vec4(0, 0, 1, 1)); // DownQuad
    // quad(rightUpBack, leftUpBack, leftUpFront, rightUpFront, glm::vec4(1, 0, 1, 1)); // UpQuad
}

void MeshBuilder::quad(
    const glm::vec3& first, const glm::vec3& second,
    const glm::vec3& third, const glm::vec3& four,
    const glm::vec4& color
) {
    vertex(first, color);
    vertex(second, color);
    vertex(third, color);

    vertex(third, color);
    vertex(four, color);
    vertex(first, color);
}

void MeshBuilder::vertex(const glm::vec3& pos, const glm::vec4& color) {
    buffer.push_back(pos.x);
    buffer.push_back(pos.y);
    buffer.push_back(pos.z);

    buffer.push_back(color.r);
    buffer.push_back(color.g);
    buffer.push_back(color.b);
    buffer.push_back(color.a);

    vertexes++;
}

const std::vector<float>& MeshBuilder::getBuffer() {
    return buffer;
}

const std::vector<int>& MeshBuilder::getAttributes() {
    return attruibutes;
}

const int& MeshBuilder::getVertexes() const {
    return vertexes;
}
