// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SOLARSYSTEM_API BodyCommon
{
private:
	void init_params(FString _body_name, double _mass, double _diameter,
		FString _centray_body_name, double _orbit_radius, double _orbit_velocity, double _orbit_angle);

public:
	// global variable, scale settings for display
	double scale_diameter = 1e-8;     // scale times for diameter, 1scale = 100
	double scale_distance = 2e-8;     // scale times for distance
	double scale_time = 864000;       // for accelate revolution, set 1 s = 10 day
	double dt = 600;                  // 600 s update the position

	// Body static property
	FString body_name;
	double mass;             // kg
	double diameter;         // m

	// Orbit information
	FString central_body_name;
	double orbit_radius;     // average orbit radius, m
	double orbit_velocity;   // average orbit linear velocity, m/s
	double orbit_angle;      // orbit inclination, degreee, orbit of earth = 0

	BodyCommon(FString _body_name);
	~BodyCommon();
};
