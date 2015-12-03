#pragma once

#include <functional>
#include "GameObject.h"
#include "Camera.h"

// seriously I don't know where else this would go

typedef std::function<bool(GameObject const&, GameObject const&)> isFurtherAwayFunc;

float DistanceFromCamera(Camera const& cam, GameObject const& go);