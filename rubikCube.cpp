#include<iostream>
#include<string>
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
public:
    int front,back,left,right,up,down;
    string operations[2000];
    int opCnt;
    //int white, blue, yellow, green, orange, red;
    void inputCube();
    void print();
    string getOpStr();
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
    cube(){
        opCnt=0;
    }
};

void cube::inputCube(){
    //std::cout<<"[FUNCTION CALL] void cube::inputCube()\n";
    operations[opCnt++]="IN";
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
    std::cout<<"..indput done!\nPrinting input cube..\n";
    print();
}

string cube::getOpStr(){
    //for()
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
    operations[opCnt++]="SPC";
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
    operations[opCnt++]="SPCC";
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
    operations[opCnt++]="RCC";
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
    operations[opCnt++]="RC";
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

void cube::solveSecondLayer(){
    bool done=false;
    while(!done){
        int ySide=getColorSide('y');
        int i;
        for(i=0;i<4;i++){
            AdjEdgeRes adj=getAdjSideEdge(ySide, CrossEdgePos[i][0], CrossEdgePos[i][1]);
            if(sides[ySide].mat[ CrossEdgePos[i][0]][ CrossEdgePos[i][1]]!='y' || adj.adjClr!='y' ){
                break;  
            }
        }
        if(i<4){
            //found second layer edge piece in yellow layer

        }else{
            //done or check second layer for wrong edge piece
        }
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
    
    cb.solveWhiteCross();
    cb.solveWhiteCorner();
    cb.setColorToFront('w');
    
    cb.print();
    cout<<"Total operations: "<<cb.opCnt;
    int tmp;
    std::cin>>tmp;
}