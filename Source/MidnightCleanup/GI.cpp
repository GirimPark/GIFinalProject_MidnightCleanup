// Fill out your copyright notice in the Description page of Project Settings.


#include "GI.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Seunggi/InGameGM.h"
#include "Kismet/GameplayStatics.h"

void UGI::Init()
{
	Super::Init();

	YAxisCheck = false;
#if UE_BUILD_SHIPPING
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UGI::InitWindowSetting, 0.1f, false);
#endif
}

void UGI::C2S_SendUserInfo_Implementation()
{
	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->UserName.Emplace(UserID);
	}
}

bool UGI::IsLogoPlayed() const
{
	return bLogoPlayed;
}

void UGI::SetLogoPlayed()
{
	bLogoPlayed = true;
}


FString UGI::GetUserID()
{
	return UserID;
}

void UGI::SetUserID(FString NewUserID)
{
	UserID = NewUserID;
}

FString UGI::GetRoomCode()
{
	return RoomCode;
}

void UGI::SetRoomCode(FString Roomcode)
{
	RoomCode = Roomcode;
}

void UGI::InitWindowSetting()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		Settings->ApplySettings(false);
	}
}
