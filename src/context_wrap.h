#ifndef LWCGL_CONTEXT_WRAP_H
#define LWCGL_CONTEXT_WRAP_H

/*
 * Used only while compiling src/lwcgl.c.  That translation unit includes
 * this before GLFW, so GLFW's own prototype and lwcgl.c's call site are both
 * redirected to the context-aware wrapper implemented in glmodern.c.
 */
#ifdef LWCGL_CONTEXT_WRAP
#define glfwCreateWindow lwcgl_glfwCreateWindow
#endif

#endif
