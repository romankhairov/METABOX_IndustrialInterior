#include "PayloadsBase.h"

// ----------------------- VARIABLES JSON --------------------------------
#pragma region VariablesJsonRegion

FIntPoint2DJson::FIntPoint2DJson()
{
}

FIntPoint2DJson::FIntPoint2DJson(const FIntPoint& IntPoint) : x(IntPoint.X), y(IntPoint.Y)
{
}

FIntPoint2DJson::FIntPoint2DJson(int32 x, int32 y)	: x(x), y(y)
{
}

FRotatorJson::FRotatorJson()
{
	
}

FRotatorJson::FRotatorJson(const FRotator& Rotator)
	: Pitch(Rotator.Pitch), Yaw(Rotator.Yaw), Roll(Rotator.Roll)
{
	
}

FRotatorJson::FRotatorJson(FRotator&& Rotator)
	: Pitch(Rotator.Pitch), Yaw(Rotator.Yaw), Roll(Rotator.Roll)
{
	
}

FVectorJson::FVectorJson()
{
}

FVectorJson::FVectorJson(const FVector& Vec) : x(Vec.X), y(Vec.Y), z(Vec.Z)
{
}

FVectorJson::FVectorJson(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

FTransformJson::FTransformJson()
{
}

FTransformJson::FTransformJson(const FTransform& Transform)
	: rotation(Transform.Rotator()), translation(Transform.GetTranslation()), Scale3D(Transform.GetScale3D())
{
	
}

FTransformJson::FTransformJson(FTransform&& Transform)
: rotation(Transform.Rotator()), translation(Transform.GetTranslation()), Scale3D(Transform.GetScale3D())
{
	
}

FLinearColorJson::FLinearColorJson()
{
}

FLinearColorJson::FLinearColorJson(const FLinearColor& Color) : r(Color.R), g(Color.G), b(Color.B), a(Color.A)
{
}

FLinearColorJson::FLinearColorJson(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

#pragma endregion

// ----------------------- BASE PAYLOADS --------------------------------
#pragma region BasePayloadsRegion

FPayloadBase::FPayloadBase()
{
	switch (FEngineVersion::Current().GetMajor())
	{
	case 4: unrealVersion = "V_4"; break;
	case 5: unrealVersion = "V_5"; break;
	default: break;
	}
}

FBoolPayload::FBoolPayload()
{
}

FFloatPayload::FFloatPayload()
{
}

FFVectorPayload::FFVectorPayload()
{
}

FFStringPayload::FFStringPayload()
{
}

#pragma endregion

// ----------------------- ASSETS --------------------------------
#pragma region AssetsRegion

FProductPayload::FProductPayload()
{
	type = EAssetType::product;
}

FLevelPayload::FLevelPayload()
{
	type = EAssetType::environment;
}

FMaterialBasePayload::FMaterialBasePayload()
{
	type = EAssetType::material;
}

#pragma endregion

// ----------------------- CONTROLS CAMERA --------------------------------
#pragma region ControlsCameraRegion

FCameraSettingsPayload::FCameraSettingsPayload()
{
	type = EAssetType::camera;
}

FCameraPresetPayload::FCameraPresetPayload()
{
	type = EAssetType::camera;
}

#pragma endregion

FTempCommandPayload::FTempCommandPayload()
{
}
