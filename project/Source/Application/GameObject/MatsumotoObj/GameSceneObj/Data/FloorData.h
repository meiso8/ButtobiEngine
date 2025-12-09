#pragma once
#include <cstdint>

namespace FloorStripHp {
	extern float kNormalFloorStripHp;
	extern float kStickyFloorStripHp;
	extern float kStrongFloorStripHp;
	extern float kBombFloorStripHp;
};

namespace FloorFlag {
	extern bool isHitMultipleFloor;
};

enum class FloorType
{
	Normal,
	Sticky,
	Strong,
	Bomb
};

struct FloorData
{
	uint32_t xIndex;
	uint32_t yIndex;
	FloorType floorType;
};