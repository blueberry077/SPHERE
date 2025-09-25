#ifndef __MGL_H__
#define __MGL_H__

#define MGL_MAX_VERTICES (4096)

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <GLAD/glad.h>

#ifndef MGLAPI
#define MGLAPI
#endif // MGLAPI

typedef struct MGLVertex {
	float x,y,z;
	float r,g,b,a;
	float u,v;
	float nx,ny,nz;
} MGLVertex;

#include <MGL/mat4x4.h>
#include <MGL/displaylist.h>

MGLAPI int mglInit(GLADloadproc load_proc);
MGLAPI void mglClose(void);

MGLAPI void mglBegin(GLenum mode);
MGLAPI void mglEnd(void);

MGLAPI void mglBindTexture(GLenum target,GLuint texture);

MGLAPI void mglTexCoord2f(GLfloat s,GLfloat t);

MGLAPI void mglColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
MGLAPI void mglColor3f(GLfloat red,GLfloat green,GLfloat blue);

MGLAPI void mglVertex3f(GLfloat x,GLfloat y,GLfloat z);
MGLAPI void mglVertex3fv(const GLfloat *v);
MGLAPI void mglVertex2f(GLfloat x,GLfloat y);

MGLAPI void mglNormal3f(GLfloat nx,GLfloat ny,GLfloat nz);

MGLAPI void mglMatrixMode(GLenum mode);
MGLAPI void mglLoadIdentity(void);
MGLAPI void mglFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);
MGLAPI void mglOrtho(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);

MGLAPI void mglScalef(GLfloat x,GLfloat y,GLfloat z);
MGLAPI void mglTranslatef(GLfloat x,GLfloat y,GLfloat z);
MGLAPI void mglRotatef(GLfloat angle,GLfloat x,GLfloat y,GLfloat z);

#endif // __MGL_H__