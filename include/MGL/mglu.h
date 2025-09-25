#ifndef __MGLU_H__
#define __MGLU_H__

#include <math.h>

#ifndef MGLAPI
#define MGLAPI
#endif // MGLAPI

MGLAPI void mgluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar);
MGLAPI void mgluOrtho2D(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top);
MGLAPI void mgluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble centerx,GLdouble centery,GLdouble centerz,GLdouble upx,GLdouble upy,GLdouble upz);

#endif // __MGLU_H__