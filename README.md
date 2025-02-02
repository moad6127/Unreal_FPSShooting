# Unreal_FPSShooting

언리얼 엔진5로 만들어진 멀티플레이 FPS_Shooting 게임
FAP에서 무료로 다운받을수 있는 FPSCore플러그인을 기본 Base로 잡고 여러가지를 추가해서 만들어진 FPS_Shooting게임이다.


InventorySystem과 EquipmentSystem이 존재하는 익스트랙션 FPS_ShootingGame.

Inventory에 아이템과 무기들이 들어가며 장착이 가능한것들은 Equipment창에 장착할수 있도록 만들었다.


<details><summary> 구분</summary>
<p>  

  * [FPSCorePlugin](#FPSCorePlugin)
  
  * [InventorySystem](#InventorySystem)
    
  * [AI](#AI)
    
  * [SaveData](#SaveData)

  * [MainMenu](#MainMenu)
</p>
</details>
<br/> <br>

# *FPSCorePlugin*

- [FPSCorePlugin Fap사이트](https://www.fab.com/ko/listings/8df23fcb-04a1-4dac-8e1a-c0ed4cf56e98)
- [FPSCorePlugins](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Plugins/FPSCore)

> 게임의 기본적인 Base로 삼은 플러그인으로 FAB에서 얻은 플러그인을 사용해서 게임을 제작하는 경험을 위해 플러그인을 게임에 넣었다.
> 기본적으로 BaseWeapon, FPSChracter, Ammo등이 존재하며 Interact기능을 담당하여 사용해서 Actor들의 기능을 사용할수 있고, InventoryComponent를 통해 총의 장착과 발사를 담당하고 있다.
> Character의 이동과 달리기, 슬라이드등이 구성되어 있고, 총의 발사와 재장전, 총마다의 반동을 추가하여 사용할수 있도록 만들어져 있다.
> 기본 플러그인은 멀티플레이를 지원하지 않기 때문에 멀티플레이 기능을 위해 직접 멀티플레이에 사용되는 Replicate와 RPC등의 코드를 추가해서 멀티플레이에서의 캐릭터의 움직임과 총의 발사등을 구현 하였다.

# *InventorySystem*

![EquipmentInventory](https://github.com/user-attachments/assets/02b4fe7c-a96d-4e63-8d1c-542a66488970)


- [Component헤더](https://github.com/moad6127/Unreal_FPSShooting/blob/master/FPSShoting/Plugins/FPSCore/Source/FPSCore/Public/Components/EquipInventoryComponent.h)      
- [ComponentC++](https://github.com/moad6127/Unreal_FPSShooting/blob/master/FPSShoting/Plugins/FPSCore/Source/FPSCore/Private/Components/EquipInventoryComponent.cpp)

> Inventory와 Equipment를 담당하는 Component로 Character의 아이템의 획득, 장착, Drop등을 담당하는 기능을 하고 있다.
> 멀티플레이환경에서도 사용이 가능하도록 RPC와 Replicate를 활용해 장착과 획득이 가능하다.



![FPSShooting_InventoryWidget](https://github.com/user-attachments/assets/0fd787a8-a934-47c9-adeb-b04630b4693e)

- [Widgets 헤더](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Public/Widget)
- [Widgets CPP](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Private/Widget)

> UI와 MVC모델을 사용해서 WidgetController를 사용해서 Widget의 기능을 담당하고 있다.
> Widget의 CPP에는 기능은 존재하지 않고 단순히 Controller가 지정되면 블루프린트로 이벤트를 보내는 기능이 되어있다.
> Widget에서 사용될 기능들은 WidgetController에서 사용 되며 UI적인 부분은 블루프린트로 설정되며 내부의 기능들은 Controller의 CPP로 만들어 두었다.




# *AI*
- [AI h](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Public/Enemy)    
- [AI cpp](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Private/Enemy)


```C++
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

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::UpdatePercention);

```
> AIPerception을 사용하도록 Controller에서 초기화및 기본 설정하기


``` C++
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
			// TODO : 어그로를 해제하는 코드 만들기
			// 시간이 지나거나, 일정부분 멀어지면 어그로가 해제되도록 만들어보기
			if (CheckDistanceToTarget() > 2000.f)
			{
				SetTarget(nullptr);
			}
		}
	}
}
```

> AIPerception으로 반응이 들어오면 해당 함수를 통해 Sense를 파악한후 Target을 설정하기



![FPSShooting_AIBehavior](https://github.com/user-attachments/assets/f29e6b34-0727-4e3d-a1a1-ca19f3e8de04)


>AIPerception의 기능들을 사용해서 시야에 플레이어가 보일경우 반응하도록 만들어 사용한다.
>반응할시 AI에 Target이 설정되며 Target의 반경으로 이동한후 일정 반경이 되면 공격을 준비한다.
>공격을 진행할때 EQS를 사용해 공격할시 위치를 잡고 Target을 향해 공격한다.


# *SaveData*


 - [SaveGame H](https://github.com/moad6127/Unreal_FPSShooting/blob/master/FPSShoting/Source/FPSShoting/Public/GameMode/FPSSaveGame.h)


> Character의 SaveGame함수
``` C++
void ASInvenFPSCharacter::SaveGame()
{

	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("InGameMode!"));
		UFPSSaveGame* SaveData = GameMode->GetSaveData();
		UE_LOG(LogTemp, Warning, TEXT("SaveDataGet!"));
		if (SaveData == nullptr)
		{
			return;
		}
		//TODO : Character의 인벤, 장비창 세이브 하기
		SaveData->InventoryItems.Empty();
		//TODO : ItemObject를 Copy해서 만들어도 인벤토리에 이상하게 들어간다,
		// 구조체를 만들어서 아이템의 정보를 저장해 사용해야 할것같다.

		// Inventory아이템 저장하기
		for (UItemObject* Item : GetEquipInventoryComponent()->GetInventoryItems())
		{
			FItemSaveData ItemData;
			ItemData.ItemID = Item->ID;
			ItemData.ItemQuantity = Item->ItemQuantity;
			ItemData.DataStruct = Item->DataStruct;
			ItemData.ItemLocation = Item->GetItemItemLocation();
			ItemData.bEquipped = false;

			SaveData->InventoryItems.Add(ItemData);
		}

		//TODO : 장착된 아이템 저장하기
		EquipItemSave(SaveData);

		UE_LOG(LogTemp, Warning, TEXT("PlayerSaveGameFunc!"));
		GameMode->SaveGame(SaveData);
	}
}

```

> GameMode에서 Save함수
``` C++
void AFPSGameModeBase::SaveGame(UFPSSaveGame* SaveGame)
{
	DeleteSlot(SaveGame);
	UGameplayStatics::SaveGameToSlot(SaveGame, SaveGame->SlotName, SaveGame->SlotIndex);
}

```
> SaveGame 파일을 사용해서 플레이어의 인벤토리와 장착아이템들을 저장한다.
> Save를 담당하는 Actor에서 Save 시퀀스를 진행하면 Character에서 Save함수를 진행한다.
> Character의 인벤토리와 장착 아이템들을 확인한후 Load할때 필요한 정보들을 담아둔 Struct인 FSaveStruct 구조체에 정보들을 담아둔후 GameMode에서 저장하도록 만든다.


> Load 함수
``` C++
void ASInvenFPSCharacter::LoadGame()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this));
	{
		if (GameMode)
		{
			UFPSSaveGame* SaveData = GameMode->GetSaveData();
			if (SaveData == nullptr)
			{
				return;
			}
			UDataTable* ItemDataTable = GameMode->ItemDataTable;
			if (!ItemDataTable)
			{
				return;
			}
			//TODO : Character의 인벤, 장비창 로드하기
			for (FItemSaveData Data : SaveData->InventoryItems)
			{
				const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(Data.ItemID, Data.ItemID.ToString());
				if (!ItemData)
				{
					return;
				}
				UItemObject* ItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());
				//TODO : ItemValueSet
				ItemObject->ID = ItemData->ID;
				ItemObject->SlotType = ItemData->SlotType;
				ItemObject->ItemQuantity = Data.ItemQuantity;
				ItemObject->ItemNumbericData = ItemData->ItemNumbericData;
				ItemObject->Asset = ItemData->Asset;
				ItemObject->ItemName = ItemData->ItemName;
				ItemObject->WeaponData = ItemData->WeaponData;
				ItemObject->SetItemSizeX(ItemData->SizeX);
				ItemObject->SetItemSizeY(ItemData->SizeY);
				ItemObject->DataStruct = Data.DataStruct;
				ItemObject->SetItemItemLocation(Data.ItemLocation);

				//장착된 상태였으면 장착하도록 만들기
				if (Data.bEquipped)
				{
					GetEquipInventoryComponent()->AddLoadedEquipItem(ItemObject);
				}
				else
				{
					GetEquipInventoryComponent()->AddLoadedInventoryItem(ItemObject);
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("PlayerLoadGameFunc!"));

		}
	}
}
```
> Load는 Save된 Data를 바탕으로 새로운 ItemObject를 생성해서 Data의 값들을 주입해 아이템들을 Load한다.
> 이때, Load로 생성된 아이템들을 바로 Inventory에 추가하지 않고 Component에 따로 저장해둔 다음 Component의 초기화가 모두 진행된후 Component의 Beginpaly함수에서 Load아이템을 추가한느 함수를 호출해 아이템들을 Load하게 된다.
>  



# *MainMenu*


![Image](https://github.com/user-attachments/assets/378352d9-490a-44df-ad0c-c442995de5da)

> ContainerMap을 통해서 창고와 플레이어의 인벤토리를 저장한후 초기 장비들을 설정한다.
> ContainerMap에서 사용할 GameMode와 Controller클래스를 생성하고 Controller를 사용해 UI를 구축해 화면에 표시하도록 만든다.
> UI들은 기존의 Inventory와 Equipment 위젯들을 사용해서 만들어 사용한후 Save파일을 이용해서 레벨이 변경되도 유지하도록 만든다.

