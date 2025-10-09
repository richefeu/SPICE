#include "see.hpp"

#include <cmath>
#include <vector>

void printHelp() {
  std::cout << std::endl;
  std::cout << "+         load next configuration file" << std::endl;
  std::cout << "-         load previous configuration file" << std::endl;
  std::cout << "=         fit the view" << std::endl;
  std::cout << "a/A       particle transparency" << std::endl;
  std::cout << "b/B       ghost particle transparency" << std::endl;
  std::cout << "c         show/hide periodic cell" << std::endl;
  std::cout << "C         show/hide contacts" << std::endl;
  std::cout << "f         show/hide normal forces" << std::endl;
  std::cout << "g         show/hide ghost particles" << std::endl;
  std::cout << "h         print this help" << std::endl;
  std::cout << "i         print information" << std::endl;
  std::cout << "m         show/hide polar plot" << std::endl;
  std::cout << "n         go to file (see terminal to enter the file number)" << std::endl;
  std::cout << "o         show/hide particle orientations" << std::endl;
  std::cout << "p         show/hide particles" << std::endl;
  std::cout << "P         switch pipe display" << std::endl;
  std::cout << "q         quit" << std::endl;
  std::cout << "s/S       tune vector sizes" << std::endl;
  std::cout << "w/W       tune displayed ghost width" << std::endl;
  std::cout << "x         show/hide surrouding material 'spider-map'" << std::endl;
  // std::cout << "x         xxxx" << std::endl;
  std::cout << std::endl;
  std::cout << "0         particles colored with light gray" << std::endl;
  std::cout << "1         particles colored with pressure" << std::endl;
  std::cout << std::endl;
}

void printInfo() {
  int V = glutGet(GLUT_VERSION);
  int V1 = V / 10000;
  int V2 = (V - V1 * 10000) / 100;
  int V3 = V - V1 * 10000 - V2 * 100;
  std::cout << "glut version " << V1 << "." << V2 << "." << V3 << "\n";
}

void keyboard(unsigned char Key, int /*x*/, int /*y*/) {
  switch (Key) {

    /*
  case '0': {
    color_option = 0;
  } break;

  case '1': { // particle pressures
    double pmin = pdata[0].p;
    double pmax = pdata[0].p;
    for (size_t i = 1; i < pdata.size(); i++) {
      if (pdata[i].p < pmin) {
        pmin = pdata[i].p;
      }
      if (pdata[i].p > pmax) {
        pmax = pdata[i].p;
      }
    }
    colorTable.setMinMax(pmin, pmax);
    colorTable.setTableID(2);
    colorTable.Rebuild();
    color_option = 1;
  } break;

  case '2': {
    // colorTable.setMinMax(pmin, pmax);
    colorTable.setTableID(2);
    colorTable.Rebuild();
    color_option = 2;
  } break;
  */

  case 'a': {
    alpha_particles = std::max(0.0f, alpha_particles - 0.05f);
  } break;
  case 'A': {
    alpha_particles = std::min(1.0f, alpha_particles + 0.05f);
  } break;

    /*
    case 'b': {
      alpha_ghosts = std::max(0.0f, alpha_ghosts - 0.05f);
    } break;
    case 'B': {
      alpha_ghosts = std::min(1.0f, alpha_ghosts + 0.05f);
    } break;
    */

    /*
    case 'c': {
      show_cell = 1 - show_cell;
    } break;
    */

  case 'C': {
    show_contacts = 1 - show_contacts;
  } break;

  case 'f': {
    show_forces = 1 - show_forces;
  } break;

    /*
    case 'g': {
      show_ghosts = 1 - show_ghosts;
    } break;
  */

  case 'h': {
    printHelp();
  } break;

  case 'i': {
    printInfo();
  } break;

  case 'n': {
    std::cout << "Go to file number ";
    int conNumTry;
    std::cin >> conNumTry;
    try_to_readConf(conNumTry, Conf, confNum);
  } break;

  case 'o': {
    showOrientations = 1 - showOrientations;
  } break;

  case 'p': {
    show_particles = 1 - show_particles;
  } break;

  case 'q': {
    exit(0);
  } break;

  case 'S': {
    vScale *= 1.05;
  } break;

  case 's': {
    vScale *= 0.95;
    if (vScale < 0.0)
      vScale = 1.0;
  } break;

  case '-': {
    if (confNum > 0) {
      try_to_readConf(confNum - 1, Conf, confNum);
    }
    updateTextLine();
  } break;

  case '+': {
    try_to_readConf(confNum + 1, Conf, confNum);
    updateTextLine();
  } break;

  case '=': {
    fit_view();
    reshape(width, height);
  } break;
  };

  glutPostRedisplay();
}

