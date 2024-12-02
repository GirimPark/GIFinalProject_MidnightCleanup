// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ToolTipData.h"
#include "CaptionData.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
UCLASS()
class MIDNIGHTCLEANUP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement
	void Look(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

	void Sprint();
	void StopSprint();

	void LeftMouse(const FInputActionValue& Value);

	void JumpStart();
	void JumpEnd();

	void Tab();
	void TabEnd();

	void LeftClickStart();
	void LeftClickOnGoing();
	void LeftClick();
	void LeftClickCanceled();
	void LeftClickEnd();

	void Interaction(bool State);

	void DropObject();
	
	void SetInventory(float Value);

	void SetFlashlight();

	void SetToolMaxDurability();

	void SortDecalsBySortOrder(TArray<class AActor*>& Decals);

	UFUNCTION(BlueprintCallable)
	void Axe_Hit();

	UFUNCTION(BlueprintCallable)
	void EndAxeWipe();

	void Wheel(const FInputActionValue& Value);

	void EnableKeypad();

	UFUNCTION(BlueprintCallable)
	void SetCameraLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	void EnableLaptop();
	void DisableLaptop();

	void UpdateToolTip(FToolTipData* InData);
	void UpdateCaption(FCaptionData* InData, float Speed);

	void ShowCleanProgressWidget();
	void HideCleanProgressWidget();
	void UpdateCleanProgressWidget();

	void StartEscAction();

	UFUNCTION()
	void BeginOverlapProximityBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapProximityBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetSpongeOverlappedDecal(bool EraseState, class AOpacityDecal* Decal);

	FRotator GetAimOffset() const;

	void SetToolHeldState(class ABasicTool* Tool, bool State);
	void SetFlashlightState(class AFlashlight* Flashlight, bool State);

	UFUNCTION()
	void OnRep_UpdateHeldState();
	
	void UpdateAimWidget();

	UFUNCTION(BlueprintCallable)
	void UpdateCaptionByCollision(FName CaptionID, float InSpeed);

	void UpdateTPSFlashlightSocket(FName SocketName);

	void EnableModifyPlayTime();

	UFUNCTION(BlueprintCallable)
	bool IsLookingAtKeypad();

	void ModifyPlayTime(bool State);

	void CheckIndicator(class AIndicatorVolumeBase* InVolume);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMeshComponent> FPSMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> FPSWatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> TPSWatch;

	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* CameraSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UWidgetInteractionComponent> Trace3DWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UBoxComponent> ProximityBox;

	// Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>  MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> InteractiveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> TapAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> LeftMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> FlashlightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ToolDurabilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MouseWheelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> EscAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> CleanProgressAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> ModifyPlayTimeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> IncreasePlayTimeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> DecreasePlayTimeAction;

	// Owned Actor
	UPROPERTY(VisibleAnywhere, Replicated)
	class AInteractionPickUpObject* OwnedActor;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing ="OnRep_SetFPSObject")
	class AInteractionPickUpObject* FPSOwnedActor;

	UPROPERTY(VisibleAnywhere)
	class USpotLightComponent* FPSFlashlight;

	UPROPERTY(VisibleAnywhere)
	class USpotLightComponent* TPSFlashlight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class APlayerCharacter*> OverlappedPlayers;

	// Interactive Object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	class ABasicInteractionObject* HighlightedActor;

	FVector SpongeFocusedPoint;

	FToolTipData CurrentToolTipData;
	FCaptionData CurrentCaptionData;
	
	// Tool
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AInteractionPickUpObject*> Inventory;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float WalkSpeed = 300.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SprintSpeed = 600.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float InterpSpeed = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MopImpulseSize = 500;

	bool bIsObjectTargetLocation=false;

	// Mouse Pos
	FVector2D PreviousMousePos;
	FVector2D CurrentMousePos;

	// State bool 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE", Replicated)
	uint8 bDisableInteraction : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE", Replicated)
	uint8 bIsLeftClickStart : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE", Replicated)
	uint8 bIsLeftClicking : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE", Replicated)
	uint8 bIsLeftClickOnGoing : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE", Replicated)
	uint8 bIsErasing : 1 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE", Replicated)
	uint8 bIsJump : 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Status")
	uint8 bIsSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsInteractive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_UpdateHeldState")
	uint8 bIsHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsHeldBox : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsHeldBucket : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsHeldTrash : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsHeldWoodenPlank : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsMopHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsAxeHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsFlashlightHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsSpongeHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bFixView : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	uint8 bUpdateInventoryByUI : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 CurrentInventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 PreviousInventoryIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsChangePreviousIndex : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsChangeCurrentIndex : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_SetEraseStartState")
	uint8 bIsEraseStart : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bFocusFurniture : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bWashSponge : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AInteractionDecal* CurrentDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATextureDecal* CurrentMopDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOpacityDecal* CurrentSpongeDecal;

	FVector GrabbedObjectLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float DistanceLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (ClampMax = 2.0f,ClampMin=0.1f))
	float MouseSensitivity=1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 YAxisCheck : 1;

	int32 Id = 0;

	

	FVector InitCameraRelativeLocation;

