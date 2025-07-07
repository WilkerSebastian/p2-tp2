#include "MeshAttribute.h"
#include "TriangleMesh.h"

using namespace tcii::cg;

auto decorate(const TriangleMesh& mesh) {

  using Color = Vec3f;
  using VA = ElementAttribute<Color>;
  using TA = ElementAttribute<Color>;
  using MA = MeshAttribute<VA, TA>;

  auto nv = mesh.data().vertexCount();
  auto nt = mesh.data().vertexCount();
  auto ma = MA::New(mesh);

  for (decltype(nv) i = 0; i < nv; ++i)
    ma->setVertexAttributes(i, Color{0, 0, 1});

  ma->setVertexAttribute<0>(0, Color{1, 1, 0});

  for (decltype(nt) i = 0; i < nt; ++i)
    ma->setTriangleAttributes(i, Color{0, 1, 0});

  ma->setTriangleAttribute<0>(0, Color{0, 1, 1});

  return ma;

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

  auto attributes = decorate(*mesh);

  std::cout << attributes->vertexAttribute<0>(0) << '\n';
  std::cout << attributes->triangleAttribute<0>(0) << '\n';

  puts("Press any key to exit...");
  (void)getchar();

  return 0;
}
