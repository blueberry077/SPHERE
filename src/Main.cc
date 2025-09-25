// Resources:
// https://mathworld.wolfram.com/SphericalCoordinates.html
// https://mathworld.wolfram.com/SphericalGeometry.html
// https://en.wikipedia.org/wiki/Spherical_coordinate_system
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

extern "C" {
#include <MGL/mgl.h>
#include <MGL/mglu.h>
#include <GLFW/glfw3.h>
}

#define W_WIDTH  (800)
#define W_HEIGHT (600)

double m_sin(double a) { return std::sin(a * M_PI/180.0); }
double m_cos(double a) { return std::cos(a * M_PI/180.0); }

struct double3 {
  double x, y, z;
};

void draw_pixel(int X, int Y);
void draw_point(int X, int Y);
void draw_circle(int X, int Y, float R);
void draw_line(int X1, int Y1, int X2, int Y2);
void draw_sphere_line(double3 A, double3 B, int steps);

inline double3 operator+(const double3& a, const double3& b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}
inline double3 operator*(float s, const double3& a) {
  return {s * a.x, s * a.y, s * a.z};
}
inline double3 operator*(const double3& a, float s) {
  return {s * a.x, s * a.y, s * a.z};
}

double3 slerp(double3 a, double3 b, float t);
void x_rot_matrix(double3& out, double3 in, double a);
void y_rot_matrix(double3& out, double3 in, double a);
void z_rot_matrix(double3& out, double3 in, double a);

struct Color {
  float r, g, b;
};

const std::vector<Color> palette = {
  {1.0, 0.0, 0.0},   // Red
  {0.0, 1.0, 0.0},   // Green
  {0.0, 0.0, 1.0},   // Blue
  {1.0, 1.0, 0.0},   // Yellow
  {1.0, 0.0, 1.0},   // Magenta
  {0.0, 1.0, 1.0},   // Cyan
  {1.0, 1.0, 1.0},   // White
  {0.5, 0.0, 0.0},   // Dark Red
  {0.0, 0.5, 0.0},   // Dark Green
  {0.0, 0.0, 0.5},   // Dark Blue
  {0.5, 0.5, 0.0},   // Dark Yellow
  {0.5, 0.0, 0.5},   // Dark Magenta
  {0.0, 0.5, 0.5},   // Dark Cyan
  {0.5, 0.5, 0.5},   // Dark Grey
};
GLFWwindow *window = NULL;
double mp[2];
int mouse_pressed = 0;
int mouse_event_var = 0;

class Point {
public:
  Point(std::string name, int color=0, double r=0.0, double th=0.0, double ph=0.0)
    : name_(std::move(name)), r_(r), th_(th), ph_(ph), color_(color) {}
  
public:
  void toCartesian(double3& p) {
    p.x = r_ * m_sin(th_) * m_cos(ph_);
    p.y = r_ * m_sin(th_) * m_sin(ph_);
    p.z = r_ * m_cos(th_);
  }
  
public:
  std::string name_;
  double r_, th_, ph_;
  int color_;
};

class Sphere {
public:
  Sphere(double r)
    : r_(r) { rot_.x = 0.0; rot_.y = 0.0; rot_.z = 0.0; }

public:
  void AddPoint(const Point& point) {
    points_.push_back(point);
  }
  
  void SetRotation(double3 rot) {
    rot_ = std::move(rot);
  }
  
  void Update() {
    if (mouse_pressed & 1 << 1) {
      selected_.clear();
    }
    
    for (int i = 0; i < points_.size(); ++i) {
      double3 ro3d;
      points_[i].toCartesian(ro3d);
      z_rot_matrix(ro3d, ro3d, rot_.z);
      y_rot_matrix(ro3d, ro3d, rot_.y);
      x_rot_matrix(ro3d, ro3d, rot_.x);
      
      int pj[2];
      pj[0] = W_WIDTH/2 + static_cast<int>(ro3d.x * 200.0);
      pj[1] = W_HEIGHT/2 + static_cast<int>(ro3d.y * 200.0);
      
      float dx = mp[0] - pj[0];
      float dy = mp[1] - pj[1];
      float d = std::sqrt(dx*dx + dy*dy);
      if (d <= 30.0 && mouse_pressed & 1) {
        if (std::find(selected_.begin(), selected_.end(), i) == selected_.end()) {
          selected_.push_back(i);
        }
      }
    }
  }
  
