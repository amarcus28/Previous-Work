#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <immintrin.h>
#include <smmintrin.h>

/* 
 * Please fill in the following team struct 
 */
who_t who = {
    "aem2nj",           /* Scoreboard name */

    "Adam Marcus",      /* First member full name */
    "aem2nj@virginia.edu",     /* First member email address */
};

/*** UTILITY FUNCTIONS ***/

/* You are free to use these utility functions, or write your own versions
 * of them. */

/* A struct used to compute averaged pixel value */
typedef struct {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short alpha;
    unsigned short num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = sum->alpha = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->alpha += (int) p.alpha;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    current_pixel->alpha = (unsigned short) (sum.alpha/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	for(int ii=max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	    accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
 
    return current_pixel;
}



/******************************************************
 * Your different versions of the smooth go here
 ******************************************************/

/* 
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    for (int i = 0; i < dim; i++)
	for (int j = 0; j < dim; j++)
            dst[RIDX(i,j, dim)] = avg(dim, i, j, src);
}
/* 
 * smooth - Your current working version of smooth
 *          Our supplied version simply calls naive_smooth
 */
char another_smooth_descr[] = "another_smooth: Another version of smooth";
void another_smooth(int dim, pixel *src, pixel *dst) 
{
    naive_smooth(dim, src, dst);
}

char my_smooth_checkpoint_descr[] = "my_smooth_checkpoint: ";
void my_smooth_checkpoint(int dim, pixel *src, pixel *dst) 
{
  // MIDDLE PIXELS
  for (int i = 1; i <= dim-2; i++){
    for (int j = 1; j <= dim-2; j++){
      pixel_sum sum;
      pixel current_pixel;

      initialize_pixel_sum(&sum);
      accumulate_sum(&sum, src[RIDX(i-1,j-1,dim)]);
      accumulate_sum(&sum, src[RIDX(i-1,j,dim)]);
      accumulate_sum(&sum, src[RIDX(i-1,j+1,dim)]);
      accumulate_sum(&sum, src[RIDX(i,j-1,dim)]);
      accumulate_sum(&sum, src[RIDX(i,j,dim)]);
      accumulate_sum(&sum, src[RIDX(i,j+1,dim)]);
      accumulate_sum(&sum, src[RIDX(i+1,j-1,dim)]);
      accumulate_sum(&sum, src[RIDX(i+1,j,dim)]);
      accumulate_sum(&sum, src[RIDX(i+1,j+1,dim)]);
      /*
      for(int jj=(j-1); jj <= (j+1); jj++) 
	for(int ii=(i-1); ii <= (i+1); ii++) 
	  accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);
      */
      current_pixel.red = (unsigned short) (sum.red/9);
      current_pixel.green = (unsigned short) (sum.green/9);
      current_pixel.blue = (unsigned short) (sum.blue/9);
      current_pixel.alpha = (unsigned short) (sum.alpha/9);
      dst[RIDX(i,j, dim)] = current_pixel;
    }
  }
  // CORNER PIXELS
  // top left
  pixel_sum sum;
  pixel current_pixel;

  initialize_pixel_sum(&sum);
  for(int jj=0; jj <= 1; jj++) 
    for(int ii=0; ii <= 1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(0,0, dim)] = current_pixel;

  // top right
  initialize_pixel_sum(&sum);
  for(int jj=dim-2; jj <= dim-1; jj++) 
    for(int ii=0; ii <= 1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(0,dim-1, dim)] = current_pixel;

  // bottom right
  initialize_pixel_sum(&sum);
  for(int jj=dim-2; jj <= dim-1; jj++) 
    for(int ii=dim-2; ii <= dim-1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(dim-1,dim-1, dim)] = current_pixel;

  // bottom left
  initialize_pixel_sum(&sum);
  for(int jj=0; jj <= 1; jj++) 
    for(int ii=dim-2; ii <= dim-1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(dim-1,0, dim)] = current_pixel;
  
  // EDGE PIXELS
  // top edge
  for (int j = 1; j <= dim-2; j++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=(j-1); jj <= (j+1); jj++) 
      for(int ii=0; ii <= 1; ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(0,j, dim)] = current_pixel;
  }

  // bottom edge
  for (int j = 1; j <= dim-2; j++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=(j-1); jj <= (j+1); jj++) 
      for(int ii=dim-2; ii <= dim-1; ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(dim-1,j, dim)] = current_pixel;
  }

  // left edge
  for (int i = 1; i <= dim-2; i++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=0; jj <= 1; jj++) 
      for(int ii=(i-1); ii <= (i+1); ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(i,0, dim)] = current_pixel;
  }

  // right edge
  for (int i = 1; i <= dim-2; i++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=dim-2; jj <= dim-1; jj++) 
      for(int ii=(i-1); ii <= (i+1); ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(i,dim-1, dim)] = current_pixel;
  }
}

