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
	PrimaryActorTick.bCanEverTick = true;

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

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyCharacter)
	{
		RotateToTarget(DeltaTime);
	}
}

void AEnemyAIController::TargetRemove()
{
	SetTarget(nullptr);
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
			SetTarget(Actor);
		}
		else if (!AIStimulus.WasSuccessfullySensed())
		{
			if (CheckDistanceToTarget() > 2000.f)
			{
				SetTarget(nullptr);
			}
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

void AEnemyAIController::RotateToTarget(float DeltaTime)
{
	if (!EnemyCharacter)
	{
		return;
	}

	FVector Direction = EnemyCharacter->GetActorLocation() - GetPawn()->GetActorLocation();
	Direction.Z = 0;

	FRotator TargetRotation = Direction.Rotation();
	FRotator CurrentRotation = GetPawn()->GetActorRotation();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.f);
	GetPawn()->SetActorRotation(NewRotation);
}

void AEnemyAIController::ResetFireCooldown()
{
	AIBot = AIBot ? AIBot : Cast<AEnemyAIBot>(GetPawn());
	AWeaponBase* BotWeapon = AIBot ? AIBot->GetInventoryComponent()->GetCurrentWeapon() : nullptr;

	if (BotWeapon)
	{
		BotWeapon->StopFire();
	}

}

void AEnemyAIController::SetTarget(AActor* Target)
{
	Blackboard->SetValueAsObject("Target", Target);
	EnemyCharacter = Cast<AFPSCharacter>(Blackboard->GetValueAsObject("Target"));
}

void AEnemyAIController::ShootEnemy()
{
	AIBot = AIBot ? AIBot : Cast<AEnemyAIBot>(GetPawn());
	AWeaponBase* BotWeapon = AIBot ? AIBot->GetInventoryComponent()->GetCurrentWeapon() : nullptr;
	if (BotWeapon == nullptr)
	{
		return;
	}


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

		float RandShootingTime;
		
		RandShootingTime = FMath::RandRange(0.3f, 1.2f);

		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			this,
			&AEnemyAIController::ResetFireCooldown,
			RandShootingTime,
			false);
	}
	else
	{
		BotWeapon->StopFire();
	}
}

float AEnemyAIController::CheckDistanceToTarget()
{
	return Blackboard->GetValueAsFloat("TargetDistance");
}
