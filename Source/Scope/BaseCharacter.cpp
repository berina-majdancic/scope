// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
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
    if (CurrentWeaponClass) {
        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass);
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
        CurrentWeapon->SetOwner(this);
        CurrentWeapon->UpdateAmmo();
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
AWeapon* ABaseCharacter::GetCurrentWeapon() const
{
    return CurrentWeapon;
}
void ABaseCharacter::Shoot()
{
    CurrentWeapon->Shoot();
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

void ABaseCharacter::Reload()
{
    CurrentWeapon->Reload();
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
    if (Health == 0) {
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
    GetCharacterMovement()->DisableMovement();
    GetController()->UnPossess();
    SetActorEnableCollision(false);
}