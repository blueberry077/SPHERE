// * Astrolog-like 3D Sphere renderer
// * 24.10.2024
#include <math.h>
#include <atom.h>

#define W_WIDTH  (480)
#define W_HEIGHT (480)

#define n_v2f(self, a, b)     {self[0] = a; self[1] = b;}
#define n_v3f(self, a, b, c)  {self[0] = a; self[1] = b; self[2] = c;}

typedef GLfloat v3f[3];
typedef GLfloat v2f[2];
typedef struct {
  v2f angle;
  v3f color;
} Point;

void draw_pixel(int X, int Y);
void draw_cross(int X, int Y);
void draw_circle(int X, int Y, float R);
void draw_line(int X1, int Y1, int X2, int Y2, BOOLEAN dotted);

void project(v2f out, v3f in);
void x_rot_matrix(v3f out, v3f in, float a);
void y_rot_matrix(v3f out, v3f in, float a);
void z_rot_matrix(v3f out, v3f in, float a);

void draw_sphere(void);
void draw_graduations(void);
void draw_point(Point point);

float m_sin(float a) { return sin(a * M_PI/180.0); }
float m_cos(float a) { return cos(a * M_PI/180.0); }

v3f sphere_angle;
v3f point_init;
Point points[4];

int main(int argc, char **argv)
{
  ATOM_CreateEngine(argv[0], 10, 10, W_WIDTH, W_HEIGHT);
  ATOM_MakeGLContext();
  
  sphere_angle[0] = 5.0;
  sphere_angle[1] = 0.0;
  sphere_angle[2] = 0.0;
  
  n_v2f(points[0].angle, 0.0, 0.0);
  n_v3f(points[0].color, 1.0, 0.0, 0.0);
  
  n_v2f(points[1].angle, 90.0, 0.0);
  n_v3f(points[1].color, 0.0, 1.0, 0.0);
  
  n_v2f(points[2].angle, -90.0, 0.0);
  n_v3f(points[2].color, 0.0, 0.0, 1.0);
  
  n_v2f(points[3].angle, 180.0, 0.0);
  n_v3f(points[3].color, 1.0, 1.0, 0.0);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, W_WIDTH, W_HEIGHT, 0);
  
  glLineStipple(1, 0x5555);
  glEnable(GL_LINE_STIPPLE);
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glViewport(0, 0, W_WIDTH, W_HEIGHT);
  while (ATOM_EngineIsRunning()) {
    // Drawing
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_sphere();
    draw_graduations();
    draw_point(points[0]);
    draw_point(points[1]);
    draw_point(points[2]);
    draw_point(points[3]);
    // Updates
    sphere_angle[1] += -0.2;
  }
  
  ATOM_DeleteGLContext();
  ATOM_DestroyEngine();
  return 0;
}

void draw_pixel(int X, int Y)
{
  glBegin(GL_POINTS);
    glVertex2i(X, Y);
  glEnd();
}

void draw_cross(int X, int Y)
{
  glBegin(GL_POINTS);
    glVertex2i(  X,   Y);
    glVertex2i(X-1,   Y);
    glVertex2i(X+1,   Y);
    glVertex2i(X,   Y-1);
    glVertex2i(X,   Y+1);
  glEnd();
}

void draw_circle(int X, int Y, float R)
{
  float m, x, y;
  y = R;
  x = 0.0;
  glBegin(GL_POINTS);
  while (x <= y) {
    glVertex2f(X+x,Y-y);
    glVertex2f(X+y,Y-x);
    glVertex2f(X-x,Y-y);
    glVertex2f(X-y,Y-x);
    glVertex2f(X+x,Y+y);
    glVertex2f(X+y,Y+x);
    glVertex2f(X-x,Y+y);
    glVertex2f(X-y,Y+x);
    x++;
    m = x*x + y*y - R*R;
    if (m > 0) {
      y--;
    }
  }
  glEnd();
}

