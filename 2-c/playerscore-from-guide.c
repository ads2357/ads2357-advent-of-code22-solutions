#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static const size_t C_CHARS_PER_ROUND = 3;
static const size_t C_CHARS_PER_LINE  = C_CHARS_PER_ROUND+1; // newline

// Return score for win/lose/draw, encoded by outcome.
//
// @parameter outcome - range [-2,2]. Encodes loss if congruent to
// 2 (modulo 3), win if 1, draw if 0.
inline static uint_fast8_t outcome_to_score(int_fast8_t outcome) {
  return (outcome == 0
          ? 3 // draw
          : (outcome == 1
             ? 6 // win
             : (outcome == -2
                ? 6 // win
                : 0
                )
             )
          );
}

// Calculate points for round described by ASCII line.  Takes
// untrusted elf input, but does no computed memory address access or
// other unsafety.
inline static uint_fast8_t score_round(char *line) {
#ifdef PART1
  char enc_opmove = line[0];
  char enc_mymove = line[2];

  // X-rock:1, Y-paper:2, Z-scissors:3
  int_fast8_t shape_score = enc_mymove - 'X' + 1;

  // win/loss determined by mod3(opp - my); range [-2,2]; loss contributes nothing.
  //    R0      // opp to my: clockwise (+1) = win
  // S2    P1                anticlockwise (-1 === +2) = lose
  //
  // outcome = my - opp
  int_fast8_t diff_score;
  int_fast8_t diff = enc_mymove - enc_opmove;
  diff_score = outcome_to_score(diff - ('X' - 'A')); // encoding offset
  return shape_score + diff_score;
#else
  char enc_opmove = line[0];
  char enc_outcome = line[2];

  // Calcuate my move:
  // outcome = my - opp (mod 3) (see above)
  // my = outcome + opp (mod 3)

  // outcome encoding, according to challenge description, is:
  // X = lose = anticlockwise (desire a +2)
  // Y = draw = nop           (desire a  0)
  // Z = win  = clockwise     (desire a +1)

  const int_fast8_t C_DECODE_OUTCOME = -1; // add this to convert from 0=lose to -1=lose
  // 1=draw to 0=draw, 2=win to 1=win as in part 1.

  int_fast8_t dec_outcome = enc_outcome - 'X' + C_DECODE_OUTCOME;
  // compute sum after decoding to standard R=0, P=1, S=2, as per the
  // above equation, to obtain player move.
  int_fast8_t sum = dec_outcome + (enc_opmove - 'A');
  // modulo 3 (down to [-2,2]) and add 1
  // (consider refactoring)
  int_fast8_t myscore = sum + ((sum < 0) ? 4 : sum == 3 ? -2 : 1);
  return myscore + outcome_to_score(enc_outcome - 'X' + C_DECODE_OUTCOME);
#endif
}

// Calculates and sums scores from the round descriptions in the file.
// Only supports UNIX newlines. No input validation.
static uint_fast32_t score_rounds(FILE *in_f_han) {
  char buf[C_CHARS_PER_LINE + 1]; // include terminator, for caution; not currently used.
  uint_fast32_t total_score = 0;

  buf[C_CHARS_PER_LINE] = '\0';

  size_t bytes_read;
  while ((bytes_read = fread(buf, 1, C_CHARS_PER_LINE, in_f_han)) == C_CHARS_PER_LINE) {
    total_score += score_round(buf);
  }

  if (feof(in_f_han) && (buf[0] != '\n' || bytes_read != 1) && bytes_read != 0) {
    fprintf(stderr, "Error reading input: unexpected end of file. `%s`, %ld\n", buf, bytes_read);
    exit(EXIT_FAILURE);
  } else if (ferror(in_f_han)) {
    perror("Error reading input.");
    exit(EXIT_FAILURE);
  }

  return total_score;
}

static const char * C_USAGE = "USAGE: playerscore-from-guide [input_file]";

int main(int argc, char *argv[]) {
  FILE *in_f_han;
  uint_fast32_t score;

  if (argc < 1) {
    fprintf(stderr, "%s\n", C_USAGE);
    return EXIT_FAILURE;
  }

  in_f_han = fopen(argv[1], "r");
  if (!in_f_han) {
    perror("");
    return EXIT_FAILURE;
  }

  score = score_rounds(in_f_han);

  printf("%ld\n", score);

  return 0;
}
