// Fill out your copyright notice in the Description page of Project Settings.

#include "BodyMotion.h"

// Sets default values
ABodyMotion::ABodyMotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    VisualMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
    if (SphereVisualAsset.Succeeded())
    {
        VisualMesh->SetStaticMesh(SphereVisualAsset.Object);
    }
}

// set the position of body
void ABodyMotion::SetPosition() {
    double scale = body->scale_distance;
    double x = pos[0] * scale;
    double y = pos[1] * scale;
    double z = pos[2] * scale;
    if (central_body_motion) {
        x += central_body_motion->pos[0] * scale;
        y += central_body_motion->pos[1] * scale;
        z += central_body_motion->pos[2] * scale;
    }

    double offset = body->diameter * body->scale_diameter * 100 / 2;
    SetActorLocation(FVector((float)x, (float)y, (float)(z - offset)));
}

// calculate the acceleration from central_mass
TArray<double> ABodyMotion::Acceleration(TArray<double> p1, TArray<double> p2, double central_mass) {
    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];
    double dz = p2[2] - p1[2];
    double dis_3 = pow(dx * dx + dy * dy + dz * dz, 1.5); 

    TArray<double> a = { 0, 0, 0 };
    double Gm2 = 6.67e-11 * central_mass;
    a[0] = Gm2 * dx / dis_3;
    a[1] = Gm2 * dy / dis_3;
    a[2] = Gm2 * dz / dis_3;
    return a;
}

// once motion during dt, dt may less than BodyCommon.dt
void ABodyMotion::MoveOnce(double dt) {
    TArray<double> gravity_pos = { 0, 0, 0 };
    TArray<double> a = Acceleration(pos, gravity_pos, central_body_motion->body->mass);

    // updata velocity
    vel[0] = vel[0] + a[0] * dt;
    vel[1] = vel[1] + a[1] * dt;
    vel[2] = vel[2] + a[2] * dt;

    // update position, only consider central body, add the position of central body
    double item = 0.5f * dt * dt;
    pos[0] = pos[0] + vel[0] * dt + a[0] * item;
    pos[1] = pos[1] + vel[1] * dt + a[1] * item;
    pos[2] = pos[2] + vel[2] * dt + a[2] * item;
}

// add current position to trajectory array
void ABodyMotion::AddTrajectory() {
    if(trajectories.Num() < max_trajectory){
        TArray<double> cur_pos = { pos[0], pos[1], pos[2] };
        trajectories.Add(cur_pos);
        last_trajectory_index = trajectories.Num() - 1;
        //UE_LOG(LogClass, Log, TEXT("[%s] add [%d] x=%lf y=%lf z=%lf"), *body_name, last_trajectory_index,
        //    trajectories[last_trajectory_index][0], trajectories[last_trajectory_index][1], trajectories[last_trajectory_index][2]);
    }
    // if the array is full, save by a loop method
    else {
        last_trajectory_index = (last_trajectory_index + 1) % max_trajectory;
        trajectories[last_trajectory_index][0] = pos[0];
        trajectories[last_trajectory_index][1] = pos[1];
        trajectories[last_trajectory_index][2] = pos[2];
        //UE_LOG(LogClass, Log, TEXT("[%s] update [%d] x=%lf y=%lf z=%lf"), *body_name, last_trajectory_index,
        //    trajectories[last_trajectory_index][0], trajectories[last_trajectory_index][1], trajectories[last_trajectory_index][2])
    }
    //UE_LOG(LogClass, Log, TEXT("Trajectories Count: %d"), trajectories.Num());
}

TArray<FVector> ABodyMotion::GetTrajectory() {
    float x, y, z;
    int i, j = 0;
    double scale = body->scale_distance;
    TArray<FVector> points;
    if (trajectories.Num() <= 0) return points;
    for (i = last_trajectory_index; i >= 0; i--, j++) {
        x = (float)((central_body_motion->pos[0] + trajectories[i][0]) * scale);
        y = (float)((central_body_motion->pos[1] + trajectories[i][1]) * scale);
        z = (float)((central_body_motion->pos[2] + trajectories[i][2]) * scale);
        points.Add(FVector(x, y, z));
    }

    // process loop part
    if (j < trajectories.Num()) {
        for (i = trajectories.Num() - 1; i > last_trajectory_index; i--, j++) {
            x = (float)((central_body_motion->pos[0] + trajectories[i][0]) * scale);
            y = (float)((central_body_motion->pos[1] + trajectories[i][1]) * scale);
            z = (float)((central_body_motion->pos[2] + trajectories[i][2]) * scale);
            points.Add(FVector(x, y, z));
        }
    }
    return points;
}

