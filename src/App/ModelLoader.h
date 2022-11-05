#pragma once

#include <QMatrix4x4>
#include <vector>
#include <QFile>
#include <QSharedPointer>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

struct Mesh {
    std::size_t indexCount;
    std::size_t indexOffset;
};

struct Node {
    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};

class ModelLoader {
public:
    ModelLoader() = default;;

    bool load(QString filePath);
    void getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<std::size_t> **indices);
    QSharedPointer<Node> getNode() { return rootNode_; };

private:
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
    void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);

    QVector<float> vertices_;
    QVector<float> normals_;
    QVector<std::size_t> indices_;

    QVector<QSharedPointer<Mesh> > meshes_;
    QSharedPointer<Node> rootNode_;
};
