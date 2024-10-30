// Copyright 2022 Ellie Kelemen. All Rights Reserved.


#include "Components/HealthComponent.h"
#include "Engine/World.h"
#include "FPSCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	Health = DefaultHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		// Binding our handling function to the owner's OnTakeAnyDamage
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		// Health is already at most 0, so no need to perform any calculations
		return;
	}


	// Updating health, clamped between 0 and 100
	Health = FMath::Clamp(Health - Damage, 0.0f, 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), Damage);
	UE_LOG(LogTemp, Warning, TEXT("Healt : %f"), Health);
	
	// Broadcasting our new health
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (Health <= 0.f)
	{
		Die();
	}

}

void UHealthComponent::Die()
{
	AFPSCharacter* OwnerCharcter = Cast<AFPSCharacter>(GetOwner());
	if (OwnerCharcter)
	{
		OwnerCharcter->Die();
	}
}
