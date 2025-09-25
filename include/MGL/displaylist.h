#ifndef __DISPLAYLIST_H__
#define __DISPLAYLIST_H__

#ifndef MGLAPI
#define MGLAPI
#endif // MGLAPI

#define MAX_DISPLAYLISTS (128)

typedef struct MGLDisplayList {
	GLenum drawmode;
	MGLVertex vtxbuffer[MGL_MAX_VERTICES];
	int count; // vtx count
} MGLDisplayList;

MGLAPI GLuint mglGenLists(GLsizei range);
MGLAPI void mglNewList(GLuint list,GLenum mode);
MGLAPI void mglEndList(void);
MGLAPI void mglCallList(GLuint list);
MGLAPI void mglDeleteLists(GLuint list,GLsizei range);

#endif // __DISPLAYLIST_H__