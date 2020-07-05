// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyCommon.h"

BodyCommon::BodyCommon(FString _body_name)
{
	if (_body_name == "sun") {
		init_params(_body_name, 1.9891e30, 1.392e9, "", 0, 0, 0);
	}
	else if (_body_name == "mercury") {
		init_params(_body_name, 3.3022e23, 4.878e6, "sun", 5.79091e10, 4.789e4, 7.00487);
	}
	else if (_body_name == "venus") {
		init_params(_body_name, 4.869e24, 1.2103e7, "sun", 1.0820893e11, 3.503e4, 3.395);
	}
	else if (_body_name == "earth") {
		init_params(_body_name, 5.965e24, 1.2756e7, "sun", 1.49597887e11, 2.9783e4, 0);
	}
	else if (_body_name == "mars") {
		init_params(_body_name, 6.4219e23, 6.794e6, "sun", 2.2793664e11, 2.4131e4, 1.85061);

	}
	else if (_body_name == "jupiter") {
		init_params(_body_name, 1.90e27, 1.42984e8, "sun", 7.785472e11, 1.307e4, 1.3051);

	}
	else if (_body_name == "saturn") {
		init_params(_body_name, 5.6846e26, 1.20540e8, "sun", 1.43344937e12, 9.69e3, 2.48524);

	}
	else if (_body_name == "uranus") {
		init_params(_body_name, 8.6810e25, 5.1118e7, "sun", 2.876679082e12, 6.8353e3, 0.772556);

	}
	else if (_body_name == "neptune") {
		init_params(_body_name, 1.0247e26, 4.9532e7, "sun", 4.503443661e12, 5.4778e3, 1.767975);
	}
	else {
		UE_LOG(LogClass, Error, TEXT("Not Found The Body: %s\n"), *_body_name);
	}
	UE_LOG(LogClass, Log, TEXT("Find The Body: %s, %lf, %lf, %s, %lf, %lf, %lf\n"),
		*body_name, mass, diameter, *central_body_name, orbit_radius, orbit_velocity, orbit_angle);
}

void BodyCommon::init_params(FString _body_name, double _mass, double _diameter,
	FString _centray_body_name, double _orbit_radius, double _orbit_velocity, double _orbit_angle) {
	body_name = _body_name;
	mass = _mass;
	diameter = _diameter;
	if (_centray_body_name != "") {
		central_body_name = _centray_body_name;
		orbit_radius = _orbit_radius;
		orbit_velocity = _orbit_velocity;
		orbit_angle = _orbit_angle;
	}
}

BodyCommon::~BodyCommon()
{
}
