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

    if (scene->mRootNode != nullptr) {
        Node *rootNode = new Node;
        processNode(scene, scene->mRootNode, 0, *rootNode);
        rootNode_.reset(rootNode);
    } else {
        qDebug() << "Error loading model";
        return false;
    }

    transformToUnitCoordinates();

    return true;
}

void ModelLoader::getBufferData(QVector<float> **vertices, QVector<float> **normals, QVector<unsigned int> **indices) {
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
        float max_x = -100;
        float min_x = 100;
        float max_y = -100;
        float min_y = 100;
        float max_z = -100;
        float min_z = 100;
        for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D &vec = mesh->mVertices[i];

            vertices_.push_back(vec.x);
            vertices_.push_back(vec.y);
            vertices_.push_back(vec.z);

            max_x = std::max(max_x, vec.x);
            min_x = std::min(min_x, vec.x);
            max_y = std::max(max_y, vec.y);
            min_y = std::min(min_y, vec.y);
            max_z = std::max(max_z, vec.z);
            min_z = std::min(min_z, vec.z);
        }

        qDebug() << "x:" << min_x << max_x << "y:" << min_y << max_y << "z:" << min_z << max_z;
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
    newNode.transformation = QMatrix4x4(node->mTransformation[0]);

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

void ModelLoader::transformToUnitCoordinates() {
    // This will transform the model to unit coordinates, so a model of any size or shape will fit on screen

    double amin = std::numeric_limits<double>::max();
    double amax = std::numeric_limits<double>::min();
    QVector3D minDimension(amin,amin,amin);
    QVector3D maxDimension(amax,amax,amax);

    // Get the minimum and maximum x,y,z values for the model
    findObjectDimensions(rootNode_.data(), QMatrix4x4(), minDimension, maxDimension);

    // Calculate scale and translation needed to center and fit on screen
    float dist = qMax(maxDimension.x() - minDimension.x(), qMax(maxDimension.y()-minDimension.y(), maxDimension.z() - minDimension.z()));
    float sc = 1.0 / dist;
    QVector3D center = (maxDimension - minDimension)/2;
    QVector3D trans = -(maxDimension - center);

    // Apply the scale and translation to a matrix
    QMatrix4x4 transformation;
    transformation.setToIdentity();
    transformation.scale(sc);
    transformation.translate(trans);

    // Multiply the transformation to the root node transformation matrix
    rootNode_.data()->transformation = transformation * rootNode_.data()->transformation;
}

void ModelLoader::findObjectDimensions(Node *node, QMatrix4x4 transformation, QVector3D &minDimension, QVector3D &maxDimension) {
    transformation *= node->transformation;

    for (auto & mesh : node->meshes) {
        int start = mesh->indexOffset;
        int end = start + mesh->indexCount;
        for(int i = start; i < end; i++) {
            int ind = indices_[i] * 3;
            QVector4D vec(vertices_[ind], vertices_[ind+1], vertices_[ind+2], 1.0);
            vec = transformation * vec;

            if(vec.x() < minDimension.x())
                minDimension.setX(vec.x());
            if(vec.y() < minDimension.y())
                minDimension.setY(vec.y());
            if(vec.z() < minDimension.z())
                minDimension.setZ(vec.z());
            if(vec.x() > maxDimension.x())
                maxDimension.setX(vec.x());
            if(vec.y() > maxDimension.y())
                maxDimension.setY(vec.y());
            if(vec.z() > maxDimension.z())
                maxDimension.setZ(vec.z());
        }
    }

    for (auto & child_node : node->nodes) {
        findObjectDimensions(&child_node, transformation, minDimension, maxDimension);
    }
}
