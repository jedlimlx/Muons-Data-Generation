# $\mu$-Net: ConvNeXt-Based U-Nets for Muon Tomography

This is the code for the data generation for the paper, $\mu$-Net: ConvNeXt-Based U-Nets for Muon Tomography. 
The code for this paper can be found at https://github.com/jedlimlx/Muon-Tomography-AI/. 
The dataset is found at https://www.kaggle.com/datasets/tomandjerry2005/muons-scattering-dataset.

## Data Generation Methods

![](https://www.googleapis.com/download/storage/v1/b/kaggle-user-content/o/inbox%2F3246529%2F8fb087cd90fadc29cbc4f5b614ebf09d%2Fgeant4_setup_v2.png?generation=1692002305568155&alt=media)

The data is generated using CERN’s Geant4 simulation software. To generate the 3D objects, we make use of fractal noise to generate objects of various shapes. The material of the object is randomly chosen from a set list of materials of different radiation lengths.

The geometry of the system can be found above. The target object is contained within a cube of side length 1 m. The input and output detectors are squares of side length 2 m. They are separated from the object by a distance of 0.5 m.

For the muon beam, we use a beam with a $cos^2$ angular distribution and a power law distribution, in
accordance with characterised values of the cosmic muon flux. Muons are calculated not to hit the
detector are killed at the start of the simulation to ensure the simulation runs at a reasonable speed for data generation.

## Running the Code

First, you will need to setup CERN's Geant4. It can be downloaded from https://geant4.web.cern.ch/. This was built with Geant4 v11.1.2.

Then, run
```
mkdir build
cd build
cmake ..
make
```

Once done, run
```
mkdir voxels
mkdir output
python3 run.py
```

The configurations within `run.py` can be adjusted accordingly, depending on your desired output directory.

## Changing configuration

There are several files which you can use to adjust the settings for the simulation. 

1. To adjust the geometry, change the 
settings in [CONSTANTS.hh](src/CONSTANTS.hh). You can adjust the resolution of the simulation, 
the size of the target object, distance from the detectors, etc. Do note the resolution should be adjusted within
[run.py](run.py) as well. You will also need to recompile the entire project by running `make` in `build`.

2. To adjust the dosage, muon angular and spacial distribution, change [run.mac](macros/run.mac). You don't need to worry
about copying it to other directories, [run.py](run.py) will do that for you. Also note that since many muons do not hit the
final detector, the dosage you will see in the final CSV output is not necessarily the same (and is usually much lower) than
what is indicated in [run.mac](macros/run.mac).

3. To adjust the resolution of the target object and the way it is generated, change [run.py](run.py).


## Output Format

You can refer to [detector.cc](src/detector.cc) for the output format. 
Also note that muons in the simulation travel from the positive z-direction to the negative z-direction.

Basically, the program outputs headerless *.csv files.

The columns in order are:
- x position of detection (mm)
- y position of detection (mm)
- z position of detection (mm)
- x component of momentum (MeV/c)
- y component of momentum (MeV/c)
- z component of momentum (MeV/c)
- initial x position (mm)
- initial y position (mm)
- initial z position (mm)
- x-component of initial momentum (normalised)
- y-component of initial momentum (normalised)
- z-component of initial momentum (normalised)
- initial energy of the muon (MeV)
