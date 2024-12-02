// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components\SkeletalMeshComponent.h"
#include "Components\CapsuleComponent.h"
#include "Components\WidgetInteractionComponent.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "PC.h"
#include "InteractiveObject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Seunggi\BasicInteractionObject.h"
#include "Seunggi\InteractionPickUpObject.h"
#include "Mop.h"
#include "GI.h"
#include "InteractionBox.h"
#include "InteractionTrash.h"
#include "Flashlight.h"
#include "Seunggi/InteractionDoor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Seunggi/LargePickUpObject.h"
#include "Sponge.h"
#include "OpacityDecal.h"	
#include "TextureDecal.h"	
#include "Algo/Sort.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "InteractionFurniture.h"
#include "InteractionBucket.h"
#include "Seunggi/InteractionFaucet.h"
#include "InteractionKeypad.h"
#include "Seunggi/InGameGM.h"
#include "Seunggi\ObjectOrganize.h"
#include "InitGamePC.h"
#include "Seunggi/InteractionSwitch.h"
#include "Axe.h"
#include "Seunggi\RadioObject.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "InGamePS.h"
#include "Seunggi/InGameGS.h"
#include "InteractionComputer.h"
#include "GRNetworkSubsystem.h"
#include "InteractionMonitor.h"
#include "Seunggi\PotObject.h"
#include "PlayerAnimInstance.h"
#include "IndicatorVolumeBase.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()), FRotator(0, -90.f, 0));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(RootComponent);

	CameraSocket = CreateDefaultSubobject<USceneComponent>(TEXT("CameraSocket"));
	CameraSocket->SetupAttachment(Camera);

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMesh->SetupAttachment(Camera);


	GetCharacterMovement()->SetCrouchedHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());

	//GetCharacterMovement()->SetIsReplicated(true);

	Sight = CreateDefaultSubobject<USceneComponent>(TEXT("Sight"));
	Sight->SetupAttachment(RootComponent);

	PreviousMousePos = FVector2D::ZeroVector;
	CurrentMousePos = FVector2D::ZeroVector;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	Trace3DWidget = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Trace3DWidget"));

	if (Trace3DWidget)
	{
		Trace3DWidget->SetupAttachment(Camera);
		Trace3DWidget->InteractionDistance = 300.f;
		Trace3DWidget->bShowDebug = false;
		Trace3DWidget->SetAutoActivate(false);
		Trace3DWidget->Deactivate();
		Trace3DWidget->bEnableHitTesting = false;
	}

	FPSWatch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FPSWatch"));
	TPSWatch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TPSWatch"));

	FPSFlashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FPSFlashlight"));
	if (FPSFlashlight)
	{
		FPSFlashlight->SetupAttachment(Camera);
		FPSFlashlight->SetIntensity(0.f);
	}

	TPSFlashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("TPSFlashlight"));
	
	Inventory.SetNum(4);
	CurrentInventoryIndex = 0;
	PreviousInventoryIndex = Inventory.Num() - 1;

	ProximityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProximityBox"));
	if (ProximityBox)
	{
		ProximityBox->SetupAttachment(RootComponent);
		ProximityBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlapProximityBox);
		ProximityBox->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::EndOverlapProximityBox);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TPSFlashlight)
	{
		TPSFlashlight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("TPSFlashlight_OnHand"));
		TPSFlashlight->SetIntensity(0.f);
	}

	if (FPSWatch)
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);
		FPSWatch->AttachToComponent(FPSMesh, AttachmentRules, FName("Watch"));
	}

	if (TPSWatch)
	{
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);
		TPSWatch->AttachToComponent(GetMesh(), AttachmentRules, FName("Watch"));
	}
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (IsLocallyControlled())
	{
		InitCameraRelativeLocation = Camera->GetRelativeLocation();

		

		GetWorldTimerManager().SetTimer(CheckPlayerStateHandle, this, &APlayerCharacter::CheckPlayerState, 1.0f, false);

		FPSMesh->SetVisibility(true);
		GetMesh()->SetVisibility(false);

		FPSWatch->SetVisibility(true);
		TPSWatch->SetVisibility(false);

		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);
			GrabbedObjectLocation = TraceEnd;

			APC* InGamePC = Cast<APC>(PC);
			if (InGamePC)
			{
				AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
				if (GS)
				{
					CurrentToolTipData = *GS->GetToolTipByID(FName(TEXT("Inventory")));

					InGamePC->ShowToolTipWidget();
					InGamePC->UpdateToolTip(CurrentToolTipData.Script);

					CurrentCaptionData = FCaptionData();
					UpdateCaption(&CurrentCaptionData, 1.0f);
				}
			}
		}
	}
	else
	{
		FPSMesh->SetVisibility(false);
		GetMesh()->SetVisibility(true);

		FPSWatch->SetVisibility(false);
		TPSWatch->SetVisibility(true);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimWidget();

	if (HighlightedActor)
	{
		HighlightedActor->DrawOutline(false);
	}
	HighlightedActor = nullptr;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsLocallyControlled())
	{
		GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 0, 0), GetActorRotation(), true);
		GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, 0, 0), GetActorRotation(), true);

		if (PC)
		{
			APC* TempPC = Cast<APC>(PC);
			if (TempPC)
			{
				TempPC->HideFurnitureProgressWidget();
			}

			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);
			
			if (PhysicsHandle->GrabbedComponent)
			{
				float Speed = 10.0f;
				AInteractionPickUpObject* IPO = Cast<AInteractionPickUpObject>(PhysicsHandle->GrabbedComponent->GetOwner());
			
				FVector StartLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();
				FVector TargetLocation = TraceStart + (CrosshairWorldDirection * IPO->PhysicsHandleDistance);
				
				float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - TargetLocation).Length();
			
				FVector2D TargetLocation2D = FVector2D((PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - GetActorLocation()).X, (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - GetActorLocation()).Y);
				//if (TargetLocation2D.Length() <= 80)

				TargetLocation2D = FVector2D(CrosshairWorldDirection.X, CrosshairWorldDirection.Y);
				TargetLocation2D.Normalize();
				TargetLocation2D = TargetLocation2D * DistanceLength;
				//TargetLocation = TraceStart + FVector(TargetLocation2D.X, TargetLocation2D.Y, (CrosshairWorldDirection *	DistanceLength).Z);

				FVector ResultLocation = FMath::VInterpTo(StartLocation, TargetLocation, DeltaTime, Speed);

				C2S_SetObjectLocation(TargetLocation);
				
			
				FTransform Transform = FTransform(FRotator::ZeroRotator) * GetTransform();
				FRotator ResultRotation = FMath::RInterpTo(PhysicsHandle->GrabbedComponent->GetOwner()->GetActorRotation(), Transform.Rotator(), DeltaTime, Speed);
				C2S_SetObjectRotation(Transform.Rotator());
			}
			TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			if(Cast<ASponge>(OwnedActor))
				Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

			bool Result;
			FHitResult OutHit;
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (GI->bDrawDebug)
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
			}
			else
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
			}

			if (Result)
			{
				// InteractiveObject
				ABasicInteractionObject* Object = Cast<ABasicInteractionObject>(OutHit.GetActor());
				if (Object)
				{
					if (TempPC)
					{
						AInteractionKeypad* Keypad = Cast<AInteractionKeypad>(Object);
						if (Keypad)
						{
							TempPC->ShowFinishWorkProgressBarWidget();
							HighlightedActor = Keypad;
						}
						else
						{
							TempPC->HideFinishWorkProgressBarWidget();
						}
					}
					
					if (bIsHeld)
					{
						AInteractionDoor* Door = Cast<AInteractionDoor>(Object);
						if (Door)
						{
							HighlightedActor = Door;
						}

						AInteractionSwitch* Switch = Cast<AInteractionSwitch>(Object);
						if (Switch)
						{
							HighlightedActor = Switch;
						}
			
						if (Cast<AObjectOrganize>(Object))
						{
							if (HighlightedActor != nullptr && HighlightedActor != Object)
							{
								HighlightedActor->DrawOutline(false);
							}
							HighlightedActor = Object;
						}
			
						ABasicTool* Tool = Cast<ABasicTool>(OwnedActor);
						if (Tool)
						{
							switch (Tool->GetToolType())
							{
							case EToolType::ToolType_Mop:
							{
								if (Object->ObjectType == EObjectType::ObjectType_PickUp)
								{
									AInteractionPickUpObject* PickUpObject = Cast<AInteractionPickUpObject>(Object);
									if (PickUpObject && PickUpObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Bucket)
									{
										if (HighlightedActor != nullptr && HighlightedActor != Object)
										{
											HighlightedActor->DrawOutline(false);
										}
										HighlightedActor = Object;
									}
								}
								else
								{
									AInteractionDecal* Decal = Cast<AInteractionDecal>(Object);
									if (Decal)
									{
										HighlightedActor = Decal;
									}
								}
							}
							break;
							case EToolType::ToolType_Axe:
							{
								if (Object->ObjectType == EObjectType::ObjectType_Furniture)
								{
									if (TempPC)
									{
										TempPC->ShowFurnitureProgressWidget();
			
										AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(Object);
										TempPC->UpdateFurnitureProgressBar(Furniture->MaxHp, Furniture->CurrentHp);
									}
			
									if (HighlightedActor != nullptr && HighlightedActor != Object)
									{
										HighlightedActor->DrawOutline(false);
									}
									HighlightedActor = Object;
								}
							}
							break;
							case EToolType::ToolType_Sponge:
							{
								if (Object->ObjectType == EObjectType::ObjectType_PickUp)
								{
									AInteractionPickUpObject* PickUpObject = Cast<AInteractionPickUpObject>(Object);
									if (PickUpObject && PickUpObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Bucket)
									{
										if (HighlightedActor != nullptr && HighlightedActor != Object)
										{
											HighlightedActor->DrawOutline(false);
										}
										HighlightedActor = Object;
									}
								}
								else
								{
									AOpacityDecal* Decal = Cast<AOpacityDecal>(Object);
									if (Decal)
									{
										HighlightedActor = Decal;
									}
								}
							}
							break;
							}
						}
						else if (Cast<AInteractionBucket>(OwnedActor))
						{
							AInteractionFaucet* Faucet = Cast<AInteractionFaucet>(Object);
							if (Faucet)
							{
								if (HighlightedActor != nullptr && HighlightedActor != Object)
								{
									HighlightedActor->DrawOutline(false);
								}
								HighlightedActor = Object;
							}
						}
					}
					else
					{
						AInteractionFaucet* Faucet = Cast<AInteractionFaucet>(Object);
						if (Faucet)
						{
							return;
						}
						if (HighlightedActor != nullptr && HighlightedActor != Object)
						{
							HighlightedActor->DrawOutline(false);
						}
						if (Object->ObjectType != EObjectType::ObjectType_Furniture)
						{
							HighlightedActor = Object;
						}
					}
				}
				else if (Object == nullptr)
				{
					if (HighlightedActor)
					{
						HighlightedActor->DrawOutline(false);
						HighlightedActor = nullptr;
					}
			
					if (TempPC)
					{
						TempPC->HideFinishWorkProgressBarWidget();
					}
					
				}
			
				if (Cast<AInteractionMonitor>(Object))
				{
					if (!Trace3DWidget->IsOverInteractableWidget())
					{
						HighlightedActor = nullptr;
					}
				}
			}

			if (HighlightedActor)
			{
				HighlightedActor->DrawOutline(true);
				HighlightedActor->UpdateToolTip(this);
				
				APC* InGamePC = Cast<APC>(PC);
				if (InGamePC)
				{
					InGamePC->ShowToolTipWidget();
				}
			}
			else
			{
				if (OwnedActor)
				{
					APC* InGamePC = Cast<APC>(PC);
					if (InGamePC)
					{
						InGamePC->ShowToolTipWidget();
					}

					AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
					if (GS)
					{
						AFlashlight* Flashlight = Cast<AFlashlight>(OwnedActor);
						if (Flashlight)
						{
							if (FPSFlashlight->Intensity > 0.f)
							{
								CurrentToolTipData = *GS->GetToolTipByID(FName(TEXT("FlashlightOff")));
							}
							else
							{
								CurrentToolTipData = *GS->GetToolTipByID(FName(TEXT("FlashlightOn")));
							}
						}
						else
						{
							CurrentToolTipData = *GS->GetToolTipByID(FName(TEXT("Drop")));
						}
					}

					UpdateToolTip(&CurrentToolTipData);
				}
				else
				{
					APC* InGamePC = Cast<APC>(PC);
					if (InGamePC)
					{
						InGamePC->HideToolTipWidget();
					}

					CurrentToolTipData = FToolTipData();
					UpdateToolTip(&CurrentToolTipData);
				}
			}

			if (PhysicsHandle->GrabbedComponent && PhysicsHandle->GrabbedComponent->GetOwner())
			{
				ABasicTool* Tool = Cast<ABasicTool>(PhysicsHandle->GrabbedComponent->GetOwner());

				if (Tool && Tool->GetToolType() == EToolType::ToolType_Sponge)
				{
					if (bFixView)
					{
						FVector WorldLocation, WorldDirection;
						PC->DeprojectScreenPositionToWorld(CurrentMousePos.X, CurrentMousePos.Y, WorldLocation, WorldDirection);

						FVector CameraPos;
						FRotator CameraRot;
						PC->GetPlayerViewPoint(CameraPos, CameraRot);
						bool SpongeResult;
						TArray<TEnumAsByte<EObjectTypeQuery>> StaticObjects;
						StaticObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
						
						TraceEnd = CameraPos + (WorldDirection * 400);
						if (GI->bDrawDebug)
						{
							SpongeResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, StaticObjects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
						}
						else
						{
							SpongeResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, StaticObjects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
						}

						if (SpongeResult)
						{
							float ImpactPointLength = (OutHit.ImpactPoint - CameraPos).Length();
							FVector SurfaceNormal = OutHit.ImpactNormal;
							FVector Min, Max;
						
							Tool->StaticMesh->GetLocalBounds(Min, Max);

							FVector TargetLocation = CameraPos + (WorldDirection * ImpactPointLength)- (Max - Min) / 2;
							C2S_SetObjectLocation(TargetLocation);

							FRotator NewRotation = SurfaceNormal.Rotation() + FRotator(0.f, -90.f, 0.f);
							C2S_SetObjectRotation(NewRotation);
						}

					}
					else
					{
						FVector Location = FPSMesh->GetSocketLocation(FName("Sponge"));
						FRotator Rotation = FPSMesh->GetSocketRotation(FName("Sponge"));

						//C2S_SetObjectLocation(Location);
						C2S_SetObjectRotation(Rotation);
					}
				}
			}
		}
	}
}

