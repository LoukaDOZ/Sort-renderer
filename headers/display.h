#ifndef DISPLAY_H
#define DISPLAY_H

#include "shared.h" // Fix unknown Shared_data

extern const short BAR_DRAWING;
extern const short DOT_DRAWING;
extern const short CIRCLE_DRAWING;
extern const short SNAIL_DRAWING;

bool run_display(Render* render, Shared_data shared_data, short drawing_mode, bool show_info, bool colorized);

#endif