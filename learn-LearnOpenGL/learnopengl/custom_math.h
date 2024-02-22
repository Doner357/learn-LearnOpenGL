#ifndef LEARN_OPENGL_CUSTOM_MATH
#define LEARN_OPENGL_CUSTOM_MATH

namespace CustomHelper {

    // Lerp function
    inline float lerp(float a, float b, float f) {
        return a + f * (b - a);
    }

} // CustomHelper


#endif // !LEARN_OPENGL_CUSTOM_MATH
