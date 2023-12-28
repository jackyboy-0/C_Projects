//-----------------------------------------------------------------------------
// a1.c
//
// This program takes user input for the flow, horizontal-distance and vertical-distance
// with this input it calculates thr grade and the friction-loss.
// With the parameters we have the program calculates the pressure and where the
// pumps will sit at. Finally, the pressure at the end will be displayed.
//
// Group: Markus_M_21
//
// Author: 11819675
//-----------------------------------------------------------------------------
//
#include <stdio.h>
#include <math.h>
#define EXIT_PRESSURE 10
// max pressure loss between 2 pumps
#define MAX_PRESSURE_LOSS 8.5
#define PRESSURE_LOSS 0.1

int inputOutput(int flow, int horizontal, int vertical);
double calculateGrade(int horizontal, int vertical);
double calculateFrictionLoss(int flow);
double calculatePumps(int flow, int horizontal, int vertical);
double calculatingPressure(int flow, int horizontal, int vertical, double change_in_y);

//-----------------------------------------------------------------------------
///
/// This is the main function, it runs the program
///
/// @param flow is declared and initialized to 0 for further use
/// @param horizontal_distance is declared and initialized to 0 for further use
/// @param vertical_distance is declared and initialized to 0 for further use
///
/// @return 0;
//
int main()
{
  printf("Welcomme to ESP\n");
  int flow = 0;
  int horizontal_distance = 0;
  int vertical_distance = 0;
  // calling the function and using the variables above as starting variables
  inputOutput(flow, horizontal_distance, vertical_distance);

  return 0;
}

//-----------------------------------------------------------------------------
/// This is the function where the input is taken and given back to the console
/// here is also the function calculate pumps called
///
/// @param flow can accept only integers between 100 and 1200.
/// @param horizontal is the distance in x direction, can accept only positive integers
/// @param vertical is the distance in y direction, can accept only positive integers
///
/// @return 0;
//
int inputOutput(int flow, int horizontal, int vertical)
{
  // do while loop for entering valid user input in the flow parameter
  do
  {
    printf("Erforderlicher Durchfluss [l/min]: ");
    scanf("%d", &flow);

    if(flow <= 99 || flow >= 1200)
    {
      printf("Invalide Eingabe! Der Durchfluss muss mindestens 100 l/min und maximal 1200 l/min betragen.\n");
    }
  }while (flow <= 99 || flow >= 1200);

  // do while loop for entering valid user input in the horizontal direction
  do
  {
    printf("Horizontale Distanz [m]: ");
    scanf("%d", &horizontal);

    if (horizontal <= 0)
    {
      printf("Invalide Eingabe!\n");
    }
  } while (horizontal <= 0);

  // do while loop for entering valid user input in the vertical direction
  do
  {
    printf("Vertikale Distanz [m]: ");
    scanf("%d", &vertical);

    if (vertical <= 0)
    {
      printf("Invalide Eingabe!\n");
    }
  } while (vertical <= 0);

  printf("\n");
  printf("Ziel: (%d, %d)\n", horizontal, vertical);
  printf("Neigung [rad]: %.4lf\n", calculateGrade(horizontal, vertical));
  printf("Durchfluss [l/min]: %d\n", flow);
  printf("Reibungsbeiwert [bar/m]: %.4lf\n", calculateFrictionLoss(flow));
  // calling the calculatePumps function
  calculatePumps(flow, horizontal, vertical);
  return 0;
}

//-----------------------------------------------------------------------------
/// This function takes two integers and calculates the grade with the help
/// of the math.h library ant the atan() function
///
/// @param result  is the grade in [rad]witch will bw returned.
/// @param horizontal x direction used for Grade calculation
/// @param vertical y direction user for Grade Calculation
///
/// @return double result = grade.
//
double calculateGrade(int horizontal, int vertical)
{
  double result = 0;
  // calculation for the grade
  result = atan(vertical / (double)horizontal);

  return result;
}