void updateTextLine() { textZone.addLine("conf %d,  t %0.4g s", confNum, Conf.t); }

void mouse(int button, int state, int x, int y) {

  if (state == GLUT_UP) {
    mouse_mode = NOTHING;
    // display();
    glutPostRedisplay();
  } else if (state == GLUT_DOWN) {
    mouse_start[0] = x;
    mouse_start[1] = y;
    switch (button) {
    case GLUT_LEFT_BUTTON: {
      if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        mouse_mode = PAN;
      } else {
        mouse_mode = ROTATION;
      }
    } break;
    case GLUT_MIDDLE_BUTTON: {
      mouse_mode = ZOOM;
    } break;
    }
  }
}

void motion(int x, int y) {

  if (mouse_mode == NOTHING) {
    return;
  }

  double dx = (double)(x - mouse_start[0]) / (double)width;
  double dy = (double)(y - mouse_start[1]) / (double)height;

  switch (mouse_mode) {

  case ZOOM: {
    double ddy = (worldBox.max.y - worldBox.min.y) * dy;
    double ddx = (worldBox.max.x - worldBox.min.x) * dy;
    worldBox.min.x -= ddx;
    worldBox.max.x += ddx;
    worldBox.min.y -= ddy;
    worldBox.max.y += ddy;
  } break;

  case PAN: {
    double ddx = (worldBox.max.x - worldBox.min.x) * dx;
    double ddy = (worldBox.max.y - worldBox.min.y) * dy;
    worldBox.min.x -= ddx;
    worldBox.max.x -= ddx;
    worldBox.min.y += ddy;
    worldBox.max.y += ddy;
  } break;

  default:
    break;
  }
  mouse_start[0] = x;
  mouse_start[1] = y;

  reshape(width, height);
  // display();
  glutPostRedisplay();
}

void display() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (show_period == 1) {
    drawPeriod();
  }
  
 

  if (show_particles == 1) {
    drawParticles();
  }
  
  if (1) {
  	drawFarSprings();
  }

  /*
  if (show_contacts == 1) {
    drawContacts();
  }

  if (show_forces == 1) {
    drawForces();
  }
  */

  textZone.draw();

  glFlush();
  glutSwapBuffers();
}

void fit_view() {
  //
  // 3 x ------- x 2
  //   |         |
  // 0 x ------- x 1
  /*
  double x0 = 0.0;
  double y0 = 0.0;
  double x1 = Conf.Cell.h.xy;
  double y1 = Conf.Cell.h.yy;
  double x2 = Conf.Cell.h.xy + Conf.Cell.h.xx;
  double y2 = Conf.Cell.h.yy + Conf.Cell.h.yx;
  double x3 = Conf.Cell.h.xx;
  double y3 = Conf.Cell.h.yx;
  worldBox.min.x = std::min(std::min(std::min(x0, x1), x2), x3);
  worldBox.min.y = std::min(std::min(std::min(y0, y1), y2), y3);
  worldBox.max.x = std::max(std::max(std::max(x0, x1), x2), x3);
  worldBox.max.y = std::max(std::max(std::max(y0, y1), y2), y3);
  */

  // TODO: woldBox fit (use the Boundaries also ?)
  for (size_t i = 0; i < Conf.Particles.size(); ++i) {

    vec2r pos = Conf.Particles[i].pos;
    double R = Conf.Particles[i].radius;

    double xmin = pos.x - R;
    double xmax = pos.x + R;
    double ymin = pos.y - R;
    double ymax = pos.y + R;
    worldBox.min.x = std::min(worldBox.min.x, xmin);
    worldBox.min.y = std::min(worldBox.min.y, ymin);
    worldBox.max.x = std::max(worldBox.max.x, xmax);
    worldBox.max.y = std::max(worldBox.max.y, ymax);
  }

  reshape(width, height);
}

void reshape(int w, int h) {
  width = w;
  height = h;

  double left = worldBox.min.x;
  double right = worldBox.max.x;
  double bottom = worldBox.min.y;
  double top = worldBox.max.y;
  double worldW = right - left;
  double worldH = top - bottom;
  double dW = 0.1 * worldW;
  double dH = 0.1 * worldH;
  left -= dW;
  right += dW;
  top += dH;
  bottom -= dH;
  worldW = right - left;
  worldH = top - bottom;

  if (worldW > worldH) {
    worldH = worldW * ((GLfloat)height / (GLfloat)width);
    top = 0.5 * (bottom + top + worldH);
    bottom = top - worldH;
  } else {
    worldW = worldH * ((GLfloat)width / (GLfloat)height);
    right = 0.5 * (left + right + worldW);
    left = right - worldW;
  }

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(left, right, bottom, top);

  glutPostRedisplay();
}

