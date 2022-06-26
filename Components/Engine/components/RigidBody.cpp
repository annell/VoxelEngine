//
// Created by Stefan Annell on 2022-02-06.
//

#include "RigidBody.h"
#include "Core.h"
#include "PhysicsHandler.h"
#include "reactphysics3d/reactphysics3d.h"

namespace voxie {

    namespace internal {
        reactphysics3d::Transform PositionToTransform(const Position &pos) {
            reactphysics3d::Transform transform;
            transform.setFromOpenGL((float *) glm::value_ptr(pos.model));
            return transform;
        }

        void TransformToPosition(const reactphysics3d::Transform &transform, Position &pos) {
            glm::mat4 model;
            transform.getOpenGLMatrix(glm::value_ptr(model));
            pos.SetModel(glm::scale(model, pos.scale));
        }

        reactphysics3d::RigidBody *createRigidBody(const Position &pos) {
            return Engine::GetEngine().GetPhysicsHandler().GetWorld()->createRigidBody(PositionToTransform(pos));
        }

        reactphysics3d::BodyType voxieToReactBodyType(BodyType bodyType) {
            switch (bodyType) {
                case BodyType::STATIC:
                    return reactphysics3d::BodyType::STATIC;
                case BodyType::DYNAMIC:
                    return reactphysics3d::BodyType::DYNAMIC;
                case BodyType::KINEMATIC:
                    return reactphysics3d::BodyType::KINEMATIC;
            }
            return reactphysics3d::BodyType::STATIC;
        }

        BodyType reactToVoxieBodyType(reactphysics3d::BodyType bodyType) {
            switch (bodyType) {
                case reactphysics3d::BodyType::STATIC:
                    return BodyType::STATIC;
                case reactphysics3d::BodyType::DYNAMIC:
                    return BodyType::DYNAMIC;
                case reactphysics3d::BodyType::KINEMATIC:
                    return BodyType::KINEMATIC;
            }
            return BodyType::STATIC;
        }
    }// namespace internal

    reactphysics3d::Collider *CreateBoxCollider(reactphysics3d::RigidBody *body, const Position &pos) {
        auto *shape = getPhysicsCommon().createBoxShape({(pos.scale.x) / 2, (pos.scale.y) / 2, (pos.scale.z) / 2});
        return body->addCollider(shape, reactphysics3d::Transform::identity());
    }

    reactphysics3d::Collider *CreateConvexCollider(reactphysics3d::RigidBody *body, const std::vector<float> &vertices, const Position &pos) {
        int numberOfFacesPerSide = 2;
        int numberOfVerticesPerFace = 3;
        int numberOfVerticesPerSide = numberOfFacesPerSide * numberOfVerticesPerFace;
        int verticeStride = 3;
        int faceSize = numberOfVerticesPerSide * verticeStride;
        int numberOfVertices = vertices.size() / verticeStride;
        int numberOfFaces = numberOfVertices / numberOfVerticesPerSide;
        std::vector<int> indices;
        for (int i = 0; i < vertices.size(); i += faceSize) {
            for (int j = 0; j < faceSize; j++) {
                indices.push_back(j);
            }
        }

        reactphysics3d::PolygonVertexArray::PolygonFace *polygonFaces = new reactphysics3d::PolygonVertexArray::PolygonFace[numberOfFaces];
        for (int f = 0; f < numberOfFaces; f++) {
            auto &face = polygonFaces[f];
            // First vertex of the face in the indices array
            face.indexBase = f * numberOfVerticesPerSide;
            // Number of vertices in the face
            face.nbVertices = numberOfVerticesPerSide;
        }


        // Create the polygon vertex array
        reactphysics3d::PolygonVertexArray *polygonVertexArray = new reactphysics3d::PolygonVertexArray(numberOfVerticesPerFace, vertices.data(), 2 * sizeof(float),
                                                                                                        indices.data(), sizeof(int), numberOfFaces, polygonFaces,
                                                                                                        reactphysics3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                                                                                                        reactphysics3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE);

        // Create the polyhedron mesh
        reactphysics3d::PolyhedronMesh *polyhedronMesh = getPhysicsCommon().createPolyhedronMesh(polygonVertexArray);

        // Create the convex mesh collision shape
        reactphysics3d::ConvexMeshShape *shape = getPhysicsCommon().createConvexMeshShape(polyhedronMesh, {pos.scale.x / 2, pos.scale.y / 2, pos.scale.z / 2});
        return body->addCollider(shape, reactphysics3d::Transform::identity());
    }

    RigidBody::RigidBody(const Position &pos)
        : rigidBody(internal::createRigidBody(pos)), collider(nullptr) {
    }

    RigidBody::~RigidBody() {
        Engine::GetEngine().GetPhysicsHandler().GetWorld()->destroyRigidBody(rigidBody);
    }

