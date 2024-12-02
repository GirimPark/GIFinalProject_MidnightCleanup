// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	FString GetUserID();
	
	UFUNCTION(BlueprintCallable)
	void SetUserID(FString NewUserID);

	UFUNCTION(BlueprintCallable)
	FString GetRoomCode();

	UFUNCTION(BlueprintCallable)
	void SetRoomCode(FString Roomcode);

	UFUNCTION(Server, Reliable)
	void C2S_SendUserInfo();
	void C2S_SendUserInfo_Implementation();

	UFUNCTION(BlueprintCallable)
	bool IsLogoPlayed() const;

	UFUNCTION(BlueprintCallable)
	void SetLogoPlayed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Cash = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bDrawDebug : 1;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Setting",meta = (ClampMin = 70.0f,ClampMax = 110.0f))
	float FOVValue = 90.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ClampMin = 0.0f, ClampMax = 6.0f))
	float BrightValue=3.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ClampMin = 0.1f, ClampMax = 2.0f))
	float MouseSensitivityValue=1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	uint8 YAxisCheck : 1;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserID;

	void InitWindowSetting();

private:
	FString RoomCode;

	uint8 bLogoPlayed : 1 = false;
};
