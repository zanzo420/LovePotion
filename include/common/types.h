#include <string>
#include <vector>
#include <map>

#include "socket/common.h"

typedef struct
{
    double ox;
    double oy;

    double r;

    double sx;
    double sy;

    double kx;
    double ky;
} StackMatrix;

typedef enum
{
    BOOLEAN,
    LUSERDATA,
    NIL,
    NUMBER,
    STRING,
    TABLE,
    UNKNOWN,
    USERDATA
} VARIANTS;

typedef enum
{
    OBJECTS
} REGISTRY;

typedef struct
{
    int x;
    int y;
    int subWidth;
    int subHeight;

    int width;
    int height;
} Viewport;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct
{
    float x;
    float y;
    float z;
} VertexPosition;

typedef struct
{
    float u;
    float v;
} VertexUV;

typedef struct
{
    VertexPosition position;
    Color color;
    VertexUV texCoord;
} Vertex;

extern std::vector<const char *> FILE_MODES;

extern std::vector<const char *> SOURCE_TYPES;

extern std::vector<const char *> DRAW_MODES;