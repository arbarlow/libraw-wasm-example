#include <stdio.h>
#include <emscripten/bind.h>
#include "libraw/libraw.h"

using namespace emscripten;

int result[2];

void process_image(int32_t buffer, size_t bufsize) {
        int err = 0;

        // Let us create an image processor
        LibRaw iProcessor;

        // Open the file and read the metadata
        iProcessor.open_buffer(reinterpret_cast<void *>(buffer), bufsize);

        // Let us unpack the image
        err = iProcessor.unpack();
        if (err != LIBRAW_SUCCESS) {
          printf("unpack failure %s\n", libraw_strerror(err));
        }

        // Convert from imgdata.rawdata to imgdata.image:
        err = iProcessor.raw2image();
        if (err != LIBRAW_SUCCESS) {
          printf("Can't raw2image %s\n", libraw_strerror(err));
        }

#define OUT iProcessor.imgdata.params

        OUT.no_auto_bright = 0;
        OUT.use_auto_wb = 0;
        OUT.use_camera_wb = 1;
        OUT.output_color = 1;
        OUT.user_qual = 3;
        OUT.half_size = 1;
        OUT.output_bps = 8;

        err = iProcessor.dcraw_process();
        if (err != LIBRAW_SUCCESS) {
          printf("Can't dcraw %s\n", libraw_strerror(err));
        }

        printf("%d x %d\n", iProcessor.imgdata.sizes.iwidth, iProcessor.imgdata.sizes.iheight);

        int raw_width, raw_height, raw_color, raw_bitsize;

        iProcessor.get_mem_image_format(&raw_width, &raw_height, &raw_color, &raw_bitsize);
        unsigned char *img_out = new unsigned char[raw_height * raw_width * 3];

        iProcessor.copy_mem_image(img_out, raw_width * 3, 0);

        /* err = iProcessor.copy_mem_image(&img_out, stride, 0); */
        /* if (err != LIBRAW_SUCCESS) { */
        /*   printf("Can't copy image as RGB Bitmap %s\n", libraw_strerror(err)); */
        /* } */

        result[0] = (int)img_out;
        result[1] = iProcessor.imgdata.sizes.iwidth * iProcessor.imgdata.sizes.iheight * 3;

        /* iProcessor.recycle(); */
}

int get_result_pointer() {
  return result[0];
}

int get_result_size() {
  return result[1];
}


EMSCRIPTEN_BINDINGS(libraw) {
    function("process_image", &process_image);
    function("get_result_pointer", &get_result_pointer);
    function("get_result_size", &get_result_size);
}
