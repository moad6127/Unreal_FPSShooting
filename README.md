# Unreal_FPSShooting

언리얼 엔진5로 만들어진 멀티플레이 FPS_Shooting 게임
FAP에서 무료로 다운받을수 있는 FPSCore플러그인을 기본 Base로 잡고 여러가지를 추가해서 만들어진 FPS_Shooting게임이다.


InventorySystem과 EquipmentSystem이 존재하는 익스트랙션 FPS_ShootingGame.

Inventory에 아이템과 무기들이 들어가며 장착이 가능한것들은 Equipment창에 장착할수 있도록 만들었다.


<details><summary> 구분</summary>
<p>  

  * [FPSCorePlugin](#FPSCorePlugin)
  
  * [InventorySystem&&EquipmentSystem](#InventorySystem&&EquipmentSystem)
    
  * [AI](#AI)
  
  * [SaveData](#SaveData)
    
</p>
</details>
<br/> <br>

# *FPSCorePlugin*

[FPSCorePlugin Fap사이트](https://www.fab.com/ko/listings/8df23fcb-04a1-4dac-8e1a-c0ed4cf56e98)

[FPSCorePlugins](https://github.com/moad6127/Unreal_FPSShooting/tree/master/FPSShoting/Plugins/FPSCore)

> 게임의 기본적인 Base로 삼은 플러그인으로 FAB에서 얻은 플러그인을 사용해서 게임을 제작하는 경험을 위해 플러그인을 게임에 넣었다.
> 기본적으로 BaseWeapon, FPSChracter, Ammo등이 존재하며 Interact기능을 담당하여 사용해서 Actor들의 기능을 사용할수 있고, InventoryComponent를 통해 총의 장착과 발사를 담당하고 있다.
> Character의 이동과 달리기, 슬라이드등이 구성되어 있고, 총의 발사와 재장전, 총마다의 반동을 추가하여 사용할수 있도록 만들어져 있다.
> 기본 플러그인은 멀티플레이를 지원하지 않기 때문에 멀티플레이 기능을 위해 직접 멀티플레이에 사용되는 Replicate와 RPC등의 코드를 추가해서 멀티플레이에서의 캐릭터의 움직임과 총의 발사등을 구현 하였다.

# *InventorySystem&&EquipmentSystem*



# *AI*

# *SaveData*



