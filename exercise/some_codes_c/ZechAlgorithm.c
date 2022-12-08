#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

int Zech_3[] = {3, 6, 1, 5, 4, 2};
int Zech_4[] = {4, 8, 14, 1, 10, 13, 9, 2, 7, 5, 12, 11, 6, 3};
int Zech_5[] = {18, 5, 29, 10, 2, 27, 22, 20, 16, 4, 19, 23, 14, 13, 24, 9, 30, 1, 11, 8, 25, 7, 12, 15, 21, 28, 6, 26, 3, 17};
int Zech_6[] = {56, 49, 13, 35, 30, 26, 8, 7, 27, 60, 23, 52, 3, 16, 34, 14, 39, 54, 48, 57, 42, 46, 11, 41, 58, 6, 9, 32, 44, 5, 59, 28, 38, 15, 4, 45, 43, 33, 17, 51, 24, 21, 37, 29, 36, 22, 61, 19, 2, 53, 40, 12, 50, 18, 62, 1, 20, 25, 31, 10, 47, 55};
int Zech_7[] = {7, 14, 63, 28, 54, 126, 1, 56, 90, 108, 87, 125, 55, 2, 31, 112, 43, 53, 29, 89, 57, 47, 82, 123, 105, 110, 66, 4, 19, 62, 15, 97, 77, 86, 109, 106, 46, 58, 100, 51, 75, 114, 17, 94, 68, 37, 22, 119, 122, 83, 40, 93, 18, 5, 13, 8, 21, 38, 104, 124, 88, 30, 3, 67, 95, 27, 64, 45, 107, 91, 79, 85, 78, 92, 41, 116, 33, 73, 71, 102, 118, 23, 50, 101, 72, 34, 11, 61, 20, 9, 70, 74, 52, 44, 65, 111, 32, 117, 103, 39, 84, 80, 99, 59, 25, 36, 69, 10, 35, 26, 96, 16, 115, 42, 113, 76, 98, 81, 48, 121, 120, 49, 24, 60, 12, 6};
int Zech_8[] = {25, 50, 223, 100, 138, 191, 112, 200, 120, 21, 245, 127, 99, 224, 33, 145, 68, 240, 92, 42, 10, 235, 196, 254, 1, 198, 104, 193, 181, 66, 45, 35, 15, 136, 32, 225, 179, 184, 106, 84, 157, 20, 121, 215, 31, 137, 101, 253, 197, 2, 238, 141, 147, 208, 63, 131, 83, 107, 82, 132, 186, 90, 55, 70, 162, 30, 216, 17, 130, 64, 109, 195, 236, 103, 199, 113, 228, 212, 174, 168, 160, 59, 57, 40, 170, 242, 167, 175, 203, 62, 209, 19, 158, 202, 176, 251, 190, 139, 13, 4, 47, 221, 74, 27, 248, 39, 58, 161, 71, 126, 246, 7, 76, 166, 243, 214, 122, 164, 153, 9, 43, 117, 183, 180, 194, 110, 12, 140, 239, 69, 56, 60, 250, 177, 144, 34, 46, 5, 98, 128, 52, 218, 150, 135, 16, 217, 53, 206, 188, 143, 178, 226, 119, 201, 159, 169, 41, 93, 155, 81, 108, 65, 182, 118, 227, 114, 87, 80, 156, 85, 211, 229, 232, 79, 88, 95, 134, 151, 37, 124, 29, 163, 123, 38, 249, 61, 204, 149, 219, 97, 6, 247, 28, 125, 72, 23, 49, 26, 75, 8, 154, 94, 89, 187, 207, 148, 205, 54, 91, 241, 171, 78, 233, 116, 44, 67, 146, 142, 189, 252, 102, 237, 3, 14, 36, 152, 165, 77, 172, 231, 230, 173, 213, 244, 22, 73, 222, 51, 129, 18, 210, 86, 115, 234, 11, 111, 192, 105, 185, 133, 96, 220, 48, 24};
int Zech_9[] = {130, 260, 420, 9, 507, 329, 67, 18, 4, 503, 470, 147, 138, 134, 126, 36, 341, 8, 421, 495, 306, 429, 337, 294, 56, 276, 142, 268, 463, 252, 486, 72, 181, 171, 231, 16, 375, 331, 214, 479, 52, 101, 370, 347, 310, 163, 301, 77, 119, 112, 454, 41, 225, 284, 408, 25, 108, 415, 469, 504, 167, 461, 263, 144, 256, 362, 7, 342, 262, 462, 269, 32, 219, 239, 435, 151, 48, 428, 307, 447, 379, 104, 161, 202, 349, 229, 354, 183, 414, 109, 94, 326, 304, 91, 423, 154, 186, 238, 220, 224, 42, 397, 158, 82, 384, 450, 275, 57, 90, 305, 496, 50, 358, 216, 484, 319, 465, 427, 49, 497, 412, 334, 356, 411, 498, 15, 232, 288, 510, 1, 290, 213, 332, 14, 499, 173, 477, 13, 333, 413, 184, 27, 425, 64, 321, 438, 12, 478, 215, 359, 76, 302, 266, 96, 278, 345, 244, 103, 380, 383, 83, 247, 46, 208, 437, 322, 61, 404, 237, 187, 34, 458, 136, 197, 472, 366, 299, 317, 312, 218, 33, 188, 88, 141, 277, 97, 170, 182, 355, 335, 446, 308, 442, 372, 401, 476, 174, 440, 378, 448, 246, 84, 395, 283, 226, 316, 300, 164, 361, 257, 418, 389, 132, 39, 149, 114, 241, 180, 73, 99, 352, 481, 351, 100, 53, 205, 281, 432, 86, 457, 35, 127, 388, 419, 261, 343, 169, 98, 74, 483, 217, 313, 271, 157, 398, 201, 162, 311, 318, 485, 253, 30, 251, 464, 320, 65, 210, 509, 289, 2, 235, 69, 63, 426, 466, 153, 424, 28, 71, 487, 243, 346, 371, 443, 107, 26, 185, 155, 406, 315, 227, 368, 204, 54, 490, 339, 387, 128, 259, 131, 390, 365, 473, 24, 409, 445, 336, 430, 177, 207, 47, 152, 467, 93, 110, 21, 79, 192, 393, 45, 248, 179, 242, 488, 280, 206, 178, 249, 116, 255, 145, 166, 505, 494, 422, 92, 468, 416, 6, 363, 38, 133, 139, 122, 190, 297, 23, 474, 286, 374, 17, 68, 236, 405, 156, 272, 44, 394, 85, 433, 223, 221, 456, 87, 189, 123, 453, 113, 150, 436, 209, 66, 330, 376, 292, 176, 431, 282, 396, 43, 273, 194, 386, 340, 37, 364, 391, 199, 81, 159, 382, 381, 160, 105, 400, 373, 287, 233, 212, 291, 377, 441, 309, 348, 203, 369, 102, 245, 449, 385, 195, 492, 460, 168, 344, 279, 489, 55, 295, 452, 124, 121, 140, 89, 58, 328, 508, 211, 234, 3, 19, 325, 95, 267, 143, 264, 118, 78, 22, 298, 367, 228, 350, 482, 75, 360, 165, 146, 471, 198, 392, 193, 274, 451, 296, 191, 80, 200, 399, 106, 444, 410, 357, 51, 480, 353, 230, 172, 500, 403, 62, 70, 29, 254, 117, 265, 303, 327, 59, 11, 439, 175, 293, 338, 491, 196, 137, 148, 40, 455, 222, 434, 240, 115, 250, 31, 270, 314, 407, 285, 475, 402, 501, 324, 20, 111, 120, 125, 135, 459, 493, 506, 10, 60, 323, 502, 5, 417, 258, 129};

