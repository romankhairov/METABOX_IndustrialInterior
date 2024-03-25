#pragma once

#include "CoreMinimal.h"
#include "PakExportRuntimeStatic.h"
#include "PayloadsBase.generated.h"

// ----------------------- VARIABLES JSON --------------------------------
#pragma region VariablesJsonRegion

/// { "Description" : "A vector in 2-D space composed of components (X, Y) with floating point precision." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FVector2DJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Vector's X component." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float x = 0.0f;
	/// { "Description" : "Vector's Y component." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float y = 0.0f;
};

/// { "Description" : "Structure for integer points in 2-d space." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FIntPoint2DJson
{
	GENERATED_BODY()
public:
	FIntPoint2DJson();
	FIntPoint2DJson(const FIntPoint& IntPoint);
	FIntPoint2DJson(int32 x, int32 y);

	/// { "Description" : "Holds the point's X-coordinate." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") int32 x = 0;
	/// { "Description" : "Holds the point's Y-coordinate." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") int32 y = 0;
};

/// { "Description" : "All rotation values are stored in degrees." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FRotatorJson
{
	GENERATED_BODY()
public:
	FRotatorJson();
	FRotatorJson(const FRotator& Rotator);
	FRotatorJson(FRotator&& Rotator);

	/// { "Description" : "Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down)" }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float Pitch = 0.0f;
	/// { "Description" : "Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float Yaw = 0.0f;
	/// { "Description" : "Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW." }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Payload|Json") float Roll = 0.0f;
};

/// { "Description" : "A vector in 3-D space composed of components (X, Y, Z) with floating point precision." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FVectorJson
{
	GENERATED_BODY()
public:
	FVectorJson();
	FVectorJson(const FVector& Vec);
	FVectorJson(float x, float y, float z);
	
	/// { "Description" : "Vector's X component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float x = 0.0f;
	/// { "Description" : "Vector's Y component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float y = 0.0f;
	/// { "Description" : "Vector's Z component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float z = 0.0f;
};

/// { "Description" : "Transform composed of Scale, Rotation (as a rotator), and Translation." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FTransformJson
{
	GENERATED_BODY()
public:
	FTransformJson();
	FTransformJson(const FTransform& Transform);
	FTransformJson(FTransform&& Transform);
	
	/// { "Description" : "Rotation of this transformation, as a rotator." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FRotatorJson rotation;
	/// { "Description" : "Translation of this transformation, as a vector." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVectorJson translation;
	/// { "Description" : "3D scale (always applied in local space) as a vector." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVectorJson Scale3D = FVectorJson(1.0f, 1.0f, 1.0f);
};

/// { 
/// "TODO" : "Are there clamps necessary?", 
/// "Description" : "Scalar material parameter." 
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FScalarJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Scalar parameter name in material." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString parameterName = "print_switch";
	/// { 
	/// "TODO" : "Are there clamps necessary?", 
	/// "Description" : "Value of the scalar material parameter." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json" /*, meta = (ClampMin = "0.0", ClampMax = "1.0") */ ) float value = 1.0f;
};

/// { 
/// "TODO" : "Are there clamps necessary?", 
/// "Description" : "LinearColor RGBA(1.1.1.1)." 
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FLinearColorJson
{
	GENERATED_BODY()
public:
	FLinearColorJson();
	FLinearColorJson(const FLinearColor& Color);
	FLinearColorJson(float r, float g, float b, float a);

	/// { "Description" : "Color's Red component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json" /*, meta = (ClampMin = "0.0", ClampMax = "1.0") */) float r = 1.0f;
	/// { "Description" : "Color's Green component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json" /*, meta = (ClampMin = "0.0", ClampMax = "1.0") */) float g = 1.0f;
	/// { "Description" : "Color's Blue component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json" /*, meta = (ClampMin = "0.0", ClampMax = "1.0") */) float b = 1.0f;
	/// { "Description" : "Color's Alpha component." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"), Category = "Math|Color" ) float a = 1.0f;
};

/// { "Description" : "Material texture." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FTextureJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Parameter for texture in material." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString parameterName = "print";
	/// { "Description" : "URL to download texture from." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString url = "https://i.ibb.co/tsjtsDf/Pngtree-duck-202201.png";
	/// { "Description" : "Enable canvas for texture or not." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") bool canvas = true;
	/// { "Description" : "Size of the texture." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson size = {1024, 1024};
	/// { "Description" : "Position of the texture in relative space." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson position = {0.5, 0.5};
	/// { "Description" : "Rotate angle in degrees." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") float Rotate = 0;
	/// { "Description" : "Pivot of the texture in relative space." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson pivot = {0.5, 0.5};
	/// { "Description" : "Grid position." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson coordinatePosition = {0, 0};
	/// { "Description" : "Grid size." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FVector2DJson coordinateSize = {1, 1};
};

