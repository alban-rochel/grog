#pragma once

#include <cstdint>

namespace Gamebuino_Meta
{
	enum class Color : uint16_t
	{
		white		= 0xFFFF,
		gray		= 0xACD0,
		darkgray	= 0x5268,
		black		= 0x0000,
		purple		= 0x9008,
		pink		= 0xCA30,
		red			= 0xD8E4,
		orange		= 0xFD42,
		brown		= 0xCC68,
		beige		= 0xFEB2,
		yellow		= 0xF720,
		lightgreen	= 0x8668,
		green		= 0x044A,
		darkblue	= 0x0210,
		blue		= 0x4439,
		lightblue	= 0x7DDF,
	};

	enum class ColorIndex : uint8_t
	{
		black,
		darkblue,
		purple,
		green,
		brown,
		darkgray,
		gray,
		white,
		red,
		orange,
		yellow,
		lightgreen,
		lightblue,
		blue,
		pink,
		beige,
	};
}
