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

#include "ColorSpace.hpp"

#include <cmath>

#include "PaletteEditor.hpp"
#include <QMouseEvent>
#include <QPainter>

extern void draw_pointer(int x, int y, QPainter& painter);

ColorSpace::ColorSpace(QWidget* parent)
: QWidget(parent)
, pix(NULL)
, hue(-1.0f)
{
    set_hsv(0.5f, 0.5f, 0.5f);
}

void ColorSpace::set_hue(float h)
{
    set_hsv(h, sat, val);
}

void ColorSpace::set_hsv(float h, float s, float v)
{
    h = std::max(0.0f, std::min(1.0f, h));
    s = std::max(0.0f, std::min(1.0f, s));
    v = std::max(0.0f, std::min(1.0f, v));
    if (hue != h)
    {
        hue = h;
        delete pix;
        pix = NULL;
    }
    sat = s;
    val = v;
    update();
}

void ColorSpace::set_mouse_pos(const QPoint& p)
{
    float s = static_cast<float>(p.x()) / static_cast<float>(width());
    float v = 1.0f - static_cast<float>(p.y()) / static_cast<float>(height());
    set_hsv(hue, s, v);
    static_cast<PaletteEditor*>(parentWidget())->set_palette();
}

void ColorSpace::mousePressEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    set_mouse_pos(event->pos());
}

void ColorSpace::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    set_mouse_pos(event->pos());
}

void ColorSpace::paintEvent(QPaintEvent* event)
{
    int w = width();
    int h = height();

    if (!pix || pix->height() != h || pix->width() != w)
    {
        delete pix;
        QImage img(w, h, QImage::Format_RGB32);
        unsigned int* pixel = reinterpret_cast<unsigned int*>(img.scanLine(0));
        for (int y = 0; y < h; y++)
        {
            const unsigned int* end = pixel + w;
            int x = 0;
            while (pixel < end)
            {
                QColor c;
                float sat = static_cast<float>(x) / static_cast<float>(w);
                float val = 1.0f - static_cast<float>(y) / static_cast<float>(h);
                c.setHsvF(hue, sat, val);
                *pixel = c.rgb();
                pixel++;
                x++;
            }
        }
        pix = new QPixmap(QPixmap::fromImage(img));
    }

    QPainter p(this);
    p.drawPixmap(0, 0, *pix);

    int p_x = static_cast<int>(sat * w);
    int p_y = static_cast<int>((1.0f - val) * h);
    draw_pointer(p_x, p_y, p);
}
