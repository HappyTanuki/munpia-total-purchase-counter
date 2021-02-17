#pragma once
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <io.h>
#include <string.h>
#include <stddef.h>
#include <locale.h>
#include <wchar.h>

typedef struct position {
    int front;
    int tale;
} L_POS;

int Find_File(char* Files[100], const char* ext);
void change_ext(char* Name, const char* ext);
void F_Get_Line(FILE* fp, wchar_t** buff);
void Del_Brac_Inside(wchar_t* buff);
void Del_Comment(wchar_t* buff);
int Record_Script_Pos(wchar_t* buff, L_POS* Pos);
void Del_square(wchar_t* buff);
void Print_Script_Pos(FILE* fp, L_POS* Pos, int pos_cnt);
int wcslength(const wchar_t* string);
int strlength(const char* string);
void Write_Script_Pos(FILE* RES_Fp, wchar_t* Dst, wchar_t* Src);
int Red_Script_Pos(wchar_t* buff, L_POS* Pos);
void Del_Comma(wchar_t* buff);