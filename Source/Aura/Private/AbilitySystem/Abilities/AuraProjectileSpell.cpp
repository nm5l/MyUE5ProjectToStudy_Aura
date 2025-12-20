// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);



}

void UAuraProjectileSpell::SpawnProjectile()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation(); // 获取插槽位置

		FTransform  SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation); // 设置生成位置
		// TODO：设置生成旋转，缩放应该不需要

		// 使用SpawnActorDeferred来生成飞弹
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(	// SpawnActorDeferred用于延迟生成Actor，以便在生成之前进行一些初始化设置
			ProjectileClass,								// 飞弹类
			SpawnTransform,									// 生成变换:位置、旋转、缩放
			GetOwningActorFromActorInfo(),					// 设置拥有者为能力的拥有者
			Cast<APawn>(GetOwningActorFromActorInfo()), // 设置Instigator为拥有者的Pawn
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn); // 碰撞处理方法:总是生成
		// TODO：给飞弹设置一些GameplayEffectSpec


		Projectile->FinishSpawning(SpawnTransform); // 完成生成过程
	}
}
