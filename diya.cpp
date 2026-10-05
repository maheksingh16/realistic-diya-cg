#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

const int W = 900;
const int H = 650;

struct Color
{
    unsigned char r, g, b;
};

Color img[H][W];

// --------------------------------------------------
// PIXEL FUNCTION
// --------------------------------------------------

void setPixel(int x, int y, Color c)
{
    if (x >= 0 && x < W && y >= 0 && y < H)
        img[y][x] = c;
}

// --------------------------------------------------
// COLOR BLENDING
// --------------------------------------------------

Color blend(Color a, Color b, double t)
{
    t = max(0.0, min(1.0, t));

    Color c;

    c.r = (unsigned char)(a.r * (1.0 - t) + b.r * t);
    c.g = (unsigned char)(a.g * (1.0 - t) + b.g * t);
    c.b = (unsigned char)(a.b * (1.0 - t) + b.b * t);

    return c;
}

// --------------------------------------------------
// ELLIPSE - BASIC GEOMETRY
// --------------------------------------------------

void ellipse(int cx, int cy, int rx, int ry, Color color)
{
    for (int y = cy - ry; y <= cy + ry; y++)
    {
        for (int x = cx - rx; x <= cx + rx; x++)
        {
            double dx = (double)(x - cx) / rx;
            double dy = (double)(y - cy) / ry;

            if (dx * dx + dy * dy <= 1.0)
            {
                setPixel(x, y, color);
            }
        }
    }
}

// --------------------------------------------------
// SHADED ELLIPSE
// --------------------------------------------------

void shadedEllipse(int cx, int cy,
                   int rx, int ry,
                   Color topColor,
                   Color bottomColor)
{
    for (int y = cy - ry; y <= cy + ry; y++)
    {
        for (int x = cx - rx; x <= cx + rx; x++)
        {
            double dx = (double)(x - cx) / rx;
            double dy = (double)(y - cy) / ry;

            if (dx * dx + dy * dy <= 1.0)
            {
                double t =
                    (double)(y - (cy - ry)) /
                    (2.0 * ry);

                setPixel(x, y,
                         blend(topColor,
                               bottomColor,
                               t));
            }
        }
    }
}

// --------------------------------------------------
// FLAME SHAPE
// --------------------------------------------------

bool insideFlame(int x, int y,
                 int cx, int cy,
                 int width, int height)
{
    double nx = (double)(x - cx) / width;
    double ny = (double)(y - cy) / height;

    if (ny < -1.0 || ny > 1.0)
        return false;

    double allowedWidth =
        1.0 - (ny + 1.0) / 2.0;

    return fabs(nx) <= allowedWidth &&
           (nx * nx + ny * ny * 0.55 <= 1.0);
}

// --------------------------------------------------
// FLAME
// --------------------------------------------------

void drawFlame(int cx, int cy)
{
    // Outer flame
    for (int y = cy - 110; y <= cy + 15; y++)
    {
        for (int x = cx - 65; x <= cx + 65; x++)
        {
            if (insideFlame(
                    x, y,
                    cx,
                    cy - 45,
                    65,
                    65))
            {
                setPixel(x, y,
                         {255, 100, 15});
            }
        }
    }

    // Inner yellow flame
    for (int y = cy - 85; y <= cy + 10; y++)
    {
        for (int x = cx - 45; x <= cx + 45; x++)
        {
            if (insideFlame(
                    x, y,
                    cx,
                    cy - 35,
                    45,
                    55))
            {
                setPixel(x, y,
                         {255, 210, 35});
            }
        }
    }

    // Bright flame core
    for (int y = cy - 55; y <= cy + 8; y++)
    {
        for (int x = cx - 25; x <= cx + 25; x++)
        {
            if (insideFlame(
                    x, y,
                    cx,
                    cy - 25,
                    25,
                    35))
            {
                setPixel(x, y,
                         {255, 245, 170});
            }
        }
    }
}

// --------------------------------------------------
// FLAME GLOW - LIGHTING
// --------------------------------------------------

void addGlow(int cx, int cy)
{
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            double dx = x - cx;
            double dy = y - cy;

            double distance =
                sqrt(dx * dx + dy * dy);

            if (distance < 190)
            {
                double strength =
                    1.0 - distance / 190.0;

                strength *= 0.20;

                Color glow =
                    {255, 150, 40};

                img[y][x] =
                    blend(img[y][x],
                          glow,
                          strength);
            }
        }
    }
}

// --------------------------------------------------
// SHADOW
// --------------------------------------------------

void drawShadow()
{
    for (int y = 465; y <= 530; y++)
    {
        for (int x = 260; x <= 640; x++)
        {
            double dx =
                (double)(x - 450) / 190;

            double dy =
                (double)(y - 490) / 35;

            if (dx * dx + dy * dy <= 1.0)
            {
                double edge = 1.0 - fabs(dx);

                unsigned char value =
                    (unsigned char)(35 + 25 * edge);

                setPixel(x, y,
                         {value, value, value});
            }
        }
    }
}

// --------------------------------------------------
// DIYA MODEL
// --------------------------------------------------

