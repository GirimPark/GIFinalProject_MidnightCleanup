// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPatrolLocation.h"
#include "BasicGhost.h"
#include "AIController.h"
#include "Components\SplineComponent.h"

EBTNodeResult::Type UBTTask_GetPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if(AIC)
	{
		ABasicGhost* Ghost = Cast<ABasicGhost>(AIC->GetPawn());
		if(Ghost)
		{
			uint32 MaxIndex = Ghost->PatrolPoints->GetNumberOfSplinePoints();
			if (Ghost->PatrolIndex == MaxIndex)
			{
				Ghost->PatrolIndex = 0;
				Ghost->MoveLocation = Ghost->PatrolPoints->GetLocationAtSplinePoint(Ghost->PatrolIndex, ESplineCoordinateSpace::World);
				return EBTNodeResult::Succeeded;
			}
			else if(Ghost->PatrolIndex < MaxIndex)
			{
				Ghost->MoveLocation = Ghost->PatrolPoints->GetLocationAtSplinePoint(Ghost->PatrolIndex, ESplineCoordinateSpace::World);
				Ghost->PatrolIndex++;
				return EBTNodeResult::Succeeded;
			}
		}

	}

	return EBTNodeResult::Failed;
}
