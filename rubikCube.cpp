#include<iostream>
#include<string>
#include<stdio.h>
#define WHITE 0
#define BLUE 1
#define YELLOW 2
#define GREEN 3
#define ORANGE 4
#define RED 5
//using namespace std;
/*int OUTCNT = 0;*/
class AdjEdgeRes{
public:
    int sidei;
    char adjClr;
    AdjEdgeRes(int i, char ch){
        sidei=i;
        adjClr=ch;
    }
};

class side{
    public:
    side *l,*r,*u,*d,*f,*b;
    int i;
    char mat[3][3];
    side(){
        for(short int i=0;i<3;i++)
            for(short int j=0;j<3;j++){
                mat[i][j]='\0';
            }
    }
};

class cube{
    side sides[6];
    void rotateSurfaceCW(int side);
    void rotateSurfaceCCW(int side);
    void setColorFrontToRight();
    void setColorRightToFront();
    AdjEdgeRes getAdjSideEdge(int s, int i,int j);
public:
    int front,back,left,right,up,down;
    int white, blue, yellow, green, orange, red;
    void inputCube();
    void print();
    void flip();
    void spinCW();
    void spinCCW();
    void setColorToFront(char clr);
    void rotateCW();
    void rotateCCW();
    void solveWhiteCross();
};

void cube::inputCube(){
    std::cout<<"[FUNCTION CALL] void cube::inputCube()\n";
    std::cout<<"Begin cube input..\n";
    std::string str[6]={"front", "right", "back", "left", "up", "down"};
    for(int s=0;s<6;s++){
        std::cout<<"Enter for "<<str[s]<<" side:";
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                std::cin>>sides[s].mat[i][j];
            }
        }
        sides[s].i=s;
        if(str[s]=="front") front=s;
        else if(str[s]=="right") right=s;
        else if(str[s]=="back") back=s;
        else if(str[s]=="left") left=s;
        else if(str[s]=="up") up=s;
        else if(str[s]=="down") down=s;
    }
    sides[front].b=&sides[back];
    sides[front].f=&sides[front];
    sides[front].r=&sides[right];
    sides[front].l=&sides[left];
    sides[front].u=&sides[up];
    sides[front].d=&sides[down];

    sides[right].b=&sides[left];
    sides[right].f=&sides[right];
    sides[right].r=&sides[back];
    sides[right].l=&sides[front];
    sides[right].u=&sides[up];
    sides[right].d=&sides[down];

    sides[back].b=&sides[front];
    sides[back].f=&sides[back];
    sides[back].r=&sides[left];
    sides[back].l=&sides[right];
    sides[back].u=&sides[up];
    sides[back].d=&sides[down];

    sides[left].b=&sides[right];
    sides[left].f=&sides[left];
    sides[left].r=&sides[front];
    sides[left].l=&sides[back];
    sides[left].u=&sides[up];
    sides[left].d=&sides[down];

    sides[up].b=&sides[back];
    sides[up].f=&sides[up];
    sides[up].r=&sides[right];
    sides[up].l=&sides[left];
    sides[up].u=&sides[up];
    sides[up].d=&sides[down];

    std::cout<<"..indput done!\nPrinting input cube..\n";
    print();
}