void drawDiya(int tx, int ty, double scale)
{
    int cx =
        (int)(450 + tx);

    int bodyY =
        (int)(430 + ty);

    int rx =
        (int)(190 * scale);

    int ry =
        (int)(75 * scale);

    // Main clay body
    shadedEllipse(
        cx,
        bodyY,
        rx,
        ry,
        {205, 105, 45},
        {105, 45, 20}
    );

    // Upper rim
    shadedEllipse(
        cx,
        bodyY - (int)(28 * scale),
        (int)(175 * scale),
        (int)(48 * scale),
        {230, 135, 65},
        {125, 55, 25}
    );

    // Oil
    shadedEllipse(
        cx,
        bodyY - (int)(38 * scale),
        (int)(135 * scale),
        (int)(25 * scale),
        {45, 25, 12},
        {15, 10, 5}
    );

    // Oil highlight
    ellipse(
        cx - (int)(35 * scale),
        bodyY - (int)(45 * scale),
        (int)(45 * scale),
        (int)(8 * scale),
        {120, 70, 25}
    );

    // Wick
    int wickX = cx;

    for (int y = bodyY - (int)(80 * scale);
         y <= bodyY - (int)(38 * scale);
         y++)
    {
        for (int x = wickX - (int)(5 * scale);
             x <= wickX + (int)(5 * scale);
             x++)
        {
            setPixel(x, y,
                     {35, 25, 15});
        }
    }

    // Flame
    drawFlame(
        wickX,
        bodyY - (int)(95 * scale)
    );
}

// --------------------------------------------------
// BMP FILE CREATION
// --------------------------------------------------

void writeBMP(const char* filename)
{
    ofstream file(filename, ios::binary);

    int rowSize = W * 3;

    int padding =
        (4 - (rowSize % 4)) % 4;

    int dataSize =
        (rowSize + padding) * H;

    int fileSize =
        54 + dataSize;

    unsigned char header[54] = {0};

    // Signature
    header[0] = 'B';
    header[1] = 'M';

    // File size
    header[2] = fileSize & 0xFF;
    header[3] = (fileSize >> 8) & 0xFF;
    header[4] = (fileSize >> 16) & 0xFF;
    header[5] = (fileSize >> 24) & 0xFF;

    // Pixel data offset
    header[10] = 54;

    // DIB header size
    header[14] = 40;

    // Width
    header[18] = W & 0xFF;
    header[19] = (W >> 8) & 0xFF;
    header[20] = (W >> 16) & 0xFF;
    header[21] = (W >> 24) & 0xFF;

    // Height
    header[22] = H & 0xFF;
    header[23] = (H >> 8) & 0xFF;
    header[24] = (H >> 16) & 0xFF;
    header[25] = (H >> 24) & 0xFF;

    // Color planes
    header[26] = 1;

    // Bits per pixel
    header[28] = 24;

    file.write(
        (char*)header,
        54
    );

    // Pixel data
    for (int y = H - 1; y >= 0; y--)
    {
        for (int x = 0; x < W; x++)
        {
            unsigned char pixel[3];

            pixel[0] = img[y][x].b;
            pixel[1] = img[y][x].g;
            pixel[2] = img[y][x].r;

            file.write(
                (char*)pixel,
                3
            );
        }

        // BMP row padding
        for (int p = 0; p < padding; p++)
        {
            unsigned char zero = 0;

            file.write(
                (char*)&zero,
                1
            );
        }
    }

    file.close();
}

// --------------------------------------------------
// MAIN PROGRAM
// --------------------------------------------------

int main()
{
    // ----------------------------------------------
    // AMBIENT BACKGROUND
    // ----------------------------------------------

    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            double t =
                (double)y / H;

            img[y][x] =
                blend(
                    {18, 12, 28},
                    {65, 35, 25},
                    t
                );
        }
    }

    // ----------------------------------------------
    // GROUND
    // ----------------------------------------------

    for (int y = 470; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            double t =
                (double)(y - 470) /
                (H - 470);

            img[y][x] =
                blend(
                    {45, 28, 25},
                    {20, 15, 18},
                    t
                );
        }
    }

    // ----------------------------------------------
    // SHADOW
    // ----------------------------------------------

    drawShadow();

    // ----------------------------------------------
    // DIYA
    // ----------------------------------------------

    drawDiya(
        0,
        0,
        1.0
    );

    // ----------------------------------------------
    // FLAME LIGHT
    // ----------------------------------------------

    addGlow(
        450,
        320
    );

    // Redraw flame over glow
    drawFlame(
        450,
        335
    );

    // ----------------------------------------------
    // HIGHLIGHT
    // ----------------------------------------------

    ellipse(
        365,
        410,
        55,
        10,
        {240, 150, 75}
    );

    // ----------------------------------------------
    // SAVE IMAGE
    // ----------------------------------------------

    writeBMP(
        "diya.bmp"
    );

    // ----------------------------------------------
    // OUTPUT INFORMATION
    // ----------------------------------------------

    cout << "\n";
    cout << "========================================\n";
    cout << "       REALISTIC DIYA - CG PROJECT\n";
    cout << "========================================\n";

    cout << "Geometry      : Ellipse-based modelling\n";
    cout << "Material      : Clay + Oil + Wick\n";
    cout << "Lighting      : Flame-based glow\n";
    cout << "Shading       : Gradient surface shading\n";
    cout << "Shadow        : Ground shadow\n";
    cout << "Transformation: Position + Scaling\n";
    cout << "Rasterization : Pixel-based rendering\n";

    cout << "\nFinal image created: diya.bmp\n";

    cout << "========================================\n";

    return 0;
}