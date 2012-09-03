/*
Copyright (c) 2010 Goran Sterjov

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

#include "Material.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Material::Material (ticpp::Element* element)
	{
		parse (element);
	}
	
	
	/* destructor */
	Material::~Material ()
	{
		
	}
	
	
	
	
	/* parse material element */
	void Material::parse (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* material properties */
		mID   = element->GetAttributeOrDefault ("id", "");
		mName = element->GetAttributeOrDefault ("name", "");
		
		
		/* sift through material elements
		 * asset[0-1], instance_effect[1], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			std::string val = iter->Value();
			ticpp::Element* child = iter.Get();
			
			
			/* found an effect instance */
			if (val == "instance_effect")
				parseEffect (child);
		}
	}
	
	
	
	/* parse effect instance */
	void Material::parseEffect (ticpp::Element* element)
	{
		/* effect properties */
		mEffect.sid  = element->GetAttributeOrDefault ("sid", "");
		mEffect.name = element->GetAttributeOrDefault ("name", "");
		mEffect.url  = element->GetAttribute ("url");
	}

}
