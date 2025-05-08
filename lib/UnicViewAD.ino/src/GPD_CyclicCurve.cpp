/*
# Arquivo:

	UnicViewAD.cpp

# Projeto:

	UnicView AD.ino - Biblioteca para Arduino

# Descri��o:

	Esta biblioteca permite que um Arduino se comunique facilmente com o Displays 
	Inteligentes Proculus, oferecendo um controle mais intuitivo e pr�tico de todas 
	as suas fun��es.

	Ela elimina a necessidade de um completo entendimento sobre todos os protocolos de 
	comunica��o e funcionamento interno do Display, abstraindo esses protocolos para 
	pequenas fun��es que garantem facilidade e rapidez de uso.

# Documenta��o do projeto:

	http://www.victorvision.com.br/projects/arduino-system/library/site/index.html

# Autores:

	* Andreo de Freitas
	* Rafael Moura (revis�o)
	* Renan Candido Vieira (revis�o)

# Vers�o:

	1.2

# Licen�a:

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

#include "GPD_CyclicCurve.h"

LcmGPDCyclicCurve::LcmGPDCyclicCurve()
{
//	_CopyPaste = new LcmGPDCopyPaste();
//	_line = new LcmGPDLine();
}

uint8_t LcmGPDCyclicCurve::begin(uint16_t vp, gpd_area_t area, gpd_cyclic_curve_configuration_t config)
{
	return lcm_gpd_cyclic_curve_begin(vp, area, &_parameters, config);
}

uint8_t LcmGPDCyclicCurve::setNormalization()
{
	return lcm_gpd_cyclic_curve_set_normalization(&_parameters);
}

uint16_t LcmGPDCyclicCurve::setMaximunNumbersOfVertices()
{
	return lcm_gpd_cyclic_curve_set_maximun_numbers_of_vertices(&_parameters);
}

uint8_t LcmGPDCyclicCurve::setColor(uint16_t color)
{
	return lcm_gpd_cyclic_curve_set_color(&_parameters, color);
}

uint8_t LcmGPDCyclicCurve::write(int32_t valueInp)
{
	return lcm_gpd_cyclic_curve_write(&_parameters, valueInp);
}

uint8_t LcmGPDCyclicCurve::write(int32_t* value, uint16_t length)
{
	return lcm_gpd_cyclic_curve_multiple_write(&_parameters, value, length);
}

uint8_t LcmGPDCyclicCurve::clear()
{
	return lcm_gpd_cyclic_curve_clear(&_parameters);
}

LcmGPDCyclicCurve::~LcmGPDCyclicCurve()
{
}
