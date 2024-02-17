//
// Created by KirilliriK on 17.02.2024.
//

#ifndef MESH_HPP
#define MESH_HPP
#include <vector>


class MeshBuilder;
class Mesh {
public:
    Mesh() = default;
    explicit Mesh(MeshBuilder& meshBuilder);
    Mesh(const std::vector<float>&buffer, const int& vertexes, const std::vector<int>& attributes);
    ~Mesh();

    void render() const;

private:
    unsigned int vao = -1;
    unsigned int vbo = -1;
    unsigned int vertices = 0;
};


#endif //MESH_HPP
