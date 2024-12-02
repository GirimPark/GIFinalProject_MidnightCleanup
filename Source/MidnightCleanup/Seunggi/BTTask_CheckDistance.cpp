// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "AIController.h"
#include "BasicGhost.h"

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		FVector Target = AIC->GetBlackboardComponent()->GetValueAsVector(TEXT("Target"));
		ABasicGhost* Ghost = AIC->GetPawn<ABasicGhost>();
		if (Ghost)
		{
			float Distance = FVector::Distance(Ghost->GetActorLocation(),
				Target);

			switch (TargetCondition)
			{
			case ECondition::LessThen: // <
			{
				if (Distance < TargetDistance)
				{
					AIC->GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"),(uint8)TargetState);
				}
			}
			break;

			case ECondition::GreaterThen: // >
			{
				if (Distance > TargetDistance)
				{
					AIC->GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), (uint8)TargetState);
				}
			}
			break;
			}
		}
	}


	return EBTNodeResult::Succeeded;
}
