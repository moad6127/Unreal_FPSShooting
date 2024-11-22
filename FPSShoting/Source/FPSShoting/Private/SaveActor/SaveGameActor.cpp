// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveActor/SaveGameActor.h"
#include "Character/SInvenFPSCharacter.h"

ASaveGameActor::ASaveGameActor()
{
	InteractionText = FText::FromString(FString("SaveActor"));
}

void ASaveGameActor::Interact()
{
	ASInvenFPSCharacter* PlayerCharacter = Cast<ASInvenFPSCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->SaveGame();
	}
}
