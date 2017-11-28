/*

    This program implements the optimum velocity model for traffic flow
    originally suggested by Bando et al. [1,2]. The traffic flows around
    a circle.
    
    The optimum velocity model is an ordinary differential equation 
    (ODE) of the form:
    
    dx_i/dt = v_i
    dv_i/dt = a*(V(x_i+1 - x_i) - v_i)
    
    where the subscripts i, i+1 denote the different cars and V is the 
    optimal velocity function:
    
    V(h) = tanh(h - 2) + tanh(2).
    
    The idea is that there is an optimum velocity for your car based on
    the distance to the car in front and you should accelerate/
    decelerate to reach this optimum velocity. Here the set of ODEs is
    integrated using an explicit Euler scheme.
    
    [1] M. Bando, K. Hasebe, A. Nakayama, A. Shibata, and Y. Sugiyama, 
            Jpn. J. Ind. Appl. Math. 11, 203 (1994).
    [2] M. Bando, K. Hasebe, A. Nakayama, A. Shibata, and Y. Sugiyama, 
            Phys. Rev. E 51, 1035 (1995). 

*/

// Include necessary header files
#include "SPG/sprig.h" /* For graphics */
#include <cmath>       /* For mathematics */

// Define the screen size
#define SCREENWIDTH 640
#define SCREENHEIGHT 480

// Define the number of cars: see difference in behaviour between 20 and
// 25 cars...
#define NCARS 25

// Define Pi
#define PI 3.141592653589793

double optimumVelocity(double h)
{
    return tanh(10*h - 2) + tanh(2);
}

class Car
{
protected:	// Overload the impact function of Ball

	Uint32 colour;
	SDL_Surface *screen;
	int lasttime;
virtual void draw(){};
public:
	double theta;
	double dtheta;
// A constructor for ChangeBall that just uses the Ball constructor
Car(double newtheta, double newdtheta, Uint32 newcolour, SDL_Surface *newscreen);
//void Car::drawCar(SDL_Surface *screen, double theta)
// Empty destructor
virtual ~Car(){};
virtual void update(double a, double Pos, double CarNo, double hmult);
};

void Car::update(double a, double Pos, double CarNo, double hmult ){

	double CarVel;
	double newtime = SDL_GetTicks();
	    	if ((newtime - lasttime) > 50)
	    		lasttime = newtime - 50;
	    	double h = 0.5*hmult*(newtime - lasttime)/1000; // Convert into seconds (the 0.5 is an arbitrary scaling to make it look nicer)
	    	lasttime = newtime;

	    	// Clear the screen
	    	        SPG_RectFilled(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT, 0x000000);

	    	        // Draw the road
	    	        //drawRoad(screen);
	    	        // Draw each of the cars
	    	        //for (int i = 0; i < NCARS; i++)
	    	        //drawCar(screen, cars[i]);

	    	        // Update the velocities of each car and calculate the average speed
	    	        for (int i = 0; i < NCARS; i++) {
	    	            // Decide which car is in front
	    	            int carinfront = i + 1;
	    	            if (carinfront == NCARS)
	    	                carinfront = 0;
	    	            // Calculate the distance (in angle) to the car in front
	    	            double headway = (Pos - theta);
	    	            // Account for periodicity in the angle
	    	            if (carinfront == 0)
	    	                headway += 2*PI;
	    	            // Update the velocity according to the optimum velocity rule
	    	            CarVel += h*a*(optimumVelocity(headway) - dtheta);
	    	        }

	    	        // Update the positions of each car
	    	        for (int i = 0; i < NCARS; i++)
	    	            Pos += h*dtheta;

	    			// Update the actual screen
	    			SDL_UpdateRect(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);


}

Car::Car(double newtheta, double newdtheta, Uint32 newcolour, SDL_Surface *newscreen)
{
	theta = newtheta;
	dtheta = newdtheta;
	colour = newcolour;
	screen = newscreen;
	lasttime = SDL_GetTicks();
}
/*
// A car structure
typedef struct {
    double theta;    // Position in polar coordinates
    double dtheta;   // Velocity in polar coordinates (dr = 0)
} Car;
*/
// drawRoad: Draw a simple circular road at the center of the screen
void drawRoad(SDL_Surface *screen)
{
    // Draw the outer circle with an anti-aliased (smooth looking) white circle
    SPG_Circle(screen, SCREENWIDTH/2, SCREENHEIGHT/2, SCREENHEIGHT/2 - 10, 0xFFFFFF);
    // Draw the inner circle with an anti-aliased (smooth looking) white circle
    SPG_Circle(screen, SCREENWIDTH/2, SCREENHEIGHT/2, SCREENHEIGHT/2 - 30, 0xFFFFFF);
    // For hexadecimal colours just do a web search for "hex color generator"
}


