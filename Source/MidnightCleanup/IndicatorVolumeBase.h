// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IndicatorVolumeBase.generated.h"

UENUM(BlueprintType)
enum class EIndicatorType : uint8
{
	IndicatorType_Trash = 0 UMETA(DisplayName = "Trash"),
	IndicatorType_Water = 1 UMETA(DisplayName = "Water"),
	IndicatorType_Wood = 2 UMETA(DisplayName = "Wood")
};

UCLASS()
class MIDNIGHTCLEANUP_API AIndicatorVolumeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIndicatorVolumeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIndicatorType IndicatorType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsActivate : 1 = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowIndicator(class APlayerCharacter* InPlayer);

	UFUNCTION(BlueprintImplementableEvent)
	void HideIndicator();

	UFUNCTION(BlueprintImplementableEvent)
	void CheckIndicator(bool State);

	UFUNCTION()
	void IndicatorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void IndicatorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsVisibleWidget();
};
