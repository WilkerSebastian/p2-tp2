#include "MeshAttribute.h"
#include "TriangleMesh.h"

using namespace tcii::cg;

auto applyColors(const ObjectPtr<MeshAttribute<void, void>>& base) {

  using Color = Vec3f;
  using VA = ElementAttribute<Color>;
  using TA = ElementAttribute<Color>;
  using MA = MeshAttribute<VA, TA>;

  auto ma = MA::New(base->mesh());

  auto nv = ma->mesh().data().vertexCount();
  auto nt = ma->mesh().data().vertexCount();

  for (decltype(nt) i = 0; i < nv; ++i)
    ma->setVertexAttributes(i, Color{0, 1, 0});

  ma->setVertexAttribute<0>(0, Color{0, 1, 1});

  for (decltype(nt) i = 0; i < nt; ++i)
    ma->setTriangleAttributes(i, Color{0, 1, 0});

  ma->setTriangleAttribute<0>(0, Color{0, 1, 1});

  return ma;

}

inline auto pipeLine(const TriangleMesh& mesh) {

  auto baseAttributes = MeshAttribute<void, void>::New(mesh);

  auto stageColor = applyColors(baseAttributes);

  return stageColor;

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

  std::cout << attributes->triangleAttribute<0>(0) << '\n';

  puts("Press any key to exit...");
  (void)getchar();

  return 0;
}