  void Render() const {
    mglColor3f(1.0, 1.0, 1.0);
    draw_point(W_WIDTH/2, W_HEIGHT/2);
    draw_circle(W_WIDTH/2, W_HEIGHT/2, 200.0);
    
    // -----------------------------------
    float angle;
    double3 s;
    
    angle = 0.0;
    for (int i = 0; i < 360; ++i) {
      s.x =   0.0;
      s.y =   0.0;
      s.z = 200.0;
      y_rot_matrix(s, s, angle);
      z_rot_matrix(s, s, rot_.z);
      y_rot_matrix(s, s, rot_.y);
      x_rot_matrix(s, s, rot_.x);
      if (s.z < 0.0) {
        mglColor3f(1.0, 1.0, 1.0);
      } else {
        mglColor3f(0.5, 0.5, 0.5);
      }
      draw_pixel(s.x + W_WIDTH/2, s.y + W_HEIGHT/2);
      angle += 1.0;
    }
    
    angle = 0.0;
    for (int i = 0; i < 360; ++i) {
      s.x =   0.0;
      s.y =   0.0;
      s.z = 200.0;
      x_rot_matrix(s, s, angle);
      z_rot_matrix(s, s, rot_.z);
      y_rot_matrix(s, s, rot_.y);
      x_rot_matrix(s, s, rot_.x);
      if (s.z < 0.0) {
        mglColor3f(0.7, 0.7, 0.7);
      } else {
        mglColor3f(0.2, 0.2, 0.2);
      }
      draw_pixel(s.x + W_WIDTH/2, s.y + W_HEIGHT/2);
      angle += 1.0;
    }
    
    // -----------------------------------
    
    for (int i = 0; i < points_.size(); ++i) {
      Point p = points_[i];
      int col = p.color_ % 14;
      mglColor3f(
        palette[col].r,
        palette[col].g,
        palette[col].b
      );
      
      double3 c3d; // catesian coordinates
      p.toCartesian(c3d);
      
      double3 ro3d = c3d;
      z_rot_matrix(ro3d, ro3d, rot_.z);
      y_rot_matrix(ro3d, ro3d, rot_.y);
      x_rot_matrix(ro3d, ro3d, rot_.x);
      
      int pj[2];
      pj[0] = W_WIDTH/2 + static_cast<int>(ro3d.x * 200.0);
      pj[1] = W_HEIGHT/2 + static_cast<int>(ro3d.y * 200.0);
      draw_line(W_WIDTH/2, W_HEIGHT/2, pj[0], pj[1]);
      draw_point(pj[0], pj[1]);
      
      if (std::find(selected_.begin(), selected_.end(), i) != selected_.end()) {
        mglColor3f(1.0, 1.0, 1.0);
        draw_circle(pj[0], pj[1], 5.0);
      }
    }
    
    // DRAW LINES
    
    if (!selected_.empty()) {
      int sz = selected_.size();
      for (int i = 0; i < sz; ++i) {
        Point p1 = points_[selected_[i % sz]];
        Point p2 = points_[selected_[(i+1) % sz]];
        
        double3 c1, c2;
        p1.toCartesian(c1);
        p2.toCartesian(c2);
        mglColor3f(0.0, 0.0, 1.0);
        draw_sphere_line(c1, c2, 200);
      }
    }
  }

public:
  double r_;
  double3 rot_;
  std::vector<Point> points_;
  std::vector<int> selected_;
};

Sphere sphere = Sphere(10.0);

int InitApplication();
void CloseApplication();

int main(int argc, char **argv)
{
  if (InitApplication() < 0) {
    std::cout << "InitApplication()\n";
    return -1;
  }

  mglMatrixMode(GL_PROJECTION);
  mglLoadIdentity();
  mgluOrtho2D(0, W_WIDTH, W_HEIGHT, 0);
  
  sphere.AddPoint(Point("A", 0, 1.0, -20.0, 0.0));
  sphere.AddPoint(Point("B", 1, 1.0, 60.0, -60.0));
  sphere.AddPoint(Point("C", 2, 1.0, 80.0, 45.0));
  
  glClearColor(0.0, 0.0, 0.0, 0.0);
  mglMatrixMode(GL_MODELVIEW);
  mglLoadIdentity();
  
  sphere.SetRotation({25.0, 200.0, 0.0});
  
  while (!glfwWindowShouldClose(window)) {
    mouse_pressed = mouse_event_var;
    mouse_event_var = 0;
    glfwGetCursorPos(window, &mp[0], &mp[1]);
    sphere.Update();
    
    glClear(GL_COLOR_BUFFER_BIT);
    double3 rot = sphere.rot_;
    rot.y += 1.0;
    sphere.SetRotation(rot);
    sphere.Render();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    Sleep(16);
    
  }
  CloseApplication();
  return 0;
}

