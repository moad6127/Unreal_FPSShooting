// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyAIBot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "WeaponBase.h"
#include "Components/InventoryComponent.h"

AEnemyAIController::AEnemyAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIPeripheralVisionAngleDegrees;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	// TODO : Hearing 도 추가하기


	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::UpdatePercention);
}


void AEnemyAIController::UpdatePercention(const TArray<AActor*>& UpdatedActors)
{
	//TODO : Sense를 구별하면서 Update를 확인하기
	for (auto Actor : UpdatedActors)
	{
		FAIStimulus AIStimulus;
		// SightActors들 모아두기
		AIStimulus = CanSenseActor(Actor, SightConfig->GetSenseImplementation());
		if (AIStimulus.WasSuccessfullySensed())
		{
			Blackboard->SetValueAsObject("Target", Actor);
		}
		else if (!AIStimulus.WasSuccessfullySensed())
		{
			Blackboard->SetValueAsObject("Target", nullptr);
		}
	}
}

void AEnemyAIController::HandleSensedSight(AActor* UpdatedActorr)
{
	
}

FAIStimulus AEnemyAIController::CanSenseActor(AActor* Actor, TSubclassOf<UAISense> SenseToUse)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;
	AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	TSubclassOf<UAISense> LastSensedStimulusClass;
	for (auto& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);
		if (SenseToUse == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;
			return ResultStimulus;
		}
	}

	return ResultStimulus;
}

void AEnemyAIController::ShootEnemy()
{
	AEnemyAIBot* AIBot = Cast<AEnemyAIBot>(GetPawn());
	AWeaponBase* BotWeapon = AIBot ? AIBot->GetInventoryComponent()->GetCurrentWeapon() : nullptr;
	if (BotWeapon == nullptr)
	{
		return;
	}

	AFPSCharacter* EnemyCharacter;
	EnemyCharacter = Cast<AFPSCharacter>(Blackboard->GetValueAsObject("Target"));

	bool CanFire = false;
	if (EnemyCharacter && EnemyCharacter->IsAlive() && BotWeapon->CanFire())
	{
		if (LineOfSightTo(EnemyCharacter, AIBot->GetActorLocation()))
		{
			CanFire = true;
		}
	}

	if (CanFire)
	{
		BotWeapon->StartFire();
	}
	else
	{
		BotWeapon->StopFire();
	}
}
