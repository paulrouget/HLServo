#pragma once

#include "pch.h"

namespace hlservo {
class Servo {
public:
    Servo(GLsizei width, GLsizei height);
    ~Servo();
    void PerformUpdates();
    void SetSize(GLsizei width, GLsizei height);

private:
    GLsizei mWindowWidth;
    GLsizei mWindowHeight;
    bool mAnimating;
};
}
