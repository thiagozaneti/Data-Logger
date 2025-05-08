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

#include "GPD_Line.h"

LcmGPDLine::LcmGPDLine()
{
}

uint8_t LcmGPDLine::begin(uint16_t vp, gpd_area_t area, uint16_t maximunNumbersOfVertices, uint16_t color)
{
	_parameters.vp = vp;
	_parameters.area = area;
	_parameters.maximum_numbers_of_vertices = maximunNumbersOfVertices;
	_parameters.color = color;
	return lcm_gpd_line_begin(&_parameters);
}


uint8_t LcmGPDLine::setColor(uint16_t color)
{
	_parameters.color = color;	
	return lcm_gpd_line_set_color(&_parameters);
}

uint8_t LcmGPDLine::setFunctionCount(uint16_t functionCount)
{
	_parameters.function_count = functionCount;	
	return lcm_gpd_line_set_function_count(&_parameters);
}

uint8_t LcmGPDLine::write(uint16_t x, uint16_t y, uint16_t vertexIndex)
{
	return lcm_gpd_line_write(&_parameters, x, y, vertexIndex);
}

uint8_t LcmGPDLine::write(uint16_t* x, uint16_t* y, uint16_t length)
{
	for (uint16_t i = 0; i < length; i++)
	{
		write(x[i], y[i], i);
	}
	return 1;
}

uint8_t LcmGPDLine::clear()
{
	return setFunctionCount(0);
};

LcmGPDLine::~LcmGPDLine()
{
}