void draw_line(int X1, int Y1, int X2, int Y2, BOOLEAN dotted)
{
  if (dotted) {
    glEnable(GL_LINE_STIPPLE);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glBegin(GL_LINES);
    glVertex2i(X1, Y1);
    glVertex2i(X2, Y2);
  glEnd();
}

void project(v2f out, v3f in)
{
  // Orthographic projection
  out[0] = in[0];
  out[1] = in[1];
}

void x_rot_matrix(v3f out, v3f in, float a)
{
  v3f tmp;
  tmp[0] = (in[0] * 1) + (in[1] *        0) + (in[2] *         0);
  tmp[1] = (in[0] * 0) + (in[1] * m_cos(a)) + (in[2] * -m_sin(a));
  tmp[2] = (in[0] * 0) + (in[1] * m_sin(a)) + (in[2] *  m_cos(a));
  out[0] = tmp[0];
  out[1] = tmp[1];
  out[2] = tmp[2];
}

void y_rot_matrix(v3f out, v3f in, float a)
{
  v3f tmp;
  tmp[0] = (in[0] *  m_cos(a)) + (in[1] * 0) + (in[2] * m_sin(a));
  tmp[1] = (in[0] *         0) + (in[1] * 1) + (in[2] *        0);
  tmp[2] = (in[0] * -m_sin(a)) + (in[1] * 0) + (in[2] * m_cos(a));
  out[0] = tmp[0];
  out[1] = tmp[1];
  out[2] = tmp[2];
}

void z_rot_matrix(v3f out, v3f in, float a)
{
  v3f tmp;
  tmp[0] = (in[0] * m_cos(a)) + (in[1] * -m_sin(a)) + (in[2] * 0);
  tmp[1] = (in[0] * m_sin(a)) + (in[1] *  m_cos(a)) + (in[2] * 0);
  tmp[2] = (in[0] *        0) + (in[1] *         0) + (in[2] * 1);
  
  out[0] = tmp[0];
  out[1] = tmp[1];
  out[2] = tmp[2];
}

void draw_sphere(void)
{
  glColor3f(1.0, 1.0, 1.0);
  draw_cross(W_WIDTH/2, W_HEIGHT/2);
  draw_circle(W_WIDTH/2, W_HEIGHT/2, 200.0);
}

void draw_graduations(void)
{
  v3f s;
  v2f P;
  int counter;
  float angle;
  // Horizontal graduations
  counter = 0;
  angle = 0.0;
  for (int i = 0; i < 360; ++i) {
    s[0] =   0.0;
    s[1] =   0.0;
    s[2] = 200.0;
    y_rot_matrix(s, s, angle);
    // apply the rotations of the sphere
    z_rot_matrix(s, s, sphere_angle[2]);
    y_rot_matrix(s, s, sphere_angle[1]);
    x_rot_matrix(s, s, sphere_angle[0]);
    project(P, s);
    P[0] += W_WIDTH/2;
    P[1] += W_HEIGHT/2;
    if (s[2] < 0.0) {
      glColor3f(0.5, 0.5, 0.5);
      draw_pixel(P[0], P[1]);
    } else {
      glColor3f(1.0, 1.0, 1.0);
      if (counter % 10 == 0) {
        draw_line(P[0], P[1]-5, P[0], P[1]+5, FALSE);
      } else
      if (counter % 5 == 0) {
        draw_line(P[0], P[1]-3, P[0], P[1]+3, FALSE);
      } else {
        draw_line(P[0], P[1]-1, P[0], P[1]+1, FALSE);
      }
    }
    counter++;
    angle += 1.0;
  }
  // Vertical graduations
  angle = 0.0;
  counter = 0;
  for (int i = 0; i < 360; ++i) {
    s[0] =   0.0;
    s[1] =   0.0;
    s[2] = 200.0;
    x_rot_matrix(s, s, angle);
    // apply the rotations of the sphere
    z_rot_matrix(s, s, sphere_angle[2]);
    y_rot_matrix(s, s, sphere_angle[1]);
    x_rot_matrix(s, s, sphere_angle[0]);
    project(P, s);
    P[0] += W_WIDTH/2;
    P[1] += W_HEIGHT/2;
    if (s[2] < 0.0) {
      glColor3f(0.5, 0.5, 0.5);
      draw_pixel(P[0], P[1]);
    } else {
      glColor3f(0.6, 0.6, 0.6);
      if (counter % 10 == 0) {
        draw_line(P[0]-5, P[1], P[0]+5, P[1], FALSE);
      } else
      if (counter % 5 == 0) {
        draw_line(P[0]-3, P[1], P[0]+3, P[1], FALSE);
      } else {
        draw_line(P[0]-1, P[1], P[0]+1, P[1], FALSE);
      }
      // draw_pixel(P[0], P[1]);
    }
    angle += 1.0;
    counter++;
  }
}

void draw_point(Point point)
{
  v2f P;
  v3f s;
  // Every point have an initial position (0, 0) on the sphere
  s[0] =   0.0;
  s[1] =   0.0;
  s[2] = 200.0;
  // Place the point using its rotations
  y_rot_matrix(s, s, point.angle[0]);
  z_rot_matrix(s, s, point.angle[1]);
  // apply the rotations of the sphere
  z_rot_matrix(s, s, sphere_angle[2]);
  y_rot_matrix(s, s, sphere_angle[1]);
  x_rot_matrix(s, s, sphere_angle[0]);
  project(P, s);
  // Align to the center of the screen
  P[0] += W_WIDTH/2;
  P[1] += W_HEIGHT/2;
  glColor3f(point.color[0], point.color[1], point.color[2]);
  if (s[2] < 0.0) {
    draw_line(W_WIDTH/2, W_HEIGHT/2, P[0], P[1], TRUE);
  } else {
    draw_line(W_WIDTH/2, W_HEIGHT/2, P[0], P[1], FALSE);
  }
  draw_cross(P[0], P[1]);
}