#pragma once

#include "CoreMinimal.h"
#include "Blaster/HUD/BlaserHUD.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ABlasterCharacter;//BlasterCharacter拥有对CombatComponent类成员的绝对访问权，包括protected和private
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class AWeapon* WeaponToEquip);
protected:
	virtual void BeginPlay() override;
	
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	void Fire();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);
	
private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	UPROPERTY()
	class ABlasterHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;//这个变量需要同步，用于客户端角色更新动画蓝图，切换动画蓝图到Unequipped状态机

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;
	
	bool bFireButtonPressed;

	/**
	 * HUD and crosshairs
	 */

	// the amount that contributes to our crosshair spread based on velocity
	float CrosshairVelocityFactor;
	// the amount that contributes to our crosshair spread based on IsFalling
	float CrosshaiInAirFactor;
	float CrosshairsAimFactor;
	float CrosshairsShootingFactor;

	// 对于每个客户端上的本地控制角色，每帧进行一次Trace，并将Trace结果存储到该变量，用于绘制DebugLine，来显示出枪口指向碰撞点的向量
	FVector HitTarget;

	FHUDPackage HUDPackage;
	
	/*
	 * Aiming and FOV
	 */

	// Field of view when not aiming; set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/** 
	* Automatic Fire
	*/
	FTimerHandle FireTimer;

	// 控制开火间隔 每次开火后一定时间内无法再次开火
	bool bCanFire = true;
	
	void StartFireTimer();
	void FireTimerFinished();
	
public:	
	
};
