// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AIController.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



	class UBehaviorTreeComponent* Comp;
};
