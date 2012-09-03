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

#include "Node.h"

#include <stdexcept>
#include <ticpp/ticpp.h>

#include "Transform.h"
#include <iostream>

namespace ColladaParser
{

	/* constructor */
	Node::Node (ticpp::Element *element)
	{
		parse (element);
	}
	
	
	
	/* destructor */
	Node::~Node ()
	{
		/* free transforms */
		TransformList::iterator iter;
		
		for (iter = mTransforms.begin(); iter != mTransforms.end(); ++iter)
			delete *iter;
		
		
		/* free children */
		NodeList::iterator it;
		
		for (it = mChildren.begin(); it != mChildren.end(); ++it)
			delete *it;
	}
	
	
	
	
	/* parse node element */
	void Node::parse (ticpp::Element* element)
	{
		/* scene properties */
		mID   = element->GetAttributeOrDefault ("id", "");
		mName = element->GetAttributeOrDefault ("name", "");
		mSID  = element->GetAttributeOrDefault ("sid", "");
		
		std::string type   = element->GetAttributeOrDefault ("type", "NODE");
		std::string layers = element->GetAttributeOrDefault ("layer", "");
		
		/* determine node type */
		if (type == "JOINT") mType = JOINT;
		else mType = NODE;
		
		
		/* parse layers */
		std::istringstream layer_stream (layers);
		
		while (true)
		{
			std::string layer;
			layer_stream >> layer;
			
			if (layer.size() <= 0)
				break;
			
			mLayers.push_back (layer);
		}
		
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		/* sift through node elements
		 * asset[0-1],
		 * transformation_elements[*],
		 * instance_camera[*],
		 * instance_controller[*],
		 * instance_geometry[*],
		 * instance_light[*],
		 * instance_node[*],
		 * node[*],
		 * extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			/* child element */
			ticpp::Element* child = iter.Get();
			std::string child_name = child->Value();
			
			std::string sid = child->GetAttributeOrDefault ("sid", "");
			
			
			/* found rotation */
			if (child_name == "rotate")
			{
				Transform* trans = new Transform (Transform::ROTATE, child);
				mTransforms.push_back (trans);
			}
			
			
			/* found scale */
			else if (child_name == "scale")
			{
				Transform* trans = new Transform (Transform::SCALE, child);
				mTransforms.push_back (trans);
			}
			
			
			/* found translation */
			else if (child_name == "translate")
			{
				Transform* trans = new Transform (Transform::TRANSLATE, child);
				mTransforms.push_back (trans);
			}
			
			
			/* found geometry instance */
			else if (child_name == "instance_geometry")
			{
				mGeometries.push_back (parseGeometry (child));
			}
			
			
			/* found child node */
			else if (child_name == "node")
			{
				mChildren.push_back (new Node (child));
			}
		}
		
	}
	
	
	
	
	/* parse instnace_geometry element */
	GeometryInstance* Node::parseGeometry (ticpp::Element* element)
	{
		GeometryInstance* instance = new GeometryInstance ();
		
		
		/* scene properties */
		instance->sid  = element->GetAttributeOrDefault ("sid", "");
		instance->name = element->GetAttributeOrDefault ("name", "");
		instance->url  = element->GetAttribute ("url");
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		/* sift through instance_geometry elements
		 * bind_material[0-1], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); ++iter)
		{
			/* found material binding */
			if (iter->Value() == "bind_material")
			{
				instance->materials = parseMaterial (iter.Get());
			}
		}
		
		return instance;
	}
	
	
	
	
	/* parse bind_material element */
	MaterialBinding* Node::parseMaterial (ticpp::Element* element)
	{
		MaterialBinding* binding = new MaterialBinding ();
		
		
		ticpp::Iterator<ticpp::Element> iter;
		
		/* sift through bind_material elements
		 * param[*], technique_common[1], technique[*], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); ++iter)
		{
			/* found material binding */
			if (iter->Value() == "technique_common")
			{
				ticpp::Iterator<ticpp::Element> it;
				
				/* sift through technique_common elements
				 * instance_material[1-*] */
				for (it = it.begin(iter.Get()); it != it.end(); ++it)
				{
					if (it->Value() == "instance_material")
					{
						std::string symbol = it->GetAttribute ("symbol");
						std::string target = it->GetAttribute ("target");
						
						binding->materials[symbol] = target;
					}
				}
				
			} /* technique_common */
		} /* bind_material */
		
		return binding;
	}

}
