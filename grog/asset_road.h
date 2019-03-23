#pragma once

namespace grog
{
  static const int32_t road_vertices[] {
    -8191,	0,	-5119,
    -7167,	0,	0,
    -3071,	0,	0,
    -4095,	0,	-4095,
    -7167,	0,	0,
    -3071,	0,	0,
    -3071,	0,	5120,
    -7167,	0,	5120,
    0,	0,	-4095,
    0,	0,	0,
    0,	0,	5120,
    3072,	0,	-4095,
    3072,	0,	0,
    0,	0,	0,
    3072,	0,	0,
    3072,	0,	5120,
    8192,	0,	5120,
    7168,	0,	0,
    8192,	0,	-4095,
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

  static const uint8_t road_colors[] {
    grog::color(grog::Color::White),
    grog::color(grog::Color::Gray),
    grog::color(grog::Color::DarkGray),
    grog::color(grog::Color::Purple),
    grog::color(grog::Color::Magenta),
    grog::color(grog::Color::Red),
    grog::color(grog::Color::Orange),
    grog::color(grog::Color::Brown),
    grog::color(grog::Color::Beige),
    grog::color(grog::Color::Yellow),
    grog::color(grog::Color::LightGreen),
    grog::color(grog::Color::Green),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
    grog::color(grog::Color::White),
  };

  static const uint32_t road_vertexCount {19};
  static const uint32_t road_faceCount {20};
}
