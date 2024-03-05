// Copyright (C) 2023 Blue Mountains GmbH. All Rights Reserved.

#include "UATProcess.h"
#include "Misc/Paths.h"
#include "Async/Async.h"

FUATProcess::FUATProcess()
	: bThreadRunning(false)
	, bCanceling(false)
	, bHidden(true)
	, ReadPipe(nullptr)
	, WritePipe(nullptr)
	, Thread(nullptr)
{
}

FUATProcess::~FUATProcess()
{
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
}

bool FUATProcess::Launch(const FString& URL, const FString& WorkingDirectory, const FString& Params)
{
	if (IsRunning())
	{
		return false;
	}

	if (!FPlatformProcess::CreatePipe(ReadPipe, WritePipe))
	{
		return false;
	}

	// Launch the builder and monitor its process
	ProcessHandle = FPlatformProcess::CreateProc(*URL, *Params, false, bHidden, bHidden, nullptr, 0, *WorkingDirectory, WritePipe);

	if (!ProcessHandle.IsValid())
	{
		return false;
	}

	// Creating name for the process
	static uint32 tempInteractiveProcessIndex = 0;
	FString ThreadName = FString::Printf(TEXT("FUATProcess %d"), tempInteractiveProcessIndex);
	tempInteractiveProcessIndex++;

	Thread = FRunnableThread::Create(this, *ThreadName);

	return Thread != nullptr;
}

void FUATProcess::ProcessOutput(const FString& Output)
{
	TArray<FString> LogLines;

	Output.ParseIntoArray(LogLines, TEXT("\n"), false);

	for (int32 LogIndex = 0; LogIndex < LogLines.Num(); ++LogIndex)
	{
		OutputDelegate.ExecuteIfBound(LogLines[LogIndex], false);
	}
}

// FRunnable interface
uint32 FUATProcess::Run()
{
	bThreadRunning = true;

	bool bIsProcRunning = false;
	do
	{
		// 1 millisecond sleep is a problem on Windows platform, dont change this
		FPlatformProcess::Sleep(0.0f);

		// Read pipe
		ProcessOutput(FPlatformProcess::ReadPipe(ReadPipe));

		bIsProcRunning = FPlatformProcess::IsProcRunning(ProcessHandle);
	}
	while (!bCanceling && bThreadRunning && bIsProcRunning);

	int32 Code = 0;
	FPlatformProcess::GetProcReturnCode(ProcessHandle, &Code);

	FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
	ReadPipe = WritePipe = nullptr;

	if (bCanceling == false)
	{
		TerminatedDelegate.ExecuteIfBound(Code);
	}

	// Ensure process is killed when this thread exists
	FPlatformProcess::CloseProc(ProcessHandle);

	bThreadRunning = false;

	return 0;
}

void FUATProcess::GetUATExecutable(FString& OutExecutablePath, FString& OutExecutable)
{
	// UAT executable
	OutExecutablePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::EngineDir(), TEXT("Build"), TEXT("BatchFiles")));
#if PLATFORM_MAC
	OutExecutable = TEXT("RunUAT.command");
#elif PLATFORM_LINUX
	OutExecutable = TEXT("RunUAT.sh");
#else
	OutExecutable = TEXT("RunUAT.bat");
#endif
}
