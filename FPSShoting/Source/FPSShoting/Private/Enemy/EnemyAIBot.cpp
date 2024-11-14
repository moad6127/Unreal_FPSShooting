// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIBot.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIBot::AEnemyAIBot()
{
	TeamID = 2;
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
	EnemyAIController->SetGenericTeamId(GetGenericTeamId());
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

	if (DamageCauser == nullptr || DamageCauser->GetOwner() == nullptr)
	{
		return;
	}
	EnemyAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), DamageCauser->GetOwner());
}

void AEnemyAIBot::Die()
{
	Super::Die();

	EnemyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("bDead"), bDead);
}
