// Copyright 2022 Ellie Kelemen. All Rights Reserved.

#include "Components/InventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "FPSCharacter.h"
#include "FPSCharacterController.h"
#include "WeaponBase.h"
#include "WeaponPickup.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Items/ItemObject.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
}

// Swapping weapons with the scroll wheel
void UInventoryComponent::ScrollWeapon(const FInputActionValue& Value)
{
	int NewID;

	// Value[0] determines the axis of input for our scroll wheel
	// a positive value indicates scrolling towards you, while a negative one
	// represents scrolling away from you
	
	if (Value[0] < 0)
	{
		NewID = FMath::Clamp(CurrentWeaponSlot + 1, 0, NumberOfWeaponSlots - 1);

		// If we've reached the end of the weapons array, loop back around to index 0
		if (CurrentWeaponSlot == NumberOfWeaponSlots - 1)
		{
			NewID = 0;
		}
	}
	else
	{        
		NewID = FMath::Clamp(CurrentWeaponSlot - 1, 0, NumberOfWeaponSlots - 1);

		// If we've reached index 0, loop back around to the max index
		if (CurrentWeaponSlot == 0)
		{
			NewID = NumberOfWeaponSlots - 1;
		}
	}

	if (bPerformingWeaponSwap && WeaponSwapBehaviour == EWeaponSwapBehaviour::UseNewValue)
	{
		TargetWeaponSlot = NewID;	
	}
	else if (!bPerformingWeaponSwap)
	{
		SwapWeapon(NewID);
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::PlaySwapAnimation()
{
	if (AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
	{
		if (CurrentWeapon)
		{
			if (CurrentWeapon->GetStaticWeaponData()->WeaponEquip)
			{
				FPSCharacter->GetHandsMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->WeaponEquip, 1.0f);
			}
		}
		FPSCharacter->SetWeaponEquip(true);
		FPSCharacter->SetMovementState(FPSCharacter->GetMovementState());
	}

}

void UInventoryComponent::SwapWeapon(const int SlotId)
{
	if (CurrentWeaponSlot == SlotId) { return; }
	if (!GetWeaponByID(SlotId)) { return; }

	// Returning if the target weapon is already equipped or it does not exist
	ServerSwapWeapon(SlotId);	
}

void UInventoryComponent::ServerSwapWeapon_Implementation(int SlotId)
{
	// Clearing the weapon swap timer in case it's still active
	GetWorld()->GetTimerManager().ClearTimer(ReloadRetry);

	if (!bPerformingWeaponSwap)
	{
		bPerformingWeaponSwap = true;
		TargetWeaponSlot = SlotId;
		if (CurrentWeapon)
		{
			if (CurrentWeapon->GetStaticWeaponData()->WeaponUnequip)
			{
				CurrentWeapon->StopFire();
				CurrentWeapon->SetCanFire(false);
				HandleUnequip();
				return;
			}
		}
	}

	CurrentWeaponSlot = SlotId;
	// Disabling the currently equipped weapon, if it exists
	if (CurrentWeapon)
	{
		CurrentWeapon->PrimaryActorTick.bCanEverTick = false;
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetCanFire(false);
		CurrentWeapon->StopFire();

	}

	// Swapping to the new weapon, enabling it and playing it's equip animation
	CurrentWeapon = GetWeaponByID(SlotId);
	
	PlaySwapAnimation();
	if (CurrentWeapon)
	{
		CurrentWeapon->PrimaryActorTick.bCanEverTick = true;
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetCanFire(true);
	}
	bPerformingWeaponSwap = false;
}

//// Spawns a new weapon (either from weapon swap or picking up a new weapon)
//void UInventoryComponent::UpdateWeapon(const TSubclassOf<AWeaponBase> NewWeapon, const int InventoryPosition, const bool bSpawnPickup,
//                                       const bool bStatic, const FTransform PickupTransform, const FRuntimeWeaponData DataStruct)
//{
//    // Determining spawn parameters (forcing the weapon pickup to spawn at all times)
//    FActorSpawnParameters SpawnParameters;
//    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//    if (InventoryPosition == CurrentWeaponSlot && EquippedWeapons.Contains(InventoryPosition))
//    {
//        if (bSpawnPickup)
//        {
//			DropWeapon(SpawnParameters, bStatic, PickupTransform, InventoryPosition);
//        }
//    }
//    // Spawns the new weapon and sets the player as it's owner
//    AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(NewWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
//    if (SpawnedWeapon)
//    {
//    	// Placing the new weapon at the correct location and finishing up it's initialisation
//        SpawnedWeapon->SetOwner(GetOwner());
//    	if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
//    	{
//    		//SpawnedWeapon->AttachToComponent(FPSCharacter->GetHandsMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
//			//SpawnedWeapon->AttachToComponent(FPSCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
//			SpawnedWeapon->GetMainMeshComp()->AttachToComponent(FPSCharacter->GetHandsMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
//			SpawnedWeapon->GetTPPMeshComp()->AttachToComponent(FPSCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
//		}
//		
//        SpawnedWeapon->SetRuntimeWeaponData(DataStruct);
//        SpawnedWeapon->SpawnAttachments();
//		
//		
//        EquippedWeapons.Add(InventoryPosition, SpawnedWeapon);
//
//		// Disabling the currently equipped weapon, if it exists
//        if (CurrentWeapon)
//        {
//            CurrentWeapon->PrimaryActorTick.bCanEverTick = false;
//            CurrentWeapon->SetActorHiddenInGame(true);
//        	CurrentWeapon->StopFire();
//        }
//
//    	
//    	// Swapping to the new weapon, enabling it and playing it's equip animation
//        CurrentWeapon = EquippedWeapons[InventoryPosition];
//        CurrentWeaponSlot = InventoryPosition; 
//        
//        if (CurrentWeapon)
//        {
//            CurrentWeapon->PrimaryActorTick.bCanEverTick = true;
//            CurrentWeapon->SetActorHiddenInGame(false);
//            if (CurrentWeapon->GetStaticWeaponData()->WeaponEquip)
//            {
//            	if (AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
//	            {
//					FPSCharacter->SetWeaponEquip(true);
//            		FPSCharacter->GetHandsMesh()->GetAnimInstance()->StopAllMontages(0.1f);
//		            FPSCharacter->GetHandsMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->WeaponEquip, 1.0f);
//            		FPSCharacter->SetMovementState(FPSCharacter->GetMovementState());
//	            }
//            }
//        }
//    }
//}

void UInventoryComponent::EquipWeapon(UItemObject* ItemObject, int InventoryPosition)
{
	if (!ItemObject->WeaponData.bIsWeapon)
	{
		return;
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(ItemObject->WeaponData.WeaponReference, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);

	if (SpawnedWeapon)
	{
		// Placing the new weapon at the correct location and finishing up it's initialisation
		SpawnedWeapon->SetOwner(GetOwner());
		if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
		{
			SpawnedWeapon->GetMainMeshComp()->AttachToComponent(FPSCharacter->GetHandsMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
			SpawnedWeapon->GetTPPMeshComp()->AttachToComponent(FPSCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnedWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
		}

		SpawnedWeapon->SetRuntimeWeaponData(ItemObject->DataStruct);
		SpawnedWeapon->SpawnAttachments();

		//TODO : Primary일지 Secondary일지 체크 하기
		if (InventoryPosition == 0)
		{
			// PrimaryWeapon에 넣기
			PrimaryWeapon = SpawnedWeapon;
		}
		else
		{
			//SecondaryWeapon에 넣기
			SecondaryWeapon = SpawnedWeapon;
		}

		// Disabling the currently equipped weapon, if it exists
		if (CurrentWeapon)
		{
			CurrentWeapon->PrimaryActorTick.bCanEverTick = false;
			CurrentWeapon->SetActorHiddenInGame(true);
			CurrentWeapon->StopFire();
		}


		// Swapping to the new weapon, enabling it and playing it's equip animation
		
		//CurrentWeapon = EquippedWeapons[InventoryPosition];

		CurrentWeapon = SpawnedWeapon;
		CurrentWeaponSlot = InventoryPosition;

		if (CurrentWeapon)
		{
			CurrentWeapon->PrimaryActorTick.bCanEverTick = true;
			CurrentWeapon->SetActorHiddenInGame(false);
			if (CurrentWeapon->GetStaticWeaponData()->WeaponEquip)
			{
				if (AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
				{
					FPSCharacter->SetWeaponEquip(true);
					FPSCharacter->GetHandsMesh()->GetAnimInstance()->StopAllMontages(0.1f);
					FPSCharacter->GetHandsMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->WeaponEquip, 1.0f);
					FPSCharacter->SetMovementState(FPSCharacter->GetMovementState());
				}
			}
		}
	}
}

void UInventoryComponent::DropWeapon(FActorSpawnParameters& SpawnParameters, const bool& bStatic, const FTransform& PickupTransform, const int& InventoryPosition)
{
	// Calculating the location where to spawn the new weapon in front of the player
	FVector TraceStart = FVector::ZeroVector;
	FRotator TraceStartRotation = FRotator::ZeroRotator;

	if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
	{
		TraceStart = FPSCharacter->GetCameraComponent()->GetComponentLocation();
		TraceStartRotation = FPSCharacter->GetCameraComponent()->GetComponentRotation();
	}
	const FVector TraceDirection = TraceStartRotation.Vector();
	const FVector TraceEnd = TraceStart + TraceDirection * WeaponSpawnDistance;

	// Spawning the new pickup
	AWeaponPickup* NewPickup = GetWorld()->SpawnActor<AWeaponPickup>(CurrentWeapon->GetStaticWeaponData()->PickupReference, TraceEnd, FRotator::ZeroRotator, SpawnParameters);
	if (bStatic)
	{
		NewPickup->GetMainMesh()->SetSimulatePhysics(false);
		NewPickup->SetActorTransform(PickupTransform);
	}
	// Applying the current weapon data to the pickup
	NewPickup->SetStatic(bStatic);
	NewPickup->SetRuntimeSpawned(true);
	NewPickup->SetWeaponReference(GetWeaponByID(InventoryPosition)->GetClass());
	NewPickup->SetCacheDataStruct(GetWeaponByID(InventoryPosition)->GetRuntimeWeaponData());
	NewPickup->SpawnAttachmentMesh();
	
	GetWeaponByID(InventoryPosition)->Destroy();
}

void UInventoryComponent::ServerRemoveEquipItems_Implementation(int index)
{
	HandleRemoveEquipItems(index);
}

void UInventoryComponent::RemoveEquipItems(int index)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerRemoveEquipItems(index);
	}
	else
	{
		HandleRemoveEquipItems(index);
	}	
}

void UInventoryComponent::HandleRemoveEquipItems(int index)
{
	if (index == 0)
	{
		if (PrimaryWeapon)
		{
			PrimaryWeapon->Destroy();
			PrimaryWeapon = nullptr;
		}
	}
	else if (index == 1)
	{
		if (SecondaryWeapon)
		{
			SecondaryWeapon->Destroy();
			SecondaryWeapon = nullptr;
		}
	}
}

AWeaponBase* UInventoryComponent::GetWeaponByID(const int WeaponID) const
{
	if (WeaponID == 0)
	{
		return PrimaryWeapon;
	}
	else if(WeaponID == 1)
	{
		return SecondaryWeapon;
	}
	return nullptr;
}

FText UInventoryComponent::GetCurrentWeaponRemainingAmmo() const
{
	if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
	{
		AFPSCharacterController* CharacterController = Cast<AFPSCharacterController>(FPSCharacter->GetController());

		if (CharacterController)	
		{
			if (CurrentWeapon != nullptr)
			{
				return FText::AsNumber(CharacterController->AmmoMap[CurrentWeapon->GetRuntimeWeaponData()->AmmoType]);
			}
			UE_LOG(LogProfilingDebugging, Log, TEXT("Cannot find Current Weapon"));
			return FText::AsNumber(0);
		}
		UE_LOG(LogProfilingDebugging, Error, TEXT("No character controller found in UInventoryComponent"))
		return FText::FromString("Err");
	}
	UE_LOG(LogProfilingDebugging, Error, TEXT("No player character found in UInventoryComponent"))
	return FText::FromString("Err");
}

// Passing player inputs to WeaponBase
void UInventoryComponent::StartFire()
{
	ServerFire();
	StartFireTimer();
}

void UInventoryComponent::ServerFire_Implementation()
{
	int32 RandomSeed = FMath::Rand();
	MulticastFire(RandomSeed);
}

void UInventoryComponent::MulticastFire_Implementation(int32 RandomSeed)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire(RandomSeed);
	}
}

// Passing player inputs to WeaponBase
void UInventoryComponent::StopFire()
{
	ServerStopFire();
	StopFireTimer();
}

void UInventoryComponent::ServerStopFire_Implementation()
{
	MulticastStopFire();
}

void UInventoryComponent::MulticastStopFire_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

// Passing player inputs to WeaponBase
void UInventoryComponent::Reload()
{
    if (CurrentWeapon)
    {
        if (!CurrentWeapon->Reload())
        {
	        switch (ReloadFailedBehaviour)
	        {
	        case EReloadFailedBehaviour::Retry:
	        	{
		            if (MaxRetryAmount == 0)
		            {
						GetWorld()->GetTimerManager().SetTimer(ReloadRetry, this, &UInventoryComponent::Reload, RetryInterval, false, RetryInterval);
						if (bDrawDebug)
						{
							GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Retrying Reload"));	
						}
		            	break;
		            }
		            if (RetryAmount < MaxRetryAmount)
		            {
						GetWorld()->GetTimerManager().SetTimer(ReloadRetry, this, &UInventoryComponent::Reload, RetryInterval, false, RetryInterval);
						if (bDrawDebug)
						{
							GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Retrying Reload"));	
						}
		            	RetryAmount++;
						break;
		            }
	        		RetryAmount = 0;
	        		break;
	        	}

	        case EReloadFailedBehaviour::ChangeState:
	        	{
	        		AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner());
	        		FPSCharacter->SetMovementState(TargetMovementState);
	        		Reload();
	        		break;
	        	}

	        case EReloadFailedBehaviour::HandleInBP:
	        	{
	        		EventFailedToReload.Broadcast();	
	        		break;
	        	}

	        case EReloadFailedBehaviour::Ignore:
		        {
			        // Ignoring it, obviously :)
	        		break;
		        }

	        default: { break; }
	        } 
        }
    }
}

