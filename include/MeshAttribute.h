#ifndef __MeshAttribute_h
#define __MeshAttribute_h

#include "TriangleMesh.h"
#include "util/SharedObject.h"
#include "util/SoA.h"
#include "DefaultSoAAllocator.h"

namespace tcii::cg {

    using MeshIndex = typename TriangleMesh::index_t;

    template <typename... Fields>
    using ElementAttribute = SoA<DefaultSoAAllocator, MeshIndex, Fields...>;

    template <typename VA, typename TA>
    class MeshAttribute: public SharedObject {

        public:

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) {
                return new MeshAttribute(mesh);
            } 

            template<size_t I>
            auto& vertexAttribute(MeshIndex i) const {
                return _va.template get<I>(i);
            }

            template <size_t I, typename Field>
            void setVertexAttribute(MeshIndex i, Field&& field) {
                _va.template get<I>(i) = std::forward<Field>(field);
            }

            template <typename... Fields>
            void setVertexAttributes(MeshIndex i, Fields&&... fields) {
                _va.set(i, std::forward<Fields>(fields)...);
            }

            template<size_t I>
            auto& triangleAttribute(MeshIndex i) const {
                return _ta.template get<I>(i);
            }

            template <size_t I, typename Field>
            void setTriangleAttribute(MeshIndex i, Field&& field) {
                _ta.template get<I>(i) = std::forward<Field>(field);
            }

            template <typename... Fields>
            void setTriangleAttributes(MeshIndex i, Fields&&... fields) {
                _ta.set(i, std::forward<Fields>(fields)...);
            }
            
        private:

            ObjectPtr<TriangleMesh> _mesh;
            VA _va;
            TA _ta;

            MeshAttribute(const TriangleMesh& mesh): 
            _mesh{&mesh},
            _va{mesh.data().vertexCount()},
            _ta{mesh.data().triangleCount()} 
            {};

    };

}

#endif