// drawCar: Draw a car on the road
void Car::drawCar(SDL_Surface *screen, double theta)
{
    // Work out the (x, y) coordinates of the car
    double x = SCREENWIDTH/2 + (SCREENHEIGHT/2 - 20)*cos(car.theta);
    double y = SCREENHEIGHT/2 + (SCREENHEIGHT/2 - 20)*sin(car.theta);
    // Draw a circle on the road where the car is
    SPG_CircleFilled(screen, (int)x, (int)y, 8, 0xFF00FF);
}

// optimumVelocity: return the optimum velocity for a given headway

// ********************************************************************************
// Entry point
extern "C"
int main(int argc, char *argv[])
{
    // Run variable
    int run = 1;

    // The last time the screen was updated
    int lasttime;

    // Variable for the screen
    SDL_Surface *screen;

    // Variable for getting keyboard events
    SDL_Event event;

    // Initialise the graphics sub-system
	SDL_Init(SDL_INIT_VIDEO);

	/* Set the size of the window to be SCREEN_SIZExSCREEN_SIZE with 32 bits per pixel (colour) */
	screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 32, SDL_SWSURFACE);
    
    // The cars
    Car *cars[NCARS];
    
    // The acceleration multiplier for the cars
    double a = 10.0;

    // The time multiplier for the cars (for speeding-up/slowing-down the simulation)
    double hmult = 1.0;
    

    // Initialise the car positions: equispace them

    for (int i = 0; i < NCARS; i++) {
        cars[i] = new Car(2.0*PI*i/(NCARS + 1), 0,0x099876, screen);

    }
    lasttime = SDL_GetTicks();

    // Main loop: run until escape is pressed or the window is closed
    while (run) {
    	/*
    	// Work out how long ago the last update was
    	double newtime = SDL_GetTicks();
    	if ((newtime - lasttime) > 50)
    		lasttime = newtime - 50;
    	double h = 0.5*hmult*(newtime - lasttime)/1000; // Convert into seconds (the 0.5 is an arbitrary scaling to make it look nicer)
    	lasttime = newtime;
		*/
        // Clear the screen
        SPG_RectFilled(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT, 0x000000);

        // Draw the road
        drawRoad(screen);

        // Draw each of the cars
        for (int i = 0; i < NCARS; i++)
          drawCar(screen, *(cars[i]));
        printf("here\n");
        // Update the velocities of each car and calculate the average speed
        for (int i = 0; i < NCARS; i++) {
            // Decide which car is in front
            int carinfront = i + 1;
            if (carinfront == NCARS)
                carinfront = 0;
            // Calculate the distance (in angle) to the car in front
            //double headway = (cars[carinfront].theta - cars[i].theta);
            // Account for periodicity in the angle
            //if (carinfront == 0)
            //   headway += 2*PI;
            // Update the velocity according to the optimum velocity rule
            cars[i]->update(10.0, cars[carinfront]->theta , (double)i, (double)1 );

        }

        // Update the positions of each car
       // for (int i = 0; i < NCARS; i++)
       //     cars[i].theta += h*cars[i].dtheta;
            
		// Update the actual screen
		SDL_UpdateRect(screen, 0, 0, SCREENWIDTH, SCREENHEIGHT);

		// Poll for events, and handle the ones we care about.
        while (SDL_PollEvent(&event)) {

            switch (event.type) {
            case SDL_KEYDOWN:
                // Do nothing when key is pressed down
                break;
            case SDL_KEYUP:
                // Do something when key is released
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    // If escape is pressed then quit
                    run = 0;
                    break;
                case SDLK_LEFT:
                    // Left arrow key pressed -> slow down simulation
                    hmult /= 1.5;
                    break;
                case SDLK_RIGHT:
                    // Right arrow key pressed -> speed up simulation (make sure it doesn't go too fast)
                	if (hmult < 10.0)
                		hmult *= 1.5;
                    break;
                case SDLK_0:
                    // Zero key pressed -> reset simulation speed
                    hmult = 1.0;
                    break;
                default:
                    // Do nothing
                    break;
                }
                break;
            case SDL_QUIT:
                // Someone closed the window so exit nicely
                run = 0;
            }
        }
        
    }
    
    // Quit
    return 0;
}
