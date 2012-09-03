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

#ifndef COLLADA_PARSER_NODE_H_
#define COLLADA_PARSER_NODE_H_


#include <vector>
#include <map>
#include <string>

#include <ColladaParser/Config.h>
#include <ColladaParser/Types.h>


/* forward declarations */
namespace ticpp { class Element; }


namespace ColladaParser
{

	/* forward declarations */
	class Node; class Transform; struct GeometryInstance;
	
	
	/**
	 * A list of nodes.
	 */
	typedef std::vector<Node*> NodeList;
	
	/**
	 * A list of transforms.
	 */
	typedef std::vector<Transform*> TransformList;
	
	/**
	 * A list of geometry instances.
	 */
	typedef std::vector<GeometryInstance*> GeometryInstanceList;
	
	/**
	 * A list of strings.
	 */
	typedef std::vector<std::string> StringList;
	
	/**
	 * A map of strings.
	 */
	typedef std::map<std::string, std::string> StringMap;
	
	
	
	/**
	 * Material instance binding.
	 */
	struct MaterialBinding
	{
		StringMap materials;
	};
	
	
	
	/**
	 * Geometry instance.
	 */
	struct GeometryInstance
	{
		std::string sid;
		std::string name;
		std::string url;
		
		MaterialBinding* materials;
		
		GeometryInstance () : materials(0) {}
	};
	
	
	
	/**
	 * Node.
	 * 
	 * Nodes determine a tree hierarchy by adding child nodes to its list
	 * and defining any transformations on the associated instance and its
	 * children.
	 */
	class COLLADA_PARSER_API Node
	{
	public:
		/**
		 * Node type.
		 */
		enum Type { JOINT, NODE };
		
		
		/**
		 * Constructor.
		 * Reads the 'node' element from the given XML node.
		 * @param element The XML node to parse.
		 */
		Node (ticpp::Element* element);
		
		/**
		 * Destructor.
		 */
		~Node ();
		
		
		/**
		 * The node ID.
		 */
		std::string getID() const { return mID; }
		
		/**
		 * The node name.
		 */
		std::string getName() const { return mName; }
		
		/**
		 * The node SID.
		 */
		std::string getSID() const { return mSID; }
		
		/**
		 * The node type.
		 */
		Type getType() const { return mType; }
		
		/**
		 * The node layer.
		 */
		const StringList& getLayers() const { return mLayers; }
		
		
		
		/**
		 * A list of transformations to be applied on the node in order.
		 */
		const TransformList& getTransforms() const { return mTransforms; }
		
		
		/**
		 * A list of URLs to geometries that should be instanced.
		 */
		const GeometryInstanceList& getGeometries() const { return mGeometries; }
		
		
		/**
		 * A list of child nodes.
		 */
		const NodeList& getChildren() const { return mChildren; }
		
		
		
	private:
		/* properties */
		std::string mID;
		std::string mName;
		std::string mSID;
		
		Type mType;
		StringList mLayers;
		
		/* instances */
		GeometryInstanceList mGeometries;
		
		NodeList mChildren;
		TransformList mTransforms;
		
		
		/* parsing methods */
		void parse (ticpp::Element* element);
		GeometryInstance* parseGeometry (ticpp::Element* element);
		MaterialBinding* parseMaterial (ticpp::Element* element);
	};

}


#endif /* COLLADA_PARSER_NODE_H_ */
