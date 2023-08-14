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
make
```

Once done, run
```
mkdir voxels
mkdir output
python3 run.py
```

The configurations within `run.py` can be adjusted accordingly.