float ABodyMotion::GetTrajectoryLineWidth() {
    return (float)(body->diameter * body->scale_diameter * 50);
}

// init the position and velocity
void ABodyMotion::InitOrbit() {
    // initial position is relative to the central body
    pos[0] = body->orbit_radius;
    pos[1] = 0;
    pos[2] = 0;
    SetPosition();
    // set the initial velocity of revolution
    // initial velocity if relative to the central body
    vel[0] = 0;
    // anticlockwise from the top
    vel[1] = -cos(PI * body->orbit_angle / 180) * body->orbit_velocity;
    vel[2] = sin(PI * body->orbit_angle / 180) * body->orbit_velocity;
}

// void adjust the orbit position and veloctiy
// because dt > 0 will pull the moon to the central body
void ABodyMotion::AdjustOrbit() {
    if ((pos[0] > 0) && (pos[1] >= 0)) {
        minY = minY < pos[1] ? minY : pos[1];
    }
    else {
        // neareast postion to initial position
        if (minY < body->orbit_radius / 2) {
            double curR = sqrt(pow(pos[0], 2) + pow(pos[1], 2) + pow(pos[2], 2));
            double delta = abs((curR - body->orbit_radius) / body->orbit_radius);
            // if orbit R delta > 5%, adjust orbit
            if (delta > 0.05) {
                UE_LOG(LogClass, Log, TEXT("Keep Orbit [%s] before: %lf %lf %lf, delta: %lf"), *(body->body_name), pos[0], pos[1], pos[2], delta);
                InitOrbit();
                UE_LOG(LogClass, Log, TEXT("Keep Orbit [%s] after: %lf %lf %lf"), *(body->body_name), pos[0], pos[1], pos[2]);
            }
            minY = body->orbit_radius;
        }
    }
}

// Called when the game starts or when spawned
void ABodyMotion::BeginPlay()
{
	Super::BeginPlay();

    body = new BodyCommon(body_name);
    if (body->body_name == "") return;
    if (body->central_body_name != "") {
        // find the central body motion
        //central_body_motion = ;
    }

    // set display size of body
    float d = (float)(body->diameter * body->scale_diameter);
    VisualMesh->SetRelativeScale3D(FVector(d, d, d));
    InitOrbit();
    t0 = std::chrono::system_clock::now();
    minY = body->orbit_radius;
    //Debug.Log(this.name + " size=" + tF.localScale + " pos=" + tF.position);
}

// Called every frame
void ABodyMotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (body->body_name == "" || central_body_motion == NULL) return;

    // true duration time
    t1 = std::chrono::system_clock::now();
    double deltaTime = std::chrono::duration<double, std::milli>(t1 - t0).count() * body->scale_time / 1000;
    t0 = t1;
    int moveCount = (int)(floor(deltaTime / body->dt));
    double tailDt = deltaTime - moveCount * body->dt;

    // move the position based on the gravity of central body
    //UE_LOG(LogClass, Log, TEXT("deltaTime: %lf, move count: %d"), deltaTime, moveCount);
    for (int i = 0; i < moveCount; i++) MoveOnce(body->dt);
    if (tailDt > 0) MoveOnce(tailDt);
    SetPosition();
    AdjustOrbit();
    
    // add new trajectory if satisfy condition and draw current trajectory array
    if (enable_trajectory) {
        if (trajectories.Num() <= 0) {
            AddTrajectory();
            last_trajectory_index = 0;
        }
        double dist = sqrt(pow(pos[0] - trajectories[last_trajectory_index][0], 2.0) +
            pow(pos[1] - trajectories[last_trajectory_index][1], 2.0) +
            pow(pos[2] - trajectories[last_trajectory_index][2], 2.0));
        if (dist >= body->diameter) AddTrajectory();
    }
}

