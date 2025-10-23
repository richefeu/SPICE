#pragma once

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include <GL/freeglut.h>

#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>

#include "SPICE.hpp"

// toofus
#include "AABB.hpp"
#include "ColorTable.hpp"
#include "fileTool.hpp"
#include "glTools.hpp"
#include "mat4.hpp"

// the conf files
SPICE Conf;
int confNum = 1;

AABB worldBox;

int main_window;

// flags
int show_background  = 1; // not used
int show_particles   = 1;
int show_ghosts      = 0;
int show_period      = 1;
int show_forces      = 0;
int show_contacts    = 0;
int showOrientations = 0;

// particle coloring
#define COLOR_NONE 0
#define COLOR_RADIUS 1
#define COLOR_VELOCITY_MAGNITUDE 2
#define COLOR_PRESSURE 3
int particle_color_option{COLOR_NONE};
std::vector<double> color_values;
ColorTable colorTable;
void setColorOption(int option);     // this will compute the colors
void setColor(int i, GLfloat alpha); // this will set a color depending on the selected option
GLfloat alpha_particles = 1.0f;

// vectors 
double arrowSize        = 0.0005;
double arrowAngle       = 0.7;
double vScale           = 0.01;

double forceTubeFactor = 1.0;

// window
int width      = 800;
int height     = 800;
float wh_ratio = (float)width / (float)height;
glTextZone textZone(1, &width, &height);


// Miscellaneous global variables
enum MouseMode { NOTHING, ROTATION, ZOOM, PAN } mouse_mode = NOTHING;
int mouse_start[2];

void printInfo();

// Drawing functions
void drawForces();
void drawContacts();
void drawBox();
void drawParticles();
void drawGhosts();
void drawPeriod();
void drawFarSprings();

// Callback functions
void keyboard(unsigned char Key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void display();
void reshape(int x, int y);
void menu(int num);

// Helper functions
void buildMenu();
void printHelp();
void fit_view();
bool try_to_readConf(int num, SPICE &CF, int &OKNum);
void updateTextLine();
