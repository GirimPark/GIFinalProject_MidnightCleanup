// Fill out your copyright notice in the Description page of Project Settings.


#include "InitGameGM.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "InGamePS.h"

void AInitGameGM::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bUseSeamlessTravel = true;
}

void AInitGameGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
    if (AvailableCharacters.Num() > 0 && NewPlayer)
    {
        TArray<AActor*> PlayerStarts;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

        if (PlayerStarts.Num() > 0)
        {
			if (Index < PlayerStarts.Num())
			{
				// Choose the first PlayerStart for spawning (can be customized)
				APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[Index]);

				if (PlayerStart)
				{
					FActorSpawnParameters SpawnParam;
					SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					// Get the PlayerStart transform (location and rotation)
					FTransform SpawnTransform = PlayerStart->GetActorTransform();

					// Spawn the character at the PlayerStart location
					UE_LOG(LogTemp, Warning, TEXT("Post Login"));
					int32 RandomIndex = FMath::RandRange(0, AvailableCharacters.Num() - 1);
					TSubclassOf<APlayerCharacter> SelectedCharacter = AvailableCharacters[RandomIndex];

					// Assign the selected character to the player
					NewPlayer->GetPawn()->Destroy(); // If they already have a pawn
					APlayerCharacter* NewCharacter = GetWorld()->SpawnActor<APlayerCharacter>(SelectedCharacter, SpawnTransform, SpawnParam);
					NewPlayer->Possess(NewCharacter);

					AInGamePS* PS = NewPlayer->GetPlayerState<AInGamePS>();
					if (PS)
					{
						PS->PlayerCharacter = SelectedCharacter;
					}

					// Remove the selected character from the available pool
					AvailableCharacters.RemoveAt(RandomIndex);
				}

				Index++;

				if (Index >= 4)
				{
					Index = 0;
				}
			}   
        }
    }
}

void AInitGameGM::HandleSeamlessTravelPlayer(AController*& C)
{
    Super::HandleSeamlessTravelPlayer(C);

	APlayerController* NewPlayer = Cast<APlayerController>(C);
	if (NewPlayer)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

		if (PlayerStarts.Num() > 0)
		{
			if (Index < PlayerStarts.Num())
				// Choose the first PlayerStart for spawning (can be customized)
			{
				APlayerStart* PlayerStart = Cast<APlayerStart>(PlayerStarts[Index]);

				if (PlayerStart)
				{
					FActorSpawnParameters SpawnParam;
					SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					// Get the PlayerStart transform (location and rotation)
					FTransform SpawnTransform = PlayerStart->GetActorTransform();

					// Spawn the character at the PlayerStart location
					//UE_LOG(LogTemp, Warning, TEXT("Post Login"));


					AInGamePS* PS = NewPlayer->GetPlayerState<AInGamePS>();
					if (PS)
					{
						TSubclassOf<APlayerCharacter> SelectedCharacter = PS->PlayerCharacter;
						if (SelectedCharacter)
						{
							// Assign the selected character to the player
							NewPlayer->GetPawn()->Destroy(); // If they already have a pawn
							APlayerCharacter* NewCharacter = GetWorld()->SpawnActor<APlayerCharacter>(SelectedCharacter, SpawnTransform, SpawnParam);
							NewPlayer->Possess(NewCharacter);
						}
					}
				}
				Index++;

				if (Index >= 4)
				{
					Index = 0;
				}
			}
		}
	}
}

void AInitGameGM::OpenStage1()
{
	GetWorld()->ServerTravel(StageName1);
}

void AInitGameGM::OpenStage2()
{
	GetWorld()->ServerTravel(StageName2);
}

void AInitGameGM::OpenInGame(FString OpenLevelName)
{
	//UE_LOG(LogTemp, Error, TEXT("AInitGameGM::OpenInGame : %s"), *OpenLevelName);

	GetWorld()->ServerTravel(OpenLevelName);
}
