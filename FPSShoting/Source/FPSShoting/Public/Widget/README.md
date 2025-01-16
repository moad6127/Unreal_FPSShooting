# FPS_Shooting_Widgets


```c++
void AFPSHUD::InitHUD(APlayerController* PC, UEquipInventoryComponent* SInventoryComponent)
{

    checkf(InventoryWidgetClass, TEXT("Inventory Widget Class uninitialized, fill out BP_FPSHUD"));
    checkf(WidgetControllerClass, TEXT("Inventory Widget Controller Class uninitialized, fill out BP_FPSHUD"));
    checkf(InteractWidgetClass, TEXT("Interact Widget Class uninitialized, fill out BP_FPSHUD"));

    const FWidgetControllerParams WidgetControllerParmas(PC, SInventoryComponent);

    UBasicWidgetController* BWidgetController = GetWidgetController(WidgetControllerParmas);

    InventoryWidget = CreateWidget<UBasicWidget>(GetWorld(), InventoryWidgetClass);
    InteractWidget = CreateWidget<UBasicWidget>(GetWorld(), InteractWidgetClass);

    InventoryWidget->SetWidgetController(BWidgetController);
    InteractWidget->SetWidgetController(WidgetController);

    InventoryWidget->AddToViewport();
    InteractWidget->AddToViewport();

    InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    bIsInventoryVisible = false;
}
```
> HUD 파일의 CPP의 Init으로 Character클래스의 Beginplay에서 HUD의 Init을 호출하면 인벤토리를 초기화 할수 있다.
>

```C++
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemObject* Payload, FIntPoint Location);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	void DropItem(UItemObject* ItemToDrop);

	UFUNCTION(BlueprintCallable)
	bool ReplaceItem(UItemObject* ItemToReplace, FIntPoint InLocation);	

	UFUNCTION(BlueprintCallable)
	void RotateItem(UItemObject* ItemToRotate);

	UFUNCTION(BlueprintCallable)
	void EquipItem(UItemObject* InItem);

	UFUNCTION(BlueprintCallable)
	bool UnEquipItem(UItemObject* ItemToUnEquip);

	UFUNCTION(BlueprintCallable)
	void SplitItem(UItemObject* SplitToItem);
```

> WidgetController의 기능들로 BlueprintCallable을 사용해서 블루프린트에서 호출할수 있도록 만들어 Widget에서 사용한다.



![FPSShooting_InventoryWidget](https://github.com/user-attachments/assets/0fd787a8-a934-47c9-adeb-b04630b4693e)

>Widget들은 Inventory, InventoryGird, ItemWidget, EquipmentWidget, EquipSlot등이 존재하며
InventoryGrid와 ItemWidget들로 인벤토리를 표시하며 EquipmentWidget과 합쳐 Inventory를 구성하고 있다.



![ScreenShot00002](https://github.com/user-attachments/assets/c207c16b-78e7-4dd1-b3ba-c4702915d09e)

> 인벤토리에 무기를 장착하고 아이템을 추가한 모습이다. 
> 
