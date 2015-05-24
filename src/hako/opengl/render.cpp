#ifdef HAKO_BUILD_GFXOPENGL


#include "render.h"
#include "gl_includes.h"
#include <hako/common/debug.h>
#include <math.h>


void Hako::common_opengl_render(Hako::Engine* engine)
{
	HAKO_UNUSED(engine);

	//Animation
	static float debug_x = 0;
	debug_x += 0.1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
	glViewport(0, 0, 640, 480);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(cos(debug_x), 0.0, -3.0);
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0, 0.7, 0.1);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);
	glEnd();
}


#endif
