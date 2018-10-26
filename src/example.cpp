#include <Serial.h>
#include <URM37.h>

int main()
{
    Serial::init(115200);
    printf("URM37 Example\n");
    URM37::init();

    int16_t dist;
    while(true){
        URM37::trigger();
        do {
            dist = URM37::get_distance();
            if(dist == -2)
                URM37::trigger();
        } while(dist < 0);
        printf("Distance (cm): %d\n", dist);
    }

    return 0;
}