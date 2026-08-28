#include <cbuild.h>

static void lwcgl_compile_settings(C_Target *target) {
    c_include(target, "include");
    c_define(target, "_POSIX_C_SOURCE=200809L");
    c_flag(target, "-Wall");
    c_flag(target, "-Wextra");

#ifdef __APPLE__
    /* Homebrew uses /opt/homebrew on Apple Silicon and /usr/local on Intel. */
    c_include(target, "/opt/homebrew/include");
    c_include(target, "/usr/local/include");
#endif
}

static void lwcgl_link_platform(C_Target *target) {
#ifdef __APPLE__
    c_link_flag(target, "-L/opt/homebrew/lib");
    c_link_flag(target, "-L/usr/local/lib");
    c_link_system(target, "glfw");
    c_framework(target, "Cocoa");
    c_framework(target, "IOKit");
    c_framework(target, "CoreVideo");
#else
    c_link_system(target, "glfw");
    c_link_system(target, "m");
    c_link_system(target, "dl");
    c_link_system(target, "pthread");
#endif
}

static C_Target *lwcgl_test(C_Build *b, const char *name, const char *source) {
    C_Target *test = c_test(b, name);
    c_sources(test, source);

    /*
     * Build the implementation into contract targets directly. C-BuildSystem
     * currently orders system libraries before linked static target archives on
     * GNU ld, which drops GLFW before liblwcgl.a is scanned. The real static
     * library remains the default build artifact; contracts exercise the exact
     * same source files while keeping Linux and macOS behavior identical.
     */
    c_sources(test, "src/*.c");

    lwcgl_compile_settings(test);
    lwcgl_link_platform(test);
    return test;
}

void build(C_Build *b) {
    C_Target *lwcgl = c_static_library(b, "liblwcgl");
    c_sources(lwcgl, "src/*.c");
    c_standard(lwcgl, C_STANDARD_C11);
    lwcgl_compile_settings(lwcgl);
    c_default_target(b, lwcgl);

    C_Target *version = lwcgl_test(
        b,
        "version-contract",
        "tests/version_contract.c"
    );
    c_standard(version, C_STANDARD_C11);

    C_Target *contract_c = lwcgl_test(
        b,
        "lwjgl3-contract-c",
        "tests/lwjgl3_contract.c"
    );
    c_standard(contract_c, C_STANDARD_C11);

    C_Target *contract_cpp = lwcgl_test(
        b,
        "lwjgl3-contract-cpp",
        "tests/lwjgl3_contract.cpp"
    );
    c_flag(contract_cpp, "-std=c++17");
}
