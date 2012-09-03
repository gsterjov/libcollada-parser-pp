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

#include "Geometry.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Primitive::Primitive (ticpp::Element *element, SourceMap* sources)
	: mIndices (new Indices ()),
	  mSources (sources)
	{
		parse (element);
	}
	
	
	
	/* destructor */
	Primitive::~Primitive ()
	{
		/* free inputs */
		for (int i = 0; i < mInputs.size(); i++)
			delete mInputs[i];
		
		/* free indices */
		if (mIndices)  delete mIndices;
	}
	
	
	
	
	/* whether geometry has normals */
	bool Primitive::hasNormals () const
	{
		for (int i = 0; i < mInputs.size(); i++)
		{
			if (mInputs[i]->getSemantic() == INPUT_SEMANTIC_NORMAL)
				return true;
		}
		
		return false;
	}
	
	
	/* whether geometry has texcoords */
	bool Primitive::hasTexCoords () const
	{
		for (int i = 0; i < mInputs.size(); i++)
		{
			if (mInputs[i]->getSemantic() == INPUT_SEMANTIC_TEXCOORD)
				return true;
		}
		
		return false;
	}
	
	
	
	
	/* get vertex */
	Vector Primitive::getVertex (int index) const
	{
		Vector vec;
		
		
		for (int i = 0; i < mInputs.size(); i++)
		{
			if (mInputs[i]->getSemantic() == INPUT_SEMANTIC_VERTEX)
			{
				std::vector<float> data;
				mInputs[i]->getData (index, data);
				
				vec.x = data[0];
				vec.y = data[1];
				vec.z = data[2];
			}
		}
		
		return vec;
	}
	
	
	/* get normal */
	Vector Primitive::getNormal (int index) const
	{
		Vector vec;
		
		
		for (int i = 0; i < mInputs.size(); i++)
		{
			if (mInputs[i]->getSemantic() == INPUT_SEMANTIC_NORMAL)
			{
				std::vector<float> data;
				mInputs[i]->getData (index, data);
				
				vec.x = data[0];
				vec.y = data[1];
				vec.z = data[2];
			}
		}
		
		return vec;
	}
	
	
	/* get texcoord */
	TexCoord Primitive::getTexCoord (int index) const
	{
		TexCoord tex;
		
		
		for (int i = 0; i < mInputs.size(); i++)
		{
			if (mInputs[i]->getSemantic() == INPUT_SEMANTIC_TEXCOORD)
			{
				std::vector<float> data;
				mInputs[i]->getData (index, data);
				
				tex.u = data[0];
				tex.v = data[1];
			}
		}
		
		return tex;
	}
	
	
	/* get index */
	int Primitive::getIndex (int index) const
	{
		for (int i = 0; i < mInputs.size(); i++)
		{
			if (mInputs[i]->getSemantic() == INPUT_SEMANTIC_VERTEX)
				return mIndices->get (index, mInputs[i]->getOffest());
		}
	}
	
	
	
	
	/* parse triangle primitive */
	void Primitive::parse (ticpp::Element *element)
	{
		/* get properties */
		mName     = element->GetAttributeOrDefault ("name", "");
		mMaterial = element->GetAttributeOrDefault ("material", "");
		
		
		/* index stride */
		int stride = 1;
		
		/* get index count */
		int count;
		element->GetAttribute ("count", &count);
		
		mIndices->reserve (count);
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through triangle elements */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* get element name */
			std::string el = iter->Value();
			
			
			/* found an input */
			if (el == "input")
			{
				/* create input */
				Input *input = new Input (iter.Get(), *mSources);
				
				
				/* use shared indicies */
				input->setIndices (mIndices);
				
				/* add to input list */
				mInputs.push_back (input);
				
				/* if its a new offset then increment stride */
				if (input->getOffest() >= stride) stride++;
			}
			
			
			/* triangle indices */
			else if (el == "p")
			{
				/* get index data */
				std::string data = iter->GetText();
				std::istringstream stream (data);
				
				
				/* total indicies. triangles have a unit size of three */
				unsigned int total = (count * stride) * 3;
				
				
				/* convert and add to array */
				for (int i = 0; i < total; i++)
				{
					unsigned int index;
					stream >> index;
					
					mIndices->add (index);
				}
				
			} /* end index */
		} /* end triangle */
		
		
		/* set indices stride */
		mIndices->setStride (stride);
	}
	
	
	
	
	

	/* constructor */
	Geometry::Geometry (ticpp::Element *element)
	{
		parse (element);
	}
	
	
	
	/* destructor */
	Geometry::~Geometry ()
	{
		SourceMap::iterator iter;
		
		/* free sources and VERTEX input */
		for (iter = mSources.begin(); iter != mSources.end(); iter++)
			delete iter->second;
	}
	
	
	
	/* parse geometry element */
	void Geometry::parse (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* geometry mesh */
		ticpp::Element* mesh = 0;
		
		
		/* geometry properties */
		mID   = element->GetAttribute ("id");
		mName = element->GetAttributeOrDefault ("name", "");
		
		
		
		/* sift through geometry elements
		 * asset[0-1], mesh[1], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			/* found mesh */
			if (iter->Value() == "mesh")
				mesh = iter.Get();
		}
		
		
		/* no mesh found */
		if (!mesh)
		{
			std::string error = "Parsing failed: No mesh specified in geometry '" + mID + " (" + mName + ")'";
			throw std::runtime_error (error.c_str());
		}
		
		
		
		/* sift through mesh elements
		 * source[1-*], vertices[1], primitives[*], extra[*] */
		for (iter = iter.begin (mesh); iter != iter.end(); iter++)
		{
			/* get element name */
			std::string el = iter->Value();
			
			
			/* found data source */
			if (el == "source")
			{
				Source *source = new Source (iter.Get());
				
				/* add source to map */
				std::string id = "#" + source->getID();
				mSources[id] = source;
			}
			
			
			/* found vertices input */
			else if (el == "vertices")
			{
				/* vertices properties */
				std::string id = "#" + iter->GetAttribute ("id");
				
				/* create input and add to sources since the vertex
				 * input can be daisy chained as it often is with VERTEX */
				mSources[id] = new Input (iter->FirstChildElement(), mSources);
			}
			
			
			/* found triangle primitive */
			else if (el == "triangles")
				mPrimitives.push_back (new Primitive (iter.Get(), &mSources));
			
		} /* end mesh */
	}

}
