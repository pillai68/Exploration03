#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHSD.h>

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P0_0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void check_x_minus(float x_coordinate) //using RPS while robot is in the -x direction
{
    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.X() > 0 && (RPS.X() < (x_coordinate - 1) || RPS.X() > (x_coordinate + 1)))
    {
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            move_forward(15, 20.25);
            Sleep(0.5);

        }
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            move_forward(-15, 20.25);
            Sleep(0.5);
        }
    }
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(RPS.Y() > 0 && (RPS.Y() < (y_coordinate - 1) || RPS.Y() > (y_coordinate + 1)))
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            move_forward(15, 20.25);
            Sleep(0.5);

        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            move_forward(-15, 20.25);
            Sleep(0.5);
        }
    }
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while( RPS.Y() >0 && (RPS.Y() < (y_coordinate - 1) || RPS.Y() > (y_coordinate + 1)))
    {
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            move_forward(-15, 20.25);
            Sleep(0.5);
        }
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction

            move_forward(15, 20.25);
            Sleep(0.5);
        }
    }
}

void check_heading(float heading) //using RPS
{

    while( RPS.Heading() < (heading -4) || RPS.Heading() > (heading +4))
    {
        if(RPS.Heading() > heading ) {
            turn_left(-15, 6);
            Sleep(0.5);
            LCD.WriteLine("checking heading");
            LCD.Clear();
        }else if(RPS.Heading() < heading){
            turn_left(15, 6);
            Sleep(0.5);
            LCD.WriteLine("checking heading");
            LCD.Clear();
        }
    }
}

int main(void)
{
    float touch_x,touch_y;
    float A_x = 28.00, A_y = 50.300, B_x = 29.50, B_y=61.099, C_x = -2.00, C_y = -2.00, D_x = 9.8, D_y = 49.5;
    float A_heading, B_heading, C_heading, D_heading;
    int power, B_C_counts, C_D_counts, turn_90_counts;

    RPS.InitializeTouchMenu();

    LCD.WriteLine("RPS & Data Logging Test");
    LCD.WriteLine("Press Screen To Start");
    while(!LCD.Touch(&touch_x,&touch_y));
    while(LCD.Touch(&touch_x,&touch_y));

    //Declare example variables
    //Open input log file
    /*FEHFile*ifptr=SD.FOpen("RPS_Test.txt","r");//Scan data using formatted string
    SD.FScanf(ifptr,"%f%f%f%f", &A_x,&A_y,&B_x, &B_y, &C_x, &C_y, &D_x, &D_y);
    //Close input log file
    SD.FClose(ifptr); */

    //WRITE CODE HERE TO SET THE HEADING DEGREES, MOTOR POWER, AND COUNTS VALUES
    A_heading = 90;
    B_heading = 180;
    C_heading = 270;
    D_heading = 0;

    power = 25;

    B_C_counts = 40.5*16;
    C_D_counts = 40.5*10;

    turn_90_counts = 40.5*6;

    //A --> B
    check_y_plus(B_y);
    check_heading(B_heading);

    //B --> C
    move_forward(power, B_C_counts);
    //check_x_minus(C_x);
    turn_left(power, turn_90_counts);
    //check_heading(C_heading);

    //C --> D
    move_forward(power, C_D_counts);
    check_y_minus(D_y);
    turn_left(power, turn_90_counts);
    check_heading(D_heading);

    return 0;
}
