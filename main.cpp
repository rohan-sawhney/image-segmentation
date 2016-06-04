#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "SegmentManager.h"

const std::string path = "/Users/rohansawhney/Desktop/developer/C++/image-segmentation/rendering.png";

SegmentManager segmentManager;
Image input;
Image output;
float range;
int gridX;
int gridY;
bool didSegment = false;
double sigma = 1.0;
double k = 1250;

void printInstructions()
{
    std::cerr << "' ': segment\n"
              << "→/←: increase/decrease threshold\n"
              << "↑/↓: increase/decrease blur coefficient sigma\n"
              << "r: reload image\n"
              << "escape: exit program\n"
              << std::endl;
}

void loadImage()
{
    input.read(path);
    range = pow(2, input.modulusDepth()) * 256.0;
    gridX = (int)input.columns();
    gridY = (int)input.rows();
    didSegment = false;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, gridX, gridY, 0.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
    
    // draw image
    const Image& image(didSegment ? output : input);
    for (size_t i = 0; i < image.columns(); i++) {
        for (size_t j = 0; j < image.rows(); j++) {
            const Color& color(image.pixelColor(i, j));
            
            glColor4f(color.redQuantum() / range, color.greenQuantum() / range,
                      color.blueQuantum() / range, color.alphaQuantum() / range);
            glVertex2f(i, j);
        }
    }
    
    glEnd();
    
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key) {
        case 27 :
            exit(0);
        case ' ':
            if (!didSegment) {
                int segments = segmentManager.segment(output, input, sigma, k);
                didSegment = true;
                std::cout << "segments: " << segments << std::endl;
            }
            break;
        case 'r':
            loadImage();
            break;
    }
    
    glutPostRedisplay();
}

void special(int i, int x0, int y0)
{
    switch (i) {
        case GLUT_KEY_UP:
            sigma += 0.5;
            break;
        case GLUT_KEY_DOWN:
            if (sigma > 0.5) sigma -= 0.5;
            break;
        case GLUT_KEY_LEFT:
            if (k > 50) k -= 50;
            break;
        case GLUT_KEY_RIGHT:
            k += 50;
            break;
    }
    
    std::stringstream title;
    title << "Image Segmentation, sigma: " << sigma << " threshold: " << k;
    glutSetWindowTitle(title.str().c_str());
}

int main(int argc, char** argv)
{
    printInstructions();
    InitializeMagick(*argv);
    loadImage();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(gridX, gridY);
    std::stringstream title;
    title << "Image Segmentation, sigma: " << sigma << " threshold: " << k;
    glutCreateWindow(title.str().c_str());
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    
    return 0;
}
