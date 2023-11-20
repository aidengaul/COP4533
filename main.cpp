#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

int ModifiedKnapsack(int n, int W, int l, const std::vector<int> &weights, const std::vector<int> &values) {
    // Initialize 3D vector for tabulation
    // Each dimension is initialized to '_ + 1' to allow for 1-indexing instead of 0-indexing
    std::vector<std::vector<std::vector<int>>> matrix(n + 1, std::vector<std::vector<int>>(W + 1, std::vector<int>(l + 1, 0)));

    // Begin bottom-up tabulation from the i = 1 item and w = 0 weight for k = 0 items
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            for (int k = 1; k <= l; k++) {
                // These are based on the provided relation
                if (i < k || w > 0 && k == 0 || w == 0 && k > 0)
                    matrix[i][w][k] = -1;
                else if (w == 0 && k == 0)
                    matrix[i][w][k] = 0;
                else if (weights[i - 1] > w)
                    matrix[i][w][k] = matrix[i - 1][w][k];
                else
                    matrix[i][w][k] = std::max(matrix[i - 1][w][k], values[i - 1] + matrix[i - 1][w - weights[i - 1]][k - 1]);
            }
        }
    }

    // This is the traceback
    // Initialize an empty vector and start at matrix[n][W][l]
    std::vector<int> traceback;
    int i = n, w = W, k = l;

    // Ensure that the indices are valid
    while (i > 0 && w > 0 && k > 0) {
        // Checks if the ith item was chosen or not and navigates upward in the tabulation matrix accordingly
        if (matrix[i][w][k] != matrix[i - 1][w][k]) {
            traceback.push_back(i - 1);
            w -= weights[i - 1];
            k -= 1;
        }
        i -= 1;
    }

    // Check to see if the solution is valid (meets l requirement)
    if (traceback.empty() || l != traceback.size())
        return -1;
    else {
        std::cout << "Selected items: ";
        for (auto elem : traceback) {
            std::cout << elem;
            if (elem != traceback.back())
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    return matrix[n][W][l];
}

int main() {
    int n, W, l;

    // Get input from user for n, W, l
    std::cout << "Please enter the desired parameters." << std::endl;
    std::cout << "Number of items: " << std::endl;
    std::cin >> n;
    std::cout << "Weight capacity of knapsack: " << std::endl;
    std::cin >> W;
    std::cout << "Target quantity of items: " << std::endl;
    std::cin >> l;

    // Intialize vectors and load them wth random ints
    std::vector<int> values(n);
    std::vector<int> weights(n);
    std::default_random_engine gen;
    std::uniform_int_distribution<int> distValues(1, 50);
    std::uniform_int_distribution<int> distWeights(1, W);

    for (int x = 0; x < n; x++) {
        weights[x] = (distWeights(gen));
        values[x] = (distValues(gen));
    }

    std::cout << std::left << std::setw(20) << "Item" << std::setw(20) << "Value" << std::setw(20) << "Weight" << std::endl;
    std::cout << "=========================================================" << std::endl;

    for (int x = 0; x < n; x++) {
        std::cout << std::left << std::setw(20) << x << std::setw(20) << values[x] << std::setw(20) << weights[x] << std::endl;
    }

    // Get result and output to console
    int result = ModifiedKnapsack(n, W, l, weights, values);

    if (result <= 0)
        std::cout << "No solution available for " << l << " tems subject to a weight limt of " << W << std::endl;
    else
        std::cout << "The maximum value of items contained in the knapsack subject to maximum weight "
            << W << " and target quantity " << l << " is " << result << '.';

    return 0;
}
