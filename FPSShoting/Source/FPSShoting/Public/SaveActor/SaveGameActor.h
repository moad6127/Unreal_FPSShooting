// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "SaveGameActor.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOTING_API ASaveGameActor : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	ASaveGameActor();
	virtual void Interact(APawn* InstigatorPawn) override;

};
