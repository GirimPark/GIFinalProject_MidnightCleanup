// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Interaction.h"
#include "BasicGhost.h"
#include "AIController.h"
#include "BasicInteractionObject.h"
#include "BehaviorTree\BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Interaction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		ABasicGhost* Ghost = AIC->GetPawn<ABasicGhost>();
		if (Ghost)
		{
			ABasicInteractionObject* Object = Cast<ABasicInteractionObject>(Ghost->Target);
			if (Object)
			{
				Object->InterAction(Ghost);
				UE_LOG(LogTemp,Warning,TEXT("Fucking interaction"))
			}
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