// Called to bind functionality to inputCurrentPos
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		UIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		UIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpStart);
		UIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::JumpEnd);
		UIC->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Sprint);
		UIC->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
		UIC->BindAction(TapAction, ETriggerEvent::Started, this, &APlayerCharacter::Tab);
		UIC->BindAction(TapAction, ETriggerEvent::Completed, this, &APlayerCharacter::TabEnd);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Started, this, &APlayerCharacter::LeftClickStart);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftClick);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Canceled, this, &APlayerCharacter::LeftClickCanceled);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Ongoing, this, &APlayerCharacter::LeftClickOnGoing);
		UIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &APlayerCharacter::LeftClickEnd);
		UIC->BindAction(InteractiveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction, true);
		//UIC->BindAction(InteractiveAction, ETriggerEvent::Completed, this, &APlayerCharacter::Interaction, false);
		UIC->BindAction(LeftMouseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LeftMouse);
		UIC->BindAction(DropAction, ETriggerEvent::Started, this, &APlayerCharacter::DropObject);
		UIC->BindAction(FlashlightAction, ETriggerEvent::Started, this, &APlayerCharacter::SetFlashlight);
		UIC->BindAction(ToolDurabilityAction, ETriggerEvent::Started, this, &APlayerCharacter::SetToolMaxDurability);
		UIC->BindAction(MouseWheelAction, ETriggerEvent::Started, this, &APlayerCharacter::Wheel);
		UIC->BindAction(EscAction, ETriggerEvent::Started, this, &APlayerCharacter::StartEscAction);
		UIC->BindAction(CleanProgressAction, ETriggerEvent::Started, this, &APlayerCharacter::ShowCleanProgressWidget);
		UIC->BindAction(CleanProgressAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UpdateCleanProgressWidget);
		UIC->BindAction(CleanProgressAction, ETriggerEvent::Completed, this, &APlayerCharacter::HideCleanProgressWidget);
		UIC->BindAction(ModifyPlayTimeAction, ETriggerEvent::Started, this, &APlayerCharacter::EnableModifyPlayTime);
		UIC->BindAction(IncreasePlayTimeAction, ETriggerEvent::Started, this, &APlayerCharacter::ModifyPlayTime, true);
		UIC->BindAction(DecreasePlayTimeAction, ETriggerEvent::Started, this, &APlayerCharacter::ModifyPlayTime, false);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}
	else
	{
		if (InitGamePC->IsEnableLaptopWidget() || InitGamePC->IsEnablePauseWidget())
		{
			return;
		}
	}
	
	if (!bFixView)
	{
		FVector2D Data = Value.Get<FVector2D>() * MouseSensitivity;
		const float MouseThreshold = 0.1f;
		if(FMath::Abs(Data.X)>MouseThreshold||FMath::Abs(Data.Y)>MouseThreshold)
		{
			AddControllerYawInput(Data.X);
			if (YAxisCheck)
			{
				AddControllerPitchInput(Data.Y * -1);
			}
			else
			{
				AddControllerPitchInput(Data.Y);
			}
		}

		if (IsLocallyControlled())
		{
			GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 0, 0), GetActorRotation(), true);
			GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, 0, 0), GetActorRotation(), true);
			if (PC)
			{
				int32 ScreenSizeX;
				int32 ScreenSizeY;
				FVector CrosshairWorldPosition; //3D
				FVector CrosshairWorldDirection; //3D 

				FVector CameraLocation;
				FRotator CameraRotation;

				PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
				PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

				PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

				FVector TraceStart = CameraLocation;
				FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

				if (PhysicsHandle->GrabbedComponent && !bIsObjectTargetLocation)
				{
					float DeltaTime = GetWorld()->GetDeltaSeconds();
					FVector StartLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();


					ALargePickUpObject* LargeObject = Cast<ALargePickUpObject>(PhysicsHandle->GrabbedComponent->GetOwner());
					if (LargeObject)
					{

						FVector CurrentLocation = GetActorLocation();
						FVector MinRange = LargeObject->GetActorLocation() - 200;
						FVector MaxRange = LargeObject->GetActorLocation() + 200;
						FVector GrabLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();
						if (LargeObject->Player1 && LargeObject->Player2)
						{
							TraceEnd = GetActorLocation() + (GetActorForwardVector() * 100);
							FVector NewLocation = (LargeObject->Player1->GetActorLocation() + LargeObject->Player2->GetActorLocation()) / 2;
							NewLocation = NewLocation + PhysicsHandle->GrabbedComponent->GetSocketLocation(PhysicsHandle->GrabbedBoneName);
							float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
							if (Distance > 500)
							{
								DropObject();
							}
							C2S_SetObjectLocation(TraceEnd);
						}
						else //if (LargeObject->Player1)
						{
							TraceEnd = TraceStart + (CrosshairWorldDirection * 200);
							float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();

							if (Distance > 500)
							{
								DropObject();
							}
							C2S_SetObjectLocation(TraceEnd);

						}
					}
					else
					{
						ASponge* Sponge = Cast<ASponge>(PhysicsHandle->GrabbedComponent->GetOwner());
						if (Sponge)
						{
							return;
						}

						// Move the grabbed component to the desired location
						TraceEnd = TraceStart + (CrosshairWorldDirection * DistanceLength);
						float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
						FVector ResultLocation = TraceEnd;
						if (Distance > DistanceLength)
						{
							ResultLocation = FMath::VInterpTo(StartLocation, TraceEnd, DeltaTime, InterpSpeed);
						}
						else
						{
							C2S_SetInterpolationSpeed(300);
							bIsObjectTargetLocation = true;
							//ResultLocation = TraceEnd;
						}
						if (Distance > 500)
						{
							DropObject();
						}
						//TraceEnd = GetMesh()->GetBoneLocation(TEXT("Box"))+GetActorLocation();
						//PhysicsHandle->SetTargetLocation(TraceEnd);
						//C2S_SetObjectLocation(ResultLocation);
						//FTransform Transform = FTransform(FRotator::ZeroRotator) * GetTransform();
						//FRotator ResultRotation = FMath::RInterpTo(PhysicsHandle->GrabbedComponent->GetOwner()-//>GetActorRotation(), Transform.Rotator(), DeltaTime, InterpSpeed);
						//C2S_SetObjectRotation(ResultRotation);
					}
				}
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}
			
			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}
	else
	{
		if (InitGamePC->IsEnableLaptopWidget() || InitGamePC->IsEnablePauseWidget())
		{
			return;
		}
	}

	if (bFixView)
	{
		return;
	}

	FVector2D Data = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator ForwardRotation = FRotator(0, Rotation.Yaw, 0);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ForwardRotation);
	const FVector RightVector = UKismetMathLibrary::GetRightVector(ForwardRotation);
	AddMovementInput(ForwardVector, Data.Y);
	AddMovementInput(RightVector, Data.X);
	
	if (IsLocallyControlled())
	{
		GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 0, 0), GetActorRotation(), true);
		GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, 0, 0), GetActorRotation(), true);
		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

			if (PhysicsHandle->GrabbedComponent && !bIsObjectTargetLocation)
			{
				float DeltaTime = GetWorld()->GetDeltaSeconds();
				FVector StartLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();


				ALargePickUpObject* LargeObject = Cast<ALargePickUpObject>(PhysicsHandle->GrabbedComponent->GetOwner());
				if (LargeObject)
				{

					FVector CurrentLocation = GetActorLocation();
					FVector MinRange = LargeObject->GetActorLocation() - 200;
					FVector MaxRange = LargeObject->GetActorLocation() + 200;
					FVector GrabLocation = PhysicsHandle->GrabbedComponent->GetComponentLocation();
					if (LargeObject->Player1 && LargeObject->Player2)
					{
						TraceEnd = GetActorLocation() + (GetActorForwardVector() * 100);
						FVector NewLocation = (LargeObject->Player1->GetActorLocation() + LargeObject->Player2->GetActorLocation()) / 2;
						NewLocation = NewLocation + PhysicsHandle->GrabbedComponent->GetSocketLocation(PhysicsHandle->GrabbedBoneName);
						float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
						if (Distance > 500)
						{
							DropObject();
						}
						C2S_SetObjectLocation(TraceEnd);
					}
					else //if (LargeObject->Player1)
					{
						TraceEnd = TraceStart + (CrosshairWorldDirection * 200);
						float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();

						if (Distance > 500)
						{
							DropObject();
						}
						C2S_SetObjectLocation(TraceEnd);

					}
				}
				else
				{
					ASponge* Sponge = Cast<ASponge>(PhysicsHandle->GrabbedComponent->GetOwner());
					if (Sponge)
					{
						return;
					}

					// Move the grabbed component to the desired location
					float Distance = (PhysicsHandle->GrabbedComponent->GetOwner()->GetActorLocation() - CameraLocation).Length();
					FVector ResultLocation;
					if (Distance > DistanceLength + 10)
					{
						ResultLocation = FMath::VInterpTo(StartLocation, TraceEnd, DeltaTime, InterpSpeed);
					}
					else
					{
						bIsObjectTargetLocation = true;
						C2S_SetInterpolationSpeed(300);
						//ResultLocation = TraceEnd;
					}
					if (Distance > 500)
					{
						DropObject();
					}
					//TraceEnd = GetMesh()->GetBoneLocation(TEXT("Box"))+GetActorLocation();
					//PhysicsHandle->SetTargetLocation(TraceEnd);
					//C2S_SetObjectLocation(ResultLocation);
					//FTransform Transform = FTransform(FRotator::ZeroRotator) * GetTransform();
					//FRotator ResultRotation = FMath::RInterpTo(PhysicsHandle->GrabbedComponent->GetOwner()-//>GetActorRotation(), Transform.Rotator(), DeltaTime, InterpSpeed);
					//C2S_SetObjectRotation(ResultRotation);

				}
			}
		}
	}
}

