#include <set>
#include <graphics.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <ctime>
#define infinit INT_MAX
#define INF 1000000000
#define epsi 0.0001
#define  MAX 100
#define MAX1 20

using namespace std;
ifstream fin("functii.txt");

///FRONTEND
int graphheight, infoheight, windows[20], windowNr=1, ant, functionTooLongOffset,
    scroll, currWindow, prevWindow, prevWindowNr=1, langCnt;

int posx=-1, posy=-1, cntvect=0, functionNr=1, functionCnt=1, pageNr=1, howManyPages, themeCnt, themeNr=5, langNr=5,
    expand, x_poz_zoom, y_poz_zoom, cnt, cnt1=0, exprLgUnmod, sepFNr, howManyF;

int widthch[10], heightch[10], fIndex[25];
char c, aux[200];
bool update=1, enter=0, exitf=0, wrongf=0, wrongi=0, refresh=0, page=0, showCoords=0, initS1=0, newWindow=0, sound=1;
double A=INF, B=-INF, DIST, scale_zoom;
const char* wavFile="Click.wav";

struct themeInfo
{
    int bkcolor, hovercolor, bordercolor;
} theme[7];

struct boxInfo
{
    unsigned short int fNr;
    int x1, y1, x2, y2, textx, texty, exprLg, textLg, insTextLg;
    char expr[200], a[20], b[20], dist[20], ind[4];
    char text[200];
    char insText[200];
};

struct screenInfo
{
    boxInfo box[50];
    char title[200];
    int titleLg;
    bool active;
} screen[10];

struct windowInfo
{
    char expr[200], a[20], b[20], dist[20];
    HWND handle;
    RECT rect;
    int id, left, right, top, bottom, width, height, screen;
    char title[50];
} window[10];


///BACKEND
bool ScaleToHeightWidth=0;
bool maimulteminime=0,maimultemaxime=0;
double min_y=INF,max_y=-INF,x_scale;
int Yaxis,Xaxis;
const int offsetW=100,offsetH=100;
const int width=getmaxwidth( )-offsetW*2,height=getmaxheight( )-offsetH*2;
int color[]= {15,15,14,2,5,6,10,7,12,9,8,11};
char auxExpr[25][200];

///EVALUATOR
int topOpd,topOp; // varfurile celor doua stive
const int max_stiva=100;
double Opd[max_stiva]; // stiva operanzilor
char Op[max_stiva]; // stiva operatorilor

char OperatiiBinare[200]="+-*/^<>=!|";
char OperatiiUnare[200]="scarelt";
char Operatii[200]="+-*/^<>=!|scarelt";
const float pi=3.1415926;

struct functie
{
    char expresie[300];
    char vect[MAX+1][MAX1];
    char a_str[20], b_str[20], dist_str[20];
    // vectorul cuprinzand cuvintele ce formeaza expresia}
    int lung; // lungimea efectiva a vec"%s %.2f %.2f %.2f\n"torului
    double a, b, dist; // intervalul de evaluare a functiei
    int n; //numarul de puncte de evaluare
    int S[20];
    int lgS;
} F[25][25];

int Prioritate(char c)  // prioritate operatorilor
{
    if(c=='(' || c==')')
        return 0;
    if(c=='+' || c=='-')
        return 1;
    if(c=='*' || c=='/')
        return 2;
    if(c=='^')
        return 3;
    if(c=='=' || c=='#' || c=='<' || c=='>' || c=='|');
    return 4;
    if(c=='c' || c=='s' || c=='l' || c=='e' || c=='t' || c=='a' || c=='r')
        return 5;
}

bool DifInf(double x)
{
    return fabs(infinit-fabs(x)) > infinit / 2.0;
}

/*double Logaritm(double x)
{
    if (x>epsi && DifInf(x))
        return log(x);
    else
        return 0;
}*/

double Logaritm(double x)
{
    if (x>epsi && DifInf(x))
        return log(x);
    if(x<=epsi)
        return -INF;
    return INF;
}

double Exponential(double x)
{
    if (DifInf(x)) return exp(x);
    else return INF;
}

double Inmultit(double x, double y)
{
    if (fabs(x < epsi) && fabs(y) < epsi) return 0;
    else if (DifInf(x) && DifInf(y)) return x*y;
    else return infinit;
}

double Putere(double x, double y, int di, int i)
{
    double rez=1;
    // float p;
    //cout<<y<<'\n';
    //cout<<x<<' '<<y<<'\n';
    if (x==0) return 0;
    else if (y==0) return 1;
    else if (x==INF || y==INF) return INF;
    else if(x<0 && floor(y)!=y)
    {
        if(abs(di%2)==0 && abs(i%2)==1)
        {
            return pow(-x, y);
        }
        else if(abs(di%2)==1 && abs(i%2)==1)
        {
            //cout<<1<<'\n';
            return -pow(-x, y);
        }
    }
    else
        //cout<<2<<'\n';
        return pow(x,y);
}

double Egal(double x, double y)
{
    return x==y;
}

double Diferit(double x, double y)
{
    return x!=y;
}

bool MaiMic(float x, float y)
{
    return x < y;
}

bool MaiMicEgal(float x, float y)
{
    return x <= y;
}

bool MaiMare(float x, float y)
{
    return x > y;
}

bool MaiMareEgal(float x, float y)
{
    return x >= y;
}

bool Sau(bool x, bool y)
{
    return x||y;
}

double Plus(double x, double y)
{
    if (DifInf(x) && DifInf(y))  return x+y;
    else return infinit;
}

double Minus(double x, double y)
{
    if (DifInf(x) && DifInf(y))  return x-y;
    else return infinit;
}

double Impartit(double x, double y)
{
    if (fabs(y)>epsi) return x/y;
    else return infinit;
}

double Sinus(double x)
{
    if (DifInf(x))  return sin(x);
    else return INF;
}

double Arcsinus(double x)
{
    if (x>=-1 && x<=1)
    {
        return asin(x);
    }
    else return 0;
}

double Cosinus(double x)
{
    if (DifInf(x))  return cos(x);
    else return INF;
}

double Arccosinus(double x)
{
    if (x>=-1 && x<=1)
    {
        return acos(x);
    }
    else return 0;
}

double Tangenta(double x)
{
    if(DifInf(x))
    {
        if(cos(x)>epsi)
        {
            return sin(x)/cos(x);
        }
        return 0;
    }
    else return 0;
}

double Arctangenta(double x)
{
    if (DifInf(x))
    {
        return atan(x);
    }
    else return infinit;
}

double Cotangenta(double x)
{
    if(DifInf(x))
    {
        if(sin(x)>epsi)
        {
            return cos(x)/sin(x);
        }
        return 0;
    }
    else return 0;
}

double Modul(double x)
{
    if (DifInf(x)) return fabs(x);
    else return infinit;
}

double Radical(double x)
{
    if (DifInf(x) && (x>epsi)) return sqrt(x);
    else return infinit;
}

bool EsteNumar(char sir[MAX1])
{
    return ((strchr("0123456789",sir[0])) /*|| (sir[0]=='-' && strchr("0123456789",sir[1]))*/);
}

void depanare()
{
    printf("Stiva operanzilor: ");
    for (int i=1; i<=topOpd; i++)
        printf("%f,",Opd[i]);
    printf("\nStiva operatorilor: ");
    for (int i=1; i<=topOp; i++)
        printf("%c,",Op[i]);
    printf("\n");
}

double ValoareFunctie(functie E, double x)
{
    int i;
    bool egal=0, cotan=0, arcsin=0, arccos=0, arctan=0, arccotan=0;
    // returneaza valoarea functiei E in punctul x
    double valo,x_1,x_2, cx_1, cx_2;
    //calculul expresiei

    ///DEBUG

    /*if(currWindow==2)
    {
        cout<<fIndex[currWindow]<<'\n';
    }*/

    for(int i=1; i<=fIndex[currWindow] && currWindow==2; i++)
    {
        //cout<<F[currWindow][i].expresie<<' ';
        /*for(int j=0; j<=F[currWindow][i].lung; j++)
        {
            cout<<F[currWindow][i].vect[j]<<'\n';
        }
        if(i==fIndex[currWindow])
        {
            update=0;
        }*/
        //cout<<'\n';
    }
    //cout<<E.expresie<<' '<<E.a<<' '<<E.b<<' '<<E.dist<<'\n';
    for(int i=0; i<=E.lung; i++)
    {
        //cout<<E.vect[i]<<'\n';
    }

    for (i=1; i<=max_stiva; i++)
    {
        Opd[i]=0;
        Op[i]='@';
    }
    topOpd=0;
    topOp=1;
    Op[topOp]='(';
    i=0;
    while (i<=E.lung && topOp>0)
    {
        //cout<<E.lung<<'\n';
        //depanare();
        i++;
        if (EsteNumar(E.vect[i]))
        {
            // printf("\nE.vect[%d]=%s\n",i,E.vect[i]);
            topOpd++;
            valo=atof(E.vect[i]);
            Opd[topOpd]=valo;
            //depanare();
        }
        else
            switch (E.vect[i][0])
            {
            /* constanta pi=3.1415926 se da sub forma literei q */
            case 'q':
                topOpd++;
                Opd[topOpd]=pi;
                break;
            //case '-':
            //    if(E.vect[i][1]=='X')
            //    /* variabila x */ topOpd++; Opd[topOpd]=-x;
            //    /* printf("\n-->x=%f\n",x); */ break;
            case 'X':
                topOpd++;
                Opd[topOpd]=x;
                break;
            case '(': /* inceput de bloc */
                topOp++;
                Op[topOp]='(';
                break;
            default:
                /* operatii binare si unare */
                while ((topOp>0) && !(strchr("()",Op[topOp])) &&
                        Prioritate(Op[topOp])>=Prioritate(E.vect[i][0]))
                {
                    if (topOpd>1) x_1=Opd[topOpd-1];
                    x_2=Opd[topOpd];
                    // depanare();
                    /* functii scrise in OPERATII */
                    switch (Op[topOp])
                    {
                    case '=':
                        valo=Egal(x_1,x_2);
                        break;
                    case '!':
                        valo=Diferit(x_1,x_2);
                        break;
                    case '<':
                        if(egal==1)
                        {
                            valo=MaiMicEgal(x_1,x_2);
                            egal=0;
                            break;
                        }
                        else
                        {
                            valo=MaiMic(x_1,x_2);
                            break;
                        }
                    case '>':
                        if(egal==1)
                        {
                            valo=MaiMareEgal(x_1,x_2);
                            egal=0;
                            break;
                        }
                        else
                        {
                            valo=MaiMare(x_1,x_2);
                            break;
                        }
                    case '|':
                        valo=Sau(x_1, x_2);
                        break;
                    case '+':
                        valo=Plus(x_1,x_2);
                        break;
                    case '-':
                        valo=Minus(x_1,x_2);
                        break;
                    case '*':
                        valo=Inmultit(x_1,x_2);
                        break;
                    case '/':
                        valo=Impartit(x_1,x_2);
                        cx_1=x_1, cx_2=x_2;
                        break;
                    case '^':
                        valo=Putere(x_1,x_2,cx_1,cx_2);
                        break;
                    case 's':
                        valo=Sinus(x_2);
                        break;
                    case 'c':
                        if(cotan==1)
                        {
                            valo=Cotangenta(x_2);
                            cotan=0;
                            break;
                        }
                        else
                        {
                            valo=Cosinus(x_2);
                            break;
                        }
                    case 'l':
                        valo=Logaritm(x_2);
                        break;
                    case 'e':
                        valo=Exponential(x_2);
                        break;
                    case 'a':
                        if(arcsin==1)
                        {
                            valo=Arcsinus(x_2);
                            arcsin=0;
                            break;
                        }
                        else if(arccos==1)
                        {
                            valo=Arccosinus(x_2);
                            arccos=0;
                            break;
                        }
                        else if(arctan==1)
                        {
                            valo=Arctangenta(x_2);
                            arctan=0;
                            break;
                        }
                        else if(arccotan==1)
                        {
                            valo=-Arctangenta(x_2)+1.5078;
                            arccotan=0;
                            break;
                        }
                        else
                        {
                            valo=Modul(x_2);
                            break;
                        }
                    case 'r':
                        valo=Radical(x_2);
                        break;
                    case 't':
                        valo=Tangenta(x_2);
                        break;
                    } //operator[topOp]
                    if (strchr(OperatiiBinare,Op[topOp])) topOpd--;
                    if (strchr(Operatii,Op[topOp])) Opd[topOpd]=valo;
                    topOp--;
                }
                //cout<<'\n';
                //depanare();
                if (topOp>0)
                    if ((Op[topOp]!='(') || (strcmp(E.vect[i],")")))
                    {
                        topOp++;
                        Op[topOp] = E.vect[i][0];
                        if((Op[topOp]=='<' || Op[topOp]=='>') && E.vect[i+1][0]=='=')
                        {
                            egal=1;
                            i++;
                        }
                        if(E.vect[i][0]=='c' && E.vect[i][1]=='t')
                        {
                            cotan=1;
                        }
                        if(E.vect[i][0]=='a' && (E.vect[i][1]=='s' || E.vect[i][1]=='c' || E.vect[i][1]=='t'))
                        {
                            if(E.vect[i][1]=='s')
                            {
                                arcsin=1;
                            }
                            else if(E.vect[i][1]=='c')
                            {
                                if(E.vect[i][2]=='t')
                                {
                                    arccotan=1;
                                }
                                else
                                {
                                    arccos=1;
                                }
                            }
                            else if(E.vect[i][1]=='t')
                            {
                                arctan=1;
                            }
                        }
                    }
                    else topOp--;
            }
    }
    if (topOp==0) return Opd[1];
    else return infinit;
}

void GetMinMaxF(double A,double B, double x_scale, int index);
void CreateGraph(double A, double B, double x_scale, double y_scale, int x_poz, int y_poz, int index);
void DrawAxis(double A, double B, double x_scale, double y_scale, int x_poz, int y_poz);
void MaximizedInt(double A,double B);
int NumberToText(double value,char c[],int lg);
double derivative(functie E, double x);
void Zoom();
bool Coords(int x_poz, int y_poz, double scale);

void functionTextBuild();
bool HoverClick(int x1, int y1, int x2, int y2, int tx, int ty, char text[100], int textlg, int widthch, int heightch,
                int s1, int s2, int next, int hovercolor, int color);

void SolveForEqualAxis()
{
    /*for(int i=1; i<=fIndex[currWindow]; i++)
    {
        cout<<F[currWindow][i].expresie<<' '<<F[currWindow][i].a<<' '<<F[currWindow][i].b<<' '<<F[currWindow][i].dist<<' '<<'\n';
    }*/
    A=INF;
    B=-INF;
    for(int i=1; i<=fIndex[currWindow]; i++)
    {
        A=min(A,F[currWindow][i].a);
        B=max(B,F[currWindow][i].b);
    }
    double x_scale=1.0*(B-A)/width, scale;
    int x_poz=0,y_poz=0;
    min_y=INF,max_y=-INF;

    for(int i=1; i<=fIndex[currWindow]; i++)
    {
        GetMinMaxF(A,B,x_scale,i);
    }
    //cout<<min_y<<' '<<max_y<<'\n';
    ScaleToHeightWidth=0;
    //double y_scale=1.0*(max_y-min_y)/height;
    if(1.0*(max_y-min_y)/x_scale>height)
    {
        ScaleToHeightWidth=1;
        scale=1.0*(max_y-min_y)/height;
        x_poz=(width-(B-A)/scale)/2;
    }
    else
    {
        scale=x_scale;
        y_poz=(height-(max_y-min_y)/scale)/2;
    }
    DrawAxis(A,B,scale,scale,x_poz,y_poz);
    setcolor(WHITE);
    x_poz_zoom=x_poz;
    y_poz_zoom=y_poz;
    scale_zoom=scale;
    for(int i=1; i<=fIndex[currWindow]; i++)
        CreateGraph(F[currWindow][i].a,F[currWindow][i].b,scale,scale,x_poz,y_poz,i);
    Coords(x_poz, y_poz, scale);
}
void Zoom()
{
    int i,xBoxOffset,yBoxOffset,widthOffset=30,heightOffset=30;
    if (kbhit())
        {
            int A1=A,B1=B;
            A=INF;
            B=-INF;
            char c=getch();
            /*if(c!='a' && c!='d' && c!='i' && c!='o' && c!='A' && c!='D' && c!='I' && c!='O')
                {closegraph();showCoords=0;}*/
            if(c=='a'){cleardevice(); for(i=1; i<=fIndex[currWindow]; i++) { F[currWindow][i].a-=F[currWindow][1].dist; F[currWindow][i].b-=F[currWindow][1].dist; } SolveForEqualAxis();} ///left
            else if(c=='d'){cleardevice(); for(i=1; i<=fIndex[currWindow]; i++) { F[currWindow][i].a+=F[currWindow][1].dist; F[currWindow][i].b+=F[currWindow][1].dist; } SolveForEqualAxis();} ///right
            else if(c=='i')
            {
                bool verif=1;
                for(i=1; i<=fIndex[currWindow]; i++)
                    if(F[currWindow][i].a+F[currWindow][1].dist>F[currWindow][i].b-F[currWindow][1].dist)
                        verif=0;
                if(verif)
                {
                    cleardevice();
                    for(i=1; i<=fIndex[currWindow]; i++)
                    {   //if(F[currWindow][i].a==A1)
                             F[currWindow][i].a+=F[currWindow][1].dist;
                        //if(F[currWindow][i].b==B1)
                            F[currWindow][i].b-=F[currWindow][1].dist;
                    }
                    SolveForEqualAxis();
                }
            } ///zoom in
            else if(c=='o'){cleardevice(); for(i=1; i<=fIndex[currWindow]; i++) { F[currWindow][i].a-=F[currWindow][1].dist; F[currWindow][i].b+=F[currWindow][1].dist; } SolveForEqualAxis();} /*A-=DIST; B+=DIST;*/ ///zoom out
        }

}

