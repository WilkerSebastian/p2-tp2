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

    template <typename T>
    struct is_element_attribute : std::false_type {};

    template <typename... Fields>
    struct is_element_attribute<SoA<DefaultSoAAllocator, MeshIndex, Fields...>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_element_attribute_v = is_element_attribute<T>::value;

    template <typename T>
    concept Defined = is_element_attribute_v<T>;

    template <typename VA, typename TA>
    class MeshAttribute;

    template <typename VA, typename TA>
        requires Defined<VA> && Defined<TA>
    class MeshAttribute<VA, TA> : public SharedObject {
        
        public:

            template<size_t I> 
            auto& vertexAttribute(MeshIndex i) const { 
                return _va.template get<I>(i); 
            }

            template<size_t I, typename Field>
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

            template<size_t I, typename Field>
            void setTriangleAttribute(MeshIndex i, Field&& field) { 
                _ta.template get<I>(i) = std::forward<Field>(field); 
            }
            
            template <typename... Fields> 
            void setTriangleAttributes(MeshIndex i, Fields&&... fields) { 
                _ta.set(i, std::forward<Fields>(fields)...); 
            }
            
            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) { 
                return new MeshAttribute(mesh); 
            }
        
        private:
            
            ObjectPtr<TriangleMesh> _mesh;
            VA _va;
            TA _ta;

            MeshAttribute(const TriangleMesh& mesh) : 
            _mesh{ &mesh }, 
            _va{ mesh.data().vertexCount() }, 
            _ta{ mesh.data().triangleCount() } 
            {}
    
    };

    template <typename VA, typename TA>
        requires std::is_void_v<VA> && Defined<TA>
    class MeshAttribute<VA, TA> : public SharedObject {
    
        public:

            template<size_t I> 
            auto& triangleAttribute(MeshIndex i) const { 
                return _ta.template get<I>(i); 
            }

            template<size_t I, typename Field>
            void setTriangleAttribute(MeshIndex i, Field&& field) { 
                _ta.template get<I>(i) = std::forward<Field>(field); 
            }

            template <typename... Fields> 
            void setTriangleAttributes(MeshIndex i, Fields&&... fields) { 
                _ta.set(i, std::forward<Fields>(fields)...); 
            }

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) { 
                return new MeshAttribute(mesh); 
            }
        
        private:

            ObjectPtr<TriangleMesh> _mesh;
            TA _ta;

            MeshAttribute(const TriangleMesh& mesh) : 
            _mesh{ &mesh }, 
            _ta{ mesh.data().triangleCount() } 
            {}
    
        };

    template <typename VA, typename TA> 
        requires Defined<VA> && std::is_void_v<TA>
    class MeshAttribute<VA, TA> : public SharedObject {
        
        public:

            template<size_t I> 
            auto& vertexAttribute(MeshIndex i) const { 
                return _va.template get<I>(i); 
            }
            
            template<size_t I, typename Field>
            void setVertexAttribute(MeshIndex i, Field&& field) { 
                _va.template get<I>(i) = std::forward<Field>(field); 
            }

            template <typename... Fields> void setVertexAttributes(MeshIndex i, Fields&&... fields) { 
                _va.set(i, std::forward<Fields>(fields)...); 
            }

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) { 
                return new MeshAttribute(mesh); 
            }

        private:
        
            ObjectPtr<TriangleMesh> _mesh;
            VA _va;

            MeshAttribute(const TriangleMesh& mesh) :
            _mesh{ &mesh }, 
            _va{ mesh.data().vertexCount() } 
            {}

    };

    template <typename VA, typename TA>
        requires std::is_void_v<VA> && std::is_void_v<TA>
    class MeshAttribute<VA, TA> : public SharedObject {

        public:

            static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh) {
                return new MeshAttribute(mesh);
            }

        private:

            ObjectPtr<TriangleMesh> _mesh;

            MeshAttribute(const TriangleMesh& mesh) :
            _mesh{ &mesh } 
            {}

    };

}

#endif