void APlayerCharacter::Sprint()
{
	//Client
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	//Server
	C2S_SetSprint(true);
}

void APlayerCharacter::StopSprint()
{
	//Client
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Server
	C2S_SetSprint(false);
}

void APlayerCharacter::LeftMouse(const FInputActionValue& Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ABasicTool* CurrentTool = Cast<ABasicTool>(FPSOwnedActor);

	if (bFixView && CurrentTool)
	{
		if (CurrentTool->GetToolType() == EToolType::ToolType_Sponge)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

			if (PC)
			{
				int32 ScreenSizeX;
				int32 ScreenSizeY;
				FVector CrosshairWorldPosition; //3D
				FVector CrosshairWorldDirection; //3D 

				FVector CameraLocation;
				FRotator CameraRotation;

				PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
				PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

				PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

				FVector TraceStart = CameraLocation;
				FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

				TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

				Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

				FHitResult OutHit;
				bool Result;
				UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI->bDrawDebug)
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}
				else
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}

				ASponge* Sponge = Cast<ASponge>(CurrentTool);
				if (Sponge)
				{
					PC->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);
					if (Sponge->Durability > 0)
					{
						if (Result)
						{
							//AOpacityDecal* Decal = Cast<AOpacityDecal>(OutHit.GetActor());
							AOpacityDecal* Decal = Cast<AOpacityDecal>(Sponge->OverlappedDecal);

							float MouseDistance = FVector2D::Distance(CurrentMousePos, PreviousMousePos);
							if (Decal)
							{
								if (MouseDistance > Decal->MinLegnth)
								{
									if (Decal->CurrentOpacity <= 0)
									{
										C2S_SetErasingState(false);
										C2S_SetCurrentDecal(nullptr);
									}
									else
									{
										C2S_SetCurrentDecal(Decal);
										C2S_DecreaseDurability(Sponge);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		C2S_SetErasingState(false);
	}
}

void APlayerCharacter::SetInventory(float Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* InGamePC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (InGamePC)
	{
		InGamePC->HideToolTipWidget();
	}

	CurrentToolTipData = FToolTipData();
	UpdateToolTip(&CurrentToolTipData);

	C2S_SetInventory(Value);
}

void APlayerCharacter::SetFlashlight()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}

	ABasicTool* CurrentTool = Cast<ABasicTool>(Inventory[CurrentInventoryIndex]);
	if (CurrentTool)
	{
		if (CurrentTool->GetToolType() == EToolType::ToolType_Flashlight)
		{
			AFlashlight* Flashlight = Cast<AFlashlight>(Inventory[CurrentInventoryIndex]);
			if (Flashlight)
			{
				if (Flashlight->bIsActivate)
				{
					C2S_SetFlashlight(Flashlight, false);
				}
				else
				{
					C2S_SetFlashlight(Flashlight, true);
				}
			}
		}
	}
}

void APlayerCharacter::SetToolMaxDurability()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	ABasicTool* Tool = Cast<ABasicTool>(Inventory[CurrentInventoryIndex]);
	C2S_SetToolMaxDurability(Tool);
}

void APlayerCharacter::SortDecalsBySortOrder(TArray<class AActor*>& Decals)
{
	Algo::Sort(Decals, [](AActor* A, AActor* B)
		{
			ATextureDecal* DecalA = Cast<ATextureDecal>(A);
			ATextureDecal* DecalB = Cast<ATextureDecal>(B);
			return DecalA->GetSortOrder() < DecalB->GetSortOrder();
		});
}

void APlayerCharacter::Axe_Hit()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && bIsAxeHeld)
	{
		int32 ScreenSizeX;
		int32 ScreenSizeY;
		FVector CrosshairWorldPosition; //3D
		FVector CrosshairWorldDirection; //3D 

		FVector CameraLocation;
		FRotator CameraRotation;

		PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
		PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector TraceStart = CameraLocation + (CrosshairWorldDirection * 50.f);
		FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);

		TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		bool Result;
		FHitResult OutHit;
		UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GI->bDrawDebug)
		{
			Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
		}
		else
		{
			Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
		}

		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(HighlightedActor);
		if (Furniture)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Furniture Interaction"));
			C2S_Axe_Hit(Furniture);
		}
		else
		{
			AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
			if (GS)
			{
				if (Result && OutHit.GetActor() != this)
				{
					int32 Temp = FMath::RandRange(1, 3);
					FString Key = UKismetStringLibrary::Concat_StrStr(TEXT("WipeAxe_ToPerson_"), FString::FromInt(Temp));
					FCaptionData* Data = GS->GetCaptionByID(FName(Key));
					if (Data)
					{
						UpdateCaption(Data, 1.0f);
					}
				}
				else
				{
					int32 Temp = FMath::RandRange(1, 3);
					FString Key = UKismetStringLibrary::Concat_StrStr(TEXT("WipeAxe_ToProps_"), FString::FromInt(Temp));
					FCaptionData* Data = GS->GetCaptionByID(FName(Key));
					if (Data)
					{
						UpdateCaption(Data, 1.0f);
					}
				}
			}

		}
	}
}

void APlayerCharacter::EndAxeWipe()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (bIsAxeHeld)
	{
		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(HighlightedActor);
		if (Furniture)
		{
			DecreaseFurnitureDurability((float)(Furniture->MaxHp), (float)(Furniture->CurrentHp));
		}
		else
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DecreaseFurnitureDurability(5.0f, 0.0f);
		}
	}
}

void APlayerCharacter::Wheel(const FInputActionValue& Value)
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	float Data = Value.Get<float>();

	SetInventory(Data);

	UE_LOG(LogTemp, Warning, TEXT("Previous : %d, Current : %d"), PreviousInventoryIndex, CurrentInventoryIndex);
}

void APlayerCharacter::EnableKeypad()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!PC)
	{
		return;
	}

	if (!PC->IsEnableFinishWorkProgressBarWidget())
	{
		return;
	}

	if (HighlightedActor)
	{
		AInteractionKeypad* Keypad = Cast<AInteractionKeypad>(HighlightedActor);
		if (Keypad)
		{
			C2S_Interaction(Keypad);
		}
	}
}

void APlayerCharacter::SetCameraLocation(FVector Location)
{
	S2A_SetCameraLocation(false, Location, FRotator(0.f, -90.f, 0.f));
}

void APlayerCharacter::EnableLaptop()
{
	S2C_EnableLaptop();
}

void APlayerCharacter::DisableLaptop()
{
	C2S_DisableLaptop();
}

void APlayerCharacter::UpdateToolTip(FToolTipData* InData)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC && InData)
	{
		PC->UpdateToolTip(InData->Script);
	}

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (InitGamePC && InData)
	{
		InitGamePC->UpdateToolTip(InData->Script);
	}
}

void APlayerCharacter::UpdateCaption(FCaptionData* InData, float Speed)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("InData : %s"), *InData->Script.ToString());

	if (PC->bEnableCaption)
	{
		if (InData && CurrentCaptionData.Priority >= InData->Priority)
		{
			PC->UpdateCaption(InData->Script, Speed, InData->SFXPath);
			CurrentCaptionData = *InData;
		}
	}
	else
	{
		PC->UpdateCaption(InData->Script, Speed, InData->SFXPath);
		CurrentCaptionData = *InData;
	}
}

void APlayerCharacter::ShowCleanProgressWidget()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!PC)
	{
		return;
	}

	if (PC->bFinishGame)
	{
		return;
	}

	if (PC->IsEnablePauseWidget())
	{
		return;
	}

	if (PC->IsEnableInventoryWidget())
	{
		return;
	}
	
	if (!IsLocallyControlled())
	{
		return;
	}

	PC->ShowCleanProgressWidget();
}

void APlayerCharacter::HideCleanProgressWidget()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->HideCleanProgressWidget();
	}
}

void APlayerCharacter::UpdateCleanProgressWidget()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->UpdateCleanProgressWidget();
	}
}

