# Black Hole Simulator
_N-Body gravity simulation_

## Build and execution environment

Visual Studio 2015, Windows.

For 8000 particles, need 300MB memory. It takes about 7 minutes on　4-core CPU with hyperthreading for 500 frames and 8000 particles. A 92MB .3dp file will be created. Play this file with 3dp-player.

https://github.com/hypernumbernet/3dp-player

## About included projects

###quaternion-logarithm

All particles are in the space above the unit quaternion. The gravitational interaction is calculated with the natural log of the quaternion. Interestingly, it results in a whirlpool like a galaxy.

###3-sphere

The method of this simulation is not related the real universe's black hole.　The purpose of this program is a research for the behavior of particles on 3-sphere (in 4 dimensions) space. In this simulator, all particles are located on the unit quaternion and calculated by the octonion slerp. The gravitational force between particles is figured out by Newton's law of gravity.

###general-relativity

It is a simulation of a distorted space using Einstein's general relativity. Approximate gravity as being transmitted instantaneously, use the Schwarzschild solution and accumulate the skewness of spacetime.

#Images

## 3-sphere simulation

![3-sphere black hole](https://github.com/hypernumbernet/blackhole-simulator/blob/master/images/3-sphere%20black%20hole.png?raw=true)

![twin black hole](https://github.com/hypernumbernet/blackhole-simulator/blob/master/images/twin%20black%20hole.png?raw=true)

## quaternion-logarithm simulation

![quaternion logarithm 001](https://github.com/hypernumbernet/blackhole-simulator/blob/master/images/galaxy%20like.png?raw=true)

![quaternion logarithm 002](https://github.com/hypernumbernet/blackhole-simulator/blob/master/images/galaxy%20ling.png?raw=true)
