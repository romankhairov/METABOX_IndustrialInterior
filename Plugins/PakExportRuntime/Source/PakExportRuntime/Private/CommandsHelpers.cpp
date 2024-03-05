#include "CommandsHelpers.h"

FVectorJson::FVectorJson()
{
}

FVectorJson::FVectorJson(const FVector Vec) : x(Vec.X), y(Vec.Y), z(Vec.Z)
{
}

FVectorJson::FVectorJson(float x, float y, float z)
	: x(x), y(y), z(z)
{
}