void UInventoryComponent::Inspect()
{
	if (CurrentWeapon)
	{
			if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
			{
				if (CurrentWeapon->GetStaticWeaponData()->HandsInspect)
				{
					FPSCharacter->GetHandsMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->HandsInspect, 1.0f);
				}
				if (CurrentWeapon->GetStaticWeaponData()->WeaponInspect)
				{
					CurrentWeapon->GetMainMeshComp()->PlayAnimation(CurrentWeapon->GetStaticWeaponData()->WeaponInspect, false);
				}
			}
	}
}

void UInventoryComponent::HandleUnequip()
{
	if (CurrentWeapon)
	{
		if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
		{
			if (CurrentWeapon->GetStaticWeaponData()->TPPWeaponEquip)
			{
				FPSCharacter->GetMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->TPPWeaponEquip, 1.0f);
			}
			if (CurrentWeapon->GetStaticWeaponData()->WeaponUnequip)
			{
				const float AnimTime = FPSCharacter->GetHandsMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->WeaponUnequip, 1.0f);
				GetWorld()->GetTimerManager().SetTimer(WeaponSwapDelegate, this, &UInventoryComponent::UnequipReturn, AnimTime, false, AnimTime);
			}
		}
	}
}

void UInventoryComponent::UnequipReturn()
{
	SwapWeapon(TargetWeaponSlot);
}

