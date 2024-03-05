// Copyright (C) 2023 Blue Mountains GmbH. All Rights Reserved.

#include "PakCreatorWindow.h"
#include "Misc/Paths.h"
#include "Misc/App.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Views/ITypedTableView.h"
#include "HAL/FileManager.h"
#include "FileHelpers.h"
#include "Widgets/SWidget.h"
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0) || (ENGINE_MAJOR_VERSION == 4)
#include "EditorStyleSet.h"
#endif
#include "Async/Async.h"
#include "PakCreator.h"
#include "UATProcess.h"
#include "AutomatedPakParams.h"
#include "PakHelperFunctions.h"

#define LOCTEXT_NAMESPACE "FPakCreatorWindow"

FPakCreatorWindow::~FPakCreatorWindow()
{
	if (Runnable.IsValid())
	{
		// Cancel thread
		Runnable->Cancel();

		// Wait for join
		Runnable.Reset();
	}
}

FText FPakCreatorWindow::GetCurrentProjectFile() const
{
	return FText::FromString(OutputProject);
}

FText FPakCreatorWindow::GetCurrentReleaseName() const
{
	return FText::FromString(OutputRelease);
}

FText FPakCreatorWindow::GetCurrentPath() const
{
	return FText::FromString(OutputPath);
}

FText FPakCreatorWindow::GetCurrentPlatform() const
{
	return FText::FromString(*FAutomatedPakParams::ValidPlatformNames[0]);
}

