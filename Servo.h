#pragma once

#include "pch.h"

namespace hlservo {
class Servo {
public:
    Servo(GLsizei width, GLsizei height);
    ~Servo();
    bool PerformUpdates();
    void SetSize(GLsizei width, GLsizei height);

private:
    GLsizei mWindowWidth;
    GLsizei mWindowHeight;
    bool mAnimating;
};
}
