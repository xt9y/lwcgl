#define LWCGL_IMPLEMENTATION
#include <lwcgl/lwcgl.h>
#include <stdint.h>
#include <limits.h>

static size_t remaining(const LWCGLBuffer *b) { return b && b->limit >= b->position ? b->limit - b->position : 0; }
static LWCGLbool has(const LWCGLBuffer *b, LWCGLBufferScalarType type, size_t n) {
    return b && b->data && b->scalarType == type && b->position <= b->limit && remaining(b) >= n;
}
static const void *ptr(const LWCGLBuffer *b) {
    if (!b || !b->data || b->position > b->limit) return NULL;
    return (const unsigned char *)b->data + b->position * b->elementSize;
}
static GLenum gl_type(const LWCGLBuffer *b) {
    if (!b || !ptr(b)) return 0;
    switch (b->scalarType) {
        case LWCGL_BUFFER_BYTE: return GL_BYTE;
        case LWCGL_BUFFER_SHORT: return GL_SHORT;
        case LWCGL_BUFFER_INT: return GL_INT;
        case LWCGL_BUFFER_FLOAT: return GL_FLOAT;
        case LWCGL_BUFFER_DOUBLE: return GL_DOUBLE;
        default: return 0;
    }
}
static size_t integer_count(GLenum pname) {
    switch (pname) {
#ifdef GL_VIEWPORT
        case GL_VIEWPORT:
#endif
#ifdef GL_SCISSOR_BOX
        case GL_SCISSOR_BOX:
#endif
#ifdef GL_COLOR_WRITEMASK
        case GL_COLOR_WRITEMASK:
#endif
            return 4;
#ifdef GL_MAX_VIEWPORT_DIMS
        case GL_MAX_VIEWPORT_DIMS:
#endif
#ifdef GL_POLYGON_MODE
        case GL_POLYGON_MODE:
#endif
            return 2;
        default: return 1;
    }
}
static LWCGLbool add_size(size_t a,size_t b,size_t *o){if(!o||a>SIZE_MAX-b)return LWCGL_FALSE;*o=a+b;return LWCGL_TRUE;}
static LWCGLbool mul_size(size_t a,size_t b,size_t *o){if(!o||(a&&b>SIZE_MAX/a))return LWCGL_FALSE;*o=a*b;return LWCGL_TRUE;}
static LWCGLbool components(GLenum f,size_t *c){
    switch(f){
        case GL_COLOR_INDEX: case GL_STENCIL_INDEX: case GL_DEPTH_COMPONENT: case GL_RED: case GL_GREEN: case GL_BLUE:
        case GL_ALPHA: case GL_LUMINANCE:*c=1;return LWCGL_TRUE;
        case GL_LUMINANCE_ALPHA:*c=2;return LWCGL_TRUE;
        case GL_RGB:*c=3;return LWCGL_TRUE;
        case GL_RGBA:*c=4;return LWCGL_TRUE;
        default:return LWCGL_FALSE;
    }
}
static LWCGLbool pixel_size(GLenum format,GLenum type,size_t *bytes){
    size_t c=0,s=0;if(!components(format,&c))return LWCGL_FALSE;
    switch(type){case GL_BYTE:case GL_UNSIGNED_BYTE:s=1;break;case GL_SHORT:case GL_UNSIGNED_SHORT:s=2;break;
        case GL_INT:case GL_UNSIGNED_INT:case GL_FLOAT:s=4;break;default:return LWCGL_FALSE;}
    return mul_size(c,s,bytes);
}
static LWCGLbool image_bytes(GLsizei width,GLsizei height,GLenum format,GLenum type,size_t *required){
    GLint alignment=4,row_length=0,skip_rows=0,skip_pixels=0; size_t px,rowpx,rowbytes,stride,start_rows,start_px,start,prior,last,tmp;
    if(!required||width<=0||height<=0||!pixel_size(format,type,&px))return LWCGL_FALSE;
    glGetIntegerv(GL_UNPACK_ALIGNMENT,&alignment); glGetIntegerv(GL_UNPACK_ROW_LENGTH,&row_length);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS,&skip_rows); glGetIntegerv(GL_UNPACK_SKIP_PIXELS,&skip_pixels);
    if((alignment!=1&&alignment!=2&&alignment!=4&&alignment!=8)||row_length<0||skip_rows<0||skip_pixels<0)return LWCGL_FALSE;
    rowpx=row_length?(size_t)row_length:(size_t)width;
    if(!mul_size(rowpx,px,&rowbytes)||!add_size(rowbytes,(size_t)alignment-1,&tmp))return LWCGL_FALSE;
    stride=(tmp/(size_t)alignment)*(size_t)alignment;
    if(!mul_size((size_t)skip_rows,stride,&start_rows)||!mul_size((size_t)skip_pixels,px,&start_px)||!add_size(start_rows,start_px,&start)||
       !mul_size((size_t)(height-1),stride,&prior)||!mul_size((size_t)width,px,&last)||!add_size(start,prior,&tmp)||!add_size(tmp,last,required))return LWCGL_FALSE;
    return LWCGL_TRUE;
}
void lwcgl_glFog(GLenum pname,const LWCGLBuffer *p){size_t n=pname==GL_FOG_COLOR?4:1;if(has(p,LWCGL_BUFFER_FLOAT,n))glFogfv(pname,(const GLfloat*)ptr(p));else if(has(p,LWCGL_BUFFER_INT,n))glFogiv(pname,(const GLint*)ptr(p));}
int lwcgl_glGetIntegerValue(GLenum pname){GLint v[16]={0};glGetIntegerv(pname,v);return v[0];}
void lwcgl_glGetIntegerBuffer(GLenum pname,IntBuffer *p){size_t n=integer_count(pname);if(!has(p,LWCGL_BUFFER_INT,n))return;glGetIntegerv(pname,(GLint*)Buffer.address(p));}
void lwcgl_glSelectBuffer(IntBuffer *b){if(!has(b,LWCGL_BUFFER_INT,1)||remaining(b)>(size_t)INT_MAX)return;glSelectBuffer((GLsizei)remaining(b),(GLuint*)Buffer.address(b));}
GLuint lwcgl_glGenTexture(void){GLuint t=0;glGenTextures(1,&t);return t;}
void lwcgl_glGenTexturesBuffer(IntBuffer *b){if(!has(b,LWCGL_BUFFER_INT,1)||remaining(b)>(size_t)INT_MAX)return;glGenTextures((GLsizei)remaining(b),(GLuint*)Buffer.address(b));}
void lwcgl_glDeleteTexture(GLuint t){glDeleteTextures(1,&t);}
void lwcgl_glVertexPointer3(GLint size,GLsizei stride,const LWCGLBuffer *p){GLenum t=gl_type(p);if(!t||p->scalarType==LWCGL_BUFFER_BYTE||p->scalarType==LWCGL_BUFFER_LONG)return;glVertexPointer(size,t,stride,ptr(p));}
void lwcgl_glVertexPointer4(GLint size,GLenum type,GLsizei stride,const ByteBuffer *p){if(!has(p,LWCGL_BUFFER_BYTE,1))return;glVertexPointer(size,type,stride,ptr(p));}
void lwcgl_glTexCoordPointer3(GLint size,GLsizei stride,const LWCGLBuffer *p){GLenum t=gl_type(p);if(!t||p->scalarType==LWCGL_BUFFER_BYTE||p->scalarType==LWCGL_BUFFER_LONG)return;glTexCoordPointer(size,t,stride,ptr(p));}
void lwcgl_glTexCoordPointer4(GLint size,GLenum type,GLsizei stride,const ByteBuffer *p){if(!has(p,LWCGL_BUFFER_BYTE,1))return;glTexCoordPointer(size,type,stride,ptr(p));}
void lwcgl_glColorPointer3(GLint size,GLsizei stride,const LWCGLBuffer *p){GLenum t=gl_type(p);if(!t||!(p->scalarType==LWCGL_BUFFER_FLOAT||p->scalarType==LWCGL_BUFFER_DOUBLE))return;glColorPointer(size,t,stride,ptr(p));}
void lwcgl_glColorPointer4(GLint size,int type_or_unsigned,GLsizei stride,const ByteBuffer *p){if(!has(p,LWCGL_BUFFER_BYTE,1))return;GLenum t=type_or_unsigned==LWCGL_FALSE?GL_BYTE:type_or_unsigned==LWCGL_TRUE?GL_UNSIGNED_BYTE:(GLenum)type_or_unsigned;glColorPointer(size,t,stride,ptr(p));}
void lwcgl_glLoadMatrixBuffer(const LWCGLBuffer *m){if(has(m,LWCGL_BUFFER_FLOAT,16))glLoadMatrixf((const GLfloat*)ptr(m));else if(has(m,LWCGL_BUFFER_DOUBLE,16))glLoadMatrixd((const GLdouble*)ptr(m));}
void lwcgl_glMultMatrixBuffer(const LWCGLBuffer *m){if(has(m,LWCGL_BUFFER_FLOAT,16))glMultMatrixf((const GLfloat*)ptr(m));else if(has(m,LWCGL_BUFFER_DOUBLE,16))glMultMatrixd((const GLdouble*)ptr(m));}
void lwcgl_gluPickMatrix(GLdouble x,GLdouble y,GLdouble w,GLdouble h,const IntBuffer *v){if(has(v,LWCGL_BUFFER_INT,4))gluPickMatrix(x,y,w,h,(const GLint*)ptr(v));}
GLint lwcgl_gluBuild2DMipmaps(GLenum target,GLint comps,GLsizei w,GLsizei h,GLenum format,GLenum type,const ByteBuffer *data){size_t req=0;if(w<=0||h<=0)return GLU_INVALID_VALUE;if(!data||data->scalarType!=LWCGL_BUFFER_BYTE||!image_bytes(w,h,format,type,&req))return GLU_INVALID_ENUM;if(req>remaining(data))return GLU_INVALID_VALUE;return gluBuild2DMipmaps(target,comps,w,h,format,type,ptr(data));}
