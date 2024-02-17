//
// Created by KirilliriK on 17.02.2024.
//

#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP
#include <vector>
#include "glm/glm.hpp"


class MeshBuilder {
public:
    static constexpr int VERTEX_SIZE = 3 + 3; // XYZ + RGB

    MeshBuilder();
    ~MeshBuilder() = default;

    void cube(const glm::vec3& pos, const glm::vec3& color, float size);
    void quad(
        const glm::vec3& first,
        const glm::vec3& second,
        const glm::vec3& third,
        const glm::vec3& four,
        const glm::vec3& color
    );
    void vertex(const glm::vec3&pos, const glm::vec3&color);

    const std::vector<float>& getBuffer();
    const std::vector<int>& getAttributes();
    const int& getVertexes() const;
private:
    std::vector<float> buffer;
    std::vector<int> attruibutes;
    int vertexes = 0;
};


#endif //MESHBUILDER_HPP
