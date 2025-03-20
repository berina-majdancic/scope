// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Kismet\GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::Shoot()
{
    if (!bCanShoot)
        return;
    bCanShoot = false;
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, [this]() { bCanShoot = true; }, FireRate, false);

    if (MuzzleFlash)
        UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GetMesh(), TEXT("SMG_Barrel"));

    AController* OwnerController = GetController();
    FVector StartLocation;
    FRotator Rotation;
    OwnerController->GetPlayerViewPoint(StartLocation, Rotation);
    FHitResult HitResult;
    FVector EndLocation = StartLocation + Rotation.Vector() * MaxBulletDistance;
    GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel1);
    if (HitResult.bBlockingHit) {
        UE_LOG(LogTemp, Display, TEXT("Hit"));
        if (MuzzleFlash)
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, HitResult.ImpactPoint, Rotation);
    }
}

FRotator ABaseCharacter::GetAimRotation()
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

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
