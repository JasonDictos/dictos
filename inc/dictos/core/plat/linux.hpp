#pragma once

// Dictos uses a default visibilty of extern
#define DPUB __attribute__ ((visibility("default")))
#define DPRV __attribute__ ((visibility("hidden")))

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