void APlayerCharacter::StartEscAction()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				if (PC->IsEnableRewardWidget())
				{
					PC->OpenInitGame();
				}

				return;
			}

			if (PC->IsEnableInventoryWidget())
			{
				return;
			}

			if (PC->IsEnableCleanProgressWidget())
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				PC->HidePauseWidget();
			}
			else
			{
				PC->ShowPauseWidget();
			}
		}
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	if (InitGamePC)
	{
		if (InitGamePC->IsEnableLaptopWidget())
		{
			InitGamePC->HideLaptopWidget();
		}
		else
		{
			if (InitGamePC->IsEnablePauseWidget())
			{
				InitGamePC->HidePauseWidget();
			}
			else
			{
				InitGamePC->ShowPauseWidget();
			}
		}
	}
}

void APlayerCharacter::BeginOverlapProximityBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(OtherActor);
	if (OtherPlayer && OtherPlayer != this)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS)
		{
			AInGamePS* PS = GetPlayerState<AInGamePS>();
			if (PS)
			{
				PS->UpdateLastOverlapTime(OtherPlayer->GetUserID(), GS->CurrentPlayTime);
			}
		}
	}
}

void APlayerCharacter::EndOverlapProximityBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority())
	{
		return;
	}

	APlayerCharacter* OtherPlayer = Cast<APlayerCharacter>(OtherActor);
	if (OtherPlayer)
	{
		AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS)
		{
			AInGamePS* PS = GetPlayerState<AInGamePS>();
			if (PS)
			{
				PS->UpdateProximityDuration(OtherPlayer->GetUserID(), GS->CurrentPlayTime);
			}
		}
	}
}

void APlayerCharacter::SetSpongeOverlappedDecal(bool EraseState, AOpacityDecal* Decal)
{
	if (IsLocallyControlled())
	{
		C2S_SetSpongeOverlappedDecal(EraseState, Decal);
	}
}

FRotator APlayerCharacter::GetAimOffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void APlayerCharacter::SetToolHeldState(ABasicTool* Tool, bool State)
{
	if (Tool)
	{
		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			bIsMopHeld = State;
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			bIsSpongeHeld = State;
		}
		break;
		case EToolType::ToolType_Axe:
		{
			bIsAxeHeld = State;
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			bIsFlashlightHeld = State;
		}
		break;
		}
	}
}

void APlayerCharacter::SetFlashlightState(AFlashlight* Flashlight, bool State)
{
	if (IsLocallyControlled())
	{
		//UE_LOG(LogTemp, Warning, TEXT("IS LOCALLY CONTROLLED"));
		SetTPSFlashlight(Flashlight, false);
		SetFPSFlashlight(Flashlight, State);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("!IS LOCALLY CONTROLLED"));
		SetFPSFlashlight(Flashlight, false);
		SetTPSFlashlight(Flashlight, State);
	}

	if (Flashlight)
	{
		Flashlight->S2A_SetFlashlightActivate(State);
	}
}

void APlayerCharacter::OnRep_UpdateHeldState()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	if (!bIsHeld)
	{
		PC->UpdateAimIcon(EIconType::IconType_Hand);
	}
}

void APlayerCharacter::UpdateAimWidget()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (!bIsHeld)
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	AInteractionPickUpObject* Object = Inventory[CurrentInventoryIndex];
	if (!Object)
	{
		return;
	}

	switch (Object->PickUpObjectType)
	{
	case EPickUpObjectType::PickUpObjectType_Tool:
	{
		ABasicTool* Tool = Cast<ABasicTool>(Object);
		if (!Tool)
		{
			return;
		}

		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			PC->UpdateAimIcon(EIconType::IconType_Mop);
		}
		break;
		case EToolType::ToolType_Flashlight:
		{
			PC->UpdateAimIcon(EIconType::IconType_Flashlight);
		}
		break;
		case EToolType::ToolType_Axe:
		{
			PC->UpdateAimIcon(EIconType::IconType_Axe);
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			PC->UpdateAimIcon(EIconType::IconType_Sponge);
		}
		break;
		}
	}
	break;
	case EPickUpObjectType::PickUpObjectType_Trash:
	{
		PC->UpdateAimIcon(EIconType::IconType_Trash);
	}
	break;
	case EPickUpObjectType::PickUpObjectType_Box:
	{
		PC->UpdateAimIcon(EIconType::IconType_Box);
	}
	break;
	case EPickUpObjectType::PickUpObjectType_Bucket:
	{
		PC->UpdateAimIcon(EIconType::IconType_Bucket);
	}
	break;
	case EPickUpObjectType::PickUpObjectType_Props:
	{
		PC->UpdateAimIcon(EIconType::IconType_Props);
	}
	break;
	}
}

void APlayerCharacter::UpdateCaptionByCollision(FName CaptionID, float InSpeed)
{
	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	FCaptionData* Data = GS->GetCaptionByID(CaptionID);
	UpdateCaption(Data, InSpeed);
}

void APlayerCharacter::UpdateTPSFlashlightSocket(FName SocketName)
{
	//UE_LOG(LogTemp, Warning, TEXT("UpdateTPSFlashlightSocket"));
	C2S_UpdateTPSFlashlightSocket(SocketName);
}

void APlayerCharacter::EnableModifyPlayTime()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GS)
	{
		return;
	}

	if (GS->CurrentPlayTime <= GS->PlayTime - GS->TutorialTime)
	{
		if (PC->bEnableModifyPlayTime)
		{
			PC->bEnableModifyPlayTime = false;
		}
		else
		{
			PC->bEnableModifyPlayTime = true;
		}
	}
	else
	{
		PC->bEnableModifyPlayTime = false;
	}
}

bool APlayerCharacter::IsLookingAtKeypad()
{
	if (HighlightedActor)
	{
		AInteractionKeypad* Keypad = Cast<AInteractionKeypad>(HighlightedActor);
		if (Keypad)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void APlayerCharacter::ModifyPlayTime(bool State)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	if (!PC->bEnableModifyPlayTime)
	{
		return;
	}

	PC->ModifyPlayTime(State);
}

void APlayerCharacter::CheckIndicator(AIndicatorVolumeBase* InVolume)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (!InVolume)
	{
		return;
	}
	
	if (!OwnedActor)
	{
		if (InVolume->IsVisibleWidget())
		{
			S2C_CheckIndicator(InVolume, false);
		}

		return;
	}

	switch (InVolume->IndicatorType)
	{
	case EIndicatorType::IndicatorType_Trash:
	{
		if (OwnedActor->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Trash || OwnedActor->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Box)
		{
			if (!InVolume->IsVisibleWidget())
			{
				S2C_CheckIndicator(InVolume, true);
			}
		}
		else
		{
			if (InVolume->IsVisibleWidget())
			{
				S2C_CheckIndicator(InVolume, false);
			}
		}
	}
	break;
	case EIndicatorType::IndicatorType_Water:
	{
		if (OwnedActor->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Bucket)
		{
			if (!InVolume->IsVisibleWidget())
			{
				S2C_CheckIndicator(InVolume, true);
			}
		}
		else
		{
			if (InVolume->IsVisibleWidget())
			{
				S2C_CheckIndicator(InVolume, false);
			}
		}
	}
	break;
	case EIndicatorType::IndicatorType_Wood:
	{
		if (OwnedActor->PickUpObjectType == EPickUpObjectType::PickUpObjectType_WoodenPlank)
		{
			if (!InVolume->IsVisibleWidget())
			{
				S2C_CheckIndicator(InVolume, true);
			}
		}
		else
		{
			if (InVolume->IsVisibleWidget())
			{
				S2C_CheckIndicator(InVolume, false);
			}
		}
	}
	break;
	}
}

void APlayerCharacter::S2C_EnableLaptop_Implementation()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ShowLaptopWidget();
	}
}

void APlayerCharacter::C2S_DisableLaptop_Implementation()
{
	TArray<AActor*> Objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractionComputer::StaticClass(), Objects);

	AInteractionComputer* Computer = Cast<AInteractionComputer>(Objects[0]);
	if (Computer)
	{
		Computer->S2A_SetComputerActivateState(false);
	}

	S2A_DisableLaptop();
}

void APlayerCharacter::S2A_DisableLaptop_Implementation()
{
	if (Camera)
	{
		Camera->bUsePawnControlRotation = true;
		Camera->SetRelativeLocation(InitCameraRelativeLocation);
		Camera->SetupAttachment(RootComponent);
	}
}

void APlayerCharacter::S2A_SetJumpState_Implementation(bool State)
{
	bIsJump = State;
}

void APlayerCharacter::C2S_SetFocusFurnitureState_Implementation(bool State)
{
	S2A_SetFocusFurnitureState(State);
}

void APlayerCharacter::S2A_SetFocusFurnitureState_Implementation(bool State)
{
	bFocusFurniture = State;
}

void APlayerCharacter::C2S_SetWashSpongeState_Implementation(bool State)
{
	S2A_SetWashSpongeState(State);
}

void APlayerCharacter::S2A_SetWashSpongeState_Implementation(bool State)
{
	bWashSponge = State;
}

bool APlayerCharacter::IsAttachActor(FName InName)
{
	TArray<USceneComponent*> AttachedActors;
	AttachedActors = GetMesh()->GetAttachChildren();
	if (AttachedActors.Num() > 0)
	{
		for (USceneComponent* Temp : AttachedActors)
		{
			if (Temp->GetAttachSocketName() == InName)
			{
				return true;
			}
		}
	}

	return false;
}

void APlayerCharacter::S2C_HidePreviousTool_Implementation(int32 Index)
{
	//UE_LOG(LogTemp, Warning, TEXT("PREV : %d"), Index);

	if (!IsLocallyControlled())
	{
		return;
	}

	if (Inventory[Index])
	{
		AMop* Mop = Cast<AMop>(Inventory[Index]);
		if (Mop)
		{
			Mop->SkeletalMesh->SetVisibility(false);
		}
		else
		{
			Inventory[Index]->StaticMesh->SetVisibility(false);
		}
	}
}

void APlayerCharacter::C2S_StopEvent_Implementation(ARadioObject* Radio)
{
	Radio->S2A_StopEvent();
}

void APlayerCharacter::WashSpongeEnd()
{
	C2S_SetWashSpongeState(false);
}

void APlayerCharacter::C2S_SetFlashlight_Implementation(AFlashlight* Flashlight, bool State)
{
	Flashlight->S2A_SoundPlay();
	S2A_SetFlashlight(Flashlight, State);
}

void APlayerCharacter::S2A_SetFlashlight_Implementation(AFlashlight* Flashlight, bool State)
{
	SetFlashlightState(Flashlight, State);
}

void APlayerCharacter::SetFPSFlashlight(AFlashlight* Flashlight, bool State)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetFPSFlashlight"));
	if (FPSFlashlight && Flashlight)
	{
		if (State)
		{
			FPSFlashlight->SetIntensity(Flashlight->MaxIntensity);
		}
		else
		{
			FPSFlashlight->SetIntensity(0.f);
		}
	}
}

void APlayerCharacter::SetTPSFlashlight(AFlashlight* Flashlight, bool State)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetTPSFlashlight"));
	if (TPSFlashlight && Flashlight)
	{
		if (State)
		{
			TPSFlashlight->SetIntensity(Flashlight->MaxIntensity);
		}
		else
		{
			TPSFlashlight->SetIntensity(0.f);
		}
	}
}



