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
    QMatrix4x4 transformation;
    QVector<QSharedPointer<Mesh> > meshes;
    QVector<Node> nodes;
};

class ModelLoader {
public:
    ModelLoader() = default;;

    bool load(QString filePath);
    void getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices);
    QSharedPointer<Node> getNode() { return rootNode_; };

private:
    QSharedPointer<Mesh> processMesh(aiMesh *mesh);
    void processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode);

    void transformToUnitCoordinates();
    void findObjectDimensions(Node *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension);

    QVector<float> vertices_;
    QVector<float> normals_;
    QVector<unsigned int> indices_;

    QVector<QSharedPointer<Mesh> > meshes_;
    QSharedPointer<Node> rootNode_;
};