void setColor(int /*i*/, GLfloat alpha) {
  switch (color_option) {

  case 0: {
    glColor4f(0.8f, 0.8f, 0.9f, alpha);
  } break;

  case 1: { // pressure
    // colorRGBA col;
    // colorTable.getRGB(pdata[i].p, &col);
    // glColor4f(col.r / 255.0, col.g / 255.0, col.b / 255.0, 1.0f);
  } break;

  case 2: {
    /*
    colorRGBA col;
    colorTable.getRGB(Conf.grains[i].zncc2, &col);
    glColor4f(col.r / 255.0, col.g / 255.0, col.b / 255.0, 1.0f);
    */
  } break;

  default: {
    glColor4f(0.8f, 0.8f, 0.9f, alpha);
  } break;
  }
}

void drawPeriod() {
  glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex2f(Conf.xmin, Conf.ymin);
  glVertex2f(Conf.xmin, Conf.ymax);
  glVertex2f(Conf.xmax, Conf.ymin);
  glVertex2f(Conf.xmax, Conf.ymax);
  glEnd();
}

void drawFarSprings() {
	
	//std::cout << Conf.bottom.Idx.size() << std::endl;
	
  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
  glLineWidth(1.0f);
  size_t idx;
  vec2r pos;
  double l;
  glBegin(GL_LINES);
  for (size_t i = 0; i < Conf.bottom.Idx.size(); ++i) {
    idx = Conf.bottom.Idx[i];
    pos = Conf.bottom.pos[i];
    l = 0.25 * Conf.Particles[idx].radius;
    glVertex2f(pos.x - l, pos.y - l);
    glVertex2f(pos.x + l, pos.y + l);
    glVertex2f(pos.x - l, pos.y + l);
    glVertex2f(pos.x + l, pos.y - l);

    glVertex2f(pos.x, pos.y);
    glVertex2f(Conf.Particles[idx].pos.x, Conf.Particles[idx].pos.y);
  }
  
  for (size_t i = 0; i < Conf.top.Idx.size(); ++i) {
    idx = Conf.top.Idx[i];
    pos = Conf.top.pos[i];
    l = 0.25 * Conf.Particles[idx].radius;
    glVertex2f(pos.x - l, pos.y - l);
    glVertex2f(pos.x + l, pos.y + l);
    glVertex2f(pos.x - l, pos.y + l);
    glVertex2f(pos.x + l, pos.y - l);

    glVertex2f(pos.x, pos.y);
    glVertex2f(Conf.Particles[idx].pos.x, Conf.Particles[idx].pos.y);
  }
  glEnd();
}

void drawParticles() {
  if (mouse_mode != NOTHING) {
    return;
  }

  glLineWidth(1.0f);

  for (size_t i = 0; i < Conf.Particles.size(); ++i) {
    vec2r pos = Conf.Particles[i].pos;
    double R = Conf.Particles[i].radius;

    setColor(i, alpha_particles);
    glBegin(GL_POLYGON);
    for (double angle = 0.0; angle < 2.0 * M_PI; angle += 0.05 * M_PI) {
      glVertex2f(pos.x + R * cos(angle), pos.y + R * sin(angle));
    }
    glEnd();

    glColor4f(0.0f, 0.0f, 0.0f, alpha_particles);
    glBegin(GL_LINE_LOOP);
    for (double angle = 0.0; angle < 2.0 * M_PI; angle += 0.05 * M_PI) {
      glVertex2f(pos.x + R * cos(angle), pos.y + R * sin(angle));
    }
    glEnd();

    if (showOrientations) {
      double rot = Conf.Particles[i].rot;
      glBegin(GL_LINES);
      glVertex2f(pos.x, pos.y);
      glVertex2f(pos.x + R * cos(rot), pos.y + R * sin(rot));
      glEnd();
    }
  }
}

