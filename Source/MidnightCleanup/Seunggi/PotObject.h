// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props.h"
#include "PotObject.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API APotObject : public AProps
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	uint8 bActivate : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATextureDecal> DecalClass;

public:
	virtual void BeginPlay()override;

	virtual void InterAction(APawn* Character)override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void C2S_StopEvent();
	void C2S_StopEvent_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_StopEvent();
	void S2A_StopEvent_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_StartEvent();
	void C2S_StartEvent_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_StartEvent();
	void S2A_StartEvent_Implementation();

	void CreateDecal(FVector HitLocation,FRotator HitRotator);

	UFUNCTION()
	void HitProcess(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
