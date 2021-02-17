#pragma once
#include "FIO.h"

int strlength(const char* string) {
    int cnt = 0;

    while (*(string + cnt++) != '\0');

    return cnt;
}

int Find_File(char* Files[100], const char* ext) {
    struct _finddata_t fd;
    long handle;
    int result = 1;
    static int FC_location = 0;
    int Temp = 0;

    handle = _findfirst(ext, &fd);

    if (handle == -1) {
        printf("There were no %s files.\n", ext);
        return 0;
    }

    while (result != -1) {
        Files[FC_location] = (char*)calloc(strlength(fd.name) + 1, sizeof(char));
        strcpy_s(Files[FC_location], _msize(Files[FC_location]), fd.name);
        printf("%s\n", fd.name);
        result = _findnext(handle, &fd);
        FC_location++;
    }

    _findclose(handle);

    return FC_location;
}

void change_ext(char* Name, const char* ext) {
    char* start_point = strchr(Name, '.');
    char* start = start_point;

    while (*start_point) {
        *start_point = 0;
        start_point++;
    }
    
    strcat_s(Name, _msize(Name), ext);
}

void F_Get_Line(FILE* fp, wchar_t** buff) {
    int ch = 0;
    wchar_t* tmp = 0;
    int cur = 0;
    int temp = _msize(*buff);

    while (!feof(fp)) {
        ch = fgetwc(fp);
        (*buff)[cur] = ch;

        if (ch == '\n') {
            tmp = (wchar_t*)calloc(cur + 2, sizeof(wchar_t));
            memcpy(tmp, *buff, (cur + 1) * sizeof(wchar_t));
            *buff = (wchar_t*)calloc(2 + cur, sizeof(wchar_t));
            memcpy(*buff, tmp, (cur + 1) * sizeof(wchar_t));
            (*buff)[cur] = L'\0';
            break;
        }
        else {
            tmp = (wchar_t*)calloc(cur + 2, sizeof(wchar_t));
            memcpy(tmp, *buff, (cur + 1) * sizeof(wchar_t));
            *buff = (wchar_t*)calloc(2 + cur, sizeof(wchar_t));
            memcpy(*buff, tmp, (cur + 1) * sizeof(wchar_t));
        }

        cur++;
        temp = _msize(*buff);
    }

    free(tmp);
}

void Del_Brac_Inside(wchar_t* buff) {
    int cnt = 0;
    char flac = 0, there_are_name = 0;

    wchar_t* name = wcsstr(buff, L"[name]");

    if (name != NULL)
        there_are_name = 1;

    while (buff[cnt]) {
        if ((buff[cnt] == L'[' && &buff[cnt] != name) || (buff[cnt] == L'[' && name == NULL))
            flac = 1;
        else if (buff[cnt] == L']' && name == NULL) {
            buff[cnt] = L'█';
            flac = 0;
        }

        if (flac)
            buff[cnt] = L'█';

        cnt++;
    }
}

void Del_Comment(wchar_t* buff) {
    int cnt = 0;
    char flac = 0;

    while (buff[cnt] != L'\0') {
        if (buff[cnt] == L';') {
            flac = 1;
        }
        else if (buff[cnt] == L'*')
            flac = 1;

        if (flac)
            buff[cnt] = L'█';

        cnt++;
    }
}

int Record_Script_Pos(wchar_t* buff, L_POS* Pos) {
    int cnt = 0, pos_cnt = 0;
    char flac = 0;

    while (buff[cnt]) {
        if (buff[cnt] != L'█' && flac == 0) {
            Pos[pos_cnt].front = cnt;
            flac = 1;
        }
        else if (buff[cnt] == L'█' && flac == 1) {
            Pos[pos_cnt].tale = cnt;
            flac = 0;
            pos_cnt++;
        }

        if (pos_cnt >= 10)
            break;
        cnt++;
    }

    return pos_cnt;
}

int wcslength(const wchar_t* string) {
    int cnt = 0;

    while (*(string + cnt++) != L'\0');

    return cnt;
}

void Del_square(wchar_t* buff) {
    int cnt = 0, Tcnt = 0, i;
    char flac = 0;
    wchar_t* Temp = (wchar_t*)malloc(wcslength(buff) * sizeof(wchar_t));

    while (1) {
        if (buff[cnt] == L'█' && flac == 0) {
            Temp[Tcnt++] = L' ';
            flac = 1;
        }
        else if (buff[cnt] != L'█' && flac == 1)
            flac = 0;

        if (buff[cnt] != L'█')
            Temp[Tcnt++] = buff[cnt];

        if (buff[cnt] == L'\0')
            break;
        cnt++;
    }

    for (i = 0; i < Tcnt; i++)
        buff[i] = Temp[i];

    free(Temp);
}

void Print_Script_Pos(FILE* fp, L_POS* Pos, int pos_cnt) {
    wchar_t Tmp[10] = L"";
    int i;

    fputws(L"[", fp);
    for (i = 0; i < pos_cnt; i++) {
        fputws(L"(", fp);
        swprintf(Tmp, 10, L"%d", Pos[i].front);
        fputws(Tmp, fp);
        fputws(L", ", fp);
        swprintf(Tmp, 10, L"%d", Pos[i].tale);
        fputws(Tmp, fp);
        fputws(L")", fp);
    }
    fputws(L"]", fp);
}

void Write_Script_Pos(FILE* RES_Fp, wchar_t* Dst, wchar_t* Src) {
    L_POS Pos[10] = { 0, };
    int Pos_count = 0;

    Pos_count = Red_Script_Pos(Src, Pos);

    for (int i = 0; i < Pos_count; i++) {
        printf("(%d, %d)", Pos[i].front, Pos[i].tale);
    }
    printf("\n");

}

int Red_Script_Pos(wchar_t* buff, L_POS* Pos) {
    int cnt = 0, Pos_count = 0;
    wchar_t* local_buff = (wchar_t*)calloc(1, _msize(buff));

    swscanf_s(buff + 1, L"%*[[]%[]]", local_buff, _msize(local_buff));

    while (local_buff[cnt] != L'\0') {
        if (local_buff[cnt] == L'(') {
            swscanf_s(buff, L"(%d, %d)", &Pos[Pos_count].front, &Pos[Pos_count].tale);
            Pos_count++;
        }

        cnt++;
    }

    free(local_buff);

    return 0;
}

void Del_Comma(wchar_t* buff) {
    int cnt = 0;
    wchar_t* Fnd = NULL;

    while (buff[cnt]) {
        if (buff[cnt] == L',')
            buff[cnt] = L' ';
        cnt++;
    }

    while (1) {
        Fnd = wcsstr(buff, L" ");

        if (!Fnd)
            break;

        while (*Fnd) {
            *Fnd = *(Fnd + 1);
            Fnd++;
        }
    }
}