/*
# Arquivo:

	UnicViewAD.h

# Projeto:

	UnicView AD.ino - Biblioteca para Arduino

# Descrição:

	Esta biblioteca permite que um Arduino se comunique facilmente com o Displays
	Inteligentes Proculus, oferecendo um controle mais intuitivo e prático de todas
	as suas funções.

	Ela elimina a necessidade de um completo entendimento sobre todos os protocolos de
	comunicação e funcionamento interno do Display, abstraindo esses protocolos para
	pequenas funções que garantem facilidade e rapidez de uso.

# Documentação do projeto:

	http://www.victorvision.com.br/projects/arduino-system/library/site/index.html

# Autores:

	* Andreo de Freitas
	* Rafael Moura (revisão)
	* Renan Candido Vieira (revisão)

# Versão:

	1.1

# Licença:

	Copyright (c) 2019 Proculus Technologies Limited and Victor Vision LTDA

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

#ifndef _GPD_CyclicCurve_h
#define _GPD_CyclicCurve_h

#include "UnicViewAD.h"

//#include "GPD_Line.h"
//#include "GPD_CopyPaste.h"


#if defined(__cplusplus)                //
extern "C" {                            // Diretrizes de compila��o para compilar em C dentro do Arduino - N�o deve ser utilizado em programas C
#endif                                  //

#include "lcm_gpd_area_c.h"
#include "lcm_gpd_cycliccurve_c.h"

#if defined(__cplusplus)                //
}                            // Diretrizes de compila��o para compilar em C dentro do Arduino - N�o deve ser utilizado em programas C
#endif                 

class LcmGPDCyclicCurve //: public Print
{
public:
	
	LcmGPDCyclicCurve();

	uint8_t begin(uint16_t vp, gpd_area_t area, gpd_cyclic_curve_configuration_t config);

	uint8_t setColor(uint16_t color);
	uint8_t clear();

	uint8_t write(int32_t value);
	uint8_t write(int32_t* value, uint16_t length);
	
	virtual ~LcmGPDCyclicCurve();
	
private:

	uint8_t setNormalization();
	uint16_t setMaximunNumbersOfVertices();

	gpd_cyclic_curve_parameters_t _parameters;
};

#endif