void cube::print(){
    std::cout<<"[FUNCTION CALL] void cube::print()\n";
    std::string str;
    for(int s=0;s<6;s++){
        if(s==front) str= "front";
        else if(s==right) str= "right";
        else if(s==back) str= "back";
        else if(s==left) str= "left";
        else if(s==up) str= "up";
        else if(s==down) str= "down";
        std::cout<<"---------"<<str<<" side-----------\n";
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                std::cout<<sides[s].mat[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
    }
}

void cube::flip(){
    std::cout<<"[FUNCTION CALL] void cube::flip()\n";
    //flip whole cube
    int tmp=left;
    left=up;
    up=right;
    right=down;
    down=tmp;
}

void cube::rotateSurfaceCW(int side){
    std::cout<<"[FUNCTION CALL] void cube::rotateSurfaceCW()\n";
    //save upper row
    int saved[3],i;
    for(i=0;i<3;i++)
        saved[i]=sides[side].mat[0][i];
    //up row = left row
    for(i=0;i<3;i++)
        sides[side].mat[0][i]=sides[side].mat[2-i][0];
    //left row = down row
    for(i=0;i<3;i++)
        sides[side].mat[i][0]=sides[side].mat[2][i];
    //down row = right row
    for(i=0;i<3;i++)
        sides[side].mat[2][i]=sides[side].mat[2-i][2];
    //right row = saved
    for(i=0;i<3;i++)
        sides[side].mat[i][2]=saved[i];
}

void cube::rotateSurfaceCCW(int side){
    std::cout<<"[FUNCTION CALL] void cube::rotateSurfaceCCW()\n";
    //save upper row
    int saved[3],i;
    for(i=0;i<3;i++)
        saved[i]=sides[side].mat[0][i];
    //up = right
    for(i=0;i<3;i++)
        sides[side].mat[0][i]=sides[side].mat[i][2];
    //right = down
    for(i=0;i<3;i++)
        sides[side].mat[i][2]=sides[side].mat[2][2-i];
    //down = left
    for(i=0;i<3;i++)
        sides[side].mat[2][i]=sides[side].mat[i][0];
    //left = saved
    for(i=0;i<3;i++)
        sides[side].mat[i][0]=saved[2-i];
}

void cube::spinCW(){
    std::cout<<"[FUNCTION CALL] void cube::spinCW()\n";
    //spin whole cube clockwise
    int tmp=front;
    front=up;
    up=back;
    back=down;
    down=tmp;
    //update left side CW
    rotateSurfaceCW(left);
    //update right side CCW
    rotateSurfaceCCW(right);
}

void cube::spinCCW(){
    std::cout<<"[FUNCTION CALL] void cube::spinCCW()\n";
    //spin whole cube counter clockwise
    int saved[3],i,tmp=front;
    front=down;
    down=back;
    back=up;
    up=tmp;
    //update left side CCW
    rotateSurfaceCCW(left);
    //update right side CW
    rotateSurfaceCW(right);
}

void cube::setColorToFront(char clr){
    std::cout<<"[FUNCTION CALL] void cube::setColorToFront( clr="<< clr <<" )\n";
    int clri=0;
    std::cout<<"Start Bringing "<<clr<<" to front..\n";
    for(int s=0;s<6;s++){
        if(sides[s].mat[2][2] == clr){
            clri=s;
            break;
        }
    }
    std::cout<<"Target Color "<<clr<<" is at ";
    if(clri==front){
        std::cout<<"front side..\n";
    } else if(clri==right){
        std::cout<<"right side..\n";
        flip();
        spinCW();
    } else if(clri==back){
        std::cout<<"back side..\n";
        spinCW();
        spinCW();
    } else if(clri==left){
        std::cout<<"left side..\n";
        flip();
        spinCCW();
    } else if(clri==up){
        std::cout<<"up side..\n";
        spinCW();
    } else if(clri==down){
        std::cout<<"down side..\n";
        spinCCW();
    }
    std::cout<<"Done bringing "<<clr<<" to front..\n";
}

void cube::rotateCW(){
    std::cout<<"[FUNCTION CALL] void cube::rotateCW()\n";
    short int i;
    //save up
    char upSaved[3];
    for(i=0;i<3;i++)
        upSaved[i] = sides[up].mat[2][i];
    //up=front
    for(i=0;i<3;i++)
        sides[up].mat[2][i] = sides[front].mat[2][i];
    //front=down
    for(i=0;i<3;i++)
        sides[front].mat[2][i] = sides[down].mat[2][i];
    //down=back
    for(i=0;i<3;i++)
        sides[down].mat[2][i] = sides[back].mat[2][i];
    //back=upSaved
    for(i=0;i<3;i++)
        sides[back].mat[2][i] = upSaved[i];
    //adjust right side CW
    rotateSurfaceCCW(right);
}

void cube::rotateCCW(){
    std::cout<<"[FUNCTION CALL] void cube::rotateCCW()\n";
    short int i;
    //save up
    char upSaved[3];
    for(i=0;i<3;i++)
        upSaved[i] = sides[up].mat[2][i];
    //up=back
    for(i=0;i<3;i++)
        sides[up].mat[2][i] = sides[back].mat[2][i];
    //back=down
    for(i=0;i<3;i++)
        sides[back].mat[2][i] = sides[down].mat[2][i];
    //down=front
    for(i=0;i<3;i++)
        sides[down].mat[2][i] = sides[front].mat[2][i];
    //front=upSaved
    for(i=0;i<3;i++)
        sides[front].mat[2][i] = upSaved[i];
    //adjust right side CW
    rotateSurfaceCW(right);
}

void cube::setColorFrontToRight(){
    std::cout<<"[FUNCTION CALL] void cube::setColorFrontToRight()\n";
    spinCW();
    flip();
}

void cube::setColorRightToFront(){
    std::cout<<"[FUNCTION CALL] void cube::setColorRightToFront()\n";
    flip();
    spinCW();
}

AdjEdgeRes cube::getAdjSideEdge(int s, int i, int j){
    std::cout<<"[FUNCTION CALL] cube::AdjEdgeRes getAdjSideEdge( "<<s<<" ,"<<i<<" ,"<<j<<" )\n";
    if(i==2 && j==1){
        return AdjEdgeRes((sides[s].d)->i, (sides[s].d)->mat[0][1]);
    }
    if(i==1 && j==0){
        return AdjEdgeRes((sides[s].l)->i, (sides[s].l)->mat[0][1]);
    }
    if(i==0&& j==1){
        return AdjEdgeRes((sides[s].u)->i, (sides[s].u)->mat[0][1]);
    }
    if(i==1&& j==2){
        return AdjEdgeRes((sides[s].r)->i, (sides[s].r)->mat[0][1]);
    }
    std::cout<<"[ERROR] getAdjSideEdge: Invalid adjacent side requested..\n";
    return AdjEdgeRes(-1,'\0');
}

void cube::solveWhiteCross(){
    std::cout<<"[FUNCTION CALL] void cube::solveWhiteCross()\n";
    setColorToFront('w');
    int crossColors[4]={right, up, left, down},i;
    bool doneCrossColor[4]={false};
    int whiteCrossEdgePos[4][2]={{2,1}, {1,0}, {0,1}, {1,2}};
    //white present in white cross?
    for(i=0;i<4;i++){
        if(sides[front].mat[whiteCrossEdgePos[i][0]][whiteCrossEdgePos[i][1]] == 'w'){
            break;
        }
    }
    if(i<4){
        //yes, white present in white cross
        std::cout<<"[MSG] white present in white cross\n";
        setColorFrontToRight();
        for(i=0;i<4;i++){
            if(sides[right].mat[whiteCrossEdgePos[i][0]][whiteCrossEdgePos[i][1]] == 'w'){
                break;
            }
        }
        //align with correct color
        AdjEdgeRes adj=getAdjSideEdge(right, whiteCrossEdgePos[i][0], whiteCrossEdgePos[i][1]);
        std::cout<<"[DEBUG] while loop in solveWhiteCross started..\n";
        while(sides[adj.sidei].mat[2][2]!=adj.adjClr){
            rotateCW();
            i=(i+1)%4;
            adj=getAdjSideEdge(right, whiteCrossEdgePos[i][0], whiteCrossEdgePos[i][1]);
        }
        doneCrossColor[i]=true;
        std::cout<<"[DEBUG] while loop in solveWhiteCross ended!\n";
        std::cout<<"[MSG] "<<adj.adjClr<<" aligned correctly for white cross\n";

    }
    for(i=0; i<4;i++){
        
    }
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
}

int main(){
    cube cb;
    cb.inputCube();
    while(true){
        char col;
        std::cout<<"Color? :";
        std::cin>>col;
        if(col=='e')
            break;
        if(col=='c'){
            cb.rotateCW();
        }else if(col=='a'){
            cb.rotateCCW();
        }
        //cb.setColorToFront(col);
        cb.print();
    }
    //cb.solveWhiteCross();
    cb.print();
}