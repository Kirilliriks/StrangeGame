//
// Created by KirilliriK on 18.02.2024.
//

#include "MeshStorage.hpp"

#include "Mesh.hpp"

std::unordered_map<std::string, Mesh*> MeshStorage::storage;

void MeshStorage::pushMesh(const std::string& name, Mesh* mesh) {
    storage[name] = mesh;
}

const Mesh& MeshStorage::getMesh(const std::string& name) {
    return *storage[name];
}

void MeshStorage::clear() {
    for (const auto& [name, mesh] : storage) {
        delete mesh;
    }
}


