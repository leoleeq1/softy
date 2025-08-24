#include "event/event_channel.h"
#include "event/window_event.h"
#include "render/buffer.h"
#include "render/color.h"
#include "window/window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <memory>

namespace softy {
struct Window::Impl {
  ~Impl() = default;
  static LRESULT CALLBACK s_WndProc(HWND hwnd, uint32_t msg, WPARAM wparam,
                                    LPARAM lparam);

  WindowDescriptor& descriptor;
  EventChannel& channel;
  Buffer& colorBuffer;
  HWND hwnd;
};

Window::Window() = default;
Window::~Window() {
  if (impl_) {
    UnregisterClassA(impl_->descriptor.name.c_str(), nullptr);
  }
}

bool Window::Create(WindowDescriptor& descriptor, EventChannel& channel,
                    Buffer& colorBuffer) {
  impl_ = std::make_unique<Impl>(descriptor, channel, colorBuffer);

  const WNDCLASSEXA wc{
      .cbSize = sizeof(WNDCLASSEXA),
      .style = CS_HREDRAW | CS_VREDRAW,
      .lpfnWndProc = Impl::s_WndProc,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = nullptr,
      .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
      .hCursor = LoadCursor(nullptr, IDC_ARROW),
      .hbrBackground = CreateSolidBrush(BLACK_BRUSH),
      .lpszMenuName = nullptr,
      .lpszClassName = descriptor.name.c_str(),
      .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
  };

  if (!RegisterClassExA(&wc)) {
    return false;
  }

  DWORD style = WS_OVERLAPPEDWINDOW;
  DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
  RECT rc = {0, 0, descriptor.width, descriptor.height};
  AdjustWindowRectEx(&rc, style, FALSE, exStyle);

  impl_->hwnd =
      CreateWindowExA(exStyle, descriptor.name.c_str(), descriptor.name.c_str(),
                      style, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                      nullptr, nullptr, nullptr, impl_.get());
  if (!impl_->hwnd) {
    return false;
  }

  ShowWindow(impl_->hwnd, SW_SHOWDEFAULT);
  return true;
}

bool Window::Update() {
  MSG msg;
  while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      return false;
    }

    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }

  return true;
}

void Window::Present() {
  InvalidateRect(impl_->hwnd, nullptr, false);
  UpdateWindow(impl_->hwnd);
}

void Window::SetTitle(std::string_view title) {
  SetWindowTextA(impl_->hwnd, title.data());
}

LRESULT Window::Impl::s_WndProc(HWND hwnd, uint32_t msg, WPARAM wparam,
                                LPARAM lparam) {
  Impl* impl = reinterpret_cast<Impl*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  switch (msg) {
    case WM_NCCREATE: {
      auto* lpcs = reinterpret_cast<LPCREATESTRUCT>(lparam);
      impl = reinterpret_cast<Impl*>(lpcs->lpCreateParams);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(impl));
      impl->channel.Send(WindowCreatedEvent{});
      break;
    }
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      HDC memdc = CreateCompatibleDC(hdc);
      HBITMAP bitmap = CreateCompatibleBitmap(hdc, impl->descriptor.width,
                                              impl->descriptor.height);
      auto* oldBitmap = reinterpret_cast<HBITMAP>(SelectObject(memdc, bitmap));

      const BITMAPINFO bmi{
          .bmiHeader{
              .biSize = sizeof(BITMAPINFOHEADER),
              .biWidth = impl->descriptor.width,
              .biHeight = impl->descriptor.height,
              .biPlanes = 1,
              .biBitCount = 32,
              .biCompression = BI_RGB,
              .biSizeImage{},
              .biXPelsPerMeter{},
              .biYPelsPerMeter{},
              .biClrUsed{},
              .biClrImportant{},
          },
          .bmiColors{},
      };

      SetDIBitsToDevice(memdc, 0, 0, static_cast<DWORD>(impl->descriptor.width),
                        static_cast<DWORD>(impl->descriptor.height), 0, 0, 0,
                        static_cast<UINT>(impl->descriptor.height),
                        impl->colorBuffer.Get(), &bmi, DIB_RGB_COLORS);
      BitBlt(hdc, 0, 0, impl->descriptor.width, impl->descriptor.height, memdc,
             0, 0, SRCCOPY);
      SelectObject(memdc, oldBitmap);
      DeleteObject(bitmap);
      DeleteDC(memdc);
      EndPaint(hwnd, &ps);
      break;
    }
    case WM_SIZE: {
      if (wparam == SIZE_MINIMIZED) {
      }
      break;
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }
    case WM_CLOSE: {
      break;
    }
    default:
      break;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}
}  // namespace softy