void draw_pixel(int X, int Y)
{
  mglBegin(GL_POINTS);
    mglVertex2f(X, Y);
  mglEnd();
}

void draw_line(int X1, int Y1, int X2, int Y2)
{
  mglBegin(GL_LINES);
    mglVertex2f(X1, Y1);
    mglVertex2f(X2, Y2);
  mglEnd();
}

void draw_point(int X, int Y)
{
  mglBegin(GL_POINTS);
    mglVertex2f(  X,   Y);
    mglVertex2f(X-1,   Y);
    mglVertex2f(X+1,   Y);
    mglVertex2f(X,   Y-1);
    mglVertex2f(X,   Y+1);
  mglEnd();
}

void draw_circle(int X, int Y, float R)
{
  float m, x, y;
  y = R;
  x = 0.0;
  mglBegin(GL_POINTS);
  while (x <= y) {
    mglVertex2f(X+x,Y-y);
    mglVertex2f(X+y,Y-x);
    mglVertex2f(X-x,Y-y);
    mglVertex2f(X-y,Y-x);
    mglVertex2f(X+x,Y+y);
    mglVertex2f(X+y,Y+x);
    mglVertex2f(X-x,Y+y);
    mglVertex2f(X-y,Y+x);
    x++;
    m = x*x + y*y - R*R;
    if (m > 0) {
      y--;
    }
  }
  mglEnd();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (action == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
      mouse_event_var |= 1;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
      mouse_event_var |= 1 << 1;
  }
}

int InitApplication(void)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Sphere Calculator", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwMakeContextCurrent(window);
  mglInit((GLADloadproc)glfwGetProcAddress);
  return 0;
}

void CloseApplication(void)
{
  mglClose();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void draw_sphere_line(double3 A, double3 B, int steps)
{
  for (int i = 0; i <= steps; i++) {
    float t = (float)i / steps;
    double3 P = slerp(A, B, t);

    z_rot_matrix(P, P, sphere.rot_.z);
    y_rot_matrix(P, P, sphere.rot_.y);
    x_rot_matrix(P, P, sphere.rot_.x);

    draw_pixel(W_WIDTH/2 + static_cast<int>(P.x * 200.0),
               W_HEIGHT/2 + static_cast<int>(P.y * 200.0));
  }
}

double3 slerp(double3 a, double3 b, float t)
{
  float dot = a.x*b.x + a.y*b.y + a.z*b.z;
  float theta = acos(dot);
  float sin_theta = sin(theta);
  return (std::sin((1 - t) * theta) / sin_theta) * a +
         (std::sin(t * theta) / sin_theta) * b;
}

void x_rot_matrix(double3& out, double3 in, double a)
{
  out.x = (in.x * 1) + (in.y *        0) + (in.z *         0);
  out.y = (in.x * 0) + (in.y * m_cos(a)) + (in.z * -m_sin(a));
  out.z = (in.x * 0) + (in.y * m_sin(a)) + (in.z *  m_cos(a));
}

void y_rot_matrix(double3& out, double3 in, double a)
{
  out.x = (in.x *  m_cos(a)) + (in.y * 0) + (in.z * m_sin(a));
  out.y = (in.x *         0) + (in.y * 1) + (in.z *        0);
  out.z = (in.x * -m_sin(a)) + (in.y * 0) + (in.z * m_cos(a));
}

void z_rot_matrix(double3& out, double3 in, double a)
{
  out.x = (in.x * m_cos(a)) + (in.y * -m_sin(a)) + (in.z * 0);
  out.y = (in.x * m_sin(a)) + (in.y *  m_cos(a)) + (in.z * 0);
  out.z = (in.x *        0) + (in.y *         0) + (in.z * 1);
}