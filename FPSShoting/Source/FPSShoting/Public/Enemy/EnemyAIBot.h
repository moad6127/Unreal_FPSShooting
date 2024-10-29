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

	UPROPERTY(EditAnywhere, Category = Behavior)
	TObjectPtr<UBehaviorTree> BotBehavior;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;
};
