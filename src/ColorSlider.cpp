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

#include "ColorSlider.hpp"

#include <cmath>

#include "ColorSpace.hpp"
#include "PaletteEditor.hpp"
#include <QMouseEvent>
#include <QPainter>

extern void draw_pointer(int x, int y, QPainter & painter);

ColorSlider::ColorSlider(QWidget * parent)
: QWidget(parent)
, pix(NULL)
{
    setFixedHeight(10);
}

void ColorSlider::set(float v)
{
    value = std::max(0.0f, std::min(1.0f, v));
    static_cast<PaletteEditor*>(parentWidget())->color_space->set_hue(value);
    update();
}

void ColorSlider::set_mouse_pos(const QPoint & p)
{
    set(float(p.x()) / float(width()));
    ((PaletteEditor*)parentWidget())->set_palette();
}

void ColorSlider::mousePressEvent(QMouseEvent * event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    set_mouse_pos(event->pos());
}

void ColorSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    set_mouse_pos(event->pos());
}

void ColorSlider::paintEvent(QPaintEvent * event)
{
    int w = width();
    int h = height();

    if (!pix || pix->width() != w) {
        delete pix;
        QImage img(w, 1, QImage::Format_RGB32);
        unsigned int * pixel = (unsigned int*)img.scanLine(0);
        const unsigned int * end = pixel + w;
        int x = 0;
        while (pixel < end) {
            QColor c;
            float hue = float(x) / float(w);
            c.setHsvF(hue, 1.0f, 1.0f);
            *pixel = c.rgb();
            pixel++;
            x++;
        }
        pix = new QPixmap(QPixmap::fromImage(img));
    }

    QPainter p(this);
    p.drawPixmap(0, 0, w, h, *pix);

    int p_x = int(value * w);
    draw_pointer(p_x, h / 2, p);
}
