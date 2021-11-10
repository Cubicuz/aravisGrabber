#include <arv.h>
#include <iostream>
#include <memory>

struct GLibDeleter {
    void operator()(const char** p) const 
    {
        g_free(p);
    }
};

int main(int, char **) {
    std::cout << "Hello, world!\n";
    arv_update_device_list();
    std::cout << "Number of cameras " << arv_get_n_devices() << std::endl;

    GError* error = nullptr;

    ArvCamera* camera = nullptr;
    camera = arv_camera_new(nullptr, &error);
    if (error){
        g_clear_error(&error);
        return 1;
    }
    guint n_pixel_formats = 0;
    auto formats = arv_camera_dup_available_pixel_formats_as_display_names(camera, &n_pixel_formats, &error);
    if (error){
        g_clear_error(&error);
        return 1;
    }
    auto formatIds = arv_camera_dup_available_pixel_formats(camera, &n_pixel_formats, &error);
    if (error){
        g_clear_error(&error);
        return 1;
    }
    std::cout << "Formats:" << std::endl;
    for (guint i=0;i<n_pixel_formats;i++){
        std::cout << formats[i] << " id: " << formatIds[i] << std::endl;
    }
    g_free(formats);
    g_free(formatIds);
}
