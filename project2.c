#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MIXER 2
#define PANTRY 1
#define REFRIGERATOR 2
#define BOWL 3
#define SPOON 5
#define OVEN 1

typedef struct {
    sem_t mixer;
    sem_t pantry;
    sem_t refrigerator;
    sem_t bowl;
    sem_t spoon;
    sem_t oven;
} Kitchen;

typedef struct {
    int id;
    Kitchen *kitchen;
} Baker;

void *baker_thread(void *arg);
void bake(Baker *baker, const char *recipe_name, const char *pantry_items[], int pantry_count, const char *refrigerator_items[], int refrigerator_count);

int main() {
    int num_bakers;
    printf("Enter the number of bakers: ");
    scanf("%d", &num_bakers);

    Kitchen kitchen;
    sem_init(&kitchen.mixer, 0, MIXER);
    sem_init(&kitchen.pantry, 0, PANTRY);
    sem_init(&kitchen.refrigerator, 0, REFRIGERATOR);
    sem_init(&kitchen.bowl, 0, BOWL);
    sem_init(&kitchen.spoon, 0, SPOON);
    sem_init(&kitchen.oven, 0, OVEN);

    pthread_t *threads = malloc(num_bakers * sizeof(pthread_t));
    Baker *bakers = malloc(num_bakers * sizeof(Baker));

    for (int i = 0; i < num_bakers; i++) {
        bakers[i].id = i + 1;
        bakers[i].kitchen = &kitchen;
        pthread_create(&threads[i], NULL, baker_thread, (void *)&bakers[i]);
    }

    for (int i = 0; i < num_bakers; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(bakers);

    return 0;
}

void *baker_thread(void *arg) {
    Baker *baker = (Baker *)arg;

    const char *cookies_pantry[] = {"Flour", "Sugar"};
    const char *cookies_refrigerator[] = {"Milk", "Butter"};
    bake(baker, "Cookies", cookies_pantry, 2, cookies_refrigerator, 2);

    const char *pancakes_pantry[] = {"Flour", "Sugar", "Baking Soda", "Salt"};
    const char *pancakes_refrigerator[] = {"Egg", "Milk", "Butter"};
    bake(baker, "Pancakes", pancakes_pantry, 4, pancakes_refrigerator, 3);

    const char *pizza_dough_pantry[] = {"Yeast", "Sugar", "Salt"};
    bake(baker, "Homemade pizza dough", pizza_dough_pantry, 3, NULL, 0);

    const char *pretzels_pantry[] = {"Flour", "Sugar", "Salt", "Yeast", "Baking Soda"};
    const char *pretzels_refrigerator[] = {"Egg"};
    bake(baker, "Soft Pretzels", pretzels_pantry, 5, pretzels_refrigerator, 1);

    const char *cinnamon_rolls_pantry[] = {"Flour", "Sugar", "Salt", "Cinnamon"};
    const char *cinnamon_rolls_refrigerator[] = {"Egg", "Butter"};
    bake(baker, "Cinnamon rolls", cinnamon_rolls_pantry, 4, cinnamon_rolls_refrigerator, 2);

    printf("Baker %d has finished all recipes.\n", baker->id);
    return NULL;
}

void bake(Baker *baker, const char *recipe_name, const char *pantry_items[], int pantry_count, const char *refrigerator_items[], int refrigerator_count) {
    printf("Baker %d is starting %s.\n", baker->id, recipe_name);

    for (int i = 0; i < pantry_count; i++) {
        sem_wait(&baker->kitchen->pantry);
        printf("Baker %d is taking %s from the pantry.\n", baker->id, pantry_items[i]);
        sem_post(&baker->kitchen->pantry);
    }

    for (int i = 0; i < refrigerator_count; i++) {
        sem_wait(&baker->kitchen->refrigerator);
        printf("Baker %d is taking %s from the refrigerator.\n", baker->id, refrigerator_items[i]);
        sem_post(&baker->kitchen->refrigerator);
    }

    sem_wait(&baker->kitchen->bowl);
    printf("Baker %d is using a bowl.\n", baker->id);
    sem_wait(&baker->kitchen->spoon);
    printf("Baker %d is using a spoon.\n", baker->id);
    sem_wait(&baker->kitchen->mixer);
    printf("Baker %d is using a mixer.\n", baker->id);

    usleep(1000);

    sem_post(&baker->kitchen->mixer);
    sem_post(&baker->kitchen->spoon);
    sem_post(&baker->kitchen->bowl);

    sem_wait(&baker->kitchen->oven);
    printf("Baker %d is using the oven.\n", baker->id);
    usleep(1000); // Simulate baking time
    sem_post(&baker->kitchen->oven);

    printf("Baker %d has finished baking %s.\n", baker->id, recipe_name);
}

