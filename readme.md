# 6DOF Roboarm manipulator software

This project is intended to make a complex things with manipulator through Wi-Fi
Like positioning in 3D unit system, grab objects by specific angles and more.


I used 6DOF Robotic arm with parameters on sheme below.
 
![Roboarm scheme](/docs/scheme.png "Roboarm scheme")

Physical parameters of roboarm can be adjusted in [armParams.h](/include/armParams.h "armParams.h")

## Build

The project is builded in VSCode using esp32doit-devkit-v1 with arduino framework

## Tests

For autotest system nodejs and mocha are used.

1. Install nodejs [nodejs](https://nodejs.org "nodejs")
2. cd /tests
3. npm install
4  npm run test
   
## API

### /move
Using angles to manipulate roboarm

Move to home
```json
{
  "gripper": 1,
  "gripper_rotate": 135,
  "rotate": 135,
  "shoulder": 90,
  "elbow": 230,
  "wrist": 235
}
```

### /position
Pick a point in 3D coordinate system and roboarm evaluates engine angles to move at specific point

Move to home
```json
{
  "x": 95,
  "y": 0,
  "z": 25
}
```




