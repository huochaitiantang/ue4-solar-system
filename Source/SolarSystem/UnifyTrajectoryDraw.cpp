// Fill out your copyright notice in the Description page of Project Settings.
#include "UnifyTrajectoryDraw.h"

// Sets default values
AUnifyTrajectoryDraw::AUnifyTrajectoryDraw()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// draw current trajectory array
void AUnifyTrajectoryDraw::DrawTrajectory() {
    line_batch_component->BatchedLines.Empty();
    TArray<FBatchedLine> lines;
    float line_width;
    if (body_motions.Num() <= 0) return;
    for (int i = 0; i < body_motions.Num(); i++) {
        TArray<FVector> points = body_motions[i]->GetTrajectory();
        line_width = body_motions[i]->GetTrajectoryLineWidth();
        line_width = line_width < 10 ? 10 : line_width;
        if (points.Num() <= 1) continue;
        for (int j = 1; j < points.Num(); j++) {
            FBatchedLine line = FBatchedLine(points[j - 1], points[j], FLinearColor(1, 0, 0, 1), 1, line_width, 4);
            lines.Add(line);
        }
    }
    line_batch_component->DrawLines(lines);
    int count_all = line_batch_component->BatchedLines.Num();
    //UE_LOG(LogClass, Log, TEXT("Draw %d points, line width: %f, all points: %d"), lines.Num(), line_width, count_all);
}

// Called when the game starts or when spawned
void AUnifyTrajectoryDraw::BeginPlay()
{
	Super::BeginPlay();
    line_batch_component = GetWorld()->PersistentLineBatcher;
}

// Called every frame
void AUnifyTrajectoryDraw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    DrawTrajectory();
}

