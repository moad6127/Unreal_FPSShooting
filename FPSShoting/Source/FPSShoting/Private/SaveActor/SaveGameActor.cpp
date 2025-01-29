// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveActor/SaveGameActor.h"
#include "Character/SInvenFPSCharacter.h"

ASaveGameActor::ASaveGameActor()
{
	InteractionText = FText::FromString(FString("SaveActor"));
}

void ASaveGameActor::Interact(APawn* InstigatorPawn)
{
	ASInvenFPSCharacter* PlayerCharacter = Cast<ASInvenFPSCharacter>(InstigatorPawn);
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame!"));
		PlayerCharacter->SaveGame();

		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/Map/ContainerMap?listen");
		}

	}
}
