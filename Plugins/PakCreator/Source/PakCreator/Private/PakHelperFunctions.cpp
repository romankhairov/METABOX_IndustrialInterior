// Copyright (C) 2023 Blue Mountains GmbH. All Rights Reserved.

#include "PakHelperFunctions.h"
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION == 27)
#include "Settings/ProjectPackagingSettings.h"
#endif

FString FPakHelperFunctions::MakeUATCommand(const FString& UProjectFile, const FString& PlatformName, const FString& Cookflavor, const FString& ConfigurationName, const FString& StageDirectory, bool bCompress)
{
	FString CommandLine = FString::Printf(TEXT("BuildCookRun -project=\"%s\" -noP4"), *UProjectFile);

	CommandLine += FString::Printf(TEXT(" -clientconfig=%s -serverconfig=%s"), *ConfigurationName, *ConfigurationName);

	// UAT should be compiled already
	CommandLine += " -nocompile -nocompileeditor";

	CommandLine += FApp::IsEngineInstalled() ? TEXT(" -installed") : TEXT("");

	CommandLine += " -utf8output";

	CommandLine += " -platform=" + PlatformName;

	if (Cookflavor.Len() > 0)
	{
		CommandLine += " -cookflavor=" + Cookflavor;
	}

	CommandLine += " -build -cook -CookCultures=en -unversionedcookedcontent -pak";

	if (bCompress)
	{
		CommandLine += " -compressed";
	}

	CommandLine += " -stage";

	CommandLine += FString::Printf(TEXT(" -stagingdirectory=\"%s\""), *StageDirectory);

	return CommandLine;
}

FString FPakHelperFunctions::MakeUATParams_BaseGame(const FString& UProjectFile, const FString& ReleaseVersionName)
{
	FString OutParams = FString::Printf(TEXT(" -package -createreleaseversion=\"%s\""), *ReleaseVersionName);

	TArray<FString> Result;
	TArray<FString> ProjectMapNames;

	const FString WildCard = FString::Printf(TEXT("*%s"), *FPackageName::GetMapPackageExtension());

	// Scan all Content folder, because not all projects follow Content/Maps convention
	IFileManager::Get().FindFilesRecursive(ProjectMapNames, *FPaths::Combine(FPaths::GetPath(UProjectFile), TEXT("Content")), *WildCard, true, false);

	for (int32 i = 0; i < ProjectMapNames.Num(); i++)
	{
		Result.Add(FPaths::GetBaseFilename(ProjectMapNames[i]));
	}

	Result.Sort();

	if (Result.Num() > 0)
	{
		// Our goal is to only have assets from inside the actual plugin content folder.
		// In order for Unreal to only put these assets inside the pak and not assets from /Game we have to specify all maps from /Game to the command line for UAT.
		// Format: -map=Value1+Value2+Value3

		OutParams += " -map=";

		for (int32 i = 0; i < Result.Num(); i++)
		{
			OutParams += Result[i];

			if (i + 1 < Result.Num())
			{
				OutParams += "+";
			}
		}
	}

	return OutParams;
}

FString FPakHelperFunctions::MakeUATParams_DLC(const FString& DLCName, const FString& ReleaseVersionName)
{
	FString CommandLine = FString::Printf(TEXT(" -basedonreleaseversion=\"%s\""), *ReleaseVersionName);

	CommandLine += " -stagebasereleasepaks -DLCIncludeEngineContent";

	CommandLine += FString::Printf(TEXT(" -dlcname=\"%s\""), *DLCName);

	return CommandLine;
}

void FPakHelperFunctions::GetPlatformNameAndFlavorBySelection(const FString& InSelection, FString& OutPlatform, FString& OutFlavor)
{
	int32 Idx;
	if (InSelection.FindChar('_', Idx))
	{
		OutPlatform = InSelection.Left(Idx);
		OutFlavor = InSelection.Right(InSelection.Len() - Idx - 1);
	}
	else
	{
		OutPlatform = InSelection;
		OutFlavor = "";
	}
}

TArray<FString> FPakHelperFunctions::GetPluginFolders(const FString& ProjectPluginDirectory)
{
	// Class used for the visitor in iterate directory
	class FPluginDirectoryMatch : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString>& Result;
		IFileManager& FileManager;

		FPluginDirectoryMatch(TArray<FString>& InResult, IFileManager& InFileManager)
			: Result(InResult)
			, FileManager(InFileManager)
		{
		}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory)
			{
				TArray<FString> FoundFiles;
				FileManager.FindFiles(FoundFiles, *FPaths::ConvertRelativePathToFull(FilenameOrDirectory), TEXT(".uplugin"));

				// Only return directories that have a uplugin file
				if (FoundFiles.Num() > 0)
				{
					// Pass back the file name
					new(Result) FString(FPaths::GetCleanFilename(FilenameOrDirectory));
				}
			}
			return true;
		}
	};

	TArray<FString> DirectoryNames;

	IFileManager& FileManager = IFileManager::Get();

	FPluginDirectoryMatch DirectoryMatch(DirectoryNames, FileManager);
	if (FileManager.IterateDirectory(*ProjectPluginDirectory, DirectoryMatch))
	{
		// Trim away the path and just keep the folder name
		for (FString& Directory : DirectoryNames)
		{
			TArray<FString> ParsedArray;
			Directory.ParseIntoArray(ParsedArray, TEXT("\\"));

			Directory = ParsedArray.Last();
		}

		// Sort the list
		DirectoryNames.Sort();
	}

	return DirectoryNames;
}

bool FPakHelperFunctions::IsIoStoreEnabled()
{
#if ENGINE_MAJOR_VERSION == 5 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION == 27)
	UProjectPackagingSettings* PackagingSettings = Cast<UProjectPackagingSettings>(UProjectPackagingSettings::StaticClass()->GetDefaultObject());
	if (PackagingSettings)
	{
		return PackagingSettings->bUseIoStore;
	}
#endif
	return false;
}
