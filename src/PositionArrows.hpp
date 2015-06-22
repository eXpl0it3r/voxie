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

#ifndef VOXIE_POSITIONARROWS_HPP
#define VOXIE_POSITIONARROWS_HPP

#include "glm.hpp"

class btCompoundShape;

#define NONE_CONE -1
#define X_CONE 0
#define Y_CONE 1
#define Z_CONE 2

#define CONE_HEIGHT 75.0f
#define CONE_RADIUS 10.0f
#define LINE_LEN 100.0f
#define CONE_START (LINE_LEN + CONE_HEIGHT / 2.0f)

#define RED_R 248
#define RED_G 0
#define RED_B 53

#define BLUE_R 30
#define BLUE_G 117
#define BLUE_B 255

#define GREEN_R 154
#define GREEN_G 200
#define GREEN_B 2

class PositionArrows
{
public:
    float scale;

    int pan;
    vec3 normal, pos, last, add;

    btCompoundShape * shape;

    PositionArrows(float scale);
    ~PositionArrows();
    void set_scale(float scale);
    void set_pos(const vec3 & p);
    void on_mouse_press(const vec3 & ray_pos, const vec3 & ray_dir);
    void on_mouse_move(const vec3 & ray_pos, const vec3 & ray_dir);
    void on_mouse_release();
    void update(const vec3 & ray_pos, const vec3 & ray_dir);
    vec3 get(float grid);
    int ray_test(const vec3 & pos, const vec3 & dir);
    void draw();
};

struct ArrowResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
    int index;

    ArrowResultCallback(const btVector3 & a, const btVector3 & b)
    : btCollisionWorld::ClosestRayResultCallback(a, b), index(NONE_CONE)
    {
    }

    btScalar addSingleResult(btCollisionWorld::LocalRayResult & r, bool n)
    {
        index = r.m_localShapeInfo->m_triangleIndex;
        return btCollisionWorld::ClosestRayResultCallback::addSingleResult(
            r, n);
    }
};

#endif // VOXIE_POSITIONARROWS_HPP