FString APlayerCharacter::GetUserID()
{
	AInGamePS* PS = Cast<AInGamePS>(GetPlayerState());
	if (PS)
	{
		return PS->UserID;
	}

	return TEXT("None");
}

void APlayerCharacter::CheckPlayerState()
{
	UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		C2S_SetUserID(GI->GetUserID());
	}
}

void APlayerCharacter::C2S_SetUserID_Implementation(const FString& InValue)
{
	S2A_SetUserID(InValue);
}

void APlayerCharacter::S2A_SetUserID_Implementation(const FString& InValue)
{
	AInGamePS* PS = Cast<AInGamePS>(GetPlayerState());

	if (PS)
	{
		PS->UserID = InValue;
	}
}

void APlayerCharacter::SetThumbsUpNum(const FString& InUserID)
{
	C2S_SetThumbsUpNum(InUserID);
}

void APlayerCharacter::C2S_SetThumbsUpNum_Implementation(const FString& InUserID)
{
	S2A_SetThumbsUpNum(InUserID);
}

void APlayerCharacter::S2A_SetThumbsUpNum_Implementation(const FString& InUserID)
{
	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	if (GS)
	{
		for (TObjectPtr<APlayerState> PS : GS->PlayerArray)
		{
			AInGamePS* TempPS = Cast<AInGamePS>(PS);
			if (TempPS && TempPS->UserID == InUserID)
			{
				if (TempPS->ThumbsUpNum < 3)
				{
					TempPS->ThumbsUpNum++;
					//UE_LOG(LogTemp, Warning, TEXT("[%s] ThumbsNum : %d"), *TempPS->UserID, TempPS->ThumbsUpNum);
				}
			}
		}
	}
}

void APlayerCharacter::UpdateEnableTriggerCount()
{
	C2S_UpdateEnableTriggerCount();
}

void APlayerCharacter::C2S_UpdateEnableTriggerCount_Implementation()
{
	AInGamePS* PS = Cast<AInGamePS>(GetPlayerState());
	if (PS)
	{
		PS->EnableTriggerCount++;
		PS->OnRep_UpdateReward();
	}
}

void APlayerCharacter::C2S_UpdateStealCount_Implementation()
{
	AInGamePS* PS = Cast<AInGamePS>(GetPlayerState());
	if (PS)
	{
		PS->StealCount++;
		PS->OnRep_UpdateReward();
	}
}

void APlayerCharacter::C2S_UpdateTPSFlashlightSocket_Implementation(FName SocketName)
{
	S2A_UpdateTPSFlashlightSocket(SocketName);
}

void APlayerCharacter::S2A_UpdateTPSFlashlightSocket_Implementation(FName SocketName)
{
	TPSFlashlight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void APlayerCharacter::S2A_EnableLoadingScreen_Implementation()
{
	AInitGamePC* PC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ShowLoadingWidget();
	}
}

void APlayerCharacter::C2S_SetDisableInteraction_Implementation(bool State)
{
	S2A_SetDisableInteraction(State);
}

void APlayerCharacter::S2A_SetDisableInteraction_Implementation(bool State)
{
	bDisableInteraction = State;
}

void APlayerCharacter::S2C_EnableIndicator_Implementation(AIndicatorVolumeBase* InVolume)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (InVolume)
	{
		InVolume->ShowIndicator(this);
	}
}

void APlayerCharacter::S2C_DisableIndicator_Implementation(AIndicatorVolumeBase* InVolume)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (InVolume)
	{
		InVolume->HideIndicator();
	}
}

void APlayerCharacter::S2C_CheckIndicator_Implementation(AIndicatorVolumeBase* InVolume, bool State)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("S2C_CheckIndicator"));

	if (InVolume)
	{
		InVolume->CheckIndicator(State);
	}
}

void APlayerCharacter::C2S_SetJumpState_Implementation(bool State)
{
	S2A_SetJumpState(State);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, bDisableInteraction);
	DOREPLIFETIME(APlayerCharacter, bIsInteractive);
	DOREPLIFETIME(APlayerCharacter, bIsLeftClickStart);
	DOREPLIFETIME(APlayerCharacter, bIsLeftClicking);
	DOREPLIFETIME(APlayerCharacter, bIsLeftClickOnGoing);
	DOREPLIFETIME(APlayerCharacter, bIsJump);
	DOREPLIFETIME(APlayerCharacter, bIsSprint);
	DOREPLIFETIME(APlayerCharacter, bIsErasing);
	DOREPLIFETIME(APlayerCharacter, bIsHeld);
	DOREPLIFETIME(APlayerCharacter, bIsHeldBox); 
	DOREPLIFETIME(APlayerCharacter, bIsHeldBucket);
	DOREPLIFETIME(APlayerCharacter, bIsHeldTrash);
	DOREPLIFETIME(APlayerCharacter, bIsHeldWoodenPlank);
	DOREPLIFETIME(APlayerCharacter, bIsMopHeld);
	DOREPLIFETIME(APlayerCharacter, bIsAxeHeld);
	DOREPLIFETIME(APlayerCharacter, bIsFlashlightHeld);
	DOREPLIFETIME(APlayerCharacter, bIsSpongeHeld);
	DOREPLIFETIME(APlayerCharacter, bFixView);
	DOREPLIFETIME(APlayerCharacter, bUpdateInventoryByUI);
	DOREPLIFETIME(APlayerCharacter, bIsChangePreviousIndex);
	DOREPLIFETIME(APlayerCharacter, bIsChangeCurrentIndex);
	DOREPLIFETIME(APlayerCharacter, bIsEraseStart);
	DOREPLIFETIME(APlayerCharacter, CurrentInventoryIndex);
	DOREPLIFETIME(APlayerCharacter, PreviousInventoryIndex);
	DOREPLIFETIME(APlayerCharacter, OwnedActor);
	DOREPLIFETIME(APlayerCharacter, FPSOwnedActor);
	DOREPLIFETIME(APlayerCharacter, ActorsToIgnore);
	DOREPLIFETIME(APlayerCharacter, DistanceLength);
	DOREPLIFETIME(APlayerCharacter, bFocusFurniture);
	DOREPLIFETIME(APlayerCharacter, bWashSponge);
	DOREPLIFETIME(APlayerCharacter, HighlightedActor);
}

void APlayerCharacter::JumpStart()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}
			
			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}
	else
	{
		if (InitGamePC->IsEnableLaptopWidget() || InitGamePC->IsEnablePauseWidget())
		{
			return;
		}
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	Super::Jump();
	C2S_SetJumpState(true);
}

void APlayerCharacter::JumpEnd()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	C2S_SetJumpState(false);
}

void APlayerCharacter::LeftClickStart()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	Trace3DWidget->PressPointerKey(EKeys::LeftMouseButton);

	C2S_SetLeftClickStart(true);
	C2S_SetLeftClicking(false);

	ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);
	if (Sponge)
	{
		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 150.f);

			TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

			FHitResult OutHit;
			bool Result;
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI->bDrawDebug)
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}
			else
			{
				Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}

			if (Result)
			{
				AOpacityDecal* Decal = Cast<AOpacityDecal>(OutHit.GetActor());
				if (Decal)
				{
					C2S_SetErasingState(true);
					ABasicTool* Tool = Cast<ABasicTool>(FPSOwnedActor);
					if (Tool->GetToolType() == EToolType::ToolType_Sponge)
					{
						Tool->PickUpFPSSponge(this);
					}

					C2S_SetFixViewState(true);

					PC->SetMouseLocation(ScreenSizeX / 2, ScreenSizeY / 2);

					PreviousMousePos.X = ScreenSizeX / 2;
					PreviousMousePos.Y = ScreenSizeY / 2;

					CurrentMousePos.X = ScreenSizeX / 2;
					CurrentMousePos.Y = ScreenSizeY / 2;
				}
				else
				{
					C2S_SetFixViewState(false);
					C2S_SetErasingState(false);
				}
			}

			AInteractionBucket* Bucket = Cast<AInteractionBucket>(HighlightedActor);
			if (Bucket)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Cleaning"));
				C2S_SetWashSpongeState(true);
				C2S_CleaningTool(Bucket);
			}
		}

		C2S_SetSponge(Sponge, true);
	}

	AAxe* Axe = Cast<AAxe>(OwnedActor);
	if (Axe)
	{
		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(HighlightedActor);
		if (Furniture)
		{
			C2S_SetFocusFurnitureState(true);
		}
		else
		{
			C2S_SetFocusFurnitureState(false);
		}
	}
}

void APlayerCharacter::LeftClickOnGoing()
{
	C2S_SetLeftClickStart(false);
	C2S_SetLeftClickOnGoing(true);
}

void APlayerCharacter::C2S_SetToolMaxDurability_Implementation(ABasicTool* Tool)
{
	if (Tool)
	{
		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			AMop* Mop = Cast<AMop>(Tool);
			if (Mop)
			{
				//Mop->C2S_SetMaxDurability();
			}
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			ASponge* Sponge = Cast<ASponge>(Tool);
			if (Sponge)
			{
				Sponge->C2S_SetMaxDurability();
			}
		}
		break;
		}
	}
}

void APlayerCharacter::C2S_SetSpongeOverlappedDecal_Implementation(bool EraseState, AOpacityDecal* InDecal)
{
	ASponge* FPSSponge = Cast<ASponge>(FPSOwnedActor);
	if (InDecal && FPSSponge)
	{
		FPSSponge->SetOverlappedDecal(EraseState, InDecal);
	}
}

void APlayerCharacter::C2S_SetErasingState_Implementation(bool State)
{
	S2A_SetErasingState(State);
}

void APlayerCharacter::S2A_SetErasingState_Implementation(bool State)
{
	bIsErasing = State;
}

