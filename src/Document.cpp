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

#include "Document.h"

#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Document::Document (const std::string &file) : mFile(file)
	{
	}
	
	
	/* destructor */
	Document::~Document ()
	{
	}
	
	
	
	
	/* read effect element */
	void Document::parseMaterials (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through material library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found effect */
			if (iter->Value() == "material")
				mMaterials.push_back (new Material (iter.Get()));
		}
	}
	
	
	
	
	/* read effect element */
	void Document::parseEffects (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through effect library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found effect */
			if (iter->Value() == "effect")
				mEffects.push_back (new Effect (iter.Get()));
		}
	}
	
	
	
	
	/* read geometry element */
	void Document::parseGeometries (ticpp::Element *element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through geometry library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found geometry */
			if (iter->Value() == "geometry")
				mGeometries.push_back (new Geometry (iter.Get()));
		}
		
	}
	
	
	
	
	/* read visual_scenes element */
	void Document::parseVisualScenes (ticpp::Element *element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through visual scene library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found visual scene */
			if (iter->Value() == "visual_scene")
				mVisualScenes.push_back (new VisualScene (iter.Get()));
		}
		
	}
	
	
	
	
	/* open document stream */
	bool Document::open ()
	{
		ticpp::Document doc (mFile);
		doc.LoadFile ();
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through all collada elements */
		for (iter = iter.begin(doc.FirstChildElement()); iter != iter.end(); iter++)
		{
			std::string name = iter->Value();
			
			/* found material library */
			if (name == "library_materials")
				parseMaterials (iter.Get());
			
			/* found effect library */
			else if (name == "library_effects")
				parseEffects (iter.Get());
			
			/* found geometry library */
			else if (name == "library_geometries")
				parseGeometries (iter.Get());
			
			/* found visual scene library */
			else if (name == "library_visual_scenes")
				parseVisualScenes (iter.Get());
			
		}
		
		
		return true;
	}

}
