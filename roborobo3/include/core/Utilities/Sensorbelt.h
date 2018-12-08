#ifndef SENSORBELT
#define SENSORBELT

// Sensor Ids mapped to readable strings
//
//
// Usage in source file:
//      #define NB_SENSORS X // with X = 8, 9, 12, 15, 16 or 18
//      #include "Misc/Sensorbelt.h"
//
// Of course, you should ensure that the gRobotSpecsImageFilename property value points to a file that is coherent w.r.t. the NB_SENSORS value. I.e.:
//          8  <=> minirobot-specs-8sensors.bmp
//          9  <=> minirobot-specs-9sensorsFront.bmp
//          12 <=> minirobot-specs-12sensors.bmp
//          15 <=> minirobot-specs-15sensorsFront.bmp
//          16 <=> minirobot-specs-16sensors.bmp
//          18 <=> minirobot-specs-18sensorsFront.bmp
//          any other file: dont use Sensorbelt.h
//
// (L)eft, (R)ight, (B)ack, (F)ront
//
// F is a front sensor, pointing towards the robot (0° w.r.t. direction)
// FFL is a front-front-left sensor, pointing very slightly to the left (22.5°).
// FL is a front-left sensor (45°)
// FLL is a front-left-left sensor, pointing nearly to the left (67.5°)
// etc.
// There may be some exceptions -- see comments below.

//
// 8 sensors
// ASSUMES #define NB_SENSORS 8
// ASSUMES gRobotSpecsImageFilename = data/minirobot-specs-8sensors.bmp
//
#if NB_SENSORS == 8
#define SENSOR_L 0
#define SENSOR_FL 1
#define SENSOR_FFL 2
#define SENSOR_FFR 3
#define SENSOR_FR 4
#define SENSOR_R 5
#define SENSOR_BR 6
#define SENSOR_BL 7
#endif

//
// 9 sensors
// ASSUMES #define NB_SENSORS 9
// ASSUMES gRobotSpecsImageFilename = data/minirobot-specs-9sensors.bmp
//
#if NB_SENSORS == 9
#define SENSOR_L 0
#define SENSOR_FLL 1
#define SENSOR_FL 2
#define SENSOR_FFL 3
#define SENSOR_F 4
#define SENSOR_FFR 5
#define SENSOR_FR 6
#define SENSOR_FRR 7
#define SENSOR_R 8
#endif

//
// 12 sensors
// ASSUMES #define NB_SENSORS 12
// ASSUMES gRobotSpecsImageFilename = data/minirobot-specs-12sensors.bmp
//
#if NB_SENSORS == 12
#define SENSOR_L 0
#define SENSOR_FLL 1
#define SENSOR_FL 2
#define SENSOR_FFL 3
#define SENSOR_F 4
#define SENSOR_FFR 5
#define SENSOR_FR 6
#define SENSOR_FRR 7
#define SENSOR_R 8
#define SENSOR_BR 9
#define SENSOR_B 10
#define SENSOR_BL 11
#endif

//
// 16 sensors
// ASSUMES #define NB_SENSORS 16
// ASSUMES gRobotSpecsImageFilename = data/minirobot-specs-16sensors.bmp
//
#if NB_SENSORS == 16
#define SENSOR_L 0
#define SENSOR_FLL 8
#define SENSOR_FL 1
#define SENSOR_FFL 9
#define SENSOR_F 2
#define SENSOR_FFR 10
#define SENSOR_FR 3
#define SENSOR_FRR 11
#define SENSOR_R 4
#define SENSOR_BRR 12
#define SENSOR_BR 5
#define SENSOR_BBR 13
#define SENSOR_B 6
#define SENSOR_BBL 14
#define SENSOR_BL 7
#define SENSOR_BLL 15
#endif

//
// 15 sensors (covering the front [-90°,+-90°])
// the -90° and +90° sensors are placed slightly back from the center.
// ASSUMES #define NB_SENSORS 15
// ASSUMES gRobotSpecsImageFilename = data/minirobot-specs-15sensorsFront.bmp
//
#if NB_SENSORS == 15
#define SENSOR_L 0          // -90°
#define SENSOR_F6L 1        // -77°
#define SENSOR_F5L 2        // -64°
#define SENSOR_F4L 3        // -52°
#define SENSOR_F3L 4        // -39°
#define SENSOR_FLL 5        // -26°
#define SENSOR_FL 6         // -13°
#define SENSOR_F 7          // 0° (front)
#define SENSOR_FR 8         // 13°
#define SENSOR_FRR 9        // 26°
#define SENSOR_F3R 10       // 38°
#define SENSOR_F4R 11       // 51°
#define SENSOR_F5R 12       // 64°
#define SENSOR_F6R 13       // 77°
#define SENSOR_R 14         // 90°
#endif

//
// 18 sensors (15 covering the front [-90°,+-90°] and 3 on the back (135°,180°,-135°)
// the -90° and +90° sensors are placed slightly back from the center.
// ASSUMES #define NB_SENSORS 18
// ASSUMES gRobotSpecsImageFilename = data/minirobot-specs-18sensorsFront.bmp
//
#if NB_SENSORS == 18
#define SENSOR_L 0          // -90°
#define SENSOR_F6L 1        // -77°
#define SENSOR_F5L 2        // -64°
#define SENSOR_F4L 3        // -52°
#define SENSOR_F3L 4        // -39°
#define SENSOR_FLL 5        // -26°
#define SENSOR_FL 6         // -13°
#define SENSOR_F 7          // 0° (front)
#define SENSOR_FR 8         // 13°
#define SENSOR_FRR 9        // 26°
#define SENSOR_F3R 10       // 38°
#define SENSOR_F4R 11       // 51°
#define SENSOR_F5R 12       // 64°
#define SENSOR_F6R 13       // 77°
#define SENSOR_R 14         // 90°
#define SENSOR_BR 15        // 135° (back-right)
#define SENSOR_B 16         // 180° (back)
#define SENSOR_BL 17        // 225° (back-left)
#endif

#endif // SENSORBELT
