#include "../../util/util.h"

#define WIDTH  96.0f
#define HEIGHT 128.0f
#define create_rect(x, y, w, h) \
    {(f32)x/WIDTH, (f32)y/HEIGHT, (f32)(x+w)/WIDTH, (f32)(y+h)/HEIGHT}

static struct {
    f32 x1, y1, x2, y2;
} pubbles_frames[10] = {
    create_rect(0, 0, 32, 32),
    create_rect(32, 0, 32, 32),
    create_rect(64, 0, 32, 32),
    create_rect(0, 32, 32, 32),
    create_rect(32, 32, 32, 32),
    create_rect(64, 32, 32, 32),
    create_rect(0, 64, 32, 32),
    create_rect(32, 64, 32, 32),
    create_rect(64, 64, 32, 32),
    create_rect(0, 96, 32, 32),
};

#undef WIDTH
#undef HEIGHT
#undef create_rect