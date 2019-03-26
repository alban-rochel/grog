#pragma once

namespace grog
{
  static const int32_t tree1_vertices[] {
    0,	0,	-311,
    312,	0,	0,
    0,	0,	312,
    -311,	0,	0,
    0,	2072,	0,
  };

  static const int32_t tree2_vertices[] {
    141,	1,	-278,
    279,	1,	141,
    -140,	1,	279,
    -278,	1,	-140,
    0,	1545,	0,
  };

  static const uint32_t tree_faces[] {
    0,	4,	1,
    1,	4,	2,
    2,	4,	3,
    3,	4,	0,
  };

  static const Gamebuino_Meta::ColorIndex tree_colors[] {
  Gamebuino_Meta::ColorIndex::green,
  Gamebuino_Meta::ColorIndex::lightgreen,
  Gamebuino_Meta::ColorIndex::green,
  Gamebuino_Meta::ColorIndex::lightgreen,
  };

  static const uint32_t tree_vertexCount {5};
  static const uint32_t tree_faceCount {4};
}