/// { "Description" : "Material." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Name of the asset root folder(almost always like this." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString pak = UPakExportRuntimeStatic::PakExportName;
	/// { "Description" : "Name of the asset relative to Content folder in project." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString name = "";
	/// { "Description" : "Material name applying material to." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString slot_name = "";
	/// { "Description" : "Vector parameter color applying to." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString colorParameterName = "";
	/// { "Description" : "Color in hex format like FF0000 of #FF0000." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString colorHex = "";
	/// { "Description" : "List of textures in the material." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FTextureJson> textures;
	/// { "Description" : "List of scalars in the material." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FScalarJson> scalars;
	/// { "Description" : "Relative path to material pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString pakFilePath = "";
};

/// { "Description" : "Mesh initial state." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FInitialStateJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Name of the asset relative to Content folder in project." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString meshName = "";
	/// { "Description" : "Product spawn transform." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FTransformJson transform;
	/// { "Description" : "List of materials applying to product." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FMaterialJson> materials;
	/// { "Description" : "Product animations." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> animations;
	/// { "Description" : "Materials slots." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> slots;
	/// { "Description" : "Founded in product morphs." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> morphs;
};

/// { "Description" : "MeshAnimation ." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMeshAnimationJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Animation name." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString key = "";
	/// { "Description" : "Animation full name with path." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") FString value = "";
	/// { 
	/// "TODO" : "what children ???", 
	/// "Description" : "Children." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json") TArray<FString> children;
};

/// { "Description" : "Object describes path to pak/use file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Relative path to pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString pakFilePath = "";
	/// { "Description" : "Name of the asset root folder(almost always like this)." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString name = UPakExportRuntimeStatic::PakExportName;
	/// { "Description" : "Name of the pak export plugin (almost always like this)." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString mountPath = UPakExportRuntimeStatic::PakExportMountPath;
	/// { "Description" : "Path to usd/usdz file overrides pak file path." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString usd_path = "";
};

/// { "Description" : "Mesh pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMeshPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Mesh initial state." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FInitialStateJson initialState;
};

/// { "Description" : "MeshAnimation pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMeshAnimationPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "MeshAnimation map names." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") TArray<FMeshAnimationJson> meshAnimations;
};

/// { "Description" : "Asset pak file." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FAssetPakJson : public FPakJson
{
	GENERATED_BODY()
public:
	/// { "Description" : "Asset name." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Json|PAK") FString assetName;
};

#pragma endregion

// ----------------------- BASE PAYLOADS --------------------------------
#pragma region BasePayloadsRegion

/// { "Description" : "Type of Asset for all PAKs." }
UENUM(BlueprintType)
enum class EAssetType : uint8
{
	none,
	product,
	material,
	environment,
	level, //Deprecated, need for json strict
	camera,
	level_sequence,
	screenshot,
	light
};

/// { "Description" : "Base payload structure." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FPayloadBase
{
	GENERATED_BODY()
public:
	FPayloadBase();

protected:
	/// { "Description" : "Asset id." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FString assetId;
	/// { "Description" : "Unreal version." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FString unrealVersion;
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") EAssetType type = EAssetType::none;
};

/// { "Description" : "Base payload with bool to enable, click, drag and etc." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FBoolPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FBoolPayload();
	/// { "Description" : "This value means enable, click, drag, etc." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") bool value = false;
};

/// { "Description" : "Base payload for float." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFloatPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FFloatPayload();
	/// { "Description" : "This value is for changing float values." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") float value = 0;
};

/// { "Description" : "Base payload for vector." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFVectorPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FFVectorPayload();
	/// { "Description" : "This value is for changing vector values." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FVectorJson value;
};

/// { "Description" : "Base payload for string." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FFStringPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FFStringPayload();
	/// { "Description" : "This value could be used for ObjectName, URL and other strings." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload") FString value;
};

#pragma endregion

// ----------------------- ASSETS --------------------------------
#pragma region AssetsRegion

/// { "Description" : "This payload is used in comand to load/spawn and unload/destroy products." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FProductPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FProductPayload();
	
	/// { "Description" : "Global(static) product ID in scene (not per instance)." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FString productName;
	/// { "Description" : "Mesh pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FMeshPakJson meshPak;
	/// { "Description" : "Material pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FPakJson materialPak;
	/// { "Description" : "Animations pak file." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FPakJson animationsPak;
	/// { "Description" : "Object name." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FString objectName;
	/// { "Description" : "Material slots." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> material_slots;
	/// { "Description" : "Tranform." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") FTransformJson transform;
	/// { "Description" : "Default vector parametres of materials." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Product") TArray<FString> material_vector_params;
};