void drawContacts() {
  if (mouse_mode != NOTHING) {
    return;
  }

  glLineWidth(1.5f);

  // grain-grain
  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  for (size_t k = 0; k < Conf.Interactions.size(); ++k) {
    size_t i = Conf.Interactions[k].i;
    size_t j = Conf.Interactions[k].j;
    vec2r posi = /*Conf.Cell.h * */ Conf.Particles[i].pos;
    vec2r sij = Conf.Particles[j].pos - Conf.Particles[i].pos;
    vec2r posj = posi + sij;
    glVertex2f(posi.x, posi.y);
    glVertex2f(posj.x, posj.y);
  }
  glEnd();
}

// remove periodic things
void drawForces() {
  if (mouse_mode != NOTHING) {
    return;
  }

  // grain-grain
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

  for (size_t k = 0; k < Conf.Interactions.size(); ++k) {
    size_t i = Conf.Interactions[k].i;
    size_t j = Conf.Interactions[k].j;
    vec2r posi = Conf.Particles[i].pos;
    vec2r sij = Conf.Particles[j].pos - Conf.Particles[i].pos;
    vec2r posj = posi + sij;

    // Calculate the width of the rectangle
    // GLfloat width = Conf.radiusMean * (Conf.Interactions[k].fn / Conf.fnMax);
    GLfloat width = 0.001;

    // Calculate the direction vector and the perpendicular vector
    vec2r dir = posj - posi;
    vec2r perp(-dir.y, dir.x);
    perp.normalize();
    perp *= 0.5 * width;

    // Calculate the four corners of the rectangle
    vec2r p1 = posi + perp;
    vec2r p2 = posi - perp;
    vec2r p3 = posj - perp;
    vec2r p4 = posj + perp;

    // Draw the filled rectangle
    glBegin(GL_QUADS);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glVertex2f(p4.x, p4.y);
    glEnd();
  }
}

bool try_to_readConf(int num, SPICE &CF, int &OKNum) {
  char file_name[256];
  snprintf(file_name, 256, "conf%d", num);
  if (fileTool::fileExists(file_name)) {
    std::cout << file_name << std::endl;
    OKNum = num;
    CF.loadConf(file_name);
    CF.accelerations();
    return true;
  } else {
    std::cout << file_name << " does not exist" << std::endl;
  }
  return false;
}

void menu(int num) {
  switch (num) {

  case 0: {
    exit(0);
  } break;

    // case xx: {} break;
  };

  glutPostRedisplay();
}

void buildMenu() {
  /*
  int submenu1 = glutCreateMenu(menu);
  glutAddMenuEntry("None", 10);
  glutAddMenuEntry("Pipe alone", 11);
  glutAddMenuEntry("Show nodes", 12);
  glutAddMenuEntry("Show loading", 13);
  glutAddMenuEntry("Show internal stress", 14);

  int submenu2 = glutCreateMenu(menu);
  glutAddMenuEntry("Axial", 20);
  glutAddMenuEntry("Bending moment", 21);
  glutAddMenuEntry("External Hoop Stress", 22);

  int submenu3 = glutCreateMenu(menu);
  glutAddMenuEntry("K", 30);
  glutAddMenuEntry("pressure", 31);
  */

  glutCreateMenu(menu); // Main menu
  // glutAddSubMenu("Pipe Display Options", submenu1);
  // glutAddSubMenu("Polar Plot Options", submenu2);
  // glutAddSubMenu("Spider Map Options", submenu3);
  glutAddMenuEntry("Quit", 0);
}

// =====================================================================
// Main function
// =====================================================================

int main(int argc, char *argv[]) {

  if (argc == 1) {
    confNum = 0;
    std::cout << "Current Configuration: ";
    try_to_readConf(confNum, Conf, confNum);
  } else if (argc == 2) {
    if (fileTool::containsOnlyDigits(argv[1])) {
      confNum = std::atoi(argv[1]);
      std::cout << "Current Configuration: ";
      try_to_readConf(confNum, Conf, confNum);
    } else {
      Conf.loadConf(argv[1]);
      std::cout << "Current Configuration: " << argv[1] << std::endl;
      confNum = Conf.iconf;
    }
  }

  mouse_mode = NOTHING;

  // ==== Init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
  int X0 = (glutGet(GLUT_SCREEN_WIDTH) - width) / 2;
  int Y0 = (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2;
  glutInitWindowPosition(X0, Y0);
  glutInitWindowSize(width, height);

  main_window = glutCreateWindow("CONF VISUALIZER");

  // ==== Register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  // ==== Menu
  buildMenu();
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // ==== Other initialisations
  glText::init();
  updateTextLine();

  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // ==== Enter GLUT event processing cycle
  fit_view();
  glutMainLoop();
  return 0;
}
