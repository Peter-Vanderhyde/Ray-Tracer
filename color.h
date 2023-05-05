#pragma once

#include "vector3d.h"

using Color = Vector3D;

const Color Black = {0, 0, 0};
const Color White = {1, 1, 1};

const Color Gray = {0.5, 0.5, 0.5};
const Color Lightgray = {0.8, 0.8, 0.8};

const Color Red = {1, 0, 0};
const Color Orange = {1.0, 0.5, 0.0};
const Color Yellow = {1, 1, 0};
const Color Green = {0, 1, 0};
const Color Blue = {0, 0, 1};
const Color Cyan = Green + Blue;
const Color Magenta = Red + Blue;
const Color Purple = (Red + Blue) / 2;
const Color Silver = {0.75, 0.75, 0.75};
const Color Pink = {1, 0.75, 0.8};
const Color Maroon = {0.5, 0, 0};
const Color Lime = {0.75, 1, 0};
const Color SkyBlue = {0.53, 0.81, 0.92};
const Color Flame = {0.89, 0.35, 0.13};

// Add 10+ new colors that you think will be useful for generating
// beautiful scenes, e.g. primary colors, secondary colors, extras
// (fuschia?, sky blue?, sun?, molten metal/fire/magma?, etc)
