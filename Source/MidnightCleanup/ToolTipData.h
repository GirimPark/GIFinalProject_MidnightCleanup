// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ToolTipData.generated.h"

/**
 * 
 */
class MIDNIGHTCLEANUP_API ToolTipData
{
public:
	ToolTipData();
	~ToolTipData();
};

USTRUCT(BlueprintType)
struct FToolTipData : public FTableRowBase 
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
    FText Script; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
    int32 Priority = 100; 
};