void UInventoryComponent::StartFireTimer()
{
	if (CurrentWeapon && CurrentWeapon->CanFire())
	{
		GetOwner()->GetWorldTimerManager().SetTimer(
			ShotDelay,
			this,
			&UInventoryComponent::FinishFireTimer,
			(60 / CurrentWeapon->GetStaticWeaponData()->RateOfFire));
	}
}

void UInventoryComponent::FinishFireTimer()
{
	if (CurrentWeapon->GetStaticWeaponData()->bAutomaticFire)
	{
		StartFire();
	}
}

void UInventoryComponent::StopFireTimer()
{
	if (!CurrentWeapon)
	{
		return;
	}
	if (CurrentWeapon->GetStaticWeaponData()->bPreventRapidManualFire && CurrentWeapon->GetHasFiredRecently())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(SpamFirePreventionDelay);
		const float TimeRemaining = GetOwner()->GetWorldTimerManager().GetTimerRemaining(ShotDelay);
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(TimeRemaining));
		if (TimeRemaining > 0.0f)
		{
			CurrentWeapon->SetHasFiredRecently(false);
			bIsWeaponReadyToFire = false;
			GetOwner()->GetWorldTimerManager().SetTimer(SpamFirePreventionDelay, this, &UInventoryComponent::ReadyToFire, GetOwner()->GetWorldTimerManager().GetTimerRemaining(ShotDelay), false, GetOwner()->GetWorldTimerManager().GetTimerRemaining(ShotDelay));
		}
	}
	GetOwner()->GetWorldTimerManager().ClearTimer(ShotDelay);
}

