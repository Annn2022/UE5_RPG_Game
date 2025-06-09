// ANNN PRO


#include "FAuraGameplayTags.h"
#include "GameplayTagsManager.h"


FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	Instance.Attribute_Primary_Strength = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
								FString("护甲"));

	Instance.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),
								FString("护甲"));

	Instance.Attribute_Primary_Resilience = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),
								FString("护甲"));

	Instance.Attribute_Primary_Vigor = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),
								FString("护甲"));

	

	Instance.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),
								FString("护甲"));

	Instance.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),
								FString("护甲"));

	Instance.Attribute_Secondary_Armor = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
								FString("护甲"));

	Instance.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),
								FString("护甲"));

	Instance.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),
								FString("护甲"));
	
	Instance.Attribute_Secondary_CritcalHitChance = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.CritcalHitChance"),
								FString("护甲"));

	Instance.Attribute_Secondary_CritcalHitDamage = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.CritcalHitDamage"),
								FString("护甲"));

	Instance.Attribute_Secondary_CritcalHitResistance = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.CritcalHitResistance"),
								FString("护甲"));

	Instance.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),
								FString("护甲"));

	Instance.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),
								FString("护甲"));

	Instance.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),
								FString("奥术抗性"));
	Instance.Attribute_Resistance_Fire = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),
								FString("火焰抗性"));
	Instance.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),
								FString("光魔法抗性"));
	Instance.Attribute_Resistance_Physical = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),
								FString("物理抗性"));


	//输入Tag
	Instance.InputTag_LMB = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("InputTag.LMB"),
								FString("InputTag"));
	Instance.InputTag_RMB = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("InputTag.RMB"),
								FString("InputTag"));
	Instance.InputTag_1 = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("InputTag.1"),
								FString("InputTag"));
	Instance.InputTag_2 = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("InputTag.2"),
								FString("InputTag"));
	Instance.InputTag_3 = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("InputTag.3"),
								FString("InputTag"));
	Instance.InputTag_4 = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("InputTag.4"),
								FString("InputTag"));

	Instance.Damage = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Damage"),
								FString("Damage"));

	/*
	 * 伤害类型
	 */

	Instance.Damage_Fire = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Damage.Fire"),
								FString("火焰伤害"));
	Instance.Damage_Lightning = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Damage.Lightning"),
								FString("闪电伤害"));
	Instance.Damage_Arcane = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Damage.Arcane"),
								FString("奥术伤害"));
	Instance.Damage_Physical = UGameplayTagsManager::Get().
							AddNativeGameplayTag(FName("Damage.Physical"),
								FString("物理伤害"));

	Instance.DamageTypes.Add(Instance.Damage_Fire);
	Instance.DamageTypes.Add(Instance.Damage_Lightning);
	Instance.DamageTypes.Add(Instance.Damage_Arcane);
	Instance.DamageTypes.Add(Instance.Damage_Physical);
	

	

	/*
	 * Debuffs
	 */

	Instance.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Arcane"),
		FString("Debuff for Arcane damage")
		);
	Instance.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff for Fire damage")
		);
	Instance.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Physical"),
		FString("Debuff for Physical damage")
		);
	Instance.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff for Lightning damage")
		);

	Instance.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance")
		);
	Instance.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
		);
	Instance.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
		);
	Instance.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
		);

	/*
	 * Meta Attributes
	 */

	Instance.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
		);

	/*
	 * 抗性集合
	 */
	Instance.DamageTypesToResistances.Add(Instance.Damage_Arcane, Instance.Attribute_Resistance_Arcane);
	Instance.DamageTypesToResistances.Add(Instance.Damage_Lightning, Instance.Attribute_Resistance_Lightning);
	Instance.DamageTypesToResistances.Add(Instance.Damage_Physical, Instance.Attribute_Resistance_Physical);
	Instance.DamageTypesToResistances.Add(Instance.Damage_Fire, Instance.Attribute_Resistance_Fire);

	/*
	 * Map of Damage Types to Debuffs
	 */
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Arcane, Instance.Debuff_Arcane);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Lightning, Instance.Debuff_Stun);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Physical, Instance.Debuff_Physical);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Fire, Instance.Debuff_Burn);

	/*
	 * Effects
	 */

	Instance.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("受击后获得Tag")
		);

	/*
	 * Abilities
	 */

	Instance.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability - like the nullptr for Ability Tags")
		);

	Instance.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("攻击类型能力")
		);

	Instance.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag")
		);

	/*
	 * Offensive Spells
	 */

	Instance.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag")
		);

	Instance.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"),
		FString("FireBlast Ability Tag")
		);

	Instance.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag")
		);

	Instance.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
		FString("Arcane Shards Ability Tag")
		);

	/*
	 * Passive Spells
	 */
	
	Instance.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.LifeSiphon"),
			FString("Life Siphon")
			);
	Instance.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.ManaSiphon"),
			FString("Mana Siphon")
			);
	Instance.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.HaloOfProtection"),
			FString("Halo Of Protection")
			);
	

	Instance.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability")
		);

	Instance.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("有资格解锁")
		);

	Instance.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("已装备")
		);

	Instance.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("锁定中")
		);

	Instance.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("已解锁")
		);

	Instance.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None")
		);

	Instance.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("主动技能")
		);

	Instance.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("被动技能")
		);

	/*
	* Cooldown
	*/
	
	Instance.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag")
		);

	/*
	 * Combat Sockets
	 */

	Instance.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
		);

	Instance.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand")
		);
	
	Instance.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand")
		);

	Instance.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail")
		);

	/*
	 * Montage Tags
	 */

	Instance.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack 1")
		);

	Instance.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack 2")
		);

	Instance.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack 3")
		);

	Instance.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack 4")
		);

	/*
	 * Player Tags
	 */

	Instance.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Block tracing under the cursor")
		);

	Instance.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input Held callback for input")
		);

	Instance.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Block Input Pressed callback for input")
		);

	Instance.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input Released callback for input")
		);

	/*
	 * GameplayCues
	 */

	Instance.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.FireBlast"),
		FString("FireBlast GameplayCue Tag")
		);

}
