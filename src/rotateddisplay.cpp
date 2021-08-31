#include "rotateddisplay.h"

static const char errorMatrix[25] = {
    1,0,0,0,1,
    0,1,0,1,0,
    0,0,1,0,0,
    0,1,0,1,0,
    1,0,0,0,1
};


static const char heartMatrix[25] = {
    0,1,0,1,0,
    1,1,1,1,1,
    1,1,1,1,1,
    0,1,1,1,0,
    0,0,1,0,0
};


static const char numbersZero2Twenty[21][25] = {
    
    {
    0,1,1,1,0,
    0,1,0,1,0,
    0,1,0,1,0,
    0,1,0,1,0,
    0,1,1,1,0
    },
    {
    0,0,1,0,0,
    0,1,1,0,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,1,1,1,0
    },
    {
    0,1,1,1,0,
    0,0,0,1,0,
    0,0,1,0,0,
    0,1,0,0,0,
    0,1,1,1,0
    },
    {
    0,1,1,1,0,
    0,0,0,1,0,
    0,0,1,1,0,
    0,0,0,1,0,
    0,1,1,1,0
    },
    {
    0,1,0,1,0,
    0,1,0,1,0,
    0,1,1,1,0,
    0,0,0,1,0,
    0,0,0,1,0
    },
    { //5
    0,1,1,1,0,
    0,1,0,0,0,
    0,1,1,1,0,
    0,0,0,1,0,
    0,1,1,1,0
    },
    {
    0,1,1,1,0,
    0,1,0,0,0,
    0,1,1,1,0,
    0,1,0,1,0,
    0,1,1,1,0
    },
    {
    0,1,1,1,0,
    0,0,0,1,0,
    0,0,1,0,0,
    0,0,1,0,0,
    0,0,1,0,0
    },
    {
    0,1,1,1,0,
    0,1,0,1,0,
    0,1,1,1,0,
    0,1,0,1,0,
    0,1,1,1,0
    },
    {
    0,1,1,1,0,
    0,1,0,1,0,
    0,1,1,1,0,
    0,0,0,1,0,
    0,1,1,1,0
    },




    //10
    {
    1,0,1,1,1,
    1,0,1,0,1,
    1,0,1,0,1,
    1,0,1,0,1,
    1,0,1,1,1
    },
    {
    0,1,0,0,1,
    1,1,0,1,1,
    0,1,0,0,1,
    0,1,0,0,1,
    0,1,0,0,1
    },
    {
    1,0,1,1,1,
    1,0,0,0,1,
    1,0,0,1,0,
    1,0,1,0,0,
    1,0,1,1,1
    },
    {
    1,0,1,1,1,
    1,0,0,0,1,
    1,0,0,1,1,
    1,0,0,0,1,
    1,0,1,1,1
    },
    {
    1,0,1,0,1,
    1,0,1,0,1,
    1,0,1,1,1,
    1,0,0,0,1,
    1,0,0,0,1
    },
    { //15
    1,0,1,1,1,
    1,0,1,0,0,
    1,0,1,1,1,
    1,0,0,0,1,
    1,0,1,1,1
    },
    {
    1,0,1,1,1,
    1,0,1,0,0,
    1,0,1,1,1,
    1,0,1,0,1,
    1,0,1,1,1
    },
    {
    1,0,1,1,1,
    1,0,0,0,1,
    1,0,0,1,0,
    1,0,0,1,0,
    1,0,0,1,0
    },
    {
    1,0,1,1,1,
    1,0,1,0,1,
    1,0,1,1,1,
    1,0,1,0,1,
    1,0,1,1,1
    },
    {
    1,0,1,1,1,
    1,0,1,0,1,
    1,0,1,1,1,
    1,0,0,0,1,
    1,0,1,1,1
    }

};




void RotatedDisplay::begin(){

    M5.dis.setBrightness(10);

    drawMatrix(heartMatrix, 0xc0ffcb);
    delay(300);

}


void RotatedDisplay::drawNumber(int number, CRGB color){
    if(number > 19){
        M5.dis.fillpix(0x00ff00);
    }
    else{
        drawMatrix(numbersZero2Twenty[number], color);
    }
    
}



void RotatedDisplay::drawMatrix(const char matrix[25], CRGB color){

    for(int i=0;i<25;i++){

        int x = i % 5;
        int y = i / 5;

        int a = 20 - 5 * x + y;

        //CRGB to CGRB

        auto b = (color.r & 0xff0000) >> 2;

        //M5Stack Atom's LED accept GRB order.
        auto c = CRGB(color.g, color.r, color.b);
        M5.dis.drawpix(i, matrix[a] == 1 ? c : CRGB::Black); 
    }

}