void APlayerCharacter::Interaction(bool State)
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}
			if (PC->IsEnableRewardWidget())
			{
				return;
			}
		}
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	C2S_SetInteractiveState(State);

	if (HighlightedActor)
	{
		AInteractionKeypad* Keypad = Cast<AInteractionKeypad>(HighlightedActor);
		if (Keypad)
		{
			C2S_SetDisableInteraction(true);
			return;
		}
		else
		{
			C2S_SetDisableInteraction(false);
		}

		AInteractionDoor* Door = Cast<AInteractionDoor>(HighlightedActor);
		AInteractionSwitch* Switch = Cast<AInteractionSwitch>(HighlightedActor);
		
		if (Door || Switch)
		{
			C2S_SetDisableInteraction(true);
		}
		else
		{
			C2S_SetDisableInteraction(false);
		}

		C2S_Interaction(HighlightedActor);
		HighlightedActor->DrawOutline(false);

		AInteractionPickUpObject* PickUpObject = Cast<AInteractionPickUpObject>(HighlightedActor);
		if (PickUpObject)
		{
			ARadioObject* Radio = Cast<ARadioObject>(PickUpObject);
			APotObject* Pot = Cast<APotObject>(PickUpObject);

			if (Radio && Radio->bActivate)
			{
				C2S_StopEvent(Radio);
			}
			else if (Pot && Pot->bActivate)
			{

			}
			else
			{
				if (!PickUpObject->ObjectOwner)
				{
					//C2S_SetInterpolationSpeed(30.0f);
				}

				if (!bIsHeld)
				{
					UE_LOG(LogTemp, Warning, TEXT("Interaction"));
					C2S_AddActorsToIgnore(Cast<AInteractionPickUpObject>(HighlightedActor));
					//C2S_Interaction(HighlightedActor);
					C2S_SetHeldState(true);
					C2S_SetOwnedActor(Cast<AInteractionPickUpObject>(HighlightedActor));
				}
			}

			AInGameGS* GS = Cast<AInGameGS>(UGameplayStatics::GetGameState(GetWorld()));
			if (!GS)
			{
				return;
			}
			
			if (!PC)
			{
				return;
			}

			switch (PickUpObject->PickUpObjectType)
			{
			case EPickUpObjectType::PickUpObjectType_Props:
			{
				if (!PC->bPrimaryProps)
				{
					FCaptionData* Data = GS->GetCaptionByID(FName(TEXT("PrimaryNotify_Props")));
					if (!Data)
					{
						return;
					}

					UpdateCaption(Data, 1.0f);

					PC->bPrimaryProps = true;
				}
			}
			break;
			case EPickUpObjectType::PickUpObjectType_WoodenPlank:
			{
				if (!PC->bPrimaryWoodenPlank)
				{
					FCaptionData* Data = GS->GetCaptionByID(FName(TEXT("PrimaryNotify_WoodenPlank")));
					if (!Data)
					{
						return;
					}

					UpdateCaption(Data, 1.0f);

					PC->bPrimaryWoodenPlank = true;
				}
			}
			break;
			
			}
		}

		HighlightedActor = nullptr;
	}
	else
	{
		C2S_SetDisableInteraction(false);
	}
}

void APlayerCharacter::C2S_Interaction_Implementation(ABasicInteractionObject* InteractiveObject)
{
	if (IsValid(InteractiveObject))
	{
		AInteractionBucket* Bucket = Cast<AInteractionBucket>(InteractiveObject);
		AInteractionFurniture* Furniture = Cast<AInteractionFurniture>(InteractiveObject);
		ABasicTool* Pickup = Cast<ABasicTool>(InteractiveObject);
		if (Pickup)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FTransform Transform = FTransform(GetActorLocation());
			ABasicTool* object = GetWorld()->SpawnActor<ABasicTool>(Pickup->GetClass(), SpawnParam);
			object->MaxDurability = Pickup->MaxDurability;
			C2S_AddActorsToIgnore(object);
			object->Box->SetSimulatePhysics(false);
			FPSOwnedActor = object;
			FPSOwnedActor->SetObjectOwner(this);

			OnRep_SetFPSObject();
		}
		if (Bucket)
		{
			Bucket->InterAction(this);
		}
		else if (Furniture)
		{
			Furniture->InterAction(this);
		}
		else
		{
			InteractiveObject->InterAction(this);
			UE_LOG(LogTemp, Warning, TEXT("Interaction"));
		}
	}

	if (PhysicsHandle->GetGrabbedComponent())
	{
		FName objectname = PhysicsHandle->GetGrabbedComponent()->GetOwner()->GetFName();
		UE_LOG(LogTemp, Warning, TEXT("Grabbed Object : %s"), *objectname.ToString());
	}

}

void APlayerCharacter::C2S_DropPreviousTool_Implementation(ABasicInteractionObject* InteractiveObject)
{
	ABasicTool* Tool = Cast<ABasicTool>(InteractiveObject);
	if (Tool)
	{
		Tool->DropPreviousTool(this);
	}
}

void APlayerCharacter::C2S_DetachTool_Implementation(ABasicInteractionObject* InteractiveObject)
{
	ABasicTool* Tool = Cast<ABasicTool>(InteractiveObject);
	if (Tool)
	{
		Tool->DetachPreviousSocket(this);

		if (Tool)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FTransform Transform = FTransform(GetActorLocation());
			ABasicTool* object = GetWorld()->SpawnActor<ABasicTool>(Tool->GetClass(), SpawnParam);
			object->MaxDurability = Tool->MaxDurability;
			C2S_AddActorsToIgnore(object);
			object->Box->SetSimulatePhysics(false);
			FPSOwnedActor = object;

			OnRep_SetFPSObject();

			Tool->SetObjectOwner(this);

			S2A_PickUpFPSObject(Tool);
		}

		//C2S_Interaction(Tool);
	}
}

void APlayerCharacter::C2A_PickUp_Implementation(ABasicInteractionObject* InteractiveObject)
{

}

void APlayerCharacter::C2S_SetInteractiveState_Implementation(bool State)
{
	bIsInteractive = State;
}

void APlayerCharacter::DropObject()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				return;
			}
		}
	}

	if (!IsLocallyControlled())
	{
		//return;
	}

	C2S_SetFixViewState(false);

	if (OwnedActor)
	{
		bIsObjectTargetLocation = false;
		C2S_SetHeldState(false);
		C2S_RemoveActorsToIgnore(OwnedActor);
		C2S_DropObject(OwnedActor);
		C2S_SetOwnedActor(nullptr);
		//HighlightedActor = nullptr;
	}
}

void APlayerCharacter::C2S_DropObject_Implementation(AInteractionPickUpObject* InteractiveObject)
{
	if (IsValid(InteractiveObject))
	{
		InteractiveObject->DropObject(this);
	}
}

void APlayerCharacter::C2S_DropPreviousObject_Implementation(AInteractionPickUpObject* InteractiveObject)
{
	if (IsValid(InteractiveObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("C2S_DropObject_Implementation"));
		InteractiveObject->DropPreviousObject(this);
	}
}

void APlayerCharacter::C2S_SetHeldState_Implementation(bool State)
{
	bIsHeld = State;
	OnRep_UpdateHeldState();
}

void APlayerCharacter::C2S_SetInventory_Implementation(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("C2S_SetInventory_Implementation"));

	if (!bUpdateInventoryByUI)
	{
		int32 CurrentTemp = CurrentInventoryIndex;
		int32 PreviousTemp = PreviousInventoryIndex;

		if (Value < 0)
		{
			if (CurrentInventoryIndex < Inventory.Num() - 1)
			{
				CurrentInventoryIndex++;
			}
			else if (CurrentInventoryIndex == Inventory.Num() - 1)
			{
				CurrentInventoryIndex = 0;
			}
		}
		else
		{
			if (CurrentInventoryIndex > 0)
			{
				CurrentInventoryIndex--;
			}
			else if (CurrentInventoryIndex == 0)
			{
				CurrentInventoryIndex = Inventory.Num() - 1;
			}
		}

		if (CurrentTemp != CurrentInventoryIndex)
		{
			PreviousInventoryIndex = CurrentTemp;
			if (PreviousTemp != PreviousInventoryIndex)
			{
				S2A_SetPreviousInventoryChangeState(true);
			}
			else
			{
				S2A_SetPreviousInventoryChangeState(false);
			}
			S2A_SetCurrentInventoryChangeState(true);
		}
		else
		{
			S2A_SetCurrentInventoryChangeState(false);
			S2A_SetPreviousInventoryChangeState(false);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("PlayerInventoryIndex : Cur(%d), Prev(%d)"), CurrentInventoryIndex, PreviousInventoryIndex);
	AInteractionPickUpObject* PreviousObject = Inventory[PreviousInventoryIndex];
	if (PreviousObject)
	{
		static bool IsFlashlightActivate = false;
		ABasicTool* PreviousTool = Cast<ABasicTool>(Inventory[PreviousInventoryIndex]);
		if (PreviousTool && bIsChangePreviousIndex)
		{
			switch (PreviousTool->GetToolType())
			{
			case EToolType::ToolType_Sponge:
			{
				ASponge* Sponge = Cast<ASponge>(PreviousTool);
				if (Sponge)
				{
					C2S_SetSponge(Sponge, false);
					C2S_SetCurrentDecal(nullptr);
				}
			}
			break;
			case EToolType::ToolType_Mop:
			{
				AMop* Mop = Cast<AMop>(PreviousTool);
				if (Mop)
				{
					C2S_SetCurrentDecal(nullptr);
				}
			}
			break;
			case EToolType::ToolType_Axe:
			{
				APlayerController* PC = Cast<APlayerController>(GetController());
				if (PC)
				{
					APC* TempPC = Cast<APC>(PC);
					if (TempPC)
					{
						TempPC->HideFurnitureProgressWidget();
					}
				}
			}
			break;
			case EToolType::ToolType_Flashlight:
			{
				IsFlashlightActivate = PreviousTool->bIsActivate;
			}
			break;
			}
			C2S_DropPreviousTool(PreviousObject);
			//PreviousTool->VisibleObject(false);
			S2C_HidePreviousTool(PreviousInventoryIndex);
			C2S_SetHeldState(false);
			C2S_SetOwnedActor(nullptr);

			AFlashlight* Flashlight = Cast<AFlashlight>(PreviousObject);
			if (Flashlight)
			{
				C2S_SetFlashlight(Flashlight, IsFlashlightActivate);
			}

			UE_LOG(LogTemp, Warning, TEXT("Previous Tool Remove"));
		}
		else
		{
			if (OwnedActor && OwnedActor->PickUpObjectType != EPickUpObjectType::PickUpObjectType_Tool)
			{
				UE_LOG(LogTemp, Warning, TEXT("DropObject"));
				C2S_SetHeldState(false);
				C2S_RemoveActorsToIgnore(OwnedActor);
				C2S_DropPreviousObject(OwnedActor);
				C2S_SetOwnedActor(nullptr);
				HighlightedActor = nullptr;
			}

			UE_LOG(LogTemp, Warning, TEXT("Previous Object Remove"));
		}
	}

	AInteractionPickUpObject* CurrentObject = Inventory[CurrentInventoryIndex];
	if (CurrentObject && bIsChangeCurrentIndex)
	{
		if (CurrentObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Tool)
		{
			if (CurrentObject->PickUpObjectType == EPickUpObjectType::PickUpObjectType_Tool)
			{
				ABasicTool* CurrentTool = Cast<ABasicTool>(Inventory[CurrentInventoryIndex]);
				if (CurrentTool)
				{
					switch (CurrentTool->GetToolType())
					{
					case EToolType::ToolType_Sponge:
					{
						ASponge* Sponge = Cast<ASponge>(CurrentTool);
						if (Sponge)
						{
							C2S_SetSponge(Sponge, true);
							C2S_SetCurrentDecal(CurrentSpongeDecal);
						}
					}
					break;
					case EToolType::ToolType_Mop:
					{
						AMop* Mop = Cast<AMop>(CurrentTool);
						if (Mop)
						{
							C2S_SetCurrentDecal(CurrentMopDecal);
						}
					}
					break;
					case EToolType::ToolType_Flashlight:
					{
						AFlashlight* Flashlight = Cast<AFlashlight>(CurrentTool);
						if (Flashlight)
						{
							C2S_SetFlashlight(Flashlight, Flashlight->bIsActivate);
						}
					}
					break;
					}

					C2S_DetachTool(CurrentTool);
					//CurrentTool->VisibleObject(true);
					UE_LOG(LogTemp, Warning, TEXT("Current Tool Add"));
				}
			}

			C2S_SetHeldState(true);
			C2S_SetOwnedActor(CurrentObject);
		}
	}

	if (PreviousObject)
	{
		ABasicTool* PreviousTool = Cast<ABasicTool>(Inventory[PreviousInventoryIndex]);
		if (PreviousTool && bIsChangePreviousIndex)
		{
			PreviousTool->S2A_AttachPreviousTool(this);
		}
	}
}

