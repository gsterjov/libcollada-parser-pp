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

#include "Input.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Input::Input (ticpp::Element *element, SourceMap &sources)
	{
		mSource = 0;
		mIndices = 0;
		parse (element, sources);
	}
	
	
	
	
	/* destructor */
	Input::~Input ()
	{
		
	}
	
	
	
	
	/* get data from source */
	void Input::getData (int index, std::vector<float> &data)
	{
		Accessor accessor = mSource->getAccessor();
		
		
		/* get source data index */
		if (mIndices)
			index = mIndices->get (index, mOffset);
		
		
		/* apply source offset and stride */
		index *= accessor.stride;
		index += accessor.offset;
		
		
		/* loop through each param. according to the spec
		 * the stride must be equal to or greater than the amount
		 * of params so we are safe from over-reading here */
		for (int i = 0; i < accessor.params.size(); i++)
		{
			/* if param isn't empty add to the data list */
			if (!accessor.params[i].skip)
				data.push_back (mSource->getData (index));
			
			index++;
		}
	}
	
	
	
	
	
	
	void Input::parse (ticpp::Element *element, SourceMap &sources)
	{
		/* get input properties */
		std::string uri = element->GetAttribute ("source");
		std::string semantic = element->GetAttribute ("semantic");
		element->GetAttributeOrDefault ("offset", &mOffset, 0);
		
		
		mSemantic = parseSemantic (semantic);
		
		
		/* couldn't find input source */
		if (sources.find (uri) == sources.end())
		{
			std::string error = "Parsing failed: No source named '" + uri + "' found";
			throw std::runtime_error (error.c_str());
		}
		
		
		/* attach input to source */
		mSource = sources[uri];
	}
	
	
	
	
	/* convert semantic type by string */
	InputSemantic Input::parseSemantic (const std::string &semantic)
	{
		/* life can be so boring at times */
		if      (semantic == "BINORMAL")        return INPUT_SEMANTIC_BINORMAL;
		else if (semantic == "COLOR")           return INPUT_SEMANTIC_COLOR;
		else if (semantic == "CONTINUITY")      return INPUT_SEMANTIC_CONTINUITY;
		else if (semantic == "IMAGE")           return INPUT_SEMANTIC_IMAGE;
		else if (semantic == "INPUT")           return INPUT_SEMANTIC_INPUT;
		else if (semantic == "IN_TANGENT")      return INPUT_SEMANTIC_IN_TANGENT;
		else if (semantic == "INTERPOLATION")   return INPUT_SEMANTIC_INTERPOLATION;
		else if (semantic == "INV_BIND_MATRIX") return INPUT_SEMANTIC_INV_BIND_MATRIX;
		else if (semantic == "JOINT")           return INPUT_SEMANTIC_JOINT;
		else if (semantic == "LINEAR_STEPS")    return INPUT_SEMANTIC_LINEAR_STEPS;
		else if (semantic == "MORPH_TARGET")    return INPUT_SEMANTIC_MORPH_TARGET;
		else if (semantic == "NORMAL")          return INPUT_SEMANTIC_NORMAL;
		else if (semantic == "OUTPUT")          return INPUT_SEMANTIC_OUTPUT;
		else if (semantic == "OUT_TANGENT")     return INPUT_SEMANTIC_OUT_TANGENT;
		else if (semantic == "POSITION")        return INPUT_SEMANTIC_POSITION;
		else if (semantic == "TANGENT")         return INPUT_SEMANTIC_TANGENT;
		else if (semantic == "TEXBINORMAL")     return INPUT_SEMANTIC_TEXBINORMAL;
		else if (semantic == "TEXCOORD")        return INPUT_SEMANTIC_TEXCOORD;
		else if (semantic == "TEXTANGENT")      return INPUT_SEMANTIC_TEXTANGENT;
		else if (semantic == "UV")              return INPUT_SEMANTIC_UV;
		else if (semantic == "VERTEX")          return INPUT_SEMANTIC_VERTEX;
		else if (semantic == "WEIGHT")          return INPUT_SEMANTIC_WEIGHT;
	}

}
