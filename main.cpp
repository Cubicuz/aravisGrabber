#include <arv.h>
#include <iostream>
#include <memory>

struct GLibDeleter {
    void operator()(const char** p) const 
    {
        g_free(p);
    }
};

bool handleerror(GError* &error){
    if (error)
    {
        std::cout << error->message << std::endl;
        g_clear_error(&error);
        return true;
    }
    return false;
}

int main(int, char **) {
    std::cout << "Hello, world!\n";
    arv_update_device_list();
    std::cout << "Number of cameras " << arv_get_n_devices() << std::endl;

    GError* error = nullptr;

    ArvCamera* camera = nullptr;
    camera = arv_camera_new(nullptr, &error);
    if (handleerror(error)){
        return 1;
    }
    guint n_pixel_formats = 0;
    auto formats = arv_camera_dup_available_pixel_formats_as_display_names(camera, &n_pixel_formats, &error);
    if (handleerror(error)){
        return 1;
    }
    auto formatIds = arv_camera_dup_available_pixel_formats(camera, &n_pixel_formats, &error);
    if (handleerror(error)){
        return 1;
    }
    std::cout << "Formats:" << std::endl;
    for (guint i=0;i<n_pixel_formats;i++){
        std::cout << formats[i] << " id: " << formatIds[i] << std::endl;
    }
    g_free(formats);
    g_free(formatIds);
    arv_camera_set_pixel_format(camera, ARV_PIXEL_FORMAT_BAYER_RG_12, &error);
    if (handleerror(error)){
        return 1;
    }
    {
        double min, max;
        arv_camera_get_frame_rate_bounds(camera, &min, &max, &error);
        if (handleerror(error)){
            return 1;
        }
        std::cout << "framerate between " << min << " and " << max << std::endl;
        arv_camera_set_frame_rate(camera, 1.0, &error);
        if (handleerror(error)){
            return 1;
        }
    }
    ArvStream* stream = nullptr;
    stream = arv_camera_create_stream(camera, [](void *user_data,
                      ArvStreamCallbackType type,
                      ArvBuffer *buffer){}, nullptr, &error);
    


    if (stream){
        g_object_unref(stream);
    }
}
