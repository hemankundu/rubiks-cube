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

class side{
    public:
    side *l,*r,*u,*d,*f,*b;

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
};

void cube::inputCube(){
    std::cout<<"Begin cube input..\n";
    std::string str[6]={"front", "right", "back", "left", "up", "down"};
    for(int s=0;s<6;s++){
        std::cout<<"Enter for "<<str[s]<<" side:";
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                std::cin>>sides[s].mat[i][j];
            }
        }
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
    //flip whole cube
    int tmp=left;
    left=up;
    up=right;
    right=down;
    down=tmp;
}

void cube::rotateSurfaceCW(int side){
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
    short int i;
    //save up
    char upSaved[3];
    for(i=0;i<3;i++)
        upSaved[i] = sides[up].mat[i][2];
    //up=front
    for(i=0;i<3;i++)
        sides[up].mat[i][2] = sides[front].mat[i][2];
    //front=down
    for(i=0;i<3;i++)
        sides[front].mat[i][2] = sides[down].mat[i][2];
    //down=back
    for(i=0;i<3;i++)
        sides[down].mat[i][2] = sides[back].mat[i][2];
    //back=upSaved
    for(i=0;i<3;i++)
        sides[back].mat[i][2] = upSaved[i];
    //adjust right side CW
    rotateSurfaceCW(right);
}

void cube::rotateCCW(){
    //code this part
    rotateCW();
    rotateCW();
    rotateCW();
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
        cb.setColorToFront(col);
        cb.print();
    }
}