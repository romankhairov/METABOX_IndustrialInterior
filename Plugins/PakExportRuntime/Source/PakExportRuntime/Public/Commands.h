#pragma once

#include "CoreMinimal.h"
#include "CommandsHelpers.h"
#include "Commands.generated.h"

///Base class for all commands
USTRUCT()
struct FCommandJson
{
	GENERATED_BODY()
public:
	///determines which command to execute
	UPROPERTY() FString command;
};

///Select product command payload
USTRUCT()
struct FSelectProductPayloadJson
{
	GENERATED_BODY()
public:
	///Global(static) product ID in scene (not per instance)
	UPROPERTY() FString productName;

	///Mesh pak file
	UPROPERTY() FMeshPakJson meshPak;

	///Material pak file
	UPROPERTY() FPakJson materialPak;
	
};

///Load and spawn product in the world
USTRUCT()
struct FSelectProductJson : public FCommandJson
{
	GENERATED_BODY()
public:	
	///Select product command payload
	UPROPERTY() FSelectProductPayloadJson payload;
};
