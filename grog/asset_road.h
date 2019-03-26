#pragma once

namespace grog
{
  static const int32_t road_vertices[] {
    -8191,	0,	-5119,
    -8191,	0,	0,
    -4095,	0,	-71,
    -4095,	0,	-5119,
    -8191,	0,	72,
    -4095,	0,	72,
    -4095,	0,	5120,
    -8191,	0,	5120,
    0,	0,	-5119,
    0,	0,	0,
    0,	0,	5120,
    4096,	0,	-5119,
    4096,	0,	0,
    0,	0,	72,
    4096,	0,	72,
    4096,	0,	5120,
    8192,	0,	5120,
    8192,	0,	0,
    8192,	0,	-5119,
  };


  static const uint32_t road_faces[] {
    2,	3,	0,
    1,	2,	0,
    1,	4,	2,
    4,	5,	2,
    6,	5,	7,
    5,	4,	7,
    8,	3,	9,
    3,	2,	9,
    6,	10,	2,
    9,	2,	10,
    11,	8,	12,
    9,	12,	8,
    12,	9,	13,
    13,	14,	12,
    14,	13,	15,
    10,	15,	13,
    15,	16,	12,
    16,	17,	12,
    12,	17,	11,
    17,	18,	11,
  };

  static const Gamebuino_Meta::ColorIndex road_colors[] {
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::white,
    Gamebuino_Meta::ColorIndex::white,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::white,
    Gamebuino_Meta::ColorIndex::white,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::gray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::darkgray,
    Gamebuino_Meta::ColorIndex::darkgray,
  };

  static const uint32_t road_vertexCount {19};
  static const uint32_t road_faceCount {20};
}
