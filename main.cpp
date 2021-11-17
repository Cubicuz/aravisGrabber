#include <arv.h>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>

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
    arv_camera_set_pixel_format(camera, ARV_PIXEL_FORMAT_BAYER_RG_8, &error);
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
            std::cout << "could not set framerate" << std::endl;
        }
    }

    arv_camera_set_region(camera, 0, 0, 1600, 1200, &error);
    if (handleerror(error)){
        std::cout << "invalid region" << std::endl;
        return 1;
    }

    {
        double min, max;
        arv_camera_get_exposure_time_bounds(camera, &min, &max, &error);
        if (handleerror(error)){
            std::cout << "cant read exposure bounds!" << std::endl;
            return -1;
        }
        arv_camera_set_exposure_time(camera, max, &error);
        if (handleerror(error)){
            std::cout << "invalid exposure" << std::endl;
            return 1;
        }
    }

   

    ArvStream* stream = nullptr;
    stream = arv_camera_create_stream(camera, [](void *user_data,
                      ArvStreamCallbackType type,
                      ArvBuffer *buffer){}, nullptr, &error);
    
    guint buffersize = arv_camera_get_payload(camera, &error);

    ArvBuffer* buffer = arv_buffer_new_allocate(buffersize);
    std::cout << "allocated buffer at address " << buffer << std::endl;

    arv_stream_push_buffer(stream, buffer);

    arv_camera_start_acquisition(camera, &error);
    if (handleerror(error)){
        return 1;
    }

    ArvBuffer* popedBuffer = arv_stream_pop_buffer(stream);
    std::cout << "received buffer at address " << popedBuffer << std::endl;

    arv_camera_stop_acquisition(camera, &error);
    if (handleerror(error)){
        return 1;
    }

    gint x(0), y(0), height(0), width(0);
    arv_buffer_get_image_region(popedBuffer, &x, &y, &width, &height);
    if (handleerror(error)){
        return 1;
    }
    std::cout << "heigh: " << height << " width: " << width << std::endl;
    std::cout << "y: " << y << " x: " << x << std::endl;


    cv::Mat imageRaw ((int) height,(int) width, CV_8UC1);
    size_t imageSize = 0;
    auto rawPtr = arv_buffer_get_data(popedBuffer, &imageSize);
    memcpy(imageRaw.data, rawPtr, imageSize);
    std::cout << "copied " << imageSize << " bytes" << std::endl;

    cv::Mat rgb8BitMat(height, width, CV_8UC3);
    cv::cvtColor(imageRaw, rgb8BitMat, cv::COLOR_BayerRG2BGR);


    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image", rgb8BitMat);

    cv::imwrite("/home/m/Pictures/grabber/test.tiff", imageRaw);

    cv::waitKey();

    if (stream){
        g_object_unref(stream);
    }
}
