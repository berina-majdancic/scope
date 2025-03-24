// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet\GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}
AWeapon* ABaseCharacter::GetCurrentWeapon() const
{
    return CurrentWeapon;
}
void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (CurrentWeaponClass) {
        CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(CurrentWeaponClass);
        CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
        CurrentWeapon->SetOwner(this);
    }
}
void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void ABaseCharacter::Shoot()
{

    CurrentWeapon->Shoot();
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
    if (DamageCauser != this)
        Health -= DamageTaken;
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