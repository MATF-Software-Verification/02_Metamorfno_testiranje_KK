#include "Rek2IterVisitor.hpp"

/* Shema transformacije
 * ----------------------------
 * fja(a, ..., b) {
 *   if (uslov) {
 *     telo1;
 *     return baza;
 *   } else {
 *     telo2;
 *     return rek(a0, ..., b0);
 *   }
 * }
 * ----------------------------
 * fja(a, ..., b) {
 *   while (!uslov) {
 *     telo2;
 *     a0 = ...; b0 = ...;
 *     a = a0; b = b0;
 *   }
 *   telo1;
 *   return baza;
 * }
 * */
