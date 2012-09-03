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

#include "Reader.h"

#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	Reader::Reader (const std::string &file, ReaderHandler *handler)
	{
		mFile = file;
		mHandler = handler;
	}
	
	
	/* destructor */
	Reader::~Reader ()
	{
		
	}
	
	
	
	
	/* read effect element */
	void Reader::parseEffects (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through effect library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found effect */
			if (iter->Value() == "effect")
			{
				Effect* effect = new Effect (iter.Get());
				mHandler->loadEffect (effect);
				delete effect;
			}
		}
	}
	
	
	
	
	/* read geometry element */
	void Reader::parseGeometries (ticpp::Element *element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through geometry library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found geometry */
			if (iter->Value() == "geometry")
			{
				Geometry* geometry = new Geometry (iter.Get());
				mHandler->loadGeometry (geometry);
				delete geometry;
			}
		}
		
	}
	
	
	
	
	/* read visual_scenes element */
	void Reader::parseVisualScenes (ticpp::Element *element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through visual scenes library */
		for (iter = iter.begin(element); iter != iter.end(); iter++)
		{
			/* found visual scene */
			if (iter->Value() == "visual_scene")
			{
				VisualScene* scene = new VisualScene (iter.Get());
				mHandler->loadVisualScene (scene);
				delete scene;
			}
		}
		
	}
	
	
	
	
	/* open reader stream */
	bool Reader::open ()
	{
		ticpp::Document doc (mFile);
		doc.LoadFile ();
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through all collada elements */
		for (iter = iter.begin(doc.FirstChildElement()); iter != iter.end(); iter++)
		{
			std::string name = iter->Value();
			
			/* found effect library */
			if (name == "library_effects")
				parseEffects (iter.Get());
			
			/* found geometry library */
			else if (name == "library_geometries")
				parseGeometries (iter.Get());
			
			/* found visual scenes library */
			else if (name == "library_visual_scenes")
				parseVisualScenes (iter.Get());
			
		}
		
		
		return true;
	}
	
	
	
	
	/* does a quick parse to get basic document information */
	DocumentInfo Reader::getInfo ()
	{
		DocumentInfo info;
		
		
		ticpp::Document doc (mFile);
		doc.LoadFile ();
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through all collada elements */
		for (iter = iter.begin(doc.FirstChildElement()); iter != iter.end(); iter++)
		{
			std::string name = iter->Value();
			
			/* found geometry library */
			if (name == "library_geometries")
			{
				ticpp::Iterator<ticpp::Element> it;
				
				/* loop through all geometries */
				for (it = it.begin (iter.Get()); it != it.end(); it++)
				{
					DocumentInfo::GeometryInfo geom;
					geom.id = it->GetAttribute ("id");
					geom.name = it->GetAttributeOrDefault ("name", "");
					
					info.geometries.push_back (geom);
				} /* end geometries */
			}
			
		} /* end document */
		
		
		return info;
	}

}
