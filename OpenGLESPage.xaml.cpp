#include "pch.h"
#include "OpenGLESPage.xaml.h"
#include "Servo.h"

using namespace hlservo;
using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;

OpenGLESPage::OpenGLESPage()
    : OpenGLESPage(nullptr)
{
}

OpenGLESPage::OpenGLESPage(OpenGLES* openGLES)
    : mOpenGLES(openGLES)
    , mRenderSurface(EGL_NO_SURFACE)
{
    InitializeComponent();
    Windows::UI::Core::CoreWindow ^ window = Windows::UI::Xaml::Window::Current->CoreWindow;
    window->VisibilityChanged += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::VisibilityChangedEventArgs ^>(this, &OpenGLESPage::OnVisibilityChanged);
    this->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLESPage::OnPageLoaded);
}

OpenGLESPage::~OpenGLESPage()
{
    StopRenderLoop();
    DestroyRenderSurface();
}

void OpenGLESPage::OnPageLoaded(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
    CreateRenderSurface();
    StartRenderLoop();
}

void OpenGLESPage::OnVisibilityChanged(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::VisibilityChangedEventArgs ^ args)
{
    if (args->Visible && mRenderSurface != EGL_NO_SURFACE) {
        StartRenderLoop();
    } else {
        StopRenderLoop();
    }
}

void OpenGLESPage::CreateRenderSurface()
{
    if (mOpenGLES && mRenderSurface == EGL_NO_SURFACE) {
        mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel);
    }
}

void OpenGLESPage::DestroyRenderSurface()
{
    if (mOpenGLES) {
        mOpenGLES->DestroySurface(mRenderSurface);
    }
    mRenderSurface = EGL_NO_SURFACE;
}

void OpenGLESPage::RecoverFromLostDevice()
{
    StopRenderLoop();
    {
        critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

        DestroyRenderSurface();
        mOpenGLES->Reset();
        CreateRenderSurface();
    }
    StartRenderLoop();
}

void OpenGLESPage::StartRenderLoop()
{
    if (mRenderLoopWorker != nullptr && mRenderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started) {
        return;
    }

    auto wakeup = []() {

      // perform_updates();
    };

    auto init = [this](Windows::Foundation::IAsyncAction ^ action) {
      critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);
      mOpenGLES->MakeCurrent(mRenderSurface);

      /*
      auto makeCurrent = [this]() {
        mOpenGLES->MakeCurrent(mRenderSurface);
      };

      auto flush = [this]() -> bool {
        return mOpenGLES->SwapBuffers(mRenderSurface);
      };
      */

      EGLint panelWidth = 0;
      EGLint panelHeight = 0;
      mOpenGLES->GetSurfaceDimensions(mRenderSurface, &panelWidth, &panelHeight);
      glViewport(0, 0, panelWidth, panelHeight);

      mServo = new Servo(panelWidth, panelHeight);

      // Interesting: CoreWindow::GetForCurrentThread()->Dispatcher

      while (action->Status == Windows::Foundation::AsyncStatus::Started) {
        // The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
        // If the call fails, then we must reinitialize EGL and the GL resources.
        mOpenGLES->GetSurfaceDimensions(mRenderSurface, &panelWidth, &panelHeight);
        mServo->SetSize(panelWidth, panelHeight);
        mServo->PerformUpdates();
        if (mOpenGLES->SwapBuffers(mRenderSurface) != GL_TRUE) {
          // XAML objects like the SwapChainPanel must only be manipulated on the UI thread.
          swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]() {
            RecoverFromLostDevice();
            }, CallbackContext::Any));
          return;
        }
      }
      
    };

    auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler(init);

    // Run Servo task in a high priority background thread.
    mRenderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(
      workItemHandler,
      Windows::System::Threading::WorkItemPriority::High,
      Windows::System::Threading::WorkItemOptions::TimeSliced);
}

void OpenGLESPage::StopRenderLoop()
{
    if (mRenderLoopWorker) {
        mRenderLoopWorker->Cancel();
        mRenderLoopWorker = nullptr;
    }
}
