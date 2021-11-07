#include <iostream>
#include <arv.h>


int main(int, char**) {
    std::cout << "Hello, world!\n";
    arv_update_device_list();
    std::cout << "Number of cameras " << arv_get_n_devices() << std::endl;
}
