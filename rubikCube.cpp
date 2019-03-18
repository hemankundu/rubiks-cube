#include<iostream>
#include<string>
#include<stdio.h>
#include<vector>
#include "cameraInput.cpp"
//#include "serial.cpp"

#define WHITE 0
#define BLUE 1
#define YELLOW 2
#define GREEN 3
#define ORANGE 4
#define RED 5
using std::cout;
using std::string;
//using namespace std;
/*int OUTCNT = 0;*/

//const char outFileName[]="cube-color.txt";
class AdjEdgeRes{
public:
    int sidei;
    char adjClr;
    AdjEdgeRes(int i, char ch){
        sidei=i;
        adjClr=ch;
    }
    AdjEdgeRes(){}
};

class cornerPiece{
public:
    int position[2];
    int sidei;
    cornerPiece(){}
    cornerPiece(int s, int i, int j){
        sidei=s;
        position[0]=i;
        position[1]=j;
    }
};

class side{
    public:
    //side *l,*r,*u,*d,*f,*b;
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
    int front,back,left,right,up,down;
    cornerPiece leftSideCorners[4][3],rightSideCorners[4][3];
    int CrossEdgePos[4][2]={{2,1}, {1,0}, {0,1}, {1,2}};
    void rotateSurfaceCW(int side);
    void rotateSurfaceCCW(int side);
    void setColorFrontToRight();
    void setColorRightToFront();
    int getAdjSide(int s, char side);
    bool matchCorner(cornerPiece c[], char x, char y, char z);
    AdjEdgeRes getAdjSideEdge(int s, int i,int j);
    int getColorSide(char clr);
    void solveWhiteCrossPosOri(AdjEdgeRes adj, int edgei);
    bool setRightEdgeToSide(char clrRight, char clrAdj, char clrSide);
    void rotateByColor(char clr, bool cw);
    void whiteEdgeAtRightWAdj(char clr);
    bool moveCornerWhiteToYellow();
    void loadLeftSideCorners();
    void loadRightSideCorners();
    bool secondLayerLR(char, char);
    void secondLayerLeftRight(char, char, bool);
    void solveYellowEdgePosition();
    void solveYellowEdgePositionHelper(char);
public:
    string operations[2000];
    char op[2000][7]={'\0'};
    int opCnt, opStrCnt;
    //int white, blue, yellow, green, orange, red;
    void inputCube();
    void print();
    void getOpStr();
    void flip();
    void spinCW();
    void spinCCW();
    void setColorToFront(char clr);
    void setColorToRight(char clr);
    void rotateCW();
    void rotateCCW();
    void solveWhiteCross();
    void solveWhiteCorner();
    void solveSecondLayer();
    void solveYellowCross();
    //void solveYellowEdgePosition();
    void positionYellowCorner();
    void orientYellowCorner();
    cube(){
        opCnt=0;
        opStrCnt=0;
    }
};



