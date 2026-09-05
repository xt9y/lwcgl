#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include "internal.h"
#include <stdlib.h>
#include <string.h>

static LWCGLBuffer *create(size_t n, size_t size, LWCGLBufferScalarType type) {
    if (n && size > SIZE_MAX / n) { lwcglSetErrorInternal("buffer size overflow"); return NULL; }
    LWCGLBuffer *b = (LWCGLBuffer *)calloc(1, sizeof *b);
    if (!b) { lwcglSetErrorInternal("out of memory allocating buffer object"); return NULL; }
    b->data = calloc(n ? n : 1u, size);
    if (!b->data) { free(b); lwcglSetErrorInternal("out of memory allocating buffer storage"); return NULL; }
    b->elementSize = size; b->capacity = n; b->limit = n; b->mark = SIZE_MAX; b->scalarType = type;
    return b;
}
static ByteBuffer *byte_buffer(size_t n){return create(n,sizeof(int8_t),LWCGL_BUFFER_BYTE);} 
static ShortBuffer *short_buffer(size_t n){return create(n,sizeof(int16_t),LWCGL_BUFFER_SHORT);} 
static IntBuffer *int_buffer(size_t n){return create(n,sizeof(int32_t),LWCGL_BUFFER_INT);} 
static LongBuffer *long_buffer(size_t n){return create(n,sizeof(int64_t),LWCGL_BUFFER_LONG);} 
static FloatBuffer *float_buffer(size_t n){return create(n,sizeof(float),LWCGL_BUFFER_FLOAT);} 
static DoubleBuffer *double_buffer(size_t n){return create(n,sizeof(double),LWCGL_BUFFER_DOUBLE);} 
static void destroy(LWCGLBuffer *b){if(!b)return;free(b->data);memset(b,0,sizeof *b);free(b);} 
static LWCGLBuffer *clear(LWCGLBuffer *b){if(!b)return NULL;b->position=0;b->limit=b->capacity;b->mark=SIZE_MAX;return b;} 
static LWCGLBuffer *flip(LWCGLBuffer *b){if(!b)return NULL;b->limit=b->position;b->position=0;b->mark=SIZE_MAX;return b;} 
static LWCGLBuffer *rewind_buffer(LWCGLBuffer *b){if(!b)return NULL;b->position=0;b->mark=SIZE_MAX;return b;} 
static LWCGLBuffer *set_limit(LWCGLBuffer *b,size_t v){if(!b||v>b->capacity){lwcglSetErrorInternal("invalid buffer limit");return NULL;}b->limit=v;if(b->position>v)b->position=v;if(b->mark>v)b->mark=SIZE_MAX;return b;} 
static LWCGLBuffer *set_position(LWCGLBuffer *b,size_t v){if(!b||v>b->limit){lwcglSetErrorInternal("invalid buffer position");return NULL;}b->position=v;if(b->mark>v)b->mark=SIZE_MAX;return b;} 
static size_t remaining(const LWCGLBuffer *b){return b&&b->limit>=b->position?b->limit-b->position:0;} 
static LWCGLbool has_remaining(const LWCGLBuffer *b){return remaining(b)?LWCGL_TRUE:LWCGL_FALSE;} 
static void *address(LWCGLBuffer *b){return b&&b->data?(unsigned char*)b->data+b->position*b->elementSize:NULL;} 
static const void *const_address(const LWCGLBuffer *b){return b&&b->data?(const unsigned char*)b->data+b->position*b->elementSize:NULL;} 
static int put(LWCGLBuffer*b,const void*v,size_t s,LWCGLBufferScalarType t,size_t i,LWCGLbool absolute){
    if(!b||!v||b->elementSize!=s||b->scalarType!=t){lwcglSetErrorInternal("buffer type mismatch");return -1;}
    size_t p=absolute?i:b->position;if(p>=b->limit){lwcglSetErrorInternal("buffer overflow");return -1;}
    memcpy((unsigned char*)b->data+p*s,v,s);if(!absolute)b->position++;return 0;
}
static int get(const LWCGLBuffer*b,void*v,size_t s,LWCGLBufferScalarType t,size_t i){
    if(!b||!v||b->elementSize!=s||b->scalarType!=t){lwcglSetErrorInternal("buffer type mismatch");return -1;}
    if(i>=b->limit){lwcglSetErrorInternal("buffer underflow");return -1;}memcpy(v,(const unsigned char*)b->data+i*s,s);return 0;
}
static int put_byte(ByteBuffer*b,int8_t v){return put(b,&v,sizeof v,LWCGL_BUFFER_BYTE,0,LWCGL_FALSE);} 
static int put_int(IntBuffer*b,int32_t v){return put(b,&v,sizeof v,LWCGL_BUFFER_INT,0,LWCGL_FALSE);} 
static int put_int_at(IntBuffer*b,size_t i,int32_t v){return put(b,&v,sizeof v,LWCGL_BUFFER_INT,i,LWCGL_TRUE);} 
static int32_t get_int(IntBuffer*b){int32_t v=0;if(b&&get(b,&v,sizeof v,LWCGL_BUFFER_INT,b->position)==0)b->position++;return v;} 
static int32_t get_int_at(const IntBuffer*b,size_t i){int32_t v=0;(void)get(b,&v,sizeof v,LWCGL_BUFFER_INT,i);return v;} 
static int put_float(FloatBuffer*b,float v){return put(b,&v,sizeof v,LWCGL_BUFFER_FLOAT,0,LWCGL_FALSE);} 
static int put_float_at(FloatBuffer*b,size_t i,float v){return put(b,&v,sizeof v,LWCGL_BUFFER_FLOAT,i,LWCGL_TRUE);} 
static float get_float(FloatBuffer*b){float v=0;if(b&&get(b,&v,sizeof v,LWCGL_BUFFER_FLOAT,b->position)==0)b->position++;return v;} 
static float get_float_at(const FloatBuffer*b,size_t i){float v=0;(void)get(b,&v,sizeof v,LWCGL_BUFFER_FLOAT,i);return v;} 
const BufferUtilsAPI BufferUtils={byte_buffer,short_buffer,int_buffer,long_buffer,float_buffer,double_buffer,destroy,sizeof(BufferUtilsAPI),LWCGL_ABI_VERSION};
const BufferAPI Buffer={clear,flip,rewind_buffer,set_limit,set_position,remaining,has_remaining,address,const_address,put_byte,put_int,put_int_at,get_int,get_int_at,put_float,put_float_at,get_float,get_float_at,sizeof(BufferAPI),LWCGL_ABI_VERSION};
