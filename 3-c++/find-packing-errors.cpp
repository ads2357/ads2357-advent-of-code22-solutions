#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <bit>

// Constants used for scoring of priorities (see challenge text)
const int unsigned C_PRIO_START_UCASE = 27;
const int unsigned C_PRIO_START_LCASE = 1;

// For a given elf manifest, find which character occurs in both
// halves of the line, and calculate the priority score of that
// character.  @param line - line from the manifest, with the newline
// character already stripped.
static int get_error_priority(std::string line)
{
  // For the sake of interest, use bits to store character presence
  // instead of arrays.  Allows for quick AND operation between
  // vectors, and slightly less memory/cache accesses, using shift/OR
  // operations instead of indirection. Probably not worth it, not
  // scaleable to longer alphabets (without using bit vectors etc.),
  // but fun to do.

  uint64_t half_mask0 = 0;
  uint64_t half_mask1 = 0;

  // record presence of characters in the first and second halves of
  // the string, indexing by character score
  //
  // trivially parallel, SIMD/unrolling possible (reduce at end)
  for (std::size_t ii=0; ii < line.size()/2; ++ii) {
    int prio = line[ii] > 'Z'
      ? line[ii] - 'a' + C_PRIO_START_LCASE
      : line[ii] - 'A' + C_PRIO_START_UCASE
      ;
    half_mask0 |= 1L << prio;
  }

  for (std::size_t ii=line.size()/2; ii < line.size(); ++ii) {
    int prio = line[ii] > 'Z'
      ? line[ii] - 'a' + C_PRIO_START_LCASE
      : line[ii] - 'A' + C_PRIO_START_UCASE
      ;
    half_mask1 |= 1L << prio;
  }

  // find shared characters
  uint64_t error_mask = half_mask0 & half_mask1;

  // recover character score as the index of the shared character
  // (assume only one shared character as per challenge text)
  int prio_idx = std::countr_zero(error_mask);
  return prio_idx > C_PRIO_START_UCASE + ('Z' - 'A')
    ? 0
    : prio_idx;
}

int main(int argc, char *argv[])
{
  std::ifstream infile(argv[1]);
  std::string line;

  int sum = 0;

  while (std::getline(infile, line))
    {
      // threading possible with sum at end, if not already i/o bound
      // (somehow!)
      int prio = get_error_priority(line);
      sum += prio;
    }

  std::cout << sum << std::endl;
}
