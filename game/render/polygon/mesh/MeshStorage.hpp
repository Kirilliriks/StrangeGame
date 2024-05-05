//
// Created by KirilliriK on 18.02.2024.
//

#ifndef MESHSTORAGE_HPP
#define MESHSTORAGE_HPP

#include <string>
#include <unordered_map>

class Mesh;

class MeshStorage {
public:
    static void pushMesh(const std::string& name, Mesh* mesh);
    static const Mesh& getMesh(const std::string& name);
    static void clear();
private:
    static std::unordered_map<std::string, Mesh*> storage;
};

#endif //MESHSTORAGE_HPP
