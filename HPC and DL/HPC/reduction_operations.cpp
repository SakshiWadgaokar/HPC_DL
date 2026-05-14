#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    int* arr = new int[n];

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    int minimum = arr[0];
    int maximum = arr[0];
    int sum = 0;

    double start_time = omp_get_wtime();

    // Parallel Min
    #pragma omp parallel for reduction(min:minimum)
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < minimum)
        {
            minimum = arr[i];
        }
    }

    // Parallel Max
    #pragma omp parallel for reduction(max:maximum)
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > maximum)
        {
            maximum = arr[i];
        }
    }

    // Parallel Sum
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }

    double average = (double)sum / n;

    double end_time = omp_get_wtime();

    cout << "\nMinimum Value: " << minimum;
    cout << "\nMaximum Value: " << maximum;
    cout << "\nSum: " << sum;
    cout << "\nAverage: " << average;

    cout << "\n\nExecution Time: "
         << (end_time - start_time) * 1000
         << " milliseconds\n";

    delete[] arr;

    return 0;
}