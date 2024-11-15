// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
struct FAIStimulus;
class UAISense;

UCLASS()
class FPSSHOTING_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:

	UFUNCTION(BlueprintCallable, Category = "Behavior")
	void ShootEnemy();

	UFUNCTION()
	void UpdatePercention(const TArray<AActor*>& UpdatedActors);

	void HandleSensedSight(AActor* UpdatedActorr);

	FAIStimulus CanSenseActor(AActor* Actor, TSubclassOf<UAISense> SenseToUse);

	


	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AISightRadius = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AILoseSightRadius = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AIPeripheralVisionAngleDegrees = 60.f;
};
