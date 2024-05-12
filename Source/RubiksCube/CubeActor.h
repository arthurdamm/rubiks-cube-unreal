// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <cmath>

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "CubeActor.generated.h"

UCLASS()
class RUBIKSCUBE_API ACubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Base unit cube template
    UPROPERTY(EditAnywhere)
    UStaticMesh* CubeMesh;

    // Cube edge length constant
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rubik's Cube")
    float CubeEdgeLength = 210.0f; // Adjust size as needed

	UPROPERTY(EditAnywhere)
	FVector StartLocation;

    UPROPERTY(EditAnywhere, Category = "Rubik's Cube")
    float MaxRotationAngle = 90;

	AStaticMeshActor* Cubes[3][3][3];

    std::vector<AStaticMeshActor*> CubesAtLayer[9];
    std::vector<AStaticMeshActor*> CubesVector;
    FVector NormalsAtLayer[9];
    FVector CentersAtLayer[9];
    TQueue<int> RotationsQueue;



private:
    bool bIsRotating = false;
    float RotationAngle = 0.0f;
    float RotationSpeed = 90.0f; // degrees per second
    FVector RotationAxis = FVector(0, 0, 1); // Default Z-axis
    int LayerToRotate = 1; // Example: middle layer
    FQuat TargetRotation;
    double StartTime = 0;


public:
    void StartRotation(int LayerIndex);
	void MaybeRotate(float DeltaTime);
    void PopulateCubesGrid();
    std::vector<AStaticMeshActor *> GetCubesInLayer(int layerIndex);
    int dtoi(double n);
};
