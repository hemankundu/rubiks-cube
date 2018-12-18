#include<iostream>
#include<string>
#define WHITE 0
#define BLUE 1
#define YELLOW 2
#define GREEN 3
#define ORANGE 4
#define RED 5
//using std::cout;
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
    int CrossEdgePos[4][2]={{2,1}, {1,0}, {0,1}, {1,2}};
    void rotateSurfaceCW(int side);
    void rotateSurfaceCCW(int side);
    void setColorFrontToRight();
    void setColorRightToFront();
    int getAdjSide(int s, char side);
    AdjEdgeRes getAdjSideEdge(int s, int i,int j);
    int getColorSide(char clr);
    void solveWhiteCrossPosOri(AdjEdgeRes adj, int edgei);
    bool setRightEdgeToSide(char clrRight, char clrAdj, char clrSide);
    void rotateByColor(int s, bool cw);
    void whiteEdgeAtRightWAdj(char clr);
public:
    int front,back,left,right,up,down;
    int white, blue, yellow, green, orange, red;
    void inputCube();
    void print();
    void flip();
    void spinCW();
    void spinCCW();
    void setColorToFront(char clr);
    void setColorToRight(char clr);
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

int cube::getColorSide(char clr){
    std::cout<<"[FUNCTION CALL] int cube::getColorSide( clr="<< clr <<" )\n";
    for(int s=0;s<6;s++){
        if(sides[s].mat[1][1]==clr){
            std::cout<<"[FUNCTION RETURN] int( "<< s <<" ) cube::getColorSide( clr="<< clr <<" )\n";
            return s;
        }
    }
    return -1;
}

void cube::setColorToFront(char clr){
    std::cout<<"[FUNCTION CALL] void cube::setColorToFront( clr="<< clr <<" )\n";
    int clri=getColorSide(clr);
    std::cout<<"Start Bringing "<<clr<<" to front..\n";
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

void cube::setColorToRight(char clr){
    std::cout<<"[FUNCTION CALL] void cube::setColorToRight( clr="<< clr <<" )\n";
    int clri=getColorSide(clr);
    std::cout<<"Start Bringing "<<clr<<" to right..\n";
    std::cout<<"Target Color "<<clr<<" is at ";
    if(clri==front){
        std::cout<<"front side..\n";
        spinCW();
        flip();
    } else if(clri==right){
        std::cout<<"right side..\n";
    } else if(clri==back){
        std::cout<<"back side..\n";
        spinCCW();
        flip();
    } else if(clri==left){
        std::cout<<"left side..\n";
        flip();
        flip();
    } else if(clri==up){
        std::cout<<"up side..\n";
        spinCW();
        spinCW();
        flip();
    } else if(clri==down){
        std::cout<<"down side..\n";
        flip();
    }
    std::cout<<"Done bringing "<<clr<<" to right..\n";
}

void cube::rotateCCW(){
    std::cout<<"[FUNCTION CALL] void cube::rotateCCW()\n";
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
    std::cout<<"[FUNCTION CALL] void cube::setColorFrontToRight()\n";
    spinCW();
    flip();
}

void cube::setColorRightToFront(){
    std::cout<<"[FUNCTION CALL] void cube::setColorRightToFront()\n";
    flip();
    spinCW();
}

void cube::rotateByColor(int s, bool cw){
    setColorToRight(sides[s].mat[1][1]);    
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
    std::cout<<"[FUNCTION CALL] cube::AdjEdgeRes getAdjSideEdge( "<<s<<" ,"<<i<<" ,"<<j<<" )\n";
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
        }
        if(j<4){
            done=true;
        }
    }
    return done;
}

void cube::solveWhiteCross(){
    std::cout<<"[FUNCTION CALL] void cube::solveWhiteCross()\n";
    setColorToFront('w');
    int crossColors[4]={right, up, left, down},i;
    bool doneCrossColor[4]={false};
    print();
    //CASE 1    
    //first time white present in white cross?
    for(i=0;i<4;i++){
        if(sides[front].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == 'w'){
            break;
        }
    }
    if(i<4){
        //yes, white present in white cross
        std::cout<<"[MSG] white present in white cross\n";
        setColorFrontToRight();
        for(i=0;i<4;i++){
            if(sides[right].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]] == 'w'){
                break;
            }
        }
        //align with correct color
        AdjEdgeRes adj=getAdjSideEdge(right, CrossEdgePos[i][0], CrossEdgePos[i][1]);
        setRightEdgeToSide('w', adj.adjClr, adj.adjClr);
        doneCrossColor[i]=true;
        //std::cout<<"[DEBUG] while loop in solveWhiteCross ended!\n";
        std::cout<<"[MSG] "<<adj.adjClr<<" aligned correctly for white cross\n";
        print();
    }

    //CASE 2
    //white present at white cross?
    for(i=0;i<4;i++){
        AdjEdgeRes adj = getAdjSideEdge(getColorSide('w'), CrossEdgePos[i][0], CrossEdgePos[i][1]);
        if(sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]]=='w'){
            if(adj.adjClr != sides[adj.sidei].mat[1][1]){
                //case: white edge not in correct position
                setColorToRight(sides[adj.sidei].mat[1][1]);
                setRightEdgeToSide(adj.adjClr, 'w', 'y');
                setColorToRight('y');
                setRightEdgeToSide('w', adj.adjClr, adj.adjClr);
                setColorToRight(adj.adjClr);
                setRightEdgeToSide(adj.adjClr, 'w', 'w');
            }
        }else if(adj.adjClr=='w'){
            //case: white edge not in correct orientation and position
            char curClr=sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]];
            setColorToRight(sides[adj.sidei].mat[1][1]);
            bool done=setRightEdgeToSide('w', curClr, curClr);
            if(!done){
                //move edge piece to yello layer
                rotateCCW();
                rotateCCW();
                setColorToRight('y');
                setRightEdgeToSide(curClr, 'w', curClr);

            }
        }
    }

    for(i=0;i<4;i++){
        std::cout<<"\ndone edge "<<CrossEdgePos[i][0]<<" , "<<CrossEdgePos[i][1]<<": "<<doneCrossColor[i];
        AdjEdgeRes adj=getAdjSideEdge(getColorSide('w'), CrossEdgePos[i][0], CrossEdgePos[i][1]);
        std::cout<<"\nedge : "<<sides[getColorSide('w')].mat[CrossEdgePos[i][0]][CrossEdgePos[i][1]]<<" "<<adj.adjClr<<" side: "<<sides[adj.sidei].mat[1][1]<<"\n";
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
    cb.inputCube()
    /*while(true){
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
    }*/
    cb.solveWhiteCross();
    //cb.setColorToFront('w');
    //cb.rotateCCW();
    cb.print();
    int tmp;
    std::cin>>tmp;
}