#pragma once
#include <cstdint>



enum class FloorType
{
	Normal,
	Sticky,
	Strong
};

struct FloorData
{
	uint32_t xIndex;
	uint32_t yIndex;
	FloorType floorType;
};