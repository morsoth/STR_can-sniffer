#ifndef CAN_DATA_H
#define CAN_DATA_H

#include "CAN.h"

#define BUFF_SIZE 200

#define POSITION_X 400
#define POSITION_Y 401
#define POSITION_VX 402
#define POSITION_VY 403
#define MOTOR_ACCDIR 768
#define LIDAR_ANGLE 512
#define CAMERA_ANGLE 256

CAN can(PB_5, PB_13);

CANMessage canMsg;

String canHTMLMessages = "START";

// DATA -----------
/* RED */
/* TELEMETRY */
/* POSITION */
static int64_t PositionX = 0; // 400
static int64_t PositionY = 0; // 401
static vector<int32_t> PositionVx(BUFF_SIZE, 0); // 402
static vector<int32_t> PositionVy(BUFF_SIZE, 0); // 403
/* MOTOR */
static int8_t MotorDir = 0;  // 768
static vector<int8_t> MotorAcc(BUFF_SIZE, 0);  // 768
/* LIDAR */
static int16_t LidarAngle = 0;  // 512
/* CAMARA */
static int16_t CameraAngle = 0;  // 256
// ----------------

#endif