#pragma once

namespace grog
{
	static const int32_t car_vertices[] {
		-679,	52,	-9,
		-686,	52,	226,
		-514,	52,	228,
		-66,	43,	75,
		-63,	209,	76,
		-67,	46,	-109,
		-62,	207,	-109,
		-688,	52,	-244,
		-510,	52,	-252,
		-523,	52,	-18,
		-528,	52,	-2,
		133,	209,	78,
		131,	209,	-108,
		149,	314,	-69,
		146,	315,	34,
		609,	201,	-6,
		634,	46,	-159,
		-56,	169,	199,
		-58,	47,	172,
		-54,	52,	-208,
		-56,	168,	-224,
		582,	168,	-181,
		582,	171,	151,
		627,	47,	151,
		693,	202,	-146,
		688,	201,	123,
		750,	237,	-138,
		803,	317,	-138,
		803,	114,	-138,
		623,	310,	-138,
		803,	328,	108,
		804,	113,	108,
		625,	311,	108,
		810,	111,	111,
		810,	114,	-142,
		803,	250,	110,
		805,	250,	-136,
		694,	250,	-133,
		690,	250,	110,
	};

	static const uint32_t car_faces[] {
		3,	4,	0,
		5,	0,	6,
		1,	2,	0,
		4,	6,	0,
		7,	0,	8,
		8,	0,	9,
		2,	10,	0,
		11,	12,	4,
		6,	4,	12,
		14,	13,	11,
		11,	13,	12,
		16,	24,	23,
		14,	15,	13,
		17,	15,	4,
		13,	15,	12,
		17,	4,	18,
		3,	18,	4,
		6,	19,	5,
		19,	6,	20,
		23,	22,	18,
		18,	22,	17,
		20,	21,	19,
		21,	20,	15,
		6,	15,	20,
		22,	15,	17,
		15,	22,	25,
		23,	25,	22,
		14,	11,	15,
		21,	15,	24,
		25,	23,	24,
		16,	21,	24,
		24,	15,	25,
		28,	29,	27,
		31,	32,	30,
		21,	16,	19,
		29,	34,	27,
		32,	33,	30,
		36,	37,	35,
		37,	38,	35,
	};

	static const uint8_t car_colors[] {
	grog::color(grog::Color::White), // Nez
	grog::color(grog::Color::White), // Nez
	grog::color(grog::Color::Red), // Aileron avant
	grog::color(grog::Color::White), // Nez
	grog::color(grog::Color::Red), // Aileron avant
	grog::color(grog::Color::Red), // Aileron avant
	grog::color(grog::Color::Red), // Aileron avant
	grog::color(grog::Color::Gray), // Pilote
	grog::color(grog::Color::Gray), // Pilote
	grog::color(grog::Color::DarkGray), // Entree d'air
	grog::color(grog::Color::DarkGray), // Entree d'air
	grog::color(grog::Color::DarkGray), // Moteur
	grog::color(grog::Color::Red), // Derriere entree d'air
	grog::color(grog::Color::White), // Cote
	grog::color(grog::Color::Red),  // Derriere entree d'air
	grog::color(grog::Color::DarkGray), // Entree d'air laterale
	grog::color(grog::Color::DarkGray), // Entree d'air laterale
	grog::color(grog::Color::DarkGray), // Entree d'air laterale
	grog::color(grog::Color::DarkGray), // Entree d'air laterale
	grog::color(grog::Color::Red), // Cote
	grog::color(grog::Color::Red), // Cote
	grog::color(grog::Color::Red), // Cote
	grog::color(grog::Color::White), // Cote haut
	grog::color(grog::Color::White), // Cote haut
	grog::color(grog::Color::White), // Cote haut
	grog::color(grog::Color::White), // Cote haut arriere
	grog::color(grog::Color::Red), // Cote arriere
	grog::color(grog::Color::Red), // Derriere entree d'air
	grog::color(grog::Color::White), // Cote haut arriere
	grog::color(grog::Color::DarkGray), // Moteur
	grog::color(grog::Color::Red), // Cote arriere
	grog::color(grog::Color::White), // Sur moteur
	grog::color(grog::Color::Red), // Aileron
	grog::color(grog::Color::Red), // Aileron
	grog::color(grog::Color::Red), // Cote
	grog::color(grog::Color::Red), // Aileron
	grog::color(grog::Color::Red), // Aileron
	grog::color(grog::Color::White),
	grog::color(grog::Color::White),
	};

	static const uint32_t car_vertexCount {39};
	static const uint32_t car_faceCount {39};
}