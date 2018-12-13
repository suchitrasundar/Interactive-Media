#ifndef IM_project_4_functions_h
#define IM_project_4_functions_h
#endif

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

inline double to_int(std::string const& str) {
	std::istringstream ss(str);
	int d;
	ss >> d;
	if (!(ss && (ss >> std::ws).eof()))
		exit(1);

	return d;
}

inline float to_float(std::string const& str) {
	std::istringstream ss(str);
	float f;
	ss >> f;
	if (!(ss && (ss >> std::ws).eof()))
		exit(1);
	return f;
}

inline char* to_char(string s)
{
	char *a = new char[s.size() + 1];
	a[s.size()] = 0;
	memcpy(a, s.c_str(), s.size());
	return a;
}

void triangleTransformation()
{
	Mat Rx, Ry, Rz;
	Mat T;

	Rx.origin();
	Ry.origin();
	Rz.origin();
	T.origin();
	Rx.element[1][1] = cos(theta_x);
	Rx.element[1][2] = -sin(theta_x);
	Rx.element[2][1] = sin(theta_x);
	Rx.element[2][2] = cos(theta_x);

	Ry.element[0][0] = cos(theta_y);
	Ry.element[0][2] = sin(theta_y);
	Ry.element[2][0] = -sin(theta_y);
	Ry.element[2][2] = cos(theta_y);

	T.element[0][3] = translate_x;
	T.element[1][3] = translate_y;

	Mat P = T * Rx * Ry;

	for (unsigned int i = 0; i < vt_list.size(); i++)
	{
		Vec current_pt, update_point;
		current_pt.element[0] = vt_list[i].x;
		current_pt.element[1] = vt_list[i].y;
		current_pt.element[2] = vt_list[i].z;
		current_pt.element[3] = 1.0f;
		update_point = P * current_pt;
		vt_list[i].x = update_point.element[0] / update_point.element[3];
		vt_list[i].y = update_point.element[1] / update_point.element[3];
		vt_list[i].z = update_point.element[2] / update_point.element[3];

		vt_list[i].normalize();
	}

	theta_x = 0;
	theta_y = 0;
	theta_z = 0;
	translate_x = 0;
	translate_y = 0;
	translate_z = 0;
}

Vertex uniformvector(Vertex v)
{
	Vertex output;
	output = v;
	float r = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	output.x = output.x / r;
	output.y = output.y / r;
	output.z = output.z / r;
	return output;
}

