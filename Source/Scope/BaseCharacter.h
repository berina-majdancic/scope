// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BaseCharacter.generated.h"
UCLASS()
class SCOPE_API ABaseCharacter : public ACharacter {
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ABaseCharacter();
    void Shoot();
    UFUNCTION(BlueprintCallable)
    FRotator GetAimRotation() const;
    UFUNCTION(BlueprintCallable)
    bool GetIsDead() const;
    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UParticleSystem> MuzzleFlash;
    void Die();

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float MaxBulletDistance = 5000;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float FireRate = 0.1f;
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float Health = 100;
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float MaxHealth = 100;
    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float BaseDamage = 30;
    FTimerHandle FireRateTimerHandle;
    bool bCanShoot = true;
    bool bIsDead = false;
};
