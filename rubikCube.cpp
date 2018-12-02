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

int front,back,left,right,up,down;
int white, blue, yellow, green, orange, red;

class side{
    public:
    char mat[3][3];
    side(){
        for(short int i=0;i<3;i++)
            for(short int j=0;j<3;j++){
                mat[i][j]='\0';
            }
    }
};
class cube{
    public:
    side sides[6];

    void inputCube(){
        std::cout<<"Begin cube input..\n";
        std::string str[6]={"front", "right", "back", "left", "up", "down"};
        for(int s=0;s<6;s++){
            std::cout<<"Enter for "<<str[s]<<" side:";
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    std::cin>>sides[s].mat[i][j];
                }
            }
        }
        std::cout<<"..indput done!\n";
        print();
    }

    void print(){
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

    void rotateCW(){
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
        //adjust right side
        //save right-up
        for(i=0;i<3;i++)
            upSaved[i]=sides[right].mat[0][i];
        //up=left
        for(i=0;i<3;i++)
            sides[right].mat[0][i]=sides[right].mat[2-i][0];
        //left=down
        for(i=0;i<3;i++)
            sides[right].mat[i][0]=sides[right].mat[2][i];
        //down=right
        for(i=0;i<3;i++)
            sides[right].mat[2][i]=sides[right].mat[2-i][2];
        //right=upSaved
        for(i=0;i<3;i++)
            sides[right].mat[i][2]=upSaved[i];
    }
    
    void rotateCCW(){
        //code this part
        rotateCW();
        rotateCW();
        rotateCW();
    }

    void moveCCW(){
        //rotate whole cube clockwise
        int tmp=right;
        right=front;
        front=left;
        left=back;
        back=tmp;
    }

    void moveCW(){
        //rotate whole cube counter clockwise
        int tmp=left;
        left=front;
        front=right;
        right=back;
        back=tmp;
    }

    void performR(){
        //perform R
        rotateCW();
    }

    void performRI(){
        //perform R'
        rotateCCW();
    }

    void performF(){
        //perform F
        moveCCW();
        rotateCW();
    }

    void performFI(){
        //perform F'
        moveCCW();
        rotateCCW();
    }

    void performL(){
        //perform L
        moveCCW();
        moveCCW();
        rotateCW();
    }

    void performLI(){
        //perform L'
        moveCCW();
        moveCCW();
        rotateCCW();
    }

    void performB(){
        //perform B
        moveCW();
        rotateCW();
    }

    void performBI(){
        //perform B'
        moveCW();
        rotateCCW();
    }


};


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
        std::cout<<"Operation? :";
        std::string op;
        std::cin>>op;

        for(int s=0;s<6;s++){
            char ch=cb.sides[s].mat[2][2];
            if(ch=='w') white=s;
            else if(ch=='g') green=s;
            else if(ch=='y') yellow=s;
            else if(ch=='b') blue=s;
            else if(ch=='r') red=s;
            else if(ch=='o') orange=s;
        }

        if(op=="r")
            cb.performR();
        else if(op=="cr")
            cb.performRI();
        else if(op=="l")
            cb.performL();
        else if(op=="cl")
            cb.performLI();
        else if(op=="f")
            cb.performF();
        else if(op=="cf")
            cb.performFI();
        else if(op=="b")
            cb.performB();
        else if(op=="cb")
            cb.performBI();
        else if(op=="exit")
            break;
        cb.print();
    }
}