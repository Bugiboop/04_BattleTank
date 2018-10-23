// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"//Must be the last include

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATank * GetControlledTank() const;

	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginPlay() override;

	//Start the tank moving the barrel so that a shot would hit where the crosshair intersects the world
	virtual void AimTowardsCrosshair();
	
	//Return an OUT parameter, true if hit landscape
	
	//virtual bool GetSightRayHitLocation() const;
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
};