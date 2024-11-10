// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOTING_API UBasicWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//~ Function
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	//~ Function

	//~ Value
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	//~ Value
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
