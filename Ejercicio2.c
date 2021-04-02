#include<time.h>
#include<stdlib.h>

double randFP(double min, double max) {
  double range = (max - min);
  double div = RAND_MAX / range;
  return min + (rand() / div);
}

int main(int argc, char* argv[]){
  time_t t;
  srand((unsigned) time(&t));
}
