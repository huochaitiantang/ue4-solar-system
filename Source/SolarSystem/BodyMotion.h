// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodyCommon.h"
#include "Components/LineBatchComponent.h"
#include<chrono>
#include "BodyMotion.generated.h"
using namespace std;

UCLASS()
class SOLARSYSTEM_API ABodyMotion : public AActor
{
	GENERATED_BODY()
	
private:
	
	TArray<double> pos = { 0, 0, 0 }; // position of body
	TArray<double> vel = { 0, 0, 0 }; // velocity of body

	chrono::system_clock::time_point t0, t1;
	double minY;    // for orbit adjust

	bool enable_trajectory = true;       // show trajectory?
	int max_trajectory = 500;            // max trajectory count
	int last_trajectory_index;            // last trajectory index
	TArray<TArray<double>> trajectories;  // trajectory array

	void SetPosition();
	TArray<double> Acceleration(TArray<double> p1, TArray<double> p2, double central_mass);
	void MoveOnce(double dt);
	void AddTrajectory();
	void InitOrbit();
	void AdjustOrbit();


public:	
	// Sets default values for this actor's properties
	ABodyMotion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyMotion")
	FString body_name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyMotion")
	ABodyMotion* central_body_motion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyMotion")
	UStaticMeshComponent* VisualMesh;

	BodyCommon* body;
	TArray<FVector> GetTrajectory();
	float GetTrajectoryLineWidth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
