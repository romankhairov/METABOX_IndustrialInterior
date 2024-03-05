// Copyright (C) 2023 Blue Mountains GmbH. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Misc/Timespan.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

// Delegates
DECLARE_DELEGATE_TwoParams(FOnInteractiveProcessOutput, const FString&, bool)
DECLARE_DELEGATE_OneParam(FOnExitProcessOutput, int32)

/**
* Implements an external process that can be interacted.
*/
class PAKCREATOR_API FUATProcess : public FRunnable
{
public:

	FUATProcess();
	~FUATProcess();

	// Is this process still running?
	bool IsRunning() const
	{
		return bThreadRunning;
	}

	void Cancel()
	{
		bCanceling = true;
	}

	// Launch the process and create the thread.
	bool Launch(const FString& InURL, const FString& InWorkingDirectory, const FString& InParams);

	// Called when the program ends naturally.
	FOnExitProcessOutput& OnTerminated()
	{
		return TerminatedDelegate;
	}

	// Logging delegate.
	FOnInteractiveProcessOutput& OnOutput()
	{
		return OutputDelegate;
	}

	// FRunnable interface
	virtual bool Init() override
	{
		return true;
	}

	virtual uint32 Run() override;

	/*virtual void Stop() override
	{
		Cancel();
	}*/

	virtual void Exit() override { }

	static void GetUATExecutable(FString& OutExecutablePath, FString& OutExecutable);

protected:

	// Pass the log file back to us.
	void ProcessOutput(const FString& Output);

private:

	// Whether this thread is running
	FThreadSafeBool bThreadRunning;

	// Whether the process is being canceled.
	FThreadSafeBool bCanceling;

	// Whether the window of the process should be hidden.
	bool bHidden : 1;

	// Holds the handle to the process.
	FProcHandle ProcessHandle;

	// Holds the read pipe.
	void* ReadPipe;

	// Holds the write pipe.
	void* WritePipe;

	// Holds the monitoring thread object.
	FRunnableThread* Thread;

	// Message to be written to pipe when ready.
	FString MessageToProcess;

	// Holds a delegate that is executed when the process has been canceled.
	FSimpleDelegate CanceledDelegate;

	// Holds a delegate that is executed when the process has been canceled.
	FOnExitProcessOutput TerminatedDelegate;

	// Holds a delegate that is executed when a interactive process produces output.
	FOnInteractiveProcessOutput OutputDelegate;
};
