// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyChase.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

// Sets default values
ABodyChase::ABodyChase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABodyChase::BeginPlay()
{
	Super::BeginPlay();

	if (!camera) return;
	TArray<UActorComponent*> aa;
	camera->GetComponents(aa);
	for (int32 b = 0; b < aa.Num(); b++)
	{
		UActorComponent* a = aa[b];
		UE_LOG(LogClass, Log, TEXT("Get Components: %s"), *(a->GetName()));
	}

	//camera->GetComponentsByClass()
	/*
	camera.fieldOfView = (float)(2 * Math.Atan(0.1 * field) * 180 / Math.PI); // for offset = 5D = 10R
	camera.nearClipPlane = 0.01f;
	camera.farClipPlane = 1000;
	*/
	
}

// Called every frame
void ABodyChase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!camera) return;

	FVector p1 = refer_body_motion->GetActorLocation();
	FVector p2 = chase_body_motion->GetActorLocation();
	FVector pos = p2 - p1;

	double offset = chase_body_motion->body->diameter * chase_body_motion->body->scale_diameter * 5;
	double orbitR = (double)(FVector::Dist(p1, p2));
	float ratio = (float)(1.0 + offset / orbitR);

	// farther
	FVector target = pos * ratio + p1;
	// higher
	target.Set(target.X, target.Y, target.Z + (float)(offset / 8));
	camera->SetActorLocation(target);

	FVector target_pos = chase_body_motion->GetActorLocation();
	FRotator target_rot = chase_body_motion->GetActorRotation();
	camera->GetActorEyesViewPoint(target_pos, target_rot);
	//camera_controller->SetViewTarget(chase_body_motion);
	//tF.LookAt(chaseTF);

	FVector cur = camera->GetActorLocation();
	UE_LOG(LogClass, Log, TEXT("Camera Location: %f %f %f"), cur.X, cur.Y, cur.Z);
}

