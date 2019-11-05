//============================================================================//
// include stuff //
#include "main.h"
using namespace okapi;

//============================================================================//
// declare stuff //
const int LEFT_FRONT = 1; //adding a "-" in front reverses the motor
const int LEFT_BACK = -2;
const int RIGHT_FRONT = -3;
const int RIGHT_BACK = 4;

const int TILTER_PORT = 6;
const int INTAKE_PORT_1 = 7;
const int INTAKE_PORT_2 = -8;
const int LIFT_PORT = 9;

auto drive = ChassisControllerFactory::create(
{LEFT_FRONT, LEFT_BACK},
{RIGHT_FRONT, RIGHT_BACK},
AbstractMotor::gearset::green,
{4_in, 12_in} // wheel diameter, robot width
);

auto profileController = AsyncControllerFactory::motionProfile(
  0.7,  // Maximum linear velocity of the Chassis in m/s
  2.0,  // Maximum linear acceleration of the Chassis in m/s/s
  10.0, // Maximum linear jerk of the Chassis in m/s/s/s
  drive // Chassis Controller
);

ControllerButton tilterBttn(ControllerDigital::B);
ControllerButton intakeIn(ControllerDigital::L1);
ControllerButton intakeOut(ControllerDigital::L2);
ControllerButton liftUp(ControllerDigital::R1);
ControllerButton liftDown(ControllerDigital::R2);

Motor tilter(TILTER_PORT);
Motor lift(LIFT_PORT);
MotorGroup intake({INTAKE_PORT_1, INTAKE_PORT_2});

bool intaking=false;

Controller master;

//============================================================================//
// custom functions //
void drive_op(){
	drive.arcade(master.getAnalog(ControllerAnalog::leftY),
						 master.getAnalog(ControllerAnalog::leftX));
}

void tilt_op(){
  tilter.moveVoltage(master.getAnalog(ControllerAnalog::rightY)*10000);
}

void lift_op(){
  if(liftUp.isPressed()){
    lift.moveVoltage(12000);
  } else if(liftDown.isPressed()){
    lift.moveVoltage(-6000);
  } else {
    lift.moveVoltage(0);
  }
}

void intake_op(){
	if(intakeIn.changedToPressed()){
    intaking=!intaking;
	}

  if(intaking){
    intake.moveVoltage(12000);
  } else {
    if(intakeOut.isPressed()){
      intake.moveVoltage(-8000);
    } else {
      intake.moveVoltage(0)
    }
  }
}

//============================================================================//
// run //
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Paige");
}

void disabled() {}

void competition_initialize() {
  /*
  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 1_ft, 0_deg}}, "A");
  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "B");
  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point(2_ft, 0.5_ft, 20_deg}}, "C");
  */
}

void autonomous() {
  /*
  profileController.setTarget("A");
  profileController.waitUntilSettled();
  profileController.setTarget("B");
  */
}

void opcontrol() {

	while (true) {
		drive_op();
		tilt_op();
		lift_op();
		intake_op();

		pros::delay(10);
	}
}
