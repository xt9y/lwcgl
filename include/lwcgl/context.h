#ifndef LWCGL_CONTEXT_H
#define LWCGL_CONTEXT_H
#ifdef __cplusplus
extern "C" {
#endif
enum {
    LWCGL_CONTEXT_ANY_PROFILE = 0,
    LWCGL_CONTEXT_CORE_PROFILE = 1,
    LWCGL_CONTEXT_COMPATIBILITY_PROFILE = 2
};
void lwcglSetContextVersion(int major, int minor);
void lwcglSetContextProfile(int profile);
void lwcglSetContextDebug(int enabled);
int lwcglRequestedContextMajorVersion(void);
int lwcglRequestedContextMinorVersion(void);
int lwcglRequestedContextProfile(void);
int lwcglRequestedContextDebug(void);
#ifdef __cplusplus
}
#endif
#endif
