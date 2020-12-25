#include "CodeImputVisitor.hpp"

/***********************
 * Shema transformacije
 * ---------------------
 * if (uslov)
 *   telo;
 * ---------------------
 * noviKod1;
 * if (uslov) {
 *   noviKod2;
 *   telo;
 *   noviKod3;
 * }
 * noviKod4;
 ***********************/

unsigned long long CodeImputVisitor::n = 0;

/* Postavljanje broja umetanja */
void CodeImputVisitor::postaviBroj(unsigned long long i) {
    i = n;
}
