FILE *stats_open_file(int run_index) {
	char f[100];
	sprintf(&f[0], "data/stats-%i.csv", run_index);
	return fopen(f, "w");

	free(f);
}

void stats_print_header(FILE *file) {
	fprintf(file, "counter,population size,max simulation length,initial infected,number of neighbors,rewiring probability,max infected,time of max infected,end susceptible,end latent,end infectious,end recovered,end frame\n");
}

void stats_save(igraph_t *graph, FILE *file, int counter, int K, double p, int max_infected, int time_of_max_infected, int end_susceptible, int end_latent, int end_infectious, int end_recovered, int end_frame) {
	fprintf(file, "%i,%i,%i,%i,%i,%f,%i,%i,%i,%i,%i,%i,%i\n",
		counter,
		NETWORK_SIZE,
		SIMULATION_LENGTH,
		NUM_INFECTED,
		K,
		p,
		max_infected,
		time_of_max_infected,
		end_susceptible,
		end_latent,
		end_infectious,
		end_recovered,
		end_frame
	);
}

void stats_close_file(FILE *file) {
	fclose(file);
}

