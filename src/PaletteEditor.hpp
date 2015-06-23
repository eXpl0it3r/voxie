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

#ifndef VOXIE_PALETTEEDITOR_HPP
#define VOXIE_PALETTEEDITOR_HPP

#include "RGBColor.hpp"

#include <QWidget>
#include <QLineEdit>

class MainWindow;
class PaletteGrid;
class QSpinBox;
class ColorSpace;
class ColorSlider;

class PaletteEditor : public QWidget
{
    Q_OBJECT

public:
    MainWindow* window;
    PaletteGrid* grid;
    ColorSpace* color_space;
    ColorSlider* color_slider;
    QSpinBox* r_edit;
    QSpinBox* g_edit;
    QSpinBox* b_edit;
    QLineEdit* name;
    bool ignore_rgb;

    PaletteEditor(MainWindow* parent);
    int get_palette_index();
    void set_current();
    void set_palette();
    RGBColor& get_palette_color();
    QSpinBox* create_color_spinbox();
    void reset_palette();

public slots:
    void rgb_changed();
    void name_changed();
};

#endif // VOXIE_PALETTEEDITOR_HPP
