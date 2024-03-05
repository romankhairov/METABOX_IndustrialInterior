#pragma once

#include "CoreMinimal.h"
#include "CommandsHelpers.generated.h"

///A vector in 2-D space composed of components (X, Y) with floating point precision.
USTRUCT()
struct FVector2DJson
{
	GENERATED_BODY()
public:
	///Vector's X component.
	UPROPERTY() float x = 0.0f;

	///Vector's Y component.
	UPROPERTY() float y = 0.0f;
};

///Implements a container for rotation information.
///All rotation values are stored in degrees.
USTRUCT()
struct FRotatorJson
{
	GENERATED_BODY()
public:
	///Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down)
	UPROPERTY() float Pitch = 0.0f;

	///Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South.
	UPROPERTY() float Yaw = 0.0f;

	///Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW.
	UPROPERTY() float Roll = 0.0f;
};

///A vector in 3-D space composed of components (X, Y, Z) with floating point precision.
USTRUCT()
struct FVectorJson
{
	GENERATED_BODY()
public:
	FVectorJson();
	FVectorJson(const FVector Vec);
	FVectorJson(float x, float y, float z);
	
	///Vector's X component.
	UPROPERTY() float x = 0.0f;

	///Vector's Y component.
	UPROPERTY() float y = 0.0f;

	///Vector's Z component.
	UPROPERTY() float z = 0.0f;
};

///Transform composed of Scale, Rotation (as a rotator), and Translation.
USTRUCT()
struct FTransformJson
{
	GENERATED_BODY()
public:
	///Rotation of this transformation, as a rotator
	UPROPERTY() FRotatorJson rotation;
	
	///Translation of this transformation, as a vector
	UPROPERTY() FVectorJson translation;

	///3D scale (always applied in local space) as a vector
	UPROPERTY() FVectorJson Scale3D = FVectorJson(1.0f, 1.0f, 1.0f);
};

//Scalar material parameter
USTRUCT()
struct FScalarJson
{
	GENERATED_BODY()
public:
	///scalar parameter name in material
	UPROPERTY() FString parameterName = "print_switch";

	///value of the scalar material parameter
	UPROPERTY(meta = (ClampMin = "0.0", ClampMax = "1.0")) float value = 1.0f;
};

///Material texture
USTRUCT()
struct FTextureJson
{
	GENERATED_BODY()
public:
	///parameter for texture in material
	UPROPERTY() FString parameterName = "print";

	///url to download texture from
	UPROPERTY() FString url = "https://i.ibb.co/tsjtsDf/Pngtree-duck-202201.png";

	///enable canvas for texture or no
	UPROPERTY() bool canvas = true;

	///size of the texture
	UPROPERTY() FVector2D size = {1024, 1024};
	
	///position of the texture in relative space
	UPROPERTY() FVector2D position = {0.5, 0.5};

	///rotate angle in degrees
	UPROPERTY() float Rotate = 0;

	///pivot of the texture in relative space
	UPROPERTY() FVector2D pivot = {0.5, 0.5};
	
	///grid position
	UPROPERTY() FVector2D coordinatePosition = {0, 0};

	///grid size
	UPROPERTY() FVector2D coordinateSize = {1, 1};
};


///Material
USTRUCT()
struct FMaterialJson
{
	GENERATED_BODY()
public:
	///Name of the asset root folder(almost always like this)
	UPROPERTY() FString pak = "PakE";

	///Name of the asset relative to Content folder in project
	UPROPERTY() FString name = "mat/paper/back";

	///Material name applying material to
	UPROPERTY() FString slot_name = "back";

	///vector parameter color applying to
	UPROPERTY() FString colorParameterName = "color_picker";
	
	///color in hex format like FF0000 of #FF0000
	UPROPERTY() FString colorHex = "colorHex";

	///List of textures in the material
	UPROPERTY() TArray<FTextureJson> textures;

	///List of scalars in the material
	UPROPERTY() TArray<FScalarJson> scalars;
};

///Mesh initial state
USTRUCT()
struct FInitialStateJson
{
	GENERATED_BODY()
public:
	///Name of the asset relative to Content folder in project
	UPROPERTY() FString meshName = "Aromat_red";

	///Product spawn transform
	UPROPERTY() FTransformJson transform;

	///list of materials applying to product
	UPROPERTY() TArray<FMaterialJson> materials;
};

///Object describes path to pak/use file
USTRUCT()
struct FPakJson
{
	GENERATED_BODY()
public:
	///relative path to pak file
	UPROPERTY() FString pakFilePath = "vps5_paks/Aromat_red.pak";

	///Name of the asset root folder(almost always like this)
	UPROPERTY() FString name = "PakE";

	///Name of the pak export plugin (almost always like this)
	UPROPERTY() FString mountPath = "../../../PakExport/Plugins/";

	///path to usd/usdz file overrides pak file
	UPROPERTY() FString usd_path = "vps5_paks/Aromat_red.usd";
};

///Mesh pak file
USTRUCT()
struct FMeshPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	///Mesh initial state
	UPROPERTY() FInitialStateJson initialState;
};