void APlayerCharacter::C2S_SetSponge_Implementation(ASponge* Sponge, bool State)
{
	if (IsValid(Sponge))
	{
		Sponge->S2A_SetSpongeActivate(State);
	}
}

void APlayerCharacter::C2S_SetObjectLocation_Implementation(FVector Location)
{
	PhysicsHandle->SetTargetLocation(Location);
	S2A_SetObjectLocation(Location);
}

void APlayerCharacter::S2A_SetObjectLocation_Implementation(FVector Location)
{
	PhysicsHandle->SetTargetLocation(Location);
}

void APlayerCharacter::C2S_SetObjectRotation_Implementation(FRotator Rotation)
{
	S2A_SetObjectRotation(Rotation);
}

void APlayerCharacter::S2A_SetObjectRotation_Implementation(FRotator Rotation)
{
	PhysicsHandle->SetTargetRotation(Rotation);
}

void APlayerCharacter::C2S_SetCurrentDecal_Implementation(AInteractionDecal* InDecal)
{
	S2A_SetCurrentDecal(InDecal);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentDecal"));
}

void APlayerCharacter::S2A_SetCurrentDecal_Implementation(AInteractionDecal* InDecal)
{
	CurrentDecal = InDecal;

	ATextureDecal* TextureDecal = Cast<ATextureDecal>(CurrentDecal);
	AOpacityDecal* OpacityDecal = Cast<AOpacityDecal>(CurrentDecal);
	if (TextureDecal)
	{
		CurrentMopDecal = TextureDecal;
	}
	else if (OpacityDecal)
	{
		CurrentSpongeDecal = OpacityDecal;
	}
}

void APlayerCharacter::C2S_SetSprint_Implementation(bool State)
{
	bIsSprint = State;
	GetCharacterMovement()->MaxWalkSpeed = State ? SprintSpeed : WalkSpeed;
}

void APlayerCharacter::C2S_ChangeSpeed_Implementation(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void APlayerCharacter::LeftClick()
{
	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC && PC->bFinishGame)
		{
			return;
		}
	}

	if (!IsLocallyControlled())
	{
		return;
	}

	C2S_SetLeftClicking(true);
}

void APlayerCharacter::LeftClickCanceled()
{
	Trace3DWidget->ReleasePointerKey(EKeys::LeftMouseButton);

	C2S_SetFixViewState(false);
	bIsEraseStart = false;

	C2S_SetLeftClickStart(false);
	C2S_SetLeftClickOnGoing(false);
	C2S_SetLeftClicking(false);
	C2S_SetLeftClickEnd(false);

	ABasicTool* Tool = Cast<ABasicTool>(FPSOwnedActor);
	if (Tool)
	{
		if (Tool->GetToolType() == EToolType::ToolType_Sponge)
		{
			if (PhysicsHandle->GetGrabbedComponent() && PhysicsHandle->GetGrabbedComponent()->GetOwner())
			{
				if (PhysicsHandle->GetGrabbedComponent()->GetOwner() == Tool)
				{
					PhysicsHandle->ReleaseComponent();
					Tool->PickUpObject(this);
					Tool->SetObjectOwner(this);
				}
			}

			if (IsLocallyControlled())
			{
				Tool->StaticMesh->SetVisibility(true);
			}
			else
			{
				OwnedActor->StaticMesh->SetVisibility(false);
			}

			ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);
			if (Sponge)
			{
				C2S_SetSponge(Sponge, false);
			}
		}
	}
}

void APlayerCharacter::LeftClickEnd()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	Trace3DWidget->ReleasePointerKey(EKeys::LeftMouseButton);

	C2S_SetFixViewState(false);
	bIsEraseStart = false;

	C2S_SetLeftClickStart(false);
	C2S_SetLeftClickOnGoing(false);
	C2S_SetLeftClicking(false);
	C2S_SetLeftClickEnd(false);

	ABasicTool* Tool = Cast<ABasicTool>(FPSOwnedActor);
	if (Tool)
	{
		if (Tool->GetToolType() == EToolType::ToolType_Sponge)
		{
			if (PhysicsHandle->GetGrabbedComponent() && PhysicsHandle->GetGrabbedComponent()->GetOwner())
			{
				if (PhysicsHandle->GetGrabbedComponent()->GetOwner() == Tool)
				{
					PhysicsHandle->ReleaseComponent();
					Tool->PickUpObject(this);
					Tool->SetObjectOwner(this);
				}
			}
			
			if (IsLocallyControlled())
			{
				Tool->StaticMesh->SetVisibility(true);
			}
			else
			{
				OwnedActor->StaticMesh->SetVisibility(false);
			}

			ASponge* Sponge = Cast<ASponge>(FPSOwnedActor);
			if (Sponge)
			{
				C2S_SetSponge(Sponge, false);
			}
		}
	}
}

void APlayerCharacter::Tab()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AInitGamePC* InitGamePC = Cast<AInitGamePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!InitGamePC)
	{
		if (PC)
		{
			if (PC->bFinishGame)
			{
				return;
			}

			if (PC->IsEnablePauseWidget())
			{
				return;
			}

			if (PC->IsEnableCleanProgressWidget())
			{
				return;
			}
		}
	}

	if (PC)
	{
		PC->HideAimWidget(this);
		PC->ShowInventoryWidget(this);
	}
}

void APlayerCharacter::TabEnd()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APC* PC = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		PC->HideInventoryWidget(this);
		PC->ShowAimWidget(this);
	}
}

void APlayerCharacter::C2S_DecreaseDurability_Implementation(class ABasicTool* Tool)
{
	AMop* Mop = Cast<AMop>(Tool);
	ASponge* Sponge = Cast<ASponge>(Tool);
	if (Mop)
	{
		Mop->C2S_DecreaseDurability(CurrentMopDecal, this);
	}
	else if (Sponge)
	{
		Sponge->C2S_DecreaseDurability(this);
	}
}

void APlayerCharacter::C2S_CreateDecal_Implementation(AMop* Mop, FVector ImpactPoint, FVector Normal)
{
	if (Mop)
	{
		Mop->CreateDecal(ImpactPoint, Normal);

		AInGamePS* PS = Cast<AInGamePS>(GetPlayerState());
		if (PS)
		{
			PS->CreateDecalCount++;
			PS->OnRep_UpdateReward();
		}

		AInGameGS* GS = Cast<AInGameGS>(GetWorld()->GetGameState());
		if (GS)
		{
			GS->CreateTextureDecalCount++;
			GS->OnRep_UpdateCount();
		}
	}
}

void APlayerCharacter::C2S_AddActorsToIgnore_Implementation(AActor* InActor)
{
	S2A_AddActorsToIgnore(InActor);
}

void APlayerCharacter::S2A_AddActorsToIgnore_Implementation(AActor* InActor)
{
	if (InActor)
	{
		ActorsToIgnore.Add(InActor);
		//UE_LOG(LogTemp, Warning, TEXT("S2A_AddActorsToIgnore"));
	}
}

void APlayerCharacter::C2S_RemoveActorsToIgnore_Implementation(AInteractionPickUpObject* InActor)
{
	S2A_RemoveActorsToIgnore(InActor);
}

void APlayerCharacter::S2A_RemoveActorsToIgnore_Implementation(AInteractionPickUpObject* InActor)
{
	if (InActor)
	{
		ActorsToIgnore.Remove(InActor);
	}
}

void APlayerCharacter::C2S_SetInterpolationSpeed_Implementation(float Speed)
{
	S2A_SetInterpolationSpeed(Speed);
}

void APlayerCharacter::S2A_SetInterpolationSpeed_Implementation(float Speed)
{
	PhysicsHandle->SetInterpolationSpeed(Speed);
}

void APlayerCharacter::RemoveInventoryIndex(int Index)
{
	if (Inventory[Index])
	{
		Inventory[Index] = nullptr;
	}
}

void APlayerCharacter::AddInventoryCurrentIndex(AInteractionPickUpObject* Object)
{
	if (!Inventory[CurrentInventoryIndex])
	{
		Inventory[CurrentInventoryIndex] = Object;
	}
}

void APlayerCharacter::DropAbsolutely()
{
	if (OwnedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropAbsolutely"));
		C2S_SetHeldState(false);
		C2S_RemoveActorsToIgnore(OwnedActor);
		C2S_DropObject(OwnedActor);
		C2S_SetOwnedActor(nullptr);
		HighlightedActor = nullptr;
	}
}

void APlayerCharacter::C2S_SetLeftClickStart_Implementation(bool State)
{
	S2A_SetLeftClickStart(State);
}

void APlayerCharacter::S2A_SetLeftClickStart_Implementation(bool State)
{
	bIsLeftClickStart = State;
}

void APlayerCharacter::C2S_SetLeftClickOnGoing_Implementation(bool State)
{
	S2A_SetLeftClickOnGoing(State);
}

void APlayerCharacter::S2A_SetLeftClickOnGoing_Implementation(bool State)
{
	bIsLeftClickOnGoing = State;
}

void APlayerCharacter::C2S_SetLeftClicking_Implementation(bool State)
{
	S2A_SetLeftClicking(State);
}

void APlayerCharacter::S2A_SetLeftClicking_Implementation(bool State)
{
	bIsLeftClicking = State;
	if (State)
	{
		bIsLeftClickOnGoing = false;
	}
}

void APlayerCharacter::C2S_SetLeftClickEnd_Implementation(bool State)
{
	S2A_SetLeftClickEnd(State);
}

void APlayerCharacter::S2A_SetLeftClickEnd_Implementation(bool State)
{
	bIsLeftClicking = State;
}