bool Coords(int x_poz, int y_poz, double scale)
{
    int prevMouseX=-1,prevMouseY=-1,i;
    while(showCoords)
    {
        //cout<<"DA";
        //settextstyle(8, 0, 1);
        int mouseX=mousex();
        int mouseY=mousey();
   //     cout<<mouseX<<'\n';
        if (mouseX!=prevMouseX || mouseY!=prevMouseY)
        {
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            bar(0, 0, 200, 30); // Clear the text area
            setcolor(WHITE);
            setbkcolor(BLACK);
            double x=mouseX,y=mouseY;
            double xValue=(x-offsetW-x_poz)*scale+A, yValue=max_y-(y-offsetH-y_poz)*scale;
            //cout<<xValue<<' '<<yValue<<'\n';
            //cout<<x_poz<<' '<<y_poz<<' '<<scale<<'\n';
            int xPixel,yPixel;
            yPixel=(height-1)-1.0*(yValue-min_y)/scale+offsetH-y_poz;
            char c[50];
            int lg=NumberToText(xValue,c,0);
            c[lg++]=',';
            NumberToText(yValue,c,lg);
            outtextxy(10, 10, c);
           // outtextxy(screen[3].box[1].x, screen[3].box[1].y1, c);
            prevMouseX=mouseX;
            prevMouseY=mouseY;
        }
        Zoom();
        int i,xBoxOffset,yBoxOffset,widthOffset=30,heightOffset=30;
        xBoxOffset=widthch[2]/2;
        yBoxOffset=widthch[2]/4;

        screen[3].box[1].x1=widthOffset, screen[3].box[1].y1=window[1].height-heightOffset-heightch[2]-2*yBoxOffset,
        screen[3].box[1].x2=widthOffset+screen[3].box[1].insTextLg*widthch[2]+2*xBoxOffset, screen[3].box[1].y2=window[1].height-heightOffset;

        setcolor(WHITE);

        rectangle(screen[3].box[1].x1, screen[3].box[1].y1, screen[3].box[1].x2, screen[3].box[1].y2);
        outtextxy(screen[3].box[1].x1+xBoxOffset, screen[3].box[1].y1+yBoxOffset, screen[3].box[1].insText);

        HoverClick(screen[3].box[1].x1, screen[3].box[1].y1, screen[3].box[1].x2, screen[3].box[1].y2, screen[3].box[1].x1+xBoxOffset, screen[3].box[1].y1+yBoxOffset,
                   screen[3].box[1].insText, screen[3].box[1].insTextLg, widthch[2], heightch[2], 3, 3, 0, DARKGRAY, BLACK);
        if(exitf)
        {
            showCoords=0;
        }
    }
}


void GetMinMaxF(double A,double B, double x_scale,int index)
{
    double x=A;
    int i,j;
    for(j=1,i=offsetW; i<=width-2+offsetW; i++,j++)
    {
        double y=ValoareFunctie(F[currWindow][index],x);
        if(y>-INF && y<INF)   //&& (y<-epsi || y>epsi)
        {
            min_y=min(min_y,y);
            max_y=max(max_y,y);
        }
        x+=x_scale;
    }
    double y=ValoareFunctie(F[currWindow][index],B);
    min_y=min(min_y,y);
    max_y=max(max_y,y);
}
int cautare(int index,double num)
{
    int exista=1,i;
    //cout<<F[currWindow][index].lgS;
    for(i=0;i<=F[currWindow][index].lgS-1 && F[currWindow][index].S[i]<num;i++);
    if(i>F[currWindow][index].lgS-1)
        return -INF;
    return F[currWindow][index].S[i];
}
void CreateGraph(double a, double b, double x_scale, double y_scale, int x_poz, int y_poz,int index)
{
    int clr=color[index];
    if(index>=11) clr=15;
    setcolor(clr);
    double x=a+x_scale;
    int i;
    bool pctminmax=0;
    int discontinuitate=0;
    double y=ValoareFunctie(F[currWindow][index],a), predX;
    y=(y<=-INF) ? min_y : (y>=INF) ? max_y : y;
    double aux=1.0*(y-min_y)/y_scale;
    int pixel_y=(height-1)-aux+offsetH-y_poz;
    int l=1.0*(a-A)/x_scale,r=1.0*(B-b)/y_scale;
    if(derivative(F[currWindow][index],a)<0.001 && derivative(F[currWindow][index],a)>-0.001)
        setcolor(LIGHTMAGENTA),line(offsetW+1+x_poz+l,pixel_y-1,offsetW+1+x_poz+l,pixel_y+1),setcolor(clr),pctminmax=1;
    else
        line(offsetW+1+x_poz+l,pixel_y-1,offsetW+1+x_poz+l,pixel_y+1);
    int predPixel=pixel_y;
    predX=x;
    for(i=offsetW+1+x_poz+l;i<=width-2+offsetW-x_poz-r;i++)
    {

        y=ValoareFunctie(F[currWindow][index],x); //cout<<y<<' '<<x<<'\n';
        y=(y<=-INF) ? min_y : (y>=INF) ? max_y : y;
        aux=1.0*(y-min_y)/y_scale; ///distanta fata  de punctul de minim la care trebuie sa plasam pixelul
        pixel_y=(height-1)-aux+offsetH-y_poz;
        if(cautare(index,predX)==-INF || x<cautare(index,predX)) //&& *F[currWindow][index].S.upper_bound(int(predX))>0) || (*F[currWindow][index].S.upper_bound(int(predX))<predX || x>*F[currWindow][index].S.upper_bound(int(predX))) && *F[currWindow][index].S.upper_bound(int(predX))<0)
            line(i-1,predPixel,i,pixel_y), line(i-1,predPixel-1,i,pixel_y-1), line(i-1,predPixel+1,i,pixel_y+1), line(i-1,predPixel,i,pixel_y-1), line(i-1,predPixel-1,i,pixel_y);
        else
            discontinuitate=2,putpixel(i,pixel_y,clr);
        if(pctminmax) ///pctul i-2 era de min/max local
            setcolor(LIGHTMAGENTA),line(i-1,predPixel-1,i-1,predPixel+1),setcolor(clr),pctminmax=0;
        ///punct de minim/maxim
        if(cautare(index,predX-x_scale)==-INF || predX<cautare(index,predX-x_scale) && x<cautare(index,predX) || !F[currWindow][index].lgS) ///verif sa nu fie discontinuitate si sa aleg pctele min/max gresit
            if((ValoareFunctie(F[currWindow][index],predX-x_scale)<ValoareFunctie(F[currWindow][index],predX) && y<ValoareFunctie(F[currWindow][index],predX) || ValoareFunctie(F[currWindow][index],predX-x_scale)>ValoareFunctie(F[currWindow][index],predX) && y>ValoareFunctie(F[currWindow][index],predX)))
                setcolor(LIGHTMAGENTA),line(i-2,predPixel-1,i-2,predPixel+1),line(i-1,predPixel-1,i-1,predPixel+1),setcolor(clr), pctminmax=1;
        predPixel=pixel_y;
        predX=x;
        x+=x_scale;
     /*   if(y-min_y<0.0001)
            setcolor(GREEN), circle(i,pixel_y,3),floodfill(i,pixel_y,GREEN), setcolor(WHITE);
        if(max_y-y<0.0001)
            setcolor(RED), circle(i,pixel_y,3), floodfill(i,pixel_y,RED), setcolor(WHITE);*/
    }
    y=ValoareFunctie(F[currWindow][index],b);
    y=(y<=-INF) ? min_y : (y>=INF) ? max_y : y;
    aux=1.0*(y-min_y)/y_scale;
    pixel_y=(height-1)-aux+offsetH-y_poz;
    if(cautare(index,predX)==-INF || b<cautare(index,predX))
        line(width-2+offsetW-x_poz-r,predPixel,width-1+offsetW-x_poz-r,pixel_y), line(width-2+offsetW-x_poz-1-r,predPixel,width-1+offsetW-x_poz-1-r,pixel_y), line(width-2+offsetW-x_poz-r,predPixel-1,width-1+offsetW-x_poz-r,pixel_y-1), line(width-2+offsetW-x_poz-1-r,predPixel-1,width-1+offsetW-x_poz-1-r,pixel_y-1);
    else
        line(width-2+offsetW-x_poz,pixel_y-1,width-2+offsetW-x_poz+1,pixel_y);
    if(derivative(F[currWindow][index],b)<0.001 && derivative(F[currWindow][index],b)>-0.001)
            setcolor(LIGHTMAGENTA),line(width-2+offsetW-x_poz-r,pixel_y-1,width-2+offsetW-x_poz+1-r,pixel_y+1),line(width-1+offsetW-x_poz-r,pixel_y-1,width-1+offsetW-x_poz+1-r,pixel_y+1),setcolor(clr);
}
void inserare(int j,int num)
{
    int verif=1,i;
    for(i=F[currWindow][j].lgS-1;i>=0;i--)
    {
        cout<<F[currWindow][j].S[i]<<' ';
        if(F[currWindow][j].S[i]==num)
            verif=0;
    }
    if(!verif)
        return;
    //cout<<"D";
    for(i=F[currWindow][j].lgS-1;F[currWindow][j].S[i]>num && i>0;i--)
    {
        F[currWindow][j].S[i+1]=F[currWindow][j].S[i];
    }
    F[currWindow][j].S[i]=num;
    F[currWindow][j].lgS++;
   // cout<<num<<' '<<F[currWindow][j].lgS<<'\n';
}
void DrawAxis(double A, double B,double x_scale, double y_scale, int x_poz, int y_poz)
{
    // cout<<maimulteminime<<' '<<maimultemaxime<<' '<<Yaxis<<' ';
    char c[50];
    int Ox=0,Oy=0; ///screen coords where A is at 0
    ///int Yaxis,Xaxis; /// screen coords
    bool center=0;
    Ox=1.0*(height-1)+offsetH-1.0*(0-min_y)/y_scale-y_poz;
    Oy=1.0*(0-A)/x_scale+offsetH+x_poz;
    if(Ox>=offsetH-1 && Ox<=offsetH+height-1 && Oy>=offsetW-1 && Oy<=offsetW+width-1)
        center=1;
    Xaxis=(Ox<offsetH-1) ? offsetH-1 : (Ox>offsetH+height-1) ? offsetH+height-1 : Ox;
    Yaxis=(Oy<offsetW-1) ? offsetW-1 : (Oy>offsetW+width-1) ? offsetW+width-1 : Oy;
    setcolor(RED); ///draw range margin
    NumberToText(A,c,0);
    outtextxy(offsetW+x_poz-18,Xaxis+32,c);
    line(offsetW+x_poz,Xaxis-28,offsetW+x_poz,Xaxis+28);
    line(offsetW+x_poz-1,Xaxis-28,offsetW+x_poz-1,Xaxis+28);
    line(offsetW+x_poz+1,Xaxis-28,offsetW+x_poz+1,Xaxis+28);
    line(offsetW+width-x_poz,Xaxis-28,offsetW+width-x_poz,Xaxis+28);
    line(offsetW+width-x_poz-1,Xaxis-28,offsetW+width-x_poz-1,Xaxis+28);
    line(offsetW+width-x_poz+1,Xaxis-28,offsetW+width-x_poz+1,Xaxis+28);
    NumberToText(B,c,0);
    outtextxy(offsetW+width-x_poz-18,Xaxis+32,c);
    setcolor(CYAN);
    ///Ox axis
    line(offsetW/2-1,Xaxis,width-1+offsetW+offsetW/2,Xaxis);
    line(offsetW/2-1,Xaxis+1,width-1+offsetW+offsetW/2,Xaxis+1);
    ///Oy axis
    line(Yaxis,offsetH/2-1,Yaxis,offsetH+height-1+offsetH/2);
    line(Yaxis-1,offsetH/2-1,Yaxis-1,offsetH+height-1+offsetH/2);
    for(int i=Yaxis-100; i>=offsetW/2-1; i-=100)
    {
        line(i,Xaxis-10,i,Xaxis+10);
        double value=1.0*(i-Oy)*x_scale;
        char c[20];
        NumberToText(value,c,0);
        outtextxy(i-13,Xaxis+11,c);
    }
    if(center)
        outtextxy(Yaxis+6,Xaxis+6,"0");
    for(int i=Yaxis+100; i<=width-1+offsetW+offsetW/2; i+=100)
    {
        line(i,Xaxis-10,i,Xaxis+10);
        double value=1.0*(i-Oy)*x_scale;
        char c[20];
        NumberToText(value,c,0);
        outtextxy(i-13,Xaxis+11,c);
    }
    for(int i=Xaxis-100; i>=offsetH/2-1; i-=100)
    {
        line(Yaxis-10,i,Yaxis+10,i);
        double value=1.0*(Ox-i)*y_scale;
        char c[20];
        NumberToText(value,c,0);
        outtextxy(Yaxis+13,i-8,c);
    }
    for(int i=Xaxis+100; i<=offsetH+height-1+offsetH/2; i+=100)
    {
        line(Yaxis-10,i,Yaxis+10,i);
        double value=1.0*(Ox-i)*y_scale;
        char c[20];
        NumberToText(value,c,0);
        outtextxy(Yaxis+13,i-8,c);
    }

    setcolor(WHITE);
}

void MaximizedInt(double A,double B)
{
    double x_scale=1.0*(B-A)/width;
    for(int i=1; i<=fIndex[currWindow]; i++)
        GetMinMaxF(A,B,x_scale,fIndex[currWindow]);
    double y_scale=1.0*(max_y-min_y)/height;
    DrawAxis(A,B,x_scale,y_scale,0,0);
    //cout<<x_scale<<' '<<min_y<<' '<<max_y<<' ';
    //cout<<y_scale;
    setcolor(WHITE);
    for(int i=1; i<=fIndex[currWindow]; i++)
        CreateGraph(A,B,x_scale,y_scale,0,0,i);
    double A1=A,B1=B;
    while(1)
    {
        char c=getch();
        if(c!='a' && c!='d')
            break;
        cleardevice();

        if(c=='a')
        {
            A1-=0.2, B1-=0.2, MaximizedInt(A1,B1);
        } ///left
        else
        {
            A1+=0.2, B1+=0.2, MaximizedInt(A1,B1);
        } ///right
    }
}

int NumberToText(double value,char c[],int lg)
{
    value=1.0*value*1000;
    int int_value=value;
    int st=lg,dr;
    if(int_value<0)
        c[lg++]='-', int_value*=-1;
    if(int_value%10>=5)
        int_value=int_value/10+1;
    else
        int_value=int_value/10;
    int contor=0;
    if(!int_value && c[lg-1]=='-')
        lg--;
    while(int_value)
    {
        c[lg++]=int_value%10+'0';
        int_value/=10;
        contor++;
        if(contor==2)
            c[lg++]='.';
    }
    if(c[lg-1]=='.')
        c[lg++]='0';
    else if(contor==1)
        c[lg++]='0',c[lg++]='.',c[lg++]='0';
    else if(!contor)
        c[lg++]='0';
    dr=lg-1;
    if(c[st]=='-')
        st++;
    for(; st<dr; st++,dr--)
        swap(c[st],c[dr]);
    c[lg]=0;
    return lg;
}

double derivative(functie E, double x)
{
    return (ValoareFunctie(E,x+epsi)-ValoareFunctie(E,x-epsi))/(2*epsi);
}
void getSet()
{
            int discontinuity=0, num=0,neg=0;
            for(int j=1;j<=fIndex[currWindow];j++)
                for(int i=0;F[currWindow][j].expresie[i];i++)
                {
                    if(F[currWindow][j].expresie[i]=='<' || F[currWindow][j].expresie[i]=='=' || F[currWindow][j].expresie[i]=='>')
                            discontinuity=1, num=0;
                    else
                    if(discontinuity==1)
                        {
                           if(F[currWindow][j].expresie[i]!='(')
                            {
                                if(F[currWindow][j].expresie[i]=='-')
                                    neg=1;
                                else
                                {
                                    if(isdigit(F[currWindow][j].expresie[i]))
                                        num=num*10+F[currWindow][j].expresie[i]-'0';
                                    else
                                    {
                                        if(neg==1)
                                            num*=-1;
                                        inserare(j,num), discontinuity=0,neg=0;
                                    }
                                }
                            }
                        }
                }
}


void InputBox(int x1, int y1, int &x2, int y2, char f[200], int &cnt, int widthch, int xBoxOffset, int yBoxOffset, bool text)
{
    int boxlg, i=0;
    char c;
    bool exec=0;
    yBoxOffset++;
    if(text)
    {
        boxlg=15*widthch+2*xBoxOffset;
    }
    else
    {
        boxlg=5*widthch+2*xBoxOffset;
    }
    while(posx>=x1 && posx<=x2 && posy>=y1 && posy<=y2)
    {
        //cout<<screen[2].box[1].insText<<'\n';
        posx=x1;
        posy=y1;
        /*
        while(kbhit()==0) /// INDICATOR SCRIERE
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                getmouseclick(WM_LBUTTONDOWN, posx, posy);
                break;
            }
            outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll*(scroll>=0));
            delay(100);
            setfillstyle(0, WHITE);
            line(x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+2, y1+yBoxOffset, x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+2, y2-yBoxOffset);
            delay(100);
            bar(x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset-1+2, y1+yBoxOffset-1, x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+1+2, y2-yBoxOffset+1);
            outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll*(scroll>=0));
        }*/
        if(kbhit()==0)
        {
            time_t timestamp = time(&timestamp);
            struct tm datetime = *localtime(&timestamp);
            int sec=datetime.tm_sec;
            int aux=sec%2;
            while(kbhit()==0) /// INDICATOR SCRIERE
            {
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    getmouseclick(WM_LBUTTONDOWN, posx, posy);
                    bar(x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset-1+2, y1+yBoxOffset-1, x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+1+2, y2-yBoxOffset+1);
                    break;
                }
                outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll*(scroll>=0));
               // delay(50);
                time_t timestamp = time(&timestamp);
                struct tm datetime = *localtime(&timestamp);
                int sec=datetime.tm_sec;
           //     cout<<sec<<' ';
                if(sec%2==aux){
                    setfillstyle(0, WHITE);
                    line(x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+2, y1+yBoxOffset, x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+2, y2-yBoxOffset);
                }
                else
                    bar(x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset-1+2, y1+yBoxOffset-1, x1+(cnt-scroll+1*(scroll>0))*widthch+xBoxOffset+1+2, y2-yBoxOffset+1);
                outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll*(scroll>=0));
            }
        }

        if(enter==1 || !(posx>=x1 && posx<=x2 && posy>=y1 && posy<=y2))
        {
            //cout<<1;
            break;
        }
        if(kbhit()==1)
        {
            //cout<<cnt;
            //cout<<f<<'\n';
            c=getch();
            if(c==13) ///ENTER
            {
                enter=1;
                break;
            }
            else if(c==8) ///BACKSPACE
            {
                if(cnt>0)
                {
                    cnt--;
                    f[cnt]='\0';
                    if(textwidth(f)>=boxlg)
                    {
                        if(scroll>0)
                        {
                            scroll--;
                        }
                        else
                        {
                            x2=x2-widthch;
                        }
                        /*else
                        {
                            scroll--;
                        }*/
                        bar(/*x2-xBoxOffset*/x1+1, y1, x2+2*widthch, y2+1);
                        rectangle(x1, y1, x2, y2);
                        outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll);
                    }
                    else
                    {
                        bar(x1+1, y1+1, x2, y2);
                        outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll);
                    }
                }
            }
            else ///ALTA TASTA
            {
                f[cnt++]=c;
                f[cnt]='\0';
                if(textwidth(f)+2*xBoxOffset>boxlg)
                {
                    if((x2-xBoxOffset)+widthch<window[1].width-widthch)
                    {
                        //cout<<1<<'\n';
                        x2=x2+widthch;
                    }
                    rectangle(x1, y1, x2, y2);
                    bar(x1+1, y1+1, x2, y2);
                    if(cnt>0)
                    {
                        if((x2-xBoxOffset)+widthch<window[1].width-widthch)
                        {
                            outtextxy(x1+xBoxOffset, y1+yBoxOffset, f);
                        }
                        else
                        {
                            //cout<<2<<'\n';
                            scroll++;
                            outtextxy(x1+xBoxOffset, y1+yBoxOffset, f+scroll);
                        }
                    }
                }
            }
        }
    }
}

