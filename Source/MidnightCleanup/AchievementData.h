// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AchievementData.generated.h"

/**
 *
 */
class MIDNIGHTCLEANUP_API AchievementData
{
public:
	AchievementData();
	~AchievementData();
};

USTRUCT(BlueprintType)
struct FAchievementData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
	FText Script;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
	int32 Minimum = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
	int32 Priority = 100;

	bool operator==(const FAchievementData& Other) const
	{
		return (Script.ToString() == Other.Script.ToString() && Minimum && Other.Minimum) && Priority == Other.Priority;
	}
};