TSharedRef<SDockTab> FPakCreatorWindow::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	for (const FString& PlatformName : FAutomatedPakParams::ValidPlatformNames)
	{
		PlatformsSource.Add(MakeShared<FString>(PlatformName));
	}
	check(PlatformsSource.Num() == FAutomatedPakParams::ValidPlatformNames.Num());

	FText ProjectPathText = LOCTEXT("ProjectPathWidgetText", ".uproject File");
	FText ReleaseNameText = LOCTEXT("ReleasenameWidgetText", "Release Name of Project");
	FText SelectPluginText = LOCTEXT("SelectPluginWidgetText", "Select Plugin Content Directory");
	FText PlatformText = LOCTEXT("PlatformText", "Platform");
	FText PlatformToolTip = LOCTEXT("PlatformToolTip", "Choose a target platform for cooking the assets for the pak file.");
	FText CreatePakText = LOCTEXT("CreatePakWidgetText", "Create .pak File");
	FText OutputPathText = LOCTEXT("OutputPathWidgetText", "Select .pak Output Path");
	FText SetFolderText = LOCTEXT("SetFolderhWidgetText", "Browse");
	FText SetProjectText = LOCTEXT("SetProjectWidgetText", "Browse");
	FText LogText = LOCTEXT("LogWidgetText", "Log goes here");

	TSharedRef<SDockTab> PluginTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.Padding(15.0f)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(SelectPluginText)
					]
					+ SVerticalBox::Slot()
					.Padding(10.0f)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(PluginListWidget, SListView<TSharedPtr<FStringEntry>>)
						.ListItemsSource(&Plugins)
						.SelectionMode(ESelectionMode::Type::Multi)
						.OnGenerateRow(this, &FPakCreatorWindow::OnGenerateRowForList)
						.ScrollbarVisibility(EVisibility::Hidden)
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(ProjectPathText)
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					.Padding(10.0f)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(ProjectFileInput, SEditableTextBox)
							.Text(this, &FPakCreatorWindow::GetCurrentProjectFile)
							.OnTextCommitted(this, &FPakCreatorWindow::OnProjectFileCommitted)
							.IsReadOnly(false)
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						.AutoWidth()
						[
							SAssignNew(ProjectBrowserButton, SButton)
							.ButtonStyle(FCoreStyle::Get(), "NoBorder")
							.OnClicked(this, &FPakCreatorWindow::HandleProjectBrowseButtonClicked)
							.HAlign(HAlign_Right)
							.VAlign(VAlign_Center)
							.ForegroundColor(FSlateColor::UseForeground())
							[
								SNew(STextBlock)
								.Text(SetProjectText)
							]
						]
					]
					/* + SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(ReleaseNameText)
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					.Padding(10.0f)
					.AutoHeight()
					[
						SAssignNew(ReleaseNameInput, SEditableTextBox)
						.Text(this, &FPakCreatorWindow::GetCurrentReleaseName)
						.OnTextCommitted(this, &FPakCreatorWindow::OnReleaseNameCommitted)
					]*/
					+SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(PlatformText)
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					.Padding(10.0f)
					.AutoHeight()
					[
						SAssignNew(PlatformComboBox, SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&PlatformsSource)
						.InitiallySelectedItem(PlatformsSource[0])
						.ToolTipText(PlatformToolTip)
						.OnSelectionChanged(this, &FPakCreatorWindow::OnPlatformSelected)
						.OnGenerateWidget(this, &FPakCreatorWindow::GenerateComboBoxWidget)
						[
							SAssignNew(PlatformSelectionTextBlock, STextBlock)
							.Text(this, &FPakCreatorWindow::GetCurrentPlatform)

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
							.Font(FAppStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont")))
#else
							.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
#endif
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(OutputPathText)
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					.Padding(10.0f)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Fill)
						.HAlign(HAlign_Fill)
						[
							SAssignNew(OutputInput, SEditableTextBox)
							.Text(this, &FPakCreatorWindow::GetCurrentPath)
							.OnTextCommitted(this, &FPakCreatorWindow::OnPathTextCommitted)
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						.AutoWidth()
						[
							SAssignNew(BrowserButton, SButton)
							.ButtonStyle(FCoreStyle::Get(), "NoBorder")
							.OnClicked(this, &FPakCreatorWindow::HandleBrowseButtonClicked)
							.HAlign(HAlign_Right)
							.VAlign(VAlign_Center)
							.ForegroundColor(FSlateColor::UseForeground())
							[
								SNew(STextBlock)
								.Text(SetFolderText)
							]
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SCheckBox)
						.IsChecked(this, &FPakCreatorWindow::HandleCompressedIsChecked)
						.OnCheckStateChanged(this, &FPakCreatorWindow::HandleCompressedCheckedStateChanged)
						.Padding(FMargin(4.0f, 0.0f))
						.ToolTipText(LOCTEXT("CompressedCheckBoxTooltip", "If checked, Unreal will compress the cooked assets inside the pak file."))
						.Content()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("CompressedCheckBoxText", "Compress assets"))
						]
					]
					+ SVerticalBox::Slot()
					.Padding(10.0f)
					.AutoHeight()
					[
						SAssignNew(CreateButton, SButton)
#if ENGINE_MAJOR_VERSION == 4
						.ButtonStyle(FCoreStyle::Get(), "NoBorder")
#endif
						.OnClicked(this, &FPakCreatorWindow::CreateButtonPressed)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.ForegroundColor(FSlateColor::UseForeground())
						[
							SNew(STextBlock)
							.Text(CreatePakText)
						]
					]
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					[
						SAssignNew(LogListWidget, SListView< TSharedPtr<FStringEntry> >)
						.ListItemsSource(&LogEntries)
						.SelectionMode(ESelectionMode::Type::None)
						.OnGenerateRow(this, &FPakCreatorWindow::OnGenerateRowForLog)
						.ScrollbarVisibility(EVisibility::Hidden)
					]
				]
			]
		];

	// Grab saved vars
	if (GConfig != nullptr)
	{
		GConfig->GetString(
			TEXT("PakCreator.Core"),
			TEXT("ReleaseGameName"),
			OutputRelease,
			GEditorPerProjectIni
		);

		GConfig->GetString(
			TEXT("PakCreator.Core"),
			TEXT("OutputPath"),
			OutputPath,
			GEditorPerProjectIni
		);
	}

	if (FPaths::IsProjectFilePathSet())
	{
		OutputProject = FPaths::GetProjectFilePath();
	}
	else
	{
		OutputProject = FPaths::ProjectDir() / FApp::GetProjectName() + ".uproject";
	}

	OutputProject = FPaths::ConvertRelativePathToFull(OutputProject);

	PopulatePluginList(FPaths::ProjectPluginsDir());

	return PluginTab;
}