void cube::inputCube(){
    //std::cout<<"[FUNCTION CALL] void cube::inputCube()\n";
    char ch;
    std::string str[6]={"front", "right", "back", "left", "up", "down"};
    vector<char*> inputCmdStr[6]={{"FL1>"}, {"FL1>"}, {"FL1>"}, {"SP1>", "FL1>"}, {"FL1>", "FL1>"}, {"FL1>"}};
    while(true){
        cout<<"Do you want to use existing file for input?\n[y/n]: ";
        ch=getchar();
        if(ch=='n'||ch=='N'){
            if(readCamera(str, inputCmdStr)){
                cout<<"Successfully read cube from camera.\n";
                break;
            }else{
                cout<<"[ERROR] Camera input error\n";
                exit(1);
            }
            break;
        }else if(ch=='y' || ch=='Y'){
            break;
        }
    }
    FILE *infp=fopen(outFileName, "r");
    if(infp==NULL){
        cout<<"[ERROR] Cube input file ("<<outFileName<<") could not be found.\n";
        exit(1);
    }
    operations[opCnt++]="IN";
    std::cout<<"Begin cube input..\n";
    char rdch;
    for(int s=0;s<6;s++){
        std::cout<<"Enter for "<<str[s]<<" side:";
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                do{
                    fscanf(infp, "%c", &rdch);
                }while(rdch=='\n' || rdch==' ');
                sides[s].mat[i][j]=rdch;
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
    std::cout<<"..indput done!\nPrinting input cube..\n";
    print();
    if(ch=='y' || ch=='Y'){
        std::cout<<"Position cube such a way that ["<<sides[front].mat[1][1]<<"] color at FRONT and ["<<sides[right].mat[1][1]<<"] color at DOWN";
        getchar();
    }
}

void cube::getOpStr(){
    string cr,nx,crop="";
    int cnt=1;
    bool lock=false;
    operations[opCnt++]="NO";
    for(int i=0;i<opCnt-1;i++){
        cr=operations[i];
        nx=operations[i+1];
        if(cr==nx){
            cnt++;
        }else{
            if(cnt==3){
                if(cr=="RC"){
                    cr="RO";    
                    cnt=1;
                }else if(cr=="RO"){
                    cr="RC";
                    cnt=1;
                }else if(cr=="SC"){
                    cr="SP";
                    cnt=1;
                }else if(cr=="SP"){
                    cr="SC";
                    cnt=1;
                }
            }
            if(cnt%4){
                //if not zero operations
                bool copy=false;
                if(lock && (cr=="FL" || cr=="SP" || cr=="SC")){
                    crop="UL1>";
                    lock=false;
                    copy=true;
                }
                if(!lock && (cr=="RO" || cr=="RC")){
                    crop="LC1>";
                    lock=true;
                    copy=true;
                }
                if(copy){
                    cout<<crop;
                    for(int j=0;j<4;j++){
                        op[opStrCnt][j]=crop[j];
                    }
                    opStrCnt++;
                }
                cout<<crop;
                cnt=cnt%4;
                if(cr=="FL"){
                    while(cnt--){
                        crop=cr+"1>";
                        for(int j=0;j<4;j++){
                            op[opStrCnt][j]=crop[j];
                        }
                        opStrCnt++;
                    }
                }else{
                    crop=cr+(char)(cnt+48)+">";
                    for(int j=0;j<4;j++){
                        op[opStrCnt][j]=crop[j];
                    }
                    opStrCnt++;
                }
                cnt=1;
            }
        }
    }
    opCnt--;
}

void cube::loadLeftSideCorners(){
    leftSideCorners[0][0]=cornerPiece(front, 0, 2);
    leftSideCorners[0][1]=cornerPiece(up, 0, 0);
    leftSideCorners[0][2]=cornerPiece(left, 2, 2);

    leftSideCorners[1][0]=cornerPiece(front, 0, 0);
    leftSideCorners[1][1]=cornerPiece(left, 2, 0);
    leftSideCorners[1][2]=cornerPiece(down, 0, 2);

    leftSideCorners[2][0]=cornerPiece(down, 0, 0);
    leftSideCorners[2][1]=cornerPiece(back, 0, 2);
    leftSideCorners[2][2]=cornerPiece(left, 0, 0);

    leftSideCorners[3][0]=cornerPiece(up, 0, 2);
    leftSideCorners[3][1]=cornerPiece(back, 0, 0);
    leftSideCorners[3][2]=cornerPiece(left, 0, 2);
}

void cube::loadRightSideCorners(){
    rightSideCorners[0][0]=cornerPiece(front, 2, 2);
    rightSideCorners[0][1]=cornerPiece(up, 2, 0);
    rightSideCorners[0][2]=cornerPiece(right, 0, 2);

    rightSideCorners[1][0]=cornerPiece(front, 2, 0);
    rightSideCorners[1][1]=cornerPiece(right, 0, 0);
    rightSideCorners[1][2]=cornerPiece(down, 2, 2);

    rightSideCorners[2][0]=cornerPiece(down, 2, 0);
    rightSideCorners[2][1]=cornerPiece(back, 2, 2);
    rightSideCorners[2][2]=cornerPiece(right, 2, 0);

    rightSideCorners[3][0]=cornerPiece(up, 2, 2);
    rightSideCorners[3][1]=cornerPiece(back, 2, 0);
    rightSideCorners[3][2]=cornerPiece(right, 2, 2);
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
        std::cout<<"============== "<<str<<" side =================\n";
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
    operations[opCnt++]="FL";
    //flip whole cube
    int tmp=left;
    left=up;
    up=right;
    right=down;
    down=tmp;
    rotateSurfaceCW(back);
    rotateSurfaceCCW(front);
    rotateSurfaceCCW(left);
    rotateSurfaceCCW(up);
    rotateSurfaceCCW(right);
    rotateSurfaceCCW(down);
    
}

void cube::rotateSurfaceCW(int side){
    //std::cout<<"[FUNCTION CALL] void cube::rotateSurfaceCW()\n";
    //save upper row
    int saved[3],i;
    for(i=0;i<3;i++)
        saved[i]=sides[side].mat[0][i];
    //up row = left row
    for(i=0;i<3;i++)
        sides[side].mat[0][2-i]=sides[side].mat[i][0];
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
    //std::cout<<"[FUNCTION CALL] void cube::rotateSurfaceCCW()\n";
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
        sides[side].mat[2][2-i]=sides[side].mat[2-i][0];
    //left = saved
    for(i=0;i<3;i++)
        sides[side].mat[i][0]=saved[2-i];
}

void cube::spinCW(){
    std::cout<<"[FUNCTION CALL] void cube::spinCW()\n";
    operations[opCnt++]="SP";
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
    operations[opCnt++]="SC";
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

int cube::getColorSide(char clr){
    //std::cout<<"[FUNCTION CALL] int cube::getColorSide( clr="<< clr <<" )\n";
    for(int s=0;s<6;s++){
        if(sides[s].mat[1][1]==clr){
            //std::cout<<"[FUNCTION RETURN] int( "<< s <<" ) cube::getColorSide( clr="<< clr <<" )\n";
            return s;
        }
    }
    return -1;
}

void cube::setColorToFront(char clr){
    //std::cout<<"[FUNCTION CALL] void cube::setColorToFront( clr="<< clr <<" )\n";
    int clri=getColorSide(clr);
    //std::cout<<"Start Bringing "<<clr<<" to front..\n";
    //std::cout<<"Target Color "<<clr<<" is at ";
    if(clri==front){
        //std::cout<<"front side..\n";
    } else if(clri==right){
        //std::cout<<"right side..\n";
        flip();
        spinCW();
    } else if(clri==back){
        //std::cout<<"back side..\n";
        spinCW();
        spinCW();
    } else if(clri==left){
        //std::cout<<"left side..\n";
        flip();
        spinCCW();
    } else if(clri==up){
        //std::cout<<"up side..\n";
        spinCW();
    } else if(clri==down){
        //std::cout<<"down side..\n";
        spinCCW();
    }
    //std::cout<<"Done bringing "<<clr<<" to front..\n";
}

void cube::setColorToRight(char clr){
    //std::cout<<"[FUNCTION CALL] void cube::setColorToRight( clr="<< clr <<" )\n";
    int clri=getColorSide(clr);
    //std::cout<<"Start Bringing "<<clr<<" to right..\n";
    //std::cout<<"Target Color "<<clr<<" is at ";
    if(clri==front){
        //std::cout<<"front side..\n";
        spinCW();
        flip();
    } else if(clri==right){
        //std::cout<<"right side..\n";
    } else if(clri==back){
        //std::cout<<"back side..\n";
        spinCCW();
        flip();
    } else if(clri==left){
        //std::cout<<"left side..\n";
        flip();
        flip();
    } else if(clri==up){
        //std::cout<<"up side..\n";
        spinCW();
        spinCW();
        flip();
    } else if(clri==down){
        //std::cout<<"down side..\n";
        flip();
    }
    //std::cout<<"Done bringing "<<clr<<" to right..\n";
}

void cube::rotateCCW(){
    std::cout<<"[FUNCTION CALL] void cube::rotateCCW()\n";
    operations[opCnt++]="RC";
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
    rotateSurfaceCW(right);
}

void cube::rotateCW(){
    std::cout<<"[FUNCTION CALL] void cube::rotateCW()\n";
    operations[opCnt++]="RO";
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
    rotateSurfaceCCW(right);
}

void cube::setColorFrontToRight(){
    //std::cout<<"[FUNCTION CALL] void cube::setColorFrontToRight()\n";
    spinCW();
    flip();
}

void cube::setColorRightToFront(){
    //std::cout<<"[FUNCTION CALL] void cube::setColorRightToFront()\n";
    flip();
    spinCW();
}

void cube::rotateByColor(char clr, bool cw){
    setColorToRight(clr);    
    if(cw){
        rotateCW();
    }else{
        rotateCCW();
    }
}

/*int cube::getAdjSide(int fside, int rside, char dir){
    if(fside==front){
        if(rside==right){
            switch(dir){
                case 'b': return back;
                case 'f': return front;
                case 'r': return right;
                case 'l': return left;
                case 'u': return up;
                case 'd': return down;
                default: return -1;
            }
        }else if(rside==left){

        }
    }else if(s==right){
        switch(dir){
            case 'b': return left;
            case 'f': return right;
            case 'r': return back;
            case 'l': return front;
            case 'u': return up;
            case 'd': return down;
            default: return -1;
        }
    }else if(s==left){
        switch(dir){
            case 'b': return right;
            case 'f': return left;
            case 'r': return front;
            case 'l': return back;
            case 'u': return up;
            case 'd': return down;
            default: return -1;
        }
    }else if(s==up){
        switch(dir){
            case 'b': return back;
            case 'f': return up;
            case 'r': return right;
            case 'l': return left;
            case 'u': return up;
            case 'd': return down;
            default: return -1;
        }
    }
    else if(s==down){
        switch(dir){
            case 'b': return up;
            case 'f': return down;
            case 'r': return right;
            case 'l': return left;
            case 'u': return up;
            case 'd': return down;
            default: return -1;
        }
    }else if(s==back){
        switch(dir){
            case 'b': return front;
            case 'f': return back;
            case 'r': return left;
            case 'l': return right;
            case 'u': return up;
            case 'd': return down;
            default: return -1;
        }
    }
    return -1;
}*/

AdjEdgeRes cube::getAdjSideEdge(int s, int i, int j){
    //std::cout<<"[FUNCTION CALL] cube::AdjEdgeRes getAdjSideEdge( "<<s<<" ,"<<i<<" ,"<<j<<" )\n";
    if(s==front){
        if(i==2 && j==1){
            return AdjEdgeRes(right, sides[right].mat[0][1]);
        }
        if(i==1 && j==0){
            return AdjEdgeRes(down, sides[down].mat[1][2]);
        }
        if(i==0&& j==1){
            return AdjEdgeRes(left, sides[left].mat[2][1]);
        }
        if(i==1&& j==2){
            return AdjEdgeRes(up, sides[up].mat[1][0]);
        }
    }else if(s==right){
        if(i==2 && j==1){
            return AdjEdgeRes(back, sides[back].mat[2][1]);
        }
        if(i==1 && j==0){
            return AdjEdgeRes(down, sides[down].mat[2][1]);
        }
        if(i==0&& j==1){
            return AdjEdgeRes(front, sides[front].mat[2][1]);
        }
        if(i==1&& j==2){
            return AdjEdgeRes(up, sides[up].mat[2][1]);
        }
    }else if(s==left){
        if(i==2 && j==1){
            return AdjEdgeRes(front, sides[front].mat[0][1]);
        }
        if(i==1 && j==0){
            return AdjEdgeRes(down, sides[down].mat[0][1]);
        }
        if(i==0&& j==1){
            return AdjEdgeRes(back, sides[back].mat[0][1]);
        }
        if(i==1&& j==2){
            return AdjEdgeRes(up, sides[up].mat[0][1]);
        }
    }else if(s==up){
        if(i==2 && j==1){
            return AdjEdgeRes(right, sides[right].mat[1][2]);
        }
        if(i==1 && j==0){
            return AdjEdgeRes(front, sides[front].mat[1][2]);
        }
        if(i==0&& j==1){
            return AdjEdgeRes(left, sides[left].mat[1][2]);
        }
        if(i==1&& j==2){
            return AdjEdgeRes(back, sides[back].mat[1][0]);
        }
    }else if(s==down){
        if(i==2 && j==1){
            return AdjEdgeRes(right, sides[right].mat[1][0]);
        }
        if(i==1 && j==0){
            return AdjEdgeRes(back, sides[back].mat[1][2]);
        }
        if(i==0&& j==1){
            return AdjEdgeRes(left, sides[left].mat[1][0]);
        }
        if(i==1&& j==2){
            return AdjEdgeRes(front, sides[front].mat[1][0]);
        }
    }else if(s==back){
        if(i==2 && j==1){
            return AdjEdgeRes(right, sides[right].mat[2][1]);
        }
        if(i==1 && j==0){
            return AdjEdgeRes(up, sides[up].mat[1][2]);
        }
        if(i==0&& j==1){
            return AdjEdgeRes(left, sides[left].mat[0][1]);
        }
        if(i==1&& j==2){
            return AdjEdgeRes(down, sides[down].mat[1][0]);
        }
    }
    std::cout<<"[ERROR] getAdjSideEdge: Invalid adjacent side requested..\n";
    return AdjEdgeRes(-1,'\0');
}

bool cube::setRightEdgeToSide(char clrRight, char clrAdj, char clrSide){
    bool done=false;
    int i,j;
    AdjEdgeRes adj;
    if(clrSide==sides[left].mat[1][1]){
        //color present at left side, so can not be placed
        return false;
    }
    for(i=0;i<4;i++){
        if(sides[right].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == clrRight){
            adj=getAdjSideEdge(right, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if(adj.sidei==-1)
                return false;
            if(adj.adjClr == clrAdj){
                break;
            }
        }
    }
    if(i<4){
        for(j=0;j<4 && sides[adj.sidei].mat[1][1]!=clrSide; j++){
            rotateCCW();
            i=(i+1)%4;
            adj=getAdjSideEdge(right, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if(adj.sidei==-1)
                return false;
        }
        if(j<4){
            done=true;
        }
    }
    return done;
}

void cube::solveWhiteCross(){
    //std::cout<<"[FUNCTION CALL] void cube::solveWhiteCross()\n";
    int crossColors[4]={right, up, left, down},i,doneCnt=0;
    //print();
    //CASE 1    
    //first time white present in white cross?
    for(i=0;i<4;i++){
        if(sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == 'w'){
            break;
        }
    }
    if(i<4){
        //yes, white present in white cross
        std::cout<<"[MSG] white present in white cross\n";
        setColorToRight('w');
        for(i=0;i<4;i++){
            if(sides[right].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == 'w'){
                break;
            }
        }
        //align with correct color
        AdjEdgeRes adj=getAdjSideEdge(right, CrossEdgePos[i][0], CrossEdgePos[i][1]);
        setRightEdgeToSide('w', adj.adjClr, adj.adjClr);
        //std::cout<<"[DEBUG] while loop in solveWhiteCross ended!\n";
        std::cout<<"[MSG] "<<adj.adjClr<<" aligned correctly for white cross\n";
        //print();
    }

    //CASE 2
    //white present at white cross?
    while(true){
        doneCnt=0;
        //check white side
        for (i=0; i<4; i++)
        {
            AdjEdgeRes adj = getAdjSideEdge(getColorSide('w'), CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if (sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == 'w')
            {
                if (adj.adjClr == sides[adj.sidei].mat[1][1])
                {
                    doneCnt++;
                    if (doneCnt >= 4)
                        return;
                    continue; 
                }else{
                    //case: white edge not in correct position
                    setColorToRight(sides[adj.sidei].mat[1][1]);
                    setRightEdgeToSide(adj.adjClr, 'w', 'y');
                    setColorToRight('y');
                    setRightEdgeToSide('w', adj.adjClr, adj.adjClr);
                    setColorToRight(adj.adjClr);
                    setRightEdgeToSide(adj.adjClr, 'w', 'w');
                }
            }
            else if (adj.adjClr == 'w')
            {
                char curClr = sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]];
                char adjSideClr = sides[adj.sidei].mat[1][1];
                if(curClr==adjSideClr){
                    //case: white edge not in correct orientation
                    setColorToRight(curClr);
                    setColorToFront('w');
                    //LRDU
                    char rightClr=curClr, upClr=sides[up].mat[1][1], backClr=sides[back].mat[1][1];
                    rotateByColor(rightClr, true);
                    rotateByColor(upClr, false);
                    rotateByColor(backClr, false);
                    rotateByColor(upClr, true);
                    rotateByColor(rightClr, true);
                    rotateByColor(rightClr, true);
                }else{
                    //case: white edge not in correct orientation and position
                    string tstr = "  ";
                    tstr[0] = curClr;
                    tstr[1] = adjSideClr;
                    bool rotateOnly = true;
                    if (tstr == "gb" || tstr == "bg" || tstr == "ro" || tstr == "or"){
                        rotateOnly = false;
                    }
                    if (!rotateOnly){
                        //move edge piece to yellow layer
                        setColorToRight(adjSideClr);
                        rotateCCW();
                        rotateCCW();
                        setColorToRight('y');
                        setRightEdgeToSide(curClr, 'w', curClr);
                        setColorToRight(curClr);
                        setColorToFront('w');
                        //LRDU
                        char upClr = sides[up].mat[1][1];
                        setRightEdgeToSide('w', curClr, upClr);
                        char rightClr = sides[right].mat[1][1];
                        char backClr = sides[back].mat[1][1];
                        rotateByColor(upClr, true);
                        rotateByColor(backClr, true);
                        rotateByColor(upClr, false);
                        rotateByColor(rightClr, true);
                        rotateByColor(rightClr, true);
                    }
                    else{
                        setColorToRight(adjSideClr);
                        setRightEdgeToSide('w', curClr, curClr);
                        setColorToRight(curClr);
                        setRightEdgeToSide(curClr, 'w', 'w');
                    }
                }
            }
        }

        //check yellow layer
        for(i=0; i<4; i++){
            int yside=getColorSide('y');
            AdjEdgeRes adj = getAdjSideEdge(yside, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if (sides[yside].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == 'w'){
                //white in yellow side
                setColorToRight('y');
                setRightEdgeToSide('w', adj.adjClr, adj.adjClr);
                setColorToRight(adj.adjClr);
                setRightEdgeToSide(adj.adjClr, 'w', 'w');
            }else if(adj.adjClr == 'w'){
                char curClr = sides[yside].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]];
                setColorToRight('y');
                //LRDU
                char frontClr=sides[front].mat[1][1], upClr=sides[up].mat[1][1];
                setRightEdgeToSide(curClr, 'w', frontClr);
                rotateByColor(frontClr, true);
                rotateByColor(upClr, true);
                rotateByColor('y', false);
                rotateByColor(upClr, false);
                rotateByColor(frontClr, false);
                //not done yet, will do it in next iteration
            }
        }
        //check remaining up and down sides
        char remainClr[2]={'o', 'r'};
        for(i=0; i<4; i++){
            for(int j=0;j<2;j++){
                int remainClrSide=getColorSide(remainClr[j]);
                char curClr=sides[remainClrSide].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]];
                AdjEdgeRes adj=getAdjSideEdge(remainClrSide, CrossEdgePos[i][0], CrossEdgePos[i][1]);
                if(sides[adj.sidei].mat[1][1]=='b' || sides[adj.sidei].mat[1][1]=='g'){
                    if(curClr=='w'){
                        setColorToRight(sides[adj.sidei].mat[1][1]);
                        setRightEdgeToSide(adj.adjClr, 'w', 'y');
                        rotateByColor('y', true);
                        setColorToRight(sides[adj.sidei].mat[1][1]);
                        setRightEdgeToSide(sides[adj.sidei].mat[1][1], 'w', 'w');
                    }else if(adj.adjClr=='w' && sides[adj.sidei].mat[1][1]!='w'){
                        setColorToRight(remainClr[j]);
                        setRightEdgeToSide(curClr, 'w', 'y');
                        rotateByColor('y', true);
                        setColorToRight(remainClr[j]);
                        setRightEdgeToSide(remainClr[j], 'w', 'w');
                    }
                }
            }
        }

    }//end while(true)
    



    // for(i=0;i<4;i++){
    //     std::cout<<"\ndone edge "<<CrossEdgePos[i][0]<<" , "<<CrossEdgePos[i][1]<<": "<<doneCrossColor[i];
    //     AdjEdgeRes adj=getAdjSideEdge(getColorSide('w'), CrossEdgePos[i][0], CrossEdgePos[i][1]);
    //     std::cout<<"\nedge : "<<sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]]<<" "<<adj.adjClr<<" side: "<<sides[adj.sidei].mat[1][1]<<"\n";
    // }
}