    void RigidBody::BeginPlay() const {
        rigidBody->setType(internal::voxieToReactBodyType(bodyType));
    }

    void RigidBody::encode(YAML::Node &node) const {
        node["mass"] = GetMass();
        node["gravity"] = GetGravity();
        node["bodyType"] = static_cast<int>(GetBodyType());

        YAML::Node offsetNode;
        offsetNode["x"] = offset[0];
        offsetNode["y"] = offset[1];
        offsetNode["z"] = offset[2];
        node["offset"] = offsetNode;

        YAML::Node linearLockNode;
        auto linearLockFactor = rigidBody->getLinearLockAxisFactor();
        linearLockNode["x"] = linearLockFactor[0];
        linearLockNode["y"] = linearLockFactor[1];
        linearLockNode["z"] = linearLockFactor[2];
        node["linearLockFactor"] = linearLockNode;

        YAML::Node angularLockNode;
        auto angularLockFactor = rigidBody->getAngularLockAxisFactor();
        angularLockNode["x"] = angularLockFactor[0];
        angularLockNode["y"] = angularLockFactor[1];
        angularLockNode["z"] = angularLockFactor[2];
        node["angularLockFactor"] = angularLockNode;
    }

    bool RigidBody::decode(const YAML::Node &node) {
        auto offsetNode = node["offset"];
        SetOffset({offsetNode["x"].as<float>(),
                   offsetNode["y"].as<float>(),
                   offsetNode["z"].as<float>()});
        SetMass(node["mass"].as<float>());
        SetGravity(node["gravity"].as<bool>());
        SetBodyType(static_cast<voxie::BodyType>(node["bodyType"].as<int>()));
        rigidBody->setType(internal::voxieToReactBodyType(BodyType::STATIC));

        auto linearLock = node["linearLockFactor"];
        SetLinearAxisFactor({linearLock["x"].as<float>(),
                             linearLock["y"].as<float>(),
                             linearLock["z"].as<float>()});

        auto angularLock = node["angularLockFactor"];
        SetAngularAxisFactor({angularLock["x"].as<float>(),
                              angularLock["y"].as<float>(),
                              angularLock["z"].as<float>()});
        return true;
    }

    void RigidBody::SetPosition(const Position &pos) const {
        assert(rigidBody && collider);
        rigidBody->setTransform(internal::PositionToTransform(GetPositionWithOffset(pos)));
        dynamic_cast<reactphysics3d::BoxShape *>(collider->getCollisionShape())->setHalfExtents({(chunkAxises.x.diff() * pos.scale.x) / 2, (chunkAxises.y.diff() * pos.scale.y) / 2, (chunkAxises.z.diff() * pos.scale.z) / 2});
    }

    Position RigidBody::GetPositionWithOffset(const Position &pos) const {
        auto modifiedpos = pos;
        modifiedpos.pos += GetOffset();
        modifiedpos.UpdateModel();
        return modifiedpos;
    }

    void RigidBody::SetOffset(const glm::vec3 &offset) {
        this->offset = offset;
    }

    glm::vec3 RigidBody::GetOffset() const {
        return offset;
    }

    glm::vec3 RigidBody::GetLinearAxisFactor() const {
        auto factor = rigidBody->getLinearLockAxisFactor();
        return {factor.x, factor.y, factor.z};
    }
    void RigidBody::SetLinearAxisFactor(const glm::vec3 &factor) const {
        rigidBody->setLinearLockAxisFactor({factor.x, factor.y, factor.z});
    }

    glm::vec3 RigidBody::GetAngularAxisFactor() const {
        auto factor = rigidBody->getAngularLockAxisFactor();
        return {factor.x, factor.y, factor.z};
    }
    void RigidBody::SetAngularAxisFactor(const glm::vec3 &factor) const {
        rigidBody->setAngularLockAxisFactor({factor.x, factor.y, factor.z});
    }

    void RigidBody::GetPosition(Position &pos) const {
        assert(rigidBody);
        internal::TransformToPosition(rigidBody->getTransform(), pos);
        pos.pos -= GetOffset();
        pos.UpdateModel();
    }

    BodyType RigidBody::GetBodyType() const {
        return bodyType;
    }
    void RigidBody::SetBodyType(BodyType type) {
        bodyType = type;
    }
    void RigidBody::SetMass(float mass) const {
        assert(rigidBody);
        rigidBody->setMass(mass);
    }
    float RigidBody::GetMass() const {
        assert(rigidBody);
        return rigidBody->getMass();
    }

    bool RigidBody::GetGravity() const {
        assert(rigidBody);
        return rigidBody->isGravityEnabled();
    }

    void RigidBody::SetGravity(bool gravity) const {
        assert(rigidBody);
        rigidBody->enableGravity(gravity);
    }

    uint32_t RigidBody::GetColliderId() const {
        return rigidBody->getEntity().getIndex();
    }

}// namespace voxie