void shadeTriangles()
{
	if (shadingType == 0)
	{
		for (unsigned int i = 0; i < tg_list.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					tg_list[i].c[j].c[k] = 1.0;

				}
			}
		}
	}
	else if (shadingType == 1)
	{
		for (int i = 0; i < tg_list.size(); i++)
		{
			Vertex n;
			Vertex n1, n2;
			n1 = vt_list[tg_list[i].v_index[1]] - vt_list[tg_list[i].v_index[0]];
			n2 = vt_list[tg_list[i].v_index[2]] - vt_list[tg_list[i].v_index[1]];
			n1 = uniformvector(n1);
			n2 = uniformvector(n2);
			n = n2.cross(n1);
			n = uniformvector(n);
			Vertex center;
			center = vt_list[tg_list[i].v_index[1]] + vt_list[tg_list[i].v_index[0]] + vt_list[tg_list[i].v_index[2]];
			center.x = center.x / 3.0;
			center.y = center.y / 3.0;
			center.z = center.z / 3.0;
			Vertex l;
			Vertex bc;
			bc.x = teddy_color.c[0];
			bc.y = teddy_color.c[1];
			bc.z = teddy_color.c[2];
			l = center - li;
			Vertex c1;
			c1 = bc;
			float cosin = 0.0;
			l = uniformvector(l);

			if (l.dotProduct(n) > 0)
				cosin = l.dotProduct(n);
			c1.x = bc.x * a_color.c[0] + li.c.c[0] * bc.x * cosin;
			c1.y = bc.y * a_color.c[1] + li.c.c[1] * bc.y * cosin;
			c1.z = bc.z * a_color.c[2] + li.c.c[2] * bc.z * cosin;
			glColor3f(c1.x, c1.y, c1.z);
			glBegin(GL_TRIANGLES);
			glVertex3f(vt_list[tg_list[i].v_index[0]].x, vt_list[tg_list[i].v_index[0]].y, vt_list[tg_list[i].v_index[0]].z); //point 1
			glVertex3f(vt_list[tg_list[i].v_index[1]].x, vt_list[tg_list[i].v_index[1]].y, vt_list[tg_list[i].v_index[1]].z); //point 2
			glVertex3f(vt_list[tg_list[i].v_index[2]].x, vt_list[tg_list[i].v_index[2]].y, vt_list[tg_list[i].v_index[2]].z); //point 3
			glEnd();
		}
	}
	else if (shadingType == 2)
	{
		for (int i = 0; i < tg_list.size(); i++)
		{
			Vertex n;
			Vertex n1, n2;
			Vertex v;
			Vertex r;
			v.x = 0;
			v.y = 2;
			v.z = 5;
			n1 = vt_list[tg_list[i].v_index[1]] - vt_list[tg_list[i].v_index[0]];
			n2 = vt_list[tg_list[i].v_index[2]] - vt_list[tg_list[i].v_index[0]];
			n1 = uniformvector(n1);
			n2 = uniformvector(n2);
			n = n1.cross(n2);
			n = uniformvector(n);
			Vertex center;
			center = vt_list[tg_list[i].v_index[1]] + vt_list[tg_list[i].v_index[0]] + vt_list[tg_list[i].v_index[2]];
			center.x = 1.0*center.x / 3.0;
			center.y = 1.0*center.y / 3.0;
			center.z = 1.0*center.z / 3.0;
			Vertex l;
			l = li - center;
			l = uniformvector(l);
			Vertex bc;
			bc.x = teddy_color.c[0];
			bc.y = teddy_color.c[1];
			bc.z = teddy_color.c[2];
			l = center - li;
			Vertex c1;
			c1 = bc;
			float cosin;
			cosin = n.dotProduct(l);
			r = n * cosin * 2 - l;
			float cosin1 = 0.0;
			if (l.dotProduct(n) > 0)
				cosin1 = l.dotProduct(n);
			float cosin2 = 0.0;
			if (v.dotProduct(r) > 0)
				cosin2 = v.dotProduct(r);
			cosin2 = pow(cosin2, p);
			c1.x = bc.x*a_color.c[0] + li.c.c[0] * bc.x*cosin1 + li.c.c[0] * s_color.c[0] * cosin2;
			c1.y = bc.y*a_color.c[1] + li.c.c[1] * bc.y*cosin1 + li.c.c[1] * s_color.c[1] * cosin2;
			c1.z = bc.z*a_color.c[2] + li.c.c[2] * bc.z*cosin1 + li.c.c[2] * s_color.c[2] * cosin2;
			glColor3f(c1.x, c1.y, c1.z);
			glBegin(GL_TRIANGLES);
			glVertex3f(vt_list[tg_list[i].v_index[0]].x, vt_list[tg_list[i].v_index[0]].y, vt_list[tg_list[i].v_index[0]].z); //point 1
			glVertex3f(vt_list[tg_list[i].v_index[1]].x, vt_list[tg_list[i].v_index[1]].y, vt_list[tg_list[i].v_index[1]].z); //point 2
			glVertex3f(vt_list[tg_list[i].v_index[2]].x, vt_list[tg_list[i].v_index[2]].y, vt_list[tg_list[i].v_index[2]].z); //point 3
			glEnd();
		}

	}
}

void initialize()
{
	li.x = 0;
	li.y = 0;
	li.z = 5;
	li.c.c[0] = 0.1;
	li.c.c[1] = 0.1;
	li.c.c[2] = 0.1;

	li.normalize();

	li.c.c[0] = 0.3;
	li.c.c[1] = 0.3;
	li.c.c[2] = 0.3;

	a_color.c[0] = 0.9;
	a_color.c[1] = 0.9;
	a_color.c[2] = 0.9;

	teddy_color.c[0] = 1.9;
	teddy_color.c[1] = 1.7;
	teddy_color.c[2] = 1.1;

	s_color.c[0] = 0.2;
	s_color.c[1] = 0.2;
	s_color.c[2] = 0.2;
}

void loadObjectFile(const char *filename, vector<Vertex> &vt_list, vector<Triangle> &tg_list)
{
	ifstream myfile(filename);
	string line;
	string valueX, valueY, valueZ, v;
	string index0, index1, index2, f;

	int n = 0;
	while (!myfile.eof())
	{
		getline(myfile, line);
		if (line[0] == 'v')
		{
			std::istringstream iss(line);
			iss >> v >> valueX >> valueY >> valueZ;
			Vertex v;
			v.x = (GLfloat)to_float(valueX);
			v.y = (GLfloat)to_float(valueY);
			v.z = (GLfloat)to_float(valueZ);

			for (int c = 0; c < 3; c++)
			{
				v.c.c[c] = 0.5;
			}
			vt_list.push_back(v);
		}
		if (line[0] == 'f')
		{
			std::istringstream iss(line);
			iss >> f >> index0 >> index1 >> index2;
			int i0 = to_int(index0) - 1;
			int i1 = to_int(index1) - 1;
			int i2 = to_int(index2) - 1;

			Triangle triangle;
			triangle.v_index[0] = i0;
			triangle.v_index[1] = i1;
			triangle.v_index[2] = i2;

			triangle.c[0] = vt_list[i0].c;
			triangle.c[1] = vt_list[i1].c;
			triangle.c[2] = vt_list[i2].c;
			tg_list.push_back(triangle);
		}
	}
}
#pragma once
