#include <unistd.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

int gameMatrix[3][3];

void init(){
	glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,500,500);
    glFlush();
}

void initializeGameMatrix(){
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            gameMatrix[i][j]=-1;    //-1 indicates blank space
}

void drawLine(float x1,float y1,float x2,float y2){
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
    glEnd();
    glFlush();
}

int threeInARow(){
    int result=1;
    if(gameMatrix[0][0]==gameMatrix[0][1] && gameMatrix[0][1]==gameMatrix[0][2] && gameMatrix[0][0]!=-1)
        drawLine(-0.66,0.66,0.66,0.66);
    else if(gameMatrix[0][0]==gameMatrix[1][1] && gameMatrix[1][1]==gameMatrix[2][2] && gameMatrix[0][0]!=-1)
        drawLine(-0.66,0.66,0.66,-0.66);
    else if(gameMatrix[0][0]==gameMatrix[1][0] && gameMatrix[1][0]==gameMatrix[2][0] && gameMatrix[0][0]!=-1)
        drawLine(-0.66,0.66,-0.66,-0.66);
    else if(gameMatrix[1][0]==gameMatrix[1][1] && gameMatrix[1][1]==gameMatrix[1][2] && gameMatrix[1][0]!=-1)
        drawLine(-0.66,0,0.66,0);
    else if(gameMatrix[2][0]==gameMatrix[2][1] && gameMatrix[2][1]==gameMatrix[2][2] && gameMatrix[2][0]!=-1)
        drawLine(-0.66,-0.66,0.66,-0.66);
    else if(gameMatrix[2][0]==gameMatrix[1][1] && gameMatrix[1][1]==gameMatrix[0][2] && gameMatrix[2][0]!=-1)
        drawLine(-0.66,-0.66,0.66,0.66);
    else if(gameMatrix[0][1]==gameMatrix[1][1] && gameMatrix[1][1]==gameMatrix[2][1] && gameMatrix[0][1]!=-1)
        drawLine(0,0.66,0,-0.66);
    else if(gameMatrix[0][2]==gameMatrix[1][2] && gameMatrix[1][2]==gameMatrix[2][2] && gameMatrix[0][2]!=-1)
        drawLine(0.66,0.66,0.66,-0.66);
    else
        result=-1;
    return result;
}

int checkDraw(){
    int allFilled=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(gameMatrix[i][j]==-1)
                allFilled=0;
    return allFilled;
}

void drawGame()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex2f(-0.33,-1);
        glVertex2f(-0.33,1);
        glVertex2f(0.33,-1);
        glVertex2f(0.33,1);
        glVertex2f(-1,-0.33);
        glVertex2f(1,-0.33);
        glVertex2f(-1,0.33);
        glVertex2f(1,0.33);
    glEnd();
    glFlush();
}

float getCoFromViewX(int view){
    float co;
    if(view >= 0 && view < 167)
        co = -0.66f;
    else if(view < 333)
        co = 0.0f;
    else
        co = 0.66f;
}

float getCoFromViewY(int view){
    float co;
    if(view >= 0 && view < 167)
        co = 0.66f;
    else if(view < 333)
        co = 0.0f;
    else
        co = -0.66f;
}

int getMatrixIndexFromCoX(float co){
    int i;
    if((float)-0.66 == co) i = 0;
    else if((float)0 == co) i = 1;
    else i = 2;
    return i;
}

int getMatrixIndexFromCoY(float co){
    int i;
    if((float)0.66 == co) i = 0;
    else if((float)0 == co) i = 1;
    else i = 2;
    return i;
}

int gameMatrixLocationEmpty(int i,int j){
    if(gameMatrix[i][j]==-1)
        return 1;
    return 0;
}

void drawShape(float xco,float yco,int shape){
    glColor3f(0,1,1);
    if(shape == 1){
        glBegin(GL_LINES);
            glVertex2f(xco+0.165,yco+0.165);
            glVertex2f(xco-0.165,yco-0.165);
            glVertex2f(xco-0.165,yco+0.165);
            glVertex2f(xco+0.165,yco-0.165);
        glEnd();
        glFlush();
    }
    else{
        float radius = 0.165;
        glBegin(GL_LINE_LOOP);
            for (int i=0; i < 360; i++){
                float degInRad = (i*3.14159)/180;
                glVertex2f(xco + cos(degInRad)*radius,yco + sin(degInRad)*radius);
            }
        glEnd();
        glFlush();
    }
}

void drawWins(){
    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.165,0.165);
        glVertex2f(-0.08,-0.165);
        glVertex2f(0,0.08);
        glVertex2f(0.08,-0.165);
        glVertex2f(0.165,0.165);
    glEnd();
    glFlush();
}

void restart(int shape){
    if(shape!=-1){
        init();
        drawShape(-0.66,0,shape);
        drawWins();
        sleep(2);
    }
    init();
    initializeGameMatrix();
    drawGame();
}

void onClick(int button,int state,int x,int y){
    static int shape = 1;
    float xco,yco;
    int i,j;
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        xco = getCoFromViewX(x);
        yco = getCoFromViewY(y);
        i = getMatrixIndexFromCoY(yco);
        j = getMatrixIndexFromCoX(xco);
        if(gameMatrixLocationEmpty(i,j)){
            drawShape(xco,yco,shape);
            gameMatrix[i][j]=shape;
            if(threeInARow()!=-1){
                printf("%d WINS\n",shape);
                sleep(2);
                restart(shape);
            }
            else if(checkDraw()){
                printf("DRAW\n");
                init();
                sleep(2);
                restart(-1);
            }
            if(shape==1) shape=0;
            else shape = 1;
        }
        else{
            printf("not empty\n");
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    init();
    initializeGameMatrix();
    glutCreateWindow("X & 0 - My First OpenGL Game");
    glutDisplayFunc(drawGame);
    glutMouseFunc(onClick);
    glutMainLoop();
    return 0;
}