typedef int FFsize;
typedef int FFexponent;

/*Zech's Algorithm, accelerate calculation of two elements of finite field GF(2,n), 
  with lookup table. 
  1+a^x = a^{Zech(x)}, where a is a primitive element of GF(2,n).  
  */
FFexponent Zech_ALgorithm(FFexponent x,FFsize n){
    if (x == 0)
        return 0;
    if (n == 3)
        return Zech_3[x-1];
    if (n == 4)
        return Zech_4[x-1];
    if (n == 5)
        return Zech_5[x-1];
    if (n == 6)
        return Zech_6[x-1];
    if (n == 7)
        return Zech_7[x-1];
    if (n == 8)
        return Zech_8[x-1];
    if (n == 9)
        return Zech_9[x-1];
}

// it returns the exponent of addition of a^x+a^y=a^x*(1+a^(y-x)) if y > x in finite field GF(2,n)
// note that it returns 0 if x equals y
FFexponent FF_add(FFexponent x,FFexponent y,FFsize n){
    if (x == y)
        return 0;
    if (x == 0)
        return Zech_ALgorithm(y,n);
    if (y == 0)
        return Zech_ALgorithm(x,n);
    if (x*y != 0)
    {
        int max = x > y ? x : y;
        int min = x > y ? y : x;
        return min + Zech_ALgorithm(max-min,n);
    }
}

FFexponent FF_exp(FFexponent x,int q,FFsize n){
    return x*q % ((1<<n) - 1);
}

int main(){
    printf("%d",FF_add(2,3,3));
}