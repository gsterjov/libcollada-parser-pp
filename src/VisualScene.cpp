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

#include "VisualScene.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	VisualScene::VisualScene (ticpp::Element *element)
	{
		parse (element);
	}
	
	
	
	/* destructor */
	VisualScene::~VisualScene ()
	{
		/* free nodes */
		NodeList::iterator iter;
		
		for (iter = mNodes.begin(); iter != mNodes.end(); ++iter)
			delete *iter;
	}
	
	
	
	
	/* parse visual_scene element */
	void VisualScene::parse (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* scene properties */
		mID   = element->GetAttributeOrDefault ("id", "");
		mName = element->GetAttributeOrDefault ("name", "");
		
		
		/* sift through visual_scene elements
		 * asset[0-1], node[1-*], evaluate_scene[*], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			/* found node */
			if (iter->Value() == "node")
			{
				ticpp::Element* child = iter.Get();
				mNodes.push_back (new Node (child));
			}
		}
		
		
		/* no node found */
		if (mNodes.size() == 0)
			throw std::runtime_error ("Parsing failed: No node specified in "
					"visual scene '" + mID + " (" + mName + ")'");
	}

}
