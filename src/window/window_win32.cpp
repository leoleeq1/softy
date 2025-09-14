#include "event/event_channel.h"
#include "event/window_event.h"
#include "input/keycode.h"
#include "render/buffer.h"
#include "render/color.h"
#include "window/window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <memory>

namespace softy {
static KeyCode GetKeyCode(WPARAM virtualKey);

struct Window::Impl {
  ~Impl() = default;
  static LRESULT CALLBACK s_WndProc(HWND hwnd, uint32_t msg, WPARAM wparam,
                                    LPARAM lparam);

  WindowDescriptor& descriptor;
  EventChannel& channel;
  ColorBuffer& colorBuffer;
  HWND hwnd;
};

Window::Window() = default;
Window::~Window() {
  if (impl_) {
    UnregisterClassA(impl_->descriptor.name.c_str(), nullptr);
  }
}

bool Window::Create(WindowDescriptor& descriptor, EventChannel& channel,
                    ColorBuffer& colorBuffer) {
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
      HBITMAP bitmap = CreateCompatibleBitmap(hdc, impl->colorBuffer.GetWidth(),
                                              impl->colorBuffer.GetHeight());
      auto* oldBitmap = reinterpret_cast<HBITMAP>(SelectObject(memdc, bitmap));

      const BITMAPINFO bmi{
          .bmiHeader{
              .biSize = sizeof(BITMAPINFOHEADER),
              .biWidth = impl->colorBuffer.GetWidth(),
              .biHeight = impl->colorBuffer.GetHeight(),
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

      SetDIBitsToDevice(
          memdc, 0, 0, static_cast<DWORD>(impl->colorBuffer.GetWidth()),
          static_cast<DWORD>(impl->colorBuffer.GetHeight()), 0, 0, 0,
          static_cast<UINT>(impl->colorBuffer.GetHeight()),
          impl->colorBuffer.GetData().Get(), &bmi, DIB_RGB_COLORS);
      StretchBlt(hdc, 0, 0, impl->descriptor.width, impl->descriptor.height,
                 memdc, 0, 0, impl->colorBuffer.GetWidth(),
                 impl->colorBuffer.GetHeight(), SRCCOPY);
      SelectObject(memdc, oldBitmap);
      DeleteObject(bitmap);
      DeleteDC(memdc);
      EndPaint(hwnd, &ps);
      break;
    }
    case WM_SIZE: {
      if (wparam == SIZE_MINIMIZED) {
      } else {
        impl->descriptor.width = LOWORD(lparam);
        impl->descriptor.height = HIWORD(lparam);
      }
      break;
    }
    case WM_KEYDOWN: {
      KeyCode key = GetKeyCode(wparam);
      impl->channel.Send(WindowKeyDownEvent{key});
      break;
    }
    case WM_KEYUP: {
      KeyCode key = GetKeyCode(wparam);
      impl->channel.Send(WindowKeyUpEvent{key});
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

static KeyCode GetKeyCode(WPARAM virtualKey) {
  switch (virtualKey) {
    case VK_LBUTTON:
      return KeyCode::Mouse0;
    case VK_RBUTTON:
      return KeyCode::Mouse1;
    case VK_MBUTTON:
      return KeyCode::Mouse2;
    case VK_BACK:
      return KeyCode::Backspace;
    case VK_TAB:
      return KeyCode::Tab;
    case VK_RETURN:
      return KeyCode::Return;
    case VK_SPACE:
      return KeyCode::Space;
    case VK_PRIOR:
      return KeyCode::PageUp;
    case VK_NEXT:
      return KeyCode::PageDown;
    case VK_END:
      return KeyCode::End;
    case VK_HOME:
      return KeyCode::Home;
    case VK_LEFT:
      return KeyCode::ArrowLeft;
    case VK_UP:
      return KeyCode::ArrowUp;
    case VK_RIGHT:
      return KeyCode::ArrowRight;
    case VK_DOWN:
      return KeyCode::ArrowDown;
    case '0':
      return KeyCode::Alpha0;
    case '1':
      return KeyCode::Alpha1;
    case '2':
      return KeyCode::Alpha2;
    case '3':
      return KeyCode::Alpha3;
    case '4':
      return KeyCode::Alpha4;
    case '5':
      return KeyCode::Alpha5;
    case '6':
      return KeyCode::Alpha6;
    case '7':
      return KeyCode::Alpha7;
    case '8':
      return KeyCode::Alpha8;
    case '9':
      return KeyCode::Alpha9;
    case 'A':
      return KeyCode::A;
    case 'B':
      return KeyCode::B;
    case 'C':
      return KeyCode::C;
    case 'D':
      return KeyCode::D;
    case 'E':
      return KeyCode::E;
    case 'F':
      return KeyCode::F;
    case 'G':
      return KeyCode::G;
    case 'H':
      return KeyCode::H;
    case 'I':
      return KeyCode::I;
    case 'J':
      return KeyCode::J;
    case 'K':
      return KeyCode::K;
    case 'L':
      return KeyCode::L;
    case 'M':
      return KeyCode::M;
    case 'N':
      return KeyCode::N;
    case 'O':
      return KeyCode::O;
    case 'P':
      return KeyCode::P;
    case 'Q':
      return KeyCode::Q;
    case 'R':
      return KeyCode::R;
    case 'S':
      return KeyCode::S;
    case 'T':
      return KeyCode::T;
    case 'U':
      return KeyCode::U;
    case 'V':
      return KeyCode::V;
    case 'W':
      return KeyCode::W;
    case 'X':
      return KeyCode::X;
    case 'Y':
      return KeyCode::Y;
    case 'Z':
      return KeyCode::Z;
    case VK_NUMPAD0:
      return KeyCode::Keypad0;
    case VK_NUMPAD1:
      return KeyCode::Keypad1;
    case VK_NUMPAD2:
      return KeyCode::Keypad2;
    case VK_NUMPAD3:
      return KeyCode::Keypad3;
    case VK_NUMPAD4:
      return KeyCode::Keypad4;
    case VK_NUMPAD5:
      return KeyCode::Keypad5;
    case VK_NUMPAD6:
      return KeyCode::Keypad6;
    case VK_NUMPAD7:
      return KeyCode::Keypad7;
    case VK_NUMPAD8:
      return KeyCode::Keypad8;
    case VK_NUMPAD9:
      return KeyCode::Keypad9;
    case VK_F1:
      return KeyCode::F1;
    case VK_F2:
      return KeyCode::F2;
    case VK_F4:
      return KeyCode::F4;
    case VK_F5:
      return KeyCode::F5;
    case VK_F6:
      return KeyCode::F6;
    case VK_F7:
      return KeyCode::F7;
    case VK_F8:
      return KeyCode::F8;
    case VK_F9:
      return KeyCode::F9;
    case VK_F10:
      return KeyCode::F10;
    case VK_F11:
      return KeyCode::F11;
    case VK_F12:
      return KeyCode::F12;
    case VK_LSHIFT:
      return KeyCode::LeftShift;
    case VK_LCONTROL:
      return KeyCode::LeftCtrl;
    case VK_LMENU:
      return KeyCode::LeftAlt;
    case VK_RSHIFT:
      return KeyCode::RightShift;
    case VK_RCONTROL:
      return KeyCode::RightCtrl;
    case VK_RMENU:
      return KeyCode::RightAlt;
    default:
      return KeyCode::None;
  };
}
}  // namespace softy