/// CONSTRUIESTE VALOAREA NUMERICA PENTRU A, B, DIST
bool BuildValue(double &x, char f[200], int cnt)
{
    bool fractionar=0;
    int p10=1;
    double xfract=0;
    wrongi=0;
    if(strlen(f)==0)
    {
        wrongi=1;
        return 0;
    }
    for(int i=0; i<cnt; i++)
    {
        if(f[i]!='-' && f[i]!='.' && f[i]!=',' && strchr("0123456789", f[i])==NULL && strlen(f)>0)
        {
            wrongi=1;
            return 0;
        }
        if(f[i]=='-')
        {
            continue;
        }
        if(f[i]=='.' || f[i]==',')
        {
            fractionar=1;
            continue;
        }
        if(fractionar==0)
        {
            x=x*10+(f[i]-'0');
        }
        else
        {
            xfract=xfract*10+(f[i]-'0');
            p10*=10;
        }
    }
    if(fractionar==1)
    {
        x+=xfract/p10;
    }
    if(f[0]=='-')
    {
        x=x*-1;
    }
}

void adaptExpr(char s[200], int &lg)
{
    int i, j, firstLg;
    char aux[200], aux2[20];
    double left=0, right=0;
    for(i=0; i<=lg; i++)
    {
        if(s[i]=='(')
        {
            if(s[i+1]=='-')
            {
                aux[0]='\0';
                strcpy(aux, "0-");
                strcat(aux, s+i+2);
                s[i+1]='\0';
                strcat(s, aux);
                lg++;
                i++;
            }
            else if(s[i+1]=='+')
            {
                aux[0]='\0';
                strcpy(aux, "0+");
                strcat(aux, s+i+2);
                s[i+1]='\0';
                strcat(s, aux);
                lg++;
                i++;
            }
        }
        if(s[i]=='!')
        {
            aux2[0]='\0';
            j=i+1;
            while(strchr("1234567890", s[j]))
            {
                aux2[j-i-1]=s[j];
                j++;
            }
            //aux2[0]='\0';
            BuildValue(left, aux2, strlen(aux2));
            right=left+0.02;
            left-=0.02;
            NumberToText(left, aux2, 0);
            aux[0]='\0';
            strcpy(aux, "(X<");
            strcat(aux, aux2);
            strcat(aux, ")||(X>");
            NumberToText(right, aux2, 0);
            strcat(aux, aux2);
            strcat(aux, ")");
            strcat(aux, s+j);
            s[i-1]='\0';
            strcat(s, aux);
            lg+=8+strlen(aux2);
            i++;
        }
        /*if(strchr(Operatii, s[i])!=NULL)
        {
            if(s[i+1]=='-')
            {
                aux[0]='\0';
                strcpy(aux, "0-");
                strcat(aux, s+i+2);
                s[i+1]='\0';
                strcat(s, aux);
                lg++; i++;
            }
            else if(s[i+1]=='+')
            {
                aux[0]='\0';
                strcpy(aux, "0+");
                strcat(aux, s+i+2);
                s[i+1]='\0';
                strcat(s, aux);
                lg++; i++;
            }
        }*/
        /*if((s[i]=='<' || s[i]=='>') && s[i+1]=='=') /// ((X>=1)*sinX)  ----->  ((X>1)*sinX+(X=1)*sinX)
        {                                           /// (sinX*(X>=1))
            firstLg=strlen(s);
            aux[0]='\0';
            strcpy(aux, s+i+1);
            s[firstLg-1]='+';
            //strcat(s, s);
            cout<<s;
        }*/
    }
    //cout<<s<<' '<<lg;
}

void AddToVect(int index)
{
    //cout<<1;
    //cout<<currWindow<<' ';
    //cout<<F[currWindow].expresie<<'\n';
    //cout<<F[currWindow].expresie[1];
    int i, j;
    bool enterwhile=0;
    cntvect=0;
    wrongf=0;
    for(i=0; F[currWindow][index].expresie[i]; i++)
    {
        //cout<<F[currWindow].expresie[i]<<' ';
        j=0;
        enterwhile=0;
        //cout<<exprLgUnmod;
        if((F[currWindow][index].expresie[i]!='X' && F[currWindow][index].expresie[i]!='(' && F[currWindow][index].expresie[i]!=')' && F[currWindow][index].expresie[i]!='.' && strchr(Operatii, F[currWindow][index].expresie[i])==NULL &&
           strchr("0123456789", F[currWindow][index].expresie[i])==NULL && F[currWindow][index].expresie[i]!='q') || F[currWindow][index].expresie[0]!='(')
        {
            //cout<<F.expresie[i]<<'\n';
            wrongf=1;
            break;
        }
        /*if(((F.expresie[i-1]=='(' && F.expresie[i]=='-') || (strchr(OperatiiBinare, F.expresie[i-1])!=NULL && F.expresie[i]=='-')) &&
            ((F.expresie[i+1]=='X' || strchr("0123456789", F.expresie[i+1])!=NULL || strchr(OperatiiUnare, F.expresie[i+1])!=NULL)))
        {
            F.vect[cntvect][j++]='-';
            i++;
            F.lung--;
            if(F.expresie[i]=='X' || strchr(OperatiiUnare, F.expresie[i])!=NULL)
            {
                enterwhile=1;
                F.vect[cntvect][j]=F.expresie[i++];
            }
        }*/
        while((strchr("0123456789", F[currWindow][index].expresie[i])!=NULL || F[currWindow][index].expresie[i]=='.')) ///ADAUGAM NUMERE, NU DOAR CIFRE
        {
            enterwhile=1;
            F[currWindow][index].vect[cntvect][j++]=F[currWindow][index].expresie[i++];
        }
        if(enterwhile)
        {
            i--;
        }
        else
        {
            F[currWindow][index].vect[cntvect][0]=F[currWindow][index].expresie[i];
        }
        if(F[currWindow][index].expresie[i]=='c')
        {
            if(F[currWindow][index].expresie[i+1]=='t')
            {
                F[currWindow][index].vect[cntvect][1]=F[currWindow][index].expresie[i+1];
            }
        }
        if(F[currWindow][index].expresie[i]=='a' && F[currWindow][index].expresie[i+1]=='r')
        {
            if(F[currWindow][index].expresie[i+3]=='c' && F[currWindow][index].expresie[i+4]=='t')
            {
                F[currWindow][index].vect[cntvect][1]=F[currWindow][index].expresie[i+3];
                F[currWindow][index].vect[cntvect][2]=F[currWindow][index].expresie[i+4];
            }
            else
            {
                F[currWindow][index].vect[cntvect][1]=F[currWindow][index].expresie[i+3];
            }
        }
        cntvect++;
        if(F[currWindow][index].expresie[i]=='a')
        {
            if(F[currWindow][index].expresie[i+1]=='r')
            {
                if(F[currWindow][index].expresie[i+3]=='t')
                {
                    if(F[currWindow][index].expresie[i+2]!='c' || F[currWindow][index].expresie[i+4]!='g')
                    {
                        wrongf=1;
                        break;
                    }
                    i+=4;
                    F[currWindow][index].lung-=4;
                }
                else
                {
                    if(F[currWindow][index].expresie[i+2]!='c' ||
                      (F[currWindow][index].expresie[i+3]=='s' && (F[currWindow][index].expresie[i+4]!='i' || F[currWindow][index].expresie[i+5]!='n')) ||
                      ((F[currWindow][index].expresie[i+3]=='c' && (F[currWindow][index].expresie[i+4]!='o' || F[currWindow][index].expresie[i+5]!='s')) && (F[currWindow][index].expresie[i+3]=='c' && (F[currWindow][index].expresie[i+4]!='t' || F[currWindow][index].expresie[i+5]!='g'))))
                    {
                        wrongf=1;
                        break;
                    }
                    i+=5;
                    F[currWindow][index].lung-=5;
                }
            }
            else
            {
                if(F[currWindow][index].expresie[i]=='a' && (F[currWindow][index].expresie[i+1]!='b' || F[currWindow][index].expresie[i+2]!='s'))
                {
                    wrongf=1;
                    break;
                }
                i+=2;
                F[currWindow][index].lung-=2;
            }
        }
        else if(F[currWindow][index].expresie[i]=='s' || F[currWindow][index].expresie[i]=='c' || F[currWindow][index].expresie[i]=='r')
        {
            if((F[currWindow][index].expresie[i]=='s' && (F[currWindow][index].expresie[i+1]!='i' || F[currWindow][index].expresie[i+2]!='n')) ||
               (F[currWindow][index].expresie[i]=='c' && (F[currWindow][index].expresie[i+1]!='o' || F[currWindow][index].expresie[i+2]!='s')) ||
               (F[currWindow][index].expresie[i]=='r' && (F[currWindow][index].expresie[i+1]!='a' || F[currWindow][index].expresie[i+2]!='d')))
            {
                wrongf=1;
                break;
            }
            i+=2;
            F[currWindow][index].lung-=2;
        }
        else if(F[currWindow][index].expresie[i]=='l' || F[currWindow][index].expresie[i]=='t' || F[currWindow][index].expresie[i]=='|')
        {
            if((F[currWindow][index].expresie[i]=='l' && F[currWindow][index].expresie[i+1]!='n') ||
               (F[currWindow][index].expresie[i]=='t' && F[currWindow][index].expresie[i+1]!='g') ||
               (F[currWindow][index].expresie[i]=='|' && F[currWindow][index].expresie[i+1]!='|'))
            {
                wrongf=1;
                break;
            }
            i++;
            F[currWindow][index].lung--;
        }
    }
}

bool HoverClick(int x1, int y1, int x2, int y2, int tx, int ty, char text[100], int textlg, int widthch, int heightch,
                int s1, int s2, int next, int hovercolor, int color)
{
    int coordsx, coordsy, i, j;
    char c;
    FILE *fp;
    exitf=0;
    while(((mousex()>=x1 && mousex()<=x2 && mousey()>=y1 && mousey()<=y2) && exitf==0) || enter==1)
    {
        if(s1==5 && s2==5 && next<themeNr+2 && next!=-1)
        {
            if(next==0)
            {
                if(theme[themeNr].bordercolor==WHITE)
                {
                    setcolor(BLACK);
                    line(screen[5].box[1].x2-15, screen[5].box[1].y1+7, screen[5].box[1].x2-5, screen[5].box[1].y1+(screen[5].box[1].y2-screen[5].box[1].y1)/2);
                    line(screen[5].box[1].x2-15, screen[5].box[1].y2-7, screen[5].box[1].x2-5, screen[5].box[1].y1+(screen[5].box[1].y2-screen[5].box[1].y1)/2);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(WHITE);
                    line(screen[5].box[1].x2-15, screen[5].box[1].y1+7, screen[5].box[1].x2-5, screen[5].box[1].y1+(screen[5].box[1].y2-screen[5].box[1].y1)/2);
                    line(screen[5].box[1].x2-15, screen[5].box[1].y2-7, screen[5].box[1].x2-5, screen[5].box[1].y1+(screen[5].box[1].y2-screen[5].box[1].y1)/2);
                    setcolor(BLACK);
                }
            }
            else if(next>0 && expand==1)
            {
                if(theme[themeNr].bordercolor==WHITE)
                {
                    setcolor(BLACK);
                    circle(screen[5].box[next].x1+(screen[5].box[next].x2-screen[5].box[next].x1)/2, screen[5].box[next].y1+(screen[5].box[next].y2-screen[5].box[next].y1)/2, 5);
                    setcolor(WHITE);
                }
                else
                {
                    setcolor(WHITE);
                    circle(screen[5].box[next].x1+(screen[5].box[next].x2-screen[5].box[next].x1)/2, screen[5].box[next].y1+(screen[5].box[next].y2-screen[5].box[next].y1)/2, 5);
                    setcolor(BLACK);
                }
            }
        }
        else
        {
            setfillstyle(1, hovercolor);
            setbkcolor(hovercolor);
            bar(x1+1, y1+1, tx, y2);
            bar(x1+1, ty+heightch, x2, y2);
            bar(tx+textlg*widthch+1, y1+1, x2, y2);
            bar(x1+1, y1+1, x2, ty+1);
            rectangle(x1, y1, x2, y2);
            outtextxy(tx, ty, text);
        }

        if(ismouseclick(WM_LBUTTONDOWN) || enter==1)
        {
            getmouseclick(WM_LBUTTONDOWN, coordsx, coordsy);
            if(coordsx>=x1 && coordsx<=x2 && coordsy>=y1 && coordsy<=y2)
            {
                if(sound==1)
                {
                PlaySound(wavFile, NULL, SND_FILENAME | SND_ASYNC);
                }
                if(s2==0)
                {
                    closegraph();
                    exit(0);
                }
                else if(s1==s2)
                {
                    if(s1==4)
                    {
                        refresh=1;
                        if(next==-1)
                        {
                            pageNr--;
                        }
                        else if(next==1)
                        {
                            pageNr++;
                        }
                    }
                    else if(s1==5)
                    {
                        refresh=1;
                        if(next==0)
                        {
                            expand=1-expand;
                        }
                        if(next>0 && next<themeNr+2)
                        {
                            //expand=1-expand;
                            themeCnt=next-1;
                        }
                        if(next>=themeNr+2)
                        {
                            for(i=0; i<langNr; i++)
                            {
                                if(next==themeNr+2+i)
                                {
                                    langCnt=i;
                                }
                            }
                        }
                        if(next==-1)
                        {
                            sound=1-sound;
                        }
                    }
                }
                else
                {
                    if(s1==4 && s2==2)
                    {
                        strcpy(screen[2].box[1].insText, screen[4].box[next].expr);
                        screen[2].box[1].insTextLg=strlen(screen[2].box[1].insText);
                        strcpy(screen[2].box[2].insText, screen[4].box[next].a);
                        screen[2].box[2].insTextLg=strlen(screen[2].box[2].insText);
                        strcpy(screen[2].box[3].insText, screen[4].box[next].b);
                        screen[2].box[3].insTextLg=strlen(screen[2].box[3].insText);
                        strcpy(screen[2].box[4].insText, screen[4].box[next].dist);
                        screen[2].box[4].insTextLg=strlen(screen[2].box[4].insText);
                    }
                    else if(s1==4 && s2==3)
                    {
                        windowNr++;
                        window[windowNr].width=getmaxwidth();
                        window[windowNr].height=getmaxheight();
                        window[windowNr].screen=3;
                        sprintf(window[windowNr].title, "Graficul %d", windowNr-1);
                        window[windowNr].id=initwindow(window[windowNr].width, window[windowNr].height, window[windowNr].title, 0, 0);
                        currWindow=windowNr-1;

                        cout<<cnt1<<' '<<screen[4].box[cnt1].fNr<<'\n';
                        for(i=1; i<=screen[4].box[cnt1].fNr; i++)
                        {
                            fIndex[currWindow]++;
                            F[currWindow][i].a=F[currWindow][i].b=F[currWindow][i].dist=0;
                            strcpy(F[currWindow][i].expresie, screen[4].box[next].expr);
                            //cout<<windowNr<<' '<<F[windowNr].expresie;
                            F[currWindow][i].lung=strlen(F[currWindow][i].expresie)-1;
                            //cout<<F[currWindow][i].expresie<<' ';
                            exprLgUnmod=F[currWindow][i].lung;
                            adaptExpr(F[currWindow][i].expresie, F[currWindow][i].lung);
                            AddToVect(i);
                            BuildValue(F[currWindow][i].a, screen[4].box[next].a, strlen(screen[4].box[next].a)-1);
                            BuildValue(F[currWindow][i].b, screen[4].box[next].b, strlen(screen[4].box[next].b)-1);
                            BuildValue(F[currWindow][i].dist, screen[4].box[next].dist, strlen(screen[4].box[next].dist));
                            //cout<<F[currWindow][i].a<<' '<<F[currWindow][i].b<<' '<<F[currWindow][i].dist<<'\n';
                            next++;
                        }
                    }
                    else if(s1==2 && s2==3 && wrongf==0 && wrongi==0 && cnt!=0)
                    {
                        windowNr++;
                        window[windowNr].width=getmaxwidth();
                        window[windowNr].height=getmaxheight();
                        //window[1].screen=2;
                        window[windowNr].screen=3;
                        sprintf(window[windowNr].title, "Graficul %d", windowNr-1);
                        window[windowNr].id=initwindow(window[windowNr].width, window[windowNr].height, window[windowNr].title, 0, 0);
                        currWindow=windowNr-1;

                        if(cnt+functionNr<=25 && cnt!=0)
                        {
                            //cout<<1;
                            fp=fopen("functii.txt", "a");
                            fprintf(fp, "%d \n", cnt);
                            fprintf(fp, "%d%s ", sepFNr, ".");
                            for(i=1; i<=cnt; i++)
                            {
                                fprintf(fp, "%s %.2f %.2f %.2f\n", auxExpr[i], F[currWindow][i].a, F[currWindow][i].b, F[currWindow][i].dist);
                            }
                            fclose(fp);
                            fp=fopen("functii.txt", "r");
                            functionCnt=1;
                            j=1;
                            while(fscanf(fp, "%hu", &screen[4].box[j].fNr)==1)
                            {
                                fscanf(fp, "%s %s %s %s %s", &screen[4].box[functionCnt].ind, &screen[4].box[functionCnt].expr, &screen[4].box[functionCnt].a, &screen[4].box[functionCnt].b, &screen[4].box[functionCnt].dist);
                                //cout<<screen[4].box[functionCnt].expr<<'\n';
                                functionTextBuild();
                                functionCnt++;
                                for(i=1; i<screen[4].box[j].fNr; i++)
                                {
                                    fscanf(fp, "%s %s %s %s", &screen[4].box[functionCnt].expr, &screen[4].box[functionCnt].a, &screen[4].box[functionCnt].b, &screen[4].box[functionCnt].dist);
                                    functionTextBuild();
                                    functionCnt++;
                                }
                                j++;
                            }
                            functionNr+=cnt;
                            sepFNr++;
                            fclose(fp);
                        }
                        //cout<<F[currWindow][fIndex[currWindow]].expresie<<' '<<F[currWindow][fIndex[currWindow]].a<<' '<<F[currWindow][fIndex[currWindow]].b<<' '<<F[currWindow][fIndex[currWindow]].dist<<'\n';
                        //getSet();
                    }
                    else if(s1==2 && s2==1)
                    {
                        wrongf=0;
                        wrongi=0;
                    }
                    /*if(s1==3 && s2==1)
                    {
                        screen[2].box[1].insText[0]=screen[2].box[2].insText[0]=screen[2].box[3].insText[0]=screen[2].box[4].insText[0]='\0';
                        screen[2].box[1].insTextLg=screen[2].box[2].insTextLg=screen[2].box[3].insTextLg=screen[2].box[4].insTextLg=0;
                        F[windowNr].a=F[windowNr].b=F[windowNr].dist=0;
                    }*/
                    if(s2==3 && wrongf==0 && wrongi==0 && cnt!=0)
                    {
                        window[1].screen=s1;
                    }
                    else if(s2!=3 && wrongf==0 && wrongi==0)
                    {

                        window[1].screen=s2;
                        screen[s1].active=0;
                        screen[s2].active=1;
                    }
                }
                exitf=1;
            }
        }
    }
    if(s1!=5 || s2!=5 || next>=themeNr+2 || next==-1)
    {
        setfillstyle(1, color);
        setbkcolor(color);
        bar(x1+1, y1+1, tx, y2);
        bar(x1+1, ty+heightch, x2, y2);
        bar(tx+textlg*widthch+1, y1+1, x2, y2);
        bar(x1+1, y1+1, x2, ty+1);
        rectangle(x1, y1, x2, y2);
        outtextxy(tx, ty, text);
    }
}

