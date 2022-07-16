/*  Command to compile the program: 
    g++ -o game -Wall game.cpp -mwindows glut32.lib -lopengl32 -lglu32 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include<string.h>
#include <direct.h>
#include <sys/stat.h>

//Game Speed
int FPS = 50;
//Game Track
int start = 0;
int gv = 0;
int level = 0;

//Track Score
int score = 0;
char highScore[100],ch;

//For move track
float roadDivTop = 0;
float roadDivMdl = 0;
float roadDivBtm = 0;

//For Car Left / RIGHT
int lrIndex = 0;

//Car Coming
int car1 = 0;
int lrIndex1 = 0;
int car2 = +35;
int lrIndex2 = 0;
int car3 = +70;
int lrIndex3 = 0;

//For Display TEXT
const int font1 = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2 = (int)GLUT_BITMAP_HELVETICA_18;
const int font3 = (int)GLUT_BITMAP_8_BY_13;
const int font4 = (int)GLUT_STROKE_ROMAN;

char* gameDataPath = getenv("APPDATA");
char gameDataFile[500];
bool isDirCreated = false;


char s[30];
void renderBitmapString(float x, float y, void *font, const char *string)
{
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void createDir(){
    strcat(gameDataPath, "\\TestDrive");
    strcpy(gameDataFile,gameDataPath);
    strcat(gameDataFile,"\\HighScoreFile.txt");
    _mkdir(gameDataPath);
    isDirCreated = true;
}

void readFromFile() {
    
	FILE *fp = fopen(gameDataFile ,"r");
	int i=0;
	if(fp!= NULL){
		while(fread(&ch,sizeof(char),1 ,fp)){
			highScore[i++] = ch;
		}
		highScore[i] = '\0';
	} 
	fclose(fp);
}

void writeIntoFile() {						//To write high score on to file
	FILE *fp = fopen(gameDataFile ,"w");
	int i=0;
	char temp[40];
	if(fp!= NULL){
		int n= score;
		while(n){
			ch = (n%10)+ '0';
			n/=10;
			temp[i++] = ch;
		}
		temp[i] = '\0';
		strrev(temp);
		puts(temp);
		if(temp[0] == '\0')
			temp[i++] = '0' ,temp[i++] = '\0';
		fwrite(temp ,sizeof(char)*i ,i ,fp);
	}
 fclose(fp);
}

void tree(int x, int y)
{
    int newx = x;
    int newy = y;
    //Bottom
    glColor3f(0.871, 0.722, 0.529);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55);
    glVertex2f(newx + 12, newy + 55 - 10);
    glVertex2f(newx + 10, newy + 55 - 10);
    glEnd();
    //Top
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55 + 3);
    glVertex2f(newx + 12 + 3, newy + 55 - 3);
    glVertex2f(newx + 10 - 3, newy + 55 - 3);
    glEnd();
}


void startGame()
{
    //Road
    glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    //Road Left Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);
    glVertex2f(20, 100);
    glVertex2f(23, 100);
    glVertex2f(23, 0);
    glEnd();

    //Road Right Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(77, 0);
    glVertex2f(77, 100);
    glVertex2f(80, 100);
    glVertex2f(80, 0);
    glEnd();

    //TOP
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivTop + 80);
    glVertex2f(48, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 100);
    glVertex2f(52, roadDivTop + 80);
    glEnd();
    roadDivTop = roadDivTop - 0.5;
    if (roadDivTop < -100)
    {
        roadDivTop = 20;
        score++;
    }
    //Middle
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivMdl + 40);
    glVertex2f(48, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 60);
    glVertex2f(52, roadDivMdl + 40);
    glEnd();
    roadDivMdl = roadDivMdl - 0.5;
    if (roadDivMdl < -60)
    {
        roadDivMdl = 60;
        score++;
    }
    //Bottom
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivBtm + 0);
    glVertex2f(48, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 20);
    glVertex2f(52, roadDivBtm + 0);
    glEnd();
    roadDivBtm = roadDivBtm - 0.5;
    if (roadDivBtm < -20)
    {
        roadDivBtm = 100;
        score++;
    }

    //Score Board
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(80, 97);
    glVertex2f(100, 97);
    glVertex2f(100, 98 - 8);
    glVertex2f(80, 98 - 8);
    glEnd();

    //Print Score
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95, (void *)font3, buffer);
    //Speed Print
    char buffer1[50];
    sprintf(buffer1, "SPEED:%dKm/h", FPS);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95 - 2, (void *)font3, buffer1);
    //level Print
    if (score % 50 == 0)
    {
        int last = score / 50;
        if (last != level)
        {
            level = score / 50;
            FPS = FPS + 2;
        }
    }
    char level_buffer[50];
    sprintf(level_buffer, "LEVEL: %d", level+1);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 95 - 4, (void *)font3, level_buffer);

    //Increase Speed With level

    //MAIN car
    //Front Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 7);
    glVertex2f(lrIndex + 26 - 2, 9);
    glVertex2f(lrIndex + 30 + 2, 9);
    glVertex2f(lrIndex + 30 + 2, 7);
    glEnd();
    //Back Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 2);
    glVertex2f(lrIndex + 26 - 2, 4);
    glVertex2f(lrIndex + 30 + 2, 4);
    glVertex2f(lrIndex + 30 + 2, 2);
    glEnd();
    //Car Body
    //1
    glColor3f(0.678, 1.000, 0.184);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 25, 8);
    glVertex2f(lrIndex + 25, 10);
    glVertex2f(lrIndex + 31, 10);
    glVertex2f(lrIndex + 31, 8);
    glEnd();
    //2
    glColor3f(0.00000000000, 0.00000000000, 0.00000000000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, 6);
    glVertex2f(lrIndex + 25, 8);
    glVertex2f(lrIndex + 31, 8);
    glVertex2f(lrIndex + 30, 6);
    glEnd();
    //3
    glColor3f(0.11764705882, 0.11764705882, 0.11764705882);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 25, 1);
    glVertex2f(lrIndex + 25, 8);
    glVertex2f(lrIndex + 26, 6);
    glVertex2f(lrIndex + 26, 2);
    glEnd();
    //4
    glColor3f(0.678, 1.000, 0.184);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, 2);
    glVertex2f(lrIndex + 26, 6);
    glVertex2f(lrIndex + 30, 6);
    glVertex2f(lrIndex + 30, 2);
    glEnd();
    //5
    glColor3f(0.11764705882, 0.11764705882, 0.11764705882);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 30, 2);
    glVertex2f(lrIndex + 30, 6);
    glVertex2f(lrIndex + 31, 8);
    glVertex2f(lrIndex + 31, 1);
    glEnd();
    //6
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 25, 1);
    glVertex2f(lrIndex + 26, 2);
    glVertex2f(lrIndex + 30, 2);
    glVertex2f(lrIndex + 31, 1);
    glEnd();

    //Opposite car 1
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 4);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 4);
    glEnd();
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100);
    glEnd();
    glColor3f(1.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26, car1 + 100);
    glVertex2f(lrIndex1 + 26, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 30, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 30, car1 + 100);
    glEnd();
    car1--;
    if (car1 < -100)
    {
        car1 = 0;
        lrIndex1 = lrIndex;
    }
    //Kill check car1
    if ((abs(lrIndex - lrIndex1) < 8) && (car1 + 100 < 10))
    {
        start = 0;
        gv = 1;
    }


    //Opposite car 2
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 4);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 4);
    glEnd();
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100);
    glEnd();
    glColor3f(0.300, 0.000, 0.500);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26, car2 + 100);
    glVertex2f(lrIndex2 + 26, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 30, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 30, car2 + 100);
    glEnd();
    car2--;
    if (car2 < -100)
    {
        car2 = 0;
        lrIndex2 = lrIndex;
    }
    //Kill check car2
    if ((abs(lrIndex - lrIndex2) < 8) && (car2 + 100 < 10))
    {
        start = 0;
        gv = 1;
    }

    //Opposite car 3
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 4);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 4);
    glEnd();
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100);
    glEnd();
    glColor3f(1.000, 0.271, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26, car3 + 100);
    glVertex2f(lrIndex3 + 26, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 30, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 30, car3 + 100);
    glEnd();
    car3--;
    if (car3 < -100)
    {
        car3 = 0;
        lrIndex3 = lrIndex;
    }
    //Kill check car3
    if ((abs(lrIndex - lrIndex3) < 8) && (car3 + 100 < 10))
    {
        start = 0;
        gv = 1;
    }
}

void fristDesign()
{

    //Road BackGround
    glColor3f(0.000, 0.350, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glColor3f(0.6, 0.8, 0.3);
    glVertex2f(100, 50 - 50);
    glVertex2f(0, 50 - 50);
    glEnd();

    //Road Design In Front Page
    glColor3f(0.12549019608, 0.12549019608, 0.12549019608);
    glBegin(GL_TRIANGLES);
    glVertex2f(32 - 2 + 21, 55);
    glVertex2f(32 + 58, 50 - 50);
    glVertex2f(32 - 22, 50 - 50);
    glEnd();
    //Road Middle
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_TRIANGLES);
    glVertex2f(32 - 2 + 21, 55);
    glVertex2f(50 + 2, 50 - 50);
    glVertex2f(50 - 2, 50 - 50);
    glEnd();

    //Road Sky
    glColor3f(0.000, 0.60, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(100, 100);
    glVertex2f(0, 100);
    glColor3f(0.933, 0.933, 0.933);
    // glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glEnd();

    //Hill 1
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(20, 55 + 10);
    glVertex2f(20 + 7, 55);
    glVertex2f(0, 55);
    glEnd();

    //Hill 2
    glColor3f(0.000, 0.400, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(20 + 15, 55 + 12);
    glVertex2f(20 + 20 + 10, 55);
    glVertex2f(0 + 10, 55);
    glEnd();

    //Hill 4
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(87, 55 + 10);
    glVertex2f(100, 55);
    glVertex2f(60, 55);
    glEnd();

    //Hill 3
    glColor3f(0.000, 0.400, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(70, 70);
    glVertex2f(90, 55);
    glVertex2f(50, 55);
    glEnd();

    //Tree on front page
    tree(5, -15);
    tree(0, 0);
    tree(9, 5);
    tree(80, 0);
    tree(75, -15);
    tree(72, 5);

    //Menu Place Holder
    glColor3f(0.098, 0.098, 0.439);
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 50 + 5 + 10);
    glVertex2f(32 + 46, 50 + 5 + 10);
    glVertex2f(32 + 46, 50 - 15 + 10);
    glVertex2f(32 - 4, 50 - 15 + 10);
    glEnd();

    glColor3f(00, 0, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 50 + 5 + 10);
    glVertex2f(32 + 46, 50 + 5 + 10);
    glVertex2f(32 + 46, 50 + 4 + 10);
    glVertex2f(32 - 4, 50 + 4 + 10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32 + 45, 50 + 5 + 10);
    glVertex2f(32 + 46, 50 + 5 + 10);
    glVertex2f(32 + 46, 50 - 15 + 10);
    glVertex2f(32 + 45, 50 - 15 + 10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 50 - 14 + 10);
    glVertex2f(32 + 46, 50 - 14 + 10);
    glVertex2f(32 + 46, 50 - 15 + 10);
    glVertex2f(32 - 4, 50 - 15 + 10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32 - 4, 50 + 5 + 10);
    glVertex2f(32 - 5, 50 + 5 + 10);
    glVertex2f(32 - 5, 50 - 15 + 10);
    glVertex2f(32 - 4, 50 - 15 + 10);
    glEnd();

    //Text Information in First Page
    if (gv == 1)
    {
        glColor3f(1.000, 0.000, 0.000);
        renderBitmapString(35, 60 + 15, (void *)font1, "GAME OVER");
        glColor3f(1.000, 0.000, 0.000);
        char buffer2[50];
        sprintf(buffer2, "Your Score is : %d", score);
        renderBitmapString(33, 60 - 4 + 15, (void *)font1, buffer2);
        glColor3f(1.000, 0.000, 0.000);
        readFromFile();
        char buffer3[50];
        sprintf(buffer3, "Highest Score :%d", score);
        if(atoi(highScore) < score){
		    writeIntoFile();
		    sprintf(buffer3, "Highest Score :%d", score);
	    } else 
		    sprintf(buffer3, "Highest Score :%s", highScore);
        renderBitmapString(33, 60 - 8 + 15, (void *)font1, buffer3);
    }

    glColor3f(0.09803921569, 0.09803921569, 0.43921568627);
    renderBitmapString(33, 85, (void *)font1, "Project Test Drive");

    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(30, 50 + 10, (void *)font2, "Press SPACE to Start");
    renderBitmapString(30, 50 - 3 + 10, (void *)font2, "Press ESC to Exit");

    glColor3f(1.000, 1.000, 1.000);
    renderBitmapString(30, 50 - 6 + 10, (void *)font3, "Press UP    to Increase Speed");
    renderBitmapString(30, 50 - 8 + 10, (void *)font3, "Press DOWN  to Decrease Speed");
    renderBitmapString(30, 50 - 10 + 10, (void *)font3, "Press RIGHT to Turn Right");
    renderBitmapString(30, 50 - 12 + 10, (void *)font3, "Press LEFT  to Turn Left");

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (start == 1)
    {
        glClearColor(0.000, 0.392, 0.000, 1);
        startGame();
    }

    else
    {
        fristDesign();
    }

    glFlush();
    glutSwapBuffers();
}

void spe_key(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:
        if (FPS > (50 + (level * 2)))
            FPS = FPS - 2;
        break;
    case GLUT_KEY_UP:
        FPS = FPS + 2;
        break;

    case GLUT_KEY_LEFT:
        if (lrIndex >= 0)
        {
            lrIndex = lrIndex - (FPS / 10);
            if (lrIndex < 0)
            {
                lrIndex = -1;
            }
        }
        break;

    case GLUT_KEY_RIGHT:
        if (lrIndex <= 44)
        {
            lrIndex = lrIndex + (FPS / 10);
            if (lrIndex > 44)
            {
                lrIndex = 45;
            }
        }
        break;

    default:
        break;
    }
}

void processKeys(unsigned char key, int x, int y)
{

    switch (key)
    {
    case ' ':
        if (start == 0)
        {
            start = 1;
            gv = 0;
            FPS = 50;
            roadDivTop = 0;
            roadDivMdl = 0;
            roadDivBtm = 0;
            lrIndex = 0;
            car1 = 0;
            lrIndex1 = 0;
            car2 = +35;
            lrIndex2 = 0;
            car3 = +70;
            lrIndex3 = 0;
            score = 0;
            level = 0;
        }
        break;

    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

int main(int argc, char *argv[])
{
    if (isDirCreated == false){
        createDir();
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 650);
    glutInitWindowPosition(200, 20);
    glutCreateWindow("Project Test Drive");

    glutDisplayFunc(display);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(processKeys);

    glOrtho(0, 100, 0, 100, -1, 1);
    glClearColor(0.184, 0.310, 0.310, 1);

    glutTimerFunc(1000, timer, 0);
    glutMainLoop();

    return 0;
}
