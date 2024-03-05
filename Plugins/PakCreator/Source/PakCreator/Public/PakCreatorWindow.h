// Copyright (C) 2023 Blue Mountains GmbH. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

struct FStringEntry
{
public:

	FStringEntry(const FString& InPluginPath)
	{
		PluginPath = InPluginPath;
	}

	FString PluginPath;
};

/**
 * 
 */
class FPakCreatorWindow : public TSharedFromThis<FPakCreatorWindow>
{
public:

	virtual ~FPakCreatorWindow();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TArray<TSharedPtr<FStringEntry>> Plugins;
	TArray<TSharedPtr<FStringEntry>> LogEntries;

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FStringEntry> Plugin, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<ITableRow> OnGenerateRowForLog(TSharedPtr<FStringEntry> Log, const TSharedRef<STableViewBase>& OwnerTable);

	// Handlers for changing text
	void OnProjectFileCommitted(const FText& InText, const ETextCommit::Type InTextAction);
	void OnReleaseNameCommitted(const FText& InText, const ETextCommit::Type InTextAction);
	void OnPathTextCommitted(const FText& InText, const ETextCommit::Type InTextAction);
	void OnPlatformSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);

	// Grab values for the input fields
	FText GetCurrentProjectFile() const;
	FText GetCurrentReleaseName() const;
	FText GetCurrentPath() const;
	FText GetCurrentPlatform() const;

	// Button press handlers
	FReply CreateButtonPressed();
	FReply HandleBrowseButtonClicked();
	FReply HandleProjectBrowseButtonClicked();
	ECheckBoxState HandleCompressedIsChecked() const;
	void HandleCompressedCheckedStateChanged(ECheckBoxState CheckState);

	// Delegates called from various exe files
	void AddLogMessage(const FString& Message, bool bShowInWindow = true);
	void ProcessComplete(int32 ErrorCode);

	// Run builds
	bool RunBuild();
	bool RunUATBuildProcess(const FString& CommandLine);

	FString GetTemporaryStagingDirectory() const;

private:

	EActiveTimerReturnType RefreshLog(double InCurrentTime, float InDeltaTime);

	// References to widgets
	TSharedPtr<SListView<TSharedPtr<FStringEntry>>> PluginListWidget;
	TSharedPtr<SListView<TSharedPtr<FStringEntry>>> LogListWidget;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> PlatformComboBox;
	TSharedPtr<class SButton> CreateButton;
	TSharedPtr<class SButton> ProjectBrowserButton;
	TSharedPtr<class SButton> BrowserButton;

	// Input texts
	TSharedPtr<class SEditableTextBox> ReleaseNameInput;
	TSharedPtr<class SEditableTextBox> ProjectFileInput;
	TSharedPtr<class SEditableTextBox> OutputInput;

	// Platform text
	TArray<TSharedPtr<FString>> PlatformsSource;
	TSharedPtr<STextBlock> PlatformSelectionTextBlock;
	TSharedRef<SWidget> GenerateComboBoxWidget(TSharedPtr<FString> Item);

	void PopulatePluginList(const FString& ProjectPluginDirectory);

	// Path to .uproject
	FString OutputProject = TEXT("");

	// Currently not used
	FString OutputRelease = TEXT("");

	// Path where to output the pak and stage packages
	FString OutputPath = TEXT("");

	// Whether to compress assets
	bool bWantsCompressed = true;

	FString CurrentTaskName = TEXT("");

	// Command queue for building paks with UAT. Pair key: name of the plugin(DLC), value: command line string
	TQueue<TPair<FString, FString>> PendingUATCommands;

protected:

	// Thread for building and packing
	TSharedPtr<class FUATProcess> Runnable;
};