/// {
/// "TODO" : "Check alld properties in BP", 
/// "Description" : "Load sublevel."
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FLevelPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FLevelPayload();
	/// { "Description" : "Level name." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FString levelName;
	/// { "Description" : "Bool that hide all levels, after that it loads level with levelName." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool hideAllLevels = false;
	/// { "Description" : "Hide last level." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool hideLastLevel = false;
	/// {
	/// "TODO" : "WTF? Rename." 
	/// "Description" : "ObjectName of product to destroy." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") bool clickable = false;
	/// { "Description" : "Array of levels to hide before loading LevelName." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") TArray<FString> levelsToHide;
	/// { "Description" : "Loaction of this sublevel." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FVectorJson location;
	/// { "Description" : "Rotation of this sublevel." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FRotatorJson rotation;
	/// {
	/// "TODO" : "Remove this property." 
	/// "Description" : "Optional name to assign for loaded sublevel." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FString optionalLevelName;
	/// { "Description" : "Level type." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") TArray<FString> levelType;
	/// { "Description" : "Level PAK." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") FAssetPakJson levelPak;
	/// {
	/// "TODO" : "WTF? Rename.", 
	/// "Description" : "Slots." 
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Level") TArray<FString> slots;
};

/// { "Description" : "Material Base Payload for PakExport." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FMaterialBasePayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FMaterialBasePayload();
	/// { "Description" : "Material name." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Matrial") FString materialName;
	/// { "Description" : "Material PAK." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Matrial") FAssetPakJson materialPak;
};

#pragma endregion

// ----------------------- CONTROLS CAMERA --------------------------------
#pragma region ControlsCameraRegion

/// { "Description" : "Camera settings." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FCameraSettingsPayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	FCameraSettingsPayload();
	/// { "Description" : "Camera aperture." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera") float aperture = 0;
	/// { "Description" : "Camera FocusOffset." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera") float focusOffset = 0;
	/// { "Description" : "Camera ArmoLength." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera") float armLength = 0;
	/// { "Description" : "Camera FocalLength." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera") float focalLength = 0;
	/// { "Description" : "Camera FOV." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Camera") float fov = 0;
};

/// { "Description" : "CameraPreset with Location and Rotation. It Used in PakExporRuntime." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FCameraPresetPayload : public FCameraSettingsPayload
{
	GENERATED_BODY()
public:
	FCameraPresetPayload();
	/// { "Description" : "Location." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FVectorJson location;
	/// { "Description" : "Rotation." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FRotatorJson rotation;
	/// { "Description" : "Camera object." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FString object;
};

#pragma endregion

// ----------------------- SEQUENCE --------------------------------
#pragma region SequenceRegion

/// { "Description" : "SequenceLoopBehavior." }
UENUM(BlueprintType)
enum class ESequenceLoopBehavior : uint8
{
	none,						//UMETA(DisplayName = "None"),
	loop,						//UMETA(DisplayName = "Loop"),
	pingPong,					//UMETA(DisplayName = "PingPong"),
	loopSection,				//UMETA(DisplayName = "LoopSection"),
	pingPongSection				//UMETA(DisplayName = "PingPongSection")
};

/// { "Description" : "Settings of sequence." }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FSettingsSequencePayload : public FPayloadBase
{
	GENERATED_BODY()
public:
	/// { "Description" : "Loop behavior of sequence playing." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") ESequenceLoopBehavior loopBehavior = ESequenceLoopBehavior::none;
	/// { 
	/// "TODO" : "Make play rate. It could be negative, so remove bool reverse.",
	/// "Description" : "Rate-speed of sequence playing"
	/// }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") float playRate;
	/// { "Description" : "Is reverse playing of sequence." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") bool reverse;
	/// { "Description" : "Time period of responding changes for callbacks." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|Sequence") float timeResponseChangeCallBack;
};

/// {
/// "TODO" : "Nowadays PAKs doesn't work. ",
/// "Description" : "Init sequence." 
/// }
USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FInitSequencePayload : public FSettingsSequencePayload
{
	GENERATED_BODY()
public:
	/// { "Description" : "sequence to init if app uses CameraCuts as sections, not LevelSequences as sections." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") FString sequenceName;
	/// { "Description" : "PAKs of sequences." }
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Payload|PAK") TArray<FAssetPakJson> paks;
};

#pragma endregion

USTRUCT(BlueprintType)
struct PAKEXPORTRUNTIME_API FTempCommandPayload
{
	GENERATED_BODY()
public:
	FTempCommandPayload();
};
