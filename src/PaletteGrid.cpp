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

#include "PaletteGrid.hpp"
#include "MainWindow.hpp"
#include "VoxelFile.hpp"
#include "PaletteEditor.hpp"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

PaletteGrid::PaletteGrid(MainWindow* parent)
: QWidget(parent)
, window(parent)
, palette_index(0)
{
    setFixedSize(192, 192);
    setAcceptDrops(true);
}

void PaletteGrid::paintEvent(QPaintEvent* event)
{
    VoxelFile* voxel = window->get_voxel();

    QPainter p(this);

    int x_size = width() / 8;
    int y_size = height() / 8;

    for (int i = 0; i <= VOXEL_AIR; i++)
    {
        int x = i % 8;
        int y = i / 8;
        int x1 = x * x_size;
        float y1 = y * y_size;
        unsigned char r, g, b;
        if (i == VOXEL_AIR || voxel == NULL)
            r = g = b = 20;
        else
        {
            RGBColor& color = global_palette[i];
            r = color.r;
            g = color.g;
            b = color.b;
        }

        const int outline = 1;
        if (palette_index == i)
        {
            p.fillRect(x1, y1, x_size, y_size, Qt::white);
            p.fillRect(x1 + outline, y1 + outline, 
                       x_size - outline * 2, y_size - outline * 2,
                       Qt::black);
            p.fillRect(x1 + outline * 2, y1 + outline * 2, 
                       x_size - outline * 4, y_size - outline * 4, 
                       QColor(r, g, b));
        }
        else
        {
            p.fillRect(x1, y1, x_size, y_size, QColor(r, g, b));
        }
    }
}

int PaletteGrid::get_index(const QPoint& p)
{
    int x_size = width() / 8;
    int y_size = height() / 8;
    int p_x = p.x() / x_size;
    int p_y = p.y() / y_size;
    return p_x + p_y * 8;
}

void PaletteGrid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;
    const QPoint& p = event->pos();
    drag_start = p;
    palette_index = get_index(p);
    static_cast<PaletteEditor*>(parentWidget())->set_current();
    update();
}

void PaletteGrid::mouseMoveEvent(QMouseEvent* event)
{
    VoxelFile* voxel = window->get_voxel();
    if (!voxel)
        return;
    if (!(event->buttons() & Qt::LeftButton))
        return;
     if ((event->pos() - drag_start).manhattanLength() < QApplication::startDragDistance())
         return;
    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    RGBColor& col = global_palette[palette_index];
    QColor col2(col.r, col.g, col.b);
    mimeData->setColorData(col2);
    drag->setMimeData(mimeData);
    //Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void PaletteGrid::dragEnterEvent(QDragEnterEvent* event)
{
    if (!event->mimeData()->hasColor())
        return;
    event->acceptProposedAction();
}

void PaletteGrid::dropEvent(QDropEvent* event)
{
    VoxelFile* voxel = window->get_voxel();
    if (!voxel)
        return;
    int r, g, b;
    QColor color = event->mimeData()->colorData().value<QColor>();
    color.getRgb(&r, &g, &b);
    int i = get_index(event->pos());
    global_palette[i] = RGBColor(r, g, b);
    event->acceptProposedAction();
    update();
    window->model_changed();
}