//-----------------------------------------------------------------------------
/// This function takes a integer and calculates the friction- loss
/// the friction los depends on the flow
///
/// @param friction_loss is checked for against the given values
/// @param flow (Durchflussmenge) 
///
/// @return double friction_loss
//
double calculateFrictionLoss(int flow)
{
  double friction_loss = 0;
  // the friction depends on the flow
  if (flow <= 200)
  {
    friction_loss = 0.001;
  }
  else if (flow <= 400)
  {
    friction_loss = 0.0025;
  }
  else if (flow <= 600)
  {
    friction_loss = 0.005;
  }
  else if (flow <= 800)
  {
    friction_loss = 0.01;
  }
  else if (flow <= 1000)
  {
    friction_loss = 0.015;
  }
  else if (flow <= 1200)
  {
    friction_loss = 0.025;
  }
  return friction_loss;
}

//-----------------------------------------------------------------------------
/// Function for calculating the pressure for the pumps
///
/// @param pressure is declared and calculates the pressure.
///
/// @return double pressure;
//
double calculatingPressure(int flow, int horizontal, int vertical, double change_in_y)
{
  double pressure = 0;
  pressure = EXIT_PRESSURE - (PRESSURE_LOSS * change_in_y + calculateFrictionLoss(flow) *
        (change_in_y / sin(calculateGrade(horizontal, vertical))));

  return pressure;
}

//-----------------------------------------------------------------------------
/// This is a function for the pumps and pressure calculations.
/// by using the given formulas we have the following parameters.
///
/// @param pump_counter used for the enumeration of pumps
/// @param delta_x parameter for calculating the placement of the pump in x direction
/// @param delta_y parameter for calculating the placement of the pump in y direction
/// @param pressure used for calculating the pressure at a given (x, y) location
/// @param x_pump_at_current_location coordinates for te pump
/// @param y_pump_at_current_location coordinates for te pump
/// @param new_y is used to determine thr new pressure at the next pump location
///
/// @return 0;
//
double calculatePumps(int flow, int horizontal, int vertical)
{
  int pump_counter = 0;
  double delta_y = 0;
  double delta_x = 0;
  double y_pump_at_current_location = 0;
  double x_pump_at_current_location = 0;
  double new_y = 0;
  double pressure = 0;

  // new_y is the parameter that changes with every loop start
  new_y = vertical;
  pressure = calculatingPressure(flow, horizontal, vertical, new_y);
  delta_y = (MAX_PRESSURE_LOSS / (PRESSURE_LOSS + (calculateFrictionLoss(flow) / 
            sin(calculateGrade(horizontal, vertical)))));
  delta_x = (delta_y / tan(calculateGrade(horizontal, vertical)));

  // the following if statement is for checking if the pressure is above 6 [bar]
  // when the pressure is ober 6 we don't need additional pumps
  if (pressure > 6)
  {
    printf("  Keine zusaetzliche Pumpe notwendig!\n");
    printf("Austrittsdruck Zielpunkt [bar]: %.2lf\n", pressure);
  }
  // the while loop calculates the pressure again and if needed prints the Pumps at the corresponding location
  while (pressure < 6)
  {
    new_y = vertical - y_pump_at_current_location;
    pressure = calculatingPressure(flow, horizontal, vertical, new_y);
    // the y_pump_at_current_location is updated for every pass of the while loop
    y_pump_at_current_location += delta_y;
    x_pump_at_current_location += delta_x;
    // here the y_pump_at_current_location is checked if it is over the desired point
    if(y_pump_at_current_location > vertical)
    {
      y_pump_at_current_location = vertical;
    }
    if (x_pump_at_current_location > horizontal)
    {
      x_pump_at_current_location = horizontal;
    }
    // here is checked again if the pressure is over 6 [bar]
    // and printed the current pump location
    if(pressure < 6)
    {
      pump_counter++;
      printf("  Pumpe%d: (%.2lf, %.2lf)\n", pump_counter, x_pump_at_current_location, y_pump_at_current_location);
    }
    else
    {
      printf("Austrittsdruck Zielpunkt [bar]: %.2lf\n", pressure);
      break;
    }
  }
  return 0;
}