public:
	// Network
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void C2S_SetToolMaxDurability(class ABasicTool* Tool);
	void C2S_SetToolMaxDurability_Implementation(class ABasicTool* Tool);

	UFUNCTION(Server, Reliable)
	void C2S_SetSpongeOverlappedDecal(bool EraseState, class AOpacityDecal* InDecal);
	void C2S_SetSpongeOverlappedDecal_Implementation(bool EraseState, class AOpacityDecal* InDecal);

	UFUNCTION(Server, Unreliable)
	void C2S_SetErasingState(bool State);
	void C2S_SetErasingState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetErasingState(bool State);
	void S2A_SetErasingState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_Interaction(class ABasicInteractionObject* InteractiveObject);
	void C2S_Interaction_Implementation(class ABasicInteractionObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_DropPreviousTool(class ABasicInteractionObject* InteractiveObject);
	void C2S_DropPreviousTool_Implementation(class ABasicInteractionObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_DetachTool(class ABasicInteractionObject* InteractiveObject);
	void C2S_DetachTool_Implementation(class ABasicInteractionObject* InteractiveObject);

	UFUNCTION(NetMulticast, Reliable)
	void C2A_PickUp(class ABasicInteractionObject* InteractiveObject);
	void C2A_PickUp_Implementation(class ABasicInteractionObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_SetInteractiveState(bool State);
	void C2S_SetInteractiveState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_DropObject(class AInteractionPickUpObject* InteractiveObject);
	void C2S_DropObject_Implementation(class AInteractionPickUpObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_DropPreviousObject(class AInteractionPickUpObject* InteractiveObject);
	void C2S_DropPreviousObject_Implementation(class AInteractionPickUpObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_SetHeldState(bool State);
	void C2S_SetHeldState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetInventory(float Value);
	void C2S_SetInventory_Implementation(float Value);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetOwnedActor(class AInteractionPickUpObject* InteractiveObject);
	void S2A_SetOwnedActor_Implementation(class AInteractionPickUpObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_SetOwnedActor(class AInteractionPickUpObject* InteractiveObject);
	void C2S_SetOwnedActor_Implementation(class AInteractionPickUpObject* InteractiveObject);

	UFUNCTION(Server, Reliable)
	void C2S_SetSponge(class ASponge* Sponge, bool State);
	void C2S_SetSponge_Implementation(class ASponge* Sponge, bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetObjectLocation(FVector Location);
	void C2S_SetObjectLocation_Implementation(FVector Location);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetObjectLocation(FVector Location);
	void S2A_SetObjectLocation_Implementation(FVector Location);

	UFUNCTION(Server, Reliable)
	void C2S_SetObjectRotation(FRotator Rotation);
	void C2S_SetObjectRotation_Implementation(FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetObjectRotation(FRotator Rotation);
	void S2A_SetObjectRotation_Implementation(FRotator Rotation);

	UFUNCTION(Server, Reliable)
	void C2S_SetCurrentDecal(class AInteractionDecal* InDecal);
	void C2S_SetCurrentDecal_Implementation(class AInteractionDecal* InDecal);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetCurrentDecal(class AInteractionDecal* InDecal);
	void S2A_SetCurrentDecal_Implementation(class AInteractionDecal* InDecal);

	UFUNCTION(Server, Reliable)
	void C2S_SetSprint(bool State);
	void C2S_SetSprint_Implementation(bool State);

	UFUNCTION(Server,Reliable)
	void C2S_ChangeSpeed(float Speed);
	void C2S_ChangeSpeed_Implementation(float Speed);

	UFUNCTION(Server, Reliable)
	void C2S_DecreaseDurability(class ABasicTool* Tool);
	void C2S_DecreaseDurability_Implementation(class ABasicTool* Tool);

	UFUNCTION(Server, Reliable)
	void C2S_CreateDecal(class AMop* Mop, FVector ImpactPoint, FVector Normal);
	void C2S_CreateDecal_Implementation(class AMop* Mop, FVector ImpactPoint, FVector Normal);

	UFUNCTION(Server, Reliable)
	void C2S_AddActorsToIgnore(AActor* InActor);
	void C2S_AddActorsToIgnore_Implementation(AActor* InActor);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_AddActorsToIgnore(AActor* InActor);
	void S2A_AddActorsToIgnore_Implementation(AActor* InActor);

	UFUNCTION(Server, Reliable)
	void C2S_RemoveActorsToIgnore(AInteractionPickUpObject* InActor);
	void C2S_RemoveActorsToIgnore_Implementation(AInteractionPickUpObject* InActor);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_RemoveActorsToIgnore(AInteractionPickUpObject* InActor);
	void S2A_RemoveActorsToIgnore_Implementation(AInteractionPickUpObject* InActor);

	UFUNCTION(Server, Reliable)
	void C2S_SetInterpolationSpeed(float Speed);
	void C2S_SetInterpolationSpeed_Implementation(float Speed);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetInterpolationSpeed(float Speed);
	void S2A_SetInterpolationSpeed_Implementation(float Speed);

	void RemoveInventoryIndex(int Index);
	void AddInventoryCurrentIndex(AInteractionPickUpObject* Object);

	void DropAbsolutely();

	UFUNCTION(Server, Reliable)
	void C2S_SetLeftClickStart(bool State);
	void C2S_SetLeftClickStart_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetLeftClickStart(bool State);
	void S2A_SetLeftClickStart_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetLeftClickOnGoing(bool State);
	void C2S_SetLeftClickOnGoing_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetLeftClickOnGoing(bool State);
	void S2A_SetLeftClickOnGoing_Implementation(bool State);

	UFUNCTION(Server, Unreliable)
	void C2S_SetLeftClicking(bool State);
	void C2S_SetLeftClicking_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetLeftClicking(bool State);
	void S2A_SetLeftClicking_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetLeftClickEnd(bool State);
	void C2S_SetLeftClickEnd_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetLeftClickEnd(bool State);
	void S2A_SetLeftClickEnd_Implementation(bool State);

	UFUNCTION(BlueprintCallable)
	void MopInteraction();

	UFUNCTION(BlueprintCallable)
	void EnableMopEffect();

	UFUNCTION(Server,Reliable)
	void C2S_CleaningTool(class AInteractionBucket* Tool);
	void C2S_CleaningTool_Implementation(class AInteractionBucket* Tool);

	UFUNCTION(Server, Reliable)
	void C2S_SetFixViewState(bool State);
	void C2S_SetFixViewState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetFixViewState(bool State);
	void S2A_SetFixViewState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetInventoryIndex(int32 Index);
	void C2S_SetInventoryIndex_Implementation(int32 Index);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetInventoryIndex(int32 Index);
	void S2A_SetInventoryIndex_Implementation(int32 Index);

	UFUNCTION(Server, Reliable)
	void C2S_SetUpdateInventoryState(bool State);
	void C2S_SetUpdateInventoryState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetUpdateInventoryState(bool State);
	void S2A_SetUpdateInventoryState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_UpdatePreviousInventoryIndex();
	void C2S_UpdatePreviousInventoryIndex_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_UpdatePreviousInventoryInde();
	void S2A_UpdatePreviousInventoryInde_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_Axe_Hit(class AInteractionFurniture* Furniture);
	void C2S_Axe_Hit_Implementation(class AInteractionFurniture* Furniture);

	UFUNCTION()
	void OnRep_SetFPSObject();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetCurrentInventoryChangeState(bool State);
	void S2A_SetCurrentInventoryChangeState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetPreviousInventoryChangeState(bool State);
	void S2A_SetPreviousInventoryChangeState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_PickUpFPSObject(class ABasicTool* Pickup);
	void S2A_PickUpFPSObject_Implementation(class ABasicTool* Pickup);

	UFUNCTION(Server,Reliable)
	void C2S_AddForce(UPrimitiveComponent* Component,FVector force, FVector Location);
	void C2S_AddForce_Implementation(UPrimitiveComponent* Component,FVector force, FVector Location);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_AddForce(UPrimitiveComponent* Component, FVector force, FVector Location);
	void S2A_AddForce_Implementation(UPrimitiveComponent* Component, FVector force, FVector Location);

	UFUNCTION()
	void OnRep_SetEraseStartState();

	void DecreaseFurnitureDurability(float InMaxHp, float InCurrentHp);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveCameraToComputer(class USplineComponent* InPath);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetCameraLocation(bool UseControlRotation, FVector Location, FRotator Rotation);
	void S2A_SetCameraLocation_Implementation(bool UseControlRotation, FVector Location, FRotator Rotation);

	UFUNCTION(Client, Reliable)
	void S2C_EnableLaptop();
	void S2C_EnableLaptop_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_DisableLaptop();
	void C2S_DisableLaptop_Implementation();

	UFUNCTION(Client, Reliable)
	void S2A_DisableLaptop();
	void S2A_DisableLaptop_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_SetJumpState(bool State);
	void C2S_SetJumpState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetJumpState(bool State);
	void S2A_SetJumpState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetFocusFurnitureState(bool State);
	void C2S_SetFocusFurnitureState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetFocusFurnitureState(bool State);
	void S2A_SetFocusFurnitureState_Implementation(bool State);

	UFUNCTION(Server, Reliable)
	void C2S_SetWashSpongeState(bool State);
	void C2S_SetWashSpongeState_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetWashSpongeState(bool State);
	void S2A_SetWashSpongeState_Implementation(bool State);

	bool IsAttachActor(FName InName);

	UFUNCTION(Client, Reliable)
	void S2C_HidePreviousTool(int32 Index);
	void S2C_HidePreviousTool_Implementation(int32 Index);

	UFUNCTION(Server, Reliable)
	void C2S_StopEvent(class ARadioObject* Radio);
	void C2S_StopEvent_Implementation(class ARadioObject* Radio);

	UFUNCTION(BlueprintCallable)
	void WashSpongeEnd();

	UFUNCTION(Server, Reliable)
	void C2S_SetFlashlight(class AFlashlight* Flashlight, bool State);
	void C2S_SetFlashlight_Implementation(class AFlashlight* Flashlight, bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetFlashlight(class AFlashlight* Flashlight, bool State);
	void S2A_SetFlashlight_Implementation(class AFlashlight* Flashlight, bool State);

	void SetFPSFlashlight(class AFlashlight* Flashlight, bool State);
	void SetTPSFlashlight(class AFlashlight* Flashlight, bool State);

	UFUNCTION(BlueprintCallable)
	FString GetUserID();

	UFUNCTION()
	void CheckPlayerState();

	FTimerHandle CheckPlayerStateHandle;

	UFUNCTION(Server, Reliable)
	void C2S_SetUserID(const FString& InValue);
	void C2S_SetUserID_Implementation(const FString& InValue);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetUserID(const FString& InValue);
	void S2A_SetUserID_Implementation(const FString& InValue);

	void SetThumbsUpNum(const FString& InUserID);

	UFUNCTION(Server, Reliable)
	void C2S_SetThumbsUpNum(const FString& InUserID);
	void C2S_SetThumbsUpNum_Implementation(const FString& InUserID);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetThumbsUpNum(const FString& InUserID);
	void S2A_SetThumbsUpNum_Implementation(const FString& InUserID);

	void UpdateEnableTriggerCount();

	UFUNCTION(Server, Reliable)
	void C2S_UpdateEnableTriggerCount();
	void C2S_UpdateEnableTriggerCount_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_UpdateStealCount();
	void C2S_UpdateStealCount_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_UpdateTPSFlashlightSocket(FName SocketName);
	void C2S_UpdateTPSFlashlightSocket_Implementation(FName SocketName);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_UpdateTPSFlashlightSocket(FName SocketName);
	void S2A_UpdateTPSFlashlightSocket_Implementation(FName SocketName);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_EnableLoadingScreen();
	void S2A_EnableLoadingScreen_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_SetDisableInteraction(bool State);
	void C2S_SetDisableInteraction_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetDisableInteraction(bool State);
	void S2A_SetDisableInteraction_Implementation(bool State);

	UFUNCTION(Client, Reliable)
	void S2C_EnableIndicator(class AIndicatorVolumeBase* InVolume);
	void S2C_EnableIndicator_Implementation(class AIndicatorVolumeBase* InVolume);

	UFUNCTION(Client, Reliable)
	void S2C_DisableIndicator(class AIndicatorVolumeBase* InVolume);
	void S2C_DisableIndicator_Implementation(class AIndicatorVolumeBase* InVolume);

	UFUNCTION(Client, Reliable)
	void S2C_CheckIndicator(class AIndicatorVolumeBase* InVolume, bool State);
	void S2C_CheckIndicator_Implementation(class AIndicatorVolumeBase* InVolume, bool State);
};
