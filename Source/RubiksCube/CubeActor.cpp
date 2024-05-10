// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "HAL/PlatformTime.h"

// Sets default values
ACubeActor::ACubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/SM_RubiksCube.SM_RubiksCube'"));
    if (MeshObj.Succeeded())
    {
		UE_LOG(LogTemp, Warning, TEXT("S!M Succeeded"));
        CubeMesh = MeshObj.Object;
    } else {
		UE_LOG(LogTemp, Warning, TEXT("S!M FAILED"));
	}
}

// Called when the game starts or when spawned
void ACubeActor::BeginPlay()
{
	Super::BeginPlay();

    StartTime = FPlatformTime::Seconds();

	SetActorLocation(StartLocation);
	FVector BasePosition = StartLocation;
	UE_LOG(LogTemp, Warning, TEXT("Starting CubeActor location: %s"), *BasePosition.ToString());

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                FVector Position = BasePosition + FVector(i * CubeEdgeLength, j * CubeEdgeLength, k * CubeEdgeLength);
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                
                AStaticMeshActor* NewCube = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Position, FRotator::ZeroRotator, SpawnParams);
                
                if (NewCube && NewCube->GetStaticMeshComponent())
                {
					UE_LOG(LogTemp, Error, TEXT("Spawned Cube at %s"), *Position.ToString());
                    NewCube->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);  // Referencing CubeMesh here
                    NewCube->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);  // Set mobility to Movable
                    Cubes[i][j][k] = NewCube;
                    CubesAtLayer[i].push_back(NewCube);
                    CubesAtLayer[3 + j].push_back(NewCube);
                    CubesAtLayer[6 + k].push_back(NewCube);
                    
                    if (j == 1 && k == 1) {
                        NormalsAtLayer[0 + i] = NewCube->GetActorForwardVector();
                    }
                    if (i == 1 && k == 1) {
                        NormalsAtLayer[3 + j] = NewCube->GetActorForwardVector();
                    }
                    if (i == 1 && j == 1) {
                        NormalsAtLayer[6 + k] = NewCube->GetActorForwardVector();
                    }
                }
            }
        }
    }

    StartRotation(2, FVector(1, 0, 0), 4.0f); // Rotate middle layer around Z-axis over 1 second
    UE_LOG(LogTemp, Warning, TEXT("ROTATING!!!!..."));
    UE_LOG(LogTemp, Warning, TEXT("MaxRotationAngle: %f\n"), MaxRotationAngle);
}


// In CubeActor.cpp

void ACubeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MaybeRotate(DeltaTime);
}


void ACubeActor::MaybeRotate(float DeltaTime) {
    if (!StartTime || FPlatformTime::Seconds() - StartTime < 1) {
        return;
    }

    if (bIsRotating)
    {
        float DeltaRotation = RotationSpeed * DeltaTime;
        RotationAngle += DeltaRotation;
        UE_LOG(LogTemp, Warning, TEXT("Angle: %f \t Delta: %f"), RotationAngle, DeltaRotation);

        if (RotationAngle >= MaxRotationAngle)
        {
            DeltaRotation -= (RotationAngle - MaxRotationAngle);
            RotationAngle = MaxRotationAngle;
            bIsRotating = false; // Stop rotation after 90 degrees
            UE_LOG(LogTemp, Warning, TEXT("ROTATION stopped"));
        }

        FQuat QuatRotation = FQuat(RotationAxis, FMath::DegreesToRadians(DeltaRotation));
        FVector RotationCenter = FVector(1420.000000,1210.000000,1210.000000);

        FVector EndPoint;

        for (AStaticMeshActor* CubeToRotate : CubesAtLayer[LayerToRotate]) {
            if (CubeToRotate)
            {
                FVector RelativePosition = CubeToRotate->GetActorLocation() - RotationCenter;
                FVector RotatedPosition = QuatRotation.RotateVector(RelativePosition);
                CubeToRotate->SetActorLocation(RotationCenter + RotatedPosition);
                CubeToRotate->AddActorLocalRotation(QuatRotation);

                EndPoint = CubeToRotate->GetActorLocation() +  CubeToRotate->GetActorForwardVector() * 1000.0f; // Extend the line along the rotation axis
                DrawDebugLine(
                    GetWorld(),
                    CubeToRotate->GetActorLocation(),
                    EndPoint,
                    FColor::Red,
                    false, // Persistent lines
                    0.1,   // Lifetime
                    0,     // Depth priority
                    10.0   // Thickness
                );
            }
        }

        EndPoint = RotationCenter + RotationAxis * 1000.0f; // Extend the line along the rotation axis
        DrawDebugLine(
            GetWorld(),
            RotationCenter,
            EndPoint,
            FColor::Red,
            false, // Persistent lines
            0.1,   // Lifetime
            0,     // Depth priority
            10.0   // Thickness
        );

    }
}

// In CubeActor.cpp

void ACubeActor::StartRotation(int LayerIndex, FVector Axis, float Duration)
{
    if (!bIsRotating)
    {
        bIsRotating = true;
        LayerToRotate = LayerIndex;
        RotationAxis = Axis;
        UE_LOG(LogTemp, Warning, TEXT("RotationAxis: %s"), *RotationAxis.ToString());
        RotationAxis.Normalize(1.0);
        UE_LOG(LogTemp, Warning, TEXT("RotationAxis: %s"), *RotationAxis.ToString());
        RotationAxis = RotationAxis.GetSafeNormal(1.0);
        UE_LOG(LogTemp, Warning, TEXT("RotationAxis: %s"), *RotationAxis.ToString());
        RotationSpeed = 90.0f / Duration; // Calculate speed based on duration
        RotationAngle = 0.0f;
        TargetRotation = FQuat(Axis, FMath::DegreesToRadians(90.0f));
    }
}


