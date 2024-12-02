// Fill out your copyright notice in the Description page of Project Settings.


#include "EventWrapper.h"

void UEventWrapper::CreateEvent()
{
	Event = FPlatformProcess::GetSynchEventFromPool();
}

void UEventWrapper::Trigger()
{
	Event->Trigger();
}

void UEventWrapper::Wait()
{
	Event->Wait();
}
