Fl_Knob -- A Knob widget for FLTK

Fl_Knob is Copyright 2000 by Yves Usson. Fl_Knob is provided under the terms of the GNU General Public License and GNU Library General Public License.

---

Fl_Knob is a widget attempting to provide a "reallistic" rendering of a potentiometer knob. It displays shiny hilites and casts a shadow... 
It inherits from the Fl_Valuator class and offers the following features :

selectectable cursor shape : DOT or LINE
selectable scale-tick type : LIN or LOG
number of scale ticks (only for LIN)
size of the cursor
The type of the knob is set with the method type(int ty) where ty can be :
Fl_Knob::DOTLIN -> Dot cursor with linear scale ticks
Fl_Knob::DOTLOG1 -> Dot cursor with log scale ticks over one decade
Fl_Knob::DOTLOG2 -> Dot cursor with log scale ticks over two decades
Fl_Knob::DOTLOG3 -> Dot cursor with log scale ticks over three decades
Fl_Knob::LINELIN -> Line cursor with linear scale ticks
Fl_Knob::LINELOG1 -> Line cursor with log scale ticks over one decade
Fl_Knob::LINELOG2 -> Line cursor with log scale ticks over two decades
Fl_Knob::LINELOG3 -> Line cursor with log scale ticks over three decades
The number of scale ticks can be set with the method scaleticks(const int tick)
where tick is an integer from 0 to 31. This is only operating for knob types
Fl_Knob::DOTLIN and Fl_Knob::LINELIN
The size of the cursor is set with the method cursor(const int pc) where pc is an integer from 0 to 100. pc is the percentage of the knob radius occupied by the cursor.