void FPakCreatorWindow::PopulatePluginList(const FString& ProjectPluginDirectory)
{
	Plugins.Empty();

	// Populate plugins
	const TArray<FString> PluginDirectories = FPakHelperFunctions::GetPluginFolders(FPaths::GetPath(OutputProject) / TEXT("Plugins"));
	for (const FString& Directory : PluginDirectories)
	{
		// Skip this plugin
		if (Directory == "PakCreator")
			continue;

		TSharedPtr<FStringEntry> Entry = MakeShareable(new FStringEntry(Directory));

		Plugins.Add(Entry);
	}

	if (PluginListWidget->IsParentValid())
	{
		PluginListWidget->RebuildList();
	}
}

FReply FPakCreatorWindow::CreateButtonPressed()
{
	ensure(IsInGameThread());

	if (PendingUATCommands.Peek())
	{
		// Already running
		return FReply::Handled();
	}

	LogEntries.Empty();

	if (FPakHelperFunctions::IsIoStoreEnabled())
	{
		AddLogMessage(FString::Printf(TEXT("Warning: \"IoStore\" is enabled in project settings. In order to only use .pak files you must disable this setting.")));
	}

	FText Reason;
	if (OutputPath.Len() == 0 || !FPaths::ValidatePath(OutputPath, &Reason))
	{
		AddLogMessage(FString::Printf(TEXT("Error: Output path or length is invalid %s (%s)"), *Reason.ToString(), *OutputPath));
		return FReply::Handled();
	}

	const TArray<TSharedPtr<FStringEntry>> SelectedItems = PluginListWidget->GetSelectedItems();
	if (SelectedItems.Num() == 0)
	{
		AddLogMessage(TEXT("Error: No plugin selected!"));
		return FReply::Handled();
	}

	const TSharedPtr<FString> SelectedPlatform = PlatformComboBox->GetSelectedItem();
	if (!SelectedPlatform.IsValid())
	{
		AddLogMessage(TEXT("Error: No platform selected!"));
		return FReply::Handled();
	}

	PendingUATCommands.Empty();

	const FString ConfigurationName = "Shipping";
	FString PlatformName = FAutomatedPakParams::ValidPlatformNames[0];
	FString Cookflavor = "";

	FPakHelperFunctions::GetPlatformNameAndFlavorBySelection(*SelectedPlatform, PlatformName, Cookflavor);

	AddLogMessage(FString::Printf(TEXT("Platform: %s%s"), *PlatformName, *Cookflavor));

	const FString BaseGameCommand =
		FPakHelperFunctions::MakeUATCommand(
			OutputProject,
			PlatformName,
			Cookflavor,
			ConfigurationName,
			FPaths::Combine(GetTemporaryStagingDirectory(), FAutomatedPakParams::ReleaseVersionName),
			bWantsCompressed)
		+ FPakHelperFunctions::MakeUATParams_BaseGame(OutputProject, FAutomatedPakParams::ReleaseVersionName);

#if ENGINE_MAJOR_VERSION == 5
	PendingUATCommands.Enqueue({ FAutomatedPakParams::ReleaseVersionName, BaseGameCommand });
#else
	PendingUATCommands.Enqueue(TPair<FString, FString>(FAutomatedPakParams::ReleaseVersionName, BaseGameCommand));
#endif

	int32 NumPluginBuilds = 0;

	for (const TSharedPtr<FStringEntry>& Item : SelectedItems)
	{
		const FString DLCCommand =
			FPakHelperFunctions::MakeUATCommand(
				OutputProject,
				PlatformName,
				Cookflavor,
				ConfigurationName,
				FPaths::Combine(GetTemporaryStagingDirectory(), Item->PluginPath),
				bWantsCompressed)
			+ FPakHelperFunctions::MakeUATParams_DLC(Item->PluginPath, FAutomatedPakParams::ReleaseVersionName);

#if ENGINE_MAJOR_VERSION == 5
		PendingUATCommands.Enqueue({ Item->PluginPath, DLCCommand });
#else
		PendingUATCommands.Enqueue(TPair<FString, FString>(Item->PluginPath, DLCCommand));
#endif

		NumPluginBuilds++;
	}

	AddLogMessage(FString::Printf(TEXT("Building %i plugin%s"), NumPluginBuilds, NumPluginBuilds > 1 ? TEXT("s") : TEXT("")));
	AddLogMessage(TEXT("Notice: Building can take longer on the first run due to shaders compiling. Please be patient."));

	// Start off first build
	RunBuild();

	return FReply::Handled();
}