void APlayerCharacter::MopInteraction()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	AMop* Mop = Cast<AMop>(Inventory[CurrentInventoryIndex]);
	if (Mop)
	{
		UE_LOG(LogTemp, Warning, TEXT("MOP DURABILITY : %d"), Mop->Durability);

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			int32 ScreenSizeX;
			int32 ScreenSizeY;
			FVector CrosshairWorldPosition; //3D
			FVector CrosshairWorldDirection; //3D 

			FVector CameraLocation;
			FRotator CameraRotation;

			PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
			PC->DeprojectScreenPositionToWorld(ScreenSizeX / 2, ScreenSizeY / 2, CrosshairWorldPosition, CrosshairWorldDirection);

			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector TraceStart = CameraLocation;
			FVector TraceEnd = TraceStart + (CrosshairWorldDirection * 300.f);
			float Radius=50.0f;

			TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

			TArray<FHitResult> OutHits;
			bool Result;
			UGI* GI = Cast<UGI>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GI->bDrawDebug)
			{
				Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd,Radius, Objects, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}
			else
			{
				Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), TraceStart, TraceEnd, Radius, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
			}
			if (Result)
			{
				EnableMopEffect();
				for (const auto& Outhit:OutHits)
				{
					AInteractionDecal* Temp = Cast<AInteractionDecal>(Outhit.GetActor());
					AInteractionBucket* Bucket = Cast<AInteractionBucket>(Outhit.GetActor());
					if (!Temp && !Bucket)
					{
						C2S_AddForce(Outhit.GetComponent(), ((TraceEnd - TraceStart).GetSafeNormal()) * MopImpulseSize, Outhit.ImpactPoint);
						Mop->S2A_SoundPlay();
					}
					if (Bucket)
					{
						C2S_CleaningTool(Bucket);
						Mop->S2A_SoundPlay();
					}
				}
			}
			Mop->S2A_SwingSoundPlay();
			if (Mop->Durability > 0)
			{
				if (Result)
				{
					int32 HighestSortOrder = -1;
					AInteractionDecal* HightestSortOrderDecal = nullptr;
					for (const FHitResult& Hit : OutHits)
					{
						AInteractionDecal* Decal = Cast<AInteractionDecal>(Hit.GetActor());
						if (Decal && Decal->Durability > 0)
						{
							int32 CurrentSortOrder = Decal->GetSortOrder();
							if (CurrentSortOrder > HighestSortOrder)
							{
								HighestSortOrder = CurrentSortOrder;
								HightestSortOrderDecal = Decal;
							}
						}
					}

					TArray<AActor*> EraseDecals;
					if (HightestSortOrderDecal)
					{
						HightestSortOrderDecal->Box->GetOverlappingActors(EraseDecals, ATextureDecal::StaticClass());
						EraseDecals.Add(HightestSortOrderDecal);
						SortDecalsBySortOrder(EraseDecals);

						if (EraseDecals.Num() >= Mop->EraseCount)
						{
							for (int i = EraseDecals.Num() - 1; i > EraseDecals.Num() - 1 - Mop->EraseCount; i--)
							{
								ATextureDecal* EraseDecal = Cast<ATextureDecal>(EraseDecals[i]);
								if (EraseDecal)
								{
									if (EraseDecal->Durability <= 0)
									{
										C2S_SetErasingState(false);
									}
									else
									{
										if (Mop->Durability > 0)
										{
											C2S_SetErasingState(true);
											C2S_SetCurrentDecal(EraseDecal);
											C2S_DecreaseDurability(Mop);
										}
									}
								}
							}
						}
						else
						{
							for (AActor* Decal : EraseDecals)
							{
								ATextureDecal* EraseDecal = Cast<ATextureDecal>(Decal);
								if (EraseDecal->Durability <= 0)
								{
									C2S_SetErasingState(false);
								}
								else
								{
									if (Mop->Durability > 0)
									{
										C2S_SetErasingState(true);
										C2S_SetCurrentDecal(EraseDecal);
										C2S_DecreaseDurability(Mop);
									}
								}
							}
						}
					}
					Mop->S2A_SoundPlay();
				}
			}
			else
			{
				FHitResult OutHit;
				TArray<TEnumAsByte<EObjectTypeQuery>> Objects1;

				Objects1.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
				Objects1.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

				if (GI->bDrawDebug)
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);
				}
				else
				{
					Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Objects1, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Blue, FLinearColor::Yellow, 5.0f);

				}

				if (Result)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit Object : %s"), *OutHit.GetActor()->GetFName().ToString());
					AInGamePS* PS = Cast<AInGamePS>(GetPlayerState());
					if (PS)
					{
						PS->CreateDecalCount++;
					}

					C2S_CreateDecal(Mop, OutHit.ImpactPoint, OutHit.Normal);
					Mop->S2A_SoundPlay();
				}

			}
		}
	}
}

void APlayerCharacter::EnableMopEffect()
{
	AMop* Mop = Cast<AMop>(Inventory[CurrentInventoryIndex]);
	if (Mop)
	{
		if (Mop->SkeletalMesh->IsVisible())
		{
			Mop->EnableEffect();
		}

		AMop* FPSMop = Cast<AMop>(FPSOwnedActor);
		if (FPSMop && FPSMop->SkeletalMesh->IsVisible())
		{
			FPSMop->EnableEffect();
		}
	}
}

void APlayerCharacter::C2S_CleaningTool_Implementation(class AInteractionBucket* Tool)
{
	Tool->Cleaning(this);
}

void APlayerCharacter::C2S_SetFixViewState_Implementation(bool State)
{
	S2A_SetFixViewState(State);
}

void APlayerCharacter::S2A_SetFixViewState_Implementation(bool State)
{
	bFixView = State;


}

void APlayerCharacter::C2S_SetInventoryIndex_Implementation(int32 Index)
{
	S2A_SetInventoryIndex(Index);
}

void APlayerCharacter::S2A_SetInventoryIndex_Implementation(int32 Index)
{
	if (CurrentInventoryIndex != Index)
	{
		if (PreviousInventoryIndex != CurrentInventoryIndex)
		{
			PreviousInventoryIndex = CurrentInventoryIndex;
			bIsChangePreviousIndex = true;
		}

		CurrentInventoryIndex = Index;
		bIsChangeCurrentIndex = true;
	}
	else
	{
		bIsChangeCurrentIndex = false;
		bIsChangePreviousIndex = false;
	}
}

void APlayerCharacter::C2S_SetUpdateInventoryState_Implementation(bool State)
{
	S2A_SetUpdateInventoryState(State);
}

void APlayerCharacter::S2A_SetUpdateInventoryState_Implementation(bool State)
{
	bUpdateInventoryByUI = State;
}

void APlayerCharacter::C2S_UpdatePreviousInventoryIndex_Implementation()
{
	S2A_UpdatePreviousInventoryInde();
}

void APlayerCharacter::S2A_UpdatePreviousInventoryInde_Implementation()
{
	if (CurrentInventoryIndex <= 0)
	{
		PreviousInventoryIndex = Inventory.Num() - 1;
	}
	else if (0 < CurrentInventoryIndex && CurrentInventoryIndex < Inventory.Num() - 1)
	{
		PreviousInventoryIndex = CurrentInventoryIndex - 1;
	}
	else if (CurrentInventoryIndex >= Inventory.Num() - 1)
	{
		PreviousInventoryIndex = 0;
	}
}

void APlayerCharacter::C2S_Axe_Hit_Implementation(AInteractionFurniture* Furniture)
{
	if (IsValid(Furniture))
	{
		Furniture->Axe_Hit(this);
	}
}

void APlayerCharacter::OnRep_SetFPSObject()
{
	if (FPSOwnedActor)
	{
		AMop* Mop = Cast<AMop>(FPSOwnedActor);
		if (Mop)
		{
			if (IsLocallyControlled())
			{
				Mop->SkeletalMesh->SetVisibility(true);
			}
			else
			{
				Mop->SkeletalMesh->SetVisibility(false);
			}
		}
		else
		{
			if (IsLocallyControlled())
			{
				FPSOwnedActor->StaticMesh->SetVisibility(true);
			}
			else
			{
				FPSOwnedActor->StaticMesh->SetVisibility(false);
			}
		}
	}
}

void APlayerCharacter::S2A_SetCurrentInventoryChangeState_Implementation(bool State)
{
	bIsChangeCurrentIndex = State;
}

void APlayerCharacter::S2A_SetPreviousInventoryChangeState_Implementation(bool State)
{
	bIsChangePreviousIndex = State;
}

void APlayerCharacter::S2A_PickUpFPSObject_Implementation(ABasicTool* Pickup)
{
	if (Pickup)
	{
		Pickup->PickUpObject(this);
	}
}

void APlayerCharacter::C2S_AddForce_Implementation(UPrimitiveComponent* Component, FVector force, FVector Location)
{
	S2A_AddForce(Component, force, Location);
}

void APlayerCharacter::S2A_AddForce_Implementation(UPrimitiveComponent* Component, FVector force, FVector Location)
{
	if (Component)
	{
		Component->AddForceAtLocation(force, Location);
	}
}

void APlayerCharacter::OnRep_SetEraseStartState()
{
	UE_LOG(LogTemp, Warning, TEXT("MOP ERASE START"));
}

void APlayerCharacter::DecreaseFurnitureDurability(float InMaxHp, float InCurrentHp)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		APC* TempPC = Cast<APC>(PC);

		if (TempPC)
		{
			TempPC->UpdateFurnitureProgressBar(InMaxHp, InCurrentHp);
		}
	}
}

void APlayerCharacter::S2A_SetCameraLocation_Implementation(bool UseControlRotation, FVector Location, FRotator Rotation)
{
	Camera->bUsePawnControlRotation = UseControlRotation;
	Camera->SetWorldLocation(Location);
	Camera->SetWorldRotation(FQuat::MakeFromRotator(Rotation));
}

void APlayerCharacter::C2S_SetOwnedActor_Implementation(class AInteractionPickUpObject* InteractiveObject)
{
	S2A_SetOwnedActor(InteractiveObject);
}

void APlayerCharacter::S2A_SetOwnedActor_Implementation(class AInteractionPickUpObject* InteractiveObject)
{
	OwnedActor = InteractiveObject;

	ABasicTool* Tool = Cast<ABasicTool>(OwnedActor);
	if (Tool)
	{
		switch (Tool->GetToolType())
		{
		case EToolType::ToolType_Mop:
		{
			AMop* Mop = Cast<AMop>(Tool);
			AMop* NewFPSMop = Cast<AMop>(FPSOwnedActor);
			if (NewFPSMop)
			{
				NewFPSMop->S2A_SetFPSMop(Mop->ObjectOwner, Mop->Durability, Mop->MaxDurability, Mop->CleanDurability, Mop->DirtyDurability);
			}
		}
		break;
		case EToolType::ToolType_Sponge:
		{
			ASponge* Sponge = Cast<ASponge>(Tool);
			
			ASponge* NewFPSSponge = Cast<ASponge>(FPSOwnedActor);
			if (NewFPSSponge)
			{
				NewFPSSponge->S2A_SetFPSSponge(Sponge->ObjectOwner, Sponge->Durability, Sponge->MaxDurability, Sponge->CleanDurability, Sponge->DirtyDurability, Sponge->OpacityRatio);
			}
		}
		break;
		}

	}
}