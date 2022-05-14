# README
Geant4 simulation for generating training data for CNN-based image reconstruction.
Objects are generated using 3D Perlin noise.

This branch contains the latest code as we iterate on the work.

Perlin noise code: https://github.com/sol-prog/Perlin_Noise

Detector and beam setup (Contains build instructions): https://github.com/MMhaidra/geant4-example_muon

# Multiple Orientations
run.py added to rotate object.
Improved model performance on test dataset.
|Accuracy|Precision|Recall|AUC|
|--------|---------|------|---|
|0.92|0.91|0.74|0.97|
