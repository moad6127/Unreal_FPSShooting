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


- [Widgets 헤더](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Public/Widget)
- [Widgets CPP](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Private/Widget)

> UI와 MVC모델을 사용해서 WidgetController를 사용해서 Widget의 기능을 담당하고 있다.
> Widget의 CPP에는 기능은 존재하지 않고 단순히 Controller가 지정되면 블루프린트로 이벤트를 보내는 기능이 되어있다.
> Widget에서 사용될 기능들은 WidgetController에서 사용 되며 UI적인 부분은 블루프린트로 설정되며 내부의 기능들은 Controller의 CPP로 만들어 두었다.




# *AI*
- [AI h](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Public/Enemy)    
- [AI cpp](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Source/FPSShoting/Private/Enemy)


![FPSShooting_AIBehavior](https://github.com/user-attachments/assets/f29e6b34-0727-4e3d-a1a1-ca19f3e8de04)

>AIPerception의 기능들을 사용해서 시야에 플레이어가 보일경우 반응하도록 만들어 사용한다.
>반응할시 AI에 Target이 설정되며 Target의 반경으로 이동한후 일정 반경이 되면 공격을 준비한다.
>공격을 진행할때 EQS를 사용해 공격할시 위치를 잡고 Target을 향해 공격한다.

![FPSShooting_TakeDamage](https://github.com/user-attachments/assets/b40ba171-16aa-46a4-9892-3f63177d0c99)
> 플레이어가 AI를 향해 공격해서 적중하였을 경우 공격한 플레이어를 Target으로 설정한후 BehaviorTree를 진행하게 만든다.


# *SaveData*



