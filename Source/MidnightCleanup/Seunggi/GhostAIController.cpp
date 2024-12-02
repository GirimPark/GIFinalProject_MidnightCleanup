// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "BehaviorTree\BehaviorTreeComponent.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "BehaviorTree\BTTaskNode.h"


AGhostAIController::AGhostAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComponent"));
}



void AGhostAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (Result.Code == EPathFollowingResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("MoveToActor succeeded"));
        if (BTComponent)
        {
            Cast<UBTTaskNode>(BTComponent->GetActiveNode())->FinishLatentTask(*BTComponent, EBTNodeResult::Succeeded);
        }
    }
    else  if (Result.Code == EPathFollowingResult::Blocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveToActor Blocked during move"));
        if (BTComponent)
        {
            Cast<UBTTaskNode>(BTComponent->GetActiveNode())->FinishLatentTask(*BTComponent, EBTNodeResult::Failed);
        }
    }
    else  if (Result.Code == EPathFollowingResult::OffPath)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveToActor OffPath during move"));
        if (BTComponent)
        {
            Cast<UBTTaskNode>(BTComponent->GetActiveNode())->FinishLatentTask(*BTComponent, EBTNodeResult::Failed);
        }
    }
    else  if (Result.Code == EPathFollowingResult::Aborted)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveToActor Aborted during move"));
        if (BTComponent)
        {
            Cast<UBTTaskNode>(BTComponent->GetActiveNode())->FinishLatentTask(*BTComponent, EBTNodeResult::Failed);
        }
    }
    else  if (Result.Code == EPathFollowingResult::Invalid)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveToActor Invalid during move"));
        if (BTComponent)
        {
            Cast<UBTTaskNode>(BTComponent->GetActiveNode())->FinishLatentTask(*BTComponent, EBTNodeResult::Failed);
        }
    }
}
