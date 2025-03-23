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
    void Shoot(FHitResult* HitResultPtr);
    void Reload();
    void BeginPlay() override;

protected:
    // Called when the game starts or when spawned
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
    float FireRate = 0.2f;
    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float LegDamage = 30;
    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float ChestDamage = 40;
    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float HeadDamage = 100;
    UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float Ammo;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float MaxAmmo = 30;

private:
    bool bCanShoot = true;
    bool bIsReloading = false;
    FTimerHandle FireRateTimerHandle;
    FHitResult* HitResult;
    UPROPERTY()
    ABaseCharacter* CharacterOwner;
    TMap<FString, float> DamageMap;

    void DamageEnemy();
    void InitializeDamageMap();
};
