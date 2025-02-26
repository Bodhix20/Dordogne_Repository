#include "Optimizer.h"

// Random number generator
double random_double(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(min, max);
    return dist(gen);
}

// Constructor definitions
Optimizer::Optimizer() : objectiveFunction(nullptr) {}

Optimizer::Optimizer(func objectiveFunction) : objectiveFunction(objectiveFunction) {}

// PSO algorithm
vector<double> Optimizer::pso(int dim, int swarm_size, int max_iter, double lower_bound, double upper_bound) {
    // Check if the objective function is set
    if (!objectiveFunction) {
        cerr << "Error: Objective function not set!" << endl;
        return {};
    }

    // PSO parameters
    const double w = 0.5;  // Inertia weight
    const double c1 = 1.5; // Personal attraction coefficient
    const double c2 = 1.5; // Social attraction coefficient

    // Particle initialization
    vector<vector<double>> positions(swarm_size, vector<double>(dim));
    vector<vector<double>> velocities(swarm_size, vector<double>(dim, 0.0));
    vector<vector<double>> personal_best_positions = positions;
    vector<double> personal_best_values(swarm_size, numeric_limits<double>::max());
    vector<double> global_best_position(dim);
    double global_best_value = numeric_limits<double>::max();

    // Random initialization of particles
    for (int i = 0; i < swarm_size; ++i) {
        for (int j = 0; j < dim; ++j) {
            positions[i][j] = random_double(lower_bound, upper_bound);
            velocities[i][j] = random_double(-1.0, 1.0);
        }
    }

    // Main PSO loop
    for (int iter = 0; iter < max_iter; ++iter) {
        for (int i = 0; i < swarm_size; ++i) {
            // Compute the objective function value for the particle
            double fitness = objectiveFunction(positions[i]);

            // Update the personal best position
            if (fitness < personal_best_values[i]) {
                personal_best_values[i] = fitness;
                personal_best_positions[i] = positions[i];
            }

            // Update the global best position
            if (fitness < global_best_value) {
                global_best_value = fitness;
                global_best_position = positions[i];
            }
        }

        // Update positions and velocities of the particles
        for (int i = 0; i < swarm_size; ++i) {
            for (int j = 0; j < dim; ++j) {
                double r1 = random_double(0.0, 1.0);
                double r2 = random_double(0.0, 1.0);

                velocities[i][j] = w * velocities[i][j] +
                    c1 * r1 * (personal_best_positions[i][j] - positions[i][j]) +
                    c2 * r2 * (global_best_position[j] - positions[i][j]);

                positions[i][j] += velocities[i][j];

                // Apply boundary constraints (clamping)
                if (positions[i][j] < lower_bound) positions[i][j] = lower_bound;
                if (positions[i][j] > upper_bound) positions[i][j] = upper_bound;
            }
        }

        // Display the current state
        cout << "Iteration " << iter + 1 << ": Best Value = " << global_best_value << endl;
    }

    return global_best_position;
}


vector<double> Optimizer::nelder_mead(int dim, int max_iter, double tol, double lower_bound, double upper_bound) {
    // Ojective function check
    if (!objectiveFunction) {
        cerr << "Error: Objective function not set!" << endl;
        return {};
    }

    // Simplex initialization
    vector<vector<double>> simplex(dim + 1, vector<double>(dim));
    vector<double> f_values(dim + 1);

    // Initialization of simplex points (random)
    for (int i = 0; i < dim + 1; ++i) {
        for (int j = 0; j < dim; ++j) {
            simplex[i][j] = random_double(lower_bound, upper_bound);
        }
    }

    // Evaluation of objective function for each submits of the simplex
    for (int i = 0; i < dim + 1; ++i) {
        f_values[i] = objectiveFunction(simplex[i]);
    }

    int iter = 0;
    while (iter < max_iter) {
        // Order
        vector<int> order(dim + 1);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) {
            return f_values[i] < f_values[j];
            });

        vector<vector<double>> sorted_simplex(dim + 1);
        vector<double> sorted_f_values(dim + 1);
        for (int i = 0; i < dim + 1; ++i) {
            sorted_simplex[i] = simplex[order[i]];
            sorted_f_values[i] = f_values[order[i]];
        }

        // 
        vector<double> centroid(dim, 0.0);
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                centroid[j] += sorted_simplex[i][j];
            }
        }
        for (int j = 0; j < dim; ++j) {
            centroid[j] /= dim;
        }

        //  (Reflexion) 
        vector<double> reflected_point(dim);
        for (int i = 0; i < dim; ++i) {
            reflected_point[i] = centroid[i] + (centroid[i] - sorted_simplex[dim][i]);
        }

        double reflected_f_value = objectiveFunction(reflected_point);

        if (reflected_f_value < sorted_f_values[dim - 1]) {
            if (reflected_f_value < sorted_f_values[0]) {
                vector<double> expanded_point(dim);
                for (int i = 0; i < dim; ++i) {
                    expanded_point[i] = centroid[i] + 2 * (centroid[i] - sorted_simplex[dim][i]);
                }
                double expanded_f_value = objectiveFunction(expanded_point);
                if (expanded_f_value < reflected_f_value) {
                    sorted_simplex[dim] = expanded_point;
                    f_values[dim] = expanded_f_value;
                }
                else {
                    sorted_simplex[dim] = reflected_point;
                    f_values[dim] = reflected_f_value;
                }
            }
            else {
                sorted_simplex[dim] = reflected_point;
                f_values[dim] = reflected_f_value;
            }
        }
        else {
            vector<double> contracted_point(dim);
            for (int i = 0; i < dim; ++i) {
                contracted_point[i] = centroid[i] + 0.5 * (sorted_simplex[dim][i] - centroid[i]);
            }
            double contracted_f_value = objectiveFunction(contracted_point);
            if (contracted_f_value < f_values[dim]) {
                sorted_simplex[dim] = contracted_point;
                f_values[dim] = contracted_f_value;
            }
            else {
                for (int i = 1; i < dim + 1; ++i) {
                    for (int j = 0; j < dim; ++j) {
                        sorted_simplex[i][j] = sorted_simplex[0][j] + 0.5 * (sorted_simplex[i][j] - sorted_simplex[0][j]);
                    }
                    f_values[i] = objectiveFunction(sorted_simplex[i]);
                }
            }
        }


        double max_diff = 0.0;
        for (int i = 0; i < dim + 1; ++i) {
            for (int j = 0; j < dim; ++j) {
                max_diff = max(max_diff, fabs(sorted_simplex[i][j] - sorted_simplex[0][j]));
            }
        }

        if (max_diff < tol) {
            break;
        }

        iter++;
    }

    return simplex[0];
}