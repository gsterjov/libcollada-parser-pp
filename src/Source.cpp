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

#include "Source.h"

#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Source::Source (ticpp::Element *element)
	{
		parse (element);
	}
	
	
	/* destructor */
	Source::~Source ()
	{
	}
	
	
	
	
	/* parse source */
	void Source::parse (ticpp::Element *element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* get source properties */
		mID   = element->GetAttribute ("id");
		mName = element->GetAttributeOrDefault ("name", "");
		
		
		/* sift through source elements */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			/* get element name */
			std::string el = iter->Value();
			
			
			/* found float array */
			if (el == "float_array")
			{
				/* get float data */
				unsigned int count;
				iter->GetAttribute ("count", &count);
				
				mData.reserve (count);
				
				
				/* float data */
				std::string data = iter->GetText ();
				std::istringstream stream (data);
				
				/* convert and add to source */
				while (count > 0)
				{
					float val;
					stream >> val;
					mData.push_back (val);
					
					count--;
				}
			}
			
			
			/* found common technique */
			else if (el == "technique_common")
			{
				/* get accessor */
				parseAccessor (iter->FirstChildElement());
			}
			
		} /* end source */
	}
	
	
	
	
	/* read the source accessor */
	void Source::parseAccessor (ticpp::Element *element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* get accessor properties */
		element->GetAttribute ("count",  &mAccessor.count);
		element->GetAttributeOrDefault ("offset", &mAccessor.offset, 0);
		element->GetAttributeOrDefault ("stride", &mAccessor.stride, 1);
		
		
		/* sift through accessor params */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			Accessor::Param param;
			param.skip = iter->HasAttribute ("name") == false;
			param.type = parseType (iter->GetAttribute ("type"));
			
			mAccessor.params.push_back (param);
		}
	}
	
	
	
	/* data type conversion */
	DataType Source::parseType (std::string type)
	{
		if      (type == "int")    return DATA_TYPE_INTEGER;
		else if (type == "float")  return DATA_TYPE_FLOAT;
		else if (type == "Name")   return DATA_TYPE_NAME;
		else if (type == "bool")   return DATA_TYPE_BOOL;
		else if (type == "IDREF")  return DATA_TYPE_IDREF;
		else if (type == "SIDREF") return DATA_TYPE_SIDREF;
		
		return DATA_TYPE_UNKNOWN;
	}

}