bool cube::moveCornerWhiteToYellow(){
    int doneCnt=0;
    setColorToRight('y');
    char frontClr=sides[front].mat[1][1], upClr=sides[up].mat[1][1], backClr=sides[back].mat[1][1], downClr=sides[down].mat[1][1];
    loadLeftSideCorners();
    //check white present int left white layer?
    int i;
    for(i=0; i<4; i++){
        bool match=false,whitePresent=false;
        for(int j=0;j<3;j++){
            cornerPiece c=leftSideCorners[i][j];
            if(sides[c.sidei].mat[1][1]==sides[c.sidei].mat[c.position[0]][c.position[1]]){
                match=true;
            }else{
                match=false;
                if(sides[c.sidei].mat[c.position[0]][c.position[1]]=='w'){
                    whitePresent=true;
                    break;
                }
            }
        }
        if(match){
            doneCnt++;
            if(doneCnt>=4)
                return true;
            continue;
        }else{
            if(whitePresent){
                //if not matched i.e not correctly placed corner and white present in corner, then move that to yellow layer
                //LRDU
                if(i==0){
                    rotateByColor(upClr, true);
                    rotateByColor('y', false);
                    rotateByColor(upClr, false);
                }else if(i==1){
                    rotateByColor(downClr, false);
                    rotateByColor('y', false);
                    rotateByColor(downClr, true);
                }else if(i==2){
                    rotateByColor(downClr, true);
                    rotateByColor('y', false);
                    rotateByColor(downClr, false);
                }else if(i==3){
                    rotateByColor(upClr, false);
                    rotateByColor('y', false);
                    rotateByColor(upClr, true);
                }
                break;
            }
        }
    }
    return false;
}

