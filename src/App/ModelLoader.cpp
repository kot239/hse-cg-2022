#include "ModelLoader.h"

#include <QDebug>

bool ModelLoader::load(QString filePath) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile( filePath.toStdString(),
                                              aiProcess_GenSmoothNormals       |
                                              aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate            |
                                              aiProcess_JoinIdenticalVertices  |
                                              aiProcess_SortByPType);

    if (!scene) {
        qDebug() << "Error loading file: (assimp:) " << importer.GetErrorString();
        return false;
    }

    if (scene->HasMeshes()) {
        for (std::size_t i = 0; i < scene->mNumMeshes; i++) {
            meshes_.push_back(processMesh(scene->mMeshes[i]));
        }
    } else {
        qDebug() << "Error: No meshes found";
        return false;
    }

    if(scene->mRootNode != nullptr) {
        Node *rootNode = new Node;
        processNode(scene, scene->mRootNode, 0, *rootNode);
        rootNode_.reset(rootNode);
    } else {
        qDebug() << "Error loading model";
        return false;
    }

    return true;
}

void ModelLoader::getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<std::size_t> **indices) {
    if (vertices != nullptr)
        *vertices = &vertices_;

    if (normals != nullptr)
        *normals = &normals_;

    if (indices != nullptr)
        *indices = &indices_;
}

QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh *mesh) {
    QSharedPointer<Mesh> newMesh(new Mesh);
    newMesh->indexOffset = indices_.size();
    std::size_t indexCountBefore = indices_.size();
    int vertIndexOffset = vertices_.size() / 3;

    // get vertices
    if (mesh->mNumVertices > 0) {
        for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D &vec = mesh->mVertices[i];

            vertices_.push_back(vec.x);
            vertices_.push_back(vec.y);
            vertices_.push_back(vec.z);
        }
    }

    // get normals
    if (mesh->HasNormals()) {
        for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D &vec = mesh->mNormals[i];

            normals_.push_back(vec.x);
            normals_.push_back(vec.y);
            normals_.push_back(vec.z);
        }
    }

    // get mesh indexes
    for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace* face = &mesh->mFaces[i];
        if (face->mNumIndices != 3) {
            qDebug() << "Warning: Mesh face with not exactly 3 indices, ignoring this primitive." << face->mNumIndices;
            continue;
        }

        indices_.push_back(face->mIndices[0] + vertIndexOffset);
        indices_.push_back(face->mIndices[1] + vertIndexOffset);
        indices_.push_back(face->mIndices[2] + vertIndexOffset);
    }

    newMesh->indexCount = indices_.size() - indexCountBefore;

    return newMesh;
}

void ModelLoader::processNode(const aiScene *scene, aiNode *node, Node *parentNode, Node &newNode) {
    newNode.meshes.resize(node->mNumMeshes);

    for(std::size_t i = 0; i < node->mNumMeshes; i++) {
        QSharedPointer<Mesh> mesh = meshes_[node->mMeshes[i]];
        newNode.meshes[i] = mesh;
    }

    for(std::size_t i = 0; i < node->mNumChildren; i++) {
        newNode.nodes.push_back(Node());
        processNode(scene, node->mChildren[i], parentNode, newNode.nodes[i]);
    }
}