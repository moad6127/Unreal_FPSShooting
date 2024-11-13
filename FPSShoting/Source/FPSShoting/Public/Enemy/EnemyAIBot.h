// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SInvenFPSCharacter.h"
#include "EnemyAIBot.generated.h"

/**
 * 
 */

class AEnemyAIController;
class UBehaviorTree;

UCLASS()
class FPSSHOTING_API AEnemyAIBot : public ASInvenFPSCharacter
{
	GENERATED_BODY()

public:	


	AEnemyAIBot();
	virtual void PossessedBy(AController* NewController) override;


protected:
	virtual void BeginPlay() override;

	virtual void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser) override;

	virtual void Die() override;

	UPROPERTY(EditAnywhere, Category = Behavior)
	TObjectPtr<UBehaviorTree> BotBehavior;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;
};
