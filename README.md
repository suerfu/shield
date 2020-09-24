# McKinsey_Janis_Fridge

## Brief Documentation
The brief documentation for users who already have knowledge in GEANT, root and the light yield measurement experiment is in the link below. Don't hesitate to ask for access if necessary, please.

https://docs.google.com/presentation/d/1XVVvk1fUp9ltZ9JOY8lGnXmUlc1viKG-4jQx4Zgf-EY/edit#slide=id.g7d26b4542d_0_173

Please don't hesitate to email Lanqing who wrote this for any question. The codes are awkward, Lanqing will try to relieve you pain.

Lanqing Yuan: yuanlq@berkeley.edu; yuanlq@shanghaitech.edu.cn (Please cc this one)

(below is a plan for this simulation project, written in Aug, 2019)

## Introduction
In dark matter direct detection experiments, it is calibration signals from the detector to sources which produce low-energy neutron recoils that build a bridge between super-sensitive detectors and capture of possible dark matter. Scintillation response can be used to derive the deposited energy on an event-by-event basis, with both type of interaction in the detector and recoil energy as contributing factors. 4-Helium is a good material for neutron recoil, and successful neutron recoil calibration requires an understanding of the particle source as well as the activities inside it, which can be obtained by a combination of experiment data with Monte Carlo simulations.

The goal of this project is to use GEANT package to set up simulations with similar geometry shown in the figure and well understood scattering behaviour in 4-Helium for Janis fridge in B255. 


<img src="figures/Arktis.png" width = "70%" />


##  Steps
### Fix the Geometry
There were some codes of similar geometry available, however the dimensions are not accurate enough to produce perfect simulations. The first step of the project is to build the correct geometry for experiments in Janis fridge based on a combination of CAD or drawing and measurement as a check since part of the fridge like sample chambers have been modified. Possible work may also be done on searching for a reasonable approximation of the dimension of the detector considering both convenience to build the geometry as well as acceptably little influence on the received signals.

### Hard-coding the Neutron Properties
Previously, we have assumed the neutrons from DD neutron generator to be distributed isotropically from the source plane, which is actually not true but matters comparatively little on the interaction. Here a more accurate distribution of neutrons from the source will be determined based on experiments and be hard-coded into the simulation. Naive theoretical calculation evaluating the influences or sensitivity of changes in signal collected on detectors due to a different geometry behavior of the source may also be derived prior to hard-coding if necessary.

### Better Event Information
This may be the most challenging part of this project. A detailed event-by-event calibration of signals will be determined based on well understood physics inside the Helium and possible mechanisms on the detectors. Ideally each line as output of GEANT program will be with a corresponding activity in Helium.

### Command Line Geometry Configuration
In real experiments, the detectors may be placed in a series of angles to detect events of different energy deposited. To balance between the computation time on clusters and influence on the quality of the simulation, we will simulate a couple of detectors at the same time, which means the simulation geometry may be a little bit different from the real one, which means modification should be added to restrict the allowed behaviour. For example, neutrons scattering between detectors are not allowed and should be ignored. Ideally we will be able to change the geometry while running without the time wasted in another compilation.
