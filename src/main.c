#include <ecs_prometheus_exporter.h>

typedef struct Vector {
    float x;
    float y;
} Vector;

typedef Vector Position;
typedef Vector Velocity;

typedef struct Damage {
    float value;
} Damage;

typedef struct Health {
    float value;
} Health;

/* Simple move system */
void Move(ecs_rows_t *rows) {
    ECS_COLUMN(rows, Position, p, 1);
    ECS_COLUMN(rows, Velocity, v, 2);

    for (int i = 0; i < rows->count; i++) {
        p[i].x += v[i].x;
        p[i].y += v[i].y;
    }
}

/* A dummy system that stages some data */
void AddDamage(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, Damage, 2);

    for (int i = 0; i < rows->count; i++) {
        if ((float)rand() / (float)RAND_MAX > 0.5) {
            ecs_set(rows->world, rows->entities[i], Damage, {10});
        }
    }
}

/* A dummy system that removes some staged data */
void ProcessDamage(ecs_rows_t *rows) {
    ECS_COLUMN(rows, Health, health, 1);
    ECS_COLUMN(rows, Damage, damage, 2);
    ECS_COLUMN_COMPONENT(rows, Damage, 2);

    for (int i = 0; i < rows->count; i++) {
        health[i].value -= damage[i].value;
        ecs_remove(rows->world, rows->entities[i], Damage);
    }  
}

int main(int argc, char *argv[]) {
    /* Create the world, pass arguments for overriding the number of threads,fps
     * or for starting the admin dashboard (see flecs.h for details). */
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import modules. Both the Prometheus and Admin module import 
     * the Flecs Http module, which for this application is implemented by
     * importing Civetweb */
    ECS_IMPORT(world, FlecsSystemsCivetweb, 0);
    ECS_IMPORT(world, FlecsSystemsPrometheus, 0);

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, Health);
    ECS_COMPONENT(world, Damage);
    ECS_TYPE(world, MyEntityType, Position, Velocity, Health);

    /* Create dummy systems that don't do anything useful but generating load.
     * The systems do some operations that rely on Flecs staging, which is done
     * to generate more interesting metrics */
    ECS_SYSTEM(world, Move, EcsOnUpdate, Position, Velocity);
    ECS_SYSTEM(world, AddDamage, EcsOnUpdate, Health, !Damage);
    ECS_SYSTEM(world, ProcessDamage, EcsOnUpdate, Health, Damage);

    /* Create the Prometheus exporter endpoint */
    ecs_set(world, 0, EcsPrometheus, {.port = 9300});

    /* Set target FPS for main loop */
    ecs_set_target_fps(world, 60);
    
    /* Create 10000 entities */
    ecs_new_w_count(world, MyEntityType, 1000);

    printf("Application ecs_helloworld is running, press CTRL-C to exit...\n");

    /* Run systems */
    while (true) { ecs_progress(world, 0); }

    /* Cleanup */
    return ecs_fini(world);
}
