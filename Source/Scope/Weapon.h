// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"
class UStaticMeshComponent;
class ABaseCharacter;
UCLASS()
class SCOPE_API AWeapon : public AActor {
    GENERATED_BODY()

public:
    AWeapon();
    void Shoot();
    void Reload();
    void BeginPlay() override;
    void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USceneComponent> Root;
    UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> GunMesh;
    UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USceneComponent> Muzzle;
    UPROPERTY(EditDefaultsOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UParticleSystem> MuzzleFlash;
    UPROPERTY(EditDefaultsOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UParticleSystem> HitEffect;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float MaxBulletDistance = 100000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float FireRate = 0.2f;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float AmmmoCapacity = 90;
    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float LegDamage = 30;
    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float ChestDamage = 40;
    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float HeadDamage = 100;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float ReloadTime = 3.0f;
    UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float Ammo;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float MaxAmmo = 30.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    float RecoilPitchMin = 0.1f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    float RecoilPitchMax = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    float RecoilYawMin = -1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    float RecoilYawMax = 1.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    float RecoverySpeed = 5.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    float RecoilSpeed = 2.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
    FRotator MaxRecoil = FRotator(15.0f, 10.0f, 0.0f);

private:
    FRotator RecoilOffset = FRotator::ZeroRotator;
    FRotator TargetRecoilOffset = FRotator::ZeroRotator;
    bool bCanShoot = true;
    bool bIsReloading = false;
    FTimerHandle FireRateTimerHandle;
    FTimerHandle ReloadTimerHandle;
    FHitResult HitResult;
    UPROPERTY()
    ABaseCharacter* CharacterOwner;
    TMap<FString, float> DamageMap;

    void DamageEnemy();
    void InitializeDamageMap();
    void FinishReload();
    void AddRecoil();
    FHitResult Trace();
};
