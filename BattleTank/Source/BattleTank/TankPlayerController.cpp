// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Public/WorldCollision.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Begin Play"));

	auto ControlledTank = GetControlledTank();

	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possessing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is possessing: %s"), *(ControlledTank->GetName()));
	}
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }
	
	FVector OutHitLocation; //Out Parameter
	if (GetSightRayHitLocation(OutHitLocation)) // Has "side-effect", is going to line trace
	{
		UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *OutHitLocation.ToString());

			// TODO Tell controlled tank to aim at this point
	}
}


ATank* ATankPlayerController::GetControlledTank() const
{

	return Cast<ATank>(GetPawn());
}

//Get world location if linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

	// De-Project the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const 
{
	FVector CameraWorldLocation; // Discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);

}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult OutHitResult;
	FCollisionQueryParams TraceParams;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if(GetWorld()->LineTraceSingleByChannel(OutHitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams))
		{

			GetWorld()->LineTraceSingleByChannel(OutHitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);
			OutHitLocation = OutHitResult.Location;
			DrawDebugLine(GetWorld(), StartLocation, (LookDirection * LineTraceRange), FColor::Green, false, 1, 0, 5);
			return true;
		}

	OutHitLocation = FVector(0.0f, 0.0f, 0.0f);
	return false;
}

/*bool ATankPlayerController::GetSightRayHitLocation() const
{
	FVector Location;
	FRotator Rotation;
	GetPlayerViewPoint(Location, Rotation);
	FVector Start = GetPawn()->GetActorLocation();
	Start.Z += 180.f;
	Start.X += 200.f;
	FVector End = Location + (Rotation.Vector() * 50000.f);
	End.Z += 10000.f;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);

	FCollisionQueryParams TraceParams (FName(TEXT("TraceGround")), false, GetPawn());
	TraceParams.bTraceComplex = true;
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	FHitResult HitResult(ForceInit);

	//call GetWorld() from within an actor extending class
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_MAX, TraceParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("Trace Impact Point: %s"), *HitResult.ImpactPoint.ToString())
	}
	return false;

}*/

