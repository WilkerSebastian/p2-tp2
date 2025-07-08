#include "MeshAttribute.h"
#include "TriangleMesh.h"

using namespace tcii::cg;

using Color = Vec3f;
using Weight = float;
using Brightness = float;
using Shadow = float;

auto applyColors(const ObjectPtr<MeshAttribute<void, void>>& base) {

  using VA = ElementAttribute<Color>;
  using TA = ElementAttribute<Color>;
  using MA = MeshAttribute<VA, TA>;

  auto ma = MA::New(base->mesh());

  auto nv = ma->mesh().data().vertexCount();
  auto nt = ma->mesh().data().triangleCount();

  for (decltype(nv) i = 0; i < nv; ++i)
    ma->setVertexAttributes(i, Color{0, 1, 0});

  ma->setVertexAttribute<0>(0, Color{0, 1, 1});

  for (decltype(nt) i = 0; i < nt; ++i)
    ma->setTriangleAttributes(i, Color{0, 1, 0});

  ma->setTriangleAttribute<0>(0, Color{0, 1, 1});

  return ma;

}

auto addVertexWeights(const ObjectPtr<MeshAttribute<ElementAttribute<Color>, ElementAttribute<Color>>>& base) {

  using VA = ElementAttribute<Color, Weight>;
  using TA = void;
  using MA = MeshAttribute<VA, TA>;

  auto ma = MA::New(base->mesh());

  auto nv = ma->mesh().data().vertexCount();

  for (decltype(nv) i = 0; i < nv; ++i) {

    const auto& vertex = ma->mesh().data().vertex(i);

    ma->setVertexAttributes(i, base->vertexAttribute<0>(i), vertex.y);

  }
    
  return ma;

}

auto addBrightness(const ObjectPtr<MeshAttribute<ElementAttribute<Color>, ElementAttribute<Color>>>& base) {

  using VA = void;
  using TA = ElementAttribute<Color, Brightness>;
  using MA = MeshAttribute<VA, TA>;

  auto ma = MA::New(base->mesh());

  auto nt = ma->mesh().data().triangleCount();

  for (decltype(nt) i = 0; i < nt; ++i)
    ma->setTriangleAttributes(i, base->triangleAttribute<0>(i), i % 2 == 0 ? 1.0f : 0.5f);

  return ma;

}

auto addShadow(const ObjectPtr<MeshAttribute<void, ElementAttribute<Color, Brightness>>>& brightness) {

  using VA = void;
  using TA = ElementAttribute<Color, Brightness, Shadow>;
  using MA = MeshAttribute<VA, TA>;

  auto ma = MA::New(brightness->mesh());

  auto nt = ma->mesh().data().triangleCount();

  for (decltype(nt) i = 0; i < nt; ++i)
    ma->setTriangleAttributes(
      i, 
      brightness->triangleAttribute<0>(i), 
      brightness->triangleAttribute<1>(i),
      1.0f - brightness->triangleAttribute<1>(i)
    );

  return ma;

}

inline auto pipeLine(const TriangleMesh& mesh) {

  auto baseAttributes = MeshAttribute<void, void>::New(mesh);

  auto stageColor = applyColors(baseAttributes);

  auto stageWeight = addVertexWeights(stageColor);

  auto stageBrightness = addBrightness(stageColor);

  auto stageShadow = addShadow(stageBrightness);

  using VA = ElementAttribute<Color, Weight>;
  using TA = ElementAttribute<Color, Brightness, Shadow>;
  using MA = MeshAttribute<VA, TA>;

  auto finalStage = MA::New(mesh); 

  auto nv = mesh.data().vertexCount();
  auto nt = mesh.data().triangleCount();

  for(decltype(nv) i = 0; i < nv; ++i)
    finalStage->setVertexAttributes(
      i,
      stageWeight->vertexAttribute<0>(i),
      stageWeight->vertexAttribute<1>(i)
    );

  for(decltype(nt) i = 0; i < nt; ++i) 
    finalStage->setTriangleAttributes(
      i,
      stageShadow->triangleAttribute<0>(i),
      stageShadow->triangleAttribute<1>(i),
      stageShadow->triangleAttribute<2>(i)
    );

  return finalStage;

}

int
main()
{

  auto filename = "meshes/f-16.obj";
  auto mesh = readOBJ(filename);

  if (!mesh)
    printf("Could not read '%s'\n", filename);
  else
    mesh->print(filename);

  auto attributes = pipeLine(*mesh);

  std::cout << std::string(30, '=') << '\n' <<
  "VERTEX ATTRIBUTES" << '\n' <<
  std::string(30, '=') << '\n';

  for (unsigned i = 0; i < 10; ++i) {
    
    std::cout << std::string(20, '-') << '\n' <<
    "Vertex Attribute " << i << '\n' << 
    "Color: " << attributes->vertexAttribute<0>(i) << '\n' <<
    "Weight: " << attributes->vertexAttribute<1>(i) << '\n';

  }

  std::cout << std::string(30, '=') << '\n' <<
  "TRIANGLE ATTRIBUTES" << '\n' <<
  std::string(30, '=') << '\n';

  for (unsigned i = 0; i < 10; ++i) {
    
    std::cout << std::string(20, '-') << '\n' <<
    "Triangle Attribute " << i << '\n' << 
    "Color: " << attributes->triangleAttribute<0>(i) << '\n' <<
    "Brightness: " << attributes->triangleAttribute<1>(i) << '\n' <<
    "Shadow: " << attributes->triangleAttribute<2>(i) << '\n';

  }

  std::cout << std::string(20, '-') << '\n';

  puts("Press any key to exit...");
  (void)getchar();

  return 0;
}
