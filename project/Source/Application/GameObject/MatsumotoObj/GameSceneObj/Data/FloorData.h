#pragma once
#include <cstdint>



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