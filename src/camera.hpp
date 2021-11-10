#pragma once

#include <arv.h>
#include <memory>

#include "bufferReceiver.hpp"

namespace aravisgrabber {
class Camera {
    private:
    ArvCamera *camera;
    std::shared_ptr<BufferReceiver> bufferReceiver;

    public:
    Camera();
    Camera(ArvDevice *device);
    Camera(Camera& camera) = delete;
    void SetBufferReceiver(decltype(bufferReceiver) bufferReceiver);
    void start();
    void stop();


};
} // namespace aravisgrabber