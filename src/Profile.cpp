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

#include "Profile.h"

#include <stdexcept>
#include <ticpp/ticpp.h>


namespace ColladaParser
{

	/* constructor */
	ProfileCommon::ProfileCommon (ticpp::Element* element)
	{
		parse (element);
	}
	
	
	/* destructor */
	ProfileCommon::~ProfileCommon ()
	{
		if (mTechnique.constant) delete mTechnique.constant;
		if (mTechnique.lambert)  delete mTechnique.lambert;
		if (mTechnique.phong)    delete mTechnique.phong;
		if (mTechnique.blinn)    delete mTechnique.blinn;
	}
	
	
	
	
	
	/* parse colour element */
	Colour parseColour (ticpp::Element* element)
	{
		Colour colour;
		ticpp::Element* child = element->FirstChildElement ();
		
		/* found colour */
		if (child->Value() == "color")
		{
			std::istringstream stream (child->GetText());
			
			stream >> colour.r;
			stream >> colour.g;
			stream >> colour.b;
			stream >> colour.a;
		}
		
		return colour;
	}
	
	
	/* parse float element */
	float parseFloat (ticpp::Element* element)
	{
		float val;
		ticpp::Element* child = element->FirstChildElement ();
		
		if (child->Value() == "float")
		{
			std::istringstream stream (child->GetText());
			stream >> val;
		}
		
		return val;
	}
	
	
	
	
	/* parse common profile */
	void ProfileCommon::parse (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* profile properties */
		mID = element->GetAttributeOrDefault ("id", "");
		
		
		/* profile technique element */
		ticpp::Element* technique = 0;
		
		
		/* sift through common profile elements
		 * asset[0-1], newparam[*], technique[1], extra[*] */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			/* found technique */
			if (iter->Value() == "technique")
				technique = iter.Get();
		}
		
		
		/* no technique found */
		if (!technique)
			throw std::runtime_error ("Parsing failed: No technique specified "
					"in the effect profile '" + mID + "'");
		
		
		/* technique properties */
		mTechnique.id  = technique->GetAttributeOrDefault ("id", "");
		mTechnique.sid = technique->GetAttribute ("sid");
		
		
		/* sift through technique elements
		 * asset[0-1], shader_element[*], extra[*] */
		for (iter = iter.begin (technique); iter != iter.end(); iter++)
		{
			std::string name = iter->Value();
			
			/* determine shader element type */
			if      (name == "blinn")    parseBlinn    (iter.Get());
			else if (name == "constant") parseConstant (iter.Get());
			else if (name == "lambert")  parseLambert  (iter.Get());
			else if (name == "phong")    parsePhong    (iter.Get());
		}
	}
	
	
	
	
	
	/* parse common shader element properties */
	void ProfileCommon::parseShaderCommon (ticpp::Element* element, ShaderCommon* shader)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* sift through shader properties */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			std::string val = iter->Value();
			ticpp::Element* child = iter.Get();
			
			if      (val == "emission")     shader->emission     = parseColour (child);
			else if (val == "reflective")   shader->reflective   = parseColour (child);
			else if (val == "transparent")  shader->transparent  = parseColour (child);
			else if (val == "reflectivity") shader->reflectivity = parseFloat  (child);
			else if (val == "transparency") shader->transparency = parseFloat  (child);
			
			else if (val == "index_of_refraction") shader->indexOfRefraction = parseFloat (child);
		}
	}
	
	
	
	
	/* parse blinn shader */
	void ProfileCommon::parseBlinn (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* parse common properties */
		Blinn* shader = new Blinn ();
		parseShaderCommon (element, shader);
		
		
		/* parse blinn specific properties */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			std::string val = iter->Value();
			ticpp::Element* child = iter.Get();
			
			if      (val == "ambient")   shader->ambient   = parseColour (child);
			else if (val == "diffuse")   shader->diffuse   = parseColour (child);
			else if (val == "specular")  shader->specular  = parseColour (child);
			else if (val == "shininess") shader->shininess = parseFloat  (child);
		}
		
		mTechnique.blinn = shader;
	}
	
	
	/* parse constant shader */
	void ProfileCommon::parseConstant (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* parse common properties */
		Constant* shader = new Constant ();
		parseShaderCommon (element, shader);
		
		mTechnique.constant = shader;
	}
	
	
	/* parse lambert shader */
	void ProfileCommon::parseLambert (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* parse common properties */
		Lambert* shader = new Lambert ();
		parseShaderCommon (element, shader);
		
		
		/* parse lambert specific properties */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			std::string val = iter->Value();
			ticpp::Element* child = iter.Get();
			
			if      (val == "ambient") shader->ambient = parseColour (child);
			else if (val == "diffuse") shader->diffuse = parseColour (child);
		}
		
		mTechnique.lambert = shader;
	}
	
	
	/* parse phong shader */
	void ProfileCommon::parsePhong (ticpp::Element* element)
	{
		ticpp::Iterator<ticpp::Element> iter;
		
		
		/* parse common properties */
		Phong* shader = new Phong ();
		parseShaderCommon (element, shader);
		
		
		/* parse phong specific properties */
		for (iter = iter.begin (element); iter != iter.end(); iter++)
		{
			std::string val = iter->Value();
			ticpp::Element* child = iter.Get();
			
			if      (val == "ambient")   shader->ambient   = parseColour (child);
			else if (val == "diffuse")   shader->diffuse   = parseColour (child);
			else if (val == "specular")  shader->specular  = parseColour (child);
			else if (val == "shininess") shader->shininess = parseFloat  (child);
		}
		
		mTechnique.phong = shader;
	}

}
