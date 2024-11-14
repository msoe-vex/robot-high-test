#include "main.h" 
//Non Drive Motor Set up here (Naming each non-drive motor and declaring their port number).
//Pneumatic Set up here as well.
//Remember to define each motor in globals.hpp as well, otherwise code wont work.

//Subsystem Motors
pros::Motor MotorName(1);


//Pneumatics
//doing this instead of using ADIDigitalOut gives access to
//get_value(), making turning this into a 1 button toggle a lot easier
//pros::ADIPort PneumaticName('a', pros::E_ADI_DIGITAL_OUT);
pros::ADIPort pneumatic1('a', pros::E_ADI_DIGITAL_OUT);
pros::ADIPort pneumatic2('b', pros::E_ADI_DIGITAL_OUT);

pros::Motor motorTransfer1(17);
pros::Motor motorTransfer2(18);
pros::Motor motorIntake1(19);
//pros::Motor motorIntake2(20);
pros::Motor motorHood(20);