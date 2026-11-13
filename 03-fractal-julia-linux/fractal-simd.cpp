#include "fractal-simd.h"
#include "palette.h"
#include <complex>
 
 
#include<immintrin.h> //AVX2
 
extern std::complex<double>  c;
 
extern int max_iteraciones;
 
 
 
void julia_simd( double x_min, double y_min, double x_max, double y_max,
    uint32_t width, uint32_t height, uint32_t* pixel_buffer){
 
        double dx = (x_max-x_min)/width;
        double dy = (y_max - y_min)/height;
 
            //Empaquetar 8 veces xmin
        __m256 xmin = _mm256_set1_ps(x_min);
        __m256 ymax = _mm256_set1_ps(y_max);
 
 
        //Empaquetar el vector de dx y dy
        __m256 xscale = _mm256_set1_ps(dx);
        __m256 yscale = _mm256_set1_ps(dy);
 
          __m256 max_norma = _mm256_set1_ps(4.0); //4*4*4*4*4*4*4*4
 
        __m256 vect = _mm256_set1_ps(1.0); //Vector de puros 1
 
        for (int i=0; i<width; i++){
            for(int j =0; j<height;j +=8 ){
 
                __m256 mx = _mm256_set1_ps(i);
                __m256 my = _mm256_setr_ps(j+7,j+6, j+5, j+4, j+3, j+2, j+1, j+0);
                
 
                //PArte real
                __m256 cr = _mm256_add_ps(xmin, _mm256_mul_ps(mx, xscale)) ;
 
                //Parte imaginaria
                __m256 ci = _mm256_sub_ps(ymax, _mm256_mul_ps(my, yscale)) ;
 
                //
                __m256 c_real = _mm256_set1_ps(c.real());
                __m256 c_imag = _mm256_set1_ps(c.imag());
 
                //Verificar si los 8 complejos estàs acotados
                //En parejas de complejos x_real y_imaginario
 
                int iter =0;
                __m256 mk = _mm256_set1_ps(iter);
 
                __m256  zr = cr;
                __m256  zi = ci;
 
              
 
 
                while(iter< max_iteraciones){
 
                    //Zn+1 = Zn**2 + c
 
                   __m256 zr2 =  _mm256_mul_ps(zr, zr);  //X al 2
                   __m256 zi2 =  _mm256_mul_ps(zi,zi);  //Y al 2
 
                    __m256 zrzi = _mm256_mul_ps(zr, zi); // x*y
 
                    //1*zr*zi + c_imag
 
                    zr = _mm256_add_ps(_mm256_sub_ps(zr2, zi2), c_real);
                   zi = _mm256_add_ps( _mm256_add_ps(zrzi, zrzi), c_imag);
 
                   //CAlcular las normas
                   zr2 = _mm256_mul_ps(zr, zr);
                   zi2 = _mm256_mul_ps(zi,zi);
 
                   __m256 norma2 = _mm256_add_ps(zr2,zi2); //norma ** 2
 
                   //Si norma2 < 4 devuelve 0XFFFFFFF, sino devuelve 0X00000000
                   __m256 mask = _mm256_cmp_ps(norma2, max_norma, _CMP_LE_OQ); //norma<4.0
 
                   mk = _mm256_add_ps(mk, _mm256_and_ps(mask, vect));
 
                    if(_mm256_testz_ps(mask, _mm256_set1_ps(-1))){
                        break;
                    }
 
                    iter++;
                }
 
                //Verificar iter y seleccionar el color
 
                float d[8];
 
                _mm256_storeu_ps(d, mk);
 
                for(int it =0; it<8; it++){
                    int index = (j+it)*width+i;
 
                    if(index<width*height){
 
                    
 
                    if(d[it] < max_iteraciones){
                        int color_index = (int)d[it] % color_ramp.size();
pixel_buffer[index] = color_ramp[color_index];
 
                    }
                    else{
                        pixel_buffer[index] = 0xFF000000;
                    }
 
                    }
                }
            }
        }
 
    }