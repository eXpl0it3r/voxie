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

#include "PaletteEditor.hpp"
#include "VoxelFile.hpp"
#include "MainWindow.hpp"
#include "ColorSpace.hpp"
#include "ColorSlider.hpp"
#include "PaletteGrid.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QKeyEvent>
#include <QLabel>

extern void draw_pointer(int x, int y, QPainter& painter);
extern QLabel* create_label(const QString& text);

// PaletteEditor

QSpinBox* PaletteEditor::create_color_spinbox()
{
    QSpinBox* box = new QSpinBox(this);
    box->setMinimum(0);
    box->setMaximum(255);
    connect(box, SIGNAL(valueChanged(int)), this, SLOT(rgb_changed()));
    return box;
}

PaletteEditor::PaletteEditor(MainWindow* parent)
: QWidget(parent)
, window(parent)
, ignore_rgb(false)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    color_space = new ColorSpace(this);
    color_slider = new ColorSlider(this);
    grid = new PaletteGrid(window);
    name = new QLineEdit(this);

    layout->addWidget(grid, 0, Qt::AlignHCenter);
    layout->addWidget(color_space);
    layout->addWidget(color_slider);

    QHBoxLayout* rgb_layout = new QHBoxLayout;
    rgb_layout->addWidget(create_label("RGB"));
    r_edit = create_color_spinbox();
    rgb_layout->addWidget(r_edit);
    g_edit = create_color_spinbox();
    rgb_layout->addWidget(g_edit);
    b_edit = create_color_spinbox();
    rgb_layout->addWidget(b_edit);

    layout->addLayout(rgb_layout);

    QHBoxLayout* name_layout = new QHBoxLayout;
    name_layout->addWidget(create_label("Name"));
    connect(name, SIGNAL(textEdited(QString)), this, SLOT(name_changed()));
    name_layout->addWidget(name);

    layout->addLayout(name_layout);

    setLayout(layout);

    set_current();
}

RGBColor& PaletteEditor::get_palette_color()
{
    return global_palette[grid->palette_index];
}

void PaletteEditor::set_current()
{
    if (!window->get_voxel())
        return;
    name->setText(palette_names[grid->palette_index]);
    RGBColor& col = get_palette_color();
    QColor color(col.r, col.g, col.b);
    qreal h, s, v;
    color.getHsvF(&h, &s, &v);
    color_space->set_hsv(h, s, v);
    color_slider->set(h);
    if (!ignore_rgb)
    {
        ignore_rgb = true;
        r_edit->setValue(col.r);
        g_edit->setValue(col.g);
        b_edit->setValue(col.b);
        ignore_rgb = false;
    }
    grid->update();
}

void PaletteEditor::set_palette()
{
    QColor col = QColor::fromHsvF(color_space->hue, color_space->sat, color_space->val);
    RGBColor& pal = get_palette_color();
    pal.r = col.red();
    pal.g = col.green();
    pal.b = col.blue();
    ignore_rgb = true;
    r_edit->setValue(pal.r);
    g_edit->setValue(pal.g);
    b_edit->setValue(pal.b);
    ignore_rgb = false;
    grid->update();
    window->model_changed();
}

int PaletteEditor::get_palette_index()
{
    return grid->palette_index;
}

void PaletteEditor::reset_palette()
{
}

void PaletteEditor::rgb_changed()
{
    if (ignore_rgb)
        return;
    RGBColor& pal = get_palette_color();
    pal.r = r_edit->value();
    pal.g = g_edit->value();
    pal.b = b_edit->value();
    ignore_rgb = true;
    set_current();
    ignore_rgb = false;
    window->model_changed();
}

void PaletteEditor::name_changed()
{
    palette_names[grid->palette_index] = name->text();
}
