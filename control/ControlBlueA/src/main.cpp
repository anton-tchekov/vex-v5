#include "vex.h"
#include <fstream>
#define FILENAME "rec_ba"

using namespace vex;

const int driveMin = 5;
const int clawOpenPosition = 107;
const int clawClosedPosition = 10;
const int clawVelocity = 20;
const int armVelocity = 20;
const int slowFactor = 3;
const int parkTimeout = 200;
const int parkMove = 5;
const int loopTimeout = 10;
const int numReadings = 7 * (1000 / loopTimeout);

competition Competition;
int8_t recLeft[numReadings], recRight[numReadings], recArm[numReadings], recClaw[numReadings];
bool clawOpen, l1Pressed, modeDriver;
int parkClawPos, parkClawLast, recIdx;

void loop_driver();
void loop_auton();
void (*loop_callback)();

void event_park_claw()
{
	parkClawPos = MotorClaw.position(degrees);
	if(abs(parkClawPos - parkClawLast) < parkMove)
	{
		MotorClaw.stop();
		MotorClaw.setPosition(0, degrees);
		MotorClaw.spinToPosition(clawClosedPosition, degrees);
		clawOpen = false;
		loop_callback();
		return;
	}

	parkClawLast = parkClawPos;
	Brain.Timer.event(event_park_claw, parkTimeout);
}

void init()
{
	MotorArm.setStopping(coast);
	MotorArm.setMaxTorque(100, percent);
	MotorArm.setVelocity(armVelocity, percent);
	MotorClaw.setStopping(hold);
	MotorClaw.setVelocity(clawVelocity, percent);
	MotorClaw.setMaxTorque(100, percent);
	MotorClaw.spin(reverse);
	MotorLeft.setMaxTorque(100, percent);
	MotorRight.setMaxTorque(100, percent);
	parkClawLast = 0;
	Brain.Timer.event(event_park_claw, parkTimeout);
}

void loop_auton()
{
	if(modeDriver)
	{
		return;
	}

	MotorLeft.setVelocity(recLeft[recIdx], percent);
	MotorRight.setVelocity(recRight[recIdx], percent);
	MotorLeft.spin(forward);
	MotorRight.spin(forward);

	switch(recArm[recIdx])
	{
		case -1:
			MotorArm.spin(reverse);
			break;

		case 0:
			MotorArm.stop();
			break;

		case 1:
			MotorArm.spin(forward);
			break;
	}

	if(recClaw[recIdx])
	{
		if(MotorClaw.position(degrees) < clawOpenPosition)
		{
			MotorClaw.spin(forward);
		}
		else
		{
			MotorClaw.stop();
		}
	}
	else
	{
		if(MotorClaw.position(degrees) > clawClosedPosition)
		{
			MotorClaw.spin(reverse);
		}
		else
		{
			MotorClaw.stop();
		}
	}

	++recIdx;
	if(recIdx == numReadings)
	{
		Brain.Screen.print("Replay End");
		Brain.Screen.newLine();
		return;
	}

	Brain.Timer.event(loop_auton, loopTimeout);
}

