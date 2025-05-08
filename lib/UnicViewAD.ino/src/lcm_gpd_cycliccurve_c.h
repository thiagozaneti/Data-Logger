/*
# Arquivo:

   UnicViewAD.h

# Projeto:

   UnicViewAD.lcm - Biblioteca Proculus em C

# Descrição:

   Esta biblioteca permite que um Arduino se comunique facilmente com o Displays
   Inteligentes Proculus, oferecendo um controle mais intuitivo e prático de todas
   as suas funções.

   Ela elimina a necessidade de um completo entendimento sobre todos os protocolos de
   comunicação e funcionamento interno do Display, abstraindo esses protocolos para
   pequenas funções que garantem facilidade e rapidez de uso.

# Autores:

   * Andreo de Freitas
   * Rafael Moura (revisão)

# Versão:

   1.0

# Licença:

   Copyright (c) 2020 Proculus Technologies Limited and Victor Vision LTDA

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

 */

#ifndef _PIC_GPD_CyclicCurve_h
#define _PIC_GPD_CyclicCurve_h

#include "lcm_gpd_area_c.h"
#include "lcm_gpd_line_c.h"
#include "lcm_gpd_copypaste_c.h"


typedef struct gpd_cyclic_curve_configuration
{
   int32_t min_value;
   int32_t max_value;
   uint16_t color;
   uint8_t gap;
   uint16_t gap_pic_id;
   uint8_t points_per_pixel;
   uint8_t horizontal_increment;
} gpd_cyclic_curve_configuration_t;


typedef struct gpd_cyclic_curve_parameters
{
   gpd_cyclic_curve_configuration_t config;
   gpd_line_parameters_t line_parameters;
   gpd_copy_paste_parameters_t copy_paste_parameters;

   uint16_t index;
   uint16_t coordinate_x_control;
   uint16_t last_value_of_y;
   uint16_t last_value_of_x;
   int32_t zoom;
   int32_t vertical_origin;
   gpd_area_t area;
   gpd_area_t copy_paste_area;
   uint16_t area_xf;
   uint8_t gap_selection_lock;
   uint8_t first_point;
   uint16_t transition_x;
   uint16_t transition_y;
   uint16_t maximun_numbers_of_vertices;

} gpd_cyclic_curve_parameters_t;


   uint8_t lcm_gpd_cyclic_curve_begin(uint16_t vp, gpd_area_t area, gpd_cyclic_curve_parameters_t* parameters, gpd_cyclic_curve_configuration_t config);

   uint8_t lcm_gpd_cyclic_curve_set_color(gpd_cyclic_curve_parameters_t* parameters, uint16_t color);
   uint8_t lcm_gpd_cyclic_curve_clear(gpd_cyclic_curve_parameters_t* parameters);

   uint8_t lcm_gpd_cyclic_curve_write(gpd_cyclic_curve_parameters_t* parameters, int32_t value);
   uint8_t lcm_gpd_cyclic_curve_multiple_write(gpd_cyclic_curve_parameters_t* parameters, int32_t* value, uint16_t length);

   uint8_t lcm_gpd_cyclic_curve_set_normalization(gpd_cyclic_curve_parameters_t* parameters);
   uint16_t lcm_gpd_cyclic_curve_set_maximun_numbers_of_vertices(gpd_cyclic_curve_parameters_t* parameters);

#endif