bool cube::matchCorner(cornerPiece c[], char x, char y, char z){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                if(i!=j && j!=k && i!=k){
                    if(sides[c[i].sidei].mat[c[i].position[0]][c[i].position[1]]==x)
                        if(sides[c[j].sidei].mat[c[j].position[0]][c[j].position[1]]==y)
                            if(sides[c[k].sidei].mat[c[k].position[0]][c[k].position[1]]==z)
                                return true;
                }
            }
        }
    }
    return false;
}

void cube::solveWhiteCorner(){
    bool done=false;
    while(!done){
        std::cout<<"\n[2]Infinite loop..\n";
        //place white corner piece correctly present in yellow layer
        setColorToRight('y');
        int i;
        bool found=false;
        loadRightSideCorners();
        for(i=0;i<4;i++){
            //cornerPiece *c=rightSideCorners[i];
            cornerPiece c[3];
            for(int j=0;j<3;j++){
                c[j]=rightSideCorners[i][j];
                if(sides[c[j].sidei].mat[c[j].position[0]][c[j].position[1]]=='w'){
                    found=true;
                    break;
                }
            }
            // if(matchCorner(c, sides[front].mat[1][1], sides[up].mat[1][1], 'w')){
            //     found=true;
            //     break;
            // }
            if(found){
                break;
            }
        }
        if(found){ 
            //set corner piece correspond to correct colors
            for(;;i++){
                i=(i%4);
                std::cout<<"\n[1]Infinite loop..\n";
                cornerPiece c[3];
                int k,l;
                char pClr[2], sClr[2];
                for(int j=0,k=0,l=0;j<3;j++){
                    c[j]=rightSideCorners[i][j];
                    if(sides[c[j].sidei].mat[c[j].position[0]][c[j].position[1]]!='w'){
                        pClr[k++]=sides[c[j].sidei].mat[c[j].position[0]][c[j].position[1]];
                    }
                    if(sides[c[j].sidei].mat[1][1]!='y'){
                        sClr[l++]=sides[c[j].sidei].mat[1][1];
                    }
                }
                if((pClr[0]==sClr[0] && pClr[1]==sClr[1]) || (pClr[0]==sClr[1] && pClr[1]==sClr[0])){
                    break;
                }else{
                    rotateCW();
                }
            }
            //set corner piece to front-right-up corner
            while(i--){
                spinCCW();
            }
            char frontClr=sides[front].mat[1][1], upClr=sides[up].mat[1][1];

            if(sides[front].mat[2][2]=='w'){
                //front rule
                rotateByColor(frontClr, false);
                rotateByColor('y', false);
                rotateByColor(frontClr, true);
            }else if(sides[up].mat[2][0]=='w'){
                //right rule [R' D' R]
                rotateByColor(upClr, true);
                rotateByColor('y', true);
                rotateByColor(upClr, false);
            }else if(sides[right].mat[0][2]=='w'){
                //down rule [R' D2 R D R' D' R]
                rotateByColor(upClr, true);
                rotateByColor('y', false);
                rotateByColor('y', false);
                rotateByColor(upClr, false);
                rotateByColor('y', false);
                rotateByColor(upClr, true);
                rotateByColor('y', true);
                rotateByColor(upClr, false);
            }

        }else{
            //no white corner is present in yellow layer, so check if done or bring from white layer
            done=moveCornerWhiteToYellow();
        }
    }
}

