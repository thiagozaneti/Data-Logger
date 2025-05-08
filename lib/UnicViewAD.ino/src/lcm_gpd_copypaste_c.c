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

#include "lcm_gpd_copypaste_c.h"

uint8_t lcm_gpd_copy_paste_begin(gpd_copy_paste_parameters_t* parameters)
{
   if (parameters->maximum_numbers_of_copy_pastes == 0)
   {
      return 0;
   }
   uint16_t fncCopyPaste = 6;
   uint16_t fncAddress = parameters->vp;

   lcm_write_vp(fncAddress, fncCopyPaste);
   	
   lcm_gpd_copy_paste_clear(parameters);

   return 1;
}

uint8_t lcm_gpd_copy_paste_set_function_count(gpd_copy_paste_parameters_t* parameters)
{
   if (parameters->function_count > parameters->maximum_numbers_of_copy_pastes)
   {
      parameters->function_count = parameters->maximum_numbers_of_copy_pastes;
   }

   uint16_t functionCountAddress = parameters->vp + 1;
   lcm_write_vp(functionCountAddress, parameters->function_count);
   return 1;
}

uint8_t lcm_gpd_copy_paste_write(gpd_copy_paste_parameters_t* parameters, uint16_t pic_id, gpd_area_t source_area, uint16_t target_xi, uint16_t target_yi, uint16_t index)
{
   const uint8_t functionLength = 7;
   uint16_t functionAddress = parameters->vp + 2 + (index * functionLength);
   uint16_t xf = source_area.xi + source_area.width;
   uint16_t yf = source_area.yi + source_area.height;

   uint16_t data[7];
   data[0] = pic_id;
   data[1] = source_area.xi;
   data[2] = source_area.yi;
   data[3] = xf;
   data[4] = yf;
   data[5] = target_xi;
   data[6] = target_yi;

   lcm_write_multiple_vps(functionAddress, data, functionLength);

   ++index;
   if (index > parameters->function_count)
   {
      parameters->function_count = index;
      lcm_gpd_copy_paste_set_function_count(parameters);
   }
   return 1;
}

uint8_t lcm_gpd_copy_paste_clear(gpd_copy_paste_parameters_t* parameters)
{
   parameters->function_count = 0;
   lcm_gpd_copy_paste_set_function_count(parameters);
   return 1;
}
