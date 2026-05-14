#include <iostream>
#include <omp.h>

using namespace std;

// Function to swap two numbers
void swapNumbers(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Parallel Bubble Sort Function
void parallelBubbleSort(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int first = i % 2;

        #pragma omp parallel for
        for (int j = first; j < n - 1; j += 2)
        {
            if (arr[j] > arr[j + 1])
            {
                swapNumbers(arr[j], arr[j + 1]);
            }
        }
    }
}

int main()
{
    int n;

    cout << "Enter total number of elements: ";
    cin >> n;

    int *arr = new int[n];

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    double start_time = omp_get_wtime();

    parallelBubbleSort(arr, n);

    double end_time = omp_get_wtime();

    cout << "\nSorted Array:\n";

    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }

    cout << "\n\nExecution Time: "
         << (end_time - start_time) * 1000
         << " milliseconds\n";

    delete[] arr;

    return 0;
}