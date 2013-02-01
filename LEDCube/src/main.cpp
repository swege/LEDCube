#include "main.h"

bool init = false;

int main(void) {
    //TCCR0 |= (1<<CS00);
    //TIMSK |= (1<<TOIE0);
    //TCNT0 = 0;

    //// enable global interrupts
    //sei();

    Cube::LEDCube cube;

    cube.Show();

    int frame = 0;
    int current_animation = 0;
    int animation_counter = 0;
    int (*animations[3]) (Cube::LEDCube &cube) = {
        animation_wireframe,
        animation_tetris,
        animation_rain,
    };
    int animation_delays[3] = {40, 5, 25};
    while(1) {
        frame++;
        frame = frame % animation_delays[current_animation];
        if (!frame) {
            animations[current_animation](cube);
            animation_counter++;
            animation_counter %= 100;
            if (!animation_counter) {
                current_animation++;
                current_animation %= 3;
                init = false;
                cube.Clear();
                cube.Show();
            }
        }
        cube.Draw();
    }
}

int animation_rain(Cube::LEDCube &cube) {
    cube.SetLayer(4, false);
    cube.Shift(0, 1, 0);
    int i = rand() % 25;
    cube.SetPixel(i);
    cube.Show();
    return 0;
}

int animation_tetris(Cube::LEDCube &cube) {
    for (int layer=4; layer>0; layer--) {
        for (int i=0; i<25; i++) {
            if (cube.GetPixel(layer-1, i) && !cube.GetPixel(layer, i)) {
                cube.SetPixel(layer, i);
                cube.SetPixel(layer-1, i, false);
            }
            cube.Draw();
        }
    }
    cube.SetPixel(0, rand() % 25);
    cube.Show();
    return 0;
}

int animation_wireframe(Cube::LEDCube &cube) {
    static int posx;
    static int posy;
    static int posz;
    static int dirx;
    static int diry;
    static int dirz;
    static int counter;
    if (!init) {
        posx = 0;
        posy = 0;
        posz = 0;
        dirx = 1;
        diry = 0;
        dirz = 0;
        counter = 0;
        init = true;
    }
    if (counter < 25) {
        cube.SetPixel(posx, posy, posz);
        cube.SetPixel(4 - posx, 4 - posy, 4 - posz);
    }
    posx += dirx;
    posy += diry;
    posz += dirz;
    if (counter > 27) {
        for (int i=1; i<4; i++) {
            cube.SetPixel((counter % 4), i, 0, false);
            cube.SetPixel((counter + 1) % 4, i, 0, true);

            cube.SetPixel(4 - (counter % 4), i, 4, false);
            cube.SetPixel(4 - ((counter + 1) % 4), i, 4, true);

            cube.SetPixel(4, i, counter % 4, false);
            cube.SetPixel(4, i, (counter + 1) % 4, true);

            cube.SetPixel(0, i, 4 - (counter % 4), false);
            cube.SetPixel(0, i, 4 - ((counter + 1) % 4), true);
        }
    } else if (posx == 0 && posy == 0 && posz == 0) {
        dirx = 0;
        diry = 1;
        dirz = 0;
    } else if (posx == 4 && posy == 0 && posz == 0) {
        dirx = 0;
        diry = 0;
        dirz = 1;
    } else if (posx == 4 && posy == 0 && posz == 4) {
        dirx = -1;
        diry = 0;
        dirz = 0;
    } else if (posx == 0 && posy == 0 && posz == 4) {
        dirx = 0;
        diry = 0;
        dirz = -1;
    } else if (posx == 0 && posy == 4 && posz == 0) {
        dirx = 0;
        diry = -1;
        dirz = 0;
        posx = 4;
        posy = 4;
        posz = 0;
    }
    counter++;
    cube.Show();
    return 0;
}

ISR(TIMER0_OVF_vect) {
}
