/*
Copyright (c) 2013 Mathias Kaerlev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include "VoxelModel.hpp"
#include "VoxelFile.hpp"

#include <glm/gtc/noise.hpp>

VoxelModel::VoxelModel(VoxelFile* file)
: file(file)
, value(0)
{
}

void VoxelModel::draw_immediate(float alpha, bool offset)
{
    int x_offset, y_offset, z_offset;
    if (offset)
    {
        x_offset = file->x_offset;
        y_offset = file->y_offset;
        z_offset = file->z_offset;
    }
    else
        x_offset = y_offset = z_offset = 0;

    glBegin(GL_QUADS);
    unsigned char alpha_c = static_cast<unsigned char>(alpha * 255.0f);
    int x, y, z;
    for (x = 0; x < file->x_size; x++)
        for (y = 0; y < file->y_size; y++)
            for (z = 0; z < file->z_size; z++)
            {
                unsigned char color = file->get(x, y, z);
                if (color == VOXEL_AIR)
                    continue;
                RGBColor & color2 = global_palette[color];

                float noise = glm::simplex(vec3(x, y, z));
                vec3 color3 = vec3(color2.r, color2.g, color2.b);
                color3 *= (1.0f + noise * 0.01f);
                color3 = glm::clamp(color3, 0, 255);

                glColor4ub(static_cast<int>(color3.x), static_cast<int>(color3.y), static_cast<int>(color3.z), alpha_c);

                float gl_x1 = static_cast<float>(x + x_offset);
                float gl_x2 = gl_x1 + 1.0f;
                float gl_y1 = static_cast<float>(y + y_offset);
                float gl_y2 = gl_y1 + 1.0f;
                float gl_z1 = static_cast<float>(z + z_offset);
                float gl_z2 = gl_z1 + 1.0f;

                // Left Face
                if (!file->is_solid(x, y + 1, z))
                {
                    glNormal3f(0.0f, 1.0f, 0.0f);
                    glVertex3f(gl_x1, gl_y2, gl_z1);
                    glVertex3f(gl_x1, gl_y2, gl_z2);
                    glVertex3f(gl_x2, gl_y2, gl_z2);
                    glVertex3f(gl_x2, gl_y2, gl_z1);
                }

                // Right face
                if (!file->is_solid(x, y - 1, z))
                {
                    glNormal3f(0.0f, -1.0f, 0.0f);
                    glVertex3f(gl_x1, gl_y1, gl_z1); // Top right
                    glVertex3f(gl_x2, gl_y1, gl_z1); // Top left
                    glVertex3f(gl_x2, gl_y1, gl_z2); // Bottom left
                    glVertex3f(gl_x1, gl_y1, gl_z2); // Bottom right
                }

                // Top face
                if (!file->is_solid(x, y, z + 1))
                {
                    glNormal3f(0.0f, 0.0f, -1.0f);
                    glVertex3f(gl_x1, gl_y1, gl_z2); // Bottom left
                    glVertex3f(gl_x2, gl_y1, gl_z2); // Bottom right
                    glVertex3f(gl_x2, gl_y2, gl_z2); // Top right
                    glVertex3f(gl_x1, gl_y2, gl_z2); // Top left
                }

                // Bottom face
                if (!file->is_solid(x, y, z - 1))
                {
                    glNormal3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(gl_x1, gl_y1, gl_z1); // Bottom right
                    glVertex3f(gl_x1, gl_y2, gl_z1); // Top right
                    glVertex3f(gl_x2, gl_y2, gl_z1); // Top left
                    glVertex3f(gl_x2, gl_y1, gl_z1); // Bottom left
                }

                // Right face
                if (!file->is_solid(x + 1, y, z))
                {
                    glNormal3f(1.0f, 0.0f, 0.0f);
                    glVertex3f(gl_x2, gl_y1, gl_z1); // Bottom right
                    glVertex3f(gl_x2, gl_y2, gl_z1); // Top right
                    glVertex3f(gl_x2, gl_y2, gl_z2); // Top left
                    glVertex3f(gl_x2, gl_y1, gl_z2); // Bottom left
                }

                // Left Face
                if (!file->is_solid(x - 1, y, z))
                {
                    glNormal3f(-1.0f, 0.0f, 0.0f);
                    glVertex3f(gl_x1, gl_y1, gl_z1); // Bottom left
                    glVertex3f(gl_x1, gl_y1, gl_z2); // Bottom right
                    glVertex3f(gl_x1, gl_y2, gl_z2); // Top right
                    glVertex3f(gl_x1, gl_y2, gl_z1); // Top left
                }
            }
    glEnd();
}

void VoxelModel::update(bool force)
{
    if (!force && value != 0)
        return;
    glNewList(value, GL_COMPILE);
    draw_immediate();
    glEndList();
}

void VoxelModel::draw()
{
    if (value == 0)
    {
        value = glGenLists(1);
        update();
    }
    glDisable(GL_TEXTURE_2D);
    glCallList(value);
}

VoxelModel::~VoxelModel()
{
    glDeleteLists(value, 1);
}

ReferencePoint* VoxelModel::get_point(const QString& name)
{
    return file->get_point(name);
}