bool FPakCreatorWindow::RunBuild()
{
	check(IsInGameThread());

#if ENGINE_MAJOR_VERSION == 5
	decltype(PendingUATCommands)::FElementType BuildItem;
#else
	TPair<FString, FString> BuildItem;
#endif
	
	if (PendingUATCommands.Dequeue(BuildItem))
	{
		AddLogMessage(FString::Printf(TEXT("Building \"%s\""), *BuildItem.Key), true);

		CurrentTaskName = BuildItem.Key;

		return RunUATBuildProcess(BuildItem.Value);
	}

	return false;
}

bool FPakCreatorWindow::RunUATBuildProcess(const FString& CommandLine)
{
	check(Runnable == nullptr);

	FString ExecutablePath, Executable;
	FUATProcess::GetUATExecutable(ExecutablePath, Executable);

	Runnable = MakeShared<FUATProcess>();

	Runnable->OnOutput().BindSP(this, &FPakCreatorWindow::AddLogMessage);
	Runnable->OnTerminated().BindSP(this, &FPakCreatorWindow::ProcessComplete);

	return Runnable->Launch(ExecutablePath / Executable, ExecutablePath, CommandLine);
}

FString FPakCreatorWindow::GetTemporaryStagingDirectory() const
{
	return FPaths::Combine(OutputPath, TEXT("__TMP_STAGING__"));
}

void FPakCreatorWindow::ProcessComplete(int32 ErrorCode)
{
	if (!IsInGameThread())
	{
		// Retry on the GameThread.
		TWeakPtr<FPakCreatorWindow> WeakShared = AsShared();
		AsyncTask(ENamedThreads::GameThread, [WeakShared, ErrorCode]()
		{
			const TSharedPtr<FPakCreatorWindow>& PakCreatorWindow = WeakShared.Pin();
			if (PakCreatorWindow.IsValid())
			{
				PakCreatorWindow->ProcessComplete(ErrorCode);
			}
		});
		return;
	}

	Runnable.Reset();

	if (ErrorCode != 0)
	{
		PendingUATCommands.Empty();

		AddLogMessage(FString::Printf(TEXT("Process exited with error code = %i. See the Output Log for more information. (Window -> Developer Tools -> Output Log)"), ErrorCode));
		return;
	}

	AddLogMessage(TEXT("Build completed"));

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Check if the current task is a pak or just the base game.
	if (CurrentTaskName != FAutomatedPakParams::ReleaseVersionName)
	{
		// Its a pak, let's move the new pak to our desired location.

		const FString CurrentStagingDirectory = GetTemporaryStagingDirectory() / CurrentTaskName;

		TArray<FString> FoundFiles;
		PlatformFile.FindFilesRecursively(FoundFiles, *CurrentStagingDirectory, TEXT(".pak"));
		PlatformFile.FindFilesRecursively(FoundFiles, *CurrentStagingDirectory, TEXT(".ucas"));
		PlatformFile.FindFilesRecursively(FoundFiles, *CurrentStagingDirectory, TEXT(".utoc"));

		if (FoundFiles.Num() > 0)
		{
			for (const FString& File : FoundFiles)
			{
				const FString CopyTo = OutputPath / FPaths::GetCleanFilename(File);

				// Delete existing pak at target location
				if (PlatformFile.FileExists(*CopyTo))
				{
					PlatformFile.DeleteFile(*CopyTo);
				}

				if (PlatformFile.MoveFile(*CopyTo, *File))
				{
					AddLogMessage(FString::Printf(TEXT("Moving %s to %s"), *FPaths::GetCleanFilename(File), *OutputPath));
				}
				else
				{
					AddLogMessage(FString::Printf(TEXT("Error: Failed to move file from %s to %s"), *File, *CopyTo));
				}
			}
		}
		else
		{
			AddLogMessage(FString::Printf(TEXT("Error: Pak file not found in directory %s"), *CurrentStagingDirectory));
		}
	}

	if (!RunBuild())
	{
		// If no more builds have been launched do some cleanup
		
		// Remove temporary staging directory
		if (!PlatformFile.DeleteDirectoryRecursively(*GetTemporaryStagingDirectory()))
		{
			AddLogMessage(FString::Printf(TEXT("Warning: Failed to delete temporary build directory %s"), *GetTemporaryStagingDirectory()));
		}

		AddLogMessage(TEXT("All builds finished"));
	}
}

