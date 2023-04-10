# cis452project2

## Run the program

```bash
...$ gcc -o project2 project2.c -pthread -lrt
...$ ./project2
```

## Design Document
In designing this program, the main objective was to simulate a shared kitchen environment where multiple bakers, represented by threads, access and use shared resources such as the pantry, refrigerator, and various kitchen tools. The first step was to define the necessary data structures and semaphores to represent the kitchen and bakers. The Kitchen structure was created to hold semaphores for each of the shared resources, while the Baker structure held an ID and a reference to the kitchen it belonged to. Semaphores were used to manage concurrent access to shared resources, ensuring that the specified number of bakers could access each resource simultaneously without conflicts.

Once the data structures and semaphores were set up, the main function was implemented to prompt the user for the number of bakers and initialize the semaphores for the shared resources. It then created an array of Baker structures and each of their pthreads to represent each baker in the kitchen. The baker_thread function simulated the process of baking various recipes. The bake function was created to handle the using and release of the necessary resources for each recipe (ingreidents and tools). Using semaphores allowed us to ensure that resources were accessed and then released in a controlled manner, preventing race conditions and allowing the bakers to work concurrently in the kitchen. 
