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

#include "../matrix library/initShader.h"
#include "../matrix library/matrix_def.h"
#include "../matrix library/matrix_utility.h"
#include "../matrix library/matrix_ops.h"
#include "../matrix library/shapes.h"
#include "../matrix library/affine.h"
#include "../matrix library/file_reader.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location; 

vector4* vertices;
vector4* colors;

int num_vertices = 360;

FILE* fp = NULL;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vector4) * num_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * num_vertices, sizeof(vector4) * num_vertices, colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * num_vertices));

    //Locate and use transformation matrix ctm
    ctm_location = glGetUniformLocation(program, "ctm");

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

    while(ui != 2 && ui != 1){printf("\nINVALID OPTION!!!"); if(scanf("%d", &ui) == EOF) return;}

    if(ui == 1) {
        printf("\nType the name of the file, located in \"files\": ");
        if(scanf("%s", dec) == EOF) return;
    }
    else strcpy(dec, "2\0");
}

//===================== ROTATION =====================
//Calculate z-Axis point
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
    mat4x4 sc; identity(&sc);

    if(button == 3) s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(button == 4) s = (affine) {s.x - .02, s.y - .02, s.z - .02};
    else s = (affine) {1,1,1};

    //Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen_to_world(&(vector4){x, y, 0, 1}, &world_points_1, 512, 512, z_treatment);
        if(!isnan(world_points_1.z)) copy_matrix(&ctm, &ctm_base);
        else printf("\nSTART AT NAN\n");
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

        //Put it all together
        mat4x4 t1, rx1, ry1, rz, ry2, rx2, t2;
        GLfloat rx, ry, deg = rad_to_degrees(acos(rad));

        trans((affine){-center.x, -center.y, -center.z}, &t1); trans((affine){center.x, center.y, center.z}, &t2);

        rx = rad_to_degrees(asin(cross.y / (sqrt(pow(cross.y, 2) + pow(cross.z, 2)))));
        rotate(rx, 'x', &rx1); rotate(-rx, 'x', &rx2);

        ry = rad_to_degrees(asin(cross.x));
        rotate(-ry, 'y', &ry1); rotate(ry, 'y', &ry2);

        rotate(deg, 'z', &rz);

        mat_mult((mat4x4[7]) {t2, rx2, ry2, rz, ry1, rx1, t1}, 7, &final_rot);
    } else {identity(&final_rot); printf("\nNAN\n");}
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {
    	glutLeaveMainLoop();
        free(vertices);
        free(colors);
        if(fp != NULL) fclose(fp);
        printf("\nEXIT SUCCESSFUL\n");
    }

    if(key == 'd') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&vertices[i]);

    if(key == 'f') if(view_file(fp) != 0) printf("\nNO FILE TO VIEW!!!\n");

    if(key == 'r') {identity(&final_rot); identity(&final_scal); identity(&ctm_base);}

    //===================== SCROLLING SIZE =====================
    mat4x4 sc; identity(&sc);

    //Optional keyboard input
    if(key == '+') s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(key == '-') s = (affine) {s.x - .02, s.y - .02, s.z - .02};
    else s = (affine) {1,1,1};

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
        vector4 cm = {0,0,0,0};
        com(vertices, num_vertices, &cm);
        printf("\nCENTER OF MASS:");
        print_vector(cm);

        //Move to origin
        //Scale to fit OpenGL Canonical View
        mat4x4 base_or, shrink, move, final;
        vector4 base[num_vertices];
        for(int i = 0; i < num_vertices; i++) copy_vector(vertices + i, base + i);
        
        rotate(-90, 'x', &base_or);
        scaling(.01, .01, .01, &shrink);
        translate(-1 * cm.x,-1 * cm.y, -1 * cm.z, &move);
        mat_mult((mat4x4[3]) {base_or, shrink, move}, 3, &final);
        matxvar(&final, base, num_vertices, vertices);
    }
    else {sphere(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, 10, .25);}

    //Assign color and print statistics
    random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);
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
