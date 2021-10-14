
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../matrix library/initShader.h"
#include "../matrix library/matrix_def.h"
#include "../matrix library/shapes.h"
#include "../matrix library/file_reader.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// Vertices of a square
vector4 vertices[6] =
{{-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5, -0.5,  0.0, 1.0},	// bottom right
 { 0.5,  0.5,  0.0, 1.0},	// top right
 {-0.5, -0.5,  0.0, 1.0},	// bottom left
 { 0.5,  0.5,  0.0, 1.0},	// top right
 {-0.5,  0.5,  0.0, 1.0}};	// top left

// Colors (not really use this but send it into the pipeline anyway)

vector4 colors[6] =
{{1.0, 0.0, 0.0, 1.0},  // red
 {1.0, 0.0, 0.0, 1.0},  // red
 {1.0, 0.0, 0.0, 1.0},  // red
 {0.0, 1.0, 0.0, 1.0},  // green
 {0.0, 1.0, 0.0, 1.0},  // green
 {0.0, 1.0, 0.0, 1.0}}; // green

int num_vertices = 6;

vector2 tex_coords[6]; //= {{0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}};

void init(void)
{   
    //Stores the texels of the image
    int width = 512;
    int height = 512;
    GLubyte my_texels[width][height][3];

    //Reads the image and puts the RGB into their respective texel
    FILE *fp = fopen("Ollie_Dup.raw", "r");
    load_raw(fp, my_texels, width, height);
    fclose(fp);

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    //Puts texel array onto graphics pipeline
    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    //idk...
    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(tex_coords), tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));

    //Assigns the attribute array vTexCoord to the last chunk of the array (tex_coords)
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));

    //Location of texture, like location of ctm
    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    printf("texture_location: %i\n", texture_location);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
	exit(0);
}

int main(int argc, char **argv)
{
    texturize(tex_coords, 6, RECTANGLE);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Texture Lab");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}