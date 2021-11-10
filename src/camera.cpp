#include "camera.hpp"
#include <libpng16/png.h>

namespace aravisgrabber {
Camera::Camera() {

    camera = arv_camera_new(nullptr, nullptr);
}
Camera::Camera(ArvDevice *device) {
    camera = arv_camera_new_with_device(device, nullptr);
}
void Camera::SetBufferReceiver(decltype(bufferReceiver) bufferReceiver) {
    this->bufferReceiver = bufferReceiver;
}
void Camera::start() {
}

void Camera::stop() {}
} // namespace aravisgrabber