#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)
class AURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// 重写IAbilitySystemInterface中的虚函数用于获取能力系统组件，正如该函数名一样。
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// 类似的，属性也可以这样写一个get方法
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 武器插槽
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapons;

	// 创建两个指针用于后续的GAS。角色的属性将会使用这些，但是敌人的属性将写到敌人类内部。
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo(); // 初始化Actor信息，绑定AbilitySystemComponent委托等

	/*属性初始化流程：
		1. 你的属性属于下面三个的哪种，如果都不属于就新开一个TSubclassOf<UGameplayEffect>变量用于在蓝图中承载你的GameplayEffectBlueprint。
		2. 在InitializeDefaultAttributes()中调用ApplyEffectToSelf()函数，传入你的TSubclassOf变量和等级参数即可。
		3. 蓝图那边创建一个GameplayEffectBlueprint，添加你需要的属性初始化效果即可。
		4. 记得在AttributeSet.h中添加你的属性变量，具体步骤在AttributeSet.h文件中有说明。
	这种属性创建方法几乎适用于所有属性的初始化，无论该属性是否基于其他属性。
	另外，如果想要在C++中创建基于其他属性的属性，需要在编辑器中添加MMC的C++类。参考MMC_MaxHealth.h文件。
	*/

	// 主要属性类，用于初始化角色主要属性，在蓝图中设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// 次要属性类，用于初始化角色次要属性，在蓝图中设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	// 基础属性类，用于初始化角色基础属性，在蓝图中设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// 初始化属性
	void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const; // 应用GameplayEffect到自己

};
