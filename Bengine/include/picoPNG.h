#ifndef PICOPNG_H_INCLUDED
#define PICOPNG_H_INCLUDED

#include <vector>

//Remember to use the "extern" keyword when a global function is defined in a different file
namespace Bengine{

    extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

}
#endif // PICOPNG_H_INCLUDED
