// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PakExportUtilityRuntime.generated.h"

//JSON data structs
UENUM()
enum class EPakType : uint8
{
	PRODUCT,
	MATERIAL,
	LEVEL,
	CAMERA
};

USTRUCT(BlueprintType)
struct FMaterialInitialStateData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite) FString pak;
	UPROPERTY(BlueprintReadWrite) FString slot;
	UPROPERTY(BlueprintReadWrite) FString name;
	UPROPERTY(BlueprintReadWrite) FString slot_name;
};

USTRUCT(BlueprintType)
struct FInitialStateData
{
	GENERATED_BODY()
public:
 	UPROPERTY(BlueprintReadWrite) FString meshName;
	UPROPERTY(BlueprintReadWrite) FTransform transform;
	UPROPERTY(BlueprintReadWrite) TArray<FString> animations;
	UPROPERTY(BlueprintReadWrite) TArray<FMaterialInitialStateData> materials;
	UPROPERTY(BlueprintReadWrite) TArray<FString> slots;
	UPROPERTY(BlueprintReadWrite) TArray<FString> morphs;
};

USTRUCT(BlueprintType)
struct FPakPathData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite) FString pakFilePath;
private:
	UPROPERTY() FString name{"PakE"};
	UPROPERTY() FString mountPath{ "../../../PakExport/Plugins/" };
};

USTRUCT(BlueprintType)
struct FLevelPakPathData : public FPakPathData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite) FString assetName;
};

USTRUCT()
struct FMaterialPakPathData : public FPakPathData
{
	GENERATED_BODY()
public:
	UPROPERTY() FString assetName;
};

USTRUCT(BlueprintType)
struct FProductPakPathData : public FPakPathData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite) FInitialStateData initialState;
};

USTRUCT()
struct FPakData
{
	GENERATED_BODY()
public:
	FPakData()
	{
		switch (FEngineVersion::Current().GetMajor())
		{
		case 4: unrealVersion = "V_4"; break;
		case 5: unrealVersion = "V_5"; break;
		default: break;
		}
	}
	
protected:
	UPROPERTY() EPakType type = EPakType::PRODUCT;
	UPROPERTY() FString unrealVersion{};
};

USTRUCT(BlueprintType)
struct FLevelPakData : public FPakData
{
	GENERATED_BODY()
public:
	FLevelPakData()
	{
		type = EPakType::LEVEL;
	}
	UPROPERTY(BlueprintReadWrite) FString levelName;
	UPROPERTY(BlueprintReadWrite) bool hideAllLevels = false;
	UPROPERTY(BlueprintReadWrite) bool hideLastLevel = false;
	UPROPERTY(BlueprintReadWrite) bool clickable = false;
	UPROPERTY(BlueprintReadWrite) TArray<FString> levelsToHide;
	UPROPERTY(BlueprintReadWrite) FVector location = FVector(0,0,0);
	UPROPERTY(BlueprintReadWrite) FRotator rotation = FRotator(0,0,0);
	UPROPERTY(BlueprintReadWrite) FString optionalLevelName;
	UPROPERTY(BlueprintReadWrite) TArray<FString> levelType;
	UPROPERTY(BlueprintReadWrite) FLevelPakPathData levelPak;
	UPROPERTY(BlueprintReadWrite) TArray<FString> slots{};
};

USTRUCT()
struct FMaterialPakData : public FPakData
{
	GENERATED_BODY()
public:
	FMaterialPakData()
	{
		type = EPakType::MATERIAL;
	}
	UPROPERTY() FString materialName;
	UPROPERTY() FMaterialPakPathData materialPak;
};

USTRUCT(BlueprintType)
struct FProductPakData : public FPakData
{
	GENERATED_BODY()
public:
	FProductPakData()
	{
		type = EPakType::PRODUCT;
	}
	UPROPERTY(BlueprintReadWrite) FString productName;
	UPROPERTY(BlueprintReadWrite) FProductPakPathData meshPak;
	UPROPERTY(BlueprintReadWrite) FPakPathData animationsPak;
	UPROPERTY(BlueprintReadWrite) FPakPathData materialPak;
};

