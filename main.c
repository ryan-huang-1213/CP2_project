#include "uchow.h"

Like like;
Backpack backpack;

int main()
{
    //reset
    Becca = 0;
    end = 0;
    wrong = 0;
    line1 = 0;
    for(int32_t i = 0; i < 4; i++)
    {
        like.Becca = 0;
        like.Don = 0;
        like.Police = 0;
        like.Street = 0;
    }
    for(int32_t i = 0; i < 3; i++)
    {
        backpack.airpods = 0;
        backpack.donpaper = 0;
        backpack.picture = 0;
        backpack.nopicture = 0;
    }

    int32_t choice = 0;
    story(choice);


    return 0;
}

