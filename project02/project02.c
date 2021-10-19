/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../Catorce/matrix_library/initShader.h"
#include "../Catorce/matrix_library/matrix_def.h"
#include "../Catorce/matrix_library/matrix_utility.h"
#include "../Catorce/matrix_library/matrix_ops.h"
#include "../Catorce/shapes_library/shapes.h"
#include "../Catorce/shapes_library/affine.h"
#include "../Catorce/other/file_reader.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

//ctm for manipulating shapes
mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location;

//Texels for texture
GLubyte* texels;
int width = 0, height = 0;

//Vertex Attributes
vector4* vertices;
vector4* colors;
vector2* texcoords;

int num_vertices = 3000;

FILE* fp = NULL;

void init(void)
{
    //Stores the texels of the image
    width = 512;
    height = 512;
    texels = (GLubyte*) malloc(sizeof(GLubyte) * width * height * 3);

    //Reads the image and puts the RGB into their respective texel
    fp = fopen("Ollie_Dup.raw", "r");
    load_raw(fp, texels, width, height);
    fclose(fp);

    //Load the vertex and fragment shaders
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    //Puts texel array onto graphics pipeline
    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    //idk...
    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    //Use this to transport between here and graphics pipeline
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Set aside memory on Graphics Card
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices + sizeof(vector2) * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vector4) * num_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * num_vertices, sizeof(vector4) * num_vertices, colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices, sizeof(vector2) * num_vertices, texcoords);

    //Use this for passing the position info into the vertex and fragment shaders
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    //Use this for passing the color info into the vertex and fragment shaders
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * num_vertices));

    //Use this for passing the texture coordinate info into the vertex and fragment shaders
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * 2 * num_vertices + 0));

    //Locate and use transformation matrix ctm
    ctm_location = glGetUniformLocation(program, "ctm");

    //Location of texture, like location of ctm
    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);


    printf("\ntexture_location: %i\n", texture_location);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    //Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

//Handles menu
void menu(char* dec) {

    printf("\n===================== PROJECT 1 =====================\n");
    printf("\nWould you like to view a file (1) or a computer-generated object(2)? ");
    int ui = 0;
    if(scanf("%d", &ui) == EOF) return;

    if(ui != 2 && ui != 1){printf("\nINVALID OPTION!!! "); exit(-1);}

    if(ui == 1) {
        printf("\nType the name of the file, located in \"files\": ");
        if(scanf("%s", dec) == EOF) return;
    }
    else strcpy(dec, "2\0");
}

//===================== ROTATION =====================
//Calculate z-Axis point and degree
GLfloat z_treatment(GLfloat x, GLfloat y) {return sqrt(1 - pow(x, 2) - pow(y, 2));}
GLfloat rad_to_degrees(GLfloat rad) {return rad * 180 / M_PI;}

vector4 world_points_1 = {0,0,0,1}, world_points_2 = {0,0,FP_NAN,1}; //Yes...
vector4 center = {0,0,0,1};
mat4x4 final_rot = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

//====================================================
//Keep the previous ctm to not reset between every call
mat4x4 ctm_base = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

//Handles zoom in and zoom out
//Global Variables associated with scale
affine s = {1,1,1};
mat4x4 final_scal = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

void mouse(int button, int state, int x, int y) {

    //===================== SCROLLING SIZE =====================
    mat4x4 sc = identity;

    if(button == 3) s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(button == 4) s = (affine) {s.x - .02, s.y - .02, s.z - .02};
    else s = (affine) {1,1,1};

    //Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen_to_world(&(vector4){x, y, 0, 1}, &world_points_1, 512, 512, z_treatment);
        if(!isnan(world_points_1.z)) copy_matrix(&ctm, &ctm_base);
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        final_rot = identity;
        copy_matrix(&ctm, &ctm_base);
    }
}

//Captures all movement of the mouse when GLUT_LEFT_BUTTON and GLUT_DOWN
void motion(int x, int y) {

    vector4 screen_points = {x, y, 0, 1};
    vector4 cross = {0,0,0,1};
    GLfloat rad = 0.0f;

    screen_to_world(&screen_points, &world_points_2, 512, 512, z_treatment);
        
    //If inside sphere of rotation
    if(!(isnan(world_points_1.z) || isnan(world_points_2.z))) {

        world_points_1.w = 0.0f;
        world_points_2.w = 0.0f;
        vector_norm(&world_points_1);
        vector_norm(&world_points_2);

        vector_cross(&world_points_1, &world_points_2, &cross);
        vector_norm(&cross);
        vector_dot(&world_points_1, &world_points_2, &rad);

        rotate_arb(rad_to_degrees(acos(rad)), &cross, &center, &final_rot);
    } else {final_rot = identity; copy_matrix(&ctm, &ctm_base);}
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {
    	glutLeaveMainLoop();
        free(vertices);
        free(colors);
        free(texels);
        free(texcoords);
        if(fp != NULL) fclose(fp);
        printf("\nEXIT SUCCESSFUL\n");
    }

    if(key == 'd') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&vertices[i]);

    if(key == 'f') if(view_file(fp) != 0) printf("\nNO FILE TO VIEW!!!\n");

    if(key == 'r') {final_rot = identity; final_scal = identity; ctm_base = identity;}

    //===================== SCROLLING SIZE =====================
    mat4x4 sc = identity;

    //Optional keyboard input
    if(key == '+') s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(key == '-') s = (affine) {s.x - .02, s.y - .02, s.z - .02};

    //Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);
}

void idle() {
    mat_mult((mat4x4[3]){final_rot, final_scal, ctm_base}, 3, &ctm);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    //Get user decision
    char dec[18];
    menu(dec);

    if(atoi(dec) != 2) { 

        printf("\nFILE NAME: %s\n", dec);

        //Load file
        fp = fopen(strcat((char[24]) {"files/"}, dec), "r");
        if(load_count(fp, &num_vertices) != 0) return -1;
        if(load_va(fp, vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices) != 0) return -1;

        //Get the center of mass
        vector4 cm = {0,0,0,1};
        com(vertices, num_vertices, &cm);
        printf("\nCENTER OF MASS:");
        print_vector(cm);

        //Move to origin
        //Scale to fit OpenGL Canonical View
        mat4x4 base_or, shrink, move, final;
        vector4 base[num_vertices];
        for(int i = 0; i < num_vertices; i++) copy_vector(vertices + i, base + i);
        
        translate(-1 * cm.x,-1 * cm.y, -1 * cm.z, &move);
        scaling(.01, .01, .01, &shrink);
        rotate(-90, 'x', &base_or);
        
        mat_mult((mat4x4[3]) {base_or, shrink, move}, 3, &final);
        matxvar(&final, base, num_vertices, vertices);
    }
    else //sphere(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, 16, .25);
        circle(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, .5, (vector4){0,0,0,1}, 'z');
    
    //Assign color and print statistics
    random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);
    texturize(texcoords = (vector2*) malloc(sizeof(vector2) * num_vertices), num_vertices, CIRCLE);
    printf("COUNT: %d\n",  num_vertices);
    
    //Get center of mass
    com(vertices, num_vertices, &center);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");

    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
