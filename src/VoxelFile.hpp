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

#ifndef VOXIE_VOXELFILE_HPP
#define VOXIE_VOXELFILE_HPP

#include "ReferencePoint.hpp"
#include "RGBColor.hpp"
#include "glm.hpp"
#include <QString>
#include <QFile>
#include <QDataStream>

#define VOXEL_AIR 255

extern RGBColor * global_palette;
extern QString * palette_names;

class VoxelModel;
class btCompoundShape;

class VoxelFile
{
public:
    unsigned char * data;
    qint32 x_size, y_size, z_size;
    qint32 x_offset, y_offset, z_offset;
    QString name;
    ReferencePoints points;
    btCompoundShape * shape;
    vec3 min, max;

    VoxelFile();
    VoxelFile(const QString & filename);
    VoxelFile(QFile & fp);
    VoxelFile(int x_size, int y_size, int z_size);
    ~VoxelFile();
    static void load_palette();
    static void save_palette();
    static unsigned char get_closest_index(RGBColor c);
    void load_fp(QFile & fp);
    bool load(const QString & filename);
    void save(const QString & filename);
    void save_fp(QFile & fp);
    void reset(int x, int y, int z);
    void add_point(const QString & name, int x, int y, int z);
    void remove_point(size_t i);
    ReferencePoint * get_point(const QString & name);
    ReferencePoint * get_point(int i);
    void resize(int x1, int y1, int z1, int x_size, int y_size, int z_size);
    void scale(float sx, float sy, float sz);
    void set_offset(int x, int y, int z);
    void optimize();
    void rotate();
    void clone(VoxelFile & other);
    vec3 get_min();
    vec3 get_max();

    inline unsigned char & get(int x, int y, int z)
    {
        return data[z + y * z_size + x * z_size * y_size];
    }

    inline unsigned char get_safe(int x, int y, int z)
    {
        if (!is_solid(x, y, z))
            return VOXEL_AIR;
        return get(x, y, z);
    }

    inline const RGBColor & get_color(int x, int y, int z)
    {
        return global_palette[get(x, y, z)];
    }

    inline bool is_solid(int x, int y, int z)
    {
        if (x < 0 || y < 0 || z < 0 ||
            x >= x_size || y >= y_size || z >= z_size)
            return false;
        return get(x, y, z) != VOXEL_AIR;
    }

    inline bool is_solid_fast(int x, int y, int z)
    {
        return get(x, y, z) != VOXEL_AIR;
    }

    inline bool is_surface(int x, int y, int z)
    {
        return !(is_solid(x - 1, y, z) && is_solid(x + 1, y, z) &&
                 is_solid(x, y - 1, z) && is_solid(x, y + 1, z) &&
                 is_solid(x, y, z - 1) && is_solid(x, y, z + 1));
    }

    inline void set(int x, int y, int z, unsigned char i)
    {
        if (x < 0 || y < 0 || z < 0 ||
            x >= x_size || y >= y_size || z >= z_size)
            return;
        get(x, y, z) = i;
    }

    VoxelModel * model;
    VoxelModel * get_model();
    void update_model();
    btCompoundShape * get_shape();
    void reset_shape();
};

#endif // VOXIE_VOXELFILE_HPP
