# 6DOF Roboarm manipulator software

This project is intended to make a complex things with roboarm manipulator through Wi-Fi
Like positioning in 3D unit system, grab objects by specific angles and more.

* Manipulate roboarm by picking point in 3D environment. Controller evaluates engine angle to achieve that point.
* Queue support
* Engine synchronization support
* You can grab and move object using specific claw angle 


I used 6DOF Robotic arm with parameters on sheme below.
 
![Roboarm scheme](/docs/scheme.png "Roboarm scheme")

Physical parameters of roboarm can be adjusted in [armParams.h](/include/armParams.h "armParams.h")

## Scenarios
For better understanding look at video

[[Make me a coffee Scenario]](https://raintech.su/video/roboarm-coffee.mp4 "Making a coffee")

Scenarios code can be found here [scenarios](/tests/scenarios "scenarios")

## Build

The project was built in VSCode using esp32doit-devkit-v1 with arduino framework

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

Set "claw-angle-y" to -90 and claw will be rotated perpendicular floor

```json
{
  "claw-x": 120,
  "claw-y": 0,
  "claw-z": 60,
  "claw-angle-y": -90
}
```

```json
{
  "claw-x": 125,
  "claw-y": 10,
  "claw-z": 60,
  "claw-angle-y": -90
}
```

```json
{
  "claw-x": 130,
  "claw-y": 0,
  "claw-z": 60,
  "claw-angle-y": -90
}
```

```json
{
  "claw-x": 125,
  "claw-y": -10,
  "claw-z": 60,
  "claw-angle-y": -90
}
```


The claw will describe a circle with radius 10.
It`s useful to mix sugar and water

#### Rotate claw over X axis

Set "claw-angle-x" parameter to rotate claw over the X axis

```json
{
"claw-x": 130,
"claw-y": 0,
"claw-z": 60,
"claw-angle-y": -90,
"claw-angle-x": -90
}
```

#### Rotate open and close claw

Set "claw-angle" parameter to open or close claw

Open claw
```json
{
"claw-x": 130,
"claw-y": 0,
"claw-z": 60,
"claw-angle-y": -90,
"claw-angle-x": 0,
"claw-angle": 140
}
```

Close claw
```json
{
"claw-x": 130,
"claw-y": 0,
"claw-z": 60,
"claw-angle-y": -90,
"claw-angle-x": 0,
"claw-angle": 0
}
```
## Speed and scenario support

All commands you send to arm will be added to internal queue. The size of queue is defined in 
[armParams.h](/include/armParams.h "armParams.h") in COMMAND_QUEUE_SIZE. It`s set to 20 by default.

Make two requests
POST /position
with following parameters

```json
{
  "claw-x": 250,
  "claw-y": 0,
  "claw-z": 80,
  "claw-angle-y": 0,
  "post-delay": 5000,
  "iterations": 1
}
```
```json
{
  "claw-x": 60,
  "claw-y": 0,
  "claw-z": 80,
  "claw-angle-y": 0,
  "iterations": 60,
  "iteration-delay": 50
}
```

* The arm will move to position (250, 0, 80) very fast.
* It waits 5 seconds
* When moves to position (60, 0, 80) very slow

Whole path will be divided to iterations. By default, it`s set 20 and defined in DEFAULT_ITERATIONS.
"iteration-delay" is delay between every iteration, 
so total time of command will be **iterations** * **iteration-delay** (60 * 50) = 3000 milliseconds.

Use **POST /version** to know how much operations queued right now. When *arm-operation-queued* parameter will be 0 it means
the arm finished current scenario and entered to standby mode.

Additional test cases can be found at [tests](/tests "tests")

## Information API
### POST /version
Returns current version, the number operations in queue and free heap.

## Update API
### GET /update
Returns simple web interface to upload new firmware

### POST /update
Send firmware directly through POST request





