// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet\GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
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
        if (HitResult.GetActor() != this) {
            if (MuzzleFlash)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, HitResult.ImpactPoint, Rotation);
            if (ABaseCharacter* Actor = Cast<ABaseCharacter>(HitResult.GetActor())) {
                FPointDamageEvent DamageEvent(BaseDamage, HitResult, -Rotation.Vector(), nullptr);
                if (HitResult.BoneName.ToString() == "head")
                    Actor->TakeDamage(BaseDamage * 4, DamageEvent, GetController(), this);
                else
                    Actor->TakeDamage(BaseDamage, DamageEvent, GetController(), this);
            }
        }
    }
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

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    float DamageTaken = FMath::Min(Health, Damage);
    UE_LOG(LogTemp, Display, TEXT("Damage Taken! %f"), DamageTaken);
    Health -= DamageTaken;
    if (Health == 0) {
        Die();
        UE_LOG(LogTemp, Display, TEXT("Dead!"));
    }
    return DamageTaken;
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
    Destroy();
}
