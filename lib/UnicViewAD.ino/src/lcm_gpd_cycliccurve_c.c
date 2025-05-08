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

#include "lcm_gpd_cycliccurve_c.h"


uint8_t lcm_gpd_cyclic_curve_begin(uint16_t vp, gpd_area_t area, gpd_cyclic_curve_parameters_t* parameters, gpd_cyclic_curve_configuration_t config)
{
   parameters->config = config;
   parameters->area = area;
   lcm_gpd_cyclic_curve_set_normalization(parameters);
   
   parameters->index = 0;
   parameters->copy_paste_area.xi = parameters->area.xi;
   parameters->copy_paste_area.yi = parameters->area.yi;
   parameters->copy_paste_area.width = 0;
   parameters->copy_paste_area.height = parameters->area.height;
   parameters->area_xf = parameters->area.xi + parameters->area.width - 1;
   parameters->first_point = 1;

   parameters->line_parameters.vp = vp + 16;
   parameters->line_parameters.area = area;
   parameters->line_parameters.maximum_numbers_of_vertices = lcm_gpd_cyclic_curve_set_maximun_numbers_of_vertices(parameters);
   parameters->line_parameters.color = config.color;
   
   parameters->copy_paste_parameters.vp = vp;
//   parameters->copy_paste_parameters.area = area;
   parameters->copy_paste_parameters.maximum_numbers_of_copy_pastes = 2;
   	
   uint8_t data = lcm_gpd_line_begin(&parameters->line_parameters);
   data = lcm_gpd_copy_paste_begin(&parameters->copy_paste_parameters);
   
   return data;
}

uint8_t lcm_gpd_cyclic_curve_set_normalization(gpd_cyclic_curve_parameters_t* parameters)
{
    int32_t temp_height = (int32_t)parameters->area.height;
   parameters->zoom = (int32_t)(temp_height << 8);
   parameters->zoom /= (parameters->config.max_value - parameters->config.min_value);
   parameters->vertical_origin = (parameters->area.yi + parameters->area.height - 1);

   return 0;
}

uint16_t lcm_gpd_cyclic_curve_set_maximun_numbers_of_vertices(gpd_cyclic_curve_parameters_t* parameters)
{
   parameters->maximun_numbers_of_vertices = parameters->area.width;
   if (parameters->config.horizontal_increment != 1)
   {
      parameters->maximun_numbers_of_vertices /= parameters->config.horizontal_increment;
      parameters->maximun_numbers_of_vertices += 2;
   }
   else
   {
      parameters->maximun_numbers_of_vertices *= parameters->config.points_per_pixel;
   }
   return parameters->maximun_numbers_of_vertices;
}

uint8_t lcm_gpd_cyclic_curve_set_color(gpd_cyclic_curve_parameters_t* parameters, uint16_t color)
{
   parameters->line_parameters.color = color;
   return lcm_gpd_line_set_color(&parameters->line_parameters);
}

uint8_t lcm_gpd_cyclic_curve_write(gpd_cyclic_curve_parameters_t* parameters, int32_t valueIn)
{
   uint16_t x;

   int32_t y = valueIn - parameters->config.min_value;
   
   y *= parameters->zoom;
   y = (y >> 8);
   y = parameters->vertical_origin - y;
   
   if (parameters->config.horizontal_increment != 1)
   {
      if (parameters->first_point == 1)
      {
         parameters->first_point = 0;
         x = parameters->area.xi;
         lcm_gpd_line_write(&parameters->line_parameters, (uint16_t)x, (uint16_t)y, 0);
         parameters->index = 1;
      }
      else
      {
         x = ((parameters->index - 1) * parameters->config.horizontal_increment) + parameters->area.xi;
      }

      if (x > (parameters->area.xi + parameters->area.width - 1))
      {
         uint16_t transitionX = (parameters->area.xi + parameters->area.width - 1);
         uint16_t transitionY = ((uint16_t)y + parameters->last_value_of_y) / 2;
         lcm_gpd_line_write(&parameters->line_parameters, transitionX, transitionY, parameters->index);
         transitionX = parameters->area.xi;
         parameters->index = 0;
         lcm_gpd_line_write(&parameters->line_parameters, transitionX, transitionY, parameters->index);
         parameters->index++;
         x -= (parameters->area.xi + parameters->area.width - 1);
      }
   }
   else
   {
      if (parameters->config.points_per_pixel != 1)
      {
         if (parameters->index == parameters->maximun_numbers_of_vertices)
         {
            parameters->index = 0;
         }
         x = (parameters->index / parameters->config.points_per_pixel) + parameters->area.xi;
      }
      else
      {
         if (parameters->index == parameters->area.width)
         {
            parameters->index = 0;
         }
         x = parameters->index + parameters->area.xi;
      }
   }

   parameters->last_value_of_y = (uint16_t)y;
   parameters->last_value_of_x = x;
   lcm_gpd_line_write(&parameters->line_parameters, (uint16_t)x, (uint16_t)y, parameters->index);

   if (parameters->config.gap > 0)
   {
      parameters->copy_paste_area.xi = x - 1;
      parameters->copy_paste_area.width = (parameters->area_xf + 1) - parameters->copy_paste_area.xi;

      if (parameters->copy_paste_area.width >= parameters->config.gap)
      {
         if (parameters->gap_selection_lock == 1)
         {
            parameters->gap_selection_lock = 0;
            parameters->copy_paste_parameters.function_count = 1;
            lcm_gpd_copy_paste_set_function_count(&parameters->copy_paste_parameters);
         }
         parameters->copy_paste_area.width = parameters->config.gap;
         lcm_gpd_copy_paste_write(&parameters->copy_paste_parameters, parameters->config.gap_pic_id, parameters->copy_paste_area, parameters->copy_paste_area.xi, parameters->copy_paste_area.yi, 0);
      }
      else
      {
         parameters->gap_selection_lock = 1;
         lcm_gpd_copy_paste_write(&parameters->copy_paste_parameters, parameters->config.gap_pic_id, parameters->copy_paste_area, parameters->copy_paste_area.xi, parameters->copy_paste_area.yi, 0);
         parameters->copy_paste_area.xi = parameters->area.xi;
         parameters->copy_paste_area.width = parameters->config.gap - parameters->copy_paste_area.width;      	
         lcm_gpd_copy_paste_write(&parameters->copy_paste_parameters, parameters->config.gap_pic_id, parameters->copy_paste_area, parameters->copy_paste_area.xi, parameters->copy_paste_area.yi, 1);
      }
   }
   parameters->index++;
   return 0;
}

uint8_t lcm_gpd_cyclic_curve_multiple_write(gpd_cyclic_curve_parameters_t* parameters, int32_t* value, uint16_t length)
{
   for (uint16_t i = 0; i < length; i++)
   {
      lcm_gpd_cyclic_curve_write(parameters, value[i]);
   }
   return 0;
}

uint8_t lcm_gpd_cyclic_curve_clear(gpd_cyclic_curve_parameters_t* parameters)
{
   parameters->index = 0;
   parameters->copy_paste_parameters.function_count = 1;
   lcm_gpd_copy_paste_set_function_count(&parameters->copy_paste_parameters);
   
   parameters->copy_paste_area.width = parameters->config.gap;
   parameters->copy_paste_area.xi = parameters->area.xi;
   lcm_gpd_copy_paste_write(&parameters->copy_paste_parameters, parameters->config.gap_pic_id, parameters->copy_paste_area, parameters->copy_paste_area.xi, parameters->copy_paste_area.yi, 0);

   lcm_gpd_line_clear(&parameters->line_parameters);

   return 0;
}
