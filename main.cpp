#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  int NUM_CUSTOMERS = 500;
  if (argc > 1) {
    NUM_CUSTOMERS = stoi(argv[1]);
  }
  vector<double> interArrival(NUM_CUSTOMERS);
  vector<double> serviceTime(NUM_CUSTOMERS);
  vector<double> arrivalTime(NUM_CUSTOMERS);
  vector<double> startService(NUM_CUSTOMERS);
  vector<double> departureTime(NUM_CUSTOMERS);
  vector<double> waitingTime(NUM_CUSTOMERS);
  vector<double> timeInSystem(NUM_CUSTOMERS);

  // Random number generation
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<double> distInter(0.5, 3.0);   // Uniform(0.5, 3)
  uniform_real_distribution<double> distService(1.0, 4.0); // Uniform(1, 4)

  // Generate inter-arrival and service times
  for (int i = 0; i < NUM_CUSTOMERS; i++) {
    interArrival[i] = distInter(gen);
    serviceTime[i] = distService(gen);
  }

  // Calculate arrival times
  arrivalTime[0] = interArrival[0];
  for (int i = 1; i < NUM_CUSTOMERS; i++) {
    arrivalTime[i] = arrivalTime[i - 1] + interArrival[i];
  }

  // Single Server Queue Simulation
  double serverFreeTime = 0.0;

  for (int i = 0; i < NUM_CUSTOMERS; i++) {
    // Customer arrives
    double arrival = arrivalTime[i];

    // Service starts when server is free and customer has arrived
    startService[i] = max(arrival, serverFreeTime);

    // Calculate waiting time
    waitingTime[i] = startService[i] - arrival;

    // Departure time
    departureTime[i] = startService[i] + serviceTime[i];

    // Time in system
    timeInSystem[i] = departureTime[i] - arrival;

    // Update server free time
    serverFreeTime = departureTime[i];
  }

  // Calculate Statistics
  double totalWaitingTime = 0.0;
  double totalTimeInSystem = 0.0;
  double maxWaitingTime = 0.0;

  for (int i = 0; i < NUM_CUSTOMERS; i++) {
    totalWaitingTime += waitingTime[i];
    totalTimeInSystem += timeInSystem[i];
    if (waitingTime[i] > maxWaitingTime)
      maxWaitingTime = waitingTime[i];
  }

  double avgWaitingTime = totalWaitingTime / NUM_CUSTOMERS;
  double avgTimeInSystem = totalTimeInSystem / NUM_CUSTOMERS;
  double totalSimulationTime = departureTime[NUM_CUSTOMERS - 1];
  double totalServiceTime = 0.0;

  for (double s : serviceTime)
    totalServiceTime += s;

  double serverUtilization = (totalServiceTime / totalSimulationTime) * 100.0;
  double avgQueueLength = totalWaitingTime / totalSimulationTime;

  // Output Results
  cout << fixed << setprecision(4);
  cout << "================================================================\n";
  cout << "          BANK QUEUE SIMULATION (Single Server)                \n";
  cout
      << "================================================================\n\n";

  cout << "Distributions Used:\n";
  cout << "Inter-arrival time : Uniform(0.5, 3.0)\n";
  cout << "Service time       : Uniform(1.0, 4.0)\n\n";

  cout << "First 10 Customers:\n";
  cout << "----------------------------------------------------------------\n";
  cout << "Cust\tInter-Arr\tArrival\t\tService\t\tWait\t\tDeparture\n";
  cout << "----------------------------------------------------------------\n";

  for (int i = 0; i < 10; i++) {
    cout << (i + 1) << "\t" << interArrival[i] << "\t\t" << arrivalTime[i]
         << "\t\t" << serviceTime[i] << "\t\t" << waitingTime[i] << "\t\t"
         << departureTime[i] << endl;
  }

  cout
      << "\n================================================================\n";
  cout << "                     QUEUE STATISTICS ( " << NUM_CUSTOMERS
       << " Customers )\n";
  cout << "================================================================\n";
  cout << "Average Waiting Time in Queue   : " << avgWaitingTime
       << " minutes\n";
  cout << "Average Time in System          : " << avgTimeInSystem
       << " minutes\n";
  cout << "Maximum Waiting Time            : " << maxWaitingTime
       << " minutes\n";
  cout << "Total Simulation Time           : " << totalSimulationTime
       << " minutes\n";
  cout << "Server Utilization              : " << serverUtilization << " %\n";
  cout << "Average Queue Length (Lq)       : " << avgQueueLength << endl;
  cout << "================================================================\n";

  return 0;
}
