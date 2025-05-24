// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "AIController.h"
#include "BasePlayerController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet\GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}
void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (PrimaryWeaponClass) {
        PrimaryWeapon = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
        PrimaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
        PrimaryWeapon->SetOwner(this);
        if (bIsPrimaryEquipped)
            PrimaryWeapon->UpdateAmmoHUD();
        PrimaryWeapon->SetActorHiddenInGame(true);
        bIsPrimaryEquipped = false;
    }
    if (SecondaryWeaponClass) {
        SecondaryWeapon = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass);
        SecondaryWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
        SecondaryWeapon->SetOwner(this);
        if (!bIsPrimaryEquipped)
            SecondaryWeapon->UpdateAmmoHUD();
    }
    if (GetController())
        BasePlayerController = Cast<ABasePlayerController>(GetController());
    UpdateHealthHUD();
}
void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
AWeapon* ABaseCharacter::GetPrimaryWeapon() const
{
    return PrimaryWeapon;
}
void ABaseCharacter::Shoot()
{
    if (bIsPrimaryEquipped) {
        if (PrimaryWeapon)
            PrimaryWeapon->Shoot();
    } else {
        if (SecondaryWeapon)
            SecondaryWeapon->Shoot();
    }
}
void ABaseCharacter::UpdateHealthHUD()
{
    if (BasePlayerController)
        BasePlayerController->UpdateHealth(Health, MaxHealth);
}
FRotator ABaseCharacter::GetAimRotation() const
{
    FVector StartLocation;
    FRotator Rotation;
    if (GetController()) {
        GetController()->GetPlayerViewPoint(StartLocation, Rotation);
        FHitResult HitResult;
        FVector EndLocation = StartLocation + Rotation.Vector() * MaxBulletDistance;
        GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel1);
        FVector AimLocation = HitResult.ImpactPoint - GetActorLocation();
        return (AimLocation.Rotation() - GetActorRotation());
    }
    return FRotator();
}

bool ABaseCharacter::GetIsDead() const
{
    return bIsDead;
}

bool ABaseCharacter::GetIsPrimaryEquipped() const
{
    return bIsPrimaryEquipped;
}

void ABaseCharacter::Reload()
{
    if (bIsPrimaryEquipped) {
        if (PrimaryWeapon)
            PrimaryWeapon->Reload();
    } else {
        if (SecondaryWeapon)
            SecondaryWeapon->Reload();
    }
}
void ABaseCharacter::SwitchWeaponLogic()
{
    if (PrimaryWeapon)
        PrimaryWeapon->SetActorHiddenInGame(bIsPrimaryEquipped);
    if (SecondaryWeapon)
        SecondaryWeapon->SetActorHiddenInGame(!bIsPrimaryEquipped);
    bIsPrimaryEquipped = !bIsPrimaryEquipped;
    AWeapon* CurrentWeapon = bIsPrimaryEquipped ? PrimaryWeapon : SecondaryWeapon;
    if (CurrentWeapon)
        CurrentWeapon->UpdateAmmoHUD();
}
void ABaseCharacter::setIsShooting(bool isShooting)
{
    bIsShooting = isShooting;
}
void ABaseCharacter::setIsReloading(bool isReloading)
{
    bIsReloading = isReloading;
}
bool ABaseCharacter::getIsShooting()
{
    return bIsShooting;
}
bool ABaseCharacter::getIsReloading()
{
    return bIsReloading;
}

void ABaseCharacter::SwitchWeapon()
{
    PlaySwitchWeaponAnimation();
    GetWorld()->GetTimerManager().SetTimer(SwitchWeaponTimerHandle, this, &ABaseCharacter::SwitchWeaponLogic, SwitchTime, false);
}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    float DamageTaken = FMath::Min(Health, Damage);
    UE_LOG(LogTemp, Display, TEXT("Damage Taken! %f"), DamageTaken);
    if (DamageCauser != this) {
        Health -= DamageTaken;
        UpdateHealthHUD();
    }
    if (Health <= 0) {
        Die();
        UE_LOG(LogTemp, Display, TEXT("Dead!"));
    }
    return DamageTaken;
}

void ABaseCharacter::Die()
{
    if (bIsDead)
        return;
    bIsDead = true;
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
        DisableInput(PC);
    // GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    if (AAIController* AICon = Cast<AAIController>(GetController())) {
        AICon->StopMovement();
        AICon->UnPossess();
    }

    // GetController()->UnPossess();
}