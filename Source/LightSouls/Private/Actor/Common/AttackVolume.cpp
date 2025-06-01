// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Common/AttackVolume.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GasSystem/Common/LsCustomEventData.h"


// Sets default values
AAttackVolume::AAttackVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 创建一个新的 SceneComponent 作为根组件
	USceneComponent* NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("NewRootComponent"));
	RootComponent = NewRootComponent;

	// 创建胶囊体碰撞组件
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(RootComponent);

	// 设置默认碰撞设置
	CapsuleCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	CapsuleCollision->SetGenerateOverlapEvents(true);
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CapsuleCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Initialize bIsAttacking
	bIsAttacking = false;


	// Bind the overlap function
	// It's common to bind delegates in the constructor or BeginPlay.
	// If CapsuleCollision is always valid here (which it should be after CreateDefaultSubobject)
	if (CapsuleCollision)
	{
		CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AAttackVolume::OnAttackOverlapBegin);
	}
	
}

// Called when the game starts or when spawned
void AAttackVolume::BeginPlay()
{
	Super::BeginPlay();

	// Set collision to NoCollision initially
	if (CapsuleCollision)
	{
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void AAttackVolume::BeginWeaponDamageEvent()
{
	bIsAttacking = true;
	HitActors.Empty(); // Clear the array of hit actors

	if (CapsuleCollision)
	{
		// Set collision to QueryOnly (No Physics Collision)
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// You might also want to set the specific collision profile or object type responses here
		// For example:
		// CapsuleCollision->SetCollisionObjectType(ECC_WorldDynamic);
		// CapsuleCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		// CapsuleCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	UE_LOG(LogTemp, Warning, TEXT("BeginWeaponDamageEvent Called"));
}

void AAttackVolume::EndWeaponDamageEvent()
{
	bIsAttacking = false;
	// HitActors.Empty(); // Clearing hit actors again, as per your BP.
	// Consider if you need to clear it here or if BeginWeaponDamageEvent is sufficient.

	if (CapsuleCollision)
	{
		// Set collision back to NoCollision
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	UE_LOG(LogTemp, Warning, TEXT("EndWeaponDamageEvent Called"));
}

void AAttackVolume::OnAttackOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{

    if (!bIsAttacking)
    {
        // UE_LOG(LogTemp, Log, TEXT("攻击体积：未处于 IsAttacking 状态。忽略重叠。"));
        return;
    }

    // OtherActor 的基本有效性检查
    if (!OtherActor)
    {
        return;
    }

    AActor* MyOwner = GetOwner(); // 获取此攻击体积的拥有者
    APawn* MyInstigator = GetInstigator(); // 获取触发此攻击体积的 Pawn
	
    // 条件检查 A: OtherActor 不是攻击体积本身。
    if (OtherActor == this)
    {
        return;
    }
	
    if (OtherActor == MyOwner || OtherActor == MyInstigator)
    {
        return;
    }
	
    // 检查此 Actor 是否在本次攻击激活期间已经被击中
    if (HitActors.Contains(OtherActor))
    {
        return;
    }
	
    AController* InstigatorController = nullptr;
    if (MyInstigator) // 获取触发此攻击的 Pawn 的控制器
    {
        InstigatorController = MyInstigator->GetController();
    }
    // 如果 InstigatorController 不知何故为空，但 Owner 有控制器（这种情况对于伤害触发不太常见），则作为备用
    else if (MyOwner)
    {
       InstigatorController = MyOwner->GetInstigatorController(); // 这获取的是 Owner 的控制器
    }
	
    // 添加到 HitActors 以防止在同一次攻击激活中多次命中
    HitActors.Add(OtherActor);
	
    if (!EventTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackVolume: WeaponHitEventTag无效，无法发送GameplayEvenst"));
        return;
    }

    // 1. 创建并填充自定义事件数据 (FLsCustomEventData)
    ULsCustomEventData* CustomHitDataPayload = NewObject<ULsCustomEventData>();

    FVector SelfLocation = CapsuleCollision ? CapsuleCollision->GetComponentLocation() : GetActorLocation();
    FVector TargetLocation = OtherActor->GetActorLocation();
    CustomHitDataPayload->VectorValue = TargetLocation - SelfLocation; // 蓝图显示 Target - Self
	

    // 3. 创建 FGameplayEventData (Payload)
    FGameplayEventData EventPayloadToSend;
    EventPayloadToSend.EventTag = EventTag;
    EventPayloadToSend.Instigator = MyInstigator; // 触发者Actor
    EventPayloadToSend.Target = OtherActor;       // 事件的目标Actor
    EventPayloadToSend.OptionalObject = CustomHitDataPayload; // 设置包装后的自定义数据

	
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MyInstigator, EventTag, EventPayloadToSend);

}

// Called every frame
void AAttackVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


