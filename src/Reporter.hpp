#include <vector>

void start_new_report();
void report(int id, int state);
void finish_report();
void report_connections(int id, std::vector<int> neighbors);
