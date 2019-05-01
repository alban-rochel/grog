#pragma once

namespace demo
{
	/**
	 * @brief Vertices of a tree, as (x, y, z), in Q21.10 fixed point.
	 * Stored in Flash memory.
	 */
  static const int32_t tree1_vertices[] {
    0,	0,	-311,
    312,	0,	0,
    0,	0,	312,
    -311,	0,	0,
    0,	2072,	0,
  };

	/**
	 * @brief Vertices of another tree, as (x, y, z), in Q21.10 fixed point.
	 * Stored in Flash memory.
	 */
  static const int32_t tree2_vertices[] {
    141,	1,	-278,
    279,	1,	141,
    -140,	1,	279,
    -278,	1,	-140,
    0,	1545,	0,
  };

	/**
	 * @brief Faces (triangles) of a tree (any one actually), as (vertex_index_1, vertex_index_2, vertex_index_3).
	 * Stored in Flash memory.
	 */
  static const uint32_t tree_faces[] {
    0,	4,	1,
    1,	4,	2,
    2,	4,	3,
    3,	4,	0,
  };

	/**
	 * @brief Colors for each face of a tree.
	 * Stored in Flash memory.
	 */
  static const Gamebuino_Meta::ColorIndex tree_colors[] {
  Gamebuino_Meta::ColorIndex::green,
  Gamebuino_Meta::ColorIndex::lightgreen,
  Gamebuino_Meta::ColorIndex::green,
  Gamebuino_Meta::ColorIndex::lightgreen,
  };

  static const uint32_t tree_vertexCount {5}; ///< Number of vertices for a tree.
  static const uint32_t tree_faceCount {4}; 	///< Number of faces for a tree.
}
