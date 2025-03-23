// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>("Root Component");
    RootComponent = Root;
    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gun Mesh");
    GunMesh->SetupAttachment(Root);
    Muzzle = CreateDefaultSubobject<USceneComponent>("Muzzle");
    Muzzle->SetupAttachment(GunMesh);
    Ammo = MaxAmmo;
    CharacterOwner = Cast<ABaseCharacter>(GetOwner());
    InitializeDamageMap();
}

void AWeapon::Shoot(FHitResult* HitResultPtr)
{
    HitResult = HitResultPtr;
    if (!CharacterOwner)
        CharacterOwner = Cast<ABaseCharacter>(GetOwner());
    if (!bCanShoot || bIsReloading)
        return;
    if (Ammo <= 0) {
        bIsReloading = true;
        if (CharacterOwner)
            CharacterOwner->PlayReloadAnimation();
        return;
    }
    Ammo--;
    bCanShoot = false;
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, [this]() { bCanShoot = true; }, FireRate, false);
    if (CharacterOwner)
        CharacterOwner->PlayShootingAnimation();
    if (MuzzleFlash)
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Muzzle);
    if (HitEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult->ImpactPoint, HitResult->TraceStart.Rotation());
    DamageEnemy();
}

void AWeapon::Reload()
{
    Ammo = MaxAmmo;
    bIsReloading = false;
}

void AWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void AWeapon::InitializeDamageMap()
{
    DamageMap.Add("head", HeadDamage);
    DamageMap.Add("spine_01", ChestDamage);
    DamageMap.Add("pelvis", ChestDamage);
    DamageMap.Add("upperarm_l", ChestDamage);
    DamageMap.Add("upperarm_r", ChestDamage);
    DamageMap.Add("thigh_l", LegDamage);
    DamageMap.Add("thigh_r", LegDamage);
    DamageMap.Add("calf_l", LegDamage);
    DamageMap.Add("calf_r", LegDamage);
    DamageMap.Add("foot_l", LegDamage);
    DamageMap.Add("foot_r", LegDamage);
    DamageMap.Add("hand_l", ChestDamage);
    DamageMap.Add("hand_r", ChestDamage);
}

void AWeapon::DamageEnemy()
{

    float Damage = DamageMap.FindRef(HitResult->BoneName.ToString());
    if (ABaseCharacter* Actor = Cast<ABaseCharacter>(HitResult->GetActor())) {
        if (!Damage)
            Damage = LegDamage;

        FPointDamageEvent DamageEvent(Damage, *HitResult, -HitResult->TraceStart.Rotation().Vector(), nullptr);
        if (CharacterOwner && CharacterOwner->GetController())
            Actor->TakeDamage(Damage, DamageEvent, CharacterOwner->GetController(), this);
    }
}
