#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Servo.h>

class MotorController {
private:
    Servo leftMotor;
    Servo rightMotor;

    int leftMotorPin;
    int rightMotorPin;

    float leftSpeed;
    float rightSpeed;

    bool movementEnabled;
    bool joystickEnabled;

    //---Kalibrációs értékek---
    float PWM_MAX_FOR = 1580;
    float CEN         = 1498;
    float PWM_MAX_BAC = 1420;
    float X_MAX       = 1022;
    float Y_MAX       = 1022;
    float X_MID       = 515;
    float Y_MID       = 516;

    float PWM_MAX_BAC_BUT = 1420;
    float PWM_MAX_FOR_BUT = 1580;

    float FORWARD_CORNERING_SEPARATION = 3.0f;

public:
    MotorController(int leftPin, int rightPin) {
        leftMotorPin = leftPin;
        rightMotorPin = rightPin;

        leftSpeed = CEN;
        rightSpeed = CEN;

        movementEnabled = true;
        joystickEnabled = true;
    }

    void init() {
        leftMotor.attach(leftMotorPin);
        rightMotor.attach(rightMotorPin);
        updateMotors();
    }

    void updateMotors() {
        if (!movementEnabled) {
            leftMotor.writeMicroseconds((int) CEN);
            rightMotor.writeMicroseconds((int) CEN);
            return;
        }

        leftMotor.writeMicroseconds((int) leftSpeed);
        rightMotor.writeMicroseconds((int) rightSpeed);
    }

    void processJoystickInput(float X, float Y) {
        if (!movementEnabled || !joystickEnabled)
            return;

        float DIR = -1.0f;
        if(Y >= Y_MID) DIR = -1.0f;
        if(Y <  Y_MID) DIR = +1.0f;

        //SEBESSÉG
        float PWM = CEN;
        if(Y >= Y_MID){
          PWM = CEN + ( ( PWM_MAX_FOR - CEN ) * (( Y - Y_MID ) / Y_MID ) );
        }else{
          PWM = CEN - ( ( CEN - PWM_MAX_BAC ) * ( 1.0F - ( Y / Y_MID ) ) );
        }

        int DIF_MAX = fabs( PWM - CEN );

        float DIF_LEF = 0.0f;
        float DIF_RIG = 0.0f;
        float PWM_LEF = PWM;
        float PWM_RIG = PWM;

        //FORDULÁS
        if(X > X_MID + FORWARD_CORNERING_SEPARATION) {
          PWM_LEF = CEN + ( ( PWM_MAX_FOR - CEN ) * (( X - X_MID ) / X_MID ) );
          PWM_RIG = CEN;
        }
        if(X < X_MID -FORWARD_CORNERING_SEPARATION) {
          PWM_LEF = CEN;
          PWM_RIG = CEN + ( ( PWM_MAX_FOR - CEN ) * ( 1.0F - ( X / X_MID ) ) );
        }

        setLeftSpeed(PWM_LEF);
        setRightSpeed(PWM_RIG);
    }

    void setLeftSpeed(float speed) {
        leftSpeed = speed;
        updateMotors();
    }

    void setRightSpeed(float speed) {
        rightSpeed = speed;
        updateMotors();
    }

    void stop() {
        movementEnabled = false;
        updateMotors();
    }

    void start() {
        movementEnabled = true;
        updateMotors();
    }

    void leftTurn() {
        joystickEnabled = false;
        setLeftSpeed(CEN);
        setRightSpeed(PWM_MAX_FOR_BUT);
    }

    void rightTurn() {
        joystickEnabled = false;
        setLeftSpeed(PWM_MAX_FOR_BUT);
        setRightSpeed(CEN);
    }

    void reset() {
        joystickEnabled = true;
        movementEnabled = true;
        setLeftSpeed(CEN);
        setRightSpeed(CEN);
    }

    void forward() {
        joystickEnabled = false;
        setLeftSpeed(PWM_MAX_FOR_BUT);
        setRightSpeed(PWM_MAX_FOR_BUT);
    }

    void backward() {
        joystickEnabled = false;
        setLeftSpeed(PWM_MAX_BAC_BUT);
        setRightSpeed(PWM_MAX_BAC_BUT);
    }

    float getLeftSpeed() const { return leftSpeed; }
    float getRightSpeed() const { return rightSpeed; }
    bool isMovementEnabled() const { return movementEnabled; }
    bool isJoystickEnabled() const { return joystickEnabled; }
};

#endif