char my_smooth_descr[] = "my_smooth: ";
void my_smooth(int dim, pixel *src, pixel *dst) 
{
  // MIDDLE PIXELS
  for (int i = 1; i <= dim-2; i++){
    for (int j = 1; j <= dim-2; j+=4){
      // top row
      __m128i x1 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1,j-1,dim)]);
      __m256i pix1 = _mm256_cvtepu8_epi16(x1);
      __m128i x2 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1,j,dim)]);
      __m256i pix2 = _mm256_cvtepu8_epi16(x2);
      __m128i x3 = _mm_loadu_si128((__m128i*) &src[RIDX(i-1,j+1,dim)]);
      __m256i pix3 = _mm256_cvtepu8_epi16(x3);
      // middle row
      __m128i x4 = _mm_loadu_si128((__m128i*) &src[RIDX(i,j-1,dim)]);
      __m256i pix4 = _mm256_cvtepu8_epi16(x4);
      __m128i x5 = _mm_loadu_si128((__m128i*) &src[RIDX(i,j,dim)]);
      __m256i pix5 = _mm256_cvtepu8_epi16(x5);
      __m128i x6 = _mm_loadu_si128((__m128i*) &src[RIDX(i,j+1,dim)]);
      __m256i pix6 = _mm256_cvtepu8_epi16(x6);
      // bottom row
      __m128i x7 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1,j-1,dim)]);
      __m256i pix7 = _mm256_cvtepu8_epi16(x7);
      __m128i x8 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1,j,dim)]);
      __m256i pix8 = _mm256_cvtepu8_epi16(x8);
      __m128i x9 = _mm_loadu_si128((__m128i*) &src[RIDX(i+1,j+1,dim)]);
      __m256i pix9 = _mm256_cvtepu8_epi16(x9);
      pix1 = _mm256_add_epi16(pix1, pix2);
      pix1 = _mm256_add_epi16(pix1, pix3);
      pix1 = _mm256_add_epi16(pix1, pix4);
      pix1 = _mm256_add_epi16(pix1, pix5);
      pix1 = _mm256_add_epi16(pix1, pix6);
      pix1 = _mm256_add_epi16(pix1, pix7);
      pix1 = _mm256_add_epi16(pix1, pix8);
      pix1 = _mm256_add_epi16(pix1, pix9);
      
      unsigned short pixel_elements[16];
      _mm256_storeu_si256((__m256i*) pixel_elements, pix1);

      pixel pA, pB, pC, pD;
      pA.red = (unsigned short) (pixel_elements[0]/9);
      pA.green = (unsigned short) (pixel_elements[1]/9);
      pA.blue = (unsigned short) (pixel_elements[2]/9);
      pA.alpha = (unsigned short) (pixel_elements[3]/9);
      dst[RIDX(i,j, dim)] = pA;
      pB.red = (unsigned short) (pixel_elements[4]/9);
      pB.green = (unsigned short) (pixel_elements[5]/9);
      pB.blue = (unsigned short) (pixel_elements[6]/9);
      pB.alpha = (unsigned short) (pixel_elements[7]/9);
      dst[RIDX(i,j+1, dim)] = pB;
      pC.red = (unsigned short) (pixel_elements[8]/9);
      pC.green = (unsigned short) (pixel_elements[9]/9);
      pC.blue = (unsigned short) (pixel_elements[10]/9);
      pC.alpha = (unsigned short) (pixel_elements[11]/9);
      dst[RIDX(i,j+2, dim)] = pC;
      pD.red = (unsigned short) (pixel_elements[12]/9);
      pD.green = (unsigned short) (pixel_elements[13]/9);
      pD.blue = (unsigned short) (pixel_elements[14]/9);
      pD.alpha = (unsigned short) (pixel_elements[15]/9);
      dst[RIDX(i,j+3, dim)] = pD;
    }
  }
  // CORNER PIXELS
  // top left
  pixel_sum sum;
  pixel current_pixel;

  initialize_pixel_sum(&sum);
  for(int jj=0; jj <= 1; jj++) 
    for(int ii=0; ii <= 1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(0,0, dim)] = current_pixel;

  // top right
  initialize_pixel_sum(&sum);
  for(int jj=dim-2; jj <= dim-1; jj++) 
    for(int ii=0; ii <= 1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(0,dim-1, dim)] = current_pixel;

  // bottom right
  initialize_pixel_sum(&sum);
  for(int jj=dim-2; jj <= dim-1; jj++) 
    for(int ii=dim-2; ii <= dim-1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(dim-1,dim-1, dim)] = current_pixel;

  // bottom left
  initialize_pixel_sum(&sum);
  for(int jj=0; jj <= 1; jj++) 
    for(int ii=dim-2; ii <= dim-1; ii++) 
      accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

  current_pixel.red = (unsigned short) (sum.red/4);
  current_pixel.green = (unsigned short) (sum.green/4);
  current_pixel.blue = (unsigned short) (sum.blue/4);
  current_pixel.alpha = (unsigned short) (sum.alpha/4);
  dst[RIDX(dim-1,0, dim)] = current_pixel;
  
  // EDGE PIXELS
  // top edge
  for (int j = 1; j <= dim-2; j++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=(j-1); jj <= (j+1); jj++) 
      for(int ii=0; ii <= 1; ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(0,j, dim)] = current_pixel;
  }

  // bottom edge
  for (int j = 1; j <= dim-2; j++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=(j-1); jj <= (j+1); jj++) 
      for(int ii=dim-2; ii <= dim-1; ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(dim-1,j, dim)] = current_pixel;
  }

  // left edge
  for (int i = 1; i <= dim-2; i++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=0; jj <= 1; jj++) 
      for(int ii=(i-1); ii <= (i+1); ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(i,0, dim)] = current_pixel;
  }

  // right edge
  for (int i = 1; i <= dim-2; i++){
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(int jj=dim-2; jj <= dim-1; jj++) 
      for(int ii=(i-1); ii <= (i+1); ii++)
	accumulate_sum(&sum, src[RIDX(ii,jj,dim)]);

    current_pixel.red = (unsigned short) (sum.red/6);
    current_pixel.green = (unsigned short) (sum.green/6);
    current_pixel.blue = (unsigned short) (sum.blue/6);
    current_pixel.alpha = (unsigned short) (sum.alpha/6);
    dst[RIDX(i,dim-1, dim)] = current_pixel;
  }
}

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth function by calling the add_smooth_function() for
 *     each test function. When you run the benchmark program, it will
 *     test and report the performance of each registered test
 *     function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&another_smooth, another_smooth_descr);
    add_smooth_function(&my_smooth_checkpoint, my_smooth_checkpoint_descr);
    add_smooth_function(&my_smooth, my_smooth_descr);
}
