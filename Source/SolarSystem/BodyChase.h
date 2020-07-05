// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodyMotion.h"
#include "Camera/CameraActor.h"
#include "BodyChase.generated.h"

UCLASS()
class SOLARSYSTEM_API ABodyChase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABodyChase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyMotion")
	ABodyMotion* refer_body_motion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyMotion")
	ABodyMotion* chase_body_motion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyMotion")
	ACameraActor* camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
