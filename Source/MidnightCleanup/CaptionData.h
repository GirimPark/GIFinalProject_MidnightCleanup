// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CaptionData.generated.h"

/**
 * 
 */
class MIDNIGHTCLEANUP_API CaptionData
{
public:
	CaptionData();
	~CaptionData();
};

USTRUCT(BlueprintType)
struct FCaptionData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MultiLine = "true"), Category = "Caption")
    FText Script;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption")
    int32 Priority = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Caption")
    FString SFXPath = TEXT("None");
};