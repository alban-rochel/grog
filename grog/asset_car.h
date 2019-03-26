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

	static const Gamebuino_Meta::ColorIndex car_colors_main[] {
	Gamebuino_Meta::ColorIndex::white, // Nez
	Gamebuino_Meta::ColorIndex::white, // Nez
	Gamebuino_Meta::ColorIndex::red, // Aileron avant
	Gamebuino_Meta::ColorIndex::white, // Nez
	Gamebuino_Meta::ColorIndex::red, // Aileron avant
	Gamebuino_Meta::ColorIndex::red, // Aileron avant
	Gamebuino_Meta::ColorIndex::red, // Aileron avant
	Gamebuino_Meta::ColorIndex::gray, // Pilote
	Gamebuino_Meta::ColorIndex::gray, // Pilote
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air
	Gamebuino_Meta::ColorIndex::darkgray, // Moteur
	Gamebuino_Meta::ColorIndex::red, // Derriere entree d'air
	Gamebuino_Meta::ColorIndex::white, // Cote
	Gamebuino_Meta::ColorIndex::red,  // Derriere entree d'air
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::red, // Cote
	Gamebuino_Meta::ColorIndex::red, // Cote
	Gamebuino_Meta::ColorIndex::red, // Cote
	Gamebuino_Meta::ColorIndex::white, // Cote haut
	Gamebuino_Meta::ColorIndex::white, // Cote haut
	Gamebuino_Meta::ColorIndex::white, // Cote haut
	Gamebuino_Meta::ColorIndex::white, // Cote haut arriere
	Gamebuino_Meta::ColorIndex::red, // Cote arriere
	Gamebuino_Meta::ColorIndex::red, // Derriere entree d'air
	Gamebuino_Meta::ColorIndex::white, // Cote haut arriere
	Gamebuino_Meta::ColorIndex::darkgray, // Moteur
	Gamebuino_Meta::ColorIndex::red, // Cote arriere
	Gamebuino_Meta::ColorIndex::white, // Sur moteur
	Gamebuino_Meta::ColorIndex::red, // Aileron
	Gamebuino_Meta::ColorIndex::red, // Aileron
	Gamebuino_Meta::ColorIndex::red, // Cote
	Gamebuino_Meta::ColorIndex::red, // Aileron
	Gamebuino_Meta::ColorIndex::red, // Aileron
	Gamebuino_Meta::ColorIndex::white,
	Gamebuino_Meta::ColorIndex::white,
	};

	static const Gamebuino_Meta::ColorIndex car_colors_other[] {
	Gamebuino_Meta::ColorIndex::white, // Nez
	Gamebuino_Meta::ColorIndex::white, // Nez
	Gamebuino_Meta::ColorIndex::blue, // Aileron avant
	Gamebuino_Meta::ColorIndex::white, // Nez
	Gamebuino_Meta::ColorIndex::blue, // Aileron avant
	Gamebuino_Meta::ColorIndex::blue, // Aileron avant
	Gamebuino_Meta::ColorIndex::blue, // Aileron avant
	Gamebuino_Meta::ColorIndex::gray, // Pilote
	Gamebuino_Meta::ColorIndex::gray, // Pilote
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air
	Gamebuino_Meta::ColorIndex::darkgray, // Moteur
	Gamebuino_Meta::ColorIndex::blue, // Derriere entree d'air
	Gamebuino_Meta::ColorIndex::white, // Cote
	Gamebuino_Meta::ColorIndex::blue,  // Derriere entree d'air
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::darkgray, // Entree d'air laterale
	Gamebuino_Meta::ColorIndex::blue, // Cote
	Gamebuino_Meta::ColorIndex::blue, // Cote
	Gamebuino_Meta::ColorIndex::blue, // Cote
	Gamebuino_Meta::ColorIndex::white, // Cote haut
	Gamebuino_Meta::ColorIndex::white, // Cote haut
	Gamebuino_Meta::ColorIndex::white, // Cote haut
	Gamebuino_Meta::ColorIndex::white, // Cote haut arriere
	Gamebuino_Meta::ColorIndex::blue, // Cote arriere
	Gamebuino_Meta::ColorIndex::blue, // Derriere entree d'air
	Gamebuino_Meta::ColorIndex::white, // Cote haut arriere
	Gamebuino_Meta::ColorIndex::darkgray, // Moteur
	Gamebuino_Meta::ColorIndex::blue, // Cote arriere
	Gamebuino_Meta::ColorIndex::white, // Sur moteur
	Gamebuino_Meta::ColorIndex::blue, // Aileron
	Gamebuino_Meta::ColorIndex::blue, // Aileron
	Gamebuino_Meta::ColorIndex::blue, // Cote
	Gamebuino_Meta::ColorIndex::blue, // Aileron
	Gamebuino_Meta::ColorIndex::blue, // Aileron
	Gamebuino_Meta::ColorIndex::white,
	Gamebuino_Meta::ColorIndex::white,
	};

	static const uint32_t car_vertexCount {39};
	static const uint32_t car_faceCount {39};

	static const int32_t wheel_vertices[] {
		2,	-89,	58,
		2,	-89,	-56,
		80,	-44,	58,
		80,	-44,	-56,
		80,	44,	58,
		80,	44,	-56,
		2,	89,	58,
		2,	89,	-56,
		-75,	44,	58,
		-75,	44,	-56,
		-75,	-44,	58,
		-75,	-44,	-56,
	};

	static const uint32_t wheel_faces[] {
		1,	2,	0,
		3,	4,	2,
		5,	6,	4,
		7,	8,	6,
		7,	5,	1,
		9,	10,	8,
		11,	0,	10,
		6,	8,	10,
		1,	3,	2,
		3,	5,	4,
		5,	7,	6,
		7,	9,	8,
		5,	3,	1,
		1,	11,	9,
		9,	7,	1,
		9,	11,	10,
		11,	1,	0,
		10,	0,	6,
		0,	2,	6,
		2,	4,	6,
	};

	static const Gamebuino_Meta::ColorIndex wheel_colors[] {
	Gamebuino_Meta::ColorIndex::gray, // Bande 1
	Gamebuino_Meta::ColorIndex::darkgray, // Bande 2
	Gamebuino_Meta::ColorIndex::gray,    // Bande 3
	Gamebuino_Meta::ColorIndex::darkgray, // Bande 4
	Gamebuino_Meta::ColorIndex::black, // Cote 1
	Gamebuino_Meta::ColorIndex::gray, // Bande 5
	Gamebuino_Meta::ColorIndex::darkgray, // Bande 6
	Gamebuino_Meta::ColorIndex::black, // Cote 2
	Gamebuino_Meta::ColorIndex::gray, // Bande 1
	Gamebuino_Meta::ColorIndex::darkgray, // Bande 2
	Gamebuino_Meta::ColorIndex::gray,    // Bande 3
	Gamebuino_Meta::ColorIndex::darkgray, // Bande 4
	Gamebuino_Meta::ColorIndex::black, // Cote 1
	Gamebuino_Meta::ColorIndex::black,   // Cote 1
	Gamebuino_Meta::ColorIndex::black, // Cote 1
	Gamebuino_Meta::ColorIndex::gray, // Bande 5
	Gamebuino_Meta::ColorIndex::darkgray, // Bande 6
	Gamebuino_Meta::ColorIndex::black, // Cote 2
	Gamebuino_Meta::ColorIndex::black,// Cote 2
	Gamebuino_Meta::ColorIndex::black,// Cote 2
	};

	static const uint32_t wheel_vertexCount {12};
	static const uint32_t wheel_faceCount {20};
}
