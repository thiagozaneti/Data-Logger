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

#include "lcm_gpd_line_c.h"

uint8_t lcm_gpd_line_begin(gpd_line_parameters_t* parameters)
{
   if (parameters->maximum_numbers_of_vertices == 0)
   {
      return 0;
   }
   uint16_t fncLine = 2;
   uint16_t fncAddress = parameters->vp;

   lcm_write_vp(fncAddress, fncLine);

   lcm_gpd_line_set_color(parameters);

   lcm_gpd_line_clear(parameters);

   uint16_t initialFunctionCount = 0;
   parameters->function_count = initialFunctionCount;
   lcm_gpd_line_set_function_count(parameters);

   return 1;
}

uint8_t lcm_gpd_line_set_color(gpd_line_parameters_t* parameters)
{
   uint16_t colorAddress = parameters->vp + 2;
   lcm_write_vp(colorAddress, parameters->color);
   return 1;
}

uint8_t lcm_gpd_line_set_function_count(gpd_line_parameters_t* parameters)
{
   if (parameters->function_count >= parameters->maximum_numbers_of_vertices)
   {
      parameters->function_count = parameters->maximum_numbers_of_vertices - 1;
   }

   uint16_t functionCountAddress = parameters->vp + 1;
   lcm_write_vp(functionCountAddress, parameters->function_count);
   return 1;
}

uint8_t lcm_gpd_line_write(gpd_line_parameters_t* parameters, uint16_t x, uint16_t y, uint16_t vertexIndex)
{
   uint16_t vertexAddress = parameters->vp + (vertexIndex * 2) + 3;
   
   if (x < parameters->area.xi)
   {
      x = parameters->area.xi;
   }
   else if (x > (parameters->area.xi + (parameters->area.width - 1)))
   {
      x = (parameters->area.xi + (parameters->area.width - 1));
   }

   if (y < parameters->area.yi)
   {
      y = parameters->area.yi;
   }
   else if (y > (parameters->area.yi + (parameters->area.height - 1)))
   {
      y = (parameters->area.yi + (parameters->area.height - 1));
   }

   uint16_t vertex[2];
   vertex[0] = x;
   vertex[1] = y;
   
   lcm_write_multiple_vps(vertexAddress, vertex, 2);

   if (vertexIndex > parameters->function_count)
   {
      parameters->function_count = vertexIndex;
      lcm_gpd_line_set_function_count(parameters);
   }
   
   return 1;
}

uint8_t lcm_gpd_line_multiple_write(gpd_line_parameters_t* parameters, uint16_t* x, uint16_t* y, uint16_t length)
{
   for (uint16_t i = 0; i < length; i++)
   {
      lcm_gpd_line_write(parameters, x[i], y[i], i);
   }
   return 0;
}

uint8_t lcm_gpd_line_clear(gpd_line_parameters_t* parameters)
{
    parameters->function_count = 0;
   lcm_gpd_line_set_function_count(parameters);

   return 0;
}