bool Screen1(int &posx, int &posy, bool &drawn) ///MENIU
{
    int titleHeightCh=heightch[4], titleHeightOffset=50, bottomOffset=100, halfBoxLenght=200, boxHeight=50, xBoxOffset=30,
        buttonNr=5, buttonDiff, i, j, longest, maxx=-1;
    //setlinestyle(SOLID_LINE, 0, 1);
    if(langCnt==0)
    {
        strcpy(screen[1].title, "Graficul unei functii");
        screen[1].titleLg=strlen(screen[1].title);
        strcpy(screen[1].box[1].text, "Functie noua");
        screen[1].box[1].textLg=strlen(screen[1].box[1].text);
        strcpy(screen[1].box[2].text, "Istoric");
        screen[1].box[2].textLg=strlen(screen[1].box[2].text);
        strcpy(screen[1].box[3].text, "Instructiuni");
        screen[1].box[3].textLg=strlen(screen[1].box[3].text);
        strcpy(screen[1].box[4].text, "Setari");
        screen[1].box[4].textLg=strlen(screen[1].box[4].text);
        strcpy(screen[1].box[5].text, "Inchidere program");
        screen[1].box[5].textLg=strlen(screen[1].box[5].text);
    }
    else if(langCnt==1)
    {
        strcpy(screen[1].title, "Function Graph");
        screen[1].titleLg=strlen(screen[1].title);
        strcpy(screen[1].box[1].text, "New function");
        screen[1].box[1].textLg=strlen(screen[1].box[1].text);
        strcpy(screen[1].box[2].text, "History");
        screen[1].box[2].textLg=strlen(screen[1].box[2].text);
        strcpy(screen[1].box[3].text, "Instructions");
        screen[1].box[3].textLg=strlen(screen[1].box[3].text);
        strcpy(screen[1].box[4].text, "Settings");
        screen[1].box[4].textLg=strlen(screen[1].box[4].text);
        strcpy(screen[1].box[5].text, "Close program");
        screen[1].box[5].textLg=strlen(screen[1].box[5].text);
    }
    else if(langCnt==2)
    {
        strcpy(screen[1].title, "Graphique d'une fonction");
        screen[1].titleLg=strlen(screen[1].title);
        strcpy(screen[1].box[1].text, "Nouvelle fonction");
        screen[1].box[1].textLg=strlen(screen[1].box[1].text);
        strcpy(screen[1].box[2].text, "Historique");
        screen[1].box[2].textLg=strlen(screen[1].box[2].text);
        strcpy(screen[1].box[3].text, "Instructions");
        screen[1].box[3].textLg=strlen(screen[1].box[3].text);
        strcpy(screen[1].box[4].text, "Parametres");
        screen[1].box[4].textLg=strlen(screen[1].box[4].text);
        strcpy(screen[1].box[5].text, "Programme de clôture");
        screen[1].box[5].textLg=strlen(screen[1].box[5].text);
    }
    else if(langCnt==3)
    {
        strcpy(screen[1].title, "Der graph einer funktion");
        screen[1].titleLg=strlen(screen[1].title);
        strcpy(screen[1].box[1].text, "Neue funktion");
        screen[1].box[1].textLg=strlen(screen[1].box[1].text);
        strcpy(screen[1].box[2].text, "Historisch");
        screen[1].box[2].textLg=strlen(screen[1].box[2].text);
        strcpy(screen[1].box[3].text, "Anweisungen");
        screen[1].box[3].textLg=strlen(screen[1].box[3].text);
        strcpy(screen[1].box[4].text, "Einstellungen");
        screen[1].box[4].textLg=strlen(screen[1].box[4].text);
        strcpy(screen[1].box[5].text, "Abschlussprogramm");
        screen[1].box[5].textLg=strlen(screen[1].box[5].text);
    }
    else if(langCnt==4)
    {
        strcpy(screen[1].title, "La gráfica de una función");
        screen[1].titleLg=strlen(screen[1].title);
        strcpy(screen[1].box[1].text, "Nueva función");
        screen[1].box[1].textLg=strlen(screen[1].box[1].text);
        strcpy(screen[1].box[2].text, "Histórico");
        screen[1].box[2].textLg=strlen(screen[1].box[2].text);
        strcpy(screen[1].box[3].text, "Instrucciones");
        screen[1].box[3].textLg=strlen(screen[1].box[3].text);
        strcpy(screen[1].box[4].text, "Ajustes");
        screen[1].box[4].textLg=strlen(screen[1].box[4].text);
        strcpy(screen[1].box[5].text, "Programa de cierre");
        screen[1].box[5].textLg=strlen(screen[1].box[5].text);
    }

    for(i=1; i<=buttonNr; i++)
    {
        if(screen[1].box[i].textLg>maxx)
        {
            maxx=screen[1].box[i].textLg;
            longest=i;
        }
    }

    halfBoxLenght=((widthch[4]*screen[1].box[longest].textLg)*(screen[1].box[longest].textLg>=15)+(15*widthch[4])*(screen[1].box[longest].textLg<15))/2+xBoxOffset;

    buttonDiff=(window[1].height-titleHeightOffset-titleHeightCh-bottomOffset*4)/buttonNr;

    screen[1].box[1].x1=window[1].width/2-halfBoxLenght, screen[1].box[1].y1=titleHeightOffset+titleHeightCh+bottomOffset, screen[1].box[1].x2=window[1].width/2+halfBoxLenght, screen[1].box[1].y2=screen[1].box[1].y1+boxHeight,
                     screen[1].box[1].textx=((screen[1].box[1].x2-screen[1].box[1].x1)-screen[1].box[1].textLg*widthch[4])/2+screen[1].box[1].x1, screen[1].box[1].texty=((screen[1].box[1].y2-screen[1].box[1].y1)-heightch[4])/2+screen[1].box[1].y1;

    screen[1].box[2].x1=window[1].width/2-halfBoxLenght, screen[1].box[2].y1=screen[1].box[1].y2+buttonDiff, screen[1].box[2].x2=window[1].width/2+halfBoxLenght, screen[1].box[2].y2=screen[1].box[2].y1+boxHeight,
                     screen[1].box[2].textx=((screen[1].box[2].x2-screen[1].box[2].x1)-screen[1].box[2].textLg*widthch[4])/2+screen[1].box[2].x1, screen[1].box[2].texty=((screen[1].box[2].y2-screen[1].box[2].y1)-heightch[4])/2+screen[1].box[2].y1;

    screen[1].box[3].x1=window[1].width/2-halfBoxLenght, screen[1].box[3].y1=screen[1].box[2].y2+buttonDiff, screen[1].box[3].x2=window[1].width/2+halfBoxLenght, screen[1].box[3].y2=screen[1].box[3].y1+boxHeight,
                     screen[1].box[3].textx=((screen[1].box[3].x2-screen[1].box[3].x1)-screen[1].box[3].textLg*widthch[4])/2+screen[1].box[3].x1, screen[1].box[3].texty=((screen[1].box[3].y2-screen[1].box[3].y1)-heightch[4])/2+screen[1].box[3].y1;

    screen[1].box[4].x1=window[1].width/2-halfBoxLenght, screen[1].box[4].y1=screen[1].box[3].y2+buttonDiff, screen[1].box[4].x2=window[1].width/2+halfBoxLenght, screen[1].box[4].y2=screen[1].box[4].y1+boxHeight,
                     screen[1].box[4].textx=((screen[1].box[4].x2-screen[1].box[4].x1)-screen[1].box[4].textLg*widthch[4])/2+screen[1].box[4].x1, screen[1].box[4].texty=((screen[1].box[4].y2-screen[1].box[4].y1)-heightch[4])/2+screen[1].box[4].y1;

    screen[1].box[5].x1=window[1].width/2-halfBoxLenght, screen[1].box[5].y1=screen[1].box[4].y2+buttonDiff, screen[1].box[5].x2=window[1].width/2+halfBoxLenght, screen[1].box[5].y2=screen[1].box[5].y1+boxHeight,
                     screen[1].box[5].textx=((screen[1].box[5].x2-screen[1].box[5].x1)-screen[1].box[5].textLg*widthch[4])/2+screen[1].box[5].x1, screen[1].box[5].texty=((screen[1].box[5].y2-screen[1].box[5].y1)-heightch[4])/2+screen[1].box[5].y1;

    //setactivepage(page);
    //setvisualpage(1-page);

    //cleardevice();

    ///TITLU
    outtextxy(window[1].width/2-screen[1].titleLg/2*widthch[4], titleHeightOffset, screen[1].title);

    ///DESENARE CASETA I - "FUNCTIE:"
    rectangle(screen[1].box[1].x1, screen[1].box[1].y1, screen[1].box[1].x2, screen[1].box[1].y2);
    outtextxy(screen[1].box[1].textx, screen[1].box[1].texty, screen[1].box[1].text);

    ///DESENARE CASETA II - "ISTORIC"
    rectangle(screen[1].box[2].x1, screen[1].box[2].y1, screen[1].box[2].x2, screen[1].box[2].y2);
    outtextxy(screen[1].box[2].textx, screen[1].box[2].texty, screen[1].box[2].text);

    ///DESENARE CASETA III - "INSTRUCTIUNI"
    rectangle(screen[1].box[3].x1, screen[1].box[3].y1, screen[1].box[3].x2, screen[1].box[3].y2);
    outtextxy(screen[1].box[3].textx, screen[1].box[3].texty, screen[1].box[3].text);

    ///DESENARE CASETA IV - "SETARI"
    rectangle(screen[1].box[4].x1, screen[1].box[4].y1, screen[1].box[4].x2, screen[1].box[4].y2);
    outtextxy(screen[1].box[4].textx, screen[1].box[4].texty, screen[1].box[4].text);

    ///DESENARE CASETA V - "INCHIDERE PROGRAM"
    rectangle(screen[1].box[5].x1, screen[1].box[5].y1, screen[1].box[5].x2, screen[1].box[5].y2);
    outtextxy(screen[1].box[5].textx, screen[1].box[5].texty, screen[1].box[5].text);

    //setactivepage(1-page);

    HoverClick(screen[1].box[1].x1, screen[1].box[1].y1, screen[1].box[1].x2, screen[1].box[1].y2, screen[1].box[1].textx, screen[1].box[1].texty,
               screen[1].box[1].text, screen[1].box[1].textLg, widthch[4], heightch[4], 1, 2, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        settextstyle(8, 0, 4);
        return 0;
    }
    HoverClick(screen[1].box[2].x1, screen[1].box[2].y1, screen[1].box[2].x2, screen[1].box[2].y2, screen[1].box[2].textx, screen[1].box[2].texty,
               screen[1].box[2].text, screen[1].box[2].textLg, widthch[4], heightch[4], 1, 4, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        settextstyle(8, 0, 2);
        return 0;
    }
    HoverClick(screen[1].box[3].x1, screen[1].box[3].y1, screen[1].box[3].x2, screen[1].box[3].y2, screen[1].box[3].textx, screen[1].box[3].texty,
               screen[1].box[3].text, screen[1].box[3].textLg, widthch[4], heightch[4], 1, 6, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        settextstyle(8, 0, 4);
        return 0;
    }
    HoverClick(screen[1].box[4].x1, screen[1].box[4].y1, screen[1].box[4].x2, screen[1].box[4].y2, screen[1].box[4].textx, screen[1].box[4].texty,
               screen[1].box[4].text, screen[1].box[4].textLg, widthch[4], heightch[4], 1, 5, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        settextstyle(8, 0, 4);
        return 0;
    }
    HoverClick(screen[1].box[5].x1, screen[1].box[5].y1, screen[1].box[5].x2, screen[1].box[5].y2, screen[1].box[5].textx, screen[1].box[5].texty,
               screen[1].box[5].text, screen[1].box[5].textLg, widthch[4], heightch[4], 1, 0, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        return 0;
    }

    //setactivepage(page);

    page=1-page;
}