void FPakCreatorWindow::AddLogMessage(const FString& Message, bool bShowInWindow)
{
	if (!IsInGameThread())
	{
		// Retry on the GameThread.
		TWeakPtr<FPakCreatorWindow> WeakShared = AsShared();
		AsyncTask(ENamedThreads::GameThread, [WeakShared, Message, bShowInWindow]()
		{
			const TSharedPtr<FPakCreatorWindow>& PakCreatorWindow = WeakShared.Pin();
			if (PakCreatorWindow.IsValid())
			{
				PakCreatorWindow->AddLogMessage(Message, bShowInWindow);
			}
		});
		return;
	}

	UE_LOG(LogPakCreator, Log, TEXT("%s"), *Message);

	if (bShowInWindow)
	{
		TSharedPtr<FStringEntry> Entry = MakeShareable(new FStringEntry(Message));

		LogEntries.Add(Entry);

		LogListWidget->RegisterActiveTimer(0.1f, FWidgetActiveTimerDelegate::CreateSP(this, &FPakCreatorWindow::RefreshLog));
	}
}

EActiveTimerReturnType FPakCreatorWindow::RefreshLog(double InCurrentTime, float InDeltaTime)
{
	if (LogListWidget.IsValid())
	{
		LogListWidget->RequestListRefresh();
	}

	return EActiveTimerReturnType::Continue;
}

FReply FPakCreatorWindow::HandleBrowseButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(CreateButton.ToSharedRef());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		FString FolderName;
		const bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			LOCTEXT("FolderDialogTitle", "Choose a directory").ToString(),
			OutputPath,
			FolderName
		);

		UE_LOG(LogPakCreator, Verbose, TEXT("Folder Name = %s"), *FolderName);
		OutputPath = FolderName;

		if (GConfig != nullptr)
		{
			GConfig->SetString(
				TEXT("PakCreator.Core"),
				TEXT("OutputPath"),
				*OutputPath,
				GEditorPerProjectIni
			);

			GConfig->Flush(false, GEditorPerProjectIni);
		}
	}

	return FReply::Handled();
}

FReply FPakCreatorWindow::HandleProjectBrowseButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(CreateButton.ToSharedRef());
		void* ParentWindowHandle = (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr;

		TArray<FString> Files;

		const bool bFileSelected = DesktopPlatform->OpenFileDialog(
			ParentWindowHandle,
			LOCTEXT("FileDialogTitle", "Choose a file").ToString(),
			FPaths::GetPath(OutputProject),
			OutputProject,
			".uproject",
			0,
			Files
		);

		if (bFileSelected && Files.Num() > 0)
		{
			UE_LOG(LogPakCreator, Verbose, TEXT("Folder Name = %s"), *Files[0]);
			
			OutputProject = FPaths::ConvertRelativePathToFull(Files[0]);

			PopulatePluginList(FPaths::GetPath(OutputProject) / TEXT("Plugins"));
		}
	}

	return FReply::Handled();
}

