// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapon.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
    InitializeDamageMap();
    CharacterOwner = Cast<ABaseCharacter>(GetOwner());
}

void AWeapon::Shoot()
{
    if (!bCanShoot || bIsReloading || !AmmmoCapacity)
        return;
    HitResult = Trace();
    if (Ammo <= 0) {
        Reload();
        return;
    }
    Ammo--;
    UpdateAmmo();
    if (!CharacterOwner)
        return;
    AddRecoil();
    bCanShoot = false;
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, [this]() { bCanShoot = true; }, FireRate, false);
    if (CharacterOwner)
        CharacterOwner->PlayShootingAnimation();
    if (MuzzleFlash)
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Muzzle);
    if (HitEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint, HitResult.TraceStart.Rotation());
    DamageEnemy();
}

void AWeapon::FinishReload()
{
    Ammo = MaxAmmo;
    bIsReloading = false;
    UpdateAmmo();
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RecoilOffset = UKismetMathLibrary::RInterpTo(RecoilOffset, FRotator::ZeroRotator, DeltaTime, RecoverySpeed);
    if (RecoilOffset == FRotator::ZeroRotator)
        TargetRecoilOffset = FRotator::ZeroRotator;
    if (CharacterOwner) {
        CharacterOwner->AddControllerPitchInput(-RecoilOffset.Pitch);
        CharacterOwner->AddControllerYawInput(RecoilOffset.Yaw);
    }
}

void AWeapon::UpdateAmmo()
{
    if (!BasePlayerController)
        GetBasePlayerController();
    if (BasePlayerController) {
        UE_LOG(LogTemp, Display, TEXT("UpdatedAmmo"));
        BasePlayerController->UpdateAmmo(Ammo, MaxAmmo, AmmmoCapacity);
    } else
        UE_LOG(LogTemp, Warning, TEXT("No Controller"));
}

void AWeapon::GetBasePlayerController()
{
    if (!BasePlayerController) {
        if (!CharacterOwner)
            CharacterOwner = Cast<ABaseCharacter>(GetOwner());
        if (CharacterOwner)
            BasePlayerController = Cast<ABasePlayerController>(CharacterOwner->GetController());
    }
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

void AWeapon::Reload()
{
    if (Ammo == MaxAmmo)
        return;
    bIsReloading = true;
    if (CharacterOwner)
        CharacterOwner->PlayReloadAnimation(ReloadTime);
    AmmmoCapacity += -MaxAmmo + Ammo;
    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeapon::FinishReload, ReloadTime, false);
}

void AWeapon::AddRecoil()
{
    if (!CharacterOwner)
        return;

    float PitchRecoil = FMath::RandRange(RecoilPitchMin * 0.1, RecoilPitchMax * 0.1);
    float YawRecoil = FMath::RandRange(RecoilYawMin * 0.1, RecoilYawMax * 0.1);

    TargetRecoilOffset.Pitch = FMath::Clamp(TargetRecoilOffset.Pitch + PitchRecoil, -MaxRecoil.Pitch * 0.1, MaxRecoil.Pitch * 0.1);
    TargetRecoilOffset.Yaw = FMath::Clamp(TargetRecoilOffset.Yaw + YawRecoil, -MaxRecoil.Yaw * 0.1, MaxRecoil.Yaw * 0.1);

    RecoilOffset = UKismetMathLibrary::RInterpTo(RecoilOffset, TargetRecoilOffset, GetWorld()->GetDeltaSeconds(), RecoilSpeed);
}

FHitResult AWeapon::Trace()
{
    AController* OwnerController = CharacterOwner->GetController();
    FVector ViewPointLoc;
    FRotator ViewPointRot;
    OwnerController->GetPlayerViewPoint(ViewPointLoc, ViewPointRot);
    FVector StartLocation = Muzzle->GetComponentLocation();
    FVector EndLocation = ViewPointLoc + ViewPointRot.Vector() * MaxBulletDistance;
    FHitResult HitResultCur;
    GetWorld()->LineTraceSingleByChannel(HitResultCur, StartLocation, EndLocation, ECC_GameTraceChannel1);
    return HitResultCur;
}

void AWeapon::DamageEnemy()
{
    float Damage = DamageMap.FindRef(HitResult.BoneName.ToString());
    if (ABaseCharacter* Actor = Cast<ABaseCharacter>(HitResult.GetActor())) {
        if (!Damage)
            Damage = LegDamage;
        FPointDamageEvent DamageEvent(Damage, HitResult, -HitResult.TraceStart.Rotation().Vector(), nullptr);
        if (CharacterOwner && CharacterOwner->GetController())
            if (Actor != Owner)
                Actor->TakeDamage(Damage, DamageEvent, CharacterOwner->GetController(), Owner);
    }
}
