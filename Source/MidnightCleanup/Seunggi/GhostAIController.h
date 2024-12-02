// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation\PathFollowingComponent.h"
#include "GhostAIController.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
public:
	AGhostAIController();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UBehaviorTreeComponent> BTComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<class UBlackboardComponent> BBComponent;

	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)override;
};
