#include "pch.h"
#include "Servo.h"

extern "C" {
#include <simpleservo.h>
}

using namespace hlservo;

static bool dirty = false;

void on_load_started() {}
void on_load_ended() {}
void on_title_changed(const char* foo) {}
void on_url_changed(const char* foo) {}
void on_history_changed(bool back, bool fwd) {}
void on_shutdown_complete() {}


void flush()
{
}

void make_current()
{
}

void wakeup()
{
  dirty = true;
}

void on_animating_changed(bool aAnimating)
{
}


Servo::Servo(GLsizei width, GLsizei height)
  : mAnimating(false)
  , mWindowHeight(height)
  , mWindowWidth(width)
{

  CInitOptions o;
  o.args = NULL;
  o.url = "https://example.com";
  o.width = mWindowWidth;
  o.height = mWindowHeight;
  o.density = 1.0;
  o.enable_subpixel_text_antialiasing = false;

  CHostCallbacks c;
  c.flush = &flush;
  c.make_current = &make_current;
  c.on_load_started = &on_load_started;
  c.on_load_ended = &on_load_ended;
  c.on_title_changed = &on_title_changed;
  c.on_url_changed = &on_url_changed;
  c.on_history_changed = &on_history_changed;
  c.on_animating_changed = &on_animating_changed;
  c.on_shutdown_complete = &on_shutdown_complete;

  init_with_egl(o, &wakeup, c);
}

Servo::~Servo()
{
}

bool Servo::PerformUpdates()
{
  if (dirty) {
    perform_updates();
    dirty = false;
    return true;
  }
  return false;
}

void Servo::SetSize(GLsizei width, GLsizei height)
{
  if (width != mWindowWidth || height != mWindowHeight) {
    mWindowWidth = width;
    mWindowHeight = height;
    resize(mWindowWidth, mWindowHeight);
  }
}
