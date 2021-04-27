// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"
#include "PawnTurret.h"

APawnTurret::APawnTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange)
	{
		return;
	}

	RotateTurretFunction(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
	// If player == null || is Dead THEN BAIL!!
	if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
	{
		return;
	}
	// If player IS in range THEN FIRE!!
	if (ReturnDistanceToPlayer() <= FireRange)
	{
		Fire();
	}
}

float APawnTurret::ReturnDistanceToPlayer() const
{
	if (!PlayerPawn)
	{
		return 0.f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}