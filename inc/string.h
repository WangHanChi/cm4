#pragma once

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int len);
void memset(char *s, int c, int len);
int strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, int len);
void itoa(int x, char str[], int d);
void ftoa(float n, char* res, int afterpoint);
void reverse(char *str);
