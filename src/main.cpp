#include "main.h"
#include "EZ-Template/util.hpp"
#include "pros/adi.hpp"
#include "encthing.cpp"
#include "pros/misc.h"

/////
// For installation, upgrading, documentations and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is used as the sensor
  {-9, -12, -13}

  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is used as the sensor
  ,{14, 15, 16}

  // IMU Port
  ,16
  
  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
  ,3.25

  // Cartridge RPM
  ,200

  // External Gear Ratio (MUST BE DECIMAL) This is WHEEL GEAR / MOTOR GEAR
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 84/36 which is 2.333
  // eg. if your drive is 60:36 where the 36t is powered, your RATIO would be 60/36 which is 0.6
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 36/60 which is 0.6
  ,1
);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();
  
  pros::delay(500); // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0); // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(8, 10); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
  default_constants(); // Set the drive to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Autonomous Selector using LLEMU
  ez::as::auton_selector.autons_add({
    Auton("Competition Auton", competition_auton),
  });

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");
  pneumatic1.set_value(false);
  pneumatic2.set_value(false);

  // pros::delay(50);
  // pros::Task imuValues(getIMU);

}



/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}



/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}



/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  chassis.pid_targets_reset(); // Resets PID targets to 0
  chassis.drive_imu_reset(); // Reset gyro position to 0
  chassis.drive_sensor_reset(); // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency

  ez::as::auton_selector.selected_auton_call(); // Calls selected auton from autonomous selector
  
}



/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

/*
void getIMU() {
  while (true) {
    master.print(0,0, "imu:%.2f",chassis.drive_imu_get());
  }
}
*/

void swap(void* valueSwap) {
  pros::delay(1000);
  *(bool*)valueSwap = true;
}

void opcontrol() {
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  master.clear_line(0);
  pros::ADIEncoder lol('C', 'd', false);
  intakeDeploy.set_value(false);
  //OdomThing odom('A', 'B', false);
  //pros::ADIEncoder* enc = &odom;
  bool lastLimitState = true;
  bool driveState = true;
  while (true) {
    // (0,0, "imu:%.2f",chassis.drive_imu_get());
    
    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    // if (!pros::competition::is_connected()) { 
    //   // Enable / Disable PID Tuner
    //   //  When enabled: 
    //   //  * use A and Y to increment / decrement the constants
    //   //  * use the arrow keys to navigate the constants
    //   if (master.get_digital_new_press(DIGITAL_X)) 
    //     chassis.pid_tuner_toggle();
        
    //   // Trigger the selected autonomous routine
    /*
       if (master.get_digital_new_press(DIGITAL_B)) {
          autonomous();
       }
       */

    //   chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
    // } 

    if(limitSwitch.get_new_press()) {
      driveState = false;
      pros::Task my_task (swap, (void*)&driveState, TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "My Task");
    }



    if(master.get_digital(DIGITAL_L1)) {   
      motorIntake1.move(-127);
      //motorIntake2.move(127);
      motorTransfer1.move(127);
      motorTransfer2.move(127);
      if(driveState) {
        motorHood.move(127);
      } else {
        motorHood.move(-127);
      }
    } else if(master.get_digital(DIGITAL_L2)) {
      motorTransfer1.move(-127);
      motorTransfer2.move(-127);
      motorHood.move(-127);
    } else if(master.get_digital(DIGITAL_R2)) {
      motorIntake1.move(127);
      //motorIntake2.move(-127);
    } else {
      motorIntake1.move(0);
      //motorIntake2.move(0);
      motorTransfer1.move(0);
      motorTransfer2.move(0);
      motorHood.move(0);
    }

    if(master.get_digital_new_press(DIGITAL_B)) {
      climb.set_value(false);
    } else if(master.get_digital_new_press(DIGITAL_X)) {
      climb.set_value(true);
    }

    if(master.get_digital_new_press(DIGITAL_R1)) {
      if(pneumatic2.get_value() == true) {
        pros::Task task{[=] {
          pneumatic2.set_value(false);
          pros::delay(500);
          pneumatic1.set_value(false);
        }};
      } else {
        pros::Task task{[=] {
          pneumatic1.set_value(true);
          pros::delay(500);
          pneumatic2.set_value(true);
        }};
      }
    }

    if(master.get_digital_new_press(DIGITAL_UP)) {
      intakeDeploy.set_value(true);
    }

    if(master.get_digital_new_press(DIGITAL_DOWN)) {
      intakeDeploy.set_value(false);
    }

    
    //std::cout << lol.get_value() << std::endl;
    //OdomThing odom('A', 'B', false);
    //pros::ADIEncoder* enc = &odom;

    //std::cout << enc->get_value() << std::endl;

    /*
    FILE* file = fopen("/usd/garbage.txt", "r");
    char buf[50];
    fread(buf, 1, 50, file);
    master.set_text(0, 1, buf);
    */

    //chassis.opcontrol_tank(); // Tank control
    chassis.opcontrol_arcade_standard(ez::SPLIT); // Standard split arcade
    // chassis.opcontrol_arcade_standard(ez::SINGLE); // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT); // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE); // Flipped single arcade

    // . . .
    // Put more user control code here!
    // . . .




    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
