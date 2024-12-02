// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		ABasicGhost* Ghost = AIC->GetPawn<ABasicGhost>();
		Ghost->State = TargetState;
		AIC->GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), (uint8)TargetState);
	}
	
	return EBTNodeResult::Type();
}