void loop_record()
{
	int xSlow, xFast, ySlow, yFast;
	if(!modeDriver)
	{
		return;
	}

	if(Controller1.ButtonL1.pressing())
	{
		if(!l1Pressed)
		{
			clawOpen = !clawOpen;
			l1Pressed = true;
		}
	}
	else
	{
		l1Pressed = false;
	}

	if(Controller1.ButtonR1.pressing())
	{
		MotorArm.spin(forward);
		recArm[recIdx] = 1;
	}
	else if(Controller1.ButtonR2.pressing())
	{
		MotorArm.spin(reverse);
		recArm[recIdx] = -1;
	}
	else
	{
		MotorArm.stop();
		recArm[recIdx] = 0;
	}

	if(clawOpen)
	{
		recClaw[recIdx] = 1;
		if(MotorClaw.position(degrees) < clawOpenPosition)
		{
			MotorClaw.spin(forward);
		}
		else
		{
			MotorClaw.stop();
		}
	}
	else
	{
		recClaw[recIdx] = 0;
		if(MotorClaw.position(degrees) > clawClosedPosition)
		{
			MotorClaw.spin(reverse);
		}
		else
		{
			MotorClaw.stop();
		}
	}

	xSlow = Controller1.Axis4.position();
	xFast = Controller1.Axis1.position();
	ySlow = Controller1.Axis3.position();
	yFast = Controller1.Axis2.position();
	if(abs(ySlow) > driveMin)
	{
		ySlow /= slowFactor;
		MotorLeft.setVelocity(ySlow, percent);
		MotorRight.setVelocity(ySlow, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
		recLeft[recIdx] = ySlow;
		recRight[recIdx] = ySlow;
	}
	else if(abs(xSlow) > driveMin)
	{
		xSlow /= slowFactor;
		MotorLeft.setVelocity(xSlow, percent);
		MotorRight.setVelocity(-xSlow, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
		recLeft[recIdx] = xSlow;
		recRight[recIdx] = -xSlow;
	}
	else if(abs(yFast) > driveMin)
	{
		MotorLeft.setVelocity(yFast, percent);
		MotorRight.setVelocity(yFast, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
		recLeft[recIdx] = yFast;
		recRight[recIdx] = yFast;
	}
	else if(abs(xFast) > driveMin)
	{
		MotorLeft.setVelocity(xFast, percent);
		MotorRight.setVelocity(-xFast, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
		recLeft[recIdx] = xFast;
		recRight[recIdx] = -xFast;
	}
	else
	{
		MotorLeft.stop();
		MotorRight.stop();
		recLeft[recIdx] = 0;
		recRight[recIdx] = 0;
	}

	++recIdx;
	if(recIdx == numReadings)
	{
		Brain.Screen.print("Recording End");
		Brain.Screen.newLine();
		Brain.Screen.print("Writing to File");
		Brain.Screen.newLine();
		std::ofstream ofs(FILENAME, std::ofstream::out);
		for(int i = 0; i < numReadings; ++i)
		{
			ofs << recLeft[i];
			ofs << recRight[i];
			ofs << recArm[i];
			ofs << recClaw[i];
		}

		ofs.close();
		Brain.Screen.print("Ready");
		Brain.Screen.newLine();
		Brain.Timer.event(loop_driver, loopTimeout);
		return;
	}

	Brain.Timer.event(loop_record, loopTimeout);
}

void loop_driver()
{
	int xSlow, xFast, ySlow, yFast;
	if(!modeDriver)
	{
		return;
	}

	if(Controller1.ButtonL1.pressing())
	{
		if(!l1Pressed)
		{
			clawOpen = !clawOpen;
			l1Pressed = true;
		}
	}
	else
	{
		l1Pressed = false;
	}

	if(Controller1.ButtonR1.pressing())
	{
		MotorArm.spin(forward);
	}
	else if(Controller1.ButtonR2.pressing())
	{
		MotorArm.spin(reverse);
	}
	else
	{
		MotorArm.stop();
	}

	if(clawOpen)
	{
		if(MotorClaw.position(degrees) < clawOpenPosition)
		{
			MotorClaw.spin(forward);
		}
		else
		{
			MotorClaw.stop();
		}
	}
	else
	{
		if(MotorClaw.position(degrees) > clawClosedPosition)
		{
			MotorClaw.spin(reverse);
		}
		else
		{
			MotorClaw.stop();
		}
	}

	xSlow = Controller1.Axis4.position();
	xFast = Controller1.Axis1.position();
	ySlow = Controller1.Axis3.position();
	yFast = Controller1.Axis2.position();
	if(abs(ySlow) > driveMin)
	{
		ySlow /= slowFactor;
		MotorLeft.setVelocity(ySlow, percent);
		MotorRight.setVelocity(ySlow, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
	}
	else if(abs(xSlow) > driveMin)
	{
		xSlow /= slowFactor;
		MotorLeft.setVelocity(xSlow, percent);
		MotorRight.setVelocity(-xSlow, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
	}
	else if(abs(yFast) > driveMin)
	{
		MotorLeft.setVelocity(yFast, percent);
		MotorRight.setVelocity(yFast, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
	}
	else if(abs(xFast) > driveMin)
	{
		MotorLeft.setVelocity(xFast, percent);
		MotorRight.setVelocity(-xFast, percent);
		MotorLeft.spin(forward);
		MotorRight.spin(forward);
	}
	else
	{
		MotorLeft.stop();
		MotorRight.stop();
	}

	if(Controller1.ButtonA.pressing())
	{
		recIdx = 0;
		Brain.Screen.print("Recording");
		Brain.Screen.newLine();
		Brain.Timer.event(loop_record, loopTimeout);
		return;
	}

	Brain.Timer.event(loop_driver, loopTimeout);
}

void autonomous()
{
	loop_callback = loop_auton;
	modeDriver = false;
	Brain.Screen.print("Reading from File");
	Brain.Screen.newLine();
	std::ifstream ifs(FILENAME, std::fstream::in | std::fstream::binary);
	for(int i = 0; i < numReadings; ++i)
	{
		recLeft[i] = ifs.get();
		recRight[i] = ifs.get();
		recArm[i] = ifs.get();
		recClaw[i] = ifs.get();
	}

	ifs.close();
	Brain.Screen.print("Ready");
	Brain.Screen.newLine();
	recIdx = 0;
	init();
}

void drivercontrol()
{
	modeDriver = true;
	loop_callback = loop_driver;
	init();
}

int main()
{
	vexcodeInit();
	modeDriver = false;
	l1Pressed = false;
	Competition.autonomous(autonomous);
	Competition.drivercontrol(drivercontrol);
	for(;;)
	{
		wait(1000, msec);
	}
}

