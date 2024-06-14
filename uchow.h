#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void story(int32_t choice);
void END();
void print_out();

typedef struct _backpack
{
    int32_t nopicture;
    int32_t picture;
    int32_t airpods;
    int32_t donpaper;
}Backpack;

typedef struct _like
{
    int32_t Don;
    int32_t Becca;
    int32_t Street;
    int32_t Police;
    int32_t girl;
}Like;

Like like;
Backpack backpack;

int32_t Becca;
int32_t end;
int32_t wrong;
int32_t line1;