ECheckBoxState FPakCreatorWindow::HandleCompressedIsChecked() const
{
	return bWantsCompressed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void FPakCreatorWindow::HandleCompressedCheckedStateChanged(ECheckBoxState CheckState)
{
	bWantsCompressed = CheckState == ECheckBoxState::Checked ? true : false;
}

TSharedRef<ITableRow> FPakCreatorWindow::OnGenerateRowForList(TSharedPtr<FStringEntry> Plugin, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!Plugins.IsValidIndex(0) || !Plugin.IsValid() || !Plugin.Get()) // Error catcher
	{
		return
			SNew(STableRow<TSharedPtr<FStringEntry>>, OwnerTable)
			[
				SNew(SBox)
			];
	}

	return SNew(STableRow<TSharedPtr<FStringEntry>>, OwnerTable)
		[
			SNew(SBox)
			[
				SNew(STextBlock) // places the timestamp
				.Text(FText::FromString(Plugin.Get()->PluginPath))
				.ColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.f))
				//.Font(fontinfo)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(1, 1))
				.AutoWrapText(true)
			]
		];
}

TSharedRef<ITableRow> FPakCreatorWindow::OnGenerateRowForLog(TSharedPtr<FStringEntry> LogEntry, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!LogEntries.IsValidIndex(0) || !LogEntry.IsValid() || !LogEntry.Get()) // Error catcher
	{
		return
			SNew(STableRow<TSharedPtr<FStringEntry>>, OwnerTable)
			[
				SNew(SBox)
			];
	}

	return SNew(STableRow<TSharedPtr<FStringEntry>>, OwnerTable)
		[
			SNew(SBox)
			[
				SNew(STextBlock)
				.Text(FText::FromString(LogEntry.Get()->PluginPath))
				.ColorAndOpacity(FLinearColor(0.25f, 0.25f, 0.25f, 1.f))
				//.Font(fontinfo)
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(1, 1))
			]
		];
}

TSharedRef<SWidget> FPakCreatorWindow::GenerateComboBoxWidget(TSharedPtr<FString> Item)
{
	return
		SNew(STextBlock)
		.Text_Lambda([this, Item]()
		{
			return FText::FromString(*Item);
	})
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
		.Font(FAppStyle::GetFontStyle(TEXT("PropertyWindow.NormalFont")))
#else
		.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
#endif
		;
}

void FPakCreatorWindow::OnPathTextCommitted(const FText& InText, const ETextCommit::Type InTextAction)
{
	OutputPath = InText.ToString();

	if (GConfig != nullptr)
	{
		GConfig->SetString(
			TEXT("PakCreator.Core"),
			TEXT("OutputPath"),
			*OutputPath,
			GEditorPerProjectIni
		);

		GConfig->Flush(false, GEditorPerProjectIni);
	}
}

void FPakCreatorWindow::OnPlatformSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	PlatformSelectionTextBlock->SetText(FText::FromString(**SelectedItem));
}

void FPakCreatorWindow::OnProjectFileCommitted(const FText& InText, const ETextCommit::Type InTextAction)
{
	OutputProject = InText.ToString();
}

void FPakCreatorWindow::OnReleaseNameCommitted(const FText& InText, const ETextCommit::Type InTextAction)
{
	OutputRelease = InText.ToString();

	if (GConfig != nullptr)
	{
		GConfig->SetString(
			TEXT("PakCreator.Core"),
			TEXT("ReleaseGameName"),
			*OutputRelease,
			GEditorPerProjectIni
		);

		GConfig->Flush(false, GEditorPerProjectIni);
	}
}

#undef LOCTEXT_NAMESPACE
