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

#ifndef COLLADA_PARSER_DATASOURCE_H_
#define COLLADA_PARSER_DATASOURCE_H_


#include <vector>
#include <map>
#include <string>

#include <ColladaParser/Config.h>


namespace ColladaParser
{

	/**
	 * Possible data types within a Collada data field.
	 */
	enum COLLADA_PARSER_API DataType
	{
		DATA_TYPE_UNKNOWN,
		
		DATA_TYPE_INTEGER,
		DATA_TYPE_FLOAT,
		DATA_TYPE_NAME,
		DATA_TYPE_BOOL,
		DATA_TYPE_IDREF,
		DATA_TYPE_SIDREF
	};
	
	
	
	/**
	 * A DataSource is the interface to all data elements which need to
	 * access a particular kind of data from a Collada document.
	 */
	class COLLADA_PARSER_API DataSource
	{
	public:
		/**
		 * The Accessor provides a means for the DataSource to access
		 * the formatted data within its field by defining its type and
		 * entry points into the raw data.
		 */
		struct COLLADA_PARSER_API Accessor
		{
			/**
			 * A Param defines the type of data held within the formatted
			 * segment of data.
			 */
			struct COLLADA_PARSER_API Param
			{
				bool skip;
				DataType type;
			};
			
			
			unsigned int count;
			unsigned int offset;
			unsigned int stride;
			
			std::vector<Param> params;
		};
		
		
		/**
		 * The amount of entries within the DataSource.
		 */
		virtual int getCount() = 0;
		
		/**
		 * Get the data from the specified index.
		 */
		virtual float getData (int index) = 0;
		
		/**
		 * Get the data Accessor which defines how the DataSource should
		 * be accessed and how it is formatted.
		 */
		virtual const Accessor &getAccessor() = 0;
		
		
		/**
		 * Get the total size of the data in the DataSource.
		 */
		int getSize() { return getCount(); }
	};
	
	
	
	/**
	 * A map of DataSources identified by its name.
	 */
	typedef std::map<std::string, DataSource*> SourceMap;

}


#endif /* COLLADA_PARSER_DATASOURCE_H_ */