bool cube::secondLayerLR(char clr, char sideClr){
    if(clr=='r'){
        if(sideClr=='b') return false;
        if(sideClr=='g') return true;
    }else if(clr=='b'){
        if(sideClr=='o') return false;
        if(sideClr=='r') return true;
    }else if(clr=='g'){
        if(sideClr=='r') return false;
        if(sideClr=='o') return true;
    }else if(clr=='o'){
        if(sideClr=='g') return false;
        if(sideClr=='b') return true;
    }
    cout<<"\n[ERROR] Invalid left or right side requested!\n";
    return true;
}

void cube::secondLayerLeftRight(char curClr, char adjClr, bool lr){
    //LRDU
    rotateByColor('y', !lr);
    rotateByColor(curClr, !lr);
    rotateByColor('y', lr);
    rotateByColor(curClr, lr);
    rotateByColor('y', lr);
    rotateByColor(adjClr, lr);
    rotateByColor('y', !lr);
    rotateByColor(adjClr, !lr);
}

void cube::solveSecondLayer(){
    bool done=false;
    while(!done){
        std::cout<<"\n[solveSecondLayer] Infinite loop..\n";
        int ySide=getColorSide('y');
        int i;
        AdjEdgeRes adj;
        char curClr;
        for(i=0;i<4;i++){
            adj=getAdjSideEdge(ySide, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            curClr=sides[ySide].mat[ CrossEdgePos[i][0]][ CrossEdgePos[i][1]];
            if(curClr!='y' && adj.adjClr!='y' ){
                break;  
            }
        }
        if(i<4){
            //found second layer edge piece in yellow layer
            setColorToRight('y');
            setRightEdgeToSide(curClr, adj.adjClr, adj.adjClr);
            bool lr=secondLayerLR(adj.adjClr, curClr);
            //true: right, false: left
            secondLayerLeftRight(curClr, adj.adjClr, lr);
            continue;
        }else{
            //done or check second layer for wrong edge piece
            setColorToRight('w');
            char layerClrs[4]={'o', 'b', 'r', 'g'};
            for(int c=0, doneCnt=0;c<4;c++){
                int s=getColorSide(layerClrs[c]);
                adj=getAdjSideEdge(s, 1, 2);
                if(layerClrs[c]==sides[s].mat[1][2] && sides[adj.sidei].mat[1][1]==adj.adjClr){
                    doneCnt++;
                    if(doneCnt>=4){
                        done=true;
                        break;
                    }
                }else{
                    secondLayerLeftRight(sides[adj.sidei].mat[1][1], layerClrs[c], true);
                    break;
                }
            }
        }
    }
}

void cube::solveYellowEdgePositionHelper(char rightClr){
    rotateByColor(rightClr, false);
    rotateByColor('y', false);
    rotateByColor(rightClr, true);
    rotateByColor('y', false);
    rotateByColor(rightClr, false);
    rotateByColor('y', false);
    rotateByColor('y', false);
    rotateByColor(rightClr, true);
    rotateByColor('y', false);
}

void cube::solveYellowEdgePosition(){
    bool done=false;
    //initial optimal adjustment
    setColorToRight('y');
    AdjEdgeRes adj;
    int k,max=0,maxCnt=0,matchCnt;
    for(k=0;k<4;k++){
        matchCnt=0;
        for(int i=0;i<4;i++){
            adj=getAdjSideEdge(right, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if(sides[adj.sidei].mat[1][1] == adj.adjClr){
                matchCnt++;
            }
        }
        if(matchCnt==4){
            done=true;
            break;
        }
        if(matchCnt>maxCnt){
            maxCnt=matchCnt;
            max=k;
        }
        rotateCCW();
    }
    if(!done){
        while(max--){
            rotateCCW();
        }
    }

    while(!done){
        std::cout<<"\n[solveYellowEdgePosition] Infinite loop..\n";
        int i,ySide=getColorSide('y');
        for(i=0;i<4;i++){
            adj=getAdjSideEdge(ySide, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if(sides[adj.sidei].mat[1][1] != adj.adjClr){
                break;
            }
        }
        if(i<4){
            //not done yet
            AdjEdgeRes adjFw;
            adjFw=getAdjSideEdge(ySide, CrossEdgePos[(i+1)%4][0], CrossEdgePos[(i+1)%4][1]);
            if(sides[adjFw.sidei].mat[1][1]!=adjFw.adjClr){
                //L type swap forward
                //R U R' U R U2 R' U
                AdjEdgeRes adjRight=getAdjSideEdge(ySide, CrossEdgePos[(i+3)%4][0], CrossEdgePos[(i+3)%4][1]);
                //LRDU
                solveYellowEdgePositionHelper(sides[adjRight.sidei].mat[1][1]);
            }else{
                adjFw=getAdjSideEdge(ySide, CrossEdgePos[(i+2)%4][0], CrossEdgePos[(i+2)%4][1]);
                if(sides[adjFw.sidei].mat[1][1]!=adjFw.adjClr){
                    // - type swap
                    //U (R U R' U R U2 R' U) y2 (R U R' U R U2 R' U)
                    //LRDU
                    rotateByColor('y', false);
                    solveYellowEdgePositionHelper(sides[adjFw.sidei].mat[1][1]);
                    solveYellowEdgePositionHelper(sides[adj.sidei].mat[1][1]);
                }else{
                    adjFw=getAdjSideEdge(ySide, CrossEdgePos[(i+3)%4][0], CrossEdgePos[(i+3)%4][1]);
                    if(sides[adjFw.sidei].mat[1][1]!=adjFw.adjClr){
                        //L type swap backward
                        //R U R' U R U2 R' U
                        AdjEdgeRes adjRight=getAdjSideEdge(ySide, CrossEdgePos[(i+2)%4][0], CrossEdgePos[(i+2)%4][1]);
                        //LRDU
                        solveYellowEdgePositionHelper(sides[adjRight.sidei].mat[1][1]);
                    }
                }
            }
        }else{
            done=true;
            break;
        }
    }
}

void cube::solveYellowCross(){
    bool done=false;
    while(!done){
        std::cout<<"\n[solveYellowCross] Infinite loop..\n";
        //check yellow cross formation
        int i,lShapePos,doneCnt,pos;
        char ch,chn;
        int ySide=getColorSide('y');
        done=true;
        bool lShape=false;
        for(i=0, doneCnt=0;i<4;i++){
            ch=sides[ySide].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]];
            chn=sides[ySide].mat[CrossEdgePos[(i+1)%4][0]][CrossEdgePos[(i+1)%4][1]];
            if(ch!='y'){
                done=false;
                pos=i;
            }else{
                doneCnt++;
                if(doneCnt>=4){
                    done=true;
                    break;
                }
                continue;
                if(chn=='y'){
                    lShape=true;
                    lShapePos=i;
                }
            }
        }
        if(!done){
            if(lShape){
                //L shape
                //LRDU
                int frontIn=getAdjSideEdge(ySide, CrossEdgePos[(lShapePos+2)%4][0], CrossEdgePos[(lShapePos+2)%4][1]).sidei;
                int rightIn=getAdjSideEdge(ySide, CrossEdgePos[(lShapePos+1)%4][0], CrossEdgePos[(lShapePos+1)%4][1]).sidei;
                char frontCLr=sides[frontIn].mat[1][1], rightClr=sides[rightIn].mat[1][1];
                //F U R U' R' F'
                rotateByColor(frontCLr, false);
                rotateByColor('y', false);
                rotateByColor(rightClr, false);
                rotateByColor('y', true);
                rotateByColor(rightClr, true);
                rotateByColor(frontCLr, true);
            }else{
                //Normal
                //F R U R' U' F'
                //LRDU
                int frontIn=getAdjSideEdge(ySide, CrossEdgePos[pos][0], CrossEdgePos[pos][1]).sidei;
                int rightIn=getAdjSideEdge(ySide, CrossEdgePos[(pos==0)?4:pos-1][0], CrossEdgePos[(pos==0)?4:pos-1][1]).sidei;
                char frontCLr=sides[frontIn].mat[1][1], rightClr=sides[rightIn].mat[1][1];
                rotateByColor(frontCLr, false);
                rotateByColor(rightClr, false);
                rotateByColor('y', false);
                rotateByColor(rightClr, true);
                rotateByColor('y', true);
                rotateByColor(frontCLr, true);                
            }
        }
        
    }
    print();
    solveYellowEdgePosition();
}

void cube::positionYellowCorner(){
    bool done=false;
    while(!done){
        int i,matchCnt=0,matchInd=0;
        setColorToRight('y');
        loadRightSideCorners();
        char sideClrs[3]; 
        for(i=0;i<4;i++){
            cornerPiece *c=rightSideCorners[i];
            for(int j=0;j<3;j++){
                sideClrs[j]=sides[c[j].sidei].mat[1][1];
            }
            if(matchCorner(c, sideClrs[0], sideClrs[1], sideClrs[2])){
                matchCnt++;
                matchInd=i;
            }
        }
        if(matchCnt<4){
            //atleast one corner is positioned correctly
            int rcnt=matchInd;
            //set that corner piece to 0th corner
            while(rcnt--){
                spinCCW();
            }
            char rightClr=sides[front].mat[1][1], leftClr=sides[back].mat[1][1];
            //U R U' L' U R' U' L
            //LRDU
            rotateByColor('y', false);
            rotateByColor(rightClr, false);
            rotateByColor('y', true);
            rotateByColor(leftClr, true);
            rotateByColor('y', false);
            rotateByColor(rightClr, true);
            rotateByColor('y', true);
            rotateByColor(leftClr, false);
        }else{
            done=true;
            break;
        }  
    }
}

void cube::orientYellowCorner(){
    bool done=false;
    string correctSeq[4]={"br", "rg", "go", "ob"};
    setColorToRight('y');
    loadRightSideCorners();
    int i;
    bool match;
    for(i=0;i<4;i++){
        cornerPiece *c=rightSideCorners[i];
        match=true;
        for(int j=0;j<3;j++){
            if(sides[c[j].sidei].mat[c[j].position[0]][c[j].position[0]]!=sides[c[j].sidei].mat[1][1]){
                match=false;
                break;
            }
        }
        if(!match){
            break;
        }
    }
    int rcnt=i;
    //set mismatched corner to correct position
    while(rcnt--){
        spinCCW();
    }
    char rightClr=sides[front].mat[1][1];
    rightSideCorners[0];
    while(!done){
        cout<<"\n[orientYellowCorner] Infinite loop..\n";
        bool oriented=false;
        char curCorner[3];
        loadRightSideCorners();
        for(i=0;i<3;i++){
            curCorner[i]=sides[rightSideCorners[0][i].sidei].mat[rightSideCorners[0][i].position[0]][rightSideCorners[0][i].position[1]];
        }
        int k,l;
        while(!oriented){
            // R' D' R D
            //LRDU
            rotateByColor(rightClr, true);
            rotateByColor('w', true);
            rotateByColor(rightClr, false);
            rotateByColor('w', false);
            setColorToRight('y');
            loadRightSideCorners();
            for(k=0;k<4;k++){
                bool found=false;
                cornerPiece *c=rightSideCorners[k];
                if(matchCorner(c, curCorner[0], curCorner[1], curCorner[2])){
                    //found target corner piece, check orientation
                    found=true;
                    for(l=0;l<3;l++){
                        if(sides[c[l].sidei].mat[1][1]=='y'){
                            oriented=(sides[c[l].sidei].mat[c[l].position[0]][c[l].position[1]]=='y');
                            break;
                        }
                    }
                }
                if(found){
                    break;
                }
            }   
        }
        while(k--){
            spinCCW();
        }
        //turn for next piece
        setColorToRight('y');
        loadRightSideCorners();
        //check if done
        bool allYellow=true; //allyellow at yellow layer coreners?
        string cornerSeq[4]={""};
        for(i=0;i<4;i++){
            cornerPiece *c=rightSideCorners[i];
            for(l=0;l<3;l++){
                if(sides[c[l].sidei].mat[1][1]=='y'){
                    if(sides[c[l].sidei].mat[c[l].position[0]][c[l].position[1]]!='y'){
                        allYellow=false;
                        break;
                    } 
                }else{
                    if(sides[c[l].sidei].mat[c[l].position[0]][c[l].position[1]]=='y'){
                        allYellow=false;
                        break;
                    }else{
                        cornerSeq[i]+=sides[c[l].sidei].mat[c[l].position[0]][c[l].position[1]];
                    }
                }
            }
            if(!allYellow){
                break;
            }
        }
        if(allYellow){
            int j;
            //search in corrct seq
            for(j=0;j<4;j++){
                if((correctSeq[j][0]==cornerSeq[0][0] && correctSeq[j][1]==cornerSeq[0][1])||(correctSeq[j][0]==cornerSeq[0][1] && correctSeq[j][1]==cornerSeq[0][0])){
                    break;
                }
            }
            //if not present
            if(j>=4){
                k=0;
            }else{
                //if present, check if corner seq correct 
                for(k=1;k<4;k++){
                    int l=(j+k)%4;
                    if(!(correctSeq[l][0]==cornerSeq[k][0] && correctSeq[l][1]==cornerSeq[k][1]) && !(correctSeq[l][0]==cornerSeq[k][1] && correctSeq[l][1]==cornerSeq[k][0])){
                        break;
                    }
                }
            }
            if(k>=4){
                //if whole seq matched
                for(j=0;j<4;j++){
                    cornerPiece *c=rightSideCorners[0];
                    if(matchCorner(c, sides[c[0].sidei].mat[1][1], sides[c[1].sidei].mat[1][1], sides[c[2].sidei].mat[1][1])){
                        break;
                    }
                    rotateCCW();
                    loadRightSideCorners();
                }
                done=true;
                break;
            }else{
                //wrong corner piece is at k
                i=k;
            }
        }
        while(i--){
            rotateCCW();
        }
    }
}

int main(){
    cube cb;
    serialInit();
    startSerialReceiveThread();
    startSerialReceiveThread();
    getchar(); //garbage collection
    cb.inputCube();
    /*cb.solveWhiteCross();
    cb.operations[cb.opCnt++]="DN1";
    cb.solveWhiteCorner();
    cb.operations[cb.opCnt++]="DN2";
    cb.solveSecondLayer();
    cb.operations[cb.opCnt++]="DN3";
    cb.solveYellowCross();
    cb.operations[cb.opCnt++]="DN4";
    cb.positionYellowCorner();
    cb.operations[cb.opCnt++]="DN5";*/
    cb.orientYellowCorner();
    cb.operations[cb.opCnt++]="DN6";
    
    cb.getOpStr();

    cb.print();
    
    cout<<"Actual Total operations: "<<cb.opCnt<<"\n";
    cout<<"Expanded Total operations: "<<cb.opStrCnt<<"\n";
    string msg[6]={"\nWhite Cross Done\n", "\nWhite Corner Done\n", "\nSecond Layer Done\n", "\nYellow Cross Done\n", "\nYellow Corner Positioned\n", "\nYellow Corner done\n"};
    for(int i=1;i<cb.opStrCnt;i++){
        if(cb.op[i][0]=='D' && cb.op[i][1]=='N'){
            int msgInd=cb.op[i][2]-48;
            cout<<"======================"<<msg[msgInd-1]<<"======================\n";
            getchar();getchar();
            continue;
        }
        startSerialSendThread(cb.op[i]);
        startSerialReceiveThread();
        printf("Sent: %s ---> Performed: ",cb.op[i]);
        getchar(); //wait for user
        //system("clear");
        cout<<"\n----------------------------------------------------------------\n";
    }
    //while(true)
        
}