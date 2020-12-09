# QuickShot - Tennis Shooting Algorithm

We have created a Python library (QuickShot) for calculating the exact parameters needed for landing a ball to a given location at a given speed/spin using TennisBot v1.

All the calculations/equations here are modified based on this [paper](http://www.actawm.pb.edu.pl/volume/vol5no4/WOJCICKI_KULESZA_PUCILOWSKI_EN_2010_085.pdf).

## Usage

```python
# Initiate shooter with the robot's physical capabilities
q = quickshot.Shooter(
  min_rpm=400,
  max_rpm=8400,
  min_angle=0,  # min/max angle in degrees
  max_angle=80,
)

# Calculate the parameters needed for shooting the ball.
params = q.calc_params(
  y=200,  # distance (cm) from the robot
  speed=20,  # speed in meters per second
  top_spin=0, # top/back spin in meters per second
  back_spin=0,
)

# Prepare the shooter (spinning up the motors) for the shot.
shooter.prepare(
  top=params['top_motor'],
  bottom=params['bottom_motor'],
  angle=params['angle'],
)

shooter.shot_once_ready()
```

All the source code for QuickShot is located in [/lib/quickshot](../lib/quickshot).