bool Screen2(bool &enter) ///INTERFATA INTRODUCERE DATE GRAFIC
{
    int widthOffset=50, heightOffset=30, xBoxOffset, yBoxOffset, titleHeightOffset=50, longer, boxFromTextOffset=15;

    ///TEXT ASOCIAT CASETELOR
    if(langCnt==0)
    {
        strcpy(screen[2].title, "Introducere functie");
        screen[2].titleLg=strlen(screen[2].title);
        strcpy(screen[2].box[1].text, "Functie:");
        screen[2].box[1].textLg=strlen(screen[2].box[1].text);
        strcpy(screen[2].box[2].text, "Interval:");
        screen[2].box[2].textLg=strlen(screen[2].box[2].text);
        strcpy(screen[2].box[3].text, "dreapta:");
        screen[2].box[3].textLg=strlen(screen[2].box[3].text);
        strcpy(screen[2].box[4].text, "Distanta zoom:");
        screen[2].box[4].textLg=strlen(screen[2].box[4].text);
        strcpy(screen[2].box[5].text, "Inapoi");
        screen[2].box[5].textLg=strlen(screen[2].box[5].text);
        strcpy(screen[2].box[6].text, "stanga:");
        screen[2].box[6].textLg=strlen(screen[2].box[6].text);
        strcpy(screen[2].box[7].insText, "Afisare grafic");
        screen[2].box[7].insTextLg=strlen(screen[2].box[7].insText);
    }
    else if(langCnt==1)
    {
        strcpy(screen[2].title, "New function");
        screen[2].titleLg=strlen(screen[2].title);
        strcpy(screen[2].box[1].text, "Function:");
        screen[2].box[1].textLg=strlen(screen[2].box[1].text);
        strcpy(screen[2].box[2].text, "Interval:");
        screen[2].box[2].textLg=strlen(screen[2].box[2].text);
        strcpy(screen[2].box[3].text, "right:");
        screen[2].box[3].textLg=strlen(screen[2].box[3].text);
        strcpy(screen[2].box[4].text, "Zoom distance:");
        screen[2].box[4].textLg=strlen(screen[2].box[4].text);
        strcpy(screen[2].box[5].text, "Back");
        screen[2].box[5].textLg=strlen(screen[2].box[5].text);
        strcpy(screen[2].box[6].text, "left:");
        screen[2].box[6].textLg=strlen(screen[2].box[6].text);
        strcpy(screen[2].box[7].insText, "Show graph");
        screen[2].box[7].insTextLg=strlen(screen[2].box[7].insText);
    }
    else if(langCnt==2)
    {
        strcpy(screen[2].title, "Entrée de fonction");
        screen[2].titleLg=strlen(screen[2].title);
        strcpy(screen[2].box[1].text, "Fonction:");
        screen[2].box[1].textLg=strlen(screen[2].box[1].text);
        strcpy(screen[2].box[2].text, "Intervalle:");
        screen[2].box[2].textLg=strlen(screen[2].box[2].text);
        strcpy(screen[2].box[3].text, "droite:");
        screen[2].box[3].textLg=strlen(screen[2].box[3].text);
        strcpy(screen[2].box[4].text, "Distance de zoom:");
        screen[2].box[4].textLg=strlen(screen[2].box[4].text);
        strcpy(screen[2].box[5].text, "Dos");
        screen[2].box[5].textLg=strlen(screen[2].box[5].text);
        strcpy(screen[2].box[6].text, "gauche:");
        screen[2].box[6].textLg=strlen(screen[2].box[6].text);
        strcpy(screen[2].box[7].insText, "Affichage graphique");
        screen[2].box[7].insTextLg=strlen(screen[2].box[7].insText);
    }
    else if(langCnt==3)
    {
        strcpy(screen[2].title, "Funktionseingabe");
        screen[2].titleLg=strlen(screen[2].title);
        strcpy(screen[2].box[1].text, "Funktion:");
        screen[2].box[1].textLg=strlen(screen[2].box[1].text);
        strcpy(screen[2].box[2].text, "Intervall:");
        screen[2].box[2].textLg=strlen(screen[2].box[2].text);
        strcpy(screen[2].box[3].text, "rechts:");
        screen[2].box[3].textLg=strlen(screen[2].box[3].text);
        strcpy(screen[2].box[4].text, "Zoomentfernung:");
        screen[2].box[4].textLg=strlen(screen[2].box[4].text);
        strcpy(screen[2].box[5].text, "Zurück");
        screen[2].box[5].textLg=strlen(screen[2].box[5].text);
        strcpy(screen[2].box[6].text, "links:");
        screen[2].box[6].textLg=strlen(screen[2].box[6].text);
        strcpy(screen[2].box[7].insText, "Grafische darstellung");
        screen[2].box[7].insTextLg=strlen(screen[2].box[7].insText);
    }
    else if(langCnt==4)
    {
        strcpy(screen[2].title, "Entrada de función");
        screen[2].titleLg=strlen(screen[2].title);
        strcpy(screen[2].box[1].text, "Función:");
        screen[2].box[1].textLg=strlen(screen[2].box[1].text);
        strcpy(screen[2].box[2].text, "Intervalo:");
        screen[2].box[2].textLg=strlen(screen[2].box[2].text);
        strcpy(screen[2].box[3].text, "bien:");
        screen[2].box[3].textLg=strlen(screen[2].box[3].text);
        strcpy(screen[2].box[4].text, "Distancia de zoom:");
        screen[2].box[4].textLg=strlen(screen[2].box[4].text);
        strcpy(screen[2].box[5].text, "Atrás");
        screen[2].box[5].textLg=strlen(screen[2].box[5].text);
        strcpy(screen[2].box[6].text, "izquierda:");
        screen[2].box[6].textLg=strlen(screen[2].box[6].text);
        strcpy(screen[2].box[7].insText, "Pantalla gráfica");
        screen[2].box[7].insTextLg=strlen(screen[2].box[7].insText);
    }

    if(screen[2].box[3].textLg>screen[2].box[6].textLg)
    {
        longer=3;
    }
    else
    {
        longer=6;
    }
    xBoxOffset=widthch[4]/2;
    yBoxOffset=widthch[4]/4;

    screen[2].box[1].x1=widthOffset,
    screen[2].box[1].y1=2*titleHeightOffset+heightOffset+heightch[4]+2*yBoxOffset,
    screen[2].box[1].x2=screen[2].box[1].x1+(15+(screen[2].box[1].insTextLg-15)*(screen[2].box[1].insTextLg>=15))*widthch[4]+2*xBoxOffset,
    screen[2].box[1].y2=screen[2].box[1].y1+heightch[4]+2*yBoxOffset,

    screen[2].box[2].x1=2*widthOffset+screen[2].box[longer].textLg*widthch[4]+boxFromTextOffset,
    screen[2].box[2].y1=screen[2].box[1].y2+4*heightOffset-heightch[4]/2+2*yBoxOffset,
    screen[2].box[2].x2=screen[2].box[2].x1+(5+(screen[2].box[2].insTextLg-5)*(screen[2].box[2].insTextLg>=5))*widthch[4]+2*xBoxOffset,
    screen[2].box[2].y2=screen[2].box[2].y1+heightch[4]+2*yBoxOffset,

    screen[2].box[3].x1=2*widthOffset+screen[2].box[longer].textLg*widthch[4]+boxFromTextOffset,
    screen[2].box[3].y1=screen[2].box[2].y2+heightOffset-heightch[4]/2+2*yBoxOffset,
    screen[2].box[3].x2=screen[2].box[3].x1+(5+(screen[2].box[3].insTextLg-5)*(screen[2].box[3].insTextLg>=5))*widthch[4]+2*xBoxOffset,
    screen[2].box[3].y2=screen[2].box[3].y1+heightch[4]+2*yBoxOffset,

    screen[2].box[4].x1=2*widthOffset+screen[2].box[4].textLg*widthch[4],
    screen[2].box[4].y1=screen[2].box[3].y2+2*heightOffset-heightch[4]/2+2*yBoxOffset,
    screen[2].box[4].x2=screen[2].box[4].x1+(5+(screen[2].box[4].insTextLg-5)*(screen[2].box[4].insTextLg>=5))*widthch[4]+2*xBoxOffset,
    screen[2].box[4].y2=screen[2].box[4].y1+heightch[4]+2*yBoxOffset,

    screen[2].box[5].x1=window[1].width-widthOffset-(screen[2].box[5].textLg*widthch[4])*(screen[2].box[5].textLg>=6)-(6*widthch[4])*(screen[2].box[5].textLg<6)-2*xBoxOffset,
    screen[2].box[5].y1=window[1].height-heightOffset-heightch[4]-2*yBoxOffset,
    screen[2].box[5].x2=window[1].width-widthOffset,
    screen[2].box[5].y2=screen[2].box[5].y1+heightch[4]+2*yBoxOffset;

    screen[2].box[6].x2=screen[2].box[5].x1-widthOffset,
                     screen[2].box[6].x1=screen[2].box[6].x2-2*xBoxOffset-screen[2].box[7].insTextLg*widthch[4];
    screen[2].box[6].y1=window[1].height-heightOffset-heightch[4]-2*yBoxOffset,
                     screen[2].box[6].y2=screen[2].box[6].y1+heightch[4]+2*yBoxOffset;

    while(kbhit()) /// GOLESC INPUT BUFFER-UL
    {
        getch();
    }

    //setactivepage(page);
    //setvisualpage(1-page);
    //setbkcolor(theme[themeCnt].bkcolor);
    //cleardevice();

    ///TITLU
    outtextxy(window[1].width/2-screen[2].titleLg/2*widthch[4], titleHeightOffset, screen[2].title);

    ///DESENARE CASETA I - "FUNCTIE"
    outtextxy(widthOffset, 2*titleHeightOffset+heightOffset, screen[2].box[1].text);
    rectangle(screen[2].box[1].x1, screen[2].box[1].y1, screen[2].box[1].x2, screen[2].box[1].y2);
    outtextxy(screen[2].box[1].x1+xBoxOffset, screen[2].box[1].y1+yBoxOffset, screen[2].box[1].insText+scroll);

    ///DESENARE CASETA II - "INTERVAL:" + "STANGA:"
    outtextxy(widthOffset, screen[2].box[1].y2+2*heightOffset, screen[2].box[2].text);
    outtextxy(2*widthOffset, screen[2].box[1].y2+4*heightOffset, screen[2].box[6].text);
    rectangle(screen[2].box[2].x1, screen[2].box[2].y1, screen[2].box[2].x2, screen[2].box[2].y2);
    outtextxy(screen[2].box[2].x1+xBoxOffset, screen[2].box[2].y1+yBoxOffset, screen[2].box[2].insText);

    ///DESENARE CASETA III - "DREAPTA:"
    //outtextxy(screen[2].box[2].x1-(screen[2].box[2].textLg-9)*widthch[4]-xBoxOffset, 5*heightOffset, screen[2].box[3].text);
    outtextxy(2*widthOffset, screen[2].box[2].y2+heightOffset, screen[2].box[3].text);
    rectangle(screen[2].box[3].x1, screen[2].box[3].y1, screen[2].box[3].x2, screen[2].box[3].y2);
    outtextxy(screen[2].box[3].x1+xBoxOffset, screen[2].box[3].y1+yBoxOffset, screen[2].box[3].insText);

    ///DESENARE CASETA IV - "DISTANTA ZOOM:"
    outtextxy(widthOffset, screen[2].box[3].y2+2*heightOffset, screen[2].box[4].text);
    rectangle(screen[2].box[4].x1, screen[2].box[4].y1, screen[2].box[4].x2, screen[2].box[4].y2);
    outtextxy(screen[2].box[4].x1+xBoxOffset, screen[2].box[4].y1+yBoxOffset, screen[2].box[4].insText);

    rectangle(screen[2].box[6].x1, screen[2].box[6].y1, screen[2].box[6].x2, screen[2].box[6].y2);
    outtextxy(screen[2].box[6].x1+xBoxOffset, screen[2].box[6].y1+yBoxOffset, screen[2].box[7].insText);

    ///DESENARE BUTON INAPOI
    rectangle(screen[2].box[5].x1, screen[2].box[5].y1, screen[2].box[5].x2, screen[2].box[5].y2);
    outtextxy(screen[2].box[5].x1+(screen[2].box[5].x2-screen[2].box[5].x1)/2-(widthch[4]*screen[2].box[5].textLg)/2, screen[2].box[5].y1+(screen[2].box[5].y2-screen[2].box[5].y1)/2-heightch[4]/2, screen[2].box[5].text);

    //setactivepage(1-page);

    if(ismouseclick(WM_LBUTTONDOWN))
    {
        getmouseclick(WM_LBUTTONDOWN, posx, posy);
    }
    InputBox(screen[2].box[1].x1, screen[2].box[1].y1, screen[2].box[1].x2, screen[2].box[1].y2, screen[2].box[1].insText, screen[2].box[1].insTextLg, widthch[4], xBoxOffset, yBoxOffset, 1);
    if(enter)
    {
        return 0;
    }
    InputBox(screen[2].box[2].x1, screen[2].box[2].y1, screen[2].box[2].x2, screen[2].box[2].y2, screen[2].box[2].insText, screen[2].box[2].insTextLg, widthch[4], xBoxOffset, yBoxOffset, 0);
    if(enter)
    {
        return 0;
    }
    InputBox(screen[2].box[3].x1, screen[2].box[3].y1, screen[2].box[3].x2, screen[2].box[3].y2, screen[2].box[3].insText, screen[2].box[3].insTextLg, widthch[4], xBoxOffset, yBoxOffset, 0);
    if(enter)
    {
        return 0;
    }
    InputBox(screen[2].box[4].x1, screen[2].box[4].y1, screen[2].box[4].x2, screen[2].box[4].y2, screen[2].box[4].insText, screen[2].box[4].insTextLg, widthch[4], xBoxOffset, yBoxOffset, 0);
    if(enter)
    {
        return 0;
    }
    HoverClick(screen[2].box[5].x1, screen[2].box[5].y1, screen[2].box[5].x2, screen[2].box[5].y2, screen[2].box[5].x1+(screen[2].box[5].x2-screen[2].box[5].x1)/2-(widthch[4]*screen[2].box[5].textLg)/2, screen[2].box[5].y1+(screen[2].box[5].y2-screen[2].box[5].y1)/2-heightch[4]/2,
               screen[2].box[5].text, screen[2].box[5].textLg, widthch[4], heightch[4], 2, 1, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        scroll=0;
        cleardevice();
        settextstyle(8, 0, 4);
        screen[2].box[1].insText[0]=screen[2].box[2].insText[0]=screen[2].box[3].insText[0]=screen[2].box[4].insText[0]='\0';
        screen[2].box[1].insTextLg=screen[2].box[2].insTextLg=screen[2].box[3].insTextLg=screen[2].box[4].insTextLg=0;
        return 0;
    }
    HoverClick(screen[2].box[6].x1, screen[2].box[6].y1, screen[2].box[6].x2, screen[2].box[6].y2, screen[2].box[6].x1+(screen[2].box[6].x2-screen[2].box[6].x1)/2-(widthch[4]*screen[2].box[7].insTextLg)/2, screen[2].box[5].y1+(screen[2].box[5].y2-screen[2].box[5].y1)/2-heightch[4]/2,
               screen[2].box[7].insText, screen[2].box[7].insTextLg, widthch[4], heightch[4], 2, 3, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        if(wrongf==0 && wrongi==0 && cnt!=0)
        {
            cnt=0;
            scroll=0;
            setbkcolor(BLACK);
            cleardevice();
            SolveForEqualAxis();
            settextstyle(8, 0, 2);
        }
        else
        {
            wrongf=0;
            wrongi=0;
        }
        //screen[2].box[1].insText[0]=screen[2].box[2].insText[0]=screen[2].box[3].insText[0]=screen[2].box[4].insText[0]='\0';
        //screen[2].box[1].insTextLg=screen[2].box[2].insTextLg=screen[2].box[3].insTextLg=screen[2].box[4].insTextLg=0;
        return 0;
    }
    ///FUNCTIE GRESITA
    if(wrongf)
    {
        bar(screen[2].box[1].x1, window[1].height-100, screen[2].box[1].x1+30*widthch[4], window[1].height-100+heightch[4]);
        outtextxy(screen[2].box[1].x1, window[1].height-100, "Functie gresita!");
    }
    ///INTERVAL GRESIT
    else if(wrongi)
    {
        bar(screen[2].box[1].x1, window[1].height-100, screen[2].box[1].x1+16*widthch[4], window[1].height-100+heightch[4]);
        outtextxy(screen[2].box[1].x1, window[1].height-100, "Interval sau distanta gresita!");
    }

    //setactivepage(page);

    page=1-page;
    enter=0;
}

bool Screen3() ///GRAFIC
{
    int widthOffset=30, heightOffset=30, xBoxOffset, yBoxOffset;

    /*strcpy(screen[3].box[0].insText, "Inapoi");
    screen[3].box[0].insTextLg=strlen(screen[3].box[0].insText);*/

    if(langCnt==0)
    {
        strcpy(screen[3].box[1].insText, "Coordonate");
        screen[3].box[1].insTextLg=strlen(screen[3].box[1].insText);
    }
    else if(langCnt==1)
    {
        strcpy(screen[3].box[1].insText, "Coordinates");
        screen[3].box[1].insTextLg=strlen(screen[3].box[1].insText);
    }
    else if(langCnt==2)
    {
        strcpy(screen[3].box[1].insText, "Coordonnées");
        screen[3].box[1].insTextLg=strlen(screen[3].box[1].insText);
    }
    else if(langCnt==3)
    {
        strcpy(screen[3].box[1].insText, "Koordinaten");
        screen[3].box[1].insTextLg=strlen(screen[3].box[1].insText);
    }
    else if(langCnt==4)
    {
        strcpy(screen[3].box[1].insText, "Coordenadas");
        screen[3].box[1].insTextLg=strlen(screen[3].box[1].insText);
    }

    xBoxOffset=widthch[2]/2;
    yBoxOffset=widthch[2]/4;

    screen[3].box[1].x1=widthOffset, screen[3].box[1].y1=window[1].height-heightOffset-heightch[2]-2*yBoxOffset,
    screen[3].box[1].x2=widthOffset+screen[3].box[1].insTextLg*widthch[2]+2*xBoxOffset, screen[3].box[1].y2=window[1].height-heightOffset;

    setcolor(WHITE);

    rectangle(screen[3].box[1].x1, screen[3].box[1].y1, screen[3].box[1].x2, screen[3].box[1].y2);
    outtextxy(screen[3].box[1].x1+xBoxOffset, screen[3].box[1].y1+yBoxOffset, screen[3].box[1].insText);

    HoverClick(screen[3].box[1].x1, screen[3].box[1].y1, screen[3].box[1].x2, screen[3].box[1].y2, screen[3].box[1].x1+xBoxOffset, screen[3].box[1].y1+yBoxOffset,
               screen[3].box[1].insText, screen[3].box[1].insTextLg, widthch[2], heightch[2], 3, 3, 0, DARKGRAY, BLACK);
    if(exitf)
    {
        showCoords=1;
    }

    Zoom();
    int prevMouseX=-1,prevMouseY=-1,i;
    double scale=scale_zoom,x_poz=x_poz_zoom,y_poz=y_poz_zoom;
    while(showCoords)
    {
        //settextstyle(8, 0, 1);
        int mouseX=mousex();
        int mouseY=mousey();
   //     cout<<mouseX<<'\n';
        if (mouseX!=prevMouseX || mouseY!=prevMouseY)
        {
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            bar(0, 0, 200, 30); // Clear the text area
            setcolor(WHITE);
            setbkcolor(BLACK);
            double x=mouseX,y=mouseY;
            double xValue=(x-offsetW-x_poz)*scale+A, yValue=max_y-(y-offsetH-y_poz)*scale;
            //cout<<xValue<<' '<<yValue<<'\n';
            //cout<<x_poz<<' '<<y_poz<<' '<<scale<<'\n';
            int xPixel,yPixel;
            yPixel=(height-1)-1.0*(yValue-min_y)/scale+offsetH-y_poz;
            char c[50];
            int lg=NumberToText(xValue,c,0);
            c[lg++]=',';
            NumberToText(yValue,c,lg);
            outtextxy(10, 10, c);
           // outtextxy(screen[3].box[1].x, screen[3].box[1].y1, c);
            prevMouseX=mouseX;
            prevMouseY=mouseY;
        }
        Zoom();
        int i,xBoxOffset,yBoxOffset,widthOffset=30,heightOffset=30;
        xBoxOffset=widthch[2]/2;
        yBoxOffset=widthch[2]/4;
        screen[3].box[1].x1=widthOffset, screen[3].box[1].y1=window[1].height-heightOffset-heightch[2]-2*yBoxOffset,
        screen[3].box[1].x2=widthOffset+screen[3].box[1].insTextLg*widthch[2]+2*xBoxOffset, screen[3].box[1].y2=window[1].height-heightOffset;

        setcolor(WHITE);

        rectangle(screen[3].box[1].x1, screen[3].box[1].y1, screen[3].box[1].x2, screen[3].box[1].y2);
        outtextxy(screen[3].box[1].x1+xBoxOffset, screen[3].box[1].y1+yBoxOffset, screen[3].box[1].insText);

        HoverClick(screen[3].box[1].x1, screen[3].box[1].y1, screen[3].box[1].x2, screen[3].box[1].y2, screen[3].box[1].x1+xBoxOffset, screen[3].box[1].y1+yBoxOffset,
                   screen[3].box[1].insText, screen[3].box[1].insTextLg, widthch[2], heightch[2], 3, 3, 0, DARKGRAY, BLACK);
        if(exitf)
        {
            showCoords=0;
        }
        //cout<<"DA";
    }


    Coords(x_poz_zoom, y_poz_zoom, scale_zoom);
    //cout<<1;
    if(exitf) return 0;

}

