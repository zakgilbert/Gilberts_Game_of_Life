
/************************
	 *  Atlas.c
	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "Atlas.h"
#include "Window_and_Renderer.h"
#include "Header.h"

char** get_alphabet_str()
{
    int i, k;
    char low_c = 97;
    char upr_c = 65;
    char buffer[2];
    char** str = malloc(sizeof(char*) * 63);
    for (i = 0; low_c < '{' && upr_c < ']'; low_c++, upr_c++, i += 2) {
        str[i]     = malloc(1);
        str[i + 1] = malloc(1);
        buffer[0]  = low_c;
        strcpy(str[i], buffer);
        buffer[0] = upr_c;
        strcpy(str[i + 1], buffer);
    }
    fflush(stdout);
    i = 52;
    for (k = 0; k < 10; k++) {
        sprintf(buffer, "%d", k);
        str[i] = malloc(1);
        strcpy(str[i], buffer);
        i++;
    }

    return str;
}
static struct Alpha_Node* new_alpha_node(const char* key, const char* path, SDL_Renderer* renderer)
{
    struct Alpha_Node* item = malloc(sizeof(struct Alpha_Node));
    item->key               = strdup(key);
    item->texture           = create_texture(renderer, path, &item->rect);
    return item;
}
static int hash(const char* letter, int prime, int size)
{
    long hash = 0;
    int i;
    const int len = strlen(letter);
    for (i = 0; i < len; i++) {
        hash += (long)pow(prime, len - (i + 1)) * letter[i];
        hash = hash % size;
    }
    return (int)hash;
}
static int hash_code(const char* letter, const int num, const int attempt)
{
    int hash_a = hash(letter, PRIME_1, num);
    int hash_b = hash(letter, PRIME_2, num);
    return (hash_a + (attempt * (hash_b + 1))) % num;
}
static void _destroy(Atlas* this)
{
    int i;
    if (NULL != this) {
        for (i = 0; i < this->size - 1; i++) {
            if (NULL != this->letters[i]) {
                SDL_DestroyTexture(this->letters[i]->texture);
                free(this->letters[i]->key);
            }
        }

        free(this);
        this = NULL;
    }
}
static void _render(struct Alpha_Node* node, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, node->texture, NULL, &node->rect);
}
static struct Alpha_Node* _search(Atlas* this, const char* key)
{
    int index               = hash_code(key, this->size, 0);
    struct Alpha_Node* item = this->letters[index];
    int i                   = 1;
    while (NULL != item) {
        if (strcmp(item->key, key) == 0) {
            return item;
        }
        index = hash_code(key, this->size, i);
        i++;
    }
    return NULL;
}
static void _insert(Atlas* this, const char* key, SDL_Renderer* renderer, const char* path)
{
    struct Alpha_Node* item     = new_alpha_node(key, path, renderer);
    int index                   = hash_code(item->key, this->size, 0);
    struct Alpha_Node* cur_item = this->letters[index];
    int i                       = 1;
    while (NULL != cur_item) {
        index    = hash_code(item->key, this->size, i);
        cur_item = this->letters[index];
        i++;
    }
    this->letters[index] = item;
    this->count++;
}
static void _map(Atlas* this, SDL_Renderer* renderer)
{
    int i          = 0;
    char** alp_str = get_alphabet_str();
    char* path     = malloc(50);
    strcpy(path, "art/letters/");
    for (i = 0; i < 62; i++) {
        strcpy(path, "art/letters/");
        this->insert(this, alp_str[i], renderer, strcat(path, alp_str[i]));
    }
    strcpy(path, "art/letters/");
    this->insert(this, ":", renderer, strcat(path, ":"));
    strcpy(path, "art/letters/");
    this->insert(this, ".", renderer, strcat(path, "per"));
    strcpy(path, "art/letters/");
    this->insert(this, "/", renderer, strcat(path, "f_slash"));
    strcpy(path, "art/letters/");
    this->insert(this, " ", renderer, strcat(path, "spc4.png"));

    free(path);
}
Atlas* CREATE_ATLAS()
{
    Atlas* this   = malloc(sizeof(*this));
    this->size    = 127;
    this->letters = calloc(this->size, sizeof(struct Alpha_Node*));
    this->insert  = _insert;
    this->destroy = _destroy;
    this->map     = _map;
    this->render  = _render;
    this->search  = _search;
    this->count   = 0;

    return this;
}
