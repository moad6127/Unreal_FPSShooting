// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipInventoryComponent.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEquipInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UEquipInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

