// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodyMotion.h"
#include "UnifyTrajectoryDraw.generated.h"

UCLASS()
class SOLARSYSTEM_API AUnifyTrajectoryDraw : public AActor
{
	GENERATED_BODY()

private:
	ULineBatchComponent* line_batch_component;
	void DrawTrajectory();

public:	
	// Sets default values for this actor's properties
	AUnifyTrajectoryDraw();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UnifyTrajectoryDraw")
	TArray<ABodyMotion*> body_motions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
