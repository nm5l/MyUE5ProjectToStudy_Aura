

#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false; // 不需要每帧调用Tick函数

	// 创建并配置飞弹的移动组件
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");	// 创建球形碰撞体子对象
	SetRootComponent(Sphere);											// 设置球形碰撞体为根组件
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 只进行查询碰撞检测，不进行物理碰撞检测
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);				// 忽略所有碰撞通道
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);	 // 与静态世界物体重叠时触发重叠事件
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // 与动态世界物体重叠时触发重叠事件
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 		// 与角色重叠时触发重叠事件

	// 创建并配置飞弹的移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectleMovement"); // 创建飞弹移动组件子对象
	ProjectileMovement->InitialSpeed = 550.f; // 设置初始速度
	ProjectileMovement->MaxSpeed = 550.f;     // 设置最大速度
	ProjectileMovement->ProjectileGravityScale = 0.f; // 不受重力影响



}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap); // 绑定重叠事件

}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

