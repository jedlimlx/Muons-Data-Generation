// resolution of the voxels
#define RESOLUTION 64

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
#define TARGET_SIZE 1.0 //0.4  // cube of TARGET_SIZE side length
#define PADDING 0.5 //0.1  // size of detector in addition to TARGET_SIZE (TARGET_SIZE + 2 * PADDING)
#define DETECTOR_DISTANCE 0.5 //0.3  // distance of detector from target (TARGET_SIZE / 2 + DETECTOR_DISTANCE)

// compute trajectory
#define TRAJECTORY false