bool Screen4() ///ISTORIC
{
    int titlelg, s1lg, s2lg, s3lg, s4lg, titleHeightOffset=50, widthOffset=50, heightOffset=150, heightDiff, pageOffset=30,
        xBoxOffset, yBoxOffset, i, j, maxLg=-10, iMaxLg;

    if(langCnt==0)
    {
        strcpy(screen[4].title, "Istoric");
        screen[4].titleLg=strlen(screen[4].title);

        strcpy(screen[4].box[1].insText, "Grafic");
        screen[4].box[1].insTextLg=strlen(screen[4].box[1].insText);

        strcpy(screen[4].box[2].insText, "Modifica");
        screen[4].box[2].insTextLg=strlen(screen[4].box[2].insText);
    }
    else if(langCnt==1)
    {
        strcpy(screen[4].title, "History");
        screen[4].titleLg=strlen(screen[4].title);

        strcpy(screen[4].box[1].insText, "Graph");
        screen[4].box[1].insTextLg=strlen(screen[4].box[1].insText);

        strcpy(screen[4].box[2].insText, "Modify");
        screen[4].box[2].insTextLg=strlen(screen[4].box[2].insText);
    }
    else if(langCnt==2)
    {
        strcpy(screen[4].title, "Historique");
        screen[4].titleLg=strlen(screen[4].title);

        strcpy(screen[4].box[1].insText, "Graphique");
        screen[4].box[1].insTextLg=strlen(screen[4].box[1].insText);

        strcpy(screen[4].box[2].insText, "Modifier");
        screen[4].box[2].insTextLg=strlen(screen[4].box[2].insText);
    }
    else if(langCnt==3)
    {
        strcpy(screen[4].title, "Historisch");
        screen[4].titleLg=strlen(screen[4].title);

        strcpy(screen[4].box[1].insText, "Graph");
        screen[4].box[1].insTextLg=strlen(screen[4].box[1].insText);

        strcpy(screen[4].box[2].insText, "Ändern");
        screen[4].box[2].insTextLg=strlen(screen[4].box[2].insText);
    }
    else if(langCnt==4)
    {
        strcpy(screen[4].title, "Histórico");
        screen[4].titleLg=strlen(screen[4].title);

        strcpy(screen[4].box[1].insText, "Gráfico");
        screen[4].box[1].insTextLg=strlen(screen[4].box[1].insText);

        strcpy(screen[4].box[2].insText, "Enmendar");
        screen[4].box[2].insTextLg=strlen(screen[4].box[2].insText);
    }

    if(pageNr==1)
    {
        if(langCnt==0)
        {
            strcpy(screen[4].box[0].text, "Inapoi");
            screen[4].box[0].textLg=strlen(screen[4].box[0].text);
        }
        else if(langCnt==1)
        {
            strcpy(screen[4].box[0].text, "Back");
            screen[4].box[0].textLg=strlen(screen[4].box[0].text);
        }
        else if(langCnt==2)
        {
            strcpy(screen[4].box[0].text, "Dos");
            screen[4].box[0].textLg=strlen(screen[4].box[0].text);
        }
        else if(langCnt==3)
        {
            strcpy(screen[4].box[0].text, "Zurück");
            screen[4].box[0].textLg=strlen(screen[4].box[0].text);
        }
        else if(langCnt==4)
        {
            strcpy(screen[4].box[0].text, "Atrás");
            screen[4].box[0].textLg=strlen(screen[4].box[0].text);
        }
    }
    else
    {
        strcpy(screen[4].box[0].text, "<-----");
        screen[4].box[0].textLg=strlen(screen[4].box[0].text);
    }
    if(howManyPages>1)
    {
        strcpy(screen[4].box[49].text, "----->");
        screen[4].box[49].textLg=strlen(screen[4].box[0].text);
    }

    setcolor(WHITE);

    xBoxOffset=widthch[2]/2;
    yBoxOffset=widthch[2]/4;

    heightDiff=(window[1].height-2*titleHeightOffset-8*(heightch[2]+2*yBoxOffset))/8;

    ///BUTOANE: "GRAFIC" + "MODIFICA"

    screen[4].box[0].x1=pageOffset, screen[4].box[0].y1=window[1].height-pageOffset-heightch[2]-2*yBoxOffset,
                     screen[4].box[0].x2=pageOffset+(widthch[2]*screen[4].box[0].textLg)*(screen[4].box[0].textLg>=6)+(widthch[2]*6)*(screen[4].box[0].textLg<6)+2*xBoxOffset, screen[4].box[0].y2=window[1].height-pageOffset;

    screen[4].box[49].x1=window[1].width-pageOffset-(widthch[2]*screen[4].box[49].textLg)*(screen[4].box[49].textLg>=6)-(widthch[2]*6)*(screen[4].box[49].textLg<6)-2*xBoxOffset, screen[4].box[49].y1=window[1].height-pageOffset-heightch[2]-2*yBoxOffset,
                      screen[4].box[49].x2=window[1].width-pageOffset, screen[4].box[49].y2=window[1].height-pageOffset;

    for(i=1; i<functionNr; i++)
    {
        screen[4].box[i].textx=2.5*widthOffset;
        screen[4].box[i].texty=heightOffset+(i%8!=0)*(i%8)*heightDiff+(i%8==0)*8*heightDiff;
        if(screen[4].box[i].textLg>maxLg)
        {
            maxLg=screen[4].box[i].textLg;
            iMaxLg=i;
        }
    }

    for(i=1; i<functionNr; i++)
    {
        screen[4].box[i].x1=screen[4].box[i].textx+screen[4].box[iMaxLg].textLg*widthch[2]+widthOffset;
        screen[4].box[i].y1=screen[4].box[i].texty-heightch[2]/4;
        screen[4].box[i].x2=screen[4].box[i].x1+screen[4].box[1].insTextLg*widthch[2]+2*xBoxOffset;
        screen[4].box[i].y2=screen[4].box[i].y1+heightch[2]+2*yBoxOffset;

        screen[4].box[functionNr+i-1].x1=screen[4].box[i].x2+widthOffset;
        screen[4].box[functionNr+i-1].y1=screen[4].box[i].y1;
        screen[4].box[functionNr+i-1].x2=screen[4].box[functionNr+i-1].x1+screen[4].box[2].insTextLg*widthch[2]+2*xBoxOffset;
        screen[4].box[functionNr+i-1].y2=screen[4].box[i].y2;
    }

    //setactivepage(page);
    //setvisualpage(1-page);

    //cleardevice();
    if(refresh)
    {
        cleardevice();
        refresh=0;
    }

    outtextxy(window[1].width/2-screen[4].titleLg/2*widthch[2], titleHeightOffset, screen[4].title);

    for(i=1+(pageNr-1)*8; i<=pageNr*8 && i<functionNr; i++)
    {
        if(screen[4].box[i].ind[0]!='\0')
        {
            outtextxy(widthOffset, screen[4].box[i].texty, screen[4].box[i].ind);
        }
        outtextxy(screen[4].box[i].textx, screen[4].box[i].texty, screen[4].box[i].text);

        if(screen[4].box[i].ind[0]!='\0')
        {
            rectangle(screen[4].box[i].x1, screen[4].box[i].y1, screen[4].box[i].x2, screen[4].box[i].y2);
            outtextxy(screen[4].box[i].x1+xBoxOffset, screen[4].box[i].y1+yBoxOffset, screen[4].box[1].insText);
        }

        if(screen[4].box[i].ind[0]!='\0')
        {
            int x=atoi(screen[4].box[i].ind);
            if(screen[4].box[x].fNr==1)
            {
                rectangle(screen[4].box[functionNr+i-1].x1, screen[4].box[functionNr+i-1].y1, screen[4].box[functionNr+i-1].x2, screen[4].box[functionNr+i-1].y2);
                outtextxy(screen[4].box[functionNr+i-1].x1+xBoxOffset, screen[4].box[functionNr+i-1].y1+yBoxOffset, screen[4].box[2].insText);
            }
        }
    }

    rectangle(screen[4].box[0].x1, screen[4].box[0].y1, screen[4].box[0].x2, screen[4].box[0].y2);
    outtextxy(screen[4].box[0].x1+(screen[4].box[0].x2-screen[4].box[0].x1)/2-(widthch[2]*screen[4].box[0].textLg)/2, screen[4].box[0].y1+(screen[4].box[0].y2-screen[4].box[0].y1)/2-heightch[2]/2, screen[4].box[0].text);

    if(howManyPages>1 && pageNr!=howManyPages)
    {
        rectangle(screen[4].box[49].x1, screen[4].box[49].y1, screen[4].box[49].x2, screen[4].box[49].y2);
        outtextxy(screen[4].box[49].x1+xBoxOffset, screen[4].box[49].y1+yBoxOffset, screen[4].box[49].text);
    }

    //setactivepage(1-page);

    for(i=1+(pageNr-1)*8; i<=pageNr*8 && i<functionNr; i++)
    {
        if(screen[4].box[i].ind[0]!='\0')
        {
            cnt1=atoi(screen[4].box[i].ind);
            HoverClick(screen[4].box[i].x1, screen[4].box[i].y1, screen[4].box[i].x2, screen[4].box[i].y2, screen[4].box[i].x1+xBoxOffset, screen[4].box[i].y1+yBoxOffset,
                       screen[4].box[1].insText, screen[4].box[1].insTextLg, widthch[2], heightch[2], 4, 3, i, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
            if(exitf)
            {
                setbkcolor(BLACK);
                cleardevice();
                for(j=1; j<=fIndex[currWindow]; j++)
                {
                    getSet();
                }
                SolveForEqualAxis();
                settextstyle(8, 0, 2);
                return 0;
            }
        }

        if(screen[4].box[i].ind[0]!='\0')
        {
            int x=atoi(screen[4].box[i].ind);
            if(screen[4].box[x].fNr==1)
            {
                HoverClick(screen[4].box[functionNr+i-1].x1, screen[4].box[functionNr+i-1].y1, screen[4].box[functionNr+i-1].x2, screen[4].box[functionNr+i-1].y2, screen[4].box[functionNr+i-1].x1+xBoxOffset, screen[4].box[functionNr+i-1].y1+yBoxOffset,
                       screen[4].box[2].insText, screen[4].box[2].insTextLg, widthch[2], heightch[2], 4, 2, i, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
                if(exitf)
                {
                    cleardevice();
                    settextstyle(8, 0, 4);
                    return 0;
                }
            }
        }
    }
    if(pageNr==1 /*&& !refresh*/)
    {
        HoverClick(screen[4].box[0].x1, screen[4].box[0].y1, screen[4].box[0].x2, screen[4].box[0].y2, screen[4].box[0].x1+(screen[4].box[0].x2-screen[4].box[0].x1)/2-(widthch[2]*screen[4].box[0].textLg)/2, screen[4].box[0].y1+(screen[4].box[0].y2-screen[4].box[0].y1)/2-heightch[2]/2,
            screen[4].box[0].text, screen[4].box[0].textLg, widthch[2], heightch[2], 4, 1, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
        if(exitf)
        {
            cleardevice();
            settextstyle(8, 0, 4);
            refresh=1;
            return 0;
        }
    }
    else if(pageNr>1 /*&& !refresh*/)
    {
        HoverClick(screen[4].box[0].x1, screen[4].box[0].y1, screen[4].box[0].x2, screen[4].box[0].y2, screen[4].box[0].x1+(screen[4].box[0].x2-screen[4].box[0].x1)/2-(widthch[2]*screen[4].box[0].textLg)/2, screen[4].box[0].y1+(screen[4].box[0].y2-screen[4].box[0].y1)/2-heightch[2]/2,
                   screen[4].box[0].text, screen[4].box[0].textLg, widthch[2], heightch[2], 4, 4, -1, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
        if(exitf)
        {
            cleardevice();
            refresh=1;
            return 0;
        }
    }
    if(pageNr<howManyPages /*&& !refresh*/)
    {
        HoverClick(screen[4].box[49].x1, screen[4].box[49].y1, screen[4].box[49].x2, screen[4].box[49].y2, screen[4].box[49].x1+xBoxOffset, screen[4].box[49].y1+yBoxOffset,
                   screen[4].box[49].text, screen[4].box[49].textLg, widthch[2], heightch[2], 4, 4, 1, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
        if(exitf)
        {
            cleardevice();
            refresh=1;
            return 0;
        }
    }

    //setactivepage(page);

    page=1-page;

    refresh=0;
}

void FillTheme(int colorWidth, int colorHeight, int nr, int i)
{
    setfillstyle(1, theme[nr].bkcolor);
    bar(screen[5].box[i].x1+1, screen[5].box[i].y1+1, screen[5].box[i].x1+colorWidth+1, screen[5].box[i].y1+colorHeight);
    setfillstyle(1, theme[nr].hovercolor);
    bar(screen[5].box[i].x1+colorWidth+1, screen[5].box[i].y1+1, screen[5].box[i].x1+2*colorWidth+1, screen[5].box[i].y1+colorHeight);
    setfillstyle(1, theme[nr].bordercolor);
    bar(screen[5].box[i].x1+2*colorWidth+1, screen[5].box[i].y1+1, screen[5].box[i].x1+3*colorWidth, screen[5].box[i].y1+colorHeight);
}

bool Screen5() ///SETARI
{
    int widthOffset=100, heightOffset=150, titleHeightOffset=50, titleWidthCh, titleHeightCh, backOffset=30,
        xBoxOffset, yBoxOffset, xOffset=30, themeBoxDiff, langBoxDiff, colorWidth=50, colorHeight=70, colorNr=3, i;

    if(langCnt==0)
    {
        strcpy(screen[5].title, "Setari");
        screen[5].titleLg=strlen(screen[5].title);
        strcpy(screen[5].box[0].text, "Inapoi");
        screen[5].box[0].textLg=strlen(screen[5].box[0].text);
        strcpy(screen[5].box[1].text, "Culoare: ");
        screen[5].box[1].textLg=strlen(screen[5].box[1].text);
        strcpy(screen[5].box[themeNr+2].text, "Limba: ");
        screen[5].box[themeNr+2].textLg=strlen(screen[5].box[themeNr+2].text);
        strcpy(screen[5].box[49].text, "Sunet: ");
        screen[5].box[49].textLg=strlen(screen[5].box[49].text);
        strcpy(screen[5].box[48].insText, "Activ");
        screen[5].box[48].insTextLg=strlen(screen[5].box[48].insText);
        strcpy(screen[5].box[49].insText, "Inactiv");
        screen[5].box[49].insTextLg=strlen(screen[5].box[49].insText);
    }
    else if(langCnt==1)
    {
        strcpy(screen[5].title, "Settings");
        screen[5].titleLg=strlen(screen[5].title);
        strcpy(screen[5].box[0].text, "Back");
        screen[5].box[0].textLg=strlen(screen[5].box[0].text);
        strcpy(screen[5].box[1].text, "Colour: ");
        screen[5].box[1].textLg=strlen(screen[5].box[1].text);
        strcpy(screen[5].box[themeNr+2].text, "Language: ");
        screen[5].box[themeNr+2].textLg=strlen(screen[5].box[themeNr+2].text);
        strcpy(screen[5].box[48].insText, "Active");
        screen[5].box[48].insTextLg=strlen(screen[5].box[48].insText);
        strcpy(screen[5].box[49].insText, "Inactive");
        screen[5].box[49].insTextLg=strlen(screen[5].box[49].insText);
    }
    else if(langCnt==2)
    {
        strcpy(screen[5].title, "Paramètres");
        screen[5].titleLg=strlen(screen[5].title);
        strcpy(screen[5].box[0].text, "Dos");
        screen[5].box[0].textLg=strlen(screen[5].box[0].text);
        strcpy(screen[5].box[1].text, "Couleur: ");
        screen[5].box[1].textLg=strlen(screen[5].box[1].text);
        strcpy(screen[5].box[themeNr+2].text, "Langue: ");
        screen[5].box[themeNr+2].textLg=strlen(screen[5].box[themeNr+2].text);
        strcpy(screen[5].box[48].insText, "Actif");
        screen[5].box[48].insTextLg=strlen(screen[5].box[48].insText);
        strcpy(screen[5].box[49].insText, "Inactif");
        screen[5].box[49].insTextLg=strlen(screen[5].box[49].insText);
    }
    else if(langCnt==3)
    {
        strcpy(screen[5].title, "Einstellungen");
        screen[5].titleLg=strlen(screen[5].title);
        strcpy(screen[5].box[0].text, "Zurück");
        screen[5].box[0].textLg=strlen(screen[5].box[0].text);
        strcpy(screen[5].box[1].text, "Farbe: ");
        screen[5].box[1].textLg=strlen(screen[5].box[1].text);
        strcpy(screen[5].box[themeNr+2].text, "Sprache: ");
        screen[5].box[themeNr+2].textLg=strlen(screen[5].box[themeNr+2].text);
        strcpy(screen[5].box[48].insText, "Aktiv");
        screen[5].box[48].insTextLg=strlen(screen[5].box[48].insText);
        strcpy(screen[5].box[49].insText, "Inaktiv");
        screen[5].box[49].insTextLg=strlen(screen[5].box[49].insText);
    }
    else if(langCnt==4)
    {
        strcpy(screen[5].title, "Ajustes");
        screen[5].titleLg=strlen(screen[5].title);
        strcpy(screen[5].box[0].text, "Atrás");
        screen[5].box[0].textLg=strlen(screen[5].box[0].text);
        strcpy(screen[5].box[1].text, "Color: ");
        screen[5].box[1].textLg=strlen(screen[5].box[1].text);
        strcpy(screen[5].box[themeNr+2].text, "Idioma: ");
        screen[5].box[themeNr+2].textLg=strlen(screen[5].box[themeNr+2].text);
        strcpy(screen[5].box[48].insText, "Activo");
        screen[5].box[48].insTextLg=strlen(screen[5].box[48].insText);
        strcpy(screen[5].box[49].insText, "Inactivo");
        screen[5].box[49].insTextLg=strlen(screen[5].box[49].insText);
    }

    strcpy(screen[5].box[themeNr+2].insText, "RO");
    screen[5].box[themeNr+2].insTextLg=strlen(screen[5].box[themeNr+2].insText);
    strcpy(screen[5].box[themeNr+3].insText, "ENG");
    screen[5].box[themeNr+3].insTextLg=strlen(screen[5].box[themeNr+3].insText);
    strcpy(screen[5].box[themeNr+4].insText, "FR");
    screen[5].box[themeNr+4].insTextLg=strlen(screen[5].box[themeNr+4].insText);
    strcpy(screen[5].box[themeNr+5].insText, "DE");
    screen[5].box[themeNr+5].insTextLg=strlen(screen[5].box[themeNr+5].insText);
    strcpy(screen[5].box[themeNr+6].insText, "ESP");
    screen[5].box[themeNr+6].insTextLg=strlen(screen[5].box[themeNr+6].insText);

    titleWidthCh=widthch[4];
    titleHeightCh=heightch[4];

    xBoxOffset=widthch[4]/2;
    yBoxOffset=widthch[4]/4;

    screen[5].box[0].x1=backOffset, screen[5].box[0].y1=window[1].height-backOffset-heightch[4]-2*yBoxOffset,
    screen[5].box[0].x2=backOffset+(screen[5].box[0].textLg*widthch[4])*(screen[5].box[0].textLg>=6)+(6*widthch[4])*(screen[5].box[0].textLg<6)+2*xBoxOffset, screen[5].box[0].y2=window[1].height-backOffset;

    screen[5].box[1].x1=widthOffset+widthch[4]*screen[5].box[1].textLg+xOffset, screen[5].box[1].y1=titleHeightOffset+heightOffset-heightch[4]/2,
    screen[5].box[1].x2=screen[5].box[1].x1+colorNr*colorWidth, screen[5].box[1].y2=screen[5].box[1].y1+colorHeight;

    screen[5].box[themeNr+2].x1=widthOffset+widthch[4]*screen[5].box[themeNr+2].textLg+xOffset, screen[5].box[themeNr+2].y1=titleHeightOffset+2*heightOffset-heightch[4]/2,
    screen[5].box[themeNr+2].x2=screen[5].box[themeNr+2].x1+colorNr*colorWidth, screen[5].box[themeNr+2].y2=screen[5].box[themeNr+2].y1+colorHeight;

    screen[5].box[49].x1=widthOffset+widthch[4]*screen[5].box[49].textLg+xOffset, screen[5].box[49].y1=titleHeightOffset+3*heightOffset-heightch[4]/2,
    screen[5].box[49].x2=screen[5].box[49].x1+widthch[4]*screen[5].box[49].insTextLg+colorWidth, screen[5].box[49].y2=screen[5].box[49].y1+colorHeight;

    themeBoxDiff=(window[1].width-themeNr*colorNr*colorWidth-screen[5].box[1].x2)/(themeNr+1);
    langBoxDiff=(window[1].width-langNr*colorNr*colorWidth-widthOffset-widthch[4]*screen[5].box[themeNr+2].textLg)/langNr;

    for(i=1; i<=themeNr; i++)
    {
        screen[5].box[i+1].x1=screen[5].box[i].x2+themeBoxDiff, screen[5].box[i+1].y1=titleHeightOffset+heightOffset-heightch[4]/2,
        screen[5].box[i+1].x2=screen[5].box[i+1].x1+colorNr*colorWidth, screen[5].box[i+1].y2=screen[5].box[i+1].y1+colorHeight;
    }

    for(i=themeNr+2; i<=2*themeNr; i++)
    {
        screen[5].box[i+1].x1=screen[5].box[i].x2+langBoxDiff, screen[5].box[i+1].y1=titleHeightOffset+2*heightOffset-heightch[4]/2,
        screen[5].box[i+1].x2=screen[5].box[i+1].x1+colorNr*colorWidth, screen[5].box[i+1].y2=screen[5].box[i+1].y1+colorHeight;
    }

    //setactivepage(page);
    //setvisualpage(1-page);
    //setbkcolor(theme[themeCnt].bkcolor);
    //cleardevice();

    ///TITLU
    outtextxy(window[1].width/2-screen[5].titleLg/2*titleWidthCh, titleHeightOffset, screen[5].title);

    ///CASETA TEMA ACTUALA
    outtextxy(widthOffset, titleHeightOffset+heightOffset, screen[5].box[1].text);
    setcolor(BLACK);
    rectangle(screen[5].box[1].x1, screen[5].box[1].y1, screen[5].box[1].x2, screen[5].box[1].y2);
    setcolor(WHITE);
    FillTheme(colorWidth, colorHeight, themeCnt, 1);

    ///BUTON INAPOI
    rectangle(screen[5].box[0].x1, screen[5].box[0].y1, screen[5].box[0].x2, screen[5].box[0].y2);
    outtextxy(screen[5].box[0].x1+(screen[5].box[0].x2-screen[5].box[0].x1)/2-widthch[4]*screen[5].box[0].textLg/2, screen[5].box[0].y1+(screen[5].box[0].y2-screen[5].box[0].y1)/2-heightch[4]/2, screen[5].box[0].text);

    if(expand==1)
    {
        for(i=1; i<=themeNr; i++)
        {
            setcolor(BLACK);
            rectangle(screen[5].box[i+1].x1, screen[5].box[i+1].y1, screen[5].box[i+1].x2, screen[5].box[i+1].y2);
            setcolor(WHITE);
            FillTheme(colorWidth, colorHeight, i, i+1);
        }
    }

    outtextxy(widthOffset, titleHeightOffset+2*heightOffset, screen[5].box[themeNr+2].text);

    outtextxy(screen[5].box[themeNr+2].x1+(screen[5].box[themeNr+2].x2-screen[5].box[themeNr+2].x1)/2-widthch[4]*screen[5].box[themeNr+2].insTextLg/2, screen[5].box[themeNr+2].y1+(screen[5].box[themeNr+2].y2-screen[5].box[themeNr+2].y1)/2-heightch[4]/2, screen[5].box[themeNr+2].insText);
    rectangle(screen[5].box[themeNr+2].x1, screen[5].box[themeNr+2].y1, screen[5].box[themeNr+2].x2, screen[5].box[themeNr+2].y2);

    outtextxy(screen[5].box[themeNr+3].x1+(screen[5].box[themeNr+3].x2-screen[5].box[themeNr+3].x1)/2-widthch[4]*screen[5].box[themeNr+3].insTextLg/2, screen[5].box[themeNr+3].y1+(screen[5].box[themeNr+3].y2-screen[5].box[themeNr+3].y1)/2-heightch[4]/2, screen[5].box[themeNr+3].insText);
    outtextxy(screen[5].box[themeNr+4].x1+(screen[5].box[themeNr+4].x2-screen[5].box[themeNr+4].x1)/2-widthch[4]*screen[5].box[themeNr+4].insTextLg/2, screen[5].box[themeNr+4].y1+(screen[5].box[themeNr+4].y2-screen[5].box[themeNr+4].y1)/2-heightch[4]/2, screen[5].box[themeNr+4].insText);
    outtextxy(screen[5].box[themeNr+5].x1+(screen[5].box[themeNr+5].x2-screen[5].box[themeNr+5].x1)/2-widthch[4]*screen[5].box[themeNr+5].insTextLg/2, screen[5].box[themeNr+5].y1+(screen[5].box[themeNr+5].y2-screen[5].box[themeNr+5].y1)/2-heightch[4]/2, screen[5].box[themeNr+5].insText);
    outtextxy(screen[5].box[themeNr+6].x1+(screen[5].box[themeNr+6].x2-screen[5].box[themeNr+6].x1)/2-widthch[4]*screen[5].box[themeNr+6].insTextLg/2, screen[5].box[themeNr+6].y1+(screen[5].box[themeNr+6].y2-screen[5].box[themeNr+6].y1)/2-heightch[4]/2, screen[5].box[themeNr+6].insText);

    for(i=themeNr+2; i<=2*themeNr; i++)
    {
        rectangle(screen[5].box[i+1].x1, screen[5].box[i+1].y1, screen[5].box[i+1].x2, screen[5].box[i+1].y2);
    }

    outtextxy(widthOffset, titleHeightOffset+3*heightOffset, screen[5].box[49].text);
    rectangle(screen[5].box[49].x1, screen[5].box[49].y1, screen[5].box[49].x2, screen[5].box[49].y2);

    if(sound==1)
    {
        outtextxy(screen[5].box[49].x1+(screen[5].box[49].x2-screen[5].box[49].x1)/2-widthch[4]*screen[5].box[48].insTextLg/2, screen[5].box[49].y1+(screen[5].box[49].y2-screen[5].box[49].y1)/2-heightch[4]/2, screen[5].box[48].insText);
    }
    else
    {
        outtextxy(screen[5].box[49].x1+(screen[5].box[49].x2-screen[5].box[49].x1)/2-widthch[4]*screen[5].box[49].insTextLg/2, screen[5].box[49].y1+(screen[5].box[49].y2-screen[5].box[49].y1)/2-heightch[4]/2, screen[5].box[49].insText);
    }

    for(i=1; i<=themeNr; i++)
    {
        //if(!refresh)
        //{
        HoverClick(screen[5].box[i+1].x1, screen[5].box[i+1].y1, screen[5].box[i+1].x2, screen[5].box[i+1].y2, screen[5].box[i+1].x1+xBoxOffset, screen[5].box[i+1].y1+yBoxOffset,
                   screen[5].box[i+1].insText, screen[5].box[i+1].insTextLg, widthch[4], heightch[4], 5, 5, i+1, theme[themeCnt].hovercolor, theme[themeCnt].bordercolor);
        if(exitf)
        {
            setbkcolor(theme[themeCnt].bkcolor);
            cleardevice();
            //refresh=1;
            return 0;
        }
        //}
    }
    //if(!refresh)
    //{
    HoverClick(screen[5].box[1].x1, screen[5].box[1].y1, screen[5].box[1].x2, screen[5].box[1].y2, screen[5].box[1].x1+xBoxOffset, screen[5].box[1].y1+yBoxOffset,
               screen[5].box[1].insText, screen[5].box[1].insTextLg, widthch[4], heightch[4], 5, 5, 0, theme[themeCnt].hovercolor, theme[themeCnt].bordercolor);
    if(exitf)
    {
        cleardevice();
        //refresh=1;
        return 0;
    }
    //}

    HoverClick(screen[5].box[0].x1, screen[5].box[0].y1, screen[5].box[0].x2, screen[5].box[0].y2, screen[5].box[0].x1+(screen[5].box[0].x2-screen[5].box[0].x1)/2-widthch[4]*screen[5].box[0].textLg/2, screen[5].box[0].y1+(screen[5].box[0].y2-screen[5].box[0].y1)/2-heightch[4]/2,
               screen[5].box[0].text, screen[5].box[0].textLg, widthch[4], heightch[4], 5, 1, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        settextstyle(8, 0, 4);
        return 0;
    }
    //setactivepage(page);
    if(sound==1)
    {
        HoverClick(screen[5].box[49].x1, screen[5].box[49].y1, screen[5].box[49].x2, screen[5].box[49].y2, screen[5].box[49].x1+(screen[5].box[49].x2-screen[5].box[49].x1)/2-widthch[4]*screen[5].box[48].insTextLg/2, screen[5].box[49].y1+(screen[5].box[49].y2-screen[5].box[49].y1)/2-heightch[4]/2,
                   screen[5].box[48].insText, screen[5].box[48].insTextLg, widthch[4], heightch[4], 5, 5, -1, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    }
    else
    {
        HoverClick(screen[5].box[49].x1, screen[5].box[49].y1, screen[5].box[49].x2, screen[5].box[49].y2, screen[5].box[49].x1+(screen[5].box[49].x2-screen[5].box[49].x1)/2-widthch[4]*screen[5].box[49].insTextLg/2, screen[5].box[49].y1+(screen[5].box[49].y2-screen[5].box[49].y1)/2-heightch[4]/2,
                   screen[5].box[49].insText, screen[5].box[49].insTextLg, widthch[4], heightch[4], 5, 5, -1, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    }

    HoverClick(screen[5].box[themeNr+2].x1, screen[5].box[themeNr+2].y1, screen[5].box[themeNr+2].x2, screen[5].box[themeNr+2].y2, screen[5].box[themeNr+2].x1+(screen[5].box[themeNr+2].x2-screen[5].box[themeNr+2].x1)/2-widthch[4]*screen[5].box[themeNr+2].insTextLg/2, screen[5].box[themeNr+2].y1+(screen[5].box[themeNr+2].y2-screen[5].box[themeNr+2].y1)/2-heightch[4]/2,
               screen[5].box[themeNr+2].insText, screen[5].box[themeNr+2].insTextLg, widthch[4], heightch[4], 5, 5, themeNr+2, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        functionCnt=1;
        for(i=1; i<=functionNr; i++)
        {
            functionTextBuild();
            functionCnt++;
        }
        cleardevice();
        //refresh=1;
        return 0;
    }
    for(i=themeNr+2; i<=2*themeNr; i++)
    {
        HoverClick(screen[5].box[i+1].x1, screen[5].box[i+1].y1, screen[5].box[i+1].x2, screen[5].box[i+1].y2, screen[5].box[i+1].x1+(screen[5].box[i+1].x2-screen[5].box[i+1].x1)/2-widthch[4]*screen[5].box[i+1].insTextLg/2, screen[5].box[i+1].y1+(screen[5].box[i+1].y2-screen[5].box[i+1].y1)/2-heightch[4]/2,
                   screen[5].box[i+1].insText, screen[5].box[i+1].insTextLg, widthch[4], heightch[4], 5, 5, i+1, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
        if(exitf)
        {
            functionCnt=1;
            for(i=1; i<=functionNr; i++)
            {
                functionTextBuild();
                functionCnt++;
            }
            cleardevice();
            //refresh=1;
            return 0;
        }
    }

    page=1-page;

    refresh=0;
}

bool Screen6() ///INSTRUCTIUNI
{
    int widthOffset=100, heightOffset=150, titleHeightOffset=50, titleWidthCh, titleHeightCh, backOffset=30,
        xBoxOffset, yBoxOffset, xOffset=30, themeBoxDiff, langBoxDiff, colorWidth=50, colorHeight=70, colorNr=3, i;

    if(langCnt==0)
    {
        strcpy(screen[6].title, "Instructiuni");
        screen[6].titleLg=strlen(screen[6].title);
        strcpy(screen[6].box[0].text, "Inapoi");
        screen[6].box[0].textLg=strlen(screen[6].box[0].text);
        strcpy(screen[6].box[1].text, "Functii:");
        screen[6].box[1].textLg=strlen(screen[6].box[1].text);
        strcpy(screen[6].box[2].text, "sinus -> sin | cosinus -> cos | tangenta -> tg | cotangenta -> ctg");
        screen[6].box[2].textLg=strlen(screen[6].box[2].text);
        strcpy(screen[6].box[3].text, "arcsinus -> arcsin | arccosinus -> arccos");
        screen[6].box[3].textLg=strlen(screen[6].box[3].text);
        strcpy(screen[6].box[4].text, "arctangenta -> arctg | arccotangenta -> arcctg");
        screen[6].box[4].textLg=strlen(screen[6].box[4].text);
        strcpy(screen[6].box[5].text, "radical -> rad | modul -> abs | exponential -> e | logaritm -> ln");
        screen[6].box[5].textLg=strlen(screen[6].box[5].text);
        strcpy(screen[6].box[6].text, "Functia introdusa trebuie sa fie intre paranteze rotunde.");
        screen[6].box[6].textLg=strlen(screen[6].box[6].text);
        strcpy(screen[6].box[7].text, "Numerele negative trebuie sa fie intre paranteze rotunde.");
        screen[6].box[7].textLg=strlen(screen[6].box[7].text);
        strcpy(screen[6].box[8].text, "Pentru introducerea unei functii in fereastra curenta se va apasa ENTER.");
        screen[6].box[8].textLg=strlen(screen[6].box[8].text);
        strcpy(screen[6].box[9].text, "Variabila X trebuie scrisa cu majuscula!");
        screen[6].box[9].textLg=strlen(screen[6].box[9].text);
        strcpy(screen[6].box[10].text, "Inchiderea unei ferestre va duce la oprirea programului!");
        screen[6].box[10].textLg=strlen(screen[6].box[10].text);
    }
    else if(langCnt==1)
    {
        strcpy(screen[6].title, "Instructions");
        screen[6].titleLg=strlen(screen[6].title);
        strcpy(screen[6].box[0].text, "Back");
        screen[6].box[0].textLg=strlen(screen[6].box[0].text);
        strcpy(screen[6].box[1].text, "Functions:");
        screen[6].box[1].textLg=strlen(screen[6].box[1].text);
        strcpy(screen[6].box[2].text, "sinus -> sin | cosinus -> cos | tangenta -> tg | cotangenta -> ctg");
        screen[6].box[2].textLg=strlen(screen[6].box[2].text);
        strcpy(screen[6].box[3].text, "arcsinus -> arcsin | arccosinus -> arccos");
        screen[6].box[3].textLg=strlen(screen[6].box[3].text);
        strcpy(screen[6].box[4].text, "arctangenta -> arctg | arccotangenta -> arcctg");
        screen[6].box[4].textLg=strlen(screen[6].box[4].text);
        strcpy(screen[6].box[5].text, "radical -> rad | modul -> abs | exponential -> e | logaritm -> ln");
        screen[6].box[5].textLg=strlen(screen[6].box[5].text);
        strcpy(screen[6].box[6].text, "The entered function must be in round parentheses.");
        screen[6].box[6].textLg=strlen(screen[6].box[6].text);
        strcpy(screen[6].box[7].text, "Negative numbers must be in round parentheses.");
        screen[6].box[7].textLg=strlen(screen[6].box[7].text);
        strcpy(screen[6].box[8].text, "For entering a function in the current window ENTER must be pressed.");
        screen[6].box[8].textLg=strlen(screen[6].box[8].text);
        strcpy(screen[6].box[9].text, "The variable X must be written in uppercase!");
        screen[6].box[9].textLg=strlen(screen[6].box[9].text);
        strcpy(screen[6].box[10].text, "Closing a window will end the program!");
        screen[6].box[10].textLg=strlen(screen[6].box[10].text);
    }
    else if(langCnt==2)
    {
        strcpy(screen[6].title, "Instructions");
        screen[6].titleLg=strlen(screen[6].title);
        strcpy(screen[6].box[0].text, "Dos");
        screen[6].box[0].textLg=strlen(screen[6].box[0].text);
        strcpy(screen[6].box[1].text, "Fonctions:");
        screen[6].box[1].textLg=strlen(screen[6].box[1].text);
        strcpy(screen[6].box[2].text, "sinus -> sin | cosinus -> cos | tangenta -> tg | cotangenta -> ctg");
        screen[6].box[2].textLg=strlen(screen[6].box[2].text);
        strcpy(screen[6].box[3].text, "arcsinus -> arcsin | arccosinus -> arccos");
        screen[6].box[3].textLg=strlen(screen[6].box[3].text);
        strcpy(screen[6].box[4].text, "arctangenta -> arctg | arccotangenta -> arcctg");
        screen[6].box[4].textLg=strlen(screen[6].box[4].text);
        strcpy(screen[6].box[5].text, "radical -> rad | modul -> abs | exponential -> e | logaritm -> ln");
        screen[6].box[5].textLg=strlen(screen[6].box[5].text);
        strcpy(screen[6].box[6].text, "La fonction saisie doit être entre parenthèses.");
        screen[6].box[6].textLg=strlen(screen[6].box[6].text);
        strcpy(screen[6].box[7].text, "Les nombres négatifs doivent être entre parenthèses.");
        screen[6].box[7].textLg=strlen(screen[6].box[7].text);
        strcpy(screen[6].box[8].text, "Pour entrer une fonction dans la fenêtre actuelle, appuyez sur ENTER.");
        screen[6].box[8].textLg=strlen(screen[6].box[8].text);
        strcpy(screen[6].box[9].text, "La variable X doit être écrite en majuscules!");
        screen[6].box[9].textLg=strlen(screen[6].box[9].text);
        strcpy(screen[6].box[10].text, "Fermer une fenêtre arrêtera le programme!");
        screen[6].box[10].textLg=strlen(screen[6].box[10].text);
    }
    else if(langCnt==3)
    {
        strcpy(screen[6].title, "Anweisungen");
        screen[6].titleLg=strlen(screen[6].title);
        strcpy(screen[6].box[0].text, "Zurück");
        screen[6].box[0].textLg=strlen(screen[6].box[0].text);
        strcpy(screen[6].box[1].text, "Funktionen:");
        screen[6].box[1].textLg=strlen(screen[6].box[1].text);
        strcpy(screen[6].box[2].text, "sinus -> sin | cosinus -> cos | tangenta -> tg | cotangenta -> ctg");
        screen[6].box[2].textLg=strlen(screen[6].box[2].text);
        strcpy(screen[6].box[3].text, "arcsinus -> arcsin | arccosinus -> arccos");
        screen[6].box[3].textLg=strlen(screen[6].box[3].text);
        strcpy(screen[6].box[4].text, "arctangenta -> arctg | arccotangenta -> arcctg");
        screen[6].box[4].textLg=strlen(screen[6].box[4].text);
        strcpy(screen[6].box[5].text, "radical -> rad | modul -> abs | exponential -> e | logaritm -> ln");
        screen[6].box[5].textLg=strlen(screen[6].box[5].text);
        strcpy(screen[6].box[6].text, "Die eingegebene Funktion muss in runden Klammern stehen.");
        screen[6].box[6].textLg=strlen(screen[6].box[6].text);
        strcpy(screen[6].box[7].text, "Negative Zahlen müssen in runden Klammern stehen.");
        screen[6].box[7].textLg=strlen(screen[6].box[7].text);
        strcpy(screen[6].box[8].text, "Um eine Funktion in das aktuelle Fenster einzugeben, drücken Sie ENTER.");
        screen[6].box[8].textLg=strlen(screen[6].box[8].text);
        strcpy(screen[6].box[9].text, "Die X-Variable muss in Großbuchstaben geschrieben werden!");
        screen[6].box[9].textLg=strlen(screen[6].box[9].text);
        strcpy(screen[6].box[10].text, "Das Schließen eines Fensters stoppt das Programm!");
        screen[6].box[10].textLg=strlen(screen[6].box[10].text);
    }
    else if(langCnt==4)
    {
        strcpy(screen[6].title, "Instrucciones");
        screen[6].titleLg=strlen(screen[6].title);
        strcpy(screen[6].box[0].text, "Atrás");
        screen[6].box[0].textLg=strlen(screen[6].box[0].text);
        strcpy(screen[6].box[1].text, "Funciones:");
        screen[6].box[1].textLg=strlen(screen[6].box[1].text);
        strcpy(screen[6].box[2].text, "sinus -> sin | cosinus -> cos | tangenta -> tg | cotangenta -> ctg");
        screen[6].box[2].textLg=strlen(screen[6].box[2].text);
        strcpy(screen[6].box[3].text, "arcsinus -> arcsin | arccosinus -> arccos");
        screen[6].box[3].textLg=strlen(screen[6].box[3].text);
        strcpy(screen[6].box[4].text, "arctangenta -> arctg | arccotangenta -> arcctg");
        screen[6].box[4].textLg=strlen(screen[6].box[4].text);
        strcpy(screen[6].box[5].text, "radical -> rad | modul -> abs | exponential -> e | logaritm -> ln");
        screen[6].box[5].textLg=strlen(screen[6].box[5].text);
        strcpy(screen[6].box[6].text, "La función ingresada debe estar entre paréntesis.");
        screen[6].box[6].textLg=strlen(screen[6].box[6].text);
        strcpy(screen[6].box[7].text, "Los números negativos deben estar entre paréntesis.");
        screen[6].box[7].textLg=strlen(screen[6].box[7].text);
        strcpy(screen[6].box[8].text, "Para ingresar una función en la ventana actual, presione ENTER");
        screen[6].box[8].textLg=strlen(screen[6].box[8].text);
        strcpy(screen[6].box[9].text, "¡La variable X debe escribirse en mayúsculas!");
        screen[6].box[9].textLg=strlen(screen[6].box[9].text);
        strcpy(screen[6].box[10].text, "Cerrar una ventana detendrá el programa!");
        screen[6].box[10].textLg=strlen(screen[6].box[10].text);
    }

    titleWidthCh=widthch[4];
    titleHeightCh=heightch[4];

    xBoxOffset=widthch[4]/2;
    yBoxOffset=widthch[4]/4;

    screen[6].box[0].x1=backOffset, screen[6].box[0].y1=window[1].height-backOffset-heightch[4]-2*yBoxOffset,
    screen[6].box[0].x2=backOffset+(screen[6].box[0].textLg*widthch[4])*(screen[6].box[0].textLg>=6)+(6*widthch[4])*(screen[6].box[0].textLg<6)+2*xBoxOffset, screen[6].box[0].y2=window[1].height-backOffset;

    screen[6].box[1].x1=window[1].width/2-screen[6].box[1].textLg/2*widthch[4], screen[6].box[1].y1=3*titleHeightOffset,
    screen[6].box[1].x2=window[1].width/2+screen[6].box[1].textLg/2*widthch[4], screen[6].box[1].y2=screen[6].box[1].y1+2*yBoxOffset;

    screen[6].box[2].x1=window[1].width/2-screen[6].box[2].textLg/2*widthch[4], screen[6].box[2].y1=screen[6].box[1].y2+titleHeightOffset,
    screen[6].box[2].x2=window[1].width/2+screen[6].box[2].textLg/2*widthch[4], screen[6].box[2].y2=screen[6].box[2].y1+2*yBoxOffset;

    screen[6].box[3].x1=window[1].width/2-screen[6].box[3].textLg/2*widthch[4], screen[6].box[3].y1=screen[6].box[2].y2+titleHeightOffset,
    screen[6].box[3].x2=window[1].width/2+screen[6].box[3].textLg/2*widthch[4], screen[6].box[3].y2=screen[6].box[3].y1+2*yBoxOffset;

    screen[6].box[4].x1=window[1].width/2-screen[6].box[4].textLg/2*widthch[4], screen[6].box[4].y1=screen[6].box[3].y2+titleHeightOffset,
    screen[6].box[4].x2=window[1].width/2+screen[6].box[4].textLg/2*widthch[4], screen[6].box[4].y2=screen[6].box[4].y1+2*yBoxOffset;

    screen[6].box[5].x1=window[1].width/2-screen[6].box[5].textLg/2*widthch[4], screen[6].box[5].y1=screen[6].box[4].y2+titleHeightOffset,
    screen[6].box[5].x2=window[1].width/2+screen[6].box[5].textLg/2*widthch[4], screen[6].box[5].y2=screen[6].box[5].y1+2*yBoxOffset;

    screen[6].box[6].x1=window[1].width/2-screen[6].box[6].textLg/2*widthch[4], screen[6].box[6].y1=screen[6].box[5].y2+2*titleHeightOffset,
    screen[6].box[6].x2=window[1].width/2+screen[6].box[6].textLg/2*widthch[4], screen[6].box[6].y2=screen[6].box[6].y1+2*yBoxOffset;

    screen[6].box[7].x1=window[1].width/2-screen[6].box[7].textLg/2*widthch[4], screen[6].box[7].y1=screen[6].box[6].y2+titleHeightOffset,
    screen[6].box[7].x2=window[1].width/2+screen[6].box[7].textLg/2*widthch[4], screen[6].box[7].y2=screen[6].box[7].y1+2*yBoxOffset;

    screen[6].box[8].x1=window[1].width/2-screen[6].box[8].textLg/2*widthch[4], screen[6].box[8].y1=screen[6].box[7].y2+titleHeightOffset,
    screen[6].box[8].x2=window[1].width/2+screen[6].box[8].textLg/2*widthch[4], screen[6].box[8].y2=screen[6].box[8].y1+2*yBoxOffset;

    screen[6].box[9].x1=window[1].width/2-screen[6].box[9].textLg/2*widthch[4], screen[6].box[9].y1=screen[6].box[8].y2+titleHeightOffset,
    screen[6].box[9].x2=window[1].width/2+screen[6].box[9].textLg/2*widthch[4], screen[6].box[9].y2=screen[6].box[9].y1+2*yBoxOffset;

    screen[6].box[10].x1=window[1].width/2-screen[6].box[10].textLg/2*widthch[4], screen[6].box[10].y1=screen[6].box[9].y2+titleHeightOffset,
    screen[6].box[10].x2=window[1].width/2+screen[6].box[10].textLg/2*widthch[4], screen[6].box[10].y2=screen[6].box[10].y1+2*yBoxOffset;


    ///TITLU
    outtextxy(window[1].width/2-screen[6].titleLg/2*titleWidthCh, titleHeightOffset, screen[6].title);

    ///INSTRUCTIUNI
    outtextxy(screen[6].box[1].x1+xBoxOffset, screen[6].box[1].y1+yBoxOffset, screen[6].box[1].text);
    outtextxy(screen[6].box[2].x1+xBoxOffset, screen[6].box[2].y1+yBoxOffset, screen[6].box[2].text);
    outtextxy(screen[6].box[3].x1+xBoxOffset, screen[6].box[3].y1+yBoxOffset, screen[6].box[3].text);
    outtextxy(screen[6].box[4].x1+xBoxOffset, screen[6].box[4].y1+yBoxOffset, screen[6].box[4].text);
    outtextxy(screen[6].box[5].x1+xBoxOffset, screen[6].box[5].y1+yBoxOffset, screen[6].box[5].text);
    outtextxy(screen[6].box[6].x1+xBoxOffset, screen[6].box[6].y1+yBoxOffset, screen[6].box[6].text);
    outtextxy(screen[6].box[7].x1+xBoxOffset, screen[6].box[7].y1+yBoxOffset, screen[6].box[7].text);
    outtextxy(screen[6].box[8].x1+xBoxOffset, screen[6].box[8].y1+yBoxOffset, screen[6].box[8].text);
    outtextxy(screen[6].box[9].x1+xBoxOffset, screen[6].box[9].y1+yBoxOffset, screen[6].box[9].text);
    outtextxy(screen[6].box[10].x1+xBoxOffset, screen[6].box[10].y1+yBoxOffset, screen[6].box[10].text);

    ///BUTON INAPOI
    rectangle(screen[6].box[0].x1, screen[6].box[0].y1, screen[6].box[0].x2, screen[6].box[0].y2);
    outtextxy(screen[6].box[0].x1+(screen[6].box[0].x2-screen[6].box[0].x1)/2-widthch[4]*screen[6].box[0].textLg/2, screen[6].box[0].y1+(screen[6].box[0].y2-screen[6].box[0].y1)/2-heightch[4]/2, screen[6].box[0].text);

    HoverClick(screen[6].box[0].x1, screen[6].box[0].y1, screen[6].box[0].x2, screen[6].box[0].y2, screen[6].box[0].x1+(screen[6].box[0].x2-screen[6].box[0].x1)/2-widthch[4]*screen[6].box[0].textLg/2, screen[6].box[0].y1+(screen[6].box[0].y2-screen[6].box[0].y1)/2-heightch[4]/2,
               screen[6].box[0].text, screen[6].box[0].textLg, widthch[4], heightch[4], 6, 1, 0, theme[themeCnt].hovercolor, theme[themeCnt].bkcolor);
    if(exitf)
    {
        cleardevice();
        settextstyle(8, 0, 4);
        return 0;
    }
}

void functionTextBuild()
{
    screen[4].box[functionCnt].text[0]='\0';
    if(langCnt==0)
    {
        strcat(screen[4].box[functionCnt].text, "Functie: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].expr);
        strcat(screen[4].box[functionCnt].text, ",  (");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].a);
        strcat(screen[4].box[functionCnt].text, ",  ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].b);
        strcat(screen[4].box[functionCnt].text, "), distanta: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].dist);
        screen[4].box[functionCnt].textLg=strlen(screen[4].box[functionCnt].text);
    }
    else if(langCnt==1)
    {
        strcat(screen[4].box[functionCnt].text, "Function: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].expr);
        strcat(screen[4].box[functionCnt].text, ",  (");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].a);
        strcat(screen[4].box[functionCnt].text, ",  ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].b);
        strcat(screen[4].box[functionCnt].text, "), distance: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].dist);
        screen[4].box[functionCnt].textLg=strlen(screen[4].box[functionCnt].text);
    }
    else if(langCnt==2)
    {
        strcat(screen[4].box[functionCnt].text, "Fonction: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].expr);
        strcat(screen[4].box[functionCnt].text, ",  (");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].a);
        strcat(screen[4].box[functionCnt].text, ",  ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].b);
        strcat(screen[4].box[functionCnt].text, "), distance: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].dist);
        screen[4].box[functionCnt].textLg=strlen(screen[4].box[functionCnt].text);
    }
    else if(langCnt==3)
    {
        strcat(screen[4].box[functionCnt].text, "Funktion: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].expr);
        strcat(screen[4].box[functionCnt].text, ",  (");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].a);
        strcat(screen[4].box[functionCnt].text, ",  ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].b);
        strcat(screen[4].box[functionCnt].text, "), distanz: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].dist);
        screen[4].box[functionCnt].textLg=strlen(screen[4].box[functionCnt].text);
    }
    else if(langCnt==4)
    {
        strcat(screen[4].box[functionCnt].text, "Función: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].expr);
        strcat(screen[4].box[functionCnt].text, ",  (");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].a);
        strcat(screen[4].box[functionCnt].text, ",  ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].b);
        strcat(screen[4].box[functionCnt].text, "), distancia: ");
        strcat(screen[4].box[functionCnt].text, screen[4].box[functionCnt].dist);
        screen[4].box[functionCnt].textLg=strlen(screen[4].box[functionCnt].text);
    }
    if(screen[4].box[functionCnt].textLg>window[1].width/12-37)
    {
        screen[4].box[functionCnt].text[window[1].width/12-42]='\0';
        strcat(screen[4].box[functionCnt].text, "...");
        screen[4].box[functionCnt].textLg=strlen(screen[4].box[functionCnt].text);
    }
}

void ThemeInit()
{
    theme[1].bkcolor=COLOR(173, 139, 115);
    theme[1].hovercolor=COLOR(206, 171, 147);
    theme[1].bordercolor=WHITE;

    theme[2].bkcolor=COLOR(75, 89, 69);
    theme[2].hovercolor=COLOR(102, 120, 95);
    theme[2].bordercolor=WHITE;

    theme[3].bkcolor=COLOR(129, 116, 160);
    theme[3].hovercolor=COLOR(168, 136, 181);
    theme[3].bordercolor=WHITE;

    theme[4].bkcolor=COLOR(33, 53, 85);
    theme[4].hovercolor=COLOR(62, 88, 121);
    theme[4].bordercolor=WHITE;

    theme[5].bkcolor=COLOR(128, 0, 0);
    theme[5].hovercolor=COLOR(152, 43, 28);
    theme[5].bordercolor=WHITE;

    themeCnt=1;
}

void TextStyleInit()
{
    settextstyle(8, 0, 1);
    widthch[1]=textwidth("a");
    heightch[1]=textheight("a");

    settextstyle(8, 0, 2);
    widthch[2]=textwidth("a");
    heightch[2]=textheight("a");

    settextstyle(8, 0, 4);
    widthch[4]=textwidth("a");
    heightch[4]=textheight("a");

    settextstyle(8, 0, 5);
    widthch[5]=textwidth("a");
    heightch[5]=textheight("a");

    settextstyle(8, 0, 4);
}

int main()
{
    FILE *fp;
    double x_scale, y_scale, y;
    int page=0, i, j, k, go=1;
    char c;
    bool exitbox=0;
    window[1].width=getmaxwidth();
    window[1].height=getmaxheight();
    window[2].width=getmaxwidth();
    window[2].height=getmaxheight();

    ///INITIALIZARE FEREASTRA MENIU

    strcpy(window[1].title, "Graficul unei functii");
    window[1].id=initwindow(window[1].width, window[1].height, window[1].title, 0, 0);
    window[1].handle=FindWindowA(NULL, "Graficul unei functii");
    aux[0]='\0';
    screen[1].active=1;
    setlinestyle(SOLID_LINE, 0, 1);
    //setcurrentwindow(win1);

    ///INITIALIZARE STILURI TEXT
    TextStyleInit();

    ///CULORI PENTRU INTERFATA
    ThemeInit();

    setcolor(theme[themeCnt].bordercolor);

    setbkcolor(theme[themeCnt].bkcolor);
    cleardevice();

    /*for(i=1; i<=24; i++)
    {
        fIndex[i]=1;
    }*/

    ///INITIALIZARE FUNCTII ANTERIOARE

    fp=fopen("functii.txt", "r");
    sepFNr=1;
    while(fscanf(fp, "%hu", &screen[4].box[sepFNr].fNr)==1)
    {
        fscanf(fp, "%3s %s %s %s %s", &screen[4].box[functionCnt].ind, &screen[4].box[functionCnt].expr, &screen[4].box[functionCnt].a, &screen[4].box[functionCnt].b, &screen[4].box[functionCnt].dist);
        functionTextBuild();
        functionCnt++;
        functionNr++;
        for(i=1; i<screen[4].box[sepFNr].fNr; i++)
        {
            fscanf(fp, "%s %s %s %s", &screen[4].box[functionCnt].expr, &screen[4].box[functionCnt].a, &screen[4].box[functionCnt].b, &screen[4].box[functionCnt].dist);
            functionTextBuild();
            functionCnt++;
            functionNr++;
        }
        sepFNr++;
    }
    fclose(fp);
    if((functionNr-1)%8==0)
    {
        howManyPages=(functionNr-1)/8;
    }
    else
    {
        howManyPages=(functionNr-1)/8+1;
    }
    setcolor(WHITE);

    ///BUCLA UPDATE
    while(true)
    {
        //cout<<window[1].screen<<' '<<window[2].screen<<'\n';
        //cout<<1;
        /*for(i=1; i<=windowNr; i++)
        {
            cout<<fIndex[i]<<' ';
        }
        cout<<'\n';*/
        char aux[200];
        HWND foregroundWin = GetForegroundWindow();
        GetWindowTextA(foregroundWin, aux, 200);
        for(i=1; i<=windowNr; i++)
        {
            if(strcmp(aux, window[i].title)==0)
            {
                setcurrentwindow(window[i].id);
                //cout<<window[i].id<<'\n';
                //cout<<window[i].screen;
                //cout<<i;
                currWindow=getcurrentwindow();
                //cout<<F[currWindow].expresie<<' '<<F[currWindow].a<<' '<<F[currWindow].b<<' '<</*F[currWindow].dist<<*/'\n';
                //cout<<currWindow<</*' '<<prevWindow<<*/'\n';
                //cout<<currWindow<<'\n';
                if(currWindow!=prevWindow || windowNr>prevWindowNr)
                {
                    //showCoords=1;
                    //cout<<F[currWindow].expresie<<' '<<F[currWindow].a<<' '<<F[currWindow].b<<' '<</*F[currWindow].dist<<*/'\n';
                    for(j=1; j<=6; j++)
                    {
                        screen[j].active=0;
                    }
                    //cout<<window[1].screen<<' '<<window[2].screen<<'\n';
                    screen[window[i].screen].active=1;
                    //screen[window[prevWindow].screen].active=0;
                }
            }
            /*else
            {
                screen[i].active=0;
            }*/
        }
        for(i=1; i<=6; i++)
        {
            if(screen[i].active==1)
            {
                //cout<<i<<'\n';
            }
        }
        //cout<<aux<<'\n';
        for(i=1; i<=windowNr; i++)
        {
            window[i].handle=FindWindowA(NULL, window[i].title);
            GetWindowRect(window[i].handle, &window[i].rect);
        }
        prevWindow=getcurrentwindow();
        prevWindowNr=windowNr;
        //cout<<window[1].rect.left<<' '<<window[1].rect.right<<' '<<window[1].rect.top<<' '<<window[1].rect.bottom<<'\n';
        if(screen[1].active==1)
        {
            Screen1(posx, posy, initS1);
        }
        if(screen[2].active==1)
        {
            //cout<<1;
            Screen2(enter);
            if(enter==1) ///A FOST APASAT ENTER
            {
                cnt++;
                currWindow=windowNr;
                fIndex[currWindow]++;
                F[currWindow][cnt].expresie[0]='\0';
                strcpy(F[currWindow][cnt].expresie, screen[2].box[1].insText);
                F[currWindow][cnt].lung=strlen(F[currWindow][cnt].expresie)-1;
                auxExpr[cnt][0]='\0';
                strcpy(auxExpr[cnt], F[currWindow][cnt].expresie);
                adaptExpr(F[currWindow][cnt].expresie, F[currWindow][cnt].lung);
                exprLgUnmod=F[currWindow][cnt].lung;
                AddToVect(cnt);
                //cout<<F[cnt].expresie;
                BuildValue(F[currWindow][cnt].a, screen[2].box[2].insText, screen[2].box[2].insTextLg);
                if(wrongi==0)
                {
                    BuildValue(F[currWindow][cnt].b, screen[2].box[3].insText, screen[2].box[3].insTextLg);
                    if(wrongi==0)
                    {
                        BuildValue(F[currWindow][cnt].dist, screen[2].box[4].insText, screen[2].box[4].insTextLg);
                        if(wrongf==0 && wrongi==0) ///FUNCTIA INTRODUSA E CORECTA, INTERVALUL INTRODUS E CORECT
                        {
                            bar(screen[2].box[1].x1, window[1].height-100, screen[2].box[1].x1+30*widthch[4], window[1].height-100+heightch[4]);
                            outtextxy(screen[2].box[1].x1, window[1].height-100, "Functie salvata!");
                            delay(1000);
                            bar(screen[2].box[1].x1, window[1].height-100, screen[2].box[1].x1+30*widthch[4], window[1].height-100+heightch[4]);
                            if(cnt==1)
                            {
                                DIST=F[currWindow][cnt].dist;
                            }
                            //screen[2].active=0;
                            //screen[3].active=1;
                            enter=0;
                        }
                        else
                        {
                            cnt--;
                            fIndex[currWindow]--;
                            F[currWindow][1].a=F[currWindow][1].b=F[currWindow][1].dist=0;
                            cntvect=0;
                            posx=posy=-1; ///SE TOT INTRA IN FUNCTIA INPUTBOX DAR RESTUL NU SE DESENA
                            enter=0;
                        }
                    }
                    else
                    {
                        cnt--;
                        fIndex[currWindow]--;
                        F[currWindow][1].a=F[currWindow][1].b=F[currWindow][1].dist=0;
                        cntvect=0;
                        posx=posy=-1;
                        enter=0;
                    }
                }
                else
                {
                    cnt--;
                    fIndex[currWindow]--;
                    F[currWindow][1].a=F[currWindow][1].b=F[currWindow][1].dist=0;
                    cntvect=0;
                    posx=posy=-1;
                    enter=0;
                }
            }
            currWindow=0;
        }
        if(screen[3].active==1)
        {
            //cout<<F.expresie<<' '<<F.lung<<' '<<A<<' '<<B<<' '<<dist<<'\n';
            /*for(i=0; i<=F.lung; i++)
            {
                cout<<F.vect[i]<<'\n';
            }*/
            //cout<<currWindow<</*' '<<prevWindow<<*/'\n';
            Screen3();
        }
        if(screen[4].active==1)
        {
            Screen4();
        }
        if(screen[5].active==1)
        {
            Screen5();
        }
        if(screen[6].active==1)
        {
            Screen6();
        }
    }
    closegraph();
}
