// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToTarget.h"

#include "BasicGhost.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GhostAIController.h"
#include "BehaviorTree\BlackboardComponent.h"

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Comp = &OwnerComp;
	AGhostAIController* AIC = Cast<AGhostAIController>(OwnerComp.GetOwner());
	if(AIC)
	{
		ABasicGhost* Ghost = AIC->GetPawn<ABasicGhost>();
		if (Ghost)
		{
			FVector Target = Ghost->MoveLocation;
			EPathFollowingRequestResult::Type Result = AIC->MoveToLocation(Target, 100);
			if (Result == EPathFollowingRequestResult::Failed)
			{
				return EBTNodeResult::Failed;
			}
			else if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				UE_LOG(LogTemp, Warning, TEXT("Move Success"));
				return EBTNodeResult::Succeeded;
			}
			else if (Result == EPathFollowingRequestResult::RequestSuccessful)
			{
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}

