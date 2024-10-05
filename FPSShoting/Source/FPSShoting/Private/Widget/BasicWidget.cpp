// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BasicWidget.h"
#include "Widget/BasicWidgetController.h"

void UBasicWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
