// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIBot.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIBot::AEnemyAIBot()
{

}

void AEnemyAIBot::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority())
	{
		return;
	}
	EnemyAIController = Cast<AEnemyAIController>(NewController);
	EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BotBehavior->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BotBehavior);
}

void AEnemyAIBot::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.Clear();
	OnTakeAnyDamage.AddDynamic(this, &AEnemyAIBot::HandleTakeAnyDamage);
}

void AEnemyAIBot::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Super::HandleTakeAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);


}
