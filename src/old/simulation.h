#ifndef SIMULATION_HEADER_GUARD
#define SIMULATION_HEADER_GUARD

struct simulation_parameters {
	int k_low, k_high, k_step;
	double p_low, p_high, p_step;
	int repititions;
};
typedef struct simulation_parameters simulation_parameters;

struct thread {
	int index;
	pthread_t thread;
	simulation_parameters parameters;
};
typedef struct thread thread;

void simulation(FILE *, double, int, int);
void test_parameters(FILE *, struct simulation_parameters);

#endif