USTRUCT()
struct FCommandData
{
	GENERATED_BODY()
public:
	UPROPERTY() FString command;
	UPROPERTY() FString name;
};

USTRUCT()
struct FLevelCommandData : public FCommandData
{
	GENERATED_BODY()
public:
	UPROPERTY() FLevelPakData payload;
};

USTRUCT()
struct FProductCommandData : public FCommandData
{
	GENERATED_BODY()
public:
	UPROPERTY() FProductPakData payload;
};

USTRUCT()
struct FHex
{
	GENERATED_BODY()
public:
	FHex() {}
	FHex(int32 R, int32 G, int32 B) : R(R), G(G), B(B) {}
	int32 R{0};
	int32 G{0};
	int32 B{0};
};

USTRUCT()
struct FMaterialVariantData
{
	GENERATED_BODY()
public:
	FMaterialVariantData() {}
	FMaterialVariantData(const FString& Lable, const FString& Name, const FHex& Hex)
	: label(Lable), name(Name) { SetHex(Hex); }
	void SetHex(const FHex& Hex)
	{
		hex = "rgb("
		+ FString::FromInt(Hex.R) + ","
		+ FString::FromInt(Hex.G) + ","
		+ FString::FromInt(Hex.B) + ")";
	}

	UPROPERTY() FString label;
	UPROPERTY() FString name;
	
protected:
	UPROPERTY() FString hex { "rgb(0,0,0)" };
};

USTRUCT()
struct FMaterialPropertyData
{
	GENERATED_BODY()
public:
	UPROPERTY() TArray<FMaterialVariantData> variants;
};

USTRUCT()
struct FMaterialsAssetData
{
	GENERATED_BODY()
public:
	UPROPERTY() FString title;
	UPROPERTY() FString sku;
	UPROPERTY() FString pakFilePath;
	UPROPERTY() FString id;
	UPROPERTY() FString previewPath;
	UPROPERTY() FMaterialPropertyData property;
	UPROPERTY() EPakType type{EPakType::MATERIAL};
};

USTRUCT()
struct FMaterialsData
{
	GENERATED_BODY()
public:
	UPROPERTY() FMaterialsAssetData data;
};

USTRUCT(BlueprintType)
struct FCameraData : public FPakData
{
	GENERATED_BODY()
public:
	FCameraData()
	{
		type = EPakType::CAMERA;
	}
	UPROPERTY(BlueprintReadWrite) FVector location = FVector(0,0,0);
	UPROPERTY(BlueprintReadWrite) FRotator rotation = FRotator(0,0,0);
	UPROPERTY(BlueprintReadWrite) FString object;
	UPROPERTY(BlueprintReadWrite) float armLength = 0.f;
	UPROPERTY(BlueprintReadWrite) float focalLength = 0.f;
	UPROPERTY(BlueprintReadWrite) float aperture = 0.f;
};

USTRUCT()
struct FCameraCommandData : public FCommandData
{
	GENERATED_BODY()
public:
	UPROPERTY() FCameraData data;
};

USTRUCT(BlueprintType)
struct FMorphTargetData
{
	GENERATED_BODY()
public:
	FMorphTargetData() {}
	FMorphTargetData(FName Name, float Value) : Name(Name), Value(Value) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName	Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Value{};
};

/**
 * 
 */
UCLASS()
class PAKEXPORTRUNTIME_API UPakExportUtilityRuntime : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "PakExport")
	static void GenerateJsonsForAssets(const TArray<FString>& InAssets, const FString& DestinationFile);

	static FCameraData GenerateCameraData(AActor* CameraActor);
	static void GenerateJsonsForAssets(const TArray<FAssetData>& InAssets, const FString& DestinationFile, bool MaterialsPakRequested = false, bool CamerasPakRequested = false);
};
