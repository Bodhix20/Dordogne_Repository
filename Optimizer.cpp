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
    if (!objectiveFunction) {
        cerr << "Error: Objective function not set!" << endl;
        return {};
    }

    vector<vector<double>> simplex(dim + 1, vector<double>(dim));
    vector<double> f_values(dim + 1);

    // Initialize simplex randomly
    for (int i = 0; i < dim + 1; ++i) {
        for (int j = 0; j < dim; ++j) {
            simplex[i][j] = random_double(lower_bound, upper_bound);
        }
        f_values[i] = objectiveFunction(simplex[i]);
    }

    const double alpha = 1.0, gamma = 2.0, rho = 0.5, sigma = 0.5;
    int iter = 0;

    while (iter < max_iter) {
        // Sort simplex points by function value
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

        // Compute centroid (excluding worst point)
        vector<double> centroid(dim, 0.0);
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                centroid[j] += sorted_simplex[i][j];
            }
        }
        for (int j = 0; j < dim; ++j) {
            centroid[j] /= dim;
        }

        // Reflection
        vector<double> reflected(dim);
        for (int i = 0; i < dim; ++i) {
            reflected[i] = centroid[i] + alpha * (centroid[i] - sorted_simplex[dim][i]);
        }
        double reflected_f = objectiveFunction(reflected);

        if (reflected_f < sorted_f_values[dim - 1]) {
            if (reflected_f < sorted_f_values[0]) {
                // Expansion
                vector<double> expanded(dim);
                for (int i = 0; i < dim; ++i) {
                    expanded[i] = centroid[i] + gamma * (reflected[i] - centroid[i]);
                }
                double expanded_f = objectiveFunction(expanded);
                if (expanded_f < reflected_f) {
                    sorted_simplex[dim] = expanded;
                    sorted_f_values[dim] = expanded_f;
                }
                else {
                    sorted_simplex[dim] = reflected;
                    sorted_f_values[dim] = reflected_f;
                }
            }
            else {
                sorted_simplex[dim] = reflected;
                sorted_f_values[dim] = reflected_f;
            }
        }
        else {
            // Contraction
            vector<double> contracted(dim);
            for (int i = 0; i < dim; ++i) {
                contracted[i] = centroid[i] + rho * (sorted_simplex[dim][i] - centroid[i]);
            }
            double contracted_f = objectiveFunction(contracted);

            if (contracted_f < sorted_f_values[dim]) {
                sorted_simplex[dim] = contracted;
                sorted_f_values[dim] = contracted_f;
            }
            else {
                // Shrink simplex towards best point
                for (int i = 1; i < dim + 1; ++i) {
                    for (int j = 0; j < dim; ++j) {
                        sorted_simplex[i][j] = sorted_simplex[0][j] + sigma * (sorted_simplex[i][j] - sorted_simplex[0][j]);
                    }
                    sorted_f_values[i] = objectiveFunction(sorted_simplex[i]);
                }
            }
        }

        // Check for convergence
        double max_diff = 0.0;
        for (int i = 0; i < dim + 1; ++i) {
            for (int j = 0; j < dim; ++j) {
                max_diff = max(max_diff, fabs(sorted_simplex[i][j] - sorted_simplex[0][j]));
            }
        }

        if (fabs((sorted_f_values[dim] - sorted_f_values[0]) / sorted_f_values[0]) < tol) {
            break;
        }

        simplex = sorted_simplex;
        f_values = sorted_f_values;
        iter++;
    }

    return simplex[0];  // Return best solution
}
