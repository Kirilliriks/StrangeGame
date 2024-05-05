//
// Created by KirilliriK on 18.02.2024.
//

#include "MeshStorage.hpp"

#include <ranges>

#include "Mesh.hpp"

std::unordered_map<std::string, Mesh*> MeshStorage::storage;

void MeshStorage::pushMesh(const std::string& name, Mesh* mesh) {
    if (storage.contains(name)) {
        delete storage[name];
        storage.erase(name);
    }

    storage[name] = mesh;
}

const Mesh& MeshStorage::getMesh(const std::string& name) {
    return *storage[name];
}

void MeshStorage::clear() {
    for (const auto& mesh : std::ranges::views::values(storage)) {
        delete mesh;
    }
}
