#include <lwcgl/lwcgl.h>

static void compile_cpp_surface() {
    auto *memory = static_cast<float *>(MemoryUtil.memAlloc(sizeof(float) * 16));
    MemoryUtil.memFree(memory);

    GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MAJOR, 4);
    GLFW.glfwWindowHint(GLFW.GLFW_CONTEXT_VERSION_MINOR, 6);
    (void)GL46.GL_SPIR_V_BINARY;
    (void)GL43.GL_COMPUTE_SHADER;
    (void)GL45.glCreateBuffers;
}

int main() {
    if (false) compile_cpp_surface();
    return LWCGL.getVersionMajor() == 3 ? 0 : 1;
}
