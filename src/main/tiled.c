#include "../common/matrix.h"
#include "../utils/utils.h"

int main(int argc, char *argv[]) {

  srand(SEED);

  int matrix_size, thread_count, chunk_size;

  get_args(argc, argv, &matrix_size, &thread_count, &chunk_size);

  return 0;
}