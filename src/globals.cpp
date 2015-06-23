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

#include "RGBColor.hpp"
#include <QString>

RGBColor * global_palette = NULL;
QString * palette_names = NULL;


#include <QPainter>

#define POINTER_SIZE 10
#define POINTER_SIZE2 (POINTER_SIZE - 2)

void draw_pointer(int x, int y, QPainter& painter)
{
    painter.setPen(Qt::black);
    painter.drawArc(x - POINTER_SIZE / 2, y - POINTER_SIZE / 2, POINTER_SIZE, POINTER_SIZE, 0, 16 * 360);
    painter.setPen(Qt::white);
    painter.drawArc(x - POINTER_SIZE2 / 2, y - POINTER_SIZE2 / 2, POINTER_SIZE2, POINTER_SIZE2, 0, 16 * 360);
}


class QLabel;
class QWidget;

#include "opengl.hpp"
#include "glm.hpp"
#include "draw.hpp"
#include "collision.hpp"
#include <QObject>
#include <QLabel>
#include <QInputDialog>
#include <QEvent>
#include <QApplication>
#include <QFileDialog>

QLabel* create_label(const QString& text)
{
    QLabel* label = new QLabel(text);
    label->setFixedWidth(50);
    label->setAlignment(Qt::AlignCenter);
    return label;
}

std::string convert_str(const QString& str)
{
    return str.toUtf8().constData();
}

QString get_model_name(QWidget* parent, bool save)
{
    QString caption = QObject::tr("Model file dialog");
    QString filter = QObject::tr("Voxie model (*.vxi);;All Files (*)");
    QString text;
    if (save)
        text = QFileDialog::getSaveFileName(parent, caption, "", filter);
    else
        text = QFileDialog::getOpenFileName(parent, caption, "", filter);
    return text;
}

void set_window_file_path(QWidget* w, const QString& name)
{
    w->setWindowFilePath(name);
    QEvent e(QEvent::WindowTitleChange);
    QApplication::sendEvent(w, &e);
}

btTransform get_pos_trans(const vec3& p)
{
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(convert_vec(p));
    return trans;
}
