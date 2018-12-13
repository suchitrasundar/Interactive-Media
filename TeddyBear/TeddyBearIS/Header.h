#pragma once
#ifndef IM_project_4_callbackFunction_h
#define IM_project_4_callbackFunction_h
#endif

//#include "data.h"
//#include "Functions.h"
//Color teddy_color;

void onKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(1);
		break;
	case 'm':
		mesh_only = (mesh_only == 1) ? 0 : 1;
		glutPostRedisplay();
		break;
	case 'g':
		teddy_color.c[0] = 0.9;
		teddy_color.c[1] = 0.7;
		teddy_color.c[2] = 0.1;
		glutPostRedisplay();
		break;

	default:
		break;
	}
}


void onMouse(int button, int state, int x, int y)
{

	GLint specialKey = glutGetModifiers();
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			mouse_down = 1;
			current_x = x;
			current_y = y;
			if (specialKey == GLUT_ACTIVE_SHIFT)
			{
				change_mode = 1;
			}
			else
			{
				change_mode = 0;
			}
		}
		else if (state == GLUT_UP)
		{
			mouse_down = 0;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			break;
	default:
		break;
	}
}

void onMouseMotion(int x, int y)
{
	if (mouse_down == 1)
	{
		if (change_mode == 0)
		{
			theta_y += static_cast<float>(x - current_x) / 100.f;
			theta_x += static_cast<float>(y - current_y) / 100.f;
		}
		else {
			translate_x += static_cast<float>(x - current_x) / 1000.f;
			translate_y += static_cast<float>(-y + current_y) / 1000.f;
		}

		current_x = x;
		current_y = y;
	}
	glutPostRedisplay();
}
void modifyTriangle()
{
	for (int i = 0; i < tg_list.size(); i++)
	{
		if (mesh_only)
		{
			glBegin(GL_LINE_LOOP);
		}
		else
		{
			glBegin(GL_TRIANGLES);
		}

		int point_0 = tg_list[i].v_index[0];
		int point_1 = tg_list[i].v_index[1];
		int point_2 = tg_list[i].v_index[2];

		glColor3f(tg_list[i].c[0].c[0], 
				tg_list[i].c[0].c[1], 
				tg_list[i].c[0].c[2]);
		glVertex3f(vt_list[point_0].x, 
					vt_list[point_0].y,
					vt_list[point_0].z);

		glColor3f(tg_list[i].c[1].c[0], 
			tg_list[i].c[1].c[1], 
			tg_list[i].c[1].c[2]);
		glVertex3f(vt_list[point_1].x, 
			vt_list[point_1].y,
			vt_list[point_1].z);

		glColor3f(tg_list[i].c[2].c[0],
			tg_list[i].c[2].c[1],
			tg_list[i].c[2].c[2]);
		glVertex3f(vt_list[point_2].x, 
			vt_list[point_2].y, 
			vt_list[point_2].z);
		glEnd();
	}
}

void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);

	if (mouse_down == 1)
		triangleTransformation();
	shadeTriangles();
	modifyTriangle();
	printf("light: %.3f, %.3f, %.3f\n", li.x, li.y, li.z);
	glFlush();
}
