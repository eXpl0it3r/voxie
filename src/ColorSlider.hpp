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

#ifndef VOXIE_COLORSLIDER_HPP
#define VOXIE_COLORSLIDER_HPP

#include <QWidget>

class ColorSlider : public QWidget
{
    Q_OBJECT

public:
    QPixmap * pix;
    float value;

    ColorSlider(QWidget * parent = 0);
    void set(float v);
    void set_mouse_pos(const QPoint & p);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent * event);
};

#endif // VOXIE_COLORSLIDER_HPP
