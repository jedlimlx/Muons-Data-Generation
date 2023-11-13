// resolution of the voxels
#define RESOLUTION 128

// geometry looks like this (top view)
// *             padd-              *
// * m             ing              * d
// * u         ----------           * e
// * o         -        -  detector * t
// * n         - object -  distance * e
// *           -        -           * c
// * g         -        -           * t
// * u         ----------           * o
// * n         target size          * r
// *                                *

// dimensions of geometry (in metres)
#define TARGET_SIZE 1.0  // cube of TARGET_SIZE side length
#define PADDING 0.5  // size of detector in addition to TARGET_SIZE (TARGET_SIZE + 2 * PADDING)
#define DETECTOR_DISTANCE 0.5  // distance of detector from target (TARGET_SIZE / 2 + DETECTOR_DISTANCE)