void UInventoryComponent::ReadyToFire()
{
	bIsWeaponReadyToFire = true;
}


void UInventoryComponent::OnRep_CurrentWeapon()
{
	if (AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
	{
		FPSCharacter->SetWeaponEquip(true);

		if (CurrentWeapon)
		{
			CurrentWeapon->PrimaryActorTick.bCanEverTick = true;
			CurrentWeapon->SetActorHiddenInGame(false);
			CurrentWeapon->SetCanFire(true);
			CurrentWeapon->GetMainMeshComp()->AttachToComponent(FPSCharacter->GetHandsMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
			CurrentWeapon->GetTPPMeshComp()->AttachToComponent(FPSCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->GetStaticWeaponData()->WeaponAttachmentSocketName);
			if (CurrentWeapon->GetStaticWeaponData()->WeaponEquip)
			{
				FPSCharacter->GetHandsMesh()->GetAnimInstance()->Montage_Play(CurrentWeapon->GetStaticWeaponData()->WeaponEquip, 1.0f);
			}
			FPSCharacter->SetMovementState(FPSCharacter->GetMovementState());
		}
	}
}

void UInventoryComponent::OnRep_PerformingWeaponSwap()
{
	if (!bPerformingWeaponSwap)
	{
		return;
	}
	HandleUnequip();
}

void UInventoryComponent::SetupInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	if (FiringAction)
	{
		// Firing
		PlayerInputComponent->BindAction(FiringAction, ETriggerEvent::Started, this, &UInventoryComponent::StartFire);
		PlayerInputComponent->BindAction(FiringAction, ETriggerEvent::Completed, this, &UInventoryComponent::StopFire);
	}
        
	if (PrimaryWeaponAction)
	{
		// Switching to the primary weapon
		PlayerInputComponent->BindAction(PrimaryWeaponAction, ETriggerEvent::Started, this, &UInventoryComponent::SwapWeapon<0>);
	}

	if (SecondaryWeaponAction)
	{            
		// Switching to the secondary weapon
		PlayerInputComponent->BindAction(SecondaryWeaponAction, ETriggerEvent::Started, this, &UInventoryComponent::SwapWeapon<1>);
	}

	if (ReloadAction)
	{
		// Reloading
		PlayerInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UInventoryComponent::Reload);
	}

	if (ScrollAction)
	{
		// Scrolling through weapons
		PlayerInputComponent->BindAction(ScrollAction, ETriggerEvent::Started, this, &UInventoryComponent::ScrollWeapon);
	}

	if (InspectWeaponAction)
	{
		// Playing the inspect animation
		PlayerInputComponent->BindAction(InspectWeaponAction, ETriggerEvent::Started, this, &UInventoryComponent::Inspect);
	}
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(UInventoryComponent, TargetWeaponSlot);
	DOREPLIFETIME(UInventoryComponent, bPerformingWeaponSwap);
	DOREPLIFETIME(UInventoryComponent, CurrentWeaponSlot);
	DOREPLIFETIME(UInventoryComponent, CurrentWeapon);
	DOREPLIFETIME(UInventoryComponent, PrimaryWeapon);
	DOREPLIFETIME(UInventoryComponent, SecondaryWeapon);
}
