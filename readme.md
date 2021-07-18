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
4. npm run test
   
## Positioning API
### POST /move
Using physical angles to manipulate roboarm

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

### POST /position
Pick a point in 3D coordinate system and roboarm evaluates engine angles to move at specific point

Move to home
```json
{
  "claw-x": 95,
  "claw-y": 0,
  "claw-z": 25
}
```

#### Move claw parallel floor

If "claw-angle-y" is defined the arm will try to move to specific point with specific claw Y angle.

The claw will be parallel floor.
```json
{
  "claw-x": 250,
  "claw-y": 0,
  "claw-z": 80,
  "claw-angle-y": 0
}
```
The arm will be moved back but claw stays parallel floor  
```json
{
  "claw-x": 60,
  "claw-y": 0,
  "claw-z": 80,
  "claw-angle-y": 0
}
```
It`s useful to grab teaspoon with sugar and move it without scattering 

#### Move claw vertical

If you set "claw-angle-y" to 90 degrees when claw will be vertical 

```json
{
  "claw-x": 95,
  "claw-y": 0,
  "claw-z": 300,
  "claw-angle-y": 90
}
```

The arm will move forward, but claw stays vertical

```json
{
  "claw-x": 150,
  "claw-y": 0,
  "claw-z": 300,
  "claw-angle-y": 90
}
```

#### Move claw perpendicular floor

Set "claw-angle-y" to 270 and claw will be rotated perpendicular floor

```json
{
  "claw-x": 120,
  "claw-y": 0,
  "claw-z": 60,
  "claw-angle-y": 270
}
```

```json
{
  "claw-x": 125,
  "claw-y": 10,
  "claw-z": 60,
  "claw-angle-y": 270
}
```

```json
{
  "claw-x": 130,
  "claw-y": 0,
  "claw-z": 60,
  "claw-angle-y": 270
}
```

```json
{
  "claw-x": 125,
  "claw-y": -10,
  "claw-z": 60,
  "claw-angle-y": 270
}
```

The claw will describe a circle with radius 10.
It`s usefull to mix sugar and water

Additional test cases can be found at [tests](/tests "tests")

## Information API
### GET /version
Returns current version of software and additional info like free heap 

## Update API
### GET /update
Returns simple web interface to upload new firmware

### POST /update
Send firmware directly through POST request





